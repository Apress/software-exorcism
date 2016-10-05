/* Object.h -------------------------------------------------------*/

#define BOOLEAN int

class Object
{
	private:
	BOOLEAN isFree;
	DWORD value;

	public:
	BOOLEAN getStatus();
	void setStatus(BOOLEAN status);
	DWORD getValue();
	void setValue(DWORD value);

};

BOOLEAN Object::getStatus(){ return(isFree); }

void Object::setStatus(BOOLEAN status)
{
	switch(status)
	{
		case TRUE:
		case FALSE:{ isFree= status; }break;
		default:{ isFree = FALSE; }
	}
}

DWORD Object::getValue(){ return(value); }

void Object::setValue(DWORD value){ (*this).value = value; }