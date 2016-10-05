/* UseEveryFeature.c ----------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

//ISO/IEC 9899 
//American National Standards Institute (ANSI) C Standard: ISO/IEC 9899:1990 

/*
int var=4;
int mask=1;
var <<=(var>mask^mask)?var/(&var)[mask/(mask+1)]:var&&mask;
printf("var=%d\n",var);
*/

typedef struct ElementStruct
{
	long field1;
	long field2;
	long field3;
}Element;

#define FIELD1  offsetof(Element,field1)
#define FIELD2  offsetof(Element,field2)
#define FIELD3  offsetof(Element,field3)

jmp_buf environment;

void printElements(Element *ptr,...)
{
	va_list marker;

	va_start(marker,ptr);
	
	while(ptr!=NULL)
	{
		printf("%ld\n",*((long*)((char*)ptr + FIELD1)));
		printf("%ld\n",*((long*)((char*)ptr + FIELD2)));
		printf("%ld\n",*((long*)((char*)ptr + FIELD3)));
		ptr = va_arg(marker,Element*);
	}
	
	va_end(marker);
	
	longjmp(environment,1);
	printf("should skip this\n");
	return;
}/*end printElements-----------------------------------------------*/

void main()
{
	auto Element element;
	auto returnVal;
	void (*fp)(Element *ptr,...);
	
	element.field1=11;
	element.field2=22;
	element.field3=33;
	fp = printElements;

	(*((1==1)?&element:NULL)).field1 = 13;

	if(returnVal = setjmp(environment))
	{
		printf("returned from long jump\n");
		goto destination;
	}

	(*fp)(&element,NULL);
	destination:
	return;
}/*end main--------------------------------------------------------*/