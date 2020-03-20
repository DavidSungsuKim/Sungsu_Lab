
#include "Logger.h"

#include <string>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

CLogger g_Logger;

CLogger::CLogger()
{
	m_fdLogFile = STDOUT_FILENO;
}

CLogger::~CLogger()
{
	if ( STDOUT_FILENO != m_fdLogFile )
	{
	//	printf( "fd=%d in the deconstructor\r\n", m_fdLogFile );
		fdatasync( m_fdLogFile ); // To syncronize writing into the file
		
		int ret = close( m_fdLogFile );
		if ( ret == -1 )
			perror("close");
	}
}

void		
CLogger::SetLogFilePath( const char* aFile )
{
	int fd 			= STDOUT_FILENO;
	int	flags 		= ( O_RDWR | O_APPEND | O_CREAT );
	mode_t	mode 	= S_IRWXU | S_IRWXG;
	
	const char* path = aFile;
	
	fd = open( path, flags, mode );
	
	g_Logger.Telemetry2( __FILE__, __LINE__, "fd=%d", fd );
	
	if ( fd == -1 )
		fd = STDOUT_FILENO;

	m_fdLogFile = fd;
}

void
CLogger::Telemetry( const char* aString, ...)
{ 
	std::string message;
	
	// To make use of variable-length argument lists
    char buf[256] = {0,}; // This is a MUST to avoid repeating strings.
	va_list ap;
	
    va_start(ap, aString);
    vsprintf(buf + strlen(buf), aString, ap);
    va_end(ap);
	
// 	puts(buf);
	
	message = buf;
	message += "\r\n";
	
	write( m_fdLogFile, message.c_str(), strlen( message.c_str()) );

	return;
}

void
CLogger::Telemetry2( const char* aFile, int aLineNo, const char* aString, ...)
{ 
	std::string message;
	
	// Attach file and line info first.
	char bufInfo[256] = {0,};
	sprintf( bufInfo,"%s L:%d ", aFile, aLineNo );	
	message = bufInfo;
	
	// To make use of variable-length argument lists
    char buf[256] = {0,}; // This is a MUST to avoid repeating strings.
	va_list ap;
	
    va_start(ap, aString);
    vsprintf(buf + strlen(buf), aString, ap);
    va_end(ap);
	
// 	puts(buf);
	
	message += buf;
	message += "\r\n";
	
	write( m_fdLogFile, message.c_str(), strlen( message.c_str()) );

	return;
}

