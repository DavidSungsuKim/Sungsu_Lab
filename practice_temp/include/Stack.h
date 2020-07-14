
#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>

class CDoublyList;

class CStack
{
public:
	CStack() : list()	{}
	~CStack()			{}

protected:
	CDoublyList	list;

public:
	int		Push	(Data& data, bool abTrace = false );
	int		Pop		(Data& data, bool abTrace = false );
	int		Peek	(Data& data, bool abTrace = false );
	int		IsEmpty	()				{ return list.Count() ? false : true; }
	int		Count	(Data& data)	{ return list.Count(); }
};

inline int		
CStack::Push(Data& data, bool abTrace) 
{ 
	int ret = list.PushBack(data);

	if ( abTrace )
		printf("Stack:Push, data=%d, ret=%d\n", data, ret);

	return ret;
}

inline int		
CStack::Pop(Data& data, bool abTrace) 
{ 
	int ret = list.PopBack(data);

	if (abTrace)
		printf("Stack:Pop, data=%d ret=%d\n", data, ret);

	return ret;
}

inline int		
CStack::Peek(Data& data, bool abTrace)
{
	int ret;
	
	ret = list.PopBack(data);
	ret = list.PushBack(data);

	if (abTrace)
		printf("Stack:Peek, data=%d ret=%d\n", data, ret);

	return ret;
}

#endif
