/* WindowsTime.c --------------------------------------------------*/

#include<stdio.h>
#include<time.h>
#include<math.h>
#include<windows.h>

#define LIMIT 25000

void doWork()
{
	double j=21.0;
	double k=3.0;

	double l=pow(j,k);	//expensive routine
	return;
}/*end doWork------------------------------------------------------*/

void useANSIClock()
{
	clock_t start;
	clock_t finish;
	float diff;
	long int i=0;

	start = clock();
	for(i=0;i<LIMIT;i++)
	{ 
		doWork(); 
	}
	finish = clock();
	
	diff=((float)(finish-start))/((float)CLOCKS_PER_SEC);
	printf("seconds elapsed=%e\n",diff);
	return;
}/*end useANSIClock------------------------------------------------*/

void useGetTickCount()
{	
	unsigned long ms_start;
	unsigned long ms_finish;
	unsigned long ms_diff;
	long int i=0;

	ms_start = GetTickCount();
	for(i=0;i<LIMIT;i++)
	{ 
		doWork(); 
	}
	ms_finish = GetTickCount();
	
	ms_diff=ms_finish-ms_start;
	printf("milliseconds elapsed=%lu\n",ms_diff);
	return;
}/*end useGetTickCount---------------------------------------------*/

void usePerformanceCounter()
{
	LARGE_INTEGER countStart;
	LARGE_INTEGER countFinish;
	LARGE_INTEGER countDiff;
	LARGE_INTEGER countFreq;
	float diff;
	long int i=0;

	if(QueryPerformanceCounter(&countStart))
	{
		for(i=0;i<LIMIT;i++)
		{ 
			doWork(); 
		}
		QueryPerformanceCounter(&countFinish);
		
		countDiff.QuadPart = countFinish.QuadPart-countStart.QuadPart;
		QueryPerformanceFrequency(&countFreq);
		
		diff = ((float)countDiff.QuadPart)/
			   ((float)countFreq.QuadPart);
		
		printf("elasped ticks=%I64lu\n",countDiff.QuadPart);
		printf("elasped seconds=%e\n",diff);
	}
	else
	{
		printf("High-Performance Timer Not Functional");
	}
	return;
}/*end usePerformanceCounter---------------------------------------*/

void main()
{
	useANSIClock();
	useGetTickCount();
	usePerformanceCounter();
	return;
}/*end main--------------------------------------------------------*/