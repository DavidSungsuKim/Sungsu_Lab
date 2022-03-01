#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>

extern const bool cTO_CHECK_SERVER_PROGRAM;

typedef struct stTCPInfo
{
	char addr[32];
	char port[16];
}StTCPInfo;

class CTCPServer
{
public:
	CTCPServer();
	~CTCPServer();

	int	Test			(const char* serverPort);
	int	TestFileSend	( );

	int Open			(const char* serverPort);
	int	Close			( );
	int	SendMessage		(char* msgBuf, int bufSize);
	int	RecvMessage		(char* msgBuf, int bufSize);

protected:
	// TCP
	bool				m_bOpened;
	SOCKET				m_sockServer;
	SOCKET				m_sockClient;

	// Task
	HANDLE				m_hTask;

	// Task
	void				InitTask();
	void				Task();
	static UINT WINAPI	TaskProc(LPVOID pParam);

};

