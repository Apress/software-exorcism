/* UnitTest.cpp ---------------------------------------------------*/

#include<stdio.h>
#include<string.h>

#define BOOL		int
#define TRUE		(1==1)
#define FALSE		!TRUE

struct Location
{
	char *testName;
	char *className;
	char *function;
};

struct TestException
{
	Location *location;
	char *message;
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define TESTER_BUFFER_SIZE  256

class Tester
{
	private:
	static char *buffer; 
	static void assertFailed
	(
		TestException *testException,
		char *message
	);
	
	public:
	static void assertEquals
	(
		TestException *testException,
		char var1,
		char var2
	);
	static void assertEquals
	(	
		TestException *testException,
		short var1,
		short var2
	);
	static void assertEquals
	(
		TestException *testException,
		int var1,
		int var2
	);
	static void assertEquals
	(
		TestException *testException,
		long var1,
		long var2
	);
	static void assertEquals
	(
		TestException *testException,
		float var1,
		float var2
	);
	static void assertEquals
	(
		TestException *testException,
		double var1,
		double var2
	);
	static void assertEquals
	(
		TestException *testException,
		char *var1,
		char *var2
	);
	static void assertEquals
	(
		TestException *testException,
		void *var1,
		void *var2
	);
	static void getExceptionInfo
	(
		char *buffer,
		int bufferSize,
		TestException *testException
	);
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

char* Tester::buffer = new char[TESTER_BUFFER_SIZE];

void Tester::assertFailed
(
	TestException *testException,
	char *message
)
{
	(*testException).message = message;
	throw(testException);
}/*end Tester::assertFailed----------------------------------------*/

void Tester::assertEquals
(
	TestException *testException,
	char var1,
	char var2
)
{
	if(var1!=var2)
	{
		sprintf(Tester::buffer,"%c!=%c",var1,var2);
		Tester::assertFailed(testException,buffer);
	}
}/*end Tester::assertEquals----------------------------------------*/

void Tester::assertEquals
(
	TestException *testException,
	short var1,
	short var2
)
{
	if(var1!=var2)
	{
		sprintf(Tester::buffer,"%d!=%d",var1,var2);
		Tester::assertFailed(testException,buffer);
	}
}/*end Tester::assertEquals----------------------------------------*/

void Tester::assertEquals
(
	TestException *testException,
	int var1,
	int var2
)
{
	if(var1!=var2)
	{
		sprintf(Tester::buffer,"%d!=%d",var1,var2);
		Tester::assertFailed(testException,buffer);
	}
}/*end Tester::assertEquals----------------------------------------*/

void Tester::assertEquals
(
	TestException *testException,
	long var1,
	long var2
)
{
	if(var1!=var2)
	{
		sprintf(Tester::buffer,"%ld!=%ld",var1,var2);
		Tester::assertFailed(testException,buffer);
	}
}/*end Tester::assertEquals----------------------------------------*/

void Tester::assertEquals
(
	TestException *testException,
	float var1,
	float var2
)
{
	if(var1!=var2)
	{
		sprintf(Tester::buffer,"%f!=%f",var1,var2);
		Tester::assertFailed(testException,buffer);
	}
}/*end Tester::assertEquals----------------------------------------*/

void Tester::assertEquals
(
	TestException *testException,
	double var1,
	double var2
)
{
	if(var1!=var2)
	{
		sprintf(Tester::buffer,"%e!=%e",var1,var2);
		Tester::assertFailed(testException,buffer);
	}
}/*end Tester::assertEquals----------------------------------------*/

void Tester::assertEquals
(
	TestException *testException,
	char *var1,
	char *var2
)
{
	if(strcmp(var1,var2))
	{
		unsigned int limit = TESTER_BUFFER_SIZE/2;
		if((strlen(var1)>=limit)||(strlen(var2)>=limit))
		{ 
			sprintf(Tester::buffer,"strings not equal");
		}
		else
		{
			sprintf(Tester::buffer,"%s!=%s",var1,var2);
		}
		Tester::assertFailed(testException,buffer);
	}
}/*end Tester::assertEquals----------------------------------------*/

void Tester::assertEquals
(
	TestException *testException,
	void *var1,
	void *var2
)
{
	if(var1!=var2)
	{
		sprintf(Tester::buffer,"objects not same instance");
		Tester::assertFailed(testException,buffer);
	}
}/*end Tester::assertEquals----------------------------------------*/

#define DELIMITERS	10

void Tester::getExceptionInfo
(
	char *buffer,
	int bufferSize,
	TestException *testException
)
{
	int length;

	length = strlen((*(*testException).location).testName)+
		     strlen((*(*testException).location).className)+
			 strlen((*(*testException).location).function)+
			 strlen((*testException).message)+
			 DELIMITERS; //for extra formatting characters

	if(length>=bufferSize)
	{
		sprintf(buffer,"\0x0");
		return;
	}

	sprintf
	(
		buffer,
		"[%s][%s.%s][%s]",
		(*(*testException).location).testName,
		(*(*testException).location).className,
		(*(*testException).location).function,
		(*testException).message
	);
}/*end Tester::getExceptionInfo------------------------------------*/

void peformStringTest()
{
	Location *location = new Location;
	(*location).className = "none";
	(*location).function  = "performStringTest()";
	(*location).testName  = "string test";

	TestException *testException = new TestException;
	(*testException).location = location;

	//Tester::assertEquals(testException,'c','d');
	//Tester::assertEquals(testException,1,2);
	Tester::assertEquals(testException,"Texas","California");

	delete((*testException).location);
	delete(testException);
}

void main()
{
	try
	{
		peformStringTest();
	}
	catch(TestException *exception)
	{
		const int size = 256;
		char buffer[size];

		Tester::getExceptionInfo(buffer,size,exception);
		
		printf("ah ha!\n%s\n",buffer);

		delete((*exception).location);
		delete(exception);
	}
}