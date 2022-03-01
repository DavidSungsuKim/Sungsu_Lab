
// SerialCommDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialComm.h"
#include "SerialCommDlg.h"
#include "afxdialogex.h"

#include "ShinbongRoverCommmand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSerialCommDlg dialog



CSerialCommDlg::CSerialCommDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERIALCOMM_DIALOG, pParent)
	, m_steeringValue(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STEERING_DATA, m_steeringValue);
}

BEGIN_MESSAGE_MAP(CSerialCommDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TEST, &CSerialCommDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_OPEN_PORT, &CSerialCommDlg::OnBnClickedOpenPort)
	ON_BN_CLICKED(IDC_SEND_PACKETS, &CSerialCommDlg::OnBnClickedSendPackets)
	ON_BN_CLICKED(IDC_SEND_STEERING, &CSerialCommDlg::OnBnClickedSendSteering)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CLOSE_PORT, &CSerialCommDlg::OnBnClickedClosePort)
	ON_BN_CLICKED(IDC_SEND_MAIN_MOTOR, &CSerialCommDlg::OnBnClickedSendMainMotor)
END_MESSAGE_MAP()


// CSerialCommDlg message handlers

BOOL CSerialCommDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: Add extra initialization here

	InitDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialCommDlg::InitDialog()
{

}

void CSerialCommDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialCommDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialCommDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSerialCommDlg::OnBnClickedTest()
{

}

void CSerialCommDlg::OnBnClickedOpenPort()
{
	if ( m_Rober.Initialize() == 0 )
	{
		GetDlgItem(IDC_OPEN_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CLOSE_PORT)->EnableWindow(TRUE);
	}
}


void CSerialCommDlg::OnBnClickedSendPackets()
{	

}

void CSerialCommDlg::OnBnClickedSendSteering()
{
	int value = GetDlgItemInt(IDC_STEERING_DATA);

	m_Rober.ChangeDirection( value );
}

void CSerialCommDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
}

void CSerialCommDlg::OnBnClickedClosePort()
{
	if ( m_Rober.Uninitialize() == 0 )
	{
		GetDlgItem(IDC_CLOSE_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPEN_PORT)->EnableWindow(TRUE);
	}
}

void CSerialCommDlg::OnBnClickedSendMainMotor()
{
	int value = GetDlgItemInt(IDC_MAIN_MOTOR_DATA);

	m_Rober.ChangeSpeed( value );
}
