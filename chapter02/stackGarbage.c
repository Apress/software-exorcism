/* stackGarbage.c -------------------------------------------------*/
#include<stdio.h>

void firstCall()
{
	char array[] = {'a','b','c','d','e','f','g','h'};
}/*end firstCall---------------------------------------------------*/

void secondCall()
{
	char ch1;
	char ch2;
	printf("%c\n",ch1);
	printf("%c\n",ch2);
}/*end secondCall--------------------------------------------------*/

char thirdCall()
{
	char ch3;
	return(ch3);
}/*end thirdCall---------------------------------------------------*/

void garbageInit()
{
	firstCall();
	secondCall();
	printf("%c\n",thirdCall());
}/*end garbageInit-------------------------------------------------*/

void main()
{
	garbageInit();
}