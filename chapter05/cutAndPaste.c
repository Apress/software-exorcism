/* CutAndPaste.c---------------------------------------------------*/

#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define BOOLEAN int
#define TRUE	1==1
#define FALSE	!TRUE

#define CODE_SIZE	4

#define FIELD_ALPHA	1
#define FIELD_DIGIT	2

/*
BOOLEAN isFieldOK(char *field, int type)
{
	int i;

	if((field==NULL)||(strlen(field)>CODE_SIZE))
	{ 
		return(FALSE); 
	}

	switch(type)
	{
		case FIELD_ALPHA:
		{
			for(i=0;i<CODE_SIZE;i++)
			{
				if(!isalpha(field[i])){ return(FALSE); }
			}
		}break;
		case FIELD_DIGIT:
		{
			for(i=0;i<CODE_SIZE;i++)
			{
				if(!isdigit(field[i])){ return(FALSE); }
			}
		}break;
		default:{ return(FALSE); }
	}
	return(TRUE);
}

BOOLEAN editEmployeeRecord
(
	char *companyID,	//[letter][letter][letter][letter]
	char *employeeID,	//[digit][digit][digit][digit]
	char *division,		//[letter][letter][letter][letter]
	char *project		//[letter][letter][letter][letter]
)
{
	if
	(
		isFieldOK(companyID,FIELD_ALPHA)&&
		isFieldOK(employeeID,FIELD_DIGIT)&&
		isFieldOK(division,FIELD_ALPHA)&&
		isFieldOK(project,FIELD_ALPHA)
	)
	{
		return(TRUE);
	}
	return(FALSE);
}
*/

BOOLEAN editEmployeeRecord
(
	char *companyID,	//[letter][letter][letter][letter]
	char *employeeID,	//[digit][digit][digit][digit]
	char *division,		//[letter][letter][letter][letter]
	char *project		//[letter][letter][letter][letter]
)
{
	int i;

	if(companyID==NULL){ return(FALSE); }
	if(strlen(companyID)>CODE_SIZE){ return(FALSE); }
	for(i=0;i<CODE_SIZE;i++)
	{
		if(!isalpha(companyID[i])){ return(FALSE); }
	}

	if(employeeID==NULL){ return(FALSE); }
	if(strlen(employeeID)>CODE_SIZE){ return(FALSE); }
	for(i=0;i<CODE_SIZE;i++)
	{
		if(!isdigit(employeeID[i])){ return(FALSE); }
	}

	if(division==NULL){ return(FALSE); }
	if(strlen(division)>CODE_SIZE){ return(FALSE); }
	for(i=0;i<CODE_SIZE;i++)
	{
		if(!isalpha(division[i])){ return(FALSE); }
	}

	if(project==NULL){ return(FALSE); }
	if(strlen(project)>CODE_SIZE){ return(FALSE); }
	for(i=0;i<CODE_SIZE;i++)
	{
		if(!isalpha(project[i])){ return(FALSE); }
	}
	return(TRUE);
}

void main()
{
	BOOLEAN ok;
	ok = editEmployeeRecord("NASC","0112","TECH","UNIX");
	if(ok){ printf("good\n"); }
	else{ printf("bad\n"); }
}