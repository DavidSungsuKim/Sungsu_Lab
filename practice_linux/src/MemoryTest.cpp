
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>
#include <alloca.h>
#include <string.h>
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

	printf("pName1=0x%lx\n", (long)pName1);
	printf("pName2=0x%lx\n", (long)pName2);
	printf("pName3=0x%lx\n", (long)pName3);	

	char* 	pName1c;
	char* 	pName2c;
	char* 	pName3c;
	
	pName1c = (char*)calloc( size, sizeof(char) );	
	pName2c = (char*)calloc( size, sizeof(char) );	
	pName3c = (char*)calloc( size, sizeof(char) );	

	printf("AllocDynamicMemory: calloc()...\n");	

	printf("pName1c=0x%lx\n", (long)pName1c);
	printf("pName2c=0x%lx\n", (long)pName2c);
	printf("pName3c=0x%lx\n", (long)pName3c);	

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
	
	long*	pNormal;	
	long*	pAligned;
	size_t	alignment	= 8 * sizeof(int);	// Note that alignment must be a power of 2 and be a multiple of the pointer
	size_t	size		= 8;

	long	rem;
	
	// Get memory
	pNormal	= (long*)malloc( size );	
	if ( !pNormal )
	{
		perror("malloc");
		return;
	}
	
	rem = (long)pNormal % alignment;

#if ( __SIZEOF_LONG == 8 )
	printf("AlignMem: pNormal=0x%lx, 	size=%ld, rem=%ld\n", (long)pNormal, size, rem );
#else
	printf("AlignMem: pNormal=0x%lx, 	size=%d, rem=%ld\n", (long)pNormal, size, rem );
#endif

	// Get aligned memory
	ret = posix_memalign( (void**)&pAligned, alignment, size );
	if ( ret )
	{
		fprintf( stderr, "posix_memalign: %s\n", strerror( ret ));	
		return;
	}
	
	// Check if it is aligned as intended
	rem = (long)pAligned % alignment;
	
#if ( __SIZEOF_LONG == 8 )
	printf("AlignMem: pAlingned=0x%lx, alignment=%ld, size=%ld rem=%ld\n", (long)pAligned, alignment, size, rem );
#else	
	printf("AlignMem: pAlingned=0x%lx, alignment=%d, size=%d rem=%ld\n", (long)pAligned, alignment, size, rem );
#endif
	
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
	
#if ( __SIZEOF_LONG__ == 8 )	
	printf("AnonymousMapping: p=0x%lx, length=%ld\n", (long)p, length );
#else
	printf("AnonymousMapping: p=0x%lx, length=%d\n", (long)p, length );
#endif
	
	// Check if it's really all zero in the memory obtained
	//	; There shouldn't be any value rather than zero since the memory is from pages pre-filled with zero.
	size_t i;
	char*	pByte = (char*)p;	
	for ( i = 0; i < length; i++ )
	{
		if ( *pByte != 0 )
		{
			printf("AnonymousMapping: not zero! 0x%lx\n", (long)pByte );
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
	
#if ( __SIZEOF_LONG__ == 8 )	
	printf("UseDevZeroObsolete: p=0x%lx, length=%ld\n", (long)p, length );
#else
	printf("UseDevZeroObsolete: p=0x%lx, length=%d\n", (long)p, length );
#endif
	
	if ( close( fd ))
		perror("close");
	
	// Try to use something on the memory obtained.	
	size_t i;
	char*	pByte = (char*)p;	
	for ( i = 0; i < length; i++ )
	{
		if ( *pByte != 0 )
		{
			printf("UseDevZeroObsolete: not zero! 0x%lx\n", (long)pByte );
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

void	
CMemoryTest::AdvancedMemoryAlloc()
{
	int		ret;
	size_t	len = 21;
	size_t	size;
	
	char* 	buf;
	
	buf = (char*)malloc( len );
	
	if ( !buf )
	{
		perror("malloc");
		return;
	}
	
	// Get the actual memory size	
	size = malloc_usable_size( buf );
	
#if ( __SIZEOF_LONG__ == 8 )
	printf("AdvancedMemoryAlloc: alloc=%ld byte, actual size alloc=%ld\n", len, size );
#else
	printf("AdvancedMemoryAlloc: alloc=%d byte, actual size alloc=%d\n", len, size );
#endif
	
	buf[size-1] = 'A';
	
#if ( __SIZEOF_LONG__ == 8 )
	printf("AdvancedMemoryAlloc: buf[%ld]=%c\n", size-1, buf[size-1] );
#else
	printf("AdvancedMemoryAlloc: buf[%d]=%c\n", size-1, buf[size-1] );
#endif

	// Try to trim the memory
	int padding = M_TOP_PAD;
	ret = malloc_trim( padding ); 	
	if ( ret != 1 )
		perror("malloc_trim");

	size = malloc_usable_size( buf );
	
#if ( __SIZEOF_LONG__ == 8 )		
	printf("AdvancedMemoryAlloc: alloc=%ld byte, actual size trimed=%ld\n", len, size );	
#else
	printf("AdvancedMemoryAlloc: alloc=%d byte, actual size trimed=%d\n", len, size );	
#endif

	// Statistics information
	struct mallinfo info;
	
	info = mallinfo();
	printf("AdvancedMemoryAlloc: mallinfo()\n");
	printf("	arena   =%d\n", info.arena );		// data segment size that malloc uses
	printf("	ordblks =%d\n", info.ordblks );		// the number of vacant memory blocks
	printf("	smblks  =%d\n", info.smblks );		// the number of fast beans
	printf("	hblks   =%d\n", info.hblks );		// the number of anonymous mapping
	printf("	hblkhd  =%d\n", info.hblkhd );		// the size of anonymous mapping
	printf("	usmblks =%d\n", info.usmblks );		// the maximum size allocatable
	printf("	fsmblks =%d\n", info.fsmblks );		// the size of fast bean usable
	printf("	uordblks=%d\n", info.uordblks );	// the overall size of memory allocated
	printf("	fordblks=%d\n", info.fordblks );	// the size of memory block available
	printf("	keepcost=%d\n", info.keepcost );	// the size of memory removable
	
	// Print emory statistics infomation through stderr.
	malloc_stats();	
		
	free( buf );
}

void	
CMemoryTest::AllocBasedOnStack( int a )
{
	int x = 2;
	int y = 3;
	
	// Get memory from alloca() and compare addresses.
	size_t size = sizeof(int);
	int* p = (int*)alloca( size );	// 'p' MUST NOT be freed by users; it's automatically freed as stack works.
	*p = 4;							// And note that getting too much memory from stack can cause stack overflow.
	
	int* m = (int*)malloc( size );
	*m = 5;
	
	printf("AllocBasedOnStack: a=0x%lx\n", (long)&a );
	printf("AllocBasedOnStack: x=0x%lx\n", (long)&x );
	printf("AllocBasedOnStack: y=0x%lx\n", (long)&y );
	printf("AllocBasedOnStack: p=0x%lx\n", (long)p );
	printf("AllocBasedOnStack: m=0x%lx\n", (long)m );	
	
	// Working of memory by alloca().
	// Although it's made from stack, it doesn't disapear getting out of a loop; it works as if that from malloc().
	int i;
	int* pp[5];
	for ( i = 0; i < 5; i++ )
	{
		pp[i] = (int*)alloca( size );
		*pp[i] = i;
	}

	for ( i = 0; i < 5; i++ )	
		printf("AllocBasedOnStack: pp[i]=%d\n", *pp[i] );
	
	
	free( m );	
}

void	
CMemoryTest::ManipulateMem()
{
	// memset
	printf("ManipulateMem: memset()...\n" );
	{
		char buf[256];
	
		int		c 	= '\0'; // the same as =0
		size_t	n	= 256;
		
		memset( buf, c, n );
	}
	
	// Comparing bytes
	printf("ManipulateMem: Comparing bytes...\n" );
	{
		int		ret;
		
		char 	buf1[] 	= "David Kim";
		char 	buf2[] 	= "David Lee";
		size_t	n 		= 5;
		
		ret = memcmp( buf1, buf2, n );
		if ( ret == 0 )
#if ( __SIZEOF_LONG__ == 8 )				
			printf("ManipulateMem: same the first n=%ld bytes\n", n );
#else
			printf("ManipulateMem: same the first n=%d bytes\n", n );			
#endif
		else
			printf("ManipulateMem: diff\n");
			
		// Note that comparing two structures may not be safe because of padding the compiler adds; Don't use.
	}

	// Moving bytes
	printf("ManipulateMem: Moving bytes...\n" );
	{
		char 	buf1[] 	= "David Kim";
		char 	buf2[] 	= "Dabid Lee";
		
		printf("ManipulateMem: %s, %s\n", buf1, buf2 );
		
		void* pDst	= buf2;
		void* pSrc	= buf1;
		size_t	n	= 5;
		
		memmove( pDst, pSrc, n );
	
	/*	memcpy() works is basically the same as memmove() 
	 * 	except it doesn't gurantee the result in case dst and src are the same.
	 * 	memcpy() is known a bit faster than memmove().
	 */
		
		printf("ManipulateMem: %s, %s\n", buf1, buf2 );
	}
	
	// Moving bytes checking a character.
	printf("ManipulateMem: Moving bytes checking a character...\n" );
	{
		char 	buf1[] 	= "David Kim";
		char 	buf2[] 	= "Dabid Lee";
		
		printf("ManipulateMem: %s, %s\n", buf1, buf2 );
		
		void* pDst	= buf2;
		void* pSrc	= buf1;
		size_t	n	= 5;
		int		c	= 'i';
		
		memccpy( pDst, pSrc, c, n );
	
		// Why not stopping..?
		printf("ManipulateMem: %s, %s\n", buf1, buf2 );
	}
	
	// Searching for bytes (useful!)
	printf("ManipulateMem: Searching for bytes...\n" );	
	{
		char name[] = "David Kim";
		
		char* pFound;
		pFound = (char*)memchr( name, 'i', sizeof(name) );
		if ( pFound )
			printf("ManipulateMem: memchr(), pFound=%c\n", *pFound );
			
		pFound = (char*)memrchr( name, 'i', sizeof(name) );
		if ( pFound )
			printf("ManipulateMem: memrchr(), pFound=%c\n", *pFound );		
	}
	
	// Searching for byte block (useful!)
	printf("ManipulateMem: Searching byte block...\n" );
	{
		char  name[] 	= "David Kim an nyoung ha se yo";
		char  needle[] 	= "se";
		char* pFound;
		
#if ( __SIZEOF__LONG == 8 )
		printf("ManipulateMem: sizeof(name)=%ld sizeof(needle)=%ld\n", sizeof(name), sizeof(needle));
		printf("ManipulateMem: strlen(name)=%ld strlen(needle)=%ld\n", strlen(name), strlen(needle));
#else
		printf("ManipulateMem: sizeof(name)=%d sizeof(needle)=%d\n", sizeof(name), sizeof(needle));
		printf("ManipulateMem: strlen(name)=%d strlen(needle)=%d\n", strlen(name), strlen(needle));
#endif
		
	/*	Note that here I have to use 'strlen(needle)' or 'sizeof(needle)-1'
	 * 	since needle contains '\0' at the end of it.
	 * */
		pFound = (char*)memmem( name, strlen(name), needle, strlen(needle));
		if ( pFound )
			printf("ManipulateMem: memmem(), pFound=%s\n", pFound);
	}
}
