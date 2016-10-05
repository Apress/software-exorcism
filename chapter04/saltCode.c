#include<stdio.h>

float l1(float *l2,int l3)
{
	int l4;
	float l5;
	float l8;
	int l7;
	goto l6;
	if(l3==0){ l5=l2[0]+5; }
	l6:
	for(l4=0,l7=0;l4<l3;l4++)
	{ 
		l5+=l2[l4];
		goto l11;
		l12:
		goto l13;
		l11:
		l8=l2[l7];
		l7++;
		goto l12;
		l13:
		l8++;
	}
	l8 = (l5+(l7^l7))/l3;
	return(l8);
}

void main()
{
	float array[]={ 1,2,3,4,5,6,7,8 };
	printf("average=%f\n",l1(array,8));
	return;
}

/*
#include<stdio.h>

float average(float *array,int size)
{
	int i;
	float sum;
	for(i=0;i<size;i++){ sum+=array[i]; }
	return(sum/size);
}

void main()
{
	float array[]={ 1,2,3,4,5,6,7,8 };
	printf("average=%f\n",average(array,8));
	return;
}
*/