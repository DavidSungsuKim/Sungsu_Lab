
#include <stdlib.h>
#include <windows.h>
#include <assert.h>

#include "..//include/DesignPattern.h"

Singleton* Singleton::m_instance = NULL;

void 
Singleton::Test()
{
	printf("Hi, I am a Singleton.\n");
}


int g_stateTask = eState1;

void StateTask();
void StateTask()
{
	switch (g_stateTask)
	{
		case eState1:

			FuncState1();
			g_stateTask = eState2;
			break;

		case eState2:

			FuncState2();
			g_stateTask = eState3;
			break;

		case eState3:

			FuncState3();
			g_stateTask = eState1;
			break;

		default:
			break;
	}
}

void StatePatternC1()
{
	long tickStart = GetTickCount();

	while (1)
	{
		StateTask();

		long timeElapsedms = GetTickCount() - tickStart;
		printf("timeElapsed=%.3f[ms]\n", (double)timeElapsedms / 1000.0);

		if (timeElapsedms > 10000)
		{
			printf("finished...\n");
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
StState g_States[eStateMax];
StState g_State;

void InitState()
{
	StFuncPool fPool;
	int i;
	for (i = 0; i < eStateMax; i++)
	{
		g_States[i].state	= i;
		g_States[i].pFunc	= fPool.list[i];
	}
}

void FuncState1(void)
{
	printf("FuncState1\n");

	Sleep(1000);
	g_State = g_States[eState2];
}

void FuncState2(void)
{
	printf("FuncState2\n");

	Sleep(1000);
	g_State = g_States[eState3];
}

void FuncState3(void)
{
	printf("FuncState3\n");

	Sleep(1000);
	g_State = g_States[eState1];
}

void StatePatternC2(void)
{
	long tickStart = GetTickCount();

	InitState();
	g_State = g_States[eState1];

	while (1)
	{
		g_State.pFunc();

		long timeElapsedms = GetTickCount() - tickStart;
		printf("timeElapsed=%.3f[ms]\n", (double)timeElapsedms/1000.0);

		if ( timeElapsedms > 10000 )
		{
			printf("finished...\n");
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

StFuncTable g_StateTbl[eStateMax][eMaxEvent];

void InitStateFunc(void)
{
	// Initialize all pfunc as NULL.
	int i,j;
	for(i = 0; i < eStateMax; i++)
		for (j = 0; j < eMaxEvent; j++)
			g_StateTbl[i][j].pfunc = NULL;

	// Make relations between state transitions by assigning function pointers and the state.
	g_StateTbl[eState1][eEvent1].pfunc		= FuncState1;
	g_StateTbl[eState1][eEvent1].stateNext	= eState2;

	g_StateTbl[eState2][eEvent2].pfunc		= FuncState2;
	g_StateTbl[eState2][eEvent2].stateNext	= eState3;
}

void StatePatternC3(void)
{
	Func pFunc;

	int state = eState1;
	int	stateNext;
	int event = eEvent1;

	InitStateFunc();

	for (;;)
	{
		// Simulate event
		if ( event++ >= eMaxEvent )
			event = eEvent1;

		// Here g_state and g_event pair must exist.
		pFunc = g_StateTbl[state][event].pfunc;
		
		// Which means there is no state transition defined on this event with the current state.
		if (pFunc == NULL)
			continue;	

		stateNext = g_StateTbl[state][event].stateNext;

		pFunc();

		state = stateNext;
	
		Sleep(1);
	}
}