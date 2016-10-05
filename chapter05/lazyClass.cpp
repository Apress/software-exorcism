/* LazyClass.cpp --------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define DEFAULT_SIZE	8

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

class LazyClass
{
	private:
	char *buffer;
	int size;

	public:
	LazyClass();
	~LazyClass();
	char *getBuffer();
	void setBuffer(char *buffer, int nbytes);
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LazyClass::LazyClass()
{
	buffer = NULL;
	size = DEFAULT_SIZE;
}/*end constructor-------------------------------------------------*/

LazyClass::~LazyClass()
{
	if(buffer!=NULL)
	{ 
		printf("destructor()\n");
		free(buffer); 
	}
}/*end destructor--------------------------------------------------*/

char *LazyClass::getBuffer()
{ 
	if(buffer==NULL)
	{ 
		printf("NULL, allocating\n");
		buffer = (char*)malloc(size);
		buffer[size-1]=0x00;
	}
	return(buffer); 
}/*end getBuffer---------------------------------------------------*/

void LazyClass::setBuffer(char *buffer, int nbytes)
{
	if((*this).buffer==NULL)
	{
		printf("NULL, allocating\n");
		(*this).buffer = (char*)malloc(nbytes);
		(*this).size = nbytes;
	}

	if((*this).size<nbytes)
	{
		printf("resizing\n");
		free((*this).buffer);
		(*this).buffer = (char*)malloc(nbytes);
		(*this).size = nbytes;
	}
	strcpy((*this).buffer,buffer);
}/*end setBuffer---------------------------------------------------*/

void main()
{
	char *string;
	LazyClass lazyClass = LazyClass();

	//printf("%s\n",lazyClass.getBuffer());

	string = "hey now!";
	lazyClass.setBuffer(string,strlen(string)+1);
	printf("%s\n",lazyClass.getBuffer());

	string = "hey now baby!";
	lazyClass.setBuffer(string,strlen(string)+1);
	printf("%s\n",lazyClass.getBuffer());

	string = "hey now!";
	lazyClass.setBuffer(string,strlen(string)+1);
	printf("%s\n",lazyClass.getBuffer());
	return;
}