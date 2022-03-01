#include "stdafx.h"
#include "SerialInterface.h"

#include <Windows.h>

CSerialInterface::CSerialInterface()
{
	m_bOpened = false;
}


CSerialInterface::~CSerialInterface()
{
	ClosePort();
}

void
CSerialInterface::Test()
{
	HANDLE hComm;

	CString ComPortName;
	ComPortName.Format(_T("COM%d"), 6);

	hComm = CreateFile(	ComPortName,                //port name
						GENERIC_READ | GENERIC_WRITE, //Read/Write
						0,                            // No Sharing
						NULL,                         // No Security
						OPEN_EXISTING,// Open existing port only
						0,            // Non Overlapped I/O
						NULL);        // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
		;//printf(¡°Error in opening serial port¡±);
	else
		;//printf(¡°opening serial port successful¡±);

	CloseHandle(hComm);//Closing the Serial Port

	m_bOpened = true;
}

int
CSerialInterface::OpenPort(CString portName)
{
	if ( m_bOpened )
		return -1;

	DCB  dcb = { sizeof(DCB) };

	m_hCom = CreateFile(portName,						//port name
						GENERIC_READ | GENERIC_WRITE,	//Read/Write
						0,								// No Sharing
						NULL,							// No Security
						OPEN_EXISTING,					// Open existing port only
						0,								// Non Overlapped I/O
						NULL);							// Null for Comm Devices

	if (m_hCom == INVALID_HANDLE_VALUE)
		return -2;

	SetCommMask(m_hCom, EV_BREAK | EV_ERR | EV_RXCHAR | EV_TXEMPTY);
	SetupComm(m_hCom, 4096, 4096);

	PurgeComm(m_hCom, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hCom, &dcb);		
	dcb.BaudRate	= CBR_9600;     
	dcb.ByteSize	= 8;            
	dcb.Parity		= NOPARITY;      
	dcb.StopBits	= ONESTOPBIT; 

	dcb.fParity		= FALSE;
	dcb.fNull		= FALSE;

	if (!SetCommState(m_hCom, &dcb))
		return -3;

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout			= 0;
	timeouts.ReadTotalTimeoutMultiplier		= 0;
	timeouts.ReadTotalTimeoutConstant		= 0;
	timeouts.WriteTotalTimeoutMultiplier	= 0;
	timeouts.WriteTotalTimeoutConstant		= 0;
	if (!SetCommTimeouts(m_hCom, &timeouts))
		return -4;

	m_bOpened = true;

	return 0;
}

DWORD
CSerialInterface::SendPackets(char* packet, long size)
{
	DWORD byteWritten = 0;
	WriteFile(m_hCom, packet, size, &byteWritten, NULL);

	return byteWritten;
}

int
CSerialInterface::ClosePort()
{
	if ( !m_bOpened )
		return 0;

	SetCommMask	(m_hCom, 0);
	PurgeComm	(m_hCom, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	CloseHandle	(m_hCom);

	m_bOpened = 0;
	return 0;
}