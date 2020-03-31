#include "stdafx.h"
#include "TCPServer.h"

const bool cONE_CLIENT = true;
const bool cTO_CHECK_SERVER_PROGRAM = true;

CTCPServer::CTCPServer()
{
	m_bOpened = false;

	m_sockServer	= 0;
	m_sockClient	= 0;
}


CTCPServer::~CTCPServer()
{
}

void
CTCPServer::InitTask()
{	
	UINT uiTaskId = 0;

	m_hTask = (HANDLE)_beginthreadex( NULL, 0, TaskProc, this, 0, &uiTaskId);	
}

void
CTCPServer::Task()
{
	while(1)
	{
		char message[] = "Welcome!";

		if ( !cONE_CLIENT )
		{
			SOCKET		sockClient;	// One-time socket for clients
			sockaddr_in	clntAddr;
			
			int szClntAddr;
			szClntAddr	= sizeof(clntAddr);
			sockClient	= accept(m_sockServer, (sockaddr*)&clntAddr, &szClntAddr);
			if (sockClient == INVALID_SOCKET)
				continue;
			
			send(sockClient, message, sizeof(message), 0);

			closesocket(sockClient);
		}
		else
		{
		//	send(m_sockClient, message, sizeof(message), 0);
		}

		Sleep(10);
	}
}

UINT WINAPI	CTCPServer::TaskProc(LPVOID pParam)
{
	CTCPServer* pThis = reinterpret_cast<CTCPServer*>( pParam );
	_ASSERTE( pThis != NULL );
	
	pThis->Task();
	
	_endthreadex( 7 );
	
	return 1L;
}

int
CTCPServer::Test(const char* serverPort)
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	sockaddr_in	servAddr, clntAddr;

	char port[] = "9901";

	int szClntAddr;
	char message[] = "Welcome!";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		return -2;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY/*INADDR_LOOPBACK*/);
	servAddr.sin_port = htons(atoi(serverPort));

	if ( bind(hServSock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR )
		return -3;

	if (listen(hServSock, 5) == SOCKET_ERROR)
		return -4;

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (sockaddr*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		return -5;

	send(hClntSock, message, sizeof(message), 0);
	closesocket(hServSock);
	closesocket(hClntSock);
	WSACleanup();

	return 0;
}

int
CTCPServer::TestFileSend()
{
#undef BUF_SIZE
#define BUF_SIZE	30

	WSADATA	wsaData;
	SOCKET	hServSock, hClntSock;
	FILE*	fp;
	char	buf[BUF_SIZE];
	int		readCnt;

	char port[] = "9901";

	sockaddr_in	servAddr, clntAdr;
	int	clntAdrSz;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	fp = fopen("file_server_win.c", "rb");
	if (!fp)
		return -2;

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		return -3;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
	servAddr.sin_port = htons(atoi(port));

	bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(hServSock, 5);

	clntAdrSz = sizeof(clntAdr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

	while (1)
	{
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if (readCnt < BUF_SIZE)
		{
			send(hClntSock, (char*)&buf, readCnt, 0);
			break;
		}
		send(hClntSock, (char*)&buf, readCnt, 0);
	}

	shutdown(hClntSock, SD_SEND);
	recv(hClntSock, (char*)&buf, BUF_SIZE, 0);
	printf("Message from client: %s \n", buf);

	fclose(fp);
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();

#undef  BUF_SIZE
	return 0;
}


int 
CTCPServer::Open(const char* serverPort)
{
	if ( m_bOpened )
		return -1;

	WSADATA		wsaData;
	SOCKET&		rSockServer = m_sockServer;
	sockaddr_in	servAddr;	
	u_long		servAddrUl;
	u_short		port;

	if ( serverPort == NULL )
	{
		servAddrUl	= htonl(INADDR_LOOPBACK);
		port		= 3000; 
	}
	else
	{
		servAddrUl  = htonl(INADDR_ANY);
		port		= htons(atoi(serverPort));
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -2;
	
	rSockServer = socket(PF_INET, SOCK_STREAM, 0);
	if (rSockServer == INVALID_SOCKET)
		return -3;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family				= AF_INET;
	servAddr.sin_addr.S_un.S_addr	= servAddrUl;
	servAddr.sin_port				= port;
	
	if ( bind(rSockServer, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR )
		return -6;

	//////////////////////////////////////////////
	// Configure buffer size.
	bool b = cTO_CHECK_SERVER_PROGRAM; // I am not sure how this is working..
	int state;
	int sndBuf	= 1024 * 3;
	int recvBuf	= 1024 * 3;
	
	state = setsockopt(rSockServer, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, sizeof(sndBuf));
	if ( state == SOCKET_ERROR )
		return -4;
	
	state = setsockopt(rSockServer, SOL_SOCKET, SO_RCVBUF, (char*)&recvBuf, sizeof(recvBuf));
	if ( state == SOCKET_ERROR )
		return -5;
	//////////////////////////////////////////////
	
	if (listen(rSockServer, 5) == SOCKET_ERROR)
		return -7;

	if ( cONE_CLIENT )
	{
		sockaddr_in	clntAddr;
		int			szClntAddr;
		
		szClntAddr	= sizeof(clntAddr);
		m_sockClient = accept(rSockServer, (sockaddr*)&clntAddr, &szClntAddr);
		if (m_sockClient == INVALID_SOCKET)
			return -6;
	}

	InitTask();

	m_bOpened = true;

	return 0;
}

int
CTCPServer::Close( )
{
	if ( !m_bOpened )
		return 0;

	closesocket(m_sockServer);
	closesocket(m_sockClient);
	WSACleanup();

	m_bOpened = 0;

	return 0;
}

int	
CTCPServer::SendMessage(char* msgBuf, int bufSize)
{
	send(m_sockClient, msgBuf, bufSize, 0);
	return 0;
}

int	
CTCPServer::RecvMessage(char* msgBuf, int bufSize)
{

	return 0;
}