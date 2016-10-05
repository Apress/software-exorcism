/* useZlibDLL.c ---------------------------------------------------*/

#include<stdio.h>
#include<windows.h>
#define BUFFER_SIZE	100 

char text[BUFFER_SIZE];
char compress[BUFFER_SIZE];
char decompress[BUFFER_SIZE];

void main()
{
	typedef int (*FunctionPointer)
	(
		char *dest, 
		unsigned long *destLen, 
		const char *source, 
		unsigned long sourceLen
	);
	FunctionPointer functionPointer;
	HINSTANCE handle;
	unsigned long length;
	unsigned long capacity;
	unsigned long i;

	for(i=0;i<=BUFFER_SIZE;i++){ text[i]='A';}

	handle = LoadLibrary("ZLIB.DLL");
	if(handle==NULL){ return; }
	
	functionPointer =(FunctionPointer)GetProcAddress(handle,"compress");
	if(functionPointer==NULL){ return; }

	(*functionPointer)(compress,&length,text,BUFFER_SIZE);

	printf("output length=%u\n",length);
	for(i=0;i<length;i++)
	{ 
		printf("%c",compress[i]); 
	} 
	printf("\n");
	
	functionPointer =(FunctionPointer)GetProcAddress(handle,"uncompress");
	if(functionPointer==NULL){ return; }

	capacity = BUFFER_SIZE;
	(*functionPointer)(decompress,&capacity,compress,length);

	printf("output length=%u\n",capacity);
	for(i=0;i<capacity;i++)
	{ 
		printf("%c",decompress[i]); 
	} 
	printf("\n");

	FreeLibrary(handle);
	return;
}
