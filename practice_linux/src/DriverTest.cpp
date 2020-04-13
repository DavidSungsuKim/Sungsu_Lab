

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "DriverTest.h"

CDriverTest::CDriverTest()
{
	
}

CDriverTest::~CDriverTest()
{
	
}

void
CDriverTest::FirstDriver()
{
	int fd = open("/dev/helloworld", O_RDONLY );	
	printf("FirstDriver: %d=helloworld()\n", fd );
	
//	int fd2 = open("/dev/virtual_device", O_RDWR );
//	printf("FirstDriver: %d=virtual_device()\n", fd2 );	
}		
		
