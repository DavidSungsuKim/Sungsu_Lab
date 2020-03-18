
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

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
	
//	g_Logger.Telemetry( "String read: %s\r\n# of characters read: %d\r\n", buf, ret );
	
	ret = close(fd);
	if ( ret != 0 )
	{
		g_Logger.Telemetry2( __FILE__, __LINE__, "error=%d", ret);
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
		g_Logger.Telemetry2( __FILE__, __LINE__, "fd=-1");
		return -1;
	}
		
	int len = strlen( aString );
		
	ret = write( fd, aString, len );
	if ( ret == -1 )
	{
		g_Logger.Telemetry2( __FILE__, __LINE__, "error=%d", ret);		
		return ret;
	}	
		
	ret = close(fd);
	if ( ret != 0 )
	{
		g_Logger.Telemetry2( __FILE__, __LINE__, "error=%d", ret);
		return ret;
	}	
		
	return 0;
}

int	
CFileTest::Select()
{
#define	TIMEOUT 5
#define	BUF_LEN	1024
	
	struct 	timeval tv;
	fd_set	readfds;
	int		ret = 0;
	
	const int	fdStdIn = STDIN_FILENO;
	int			fd;
	fd = open("/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testSelectRead.txt", O_RDONLY);

	FD_ZERO( &readfds );
	FD_SET( fdStdIn, 	&readfds );
	if ( fd != -1 )
		FD_SET( fd,		&readfds );
	else
	{
		fd = fdStdIn;
		g_Logger.Telemetry2( __FILE__, __LINE__, "%d=open()", fd);
	}
	
	tv.tv_sec	= TIMEOUT;
	tv.tv_usec	= 0;
	
	ret = select( fd+1, &readfds, NULL, NULL, &tv );
	if ( ret == -1 )
		g_Logger.Telemetry2( __FILE__, __LINE__, "error. %d=select()", ret);
	if ( ret == 0 )
	{
		g_Logger.Telemetry2( __FILE__, __LINE__, "timeout:elapsed=%dsec", TIMEOUT );
		if ( fd != STDIN_FILENO )
		{
			close( fd );
			return -1;
		}
	}
			
	int fdWait = -1;
	
	if ( FD_ISSET( fdStdIn, &readfds ) )
		fdWait = fdStdIn;
	else if ( FD_ISSET( fd, &readfds ) )
		fdWait = fd;
	
	if ( fdWait != -1 )
	{
		char 	buf[ BUF_LEN ] = {0,};
		int		len;		
		
		len = read( fdWait, buf, BUF_LEN );
	
		g_Logger.Telemetry( "fd=%d, len=%d String:%s\r\n", fdWait, len, buf  );
	
		if ( fdWait != STDIN_FILENO )
			close( fdWait );
	}
	
	return ret;
}

int 
CFileTest::Poll()
{
	struct 	pollfd fds[2];
	int		ret;
	
	enum
	{
		eIndex0 = 0,
		eIndex1 = 1
	};
	
	fds[eIndex0].fd		= STDIN_FILENO;
	fds[eIndex0].events	= POLLIN;
	
	fds[eIndex1].fd		= STDOUT_FILENO;
	fds[eIndex1].events	= POLLOUT;

	enum { eTIMEOUT = 10 };
	
	ret = poll( fds, 2, eTIMEOUT*1000 );
	if ( ret == -1 )
	{
		perror("poll");
		return -1;
	}	
	
	if ( !ret ) 
	{
		g_Logger.Telemetry2( __FILE__, __LINE__, "timeout");
	}
	
	if ( fds[eIndex0].revents & POLLIN )
		g_Logger.Telemetry2( __FILE__, __LINE__, "stdin is readable" );

	if ( fds[eIndex1].revents & POLLOUT )
		g_Logger.Telemetry2( __FILE__, __LINE__, "stdout is writable");
			
	return ret;
}














