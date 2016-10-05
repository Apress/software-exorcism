/* dangling.c -----------------------------------------------------*/

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void shuffleMemory(int *iptr)
{
	srand((unsigned)time( NULL ));
	*iptr = rand();
	return;
}/*end shuffleMemory-----------------------------------------------*/

void dangling()
{
	int *ptr;

	ptr = (int*)malloc(sizeof(int));
	*ptr = 5;

	printf("*ptr = %d\n",*ptr);
	
	free(ptr);
	shuffleMemory(ptr);
	
	printf("*ptr = %d\n",*ptr);
	
	return;
}/*end dangling----------------------------------------------------*/

void main()
{
	dangling();
	return;
}/*end main--------------------------------------------------------*/