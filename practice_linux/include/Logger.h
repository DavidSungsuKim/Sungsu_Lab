
#ifndef __LOGGER_H__
#define __LOGGER_H__
#endif

// Forward decleration
class CLogger;

extern CLogger g_Logger;

class CLogger
{
public:
	CLogger();
	~CLogger();
	
	int				SetLogFilePath	( const char* aFile );
	virtual void 	Telemetry		( const char* aString, ... );
	void 			Telemetry2		( const char* aFile, int aLineNo, const char* aString, ... );
		
private:
	int		m_fdLogFile;
};
