
#include "Common.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>

#include <sys/epoll.h>

#include <sys/mman.h>

#include <sys/ioctl.h>
#include <linux/fs.h>

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

#ifdef _LONG_8BYTE__
	printf("WriteVector: word %ld bytes\n", nr);
#else
	printf("WriteVector: word %d bytes\n", nr);
#endif
	
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
	int logical_events;
	
	logical_events = epoll_wait( epfd, events, MAX_EVENTS, timeout );
	if ( logical_events < 0 )
	{
		perror("epoll_wait");
		return;
	}
	
	int i;
	for ( i = 0; i < logical_events; i++ )
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

void	
CFileTestAdv::MemoryMappedRead()
{
	int  ret;

	long pagesize = sysconf(_SC_PAGESIZE);
	printf("MemoryMappedRead: page size=%ld[byte]\n", pagesize );
	
	int fd;
	fd = open("testMemoryMappedRead.txt", O_RDONLY );
	if ( fd == -1 )
	{
		perror("open");
		return;
	}
		
	struct stat sb;
	ret = fstat( fd, &sb );
	if ( ret == -1 )
	{
		perror("fstat");
		return;
	}
	
	printf("MemoryMappedRead: sb.st_mode=%d\n", sb.st_mode);
	printf("MemoryMappedRead: sb.st_size=%ld\n", sb.st_size);
	
	void* 	pFile;	
	void* 	pAddrDesired	= 0; 			// 0 means it has no preferences
	size_t	len				= sb.st_size;
	int		prot			= PROT_READ;	// PROT_NONE, PROT_READ, PROT_WRITE, or PROT_EXEC
	int		flags			= MAP_SHARED;	// MAP_SHARED or MAP_PRIVATE
	off_t	offset			= 0;			// It should be multiple of the page size in the system
	
	pFile = mmap( pAddrDesired, len, prot, flags, fd, offset );
	if ( pFile == MAP_FAILED )
	{
		perror("mmap");
		close( fd );
		return;
	}
	
	printf("%s\n", (char*)pFile);
}

void	
CFileTestAdv::MemoryMappedWrite()
{
	int  ret;

	long pagesize = sysconf(_SC_PAGESIZE);
	printf("MemoryMappedWrite: page size=%ld[byte]\n", pagesize );
	
	// 1. Open a file.
	int fd;
	fd = open("testMemoryMappedWrite.txt", O_RDWR );
	if ( fd == -1 )
	{
		perror("open");	// This can happen if it has no proper permissions.
		return;
	}
	
	struct stat sb;
	ret = fstat( fd, &sb );
	if ( ret == -1 )
	{
		perror("fstat");
		return;
	}
	
	printf("MemoryMappedWrite: sb.st_mode=%d\n", sb.st_mode);
	printf("MemoryMappedWrite: sb.st_size=%ld\n", sb.st_size);
	
	// 2. Open a map for the file.
	void* 	pFile;	
	void* 	pAddrDesired	= 0; 						// 0 means it has no preferences
	size_t	len				= pagesize;					// It doesn't seem to affect when writing. Why?
	int		prot			= PROT_READ | PROT_WRITE;	// PROT_NONE, PROT_READ, PROT_WRITE, or PROT_EXEC
	int		flags			= MAP_SHARED;				// MAP_SHARED or MAP_PRIVATE
	off_t	offset			= 0;						// It should be multiple of the page size in the system
	
	pFile = mmap( pAddrDesired, len, prot, flags, fd, offset );
	if ( pFile == MAP_FAILED )
	{
		perror("mmap");
		close( fd );
		return;
	}
	
	close( fd );										// The file may be closed after getting a proper map regarding it.
	
#ifdef USE_MADVISE // not mandatory
	printf("MemoryMappedWrite: call madvise()\n");

	int advice = MADV_NORMAL;	
/*	MADV_NORMAL	
 * 	MADV_RANDOM
 *	MADV_SEQUENTIAL	: addresses in the ascending order
 * 	MADV_WILLNEED
 * 	MADV_DONTNEED	: reasonable for applications like video streaming where data is not used anymore after being displayed once.
 */ 
	ret = madvise( pFile, len, advice );				// if len=0, it supposes that the advice is applied to all memory region starting from pFile.
	if ( ret == -1 )
		perror("madvice");
#endif	
	
	// 3. Test writing into file via mapping
	printf("MemoryMappedWrite: Test Writing...\n");
		
	const char* strTest = "Hello. I live in Canada. This sentence will overwrite some of the content already existing.\n";
	int			strLen 	= strlen( strTest );
	
	char* 	pChar = (char*)pFile;
	int 	i;
	for ( i = 0; i < strLen; i++ )
		*pChar++ = *strTest++;

#ifdef USE_MSYNC // not mandatory
	printf("MemoryMappedWrite: call msync()\n");
	
	int flagsSync = MS_SYNC; // MS_SYNC, MS_ASYNC, or MS_INVALIDATE
	ret = msync( pFile, len, flagsSync );
	if ( ret == -1 )
		perror("msync");
#endif

	printf("%s\n", (char*)pFile);						// Here I can see overwritten strings in the file.

	munmap( pFile, len );
}

void	
CFileTestAdv::GetFileINode(int argc, const char* argv[])
{
	if ( argc < 2 )
		return;
	
	const char* pFilePath = argv[1];
			
	int fd;
	
	fd = open( pFilePath, O_RDONLY );
	if ( fd < 0 )
	{
		perror("open");
		return;
	}
	
	int 	ret;
	long	inode;	
	struct stat buf;
	
	ret = fstat( fd, &buf );
	if ( ret < 0 )
	{
		perror("fstat");
		return;
	}
	
	inode = buf.st_ino;
	
	printf("GetFileINode: FileName=%s inode=%ld\n", pFilePath, inode );
	return;
}

void	
CFileTestAdv::SortFilebyPhysicalBlock(int argc, const char* argv[])
{
	if ( argc < 2 )
		return;
	
	const char* pFilePath = argv[1];
			
	int fd;
	
	fd = open( pFilePath, O_RDONLY );
	if ( fd < 0 )
	{
		perror("open");
		return;
	}
	
	PrintBlocks(fd);
	return;	
}

void	
CFileTestAdv::PrintBlocks(int fd)
{
	int logical_blocks, i;
	
	logical_blocks = GetNrBlocks( fd );
	if ( logical_blocks < 0 )
	{
		fprintf( stderr, "GetNrBlocks failed!\n");
		return;
	}

	printf("PrintBlocks: %d logical blocks\n", logical_blocks);
	
	if ( logical_blocks == 0 )
		return;
		
	for( i = 0; i < logical_blocks; i++ )
	{
		int phys_block;
		
		phys_block = GetBlock( fd, i );
		if ( phys_block < 0 )
		{
			fprintf( stderr, "GetBlock failed!\n");
			return;
		}
		
		if ( !phys_block )
			continue;
			
		printf("i=%d, physical_block=%d\n", i, phys_block );
	}
	
	putchar('\n');	
}

int		
CFileTestAdv::GetBlock(int fd, int logical_block)
{
	int ret;
	
	// This ioctl function requires 'sudo'
	ret = ioctl( fd, FIBMAP, &logical_block );
	if ( ret < 0 )
	{
		perror("ioctl");
		return -1;
	}
	
	return logical_block;	// physical block
}

int		
CFileTestAdv::GetNrBlocks(int fd)
{
	int ret;
	struct stat buf;
	
	ret = fstat( fd, &buf );
	if ( ret < 0 )
	{
		perror("fstat");
		return -1;
	}
	
	return buf.st_blocks;
}
