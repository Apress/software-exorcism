/* varyArgs.c -----------------------------------------------------*/

#include<stdio.h>
#include <stdarg.h>

double average(double value,...)
{
	int size;
	double sum;
	va_list marker;

	size=0;
	va_start(marker,value);
	
	while(value!=0.0)
	{
		sum += value;
		size++;
		printf("value=%f\n",value);
		value = va_arg(marker,double);
	}
	va_end(marker);
	return(sum/size);
}

void main()
{
	double sampleMean;
	sampleMean = average(1.01,34.02,7.45,0.0);
	printf("sampleMean=%f\n",sampleMean);
}