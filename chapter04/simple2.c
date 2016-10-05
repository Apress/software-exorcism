/* simple2.c ------------------------------------------------------*/

#include<stdio.h>

int i;
int j;

void main()
{
	//do stuff (create signtaure)

	_asm{ nop }		//0x90
	_asm{ int 3 }	//0xCC
	_asm{ nop }		//0x90
	_asm{ inc bx}	//0x66 0x43

	for(i=0;i<10;i++)
	{
		j=i;
	}
	
	fprintf(stdout,"j=%d\n",j);
	fflush(stdout);
	return;
}