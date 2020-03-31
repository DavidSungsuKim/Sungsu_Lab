#include "stdafx.h"
#include "TCPClient.h"

const bool cONE_CLIENT = true;
const bool cTO_CHECK_CLIENT_PROGRAM = true;

CTCPClient::CTCPClient()
{
	m_bOpened		= false;
	m_sockClient	= false;
}


CTCPClient::~CTCPClient()
{
}

void
CTCPClient::InitTask()
{	
	UINT uiTaskId = 0;
	
	m_hTask = (HANDLE)_beginthreadex( NULL, 0, TaskProc, this, 0, &uiTaskId);	
}

void
CTCPClient::Task()
{
	while(1)
	{
		return;

// 		char message[256];
// 		int strLen;
// 		strLen = recv(m_sockServer, message, sizeof(message) - 1, 0);
// 		if (strLen == -1)
// 		{
// 			TRACE("Message error\n");		
// 			bool a = cTO_CHECK_CLIENT_PROGRAM;
// 			// When I close the server socket in the server,
// 			// strLen becomes -1.
// 			// I need to deal with this situation.
// 			Sleep(1000);
// 			continue;
// 		}
// 		
// 		TRACE("Message from the server : %s\n", message);		
// 		Sleep(10);
	}
}

UINT WINAPI	CTCPClient::TaskProc(LPVOID pParam)
{
	CTCPClient* pThis = reinterpret_cast<CTCPClient*>( pParam );
	_ASSERTE( pThis != NULL );
	
	pThis->Task();
	
	_endthreadex( 7 );
	
	return 1L;
}

int 
CTCPClient::Test(const char* serverIP, const char* serverPort)
{
// 	FILE * pFile;
// 	char sentence[10];
// 
// 	fputs("test", stdout);
// 	fgets(sentence, 10, stdin);

	WSADATA	wsaData;
	SOCKET	hSocket;
	sockaddr_in	servAddr;
	u_long		servAddrUl;
	u_short		port;

	if ( serverIP == NULL )	
	{
		servAddrUl	= htonl(INADDR_LOOPBACK);
		port		= 3000;
	}
	else
	{
		servAddrUl	= inet_addr(serverIP);
		port		= htons(atoi(serverPort));
	}

	char message[30];
	int strLen;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		return -2;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family				= AF_INET;
	servAddr.sin_addr.S_un.S_addr	= servAddrUl;
	servAddr.sin_port				= port;

	// Server must be in ready state for clients to connect to it.
	if (connect(hSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		return -3;

	strLen = recv(hSocket, message, sizeof(message) - 1, 0);
	if (strLen == -1)
		return -4;

	printf("Message from the server : %s\n", message);

	closesocket(hSocket);
	WSACleanup();

	return 0;
}

int
CTCPClient::TestFileRecv(const char* serverIP, const char* serverPort)
{
#undef BUF_SIZE
#define BUF_SIZE	30

	WSADATA	wsaData;
	SOCKET	hSocket;
	FILE*	fp;

	char	buf[BUF_SIZE];
	int		readCnt;
	sockaddr_in	servAdr;
	
// 	char port[] = "9901";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	fp = fopen("file_received.dat", "wb");
	if (!fp)
		return -2;
	
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		return -3;

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = inet_addr(serverIP);//htonl(INADDR_LOOPBACK);
	servAdr.sin_port = htons(atoi(serverPort));

	connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while ((readCnt = recv(hSocket, buf, BUF_SIZE, 0)) != 0)
		fwrite((void*)buf, 1, readCnt, fp);

	puts("Received file data");
	send(hSocket, "Thank you", 10, 0);
	fclose(fp);
	closesocket(hSocket);
	WSACleanup();


#undef BUF_SIZE

	return 0;
}

int 
CTCPClient::Open(const char* serverIP, const char* serverPort)
{
	WSADATA		wsaData;
	SOCKET&		rSocket = m_sockClient;
	sockaddr_in	servAddr;
	u_long		servAddrUl;
	u_short		port;
	
	if ( serverIP == NULL )	
	{
		servAddrUl	= htonl(INADDR_LOOPBACK);
		port		= 3000;
	}
	else
	{
		servAddrUl	= inet_addr(serverIP);
		port		= htons(atoi(serverPort));
	}
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;
	
	rSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (rSocket == INVALID_SOCKET)
		return -2;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family				= AF_INET;
	servAddr.sin_addr.S_un.S_addr	= servAddrUl;
	servAddr.sin_port				= port;
	
	// Server must be in ready state for clients to connect to it.
	if (connect(rSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		return -3;

	//////////////////////////////////////////////
	// Configure buffer size.
	bool b = cTO_CHECK_CLIENT_PROGRAM; // I am not sure how this is working..
	int state;
	int sndBuf	= 1024 * 3;
	int recvBuf	= 1024 * 3;
	
	state = setsockopt(rSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, sizeof(sndBuf));
	if ( state == SOCKET_ERROR )
		return -4;
	
	state = setsockopt(rSocket, SOL_SOCKET, SO_RCVBUF, (char*)&recvBuf, sizeof(recvBuf));
	if ( state == SOCKET_ERROR )
		return -5;
	//////////////////////////////////////////////

	InitTask();

	m_bOpened = true;

	return 0;
}

int	
CTCPClient::Close( )
{
	if ( !m_bOpened )
		return 0;
	
	closesocket(m_sockClient);
	WSACleanup();
	
	m_bOpened = 0;
	
	return 0;
}

int	
CTCPClient::SendMessage(char* msgBuf, int bufSize )
{

	return 0;
}

int	
CTCPClient::RecvMessage(char* msgBuf, int bufSize )
{
	int strLen;
	strLen = recv(m_sockClient, msgBuf, bufSize, 0);
	if (strLen == -1)
		return -1;

	return 0;
}