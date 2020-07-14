
#ifndef _STRUCT_TEST_H_
#define _STRUCT_TEST_H_

typedef union uniStatus
{
	long ulStatus;
	struct
	{
		unsigned int bMoveStarted	: 1;
		unsigned int bMoveDone		: 1;
		unsigned int bAmpFault		: 1;
		unsigned int bReserved		: 29;
	};

}UniStatus;

typedef int (*HandleEvent) (void);

int HandleEventDefault	();
int HandleEventStep		();
int HandleEventServo	();

typedef struct stMotor
{
	long		type;
	UniStatus	status;
	long		cntCommand;
	long		cntActual;

	HandleEvent EventHandler;

	stMotor() { status.ulStatus = 0L; EventHandler = HandleEventDefault; }

	int	Move	(int pos);
	int	Stop	();

}StMotor;

#endif