/* ExtendableArray.cpp --------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

class ExtendableArray
{
	private:
	int increment;	/*controls re-allocation size*/
	int *start;		/*pointer to start of list*/
	int capacity;	/*current capacity*/
	int nextIndex;	/*next free space*/

	public:
	ExtendableArray(int initialSize,int increment);
	~ExtendableArray();
	void addToList(int value);
	int getArrayElement(int index);
	void printList();	
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

ExtendableArray::ExtendableArray
(
	int initialSize,
	int increment
)
{
	(*this).increment = increment;

	start = (int*)malloc(initialSize*sizeof(int));
	
	if(start==NULL)
	{
		printf("constructor(): cannot allocate memory\n");
		exit(1);
	}
	nextIndex = 0;
	capacity = initialSize;
	return;

}/*end constructor-------------------------------------------------*/

ExtendableArray::~ExtendableArray()
{ 
	free(start); 
	return;
	
}/*end destructor--------------------------------------------------*/

void ExtendableArray::addToList(int value)
{
	int *temp;
	int mallocSize;

	if(nextIndex >= capacity)
	{
		mallocSize = capacity + increment;
		temp = (int*)malloc(mallocSize*sizeof(int));
		
		if(temp==NULL)
		{
			printf("addToList(): cannot allocate more memory\n");
			exit(1);
		}
		else
		{
			printf("addToList(): not enough room for %d\n",value);
			printf("addToList(): allocating %d more cells\n",increment);
			memcpy(temp,start,capacity*sizeof(int));
			free(start);
			start = temp;
			capacity = capacity+increment;
		}
	}
	
	start[nextIndex]=value;
	//printf("adding %d\n",start[nextIndex]);
	nextIndex++; 
	return;

}/*end addToList---------------------------------------------------*/

int ExtendableArray::getArrayElement(int index)
{
	if((index>=0)&&(index<capacity))
	{
		return(start[index]);
	}
	printf("getArrayElement(): index %d out of bounds\n",index);
	return(0);
}/*end getArrayElement---------------------------------------------*/

void ExtendableArray::printList()
{
	int i;

	printf("capacity =%d\n",capacity); 
	printf("next index    =%d\n",nextIndex);
	
	for(i=0;i<nextIndex;i++)
	{
		printf("%d) %d\n",i,start[i]);
	}
	return;

}/*printList-------------------------------------------------------*/

void main()
{

	ExtendableArray array(4,4);
	
	array.addToList(4); 
	array.addToList(-5); 
	array.addToList(1); 
	array.addToList(11); 
	
	array.addToList(7); 
	array.addToList(8); 
	array.addToList(-12); 
	array.addToList(122); 
	
	array.addToList(4);
	array.addToList(5); 
	array.addToList(5); 
	array.addToList(-101); 

	array.addToList(3); 
	
	array.printList();

	printf("array[2]=%d\n",array.getArrayElement(2));

	return;

}/*end main--------------------------------------------------------*/