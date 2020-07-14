
#ifndef _DESIGN_PATTERN_H_
#define _DESIGN_PATTERN_H_

#include <stdio.h>

//////////////////////////////////////////////////////////////////////////
class Singleton
{
public:
	static Singleton* Get()
	{
		if ( m_instance == NULL )
			m_instance = new Singleton;
		return m_instance;
	}

	void Test( );

protected:
	Singleton() {}

private:
	static Singleton* m_instance;
};
//////////////////////////////////////////////////////////////////////////

class CApplication
{
	int data;
};

class Command
{
public:
	Command () {}
	virtual ~Command() {}

public:
	void Execute() {}
};

class OpenCommand : public Command
{
public:
	OpenCommand(CApplication *apApp);
};


//////////////////////////////////////////////////////////////////////////
enum eState
{
	eState1 = 0,
	eState2,
	eState3,
	eStateMax
};

void FuncState1(void);
void FuncState2(void);
void FuncState3(void);

//////// State Pattern 1

void StatePatternC1 ();

//////// State Pattern 2

typedef void FuncState(void);
typedef struct stState
{
	int			state;
	FuncState*	pFunc;
}StState;

typedef struct stFuncPool
{
	FuncState* list[eStateMax] = 
	{
		FuncState1, 
		FuncState2, 
		FuncState3 
	};
}StFuncPool;

void StatePatternC2	(void);

//////// State Pattern 3
enum 
{
	eEvent1	= 0,
	eEvent2,
	eEvent3,
	eMaxEvent = eEvent3 + 1
};

typedef void (*Func)(void);
typedef struct stFuncTable
{
	Func	pfunc;
	int		stateNext;

}StFuncTable;

void InitStateFunc	(void);

void StatePatternC3 (void);


#endif