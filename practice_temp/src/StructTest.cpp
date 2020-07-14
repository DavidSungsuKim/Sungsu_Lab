
#include <stdio.h>

#include "..//include/StructTest.h"

int 
HandleEventDefault() 
{ 
	printf("HandleEventDefault...done\n");
	return -1; 
}

int 
HandleEventStep() 
{ 
	printf("HandleEventStep...done\n");
	return 0; 
}

int 
HandleEventServo() 
{ 
	printf("HandleEventServo...done\n");
	return 0; 
}

int	
StMotor::Move(int pos)
{
	printf("Move...done\n");
	return 0;
}

int	
StMotor::Stop()
{
	printf("Stop...done\n");
	return 0;
}
