/* cutAndPaste.c --------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>

float computeSum(float data[], int size)
{
	float ret = 0;
	int i;

	for(i=0;i<size;i++){ ret = ret + data[i]; }
	
	return(ret);
}/*end computeSum--------------------------------------------------*/ 

float computeMean(float data[], int size)
{
	return(computeSum(data,size)/((float)size));
}/*end computeMean-------------------------------------------------*/

float computeSampleVariance(float data[], int size)
{
	float mean;
	float *ptr;
	float meanSquares;
	int i;

	mean = computeMean(data,size);
	
	ptr = (float*)malloc(sizeof(float)*size);
	for(i=0;i<size;i++){ptr[i]=data[i]*data[i];}
	meanSquares = computeMean(ptr,size);
	free(ptr);

	return(meanSquares-(mean*mean));
}/*end computeSampleVariance---------------------------------------*/

void printStats(float data[], int size)
{
	if(size<=0)
	{ 
			fprintf(stderr,"size must be positive\n");
			return; 
	}
	printf("mean = %f\n",computeMean(data,size));
	printf("var = %f\n",computeSampleVariance(data,size));
	return;
}/*end printStats--------------------------------------------------*/

void printStats2(float data[], int size)
{
		float sum;
		float variance;
		int i;

		if(size<=0)
		{ 
			fprintf(stderr,"size must be positive\n");
			return; 
		}
		sum = 0;
		for(i=0;i<size;i++){ sum = sum + data[i]; }
		printf("mean = %f\n",sum/((float)size));
		
		sum = 0;
		for(i=0;i<size;i++){ sum = sum + (data[i]*data[i]); }
		variance = sum/((float)size);

		sum = 0;
		for(i=0;i<size;i++){ sum = sum + data[i]; }
		variance -= (sum/((float)size))*(sum/((float)size));
		
		printf("var = %f\n",variance);
		return;
}/*end printStats2-------------------------------------------------*/

void main()
{
	float data[]={1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};
	int size = 10;
	printStats2(data,size);
	printStats(data,size);
	return;
}/*end main--------------------------------------------------------*/