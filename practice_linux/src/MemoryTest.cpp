
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "MemoryTest.h"

CMemoryTest::CMemoryTest()
{
	
}

CMemoryTest::~CMemoryTest()
{
	
}

void 
CMemoryTest::AllocDynamicMemory()
{
	size_t	size = 10;

	char* 	pName1;
	char* 	pName2;
	char* 	pName3;		
	
	pName1 = (char*)malloc( size );	
	pName2 = (char*)malloc( size );	
	pName3 = (char*)malloc( size );	
	
	printf("AllocDynamicMemory: malloc()...\n");
	printf("pName1=0x%x\n", (int)pName1);
	printf("pName2=0x%x\n", (int)pName2);
	printf("pName3=0x%x\n", (int)pName3);
	
	char* 	pName1c;
	char* 	pName2c;
	char* 	pName3c;
	
	pName1c = (char*)calloc( size, sizeof(char) );	
	pName2c = (char*)calloc( size, sizeof(char) );	
	pName3c = (char*)calloc( size, sizeof(char) );	

	printf("AllocDynamicMemory: calloc()...\n");	
	printf("pName1c=0x%x\n", (int)pName1c);
	printf("pName2c=0x%x\n", (int)pName2c);
	printf("pName3c=0x%x\n", (int)pName3c);	
	
	free( pName1 );
	free( pName2 );
	free( pName3 );
	
	free( pName1c );
	free( pName2c );
	free( pName3c );
}

void	
CMemoryTest::AlignMem()
{
	int ret;
	
	int*	pNormal;	
	int*	pAligned;
	size_t	alignment	= 8 * sizeof(int);	// Note that alignment must be a power of 2 and be a multiple of the pointer
	size_t	size		= 8;

	int		rem;
	
	// Get memory
	pNormal	= (int*)malloc( size );	
	if ( !pNormal )
	{
		perror("malloc");
		return;
	}
	
	rem = (int)pNormal % alignment;
	printf("AlignMem: pNormal=0x%x, 	size=%d, rem=%d\n", (int)pNormal, size, rem );
	
	// Get aligned memory
	ret = posix_memalign( (void**)&pAligned, alignment, size );
	if ( ret )
	{
		fprintf( stderr, "posix_memalign: %s\n", strerror( ret ));	
		return;
	}
	
	// Check if it is aligned as intended
	rem = (int)pAligned % alignment;
	printf("AlignMem: pAlingned=0x%x, alignment=%d, size=%d rem=%d\n", (int)pAligned, alignment, size, rem );
	
	// Release them
	free( pNormal );
	free( pAligned );
}

void	
CMemoryTest::DataSegment()
{
	void* 	pBrk;
	int		increment	= 0;
	
	pBrk = sbrk( increment );
	
	printf("DataSegment: current breakpoint is %p\n", pBrk );
	
	pBrk = sbrk( 10 );
	printf("DataSegment: current breakpoint is %p\n", pBrk );
	
	pBrk = sbrk( -10 );
	printf("DataSegment: current breakpoint is %p\n", pBrk );
	
//	*(int*)pBrk = 1;	// Of course, not allowed.
}

void	
CMemoryTest::AnonymousMapping()
{
	int		ret;
	void* 	p;
	
	void*	pStart 	= NULL;	// which means it doesn't matter
	size_t	length	= 512 * 1024;
	int		prots	= PROT_READ | PROT_WRITE;
	int		flags	= MAP_ANONYMOUS | MAP_PRIVATE;
	int		fd		= -1; 	// If 'MAP_ANONYMOUS' is set, fd and offset are ignored.
	int		offset	= 0;
	
	// Get anonymous memory map
	p = mmap( pStart, length, prots, flags, fd, offset );
	if ( p == MAP_FAILED )
	{
		perror("mmap");
		return;
	}
	
	printf("AnonymousMapping: p=0x%x, length=%d\n", (int)p, length );
	
	// Check if it's really all zero in the memory obtained
	//	; There shouldn't be any value rather than zero since the memory is from pages pre-filled with zero.
	size_t i;
	char*	pByte = (char*)p;	
	for ( i = 0; i < length; i++ )
	{
		if ( *pByte != 0 )
		{
			printf("AnonymousMapping: not zero! 0x%x\n", (int)pByte );
			break;
		}
		
		pByte++;
	}

	pByte = (char*)p;	
	// Writing test
	for ( i = 0; i < length; i++ )
	{
		*pByte = (char)i;
		pByte++;
	}
	
	printf("AnonymousMapping: Writing test okay...\n");
	
	// Release the map
	ret = munmap( p, length );
	if ( ret )
		perror("mumap");
}

void	
CMemoryTest::UseDevZeroObsolete()
{
	int		ret;
	void* 	p;
	int		fd;
	
	fd = open( "/dev/zero", O_RDWR );
	if ( fd < 0 )
	{
		perror("open");
		return;
	}
	
	void*	pStart 	= NULL;
	size_t	length	= getpagesize();
	int		prots	= PROT_READ | PROT_WRITE;
	int		flags	= MAP_PRIVATE;
	int		offset	= 0;
	
	// Memory map from /dev/zero
	p = mmap( pStart, length, prots, flags, fd, offset );
	
	if ( p == MAP_FAILED )
	{
		perror("mmap");
		if ( close( fd ) )
			perror("close");
		return;
	}
	
	printf("UseDevZeroObsolete: p=0x%x, length=%d\n", (int)p, length );
	
	if ( close( fd ))
		perror("close");
	
	// Try to use something on the memory obtained.	
	size_t i;
	char*	pByte = (char*)p;	
	for ( i = 0; i < length; i++ )
	{
		if ( *pByte != 0 )
		{
			printf("UseDevZeroObsolete: not zero! 0x%x\n", (int)pByte );
			break;
		}
		
		pByte++;
	}

	pByte = (char*)p;	
	// Writing test
	for ( i = 0; i < length; i++ )
	{
		*pByte = (char)i;
		pByte++;
	}
	
	printf("UseDevZeroObsolete: Writing test okay...\n");
	
	// Release the map
	ret = munmap( p, length );
	if ( ret )
		perror("mumap");	
}
