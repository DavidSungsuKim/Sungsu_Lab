#include <stdio.h>
#include "class.h"
#include "FileTest.h"
#include "Logger.h"

int main() 
{
	CTestChild testChild;
	CTest* pTest = &testChild;

	pTest->Func();
	
//	const char* fileLogging = "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testLogging.txt";		
//	g_Logger.SetLogFilePath( fileLogging );

	CFileTest testFile;

	const char* fileRead	= "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/test.txt";
	const char* fileWrite 	= "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testWrite.txt";
			
	testFile.Read( fileRead );
	testFile.Write( fileWrite, "test....write\r\n");
	testFile.Select();	
	testFile.Poll();
			
	return 0;
}
