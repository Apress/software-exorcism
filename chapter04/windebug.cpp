/* windebug.c -----------------------------------------------------*/

#include<stdio.h>
#include<windows.h>

#define RETURN_OK		0
#define RETURN_ERROR	1

//data types (addresses in Windows 2000 are 32-bits)

#define BOOLEAN	int
#define U4		unsigned long

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//this class handles the command line

#define MAX_ARGUMENTS	3

class CommandLine
{
	private:
	int argc;
	char *argv[MAX_ARGUMENTS];

	BOOLEAN fileExists(char *fname);
	void printHelp();

	public:
	CommandLine(int argc, char *argv[]);
	BOOLEAN validArguments();
	char* getFileName();
	char* getArgument();
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

CommandLine::CommandLine(int argc, char *argv[])
{
	int i;
	(*this).argc = argc;
	for(i=0;i<MAX_ARGUMENTS;i++)
	{
		(*this).argv[i]=NULL;
		if(argv[i]!=NULL)
		{
			(*this).argv[i]=argv[i];
		}
	}
	return;
}/*end constructor-------------------------------------------------*/

#define FILE_INDEX		1
#define NO_ARGUMENT		2
#define HAS_ARGUMENT	3

BOOLEAN CommandLine::validArguments()
{
	switch(argc)
	{
		case NO_ARGUMENT:
		case HAS_ARGUMENT:
		{
			if(!fileExists(argv[FILE_INDEX]))
			{ 
				printf("%s does not exist\n",argv[FILE_INDEX]);
				return(FALSE); 
			}	
		}break;
		default:
		{
			printHelp();
			return(FALSE);
		}
	}
	return(TRUE);
}/*end validArguments----------------------------------------------*/

BOOLEAN CommandLine::fileExists(char *fname)
{
	FILE* file;
	file = fopen(fname,"r");
	if(file==NULL)
	{
		return(FALSE);
	}
	fclose(file);
	return(TRUE);
}/*end fileExists--------------------------------------------------*/

void CommandLine::printHelp()
{
	printf("usage:dbg program.exe [argument]\n");
	return;
}/*end printHelp---------------------------------------------------*/

char* CommandLine::getFileName()
{
	return(argv[FILE_INDEX]);
}/*end getFileName-------------------------------------------------*/

char* CommandLine::getArgument()
{
	switch(argc)
	{
		case HAS_ARGUMENT:
		{
			return(argv[FILE_INDEX+1]);
		}break;
	}
	return(NULL);
}/*end getArgument-------------------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//represents the process being debugged

class Debugee
{
	private:
	char *fileName;
	char *argument;
	void getSecurity(SECURITY_ATTRIBUTES *, SECURITY_DESCRIPTOR *);
	
	public:
	Debugee(char *fileName, char *argument);
	PROCESS_INFORMATION getProcessHandle();	
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

Debugee::Debugee(char *fileName, char *argument)
{
	(*this).fileName = fileName;
	(*this).argument = argument;
}/*end constructor-------------------------------------------------*/

PROCESS_INFORMATION Debugee::getProcessHandle()
{
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startUpInfo;
	SECURITY_ATTRIBUTES attributes;
	SECURITY_DESCRIPTOR descriptor;

	BOOLEAN ok;
	getSecurity(&attributes,&descriptor);
	GetStartupInfo(&startUpInfo);

	ok = CreateProcess
	(
		fileName,		//executable
		argument,		//command line
		&attributes,	//process security descriptor
		&attributes,	//thread security descriptor
		FALSE,			//inherits handles of debugger
		DEBUG_ONLY_THIS_PROCESS, 
		NULL,			//use environment of calling process
		NULL,			//current directory of calling process
		&startUpInfo,
		&processInfo	//handles for debugee
	);
	if(!ok)
	{ 
		printf("could not load %s\n",fileName);
		exit(RETURN_ERROR);
	}
	return(processInfo); 
}/*end getProcessHandle--------------------------------------------*/

void Debugee::getSecurity
(
	SECURITY_ATTRIBUTES *attributes,
	SECURITY_DESCRIPTOR *descriptor
)
{
	InitializeSecurityDescriptor
	(
		descriptor,
		SECURITY_DESCRIPTOR_REVISION
	);

	(*attributes).nLength =sizeof(SECURITY_ATTRIBUTES);
	(*attributes).lpSecurityDescriptor=descriptor;
	(*attributes).bInheritHandle =TRUE;
	return;
}/*end getSecurity-------------------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//Command-line debugger

class Debugger
{
	private:
	PROCESS_INFORMATION *processInfo;

	void displayRegisters();
	void displayMemory();
	void displayByte(U4 index,U4 address,char byte);
	BOOLEAN isEndOfDisplayGroup(U4 index);
	BOOLEAN isEndOfDisplayLine(U4 index);

	public:
	Debugger(PROCESS_INFORMATION *processInfo);
	void processDebugCommand();
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

Debugger::Debugger(PROCESS_INFORMATION *processInfo)
{
	(*this).processInfo = processInfo;
}/*end constructor-------------------------------------------------*/

void Debugger::processDebugCommand()
{
	BOOLEAN exitLoop;
	char ch;

	exitLoop=FALSE;
	while(!exitLoop)
	{
		printf("-");
		scanf("%c",&ch);
		fflush(stdin);
		switch(ch)
		{
			case 'r':{ displayRegisters(); }break;
			case 'd':{ displayMemory(); }break;
			case 'q':{ exitLoop = TRUE; }break;
		}
	}
	printf("exiting command loop\n");
	return;
}/*end processDebugCommand-----------------------------------------*/

void Debugger::displayRegisters()
{
	BOOLEAN ok;
	CONTEXT context;
	ok = GetThreadContext
	( 
		(*processInfo).hThread,  
		&context
	); 
	if(!ok)
	{ 
		printf("could not get context\n"); 
		return; 
	}
	printf(" CS=%08lX",context.SegCs);
	printf(" DS=%08lX",context.SegDs);
	printf(" SS=%08lX",context.SegSs);
	printf(" ES=%08lX",context.SegEs);
	printf(" FS=%08lX",context.SegFs);
	printf(" GS=%08lX\n",context.SegGs);

	printf(" EIP=%08lX",context.Eip);
	printf(" ESP=%08lX",context.Esp);
	printf(" EBP=%08lX\n",context.Ebp);

	printf(" EAX=%08lX",context.Eax);
	printf(" EBX=%08lX",context.Ebx);
	printf(" ECX=%08lX",context.Ecx);
	printf(" EDX=%08lX\n",context.Edx);
	printf(" EDI=%08lX",context.Edi);
	printf(" ESI=%08lX\n",context.Esi);
	printf(" EFLAGS=%08lX\n",context.EFlags);
	return;
}/*end displayRegisters--------------------------------------------*/

void Debugger::displayMemory()
{
	BOOLEAN ok;
	char *buffer;
	unsigned long address;
	unsigned long nbytes;
	unsigned long i;

	printf("[base address]:");
	scanf("%lX",&address);
	fflush(stdin);

	printf("[# bytes]:");
	scanf("%lX",&nbytes);
	fflush(stdin);

	buffer = (char*)malloc(nbytes);

	ok = ReadProcessMemory
	( 
		(*processInfo).hProcess, 
		(LPCVOID)address,  
		buffer,  
		nbytes,
		&nbytes
	); 
	if(!ok)
	{ 
		printf("could not read memory\n");
		free(buffer);
		return;
	}

	for(i=0;i<nbytes;i++)
	{
		displayByte(i,address+i,buffer[i]);	
	}
	printf("\n");
	free(buffer);
	return;
}/*end displayMemory-----------------------------------------------*/

void Debugger::displayByte
(
	U4 index,
	U4 address,
	char byte
)
{
	if((index==0)||isEndOfDisplayLine(index))
	{ 
		printf("\n"); 
		printf("[%08X] ",address); 
	}
	else if(isEndOfDisplayGroup(index))
	{ 
		printf("-"); 
	}

	printf(" %02X",(unsigned char)byte);
	return;
}/*end displayByte-------------------------------------------------*/

#define FOUR_BYTES	4
#define LINE_BREAK	16

BOOLEAN Debugger::isEndOfDisplayGroup(U4 index)
{
	if((index>0)&&((index%FOUR_BYTES)==0)){ return(TRUE); }
	return(FALSE);
}/*end isEndOfDisplayGroup-----------------------------------------*/

BOOLEAN Debugger::isEndOfDisplayLine(U4 index)
{
	if((index>0)&&((index%LINE_BREAK)==0)){ return(TRUE); }
	return(FALSE);
}/*end isEndOfDisplayLine------------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//Handles debug events

class DebugEventHandler
{
	private:
	DEBUG_EVENT *event;
	PROCESS_INFORMATION *processInfo;

	void displayError();

	BOOLEAN processDebugEvent();
	void printDebugEvent(DEBUG_EVENT *event);

	BOOLEAN processExceptionEvent();
	void printExceptionEvent(DEBUG_EVENT *event);

	public:
	void startDebugLoop(PROCESS_INFORMATION);
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void DebugEventHandler::startDebugLoop(PROCESS_INFORMATION processInfo)
{
	BOOLEAN ok;

	(*this).processInfo = &processInfo;
	(*this).event = (DEBUG_EVENT*)malloc(sizeof(DEBUG_EVENT));

	ok = WaitForDebugEvent(event,INFINITE);
	if(!ok){ displayError(); }

	while((*event).dwDebugEventCode!=EXIT_PROCESS_DEBUG_EVENT)
	{
		ok = processDebugEvent();
		if(!ok){ break;}

		ok = ContinueDebugEvent
		(
			processInfo.dwProcessId, 
			processInfo.dwThreadId,
			DBG_CONTINUE
		); 
		if(!ok){ displayError(); break; }

		ok = WaitForDebugEvent(event,INFINITE);
		if(!ok){ displayError(); break; }
	}

	if((*event).dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT)
	{
		printf("debugged process has exited, ");
	}
	printf("exiting debugger\n");
	
	free(event);
	return;
}/*end startDebugLoop----------------------------------------------*/

void DebugEventHandler::displayError()
{
	LPVOID lpMsgBuf;
	FormatMessage
	( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	printf("error: %s\n",lpMsgBuf);
	LocalFree(lpMsgBuf);
	return;
}/*end displayError------------------------------------------------*/

BOOLEAN DebugEventHandler::processDebugEvent()
{
	printDebugEvent(event);

	switch((*event).dwDebugEventCode)
	{
		case EXCEPTION_DEBUG_EVENT:
		{
			printExceptionEvent(event);
			return(processExceptionEvent());
		}break;
	}
	return(TRUE);
}/*end processDebugEven--------------------------------------------*/

void DebugEventHandler::printDebugEvent(DEBUG_EVENT *event)
{
	switch((*event).dwDebugEventCode)
	{
		case EXCEPTION_DEBUG_EVENT:
		{ 
			printf("EXCEPTION_DEBUG_EVENT\n"); 
		}break;
		case CREATE_THREAD_DEBUG_EVENT:
		{ 
			printf("CREATE_THREAD_DEBUG_EVENT\n"); 
		}break;
		case CREATE_PROCESS_DEBUG_EVENT:
		{ 
			printf("CREATE_PROCESS_DEBUG_EVENT \n"); 
		}break;
		case EXIT_THREAD_DEBUG_EVENT :
		{ 
			printf("EXIT_THREAD_DEBUG_EVENT \n"); 
		}break;
		case EXIT_PROCESS_DEBUG_EVENT:
		{ 
			printf("EXIT_PROCESS_DEBUG_EVENT\n"); 
		}break;
		case LOAD_DLL_DEBUG_EVENT:
		{ 
			printf("LOAD_DLL_DEBUG_EVENT\n"); 
		}break;
		case UNLOAD_DLL_DEBUG_EVENT:
		{ 
			printf("UNLOAD_DLL_DEBUG_EVENT\n"); 
		}break;
		case OUTPUT_DEBUG_STRING_EVENT:
		{ 
			printf("OUTPUT_DEBUG_STRING_EVENT\n"); 
		}break;
		case RIP_EVENT :
		{ 
			printf("RIP_EVENT \n"); 
		}break;
		default:{ printf("bad event code\n"); }
	} 
	return;
}/*end printDebugEvent---------------------------------------------*/

BOOLEAN DebugEventHandler::processExceptionEvent()
{
	EXCEPTION_DEBUG_INFO debugInfo;
	EXCEPTION_RECORD record;
	Debugger debugger(processInfo);

	debugInfo = ((*event).u).Exception;
	record = (debugInfo).ExceptionRecord;

	switch(record.ExceptionCode) 
	{ 
		case EXCEPTION_BREAKPOINT:
		{
             debugger.processDebugCommand();
		}break;
		default:
		{
			printf("debug exception not handled");
			return(FALSE);
		}break;
    } 
	return(TRUE);
}/*end processExceptionEvent---------------------------------------*/

void DebugEventHandler::printExceptionEvent(DEBUG_EVENT *event)
{
	EXCEPTION_DEBUG_INFO exceptionInfo;
	EXCEPTION_RECORD record;

	exceptionInfo = ((*event).u).Exception;
	record = (exceptionInfo).ExceptionRecord;

	switch(record.ExceptionCode) 
	{ 
		case EXCEPTION_BREAKPOINT:{ printf("EXCEPTION_BREAKPOINT\n"); }break;
		default:{ printf("record code not handled\n"); }
    } 
	return;	
}/*end printExceptionEvent-----------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Driver                                                            +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//program entry point

int main(int argc, char *argv[])
{
	CommandLine commandLine(argc,argv);
	
	if(!commandLine.validArguments())
	{ 
		return(RETURN_ERROR); 
	}

	Debugee debugee
	(
		commandLine.getFileName(),
		commandLine.getArgument()
	);
	
	DebugEventHandler handler;
	handler.startDebugLoop(debugee.getProcessHandle());
	return(RETURN_OK);
}
