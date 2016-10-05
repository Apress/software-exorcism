/* BufferOverflow.c -----------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFFER_SIZE		4

void victim(char *str)
{
	char buffer[BUFFER_SIZE];
	strcpy(buffer,str);
	return;
}/*end victim------------------------------------------------------*/

void redirected()
{
	printf("\tYou've been redirected!\n");
	exit(0);
	return;
}/*end redirected--------------------------------------------------*/

void main()
{
	char buffer[]={'1','2','3','4',					/*buffer[4]*/
                   '5','6','7','8',					/*EBP[4] */
                   '\x0','\x0','\x0','\x0','\x0'};	/*EIP[4]*/
	void *fptr;
	unsigned long *lptr;

	printf("buffer = %s\n",buffer);
	
	fptr = redirected;
	lptr = (unsigned long*)(&buffer[8]);
	*lptr = (unsigned long)fptr;
	
	printf("main()\n");
	victim(buffer);
	printf("main()\n");	//will never make it to this code
	return;
}/*end main--------------------------------------------------------*/
