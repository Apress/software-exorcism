/* floatingPoint.c ------------------------------------------------*/

#include<stdio.h>
#include<windows.h>

#define BEGIN	begin = GetTickCount()
#define END		end = GetTickCount()
#define TIME	printf("msecs=%lu\n",end-begin)
#define MILLION	1000000

void main()
{
	unsigned long begin;
	unsigned long end;
	unsigned long i;
	long value1;
	long value2 = 42;
	long value3 = 7;
	double fvalue1;
	double fvalue2 = 54645.23423;
	double fvalue3 = 343245.324234;

	BEGIN;
	for(i=0;i<MILLION;i++)
	{
		value1 = value2+value3;
		value1 = value2-value3;
		value1 = value2*value3;
		value1 = value2/value3;
	}
	END;
	TIME;

	BEGIN;
	for(i=0;i<MILLION;i++)
	{
		fvalue1 = fvalue2+fvalue3;
		fvalue1 = fvalue2-fvalue3;
		fvalue1 = fvalue2*fvalue3;
		fvalue1 = fvalue2/fvalue3;
	}
	END;
	TIME;

	BEGIN;
	for(i=0;i<MILLION;i++)
	{
		fvalue1 = (double)value1;
		fvalue2 = (double)value2;
		fvalue3 = (double)value3;
		value1 = (long)fvalue1;
	}
	END;
	TIME;

	value1 = value1 + 0xFF;
	value1 += 0xFF;
}