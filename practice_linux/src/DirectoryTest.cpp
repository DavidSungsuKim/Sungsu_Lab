
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <grp.h>
#include <dirent.h>
#include <errno.h> 
#include <string>

#include "DirectoryTest.h"

extern int errno;

CDirectoryTest::CDirectoryTest()
{
	
}

CDirectoryTest::~CDirectoryTest()
{
	
}

void
CDirectoryTest::Stat(int argc, const char* argv[])
{	
	struct stat sb;
/*	dev_t st_dev            Device ID of device containing file.
	ino_t st_ino            File serial number.
	mode_t st_mode          Mode of file (see below).
	nlink_t st_nlink        Number of hard links to the file.
	uid_t st_uid            User ID of file.
	gid_t st_gid            Group ID of file.
	dev_t st_rdev           Device ID (if file is character or block special).
	off_t st_size           For regular files, the file size in bytes.
						   For symbolic links, the length in bytes of the
						   pathname contained in the symbolic link.
						   For a shared memory object, the length in bytes.
						   For a typed memory object, the length in bytes.
						   For other file types, the use of this field is
						   unspecified.
	struct timespec st_atim Last data access timestamp.
	struct timespec st_mtim Last data modification timestamp.
	struct timespec st_ctim Last file status change timestamp.
	blksize_t st_blksize    A file system-specific preferred I/O block size
						   for this object. In some file system types, this
						   may vary from file to file.
	blkcnt_t st_blocks      Number of blocks allocated for this object.
	*/
	
	int ret;
	
	if ( argc < 2 )
		return;
	
	// Get file's stat.	
	const char* pFilePath = argv[1];	
	ret = stat( pFilePath, &sb );
	if ( ret == -1 )
	{
		perror("stat");
		return;
	}
	
	// Check files basic information
	printf("Stat: %s, size=%ld bytes\n", pFilePath, sb.st_size );
	printf("Stat: %s, inode=%ld bytes\n", pFilePath, sb.st_ino );

	// Check file's type
	std::string strFileType;
	switch( sb.st_mode & S_IFMT )
	{
		case S_IFBLK:	strFileType = "block device"; 		break;
		case S_IFCHR:	strFileType = "character device"; 	break;
		case S_IFDIR:	strFileType = "directory"; 			break;
		case S_IFIFO:	strFileType = "FIFO"; 				break;
		case S_IFLNK:	strFileType = "symbolic link"; 		break;
		case S_IFREG:	strFileType = "regular file"; 		break;
		case S_IFSOCK:	strFileType = "socket";				break;		
		default:		strFileType = "unknown"; 			break;
	}
	
	printf("Stat: %s, file type=%s\n", pFilePath, strFileType.c_str() );
	
	// Check is the file is on physical device
	ret = 
	printf("Stat: %s, Is on physical device?:%s\n", pFilePath, ( gnu_dev_major( sb.st_dev ) > 0 ) ? " yes" : " no" );
	
//	printf("Stat: %s, size=%ld bytes\n", pFilePath, sb.st_size );

	return; 
}

void	
CDirectoryTest::ChangePermission(int argc, const char* argv[])
{
/* 	From : https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html
 * 	These symbolic constants are defined for the file mode bits that control access permission for the file:
	S_IRUSR
	S_IREAD
	Read permission bit for the owner of the file. On many systems this bit is 0400. S_IREAD is an obsolete synonym provided for BSD compatibility.

	S_IWUSR
	S_IWRITE
	Write permission bit for the owner of the file. Usually 0200. S_IWRITE is an obsolete synonym provided for BSD compatibility.

	S_IXUSR
	S_IEXEC
	Execute (for ordinary files) or search (for directories) permission bit for the owner of the file. Usually 0100. S_IEXEC is an obsolete synonym provided for BSD compatibility.

	S_IRWXU
	This is equivalent to ‘(S_IRUSR | S_IWUSR | S_IXUSR)’.

	S_IRGRP
	Read permission bit for the group owner of the file. Usually 040.

	S_IWGRP
	Write permission bit for the group owner of the file. Usually 020.

	S_IXGRP
	Execute or search permission bit for the group owner of the file. Usually 010.

	S_IRWXG
	This is equivalent to ‘(S_IRGRP | S_IWGRP | S_IXGRP)’.

	S_IROTH
	Read permission bit for other users. Usually 04.

	S_IWOTH
	Write permission bit for other users. Usually 02.

	S_IXOTH
	Execute or search permission bit for other users. Usually 01.

	S_IRWXO
	This is equivalent to ‘(S_IROTH | S_IWOTH | S_IXOTH)’.

	S_ISUID
	This is the set-user-ID on execute bit, usually 04000. See How Change Persona.

	S_ISGID
	This is the set-group-ID on execute bit, usually 02000. See How Change Persona.

	S_ISVTX
	This is the sticky bit, usually 01000.
	*/	
	
	if ( argc < 2 )
		return;

	const char* pFilePath = argv[1];	

	int ret;
	ret = chmod( pFilePath, S_IRUSR | S_IWUSR );
	if ( ret )
	{
		perror("chmod");
		return;
	}
		
/*	// Or this way..
  	int fd = open( pFilePath, O_WRONLY );
	ret = fchmod( fd, S_IRUSR | S_IWUSR );
	if ( ret )
	{
		perror("fchmod");
		return;
	}	
	*/
	
	printf("ChangePermission: %s's permission has been changed.\n", pFilePath );
}

void	
CDirectoryTest::ChangeOwnership(int argc, const char* argv[])
{
	if ( argc < 2 )
		return;

	const char* pFilePath = argv[1];	
		
	struct group* gr;
	int	ret;
	
	gr = getgrnam("officers");
	
	uid_t	owner 	= -1;	// which means do not change for this part.
	gid_t	group	= gr->gr_gid;
	
	free (gr); 
	
	ret = chown( pFilePath, owner, group );
			
	if ( ret )
	{
		perror("chown");
		return;
	}
	
	printf("ChangeOwnership: %s's ownership has been changed.\n", pFilePath );	
}

void	
CDirectoryTest::ChangeOwnershipRoot(int argc, const char* argv[])
{
	if ( argc < 2 )
		return;

	const char* pFilePath = argv[1];	
		
	int	ret;
	uid_t	owner 	= 0;
	gid_t	group	= 0;
	
	ret = chown( pFilePath, owner, group );
	if ( ret )
	{
		perror("chown");
		return;
	}
	
	printf("ChangeOwnershipRoot: %s's ownership has been changed to root.\n", pFilePath );	
}

void	
CDirectoryTest::GetCurrentDir()
{
#define OPTION1	
#ifdef OPTION1
	char currentdir[256] = {0,};

	if ( getcwd( currentdir, 256 ) == NULL )
	{
		perror("getcwd");
		return;
	}

	printf( "GetCurrentDir: currentdir= %s\n", currentdir );	

#else		
	// Option 2. 	
	char* currentdir2;
	currentdir2 = getcwd( NULL, 0 );
	if ( currentdir2 == NULL )
	{
		free( currentdir2 );
		perror("getcwd");
		return;
	}	
	
	printf( "GetCurrentDir: currentdir2=%s\n", currentdir2 );	
	
	free( currentdir2 );
#endif
}

void	
CDirectoryTest::ChangeCurrentDir(int argc, const char* argv[])
{
	GetCurrentDir();
	
	if ( argc < 2 )
		return;
	
	int ret;	
	const char* path = argv[1];
	
	ret = chdir( path );
	if ( ret == -1 )
	{
		perror("chdir");
		return;
	}		
		
	GetCurrentDir();
}

void	
CDirectoryTest::ChangeCurrentDirResotore(int argc, const char* argv[])
{
	if ( argc < 2 )
		return;
	const char* path = argv[1];
	
	printf("ChangeCurrentDirResotore: check the current dir.\n");
	
	GetCurrentDir();
	
	int ret;
	int swd_fd;
	
/*	Note:
 * 	Using a dummy file descriptor to recover the current directory is faster than using getcwd()
 * 	since the kernel has to create path name by going through directory structure, which burdens itself.
 */
	swd_fd = open(".", O_RDONLY);	
	if ( swd_fd == -1 )
	{
		perror("open");
		return;
	}
	
	printf("ChangeCurrentDirResotore: change.\n");
	
	ret = chdir( path );
	if ( ret == -1 )
	{
		perror("chdir");
		return;
	}
	
	GetCurrentDir();
	
	fchdir( swd_fd );
	if ( ret == -1 )
	{
		perror("fchdir");
		return;
	}	
	
	printf("ChangeCurrentDirResotore: check if it's been recovered.\n");
	
	GetCurrentDir();
}

void
CDirectoryTest::MakeDir(int argc, const char* argv[])
{
	if ( argc < 2 )
		return;

	int ret;		
	const char* path 	= argv[1];
	mode_t		mode	= S_IRUSR | S_IWUSR;
	
	ret = mkdir( path, mode );
	if ( ret == -1 )
	{
		perror("mkdir");
		return;
	}
	
	if ( path[0] == '/' )
	{		
		// absolute path
		printf("MakeDir: %s\n", path );		
	}
	else
	{
		// relative path
		char currentdir[256] = {0,};
		if ( getcwd( currentdir, 256 ) == NULL )
		{
			perror("getcwd");
			return;
		}
		
		printf("MakeDir: %s/%s\n", currentdir, path );				
	}
}

void	
CDirectoryTest::RemoveDir(int argc, const char* argv[])
{
	if ( argc < 2 )
		return;

	int ret;		
	const char* path 	= argv[1];	
		
	ret = rmdir( path );
	if ( ret == -1 )
	{
		perror("rmdir");
		return;
	}
	
	printf("RemoveDir: directory was deleted(%s)\n", path );
}

void	
CDirectoryTest::DirStream(int argc, const char* argv[])
{
	std::string path;
	
	if ( argc < 2 )
	{
		char currentdir[256] = {0,};
		getcwd( currentdir, 256 );
	
		path = currentdir;
		printf("DirStream: start(current directory)...\n");
	}
	else
	{
		path = argv[1];
		printf("DirStream: start(%s)...\n", path.c_str() );
	}
	
	struct dirent* entry;
/*	ino_t			d_ino		: inode number
 * 	off_t			d_off		: offset for the next dir entry
 * 	unsigned short	d_reclen	: the length of this content?
 * 	unsigned char	d_type		: file type
 * 	char			d_name[256]	: file name	(This is the only mandatory field by POSIX; The others may not compatible on other system)
 */			
	DIR* dir;
	
	// 1. Open dir stream
	dir = opendir( path.c_str() );
	
	// 2. Browse entries
	errno = 0;
	int i = 0;
	while( (entry = readdir( dir )) != NULL )
		printf("	%d	(%d)	%s\n", i++, entry->d_type, entry->d_name );

	// 3. Close dir stream
	closedir( dir );

	printf("DirStream: end...\n");	
}

void	
CDirectoryTest::MakeHardLink(int argc, const char* argv[])
{
	if ( argc < 3 )
		return;
	
	int ret;	
	const char* pathOld = argv[1];
	const char* pathNew	= argv[2];
	
	printf("MakeHardLink: pathOld=%s\n", pathOld );
	printf("MakeHardLink: pathNew=%s\n", pathNew );
	
	ret = link( pathOld, pathNew );
	if ( ret == -1 )
	{
		perror("link");
		return;
	}
	
	printf("MakeHardLink: check the link you made.\n");
}

void
CDirectoryTest::MakeSymbolicLink(int argc, const char* argv[])
{
	if ( argc < 3 )
		return;
	
	int ret;	
	const char* pathOld = argv[1];
	const char* pathNew	= argv[2];
	
	printf("MakeSymbolicLink: pathOld=%s\n", pathOld );
	printf("MakeSymbolicLink: pathNew=%s\n", pathNew );
	
	ret = symlink( pathOld, pathNew );
	if ( ret == -1 )
	{
		perror("symlink");
		return;
	}
	
	printf("MakeSymbolicLink: check the link you made.\n");
}

void	
CDirectoryTest::DoUnLink(int argc, const char* argv[])
{
	if ( argc < 2 )
		return;
	
	int ret;	
	const char* path = argv[1];
	
	printf("DoUnLink: path=%s\n", path );

	ret = unlink( path );
	if ( ret == -1 )
	{
		perror("unlink");
		return;
	}
	
	printf("DoUnLink: check if the link has been removed.\n");		
}

void	
CDirectoryTest::MoveFile(int argc, const char* argv[])
{
	if ( argc < 3 )
		return;
	
	int ret;	
	const char* pathOld = argv[1];
	const char* pathNew	= argv[2];
	
	printf("MoveFile: pathOld=%s\n", pathOld );
	printf("MoveFile: pathNew=%s\n", pathNew );
	
	ret = rename( pathOld, pathNew );
	if ( ret == -1 )
	{
		perror("rename");
		return;
	}

	printf("MoveFile: check if the file has been moved.\n");
}
