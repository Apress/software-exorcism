/* LogFramework.cpp -----------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

class LogHandler
{
	public:
	virtual void persist(char *string);
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void LogHandler::persist(char *string)
{
	return;
}/* end LogHandler::persist----------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

class FileLogHandler:public LogHandler
{
	private:
	FILE *filePtr;
	unsigned int maxMessageSize;
	
	public:
	FileLogHandler(unsigned int maxMessageSize, FILE *filePtr);
	~FileLogHandler();
	void persist(char *string);
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

FileLogHandler::FileLogHandler
(
	unsigned int maxMessageSize,
	FILE *filePtr
)
{
	(*this).maxMessageSize = maxMessageSize;
	(*this).filePtr = filePtr;
}/*end constructor-------------------------------------------------*/

FileLogHandler::~FileLogHandler()
{
	fclose(filePtr);
}/*end destructor--------------------------------------------------*/

void FileLogHandler::persist(char *string)
{
	if(strlen(string)>=maxMessageSize)
	{ 
		fwrite(string,1,maxMessageSize,filePtr);
		return; 
	}
	fputs(string,filePtr);
}/*end FileLogHandler::persist-------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define MAX_HANDLERS	10
#define MAX_FILTERS		50

class LogFilter
{
	private:
	int nHandlers;
	int nFilters;

	LogHandler *handlerArray[MAX_HANDLERS];
	char *textFilters[MAX_FILTERS];
	char *screen(char *string);

	public:
	LogFilter();
	void addHandler(LogHandler *logHandler);
	void addFilter(char *string);
	void filter(char *string);
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

LogFilter::LogFilter()
{ 
	nHandlers = 0;
	nFilters  = 0;
}/*end constructor-------------------------------------------------*/

void LogFilter::addHandler(LogHandler *logHandler)
{
	if(nHandlers==MAX_HANDLERS){ return; }
	handlerArray[nHandlers]=logHandler;
	nHandlers++;
}/*end LogFilter::addHandler---------------------------------------*/

void LogFilter::addFilter(char *string)
{
	if(nFilters==MAX_FILTERS){ return; }
	textFilters[nFilters]=string;
	nFilters++;
}/*end LogFilter::addFilter----------------------------------------*/

void LogFilter::filter(char *string)
{
	int i;
	if(nFilters==0)
	{
		for(i=0;i<nHandlers;i++)
		{
			(*handlerArray[i]).persist(string);
		}
		return;
	}

	for(i=0;i<nHandlers;i++)
	{
		if(screen(string)!=NULL)
		{
			(*handlerArray[i]).persist(string);
		}
	}
}/*end LogFilter::filter-------------------------------------------*/

char* LogFilter::screen(char *string)
{
	int i;
	for(i=0;i<nFilters;i++)
	{
		if(strstr(string,textFilters[i])!=NULL)
		{ 
			return(string); 
		}
	}
	return(NULL);
}/*end LogFilter::screen-------------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Declaration                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

class LogMessage
{
	private:
	static LogMessage *logMessage;
	static char *buffer;
	LogFilter *logFilter;
	
	LogMessage();
	void log
	(
		char *className,
		char *function,
		const char *level,
		char *message
	);

	public:
	const static int MAX_BUFFER_SIZE;
	const static char *ADMIN;
	const static char *ERROR;
	const static char *TRACE;
	
	static LogMessage* getInstance();
	void registerFilter(LogFilter *logFilter);
	void admin
	(
		char *className,
		char *function,
		char *message	
	);
	void error
	(
		char *className,
		char *function,
		char *message	
	);
	void trace
	(
		char *className,
		char *function,
		char *message	
	);
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Definitions                                                       +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

const int LogMessage::MAX_BUFFER_SIZE = 1024;
char* LogMessage::buffer = new char[MAX_BUFFER_SIZE];

LogMessage* LogMessage::logMessage=NULL;

const char* LogMessage::ADMIN = "ADMIN";
const char* LogMessage::ERROR = "ERROR";
const char* LogMessage::TRACE = "TRACE";

LogMessage::LogMessage(){}/*end constructor------------------------*/

LogMessage* LogMessage::getInstance()
{ 
	if(logMessage!=NULL)
	{ 
			return(logMessage); 
	}
	logMessage = new LogMessage;
	return(logMessage);
}/*end LogMessage::getInstance()-----------------------------------*/

void LogMessage::registerFilter(LogFilter *logFilter)
{
	if(logFilter!=NULL)
	{
		(*this).logFilter = logFilter;
	}
}/*end LogMessage::registerFilter----------------------------------*/

void LogMessage::log
(
	char *className,
	char *function,
	const char *level,
	char *message
)
{
	int length;
	length = strlen(className)+
		     strlen(function)+
			 strlen(level)+
			 strlen(message);

	if(length>=MAX_BUFFER_SIZE){ return; }

	sprintf
	(
		buffer,
		"[%s.%s][%s][%s]\n",
		className,
		function,
		level,
		message
	);
	if(logFilter!=NULL)
	{
		(*logFilter).filter(buffer);
	}
}/*end LogMessage::log---------------------------------------------*/

void LogMessage::admin
(
	char *className,
	char *function,
	char *message	
)
{
	(*this).log(className,function,ADMIN,message);
}/*end LogMessage::admin-------------------------------------------*/

void LogMessage::error
(
	char *className,
	char *function,
	char *message	
)
{
	(*this).log(className,function,ERROR,message);
}/*end LogMessage::error-------------------------------------------*/

void LogMessage::trace
(
	char *className,
	char *function,
	char *message	
)
{
	(*this).log(className,function,TRACE,message);
}/*end LogMessage::trace-------------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Driver                                                            +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void main()
{
	FileLogHandler logHandler(LogMessage::MAX_BUFFER_SIZE,stdout);

	LogFilter logFilter = LogFilter();
	logFilter.addHandler(&logHandler);
	logFilter.addFilter((char*)LogMessage::TRACE);
	logFilter.addFilter("error msg");

	LogMessage *message = LogMessage::getInstance();
	(*message).registerFilter(&logFilter);
	(*message).admin("class","function","admin msg");
	(*message).trace("class","function","trace msg");
	(*message).error("class","function","error msg");

	return;
}