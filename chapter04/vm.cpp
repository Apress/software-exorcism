/* vm.cpp ---------------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>

//data types

#define U1		unsigned char
#define S4		signed long
#define U4		unsigned long

#define BOOLEAN	int
#define TRUE	1==1
#define FALSE	!TRUE

//virtual machine environment

#define R0			0	//indices into Context.R[]
#define R1			1
#define R2			2
#define R3			3
#define R4			4

#define nREGISTERS	5
#define isRegister(reg)	(((reg>-1)&&(reg<5))?TRUE:FALSE)

struct Context
{
	U1 *memory;			//program segment
	U4 IP;				//instruction pointer
	S4 R[nREGISTERS];	//general-purpose registers
	BOOLEAN singleStep;	//toggles single-stepping
};

//instruction set

#define PUT		0	//PUT   constant,R*
#define STORE	1	//STORE R*,address
#define LOAD	2	//LOAD  address,R*
#define MOV		3	//MOV	R*,R*
#define ADD		4	//ADD	R*,R*,R*
#define SUB		5	//SUB	R*,R*,R*
#define STOP	6	//STOP
#define PRINT	7	//PRINT R*
#define BREAK	8	//BREAK

//executable meta-data

struct FileInfo
{
	U4 nbytes;
	FILE *filePointer;
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//handles the command line

#define MAX_ARGUMENTS	2

class CommandLine
{
	private:
	int argc;
	char *argv[MAX_ARGUMENTS];
	struct FileInfo *fileInfo;

	public:
	CommandLine(int argc, char* argv[],struct FileInfo *fileInfo);
	BOOLEAN processCommandLine();
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define NUMBER_ARGS		2
#define FILE_ARGUMENT	1

CommandLine::CommandLine
(
	int argc, 
	char* argv[],
	struct FileInfo *fileInfo
)
{
	int i;
	(*this).argc = argc;
	(*this).fileInfo = fileInfo;
	for(i=0;i<MAX_ARGUMENTS;i++)
	{
		(*this).argv[i]=argv[i];
	}
}/*end constructor-------------------------------------------------*/

BOOLEAN CommandLine::processCommandLine()
{
	if(argc!=NUMBER_ARGS)
	{ 
		printf("usage: vm program.run\n");
		return(FALSE); 
	}

	(*fileInfo).filePointer = fopen(argv[FILE_ARGUMENT],"rb");
	if((*fileInfo).filePointer==NULL)
	{ 
		printf("could not open %s\n",argv[FILE_ARGUMENT]);
		return(FALSE); 
	}
	
	(*fileInfo).nbytes=0;
	while(fgetc((*fileInfo).filePointer)!=EOF)
	{
		(*fileInfo).nbytes++;
	}
	
	if((*fileInfo).nbytes==0)
	{
		printf("file zero bytes in size\n");
		return(FALSE);
	}

	rewind((*fileInfo).filePointer);
	return(TRUE);
}/*end processCommandLine------------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//provides debugging services

class Debugger
{
	private:
	struct Context *context;

	BOOLEAN getCommand();
	void displayContext();
	void dumpMemory();
	void displayByte(U4 index,U4 address,char byte);
	BOOLEAN isEndOfDisplayGroup(U4 index);
	BOOLEAN isEndOfDisplayLine(U4 index);

	public:
	Debugger(struct Context *context);
	void debugLoop();
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

Debugger::Debugger(struct Context *context)
{
	(*this).context = context;
}/*end constructor-------------------------------------------------*/

void Debugger::debugLoop()
{
	BOOLEAN ok;
	ok = getCommand();
	while(ok)
	{
		ok = getCommand();
	}
}/*end debugLoop---------------------------------------------------*/

BOOLEAN Debugger::getCommand()
{
	char ch;
	printf("-");
	scanf("%c",&ch);
	fflush(stdin);

	switch(ch)
	{
		case 'r':
		{
			displayContext();
		}break;
		case 'd':
		{
			dumpMemory();
		}break;
		case 't':
		{ 
			printf("single-step on\n");
			(*context).singleStep=TRUE; 
		}break;
		case 'T':
		{ 
			printf("single-step off\n");
			(*context).singleStep=FALSE; 
		}break;
		case 'q':
		{ 
			return(FALSE); 
		}break;
		default:
		{
			printf("bad command\n");
		}break;
	}
	return(TRUE);
}/*end getCommand--------------------------------------------------*/

void Debugger::displayContext()
{
	printf("IP=%08lX\n",(*context).IP);
	printf("R0=%08lX\n",(*context).R[R0]);
	printf("R1=%08lX\n",(*context).R[R1]);
	printf("R2=%08lX\n",(*context).R[R2]);
	printf("R3=%08lX\n",(*context).R[R3]);
	printf("R4=%08lX\n",(*context).R[R4]);
	if((*context).singleStep){ printf("single-stepping on\n"); }
	else{ printf("single-stepping off\n");}
}/*end displayContext----------------------------------------------*/

void Debugger::dumpMemory()
{
	U4 address;
	U4 nbytes;
	U4 i;

	printf("[address]:");
	scanf("%lX",&address);
	fflush(stdin);

	printf("nbytes:");
	scanf("%lu",&nbytes);
	fflush(stdin);

	for(i=0;i<nbytes;i++)
	{
		displayByte(i,address+i,(*context).memory[address+i]);	
	}
	printf("\n");
}/*end dumpMemory--------------------------------------------------*/

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

//executes the bytcode

class VirtualMachine
{
	private:
	struct Context *context;

	void handlePut();
	void badRegister(U1 reg);
	void printRegister(U1 reg);
	void handleLoad();
	void handleStore();
	void handleMov();
	void handleAdd();
	void handleSub();
	void handlePrint();
	void handleBreakPoint();
	void handleSingleStep();

	public:
	BOOLEAN initContext(struct Context *,struct FileInfo *);
	void execute();
	void cleanup();
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

BOOLEAN VirtualMachine::initContext
(
	struct Context *context,
	struct FileInfo *fileInfo
)
{
	U4 i;

	//this function is the firewall for the rest of the application

	(*context).memory = (U1*)malloc((*fileInfo).nbytes);
	if((*context).memory==NULL){ return(FALSE);}

	for(i=0;i<(*fileInfo).nbytes;i++)
	{
		(*context).memory[i]=fgetc((*fileInfo).filePointer);
	}
	fclose((*fileInfo).filePointer);

	(*context).IP=0;
	(*context).R[R0]=0;
	(*context).R[R1]=0;
	(*context).R[R2]=0;
	(*context).R[R3]=0;
	(*context).R[R4]=0;
	(*context).singleStep=FALSE;
	
	(*this).context = context;
	return(TRUE);
}/*end initContext-------------------------------------------------*/

void VirtualMachine::execute()
{
	U1 currentOpcode;
	currentOpcode = (*context).memory[(*context).IP];

	while(currentOpcode!=STOP)
	{
		switch(currentOpcode)
		{
			case PUT:
			{
				handlePut();
			}break;
			case LOAD:
			{
				handleLoad();
			}break;
			case STORE:
			{
				handleStore();
			}break;
			case MOV:
			{
				handleMov();
			}break;
			case ADD:
			{
				handleAdd();
			}break;
			case SUB:
			{
				handleSub();
			}break;
			case PRINT:
			{
				handlePrint();
			}break;
			case BREAK:
			{
				handleBreakPoint();
			}break;
		}

		currentOpcode = (*context).memory[(*context).IP];

		if((*context).singleStep)
		{ 
			handleSingleStep(); 
		}
	}
	if((*context).singleStep){ printf("STOP\n"); }
	return;
}/*end execute-----------------------------------------------------*/

//simplify notation without sacrificing clarity

#define MEMORY			(*context).memory
#define CURRENT_IP		(*context).IP

#define getInteger(addr)	*((S4*)(&(MEMORY[addr])))
#define getAddress(addr)	*((U4*)(&(MEMORY[addr])))
#define getRegister(addr)	MEMORY[addr]

void VirtualMachine::handlePut()
{
	U4 constant;
	U1 reg;

	//PUT constant,R*

	constant = getInteger(CURRENT_IP+1);
	reg = getRegister(CURRENT_IP+5);

	if(!isRegister(reg)){ badRegister(reg); return; }

	(*context).R[reg]=constant;
	CURRENT_IP+=6;

	if((*context).singleStep)
	{
		printf("PUT ");
		printf("%08lX,",constant);
		printRegister(reg);
		printf("\n");
	}
	return;
}/*end handlePUT---------------------------------------------------*/

void VirtualMachine::badRegister(U1 reg)
{
	printf("bad register R[%u]\n",reg);
}/*end badRegister-------------------------------------------------*/

void VirtualMachine::printRegister(U1 reg)
{
	switch(reg)
	{
		case R0:{ printf("R0"); }break;
		case R1:{ printf("R1"); }break;
		case R2:{ printf("R2"); }break;
		case R3:{ printf("R3"); }break;
		case R4:{ printf("R4"); }break;
		default:{ printf("R?"); }break;
	}
	return;
}/*end printRegister-----------------------------------------------*/

void VirtualMachine::handleStore()
{
	U1 reg;
	U4 address;

	//STORE R*,memory

	reg = getRegister(CURRENT_IP+1);
	address = getAddress(CURRENT_IP+2);

	if(!isRegister(reg)){ badRegister(reg); return; }

	*((S4*)(&MEMORY[address])) = (*context).R[reg];
	CURRENT_IP+=6;

	if((*context).singleStep)
	{
		printf("STORE ");
		printRegister(reg);
		printf(",%08lX",address);
		printf("\n");
	}
	return;
}/*end handleStore-------------------------------------------------*/

void VirtualMachine::handleLoad()
{
	U4 address;
	U1 reg;

	//LOAD memory,R*

	address = getAddress(CURRENT_IP+1);
	reg = getRegister(CURRENT_IP+5);

	if(!isRegister(reg)){ badRegister(reg); return; }

	(*context).R[reg] = *((S4*)(&MEMORY[address]));
	CURRENT_IP+=6;

	if((*context).singleStep)
	{
		printf("LOAD ");
		printf("%08lX,",address);
		printRegister(reg);
		printf("\n");
	}
	return;
}/*end handleLoad--------------------------------------------------*/

void VirtualMachine::handleMov()
{
	U1 reg1;
	U1 reg2;

	//MOV R*,R*

	reg1 = getRegister(CURRENT_IP+1);
	reg2 = getRegister(CURRENT_IP+2);

	if(!isRegister(reg1)){ badRegister(reg1); return; }
	if(!isRegister(reg2)){ badRegister(reg2); return; }

	(*context).R[reg2] = (*context).R[reg1];
	CURRENT_IP+=3;	

	if((*context).singleStep)
	{
		printf("MOV ");
		printRegister(reg1);
		printf(",");
		printRegister(reg2);
		printf("\n");
	}
	return;
}/*end handleMov---------------------------------------------------*/

void VirtualMachine::handleAdd()
{
	U1 reg1;
	U1 reg2;
	U1 reg3;

	//ADD R*,R*,R*

	reg1 = getRegister(CURRENT_IP+1);
	reg2 = getRegister(CURRENT_IP+2);
	reg3 = getRegister(CURRENT_IP+3);

	if(!isRegister(reg1)){ badRegister(reg1); return; }
	if(!isRegister(reg2)){ badRegister(reg2); return; }
	if(!isRegister(reg3)){ badRegister(reg3); return; }

	(*context).R[reg3] = (*context).R[reg2]+(*context).R[reg1];
	CURRENT_IP+=4;	

	if((*context).singleStep)
	{
		printf("ADD ");
		printRegister(reg1);
		printf(",");
		printRegister(reg2);
		printf(",");
		printRegister(reg3);
		printf("\n");
	}
}/*end handleAdd---------------------------------------------------*/

void VirtualMachine::handleSub()
{
	U1 reg1;
	U1 reg2;
	U1 reg3;

	//SUB R*,R*,R*

	reg1 = getRegister(CURRENT_IP+1);
	reg2 = getRegister(CURRENT_IP+2);
	reg3 = getRegister(CURRENT_IP+3);

	if(!isRegister(reg1)){ badRegister(reg1); return; }
	if(!isRegister(reg2)){ badRegister(reg2); return; }
	if(!isRegister(reg3)){ badRegister(reg3); return; }

	(*context).R[reg3] = (*context).R[reg2]-(*context).R[reg1];
	CURRENT_IP+=4;	

	if((*context).singleStep)
	{
		printf("SUB ");
		printRegister(reg1);
		printf(",");
		printRegister(reg2);
		printf(",");
		printRegister(reg3);
		printf("\n");
	}
}/*end handleSub---------------------------------------------------*/

void VirtualMachine::handlePrint()
{
	U1 reg;

	//PRINT R*

	reg = getRegister(CURRENT_IP+1);

	if(!isRegister(reg)){ badRegister(reg); return; }

	printRegister(reg);
	printf("=%08lX\n",(*context).R[reg]);
	CURRENT_IP+=2;
	
	if((*context).singleStep)
	{
		printf("PRINT ");
		printRegister(reg);
		printf("\n");
	}
}/*end handlePrint-------------------------------------------------*/

void VirtualMachine::handleBreakPoint()
{
	(*context).singleStep=TRUE;
	CURRENT_IP+=1;	
	printf("BREAK POINT HIT\n");
}/*end handleBreakPoint--------------------------------------------*/

void VirtualMachine::handleSingleStep()
{	
	Debugger debugger(context);
	debugger.debugLoop();
}/*end handleSingleStep--------------------------------------------*/

void VirtualMachine::cleanup()
{
	free((*context).memory);
	return;
}/*end cleanup-----------------------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Driver                                                            +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//interpreter entry point

void main(int argc, char* argv[])
{
	struct Context context;
	struct FileInfo fileInfo;
	BOOLEAN ok;

	CommandLine commandLine(argc,argv,&fileInfo);
	ok = commandLine.processCommandLine();
	if(!ok){ return; }

	VirtualMachine vm;
	ok = vm.initContext(&context,&fileInfo);
	if(!ok){ return; }

	vm.execute();
	vm.cleanup();
	return;
}