#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>

extern const bool cTO_CHECK_CLIENT_PROGRAM;

class CTCPClient
{
public:
	CTCPClient();
	~CTCPClient();

	int		Test		(const char* serverIP, const char* serverPort);
	int		TestFileRecv(const char* serverIP, const char* serverPort);

	int		Open		(const char* serverIP, const char* serverPort);
	int		Close		( );
	int		SendMessage	(char* msgBuf, int bufSize);
	int		RecvMessage	(char* msgBuf, int bufSize);

	bool	IsOpened	( ) { return m_bOpened; }

protected:
	// TCP
	bool				m_bOpened;
	SOCKET				m_sockClient;

	// Task
	HANDLE				m_hTask;
	
	// Task
	void				InitTask();
	void				Task();
	static UINT WINAPI	TaskProc(LPVOID pParam);
};

