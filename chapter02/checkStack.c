/*checkStack.c ----------------------------------------------------*/
#include<stdio.h>
#include<stdlib.h>

#define SIG_VAL		0x11223344
#define SIGNATURE	int sig = SIG_VAL
#define RETURN		check(sig); return

void check(int sig)
{
	printf("signature=%x\n",sig);
	if(sig==SIG_VAL){ return; }
	printf("Stack has been corupted!\n");
	exit(1);
}/*end check-------------------------------------------------------*/

void corruptStack(char *str,int limit)
{
	int i;
	for(i=0;i<limit;i++){ str[i]='a'+i; }
}/*end corruptStack------------------------------------------------*/

void testCall(int limit)
{	
	SIGNATURE;
	char array[4];
	printf("limit=%d ",limit);
	corruptStack(array,limit);
	RETURN;
}/*end testCall----------------------------------------------------*/

void main()
{
	testCall(1);
	testCall(2);
	testCall(3);
	testCall(4);
	testCall(5);
	testCall(6);
	testCall(7);
	return;
}/*end main--------------------------------------------------------*/