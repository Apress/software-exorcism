/*alias.c ---------------------------------------------------------*/

enum LogicalOperator{AND,OR,XOR,NOT,NULL_OP};
enum RelationalOperator{LESS,GREATER,EQUAL};

struct Field
{
	char *fieldName;
};

//forms a single condition element (i.e. (A>B) )

struct ConditionElement
{
	enum RelationalOperator op;	
	struct Field leftField;
	struct Field rightField;
};

#define MAX_CONDITIONS  8

//a series of elements (i.e. (A>B)OR(B<C) )

struct Condition
{
	int nElements;
	int opArray[MAX_CONDITIONS-1];
	struct ConditionElement elementArray[MAX_CONDITIONS];
};

void addConditionElement
(
	struct Condition *condition,
	struct ConditionElement *leftElement,
	enum LogicalOperator op,
	struct ConditionElement *rightElement
)
{
	int currentIndex;
	currentIndex = (*condition).nElements;

	if(currentIndex>=MAX_CONDITIONS-1){ return; }
	
	(*condition).elementArray[currentIndex]=*leftElement;
	(*condition).opArray[currentIndex]=op;
	(*condition).elementArray[currentIndex+1]=*rightElement;
	
	if(op==NULL_OP){ (*condition).nElements++; }
	else{ (*condition).nElements+=2; }
}

#define CND		struct Condition
#define CNDELM	struct ConditionElement
#define OP		enum LogicalOperator
 
#define cnd		*condition
#define cnd_EA	(cnd).elementArray
#define cnd_OA	(cnd).opArray
#define cnd_SZ	(cnd).nElements

void addConditionElement2
(
	CND cnd,
	CNDELM *leftElement,
	OP op,
	CNDELM *rightElement
)
{
	int currentIndex;
	currentIndex = cnd_SZ;

	if(currentIndex>=MAX_CONDITIONS-1){ return; }
	
	cnd_EA[currentIndex]=*leftElement;
	cnd_OA[currentIndex]=op;
	cnd_EA[currentIndex+1]=*rightElement;
	
	if(op==NULL_OP){ cnd_SZ++; }
	else{ cnd_SZ+=2; }
}

void main()
{
	struct Condition condition;
}