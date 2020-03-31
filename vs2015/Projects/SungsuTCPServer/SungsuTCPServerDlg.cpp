// SungsuTCPServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SungsuTCPServer.h"
#include "SungsuTCPServerDlg.h"

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
// CSungsuTCPServerDlg dialog

CSungsuTCPServerDlg::CSungsuTCPServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSungsuTCPServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSungsuTCPServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSungsuTCPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSungsuTCPServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSungsuTCPServerDlg, CDialog)
	//{{AFX_MSG_MAP(CSungsuTCPServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TEST, OnBtnTest)
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_SEND_MSG, OnBtnSendMsg)
	ON_BN_CLICKED(IDC_BTN_SEND_MSG2, OnBtnSendMsg2)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSungsuTCPServerDlg message handlers

BOOL CSungsuTCPServerDlg::OnInitDialog()
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
	
	m_bSendRun = false;
	
	InitControl();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSungsuTCPServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSungsuTCPServerDlg::OnPaint() 
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
HCURSOR CSungsuTCPServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSungsuTCPServerDlg::InitControl()
{
	m_pWndTest		= GetDlgItem(IDC_BTN_TEST);
	m_pWndOpen		= GetDlgItem(IDC_BTN_OPEN);
	m_pWndClose		= GetDlgItem(IDC_BTN_CLOSE);
	m_pWndSendMsg	= GetDlgItem(IDC_BTN_SEND_MSG);
	m_pWndSendMsg2	= GetDlgItem(IDC_BTN_SEND_MSG2);

	SetDlgItemText(IDC_EDIT_SERVER_PORT, "3000");

	m_pWndTest->EnableWindow( false );
	m_pWndClose->EnableWindow( false );
	m_pWndSendMsg->EnableWindow( false );
	m_pWndSendMsg2->EnableWindow( false );
}

void CSungsuTCPServerDlg::OnBtnTest() 
{
	CString str;
	char	serverPort[32];
		
	GetDlgItemText(IDC_EDIT_SERVER_PORT, str);
	strcpy(serverPort, str.GetBuffer(0));

//	int ret = m_TcpServer.Test( serverPort );
	int ret = m_TcpServer.Open( serverPort );

	str.Format("ret=%d\n", ret);
	AfxMessageBox(str);
}

void CSungsuTCPServerDlg::OnBtnOpen() 
{
	CString str;
	char	serverPort[32];
	
	GetDlgItemText(IDC_EDIT_SERVER_PORT, str);
	strcpy(serverPort, str.GetBuffer(0));

	int ret = m_TcpServer.Open( serverPort );

	if ( ret == 0 )
	{
		m_pWndOpen->EnableWindow( false );
		m_pWndClose->EnableWindow( true );
		m_pWndSendMsg->EnableWindow( true );
		m_pWndSendMsg2->EnableWindow( true );
	}
}

void CSungsuTCPServerDlg::OnBtnSendMsg() 
{
	CString str;
	enum	{ eMaxSendSize = 256 };
	char	message[eMaxSendSize];
	int		sizeMsg;
	
	GetDlgItemText(IDC_EDIT_MESSAGE_TO_SEND, str);
	strcpy(message, str.GetBuffer(0));

	sizeMsg = str.GetLength();
	if ( sizeMsg < eMaxSendSize )
		message[sizeMsg] = '\0';
	
	bool a = cTO_CHECK_SERVER_PROGRAM;	// I need to take care of the maximum size to send.
	bool b = cTO_CHECK_SERVER_PROGRAM;	// When I send more than two, the client receives more than two. 

	int ret = m_TcpServer.SendMessage( message, sizeMsg+1 );
	
	if ( ret != 0 )
		AfxMessageBox("Message send error.\n");
}

void CSungsuTCPServerDlg::OnBtnClose() 
{
	int ret = m_TcpServer.Close();
	
	if ( ret == 0 )
	{
		m_pWndOpen->EnableWindow( true );
		m_pWndClose->EnableWindow( false );
		m_pWndSendMsg->EnableWindow( false );
		m_pWndSendMsg2->EnableWindow( false );
	}
}
void CSungsuTCPServerDlg::OnBtnSendMsg2() 
{
	if ( !m_bSendRun )
	{
		m_bSendRun = true;
		m_pWndSendMsg->EnableWindow( false );
		SetDlgItemText(IDC_BTN_SEND_MSG2, "Send Run");

		int period = GetDlgItemInt(IDC_EDIT_TIMER_PERIOD);
		if ( period < 1 )
			period = 1;		
	
		SetDlgItemInt(IDC_EDIT_TIMER_PERIOD, period);
		SetTimer(0, period, 0);
	}
	else
	{
		m_bSendRun = false;

		KillTimer(0);
		SetDlgItemText(IDC_BTN_SEND_MSG2, "Run Stopped");
		m_pWndSendMsg->EnableWindow( true );
	}
}

void CSungsuTCPServerDlg::OnTimer(UINT nIDEvent) 
{
	OnBtnSendMsg();

	CDialog::OnTimer(nIDEvent);
}
