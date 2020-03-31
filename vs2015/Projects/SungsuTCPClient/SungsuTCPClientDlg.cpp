// SungsuTCPClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SungsuTCPClient.h"
#include "SungsuTCPClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSungsuTCPClientDlg dialog

CSungsuTCPClientDlg::CSungsuTCPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSungsuTCPClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSungsuTCPClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSungsuTCPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSungsuTCPClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSungsuTCPClientDlg, CDialog)
	//{{AFX_MSG_MAP(CSungsuTCPClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TEST, OnBtnTest)
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
	ON_BN_CLICKED(IDC_BTN_SEND_MSG, OnBtnSendMsg)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_CLEAR_RECV_MSG, OnBtnClearRecvMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSungsuTCPClientDlg message handlers

BOOL CSungsuTCPClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_curRecvRow	= 0;
	m_maxRecvRow	= 20;
	m_maxRecvCol	= 30;

	m_numMsgReceived	= 0;

	InitControl();	
	InitTask();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSungsuTCPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSungsuTCPClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSungsuTCPClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSungsuTCPClientDlg::InitTask()
{
	UINT uiTaskId = 0;
	
	m_hTask = (HANDLE)_beginthreadex( NULL, 0, TaskProc, this, 0, &uiTaskId);	
}

void CSungsuTCPClientDlg::Task()
{
	while(1)
	{
		if ( !m_TcpClient.IsOpened() )
		{
			Sleep(1000);
			continue;
		}

		int	 ret;
		char message[256];
		ret = m_TcpClient.RecvMessage(message, 256);

		TRACE("Task:msg recv, ret=%d, msg=%s\n", ret, message);
		if ( ret == 0 )
		{
			m_numMsgReceived++;
			DisplayRecvMessage(message);
		}
	}
}

UINT WINAPI	CSungsuTCPClientDlg::TaskProc(LPVOID pParam)
{
	CSungsuTCPClientDlg* pThis = reinterpret_cast<CSungsuTCPClientDlg*>( pParam );
	_ASSERTE( pThis != NULL );
	
	pThis->Task();
	
	_endthreadex( 7 );
	
	return 1L;
}

void CSungsuTCPClientDlg::InitControl()
{
	m_pWndTest		= GetDlgItem(IDC_BTN_TEST);
	m_pWndOpen		= GetDlgItem(IDC_BTN_OPEN);
	m_pWndClose		= GetDlgItem(IDC_BTN_CLOSE);
	m_pWndSendMsg	= GetDlgItem(IDC_BTN_SEND_MSG);

	SetDlgItemText(IDC_EDIT_SERVER_IP, "55.101.42.80");
	SetDlgItemText(IDC_EDIT_SERVER_PORT, "3000");
	
	m_pWndTest->EnableWindow( false );
	m_pWndClose->EnableWindow( false );
	m_pWndSendMsg->EnableWindow( false );
}

void CSungsuTCPClientDlg::DisplayRecvMessage(const char* newMsg)
{
	CString msgCurrent = newMsg;

	// Something needs to be improved....here.

	m_curRecvRow++;
	if ( msgCurrent.GetLength() > m_maxRecvCol )
	{
		msgCurrent.Insert(m_maxRecvCol, '\n');
		m_curRecvRow++;
	}

	CString numMsg;	
	numMsg.Format("# %d:	", m_numMsgReceived);

	m_strRecvMsg += numMsg;
	m_strRecvMsg += msgCurrent;
	m_strRecvMsg += "\n";

 	int sizeStr = m_strRecvMsg.GetLength();
 	int index;	

	if ( m_curRecvRow > m_maxRecvRow )
	{
		index = m_strRecvMsg.Find('#', 1);
 		m_strRecvMsg.Delete(0, index );
	}
	
	SetDlgItemText(IDC_STATIC_MSG_RECVED, m_strRecvMsg);
}

void CSungsuTCPClientDlg::OnBtnTest() 
{
	CString str;
	char	serverAddr[32];
	char	serverPort[32];

	GetDlgItemText(IDC_EDIT_SERVER_IP, str);
	strcpy(serverAddr, str.GetBuffer(0));
		
	GetDlgItemText(IDC_EDIT_SERVER_PORT, str);
	strcpy(serverPort, str.GetBuffer(0));

	int ret = m_TcpClient.Test(serverAddr, serverPort);

	str.Format("ret=%d\n", ret);
	AfxMessageBox(str);
}

void CSungsuTCPClientDlg::OnBtnOpen() 
{
	CString str;
	char	serverAddr[32];
	char	serverPort[32];

	GetDlgItemText(IDC_EDIT_SERVER_IP, str);
	strcpy(serverAddr, str.GetBuffer(0));
	
	GetDlgItemText(IDC_EDIT_SERVER_PORT, str);
	strcpy(serverPort, str.GetBuffer(0));
	
	int ret = m_TcpClient.Open(serverAddr, serverPort);
	
	if ( ret == 0 )
	{
		m_pWndOpen->EnableWindow( false );
		m_pWndClose->EnableWindow( true );
		m_pWndSendMsg->EnableWindow( true );
	}	
}

void CSungsuTCPClientDlg::OnBtnSendMsg() 
{
	
}

void CSungsuTCPClientDlg::OnBtnClose() 
{
	int ret = m_TcpClient.Close();
	
	if ( ret == 0 )
	{
		m_pWndOpen->EnableWindow( true );
		m_pWndClose->EnableWindow( false );
		m_pWndSendMsg->EnableWindow( false );
	}	
}

void CSungsuTCPClientDlg::OnBtnClearRecvMsg() 
{
	m_strRecvMsg		= "";
	m_curRecvRow		= 0;
	m_numMsgReceived	= 0;
	SetDlgItemText(IDC_STATIC_MSG_RECVED, m_strRecvMsg);
}
