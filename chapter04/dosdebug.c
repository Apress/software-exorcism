/* dosdebug.c------------------------------------------------------*/

#include<stdio.h>

//data types (addresses in DOS are 16-bits)

#define U2 unsigned short
#define U1 unsigned char

#define BOOLEAN	U2
#define TRUE	1==1
#define FALSE	!TRUE

//context of process (i.e. all the 8088 registers)

struct Context
{
	U2 rCS;
	U2 rDS;
	U2 rSS;
	U2 rES;

	U2 rIP;
	U2 rSP;
	U2 rBP;
	U2 rSI;
	U2 rDI;
	
	U2 rAX;
	U2 rBX;
	U2 rCX;
	U2 rDX;
	
	U2 flags;
};

//logical IVT vector, segment:offset address of ISR in memory

struct ISRVector
{
	U2 segment;
	U2 address;
};

//debugger's environment (save so can restore later)
//a single new vector replaces both INT 0x1 and INT 0x3 ISRs

struct Environment
{
	struct ISRVector oldBreakPointVector;
	struct ISRVector oldSingleStepVector;
	struct ISRVector newVector;
	U1 breakPointIndex;			//index into the IVT
	U1 singleStepIndex;			//index into the IVT
};

//high-level interface to code (i.e. what see in main())

struct Environment *enableDebug();
void disableDebug(struct Environment *env);

//IVT entry manipulation

void swapISRVectors(struct ISRVector *, struct ISRVector *,U1 index);
void getISR(struct ISRVector *vector,U1 index);
void setISR(struct ISRVector *vector,U1 index);

//interrupt handler and debug services

void handleInterrupts(struct ISRVector *vector, int function);

//debugger implementation

BOOLEAN command(U2 *trace, struct Context *context);
void displayRegisters(struct Context *context);
void dumpMemory();
void help();

//very crude driver

void main()
{
	struct Environment *env;	
	env = enableDebug();
	
	//do stuff (and provide a binary signature)

	_asm{ nop }		//0x90
	_asm{ int 3 }	//0xCC
	_asm{ nop }		//0x90
	_asm{ inc bx}	//0x43

	disableDebug(env);
	return;
}

#define GET_ADDRESS			1
#define BREAK_POINT_ISR		3
#define SINGLE_STEP_ISR		1

struct Environment *enableDebug()
{
	struct Environment *env;
	U2 size;

	size = sizeof(struct Environment);
	env = (struct Environment*)malloc(size);
	
	//special, 1-time, call to get address of new ISR

	handleInterrupts(&((*env).newVector),GET_ADDRESS);
	(*env).breakPointIndex = BREAK_POINT_ISR;
	(*env).singleStepIndex = SINGLE_STEP_ISR;

	//replace old ISRs with new ISR entry in IVT

	swapISRVectors
	(
		&((*env).oldBreakPointVector),
		&((*env).newVector),
		(*env).breakPointIndex
	);

	swapISRVectors
	(
		&((*env).oldSingleStepVector),
		&((*env).newVector),
		(*env).singleStepIndex
	);
	return(env);
}

void disableDebug(struct Environment *env)
{
	setISR(&((*env).oldBreakPointVector),(*env).breakPointIndex);
	setISR(&((*env).oldSingleStepVector),(*env).singleStepIndex);
	free(env);
}

void swapISRVectors
(
	struct ISRVector *oldVector, 
	struct ISRVector *newVector,
	U1 index
)
{
	getISR(oldVector,index);
	setISR(newVector,index);	
	return;
}

void printISR(struct ISRVector *vector)
{
	printf("ISR[");
	printf("CS=%04X:",(*vector).segment);
	printf("IP=%04X]\n",(*vector).address);
	return;
}

//get CS:IP values for ISR at specified index in IVT

void getISR(struct ISRVector *vector,U1 index)
{
	U2 segment;
	U2 address;
	_asm{ mov ah,0x35 }
	_asm{ mov al,index }
	_asm{ int 0x21 }
	_asm{ mov segment,ES }
	_asm{ mov address,BX }
	(*vector).segment = segment;
	(*vector).address = address;
	return;
}

//set CS:IP values for ISR at specified index in IVT

void setISR(struct ISRVector *vector,U1 index)
{
	U2 segment;
	U2 address;
	segment = (*vector).segment;
	address = (*vector).address;

	//code uses DS, so must save it and restore it (via PUSH/POP)

	_asm{ push ds }
	_asm{ mov ah,0x25 }
	_asm{ mov al,index }
	_asm{ mov ds,segment }
	_asm{ mov dx,address }
	_asm{ int 0x21 }
	_asm{ pop ds }
	return;
}

//I embedded the new ISR within the handleInterrupts() function.
//The problem is that an ISR does not utilize a stack frame, so
//variables which might otherwise be local have to be global.
//(for encapsulation, don't access outside of handleInterrupts())

struct Context context;
U2 trace;

U2 rCS;
U2 rDS;
U2 rSS;
U2 rES;

U2 rIP;
U2 rSP;
U2 rBP;
U2 rSI;
U2 rDI;

U2 rAX;
U2 rBX;
U2 rCX;
U2 rDX;
U2 flags;

void handleInterrupts
(
	struct ISRVector *vector, 
	int function
)
{ 
	//special case, asking for address of ISR

	if(function==GET_ADDRESS)
	{
		U2 segment;
		U2 address;

		_asm{ mov AX,CS }
		_asm{ mov segment,AX }
		_asm{ mov AX,OFFSET start }
		_asm{ mov address,AX }

		(*vector).segment = segment;
		(*vector).address = address;
		return;
	}

	//ISR actually begins here...

	start:
	
	saveState:
	_asm{ sti }
	_asm{ mov rDS,DS }
	_asm{ mov rSS,SS }
	_asm{ mov rES,ES }

	_asm{ mov rSP,SP }
	_asm{ mov rBP,BP }
	_asm{ mov rDI,DI }
	_asm{ mov rSI,SI }

	_asm{ mov rAX,AX }
	_asm{ mov rBX,BX }
	_asm{ mov rCX,CX }
	_asm{ mov rDX,DX }

	/*
	interrupt stack		[IP]	= instruction follwing interrupt
						[CS]	= code segment
						[FLAGS] = contents of flags register
	*/
	clearTfFlag:
	_asm{ pop cx }
	_asm{ mov rIP,cx }
	_asm{ pop dx }
	_asm{ mov rCS,dx }
	_asm{ pop ax }
	_asm{ mov flags,ax }
	_asm{ and ax,0xFEFF } /* AX && 1111 1110 1111 1111*/
	_asm{ push ax }
	_asm{ push dx }
	_asm{ push cx }

	context.rCS = rCS;
	context.rDS = rDS;
	context.rSS = rSS;
	context.rES = rES;
	context.rIP = rIP;
	context.rSP = rSP;
	context.rBP = rBP;
	context.rDI = rDI;
	context.rSI = rSI;
	context.rAX = rAX;
	context.rBX = rBX;
	context.rCX = rCX;
	context.rDX = rDX;
	context.flags = flags;


	invokedDebuggerLoop:
	trace = FALSE;
	while(command(&trace,&context)){}
	printf("leaving debug API\n");

	if(!trace){ goto restoreState; }
	
	setTfFlag:
	_asm{ pop cx }
	_asm{ pop dx }
	_asm{ pop ax }
	_asm{ or ax,0x0100 } /* AX || 0000 0001 0000 0000*/
	_asm{ push ax }
	_asm{ push dx }
	_asm{ push cx }

	restoreState:
	_asm{ mov SP,rSP }
	_asm{ mov BP,rBP }
	_asm{ mov DI,rDI }
	_asm{ mov SI,rSI }

	_asm{ mov AX,rAX }
	_asm{ mov BX,rBX }
	_asm{ mov CX,rCX }
	_asm{ mov DX,rDX }

	endISR:
	_asm{ iret }

	return; 
}

BOOLEAN command(U2 *trace, struct Context *context)
{
	U1 ch;

	printf("-");
	scanf("%c",&ch);
	fflush(stdin);

	switch(ch)
	{
		case 'r':
		{ 
			displayRegisters(context); 
		}break;
		case 'd':
		{ 
			dumpMemory(); 
		}break;
		case 'T':
		{ 
			*trace=FALSE;
			printf("tracing disabled\n");
		}break;
		case 't':
		{ 
			*trace=TRUE;
			printf("tracing enabled\n");
		}break;
		case 'q':
		{ 
			return(FALSE); 
		}break;
		default:
		{ 
			help(); 
		}
	}
	return(TRUE);
}

void displayRegisters(struct Context *context)
{
	printf("CS=%04X ",(*context).rCS);
	printf("DS=%04X ",(*context).rDS);
	printf("SS=%04X ",(*context).rSS);
	printf("ES=%04X\n",(*context).rES);

	printf("IP=%04X ",(*context).rIP);
	printf("SP=%04X ",(*context).rSP);
	printf("BP=%04X\n",(*context).rBP);

	printf("SI=%04X ",(*context).rSI);
	printf("DI=%04X\n",(*context).rDI);

	printf("AX=%04X ",(*context).rAX);
	printf("BX=%04X ",(*context).rBX);
	printf("CX=%04X ",(*context).rCX);
	printf("DX=%04X\n",(*context).rDX);

	printf("FLAGS=%04X\n",(*context).flags);
}

U1 getByteValue(U2 segment,U2 currentOffset)
{
	U1 value;
	_asm{ mov ES,segment }
	_asm{ mov BX,currentOffset }
	_asm{ mov al,ES:[BX] }
	_asm{ mov value,al }
	return(value);
}

//display bytes as lines of 16 (in groups of 4)

#define FOUR_BYTES	4
#define LINE_BREAK	16

int isEndOfDisplayGroup(index)
{
	if((index>0)&&((index%FOUR_BYTES)==0)){ return(TRUE); }
	return(FALSE);
}

int isEndOfDisplayLine(index)
{
	if((index>0)&&((index%LINE_BREAK)==0)){ return(TRUE); }
	return(FALSE);
}

void displayByte(U2 index,U2 segment,U2 currentOffset)
{
	if((index==0)||isEndOfDisplayLine(index))
	{ 
		printf("\n"); 
		printf("[%04X][%04X] ",segment,currentOffset); 
	}
	else if(isEndOfDisplayGroup(index))
	{ 
		printf("-"); 
	}

	printf(" %02X",getByteValue(segment,currentOffset));
	return;
}

#define DUMP_LIMIT	128

void dumpMemory()
{
	U2 segmentAddress;
	U2 offsetAddress;
	U2 i;

	printf("enter segment: ");
	scanf("%X",&segmentAddress);
	fflush(stdin);

	printf("enter offset: ");
	scanf("%X",&offsetAddress);
	fflush(stdin);

	for(i=0;i<DUMP_LIMIT;i++)
	{
		displayByte(i,segmentAddress,offsetAddress+i);	
	}
	printf("\n");
}

void help()
{
	printf("--commands--\n");
	printf("r	display registers\n");
	printf("d	dump memory starting at addr\n");
	printf("t	step into next instruction\n");
	printf("T	turn off single-stepping\n");
	printf("q	quit\n");
	return;
}
