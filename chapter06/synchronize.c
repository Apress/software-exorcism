/*synchronize.c ---------------------------------------------------*/

#include<stdio.h>
#include<windows.h>

//NOTA BENE - make sure to link with multi-threaded libraries!

#define BEGIN	begin = GetTickCount()
#define END		end = GetTickCount()
#define TIME	printf("msecs=%lu\n",end-begin)
#define nLOOPS	500000

void routine(char *string)
{
	//printf("called by %s\n",string);
}

CRITICAL_SECTION criticalSection;

void synchRoutine(char *string)
{
	EnterCriticalSection(&criticalSection);
	//printf("called by %s\n",string);
	LeaveCriticalSection(&criticalSection);
}

#define NAME_LENGTH		8	/* [t][h][r][e][a][d][#][0x00] */
#define OK				0

DWORD threadFunc(int *index)
{
	char name[NAME_LENGTH];
	long begin;
	long end;
	unsigned long i;
		
	strcpy(name,"thread");
	name[NAME_LENGTH-2]='0'+((char)(*index));
	name[NAME_LENGTH-1]=0x00;
	printf("created %s\n",name);

	BEGIN;
	for(i=0;i<nLOOPS;i++)
	{ 
		synchRoutine(name); 
	}
	END;
	printf("END thread %d msecs=%lu\n",*index,(end-begin));
	return(OK);
}

#define nTHREADS	5

void main()
{
	long begin;
	long end;
	unsigned long i;

	HANDLE hThread[nTHREADS];
	DWORD  threadID[nTHREADS];
	int    argument[nTHREADS];
	DWORD (*functionPointer)(int *index);

	//no synchronization

	BEGIN;
	for(i=0;i<nLOOPS;i++){ routine("main()"); }
	END;
	TIME;

	InitializeCriticalSection(&criticalSection);
	
	//uncontended synchronization

	BEGIN;
	for(i=0;i<nLOOPS;i++){ synchRoutine("main()"); }
	END;
	TIME;

	//now add some contention

	functionPointer = threadFunc;
	
	for(i=0;i<nTHREADS;i++)
	{
		argument[i]=i;
		hThread[i] = CreateThread
		(
			NULL,
			0,
			(LPTHREAD_START_ROUTINE)functionPointer,
			(LPVOID)&argument[i],
			0,
			&threadID[i]
		);
	}
	
	WaitForMultipleObjects(nTHREADS,hThread,TRUE,INFINITE);
	for(i=0;i<nTHREADS;i++)
	{ 
		CloseHandle(hThread[i]); 
	}

	DeleteCriticalSection(&criticalSection);
}

/*
msecs=9
msecs=46			= 5x non-synchronized
created thread0
created thread1
created thread2
created thread3
created thread4
END thread 1 msecs=635   = 13x noncontended
END thread 0 msecs=1301
END thread 3 msecs=1544
END thread 4 msecs=1509
END thread 2 msecs=1585
*/