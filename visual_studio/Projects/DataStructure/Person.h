#pragma once

#ifndef _PERSON_
#define _PERSON_		1

#define STR_LEN			50

typedef struct _person
{
	int		ssn;
	char	name[STR_LEN];
	char	addr[STR_LEN];
}Person;

int			GetSSN			(Person* p);
void		ShowPerInfo		(Person* p);
Person*		MakePersonData	(int ssn, char* name, char* addr);

#endif
