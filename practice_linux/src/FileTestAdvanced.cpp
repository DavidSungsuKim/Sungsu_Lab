
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>

#include <sys/epoll.h>

#include "FileTestAdvanced.h"

CFileTestAdv::CFileTestAdv()
{
	
}

CFileTestAdv::~CFileTestAdv()
{
	
}

void 
CFileTestAdv::WriteVector()
{
	struct iovec iov[3];
	ssize_t	nr;
	int fd, i;
	
	const char *buf[] = 
	{
		"David Sungsu Kim1",
		"David Sungsu Kim2",
		"David Sungsu Kim3"
	};
	
	fd = open( "testAdvancedFile.txt", (O_WRONLY | O_CREAT | O_TRUNC) );
	if ( fd == -1 )
	{
		perror("open");
		return;		
	}
	
	for ( i = 0; i < 3; i++ )
	{
		iov[i].iov_base	= (char*)buf[i];
		iov[i].iov_len	= strlen(buf[i]) + 1;	// Why do I need to add '1'?
												// ==> Try removing it and see what happens.
	}
	
	nr = writev(fd, iov, 3);
	if ( nr == -1 )
	{
		perror("writev");
		return;
	}
	printf("WriteVector: word %d bytes\n", nr);
	
	if ( close(fd) )
		perror("close");
	
	return;
}

void
CFileTestAdv::ReadVector()
{
	int i;
	char buf[3][18];
	for ( i = 0; i < 3; i++ )
		memset(buf[i], 0, sizeof(buf[i]));			
			
	struct iovec iov[3];
	ssize_t	nr;
	int fd;
	
	fd = open( "testAdvancedFile.txt", O_RDONLY );
	if ( fd == -1 )
	{
		perror("open");
		return;
	}
	
	for ( i = 0; i < 3; i++ )
	{
		iov[i].iov_base	= buf[i];
		iov[i].iov_len	= sizeof(buf[i]);
	}
	
	nr = readv( fd, iov, 3 );
	if ( nr == -1 )
	{
		perror("readv");
		return;
	}
	
	for ( i = 0; i < 3; i++ )
		printf("ReadVector: %s\n", (char*)iov[i].iov_base );
	
	if ( close(fd) )
		perror("close");
			
	return;
}

void	
CFileTestAdv::Epoll()
{
#define MAX_EVENTS 64
	
	int ret = 0;
	
	int epfd;
	int flags = EPOLL_CLOEXEC;
	
	epfd = epoll_create1( flags );
	if ( epfd < 0 )
	{
		perror("epoll_create1");
		return;
	}
	
	int	fd = STDIN_FILENO;	// Try fd for another type of file like..pipe?
	
/*	epoll() is not for regular files. 
 * 	fd = open("testEpoll.txt", O_RDONLY );
	if ( !fd )
	{
		perror("open");
		return;
	}*/
	
	int op;
	struct epoll_event event;
	
	op	= EPOLL_CTL_ADD;
/*	EPOLL_CTL_ADD
 * 	EPOLL_CTL_DEL
 * 	EPOLL_CTL_MOD
 */	
	event.data.fd 	= fd;
	event.events 	= EPOLLIN | EPOLLOUT;
	
	ret = epoll_ctl( epfd, op, fd, &event );
	if ( ret )
	{
		perror("epoll_ctl");
		return;
	}
	
	struct epoll_event events[ MAX_EVENTS ];
	int	timeout = -1; // Infinite
	int nr_events;
	
	nr_events = epoll_wait( epfd, events, MAX_EVENTS, timeout );
	if ( nr_events < 0 )
	{
		perror("epoll_wait");
		return;
	}
	
	int i;
	for ( i = 0; i < nr_events; i++ )
	{
		printf("i=%d event=%ld on fd=%d\n",
				i,
				(long)events[i].events, 
				events[i].data.fd );
	}
	
	if ( close( epfd ) )
		perror("close");
	
	return;
}
