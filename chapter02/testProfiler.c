/* testProfiler.c -------------------------------------------------*/
#include<stdio.h>
#include<math.h>

void doWork()
{ 
	double var = 12.033/34.00032; 
}/*end doWork------------------------------------------------------*/

void doMoreWork()
{  
	long var = 0x12345678 && 0x99887766; 
}/*end doMoreWork--------------------------------------------------*/

void longLoop()
{
	unsigned long int i;
	unsigned long begin;
	unsigned long end;
	begin = time();
	for(i=0;i<100000000;i++)
	{
		double retVal;
		if(i%2==0){ doWork(); }
		else{ doMoreWork(); }
	}
	end = time();
	printf("loop took %d seconds\n",(end-begin));
}/*end longLoop----------------------------------------------------*/

int main()
{
	longLoop();
	return(0);
}/*end main--------------------------------------------------------*/
