

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>

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
	int ret;
	
	int fd = open("/dev/helloworld", O_RDONLY );	
	printf("FirstDriver: %d=open@helloworld\n", fd );

	close( fd );
	
	// 1. Open myscull	
	int fd2 = open("/dev/myscull", O_RDONLY );
	printf("FirstDriver: %d=open@myscull\n", fd2 );	

	enum { eBUFF = 4 };
	int  i;
	char buf[eBUFF] = {0, };	
	
	// 2. Write something
	for ( i = 0; i < eBUFF; i++ )
	{
		buf[i] = i*2+1;
		printf("	buf[%2d]=%2d\n", i, (int)buf[i] );
	}

	ret = write( fd2, buf, eBUFF * sizeof(char) );
	printf("FirstDriver: write@myscull...done, count=%d\n", ret );	
	memset(buf, 0, sizeof(buf));	
	
	// 3. Read something
	ret = read( fd2, buf, eBUFF * sizeof(char) );
	for ( i = 0; i < eBUFF; i++ )
		printf("	buf[%2d]=%2d\n", i, (int)buf[i] );
	
	printf("FirstDriver: read@myscull...done, count=%d\n", ret );
	
	// 4. Close fd.
	close( fd2 );
}		
		
