
#include "Common.h"

#include "Person.h"

int	GetSSN(Person* p)
{
	return p->ssn;
}

void ShowPerInfo(Person* p)
{
	printf("SSN	: %d\n", p->ssn);
	printf("Name	: %s\n", p->name);
	printf("Address	: %s\n", p->addr);
}

Person*	MakePersonData(int ssn, char* name, char* addr)
{
	Person*	newP	= (Person*)malloc(sizeof(Person));
	
	newP->ssn		= ssn;
	strcpy_s(newP->name, name);
	strcpy_s(newP->addr, addr);

	return newP;
}
