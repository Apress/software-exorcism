/* Pool.cpp--------------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<object.h>

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

class Pool
{
	private:
	Object *object;
	Object *first;
	Object *last;
	unsigned long size;
	unsigned long currentIndex;

	public:
	Pool(unsigned long size);
	~Pool();
	Object *allocate();
	void free(Object *address);
	void printArray();

};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

Pool::Pool(unsigned long size)
{
	unsigned long i;

	(*this).size =size;
	object = (Object*)malloc(size*sizeof(Object));
	if(object==NULL){ printf("malloc() failed\n"); }
	for(i=0;i<size;i++)
	{
		object[i].setStatus(TRUE);
	}
	currentIndex=0;
	first = &((*this).object[0]);
	last  = &((*this).object[size-1]);
	return;
}/*end constructor-------------------------------------------------*/

Pool::~Pool()
{ 
	free(object); 
}/*end destructor--------------------------------------------------*/

Object* Pool::allocate()
{
	unsigned long i;
	
	if(currentIndex==size){ currentIndex=0; }
	
	for(i=currentIndex;i<size;i++)
	{
		if(object[currentIndex].getStatus())
		{
			object[currentIndex].setStatus(FALSE);
			currentIndex++;
			return(&object[currentIndex-1]);
		}
	}
	return(NULL);
}/*end allocate----------------------------------------------------*/

void Pool::free(Object *address)
{
	if((address>=first)&&(address<=last))
	{
		(*address).setStatus(TRUE);
	}
	else
	{
		printf("release out of range");
	}
	return;
}/*end free--------------------------------------------------------*/

void Pool::printArray()
{
	unsigned long i;
	printf("----------------------------\n");
	for(i=0;i<size;i++)
	{
		if(object[i].getStatus())
		{ 
			printf("[%6lu]=FREE\n",i); 
		}
		else
		{
			printf("[%6lu]=OCCUPIED\n",i);
		}
	}
}/*end printArray--------------------------------------------------*/

/*
#define LIMIT 1000*100
//#define LIMIT 5
Object *object[LIMIT];

void main()
{
	long i;
	unsigned long start;
	unsigned long finish;

	Pool pool(LIMIT);

	start = GetTickCount();
	for(i=0;i<LIMIT;i++)
	{
		object[i]= pool.allocate();
		//pool.printArray();
	}

	for(i=0;i<LIMIT;i++)
	{
		(*object[i]).setValue(0xAABBCCDD);
	}

 	for(i=(LIMIT-1);i>=0;i=i-1)
	{ 
		pool.free(object[i]);
		//pool.printArray();
	}
	finish = GetTickCount();
	printf("msecs elapsed=%lu\n",(finish-start));
	return;
}*/

#define LIMIT 1000*100
//#define LIMIT 10
Object *object[LIMIT];

void main()
{
	long i;
	unsigned long start;
	unsigned long finish;

	start = GetTickCount();
	for(i=0;i<LIMIT;i++)
	{
		object[i]= (Object*)malloc(sizeof(Object));
	}

	for(i=0;i<LIMIT;i++)
	{
		(*object[i]).setValue(0xAABBCCDD);
		(*object[i]).setStatus(TRUE);
	}

 	for(i=(LIMIT-1);i>=0;i--)
	{
		//printf("free %ld\n",i);
		free(object[i]);
	}

	finish = GetTickCount();
	printf("msecs elapsed=%lu\n",(finish-start));
	return;
}
