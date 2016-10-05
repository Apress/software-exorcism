/*SpaghettiCode.c--------------------------------------------------*/

#include<stdio.h>

#define MAX_SIZE  10
long array[MAX_SIZE];
long temp;
int	 nValues=0;
int  current;
int  outerIndex;
int  innerIndex;

void main(int argc, char* argv[])
{
	enterValues:
	printf("enter value [enter -1 to quit]:\n");
	scanf("%ld",&array[nValues]);
	fflush(stdin);
	if(array[nValues]==-1)
	{ 
		nValues--;
		goto sortValues; 
	}
	if(nValues==MAX_SIZE-1){ goto sortValues; }
	nValues++;
	goto enterValues;

	sortValues:
	printf("entered %d integers\n",nValues+1);
	
	outerIndex=0;
	outerLoop:
	if(outerIndex==nValues+1){ goto endOuterLoop; }
		
		innerIndex=outerIndex;
		innerLoop:
		if
		(
			(innerIndex>0)&&
			(array[innerIndex]<array[innerIndex-1])
		)
		{ 
			int temp = array[innerIndex];
			array[innerIndex]=array[innerIndex-1];
			array[innerIndex-1]=temp;
		}
		else{ goto endInnerLoop; }
		innerIndex--;
		goto innerLoop;
		endInnerLoop:

	outerIndex++;
	goto outerLoop;
	endOuterLoop:

	printf("\nsorted values are:\n");
	if(nValues==0){ goto end; }
	current=0;
	displayValues:
	printf("array[%d]=%ld\n",current,array[current]);
	current++;
	if(current==nValues+1){ goto end; }
	goto displayValues;

	end:
	printf("program is ending\n");
	return;
}