
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "FileTest.h"
#include "Logger.h"

CFileTest::CFileTest()
{
	;
}

CFileTest::~CFileTest()
{
	;
}

int
CFileTest::Read(const char* aFilePath)
{
	int ret = -1;
	int fd 	= -1;
	
	const char* path = aFilePath;
	
	fd = open(path, O_RDONLY);
	if ( fd == -1 )
		return -1;
	
	enum 
	{
		eMaxLen = 256
	};
	
	char buf[eMaxLen] = {0,};
	
	ret = read(fd, buf, eMaxLen);
	if ( ret <= 0 )
		return ret;
	
	printf( "String read: %s\r\n# of characters read: %d\r\n", buf, ret );
	
	ret = close(fd);
	if ( ret != 0 )
	{
		g_Logger.Telemetry( __FILE__, __LINE__, "error=%d", ret);
		return ret;
	}
	
	return 0;
}

int 
CFileTest::Write(const char* aFilePath, const char* aString)
{
	int ret = -1;
	int fd 	= -1;
	int	flags = O_WRONLY | O_APPEND | O_CREAT;
	
	const char* path = aFilePath;
	
	fd = open(path, flags);
	if ( fd == -1 )
	{
		g_Logger.Telemetry( __FILE__, __LINE__, "fd=-1");
		return -1;
	}
		
	int len = strlen( aString );
		
	ret = write( fd, aString, len );
	if ( ret == -1 )
	{
		g_Logger.Telemetry( __FILE__, __LINE__, "error=%d", ret);		
		return ret;
	}	
		
	ret = close(fd);
	if ( ret != 0 )
	{
		g_Logger.Telemetry( __FILE__, __LINE__, "error=%d", ret);
		return ret;
	}	
		
	return 0;
}
