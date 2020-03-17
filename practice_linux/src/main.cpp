#include <stdio.h>
#include "class.h"
#include "FileTest.h"
#include "Logger.h"

int main() 
{
	printf( "Hello Sungsu! I am RP 4B again!\r\n" );
	
	CTestChild testChild;
	CTest* pTest = &testChild;

	int ret = 0;
	ret = pTest->Func();
	
	CFileTest testFile;
	ret = testFile.Read("/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/test.txt");
	
	const char* fileWrite = "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testWrite.txt";
	ret = testFile.Write( fileWrite, "test....write\r\n");
	
	const char* fileLogging = "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testLogging.txt";	
	g_Logger.SetLogFilePath( fileLogging );
	
	g_Logger.Telemetry( "ret=%d", ret );
	g_Logger.Telemetry2( __FILE__, __LINE__, "Test Telemetry2..." );
		
	return 0;
}
