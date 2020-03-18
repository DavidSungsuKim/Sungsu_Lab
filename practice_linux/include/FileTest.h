
#ifndef __FILE_TEST__
#define __FILE_TEST__
#endif

class CLogger;

class CFileTest
{
public: 
	CFileTest	();
	~CFileTest	();
	
	int		Read	(const char* aFilePath);
	int 	Write	(const char* aFilePath, const char* aString);
	int		Select	();	
	int		Poll	();
	
	int		ReadStd				(const char* aFilePath);
	int 	WriteStd			(const char* aFilePath, const char* aString);
	int		ReadWriteBinaryStd	(const char* aFilePath);
};
	
