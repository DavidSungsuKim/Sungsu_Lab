
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>

class CDoublyList;

typedef int Data;


class CQueue
{
public: 
	CQueue() : list() {}
	~CQueue() {}

protected:
	CDoublyList list;

public:
	void	Init	()	{};
	bool	IsEmpty	()	{ return list.Count() ? true : false; }
	int		Enqueue	(Data& arData, bool abTrace = false);
	int		Dequeue	(Data& arData, bool abTrace = false);
	int		Peek	(Data& arData, bool abTrace = false);
};

inline int
CQueue::Enqueue(Data& arData, bool abTrace)
{
	int ret = list.PushBack( arData );

	if ( abTrace )
		printf("Enqueue, data=%d, ret=%d\n", arData, ret);

	return ret;
}

inline int
CQueue::Dequeue(Data& arData, bool abTrace)
{
	int ret = list.PopFront(arData);

	if (abTrace)
		printf("Dequeue, data=%d, ret=%d\n", arData, ret);

	return ret;
}

inline int
CQueue::Peek(Data& arData, bool abTrace)
{
	int ret;
	
	ret = list.PopBack(arData);
	ret = list.PushBack(arData);

	if (abTrace)
		printf("Peek, data=%d, ret=%d\n", arData, ret);

	return ret;
}


#endif