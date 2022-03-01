#pragma once
class CSerialInterface
{
public:
	CSerialInterface();
	~CSerialInterface();

	void	Test();

	int		OpenPort	( CString portName );
	DWORD	SendPackets	( char* packet, long size );
	int		ClosePort	( );

protected:
	HANDLE	m_hCom;
	bool	m_bOpened;

};

