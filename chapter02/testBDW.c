/* testBDW.c-------------------------------------------------------*/

#include<stdio.h>
#define GC_NOT_DLL
#define GC_DEBUG
#include<gc.h>

#define KB 1024
unsigned long freeBytes=0;

void printStatus()
{
	unsigned long currentBytes;
	currentBytes = GC_get_free_bytes();
	printf("heap size=%7lu\t",GC_get_heap_size());
	printf("free size=%7lu\t",currentBytes);
	if(freeBytes!=0)
	{
		printf("diff=%ld",(freeBytes-currentBytes));
	}
	printf("\n");
	freeBytes = currentBytes;
	return;
}/*end printStatus-------------------------------------------------*/

void main()
{
	int j;
	int i;
	int limit = 10;

	GC_find_leak = 1;

	for(j=0;j<limit;j++)
	{
		unsigned char *pointer;
		pointer = GC_malloc(10*KB);
		for(i=0;i<KB;i++){ pointer[i]=0x01; }
		printStatus();
	}
	printf("\n--explicit collection--\n");
	GC_gcollect();
	printStatus();
	return;
}/*end main--------------------------------------------------------*/