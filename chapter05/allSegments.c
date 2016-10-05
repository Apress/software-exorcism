/* allSegments.c---------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>

unsigned long var;

void printBinary(unsigned long *pointer)
{
	unsigned long mask;
	int bitSize;
	int i;

	if(pointer==NULL){ pointer = &var; }

	mask = 1;
	bitSize = sizeof(var)*8;
	for(i=0;i<bitSize;i++)
	{
		if(mask&(*pointer)){ printf("1"); }
		else{ printf("0"); }
		mask*=2;
	}
	printf("\n");
	return;
}

void main()
{
	unsigned long *pointer;
	
	pointer = (unsigned long*)malloc(sizeof(unsigned long));

	var = 0xFF00FF00;
	*pointer=0xFFFF0000;
	
	printBinary(NULL);
	printBinary(pointer);

	free(pointer);
	return;
}