
// MFCPracticeDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCPracticeDialog.h"
#include "MFCPracticeDialogDlg.h"
#include "afxdialogex.h"

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


// CMFCPracticeDialogDlg dialog



CMFCPracticeDialogDlg::CMFCPracticeDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCPRACTICEDIALOG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCPracticeDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCPracticeDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCPracticeDialogDlg::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCPracticeDialogDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCPracticeDialogDlg::OnBnClickedDown)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCPracticeDialogDlg::OnBnClickedRight)
END_MESSAGE_MAP()


// CMFCPracticeDialogDlg message handlers

BOOL CMFCPracticeDialogDlg::OnInitDialog()
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
	Initialize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCPracticeDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCPracticeDialogDlg::OnPaint()
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
HCURSOR CMFCPracticeDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void	
CMFCPracticeDialogDlg::Initialize()
{
	m_posX = 0;
	m_posY = 0;

//	GetClientRect( m_rect );
	CWnd* pWnd = GetDlgItem(IDC_WHITE_BOARD);
	pWnd->GetClientRect( m_rect );

	CClientDC dc(pWnd/*this*/);

	ShowBackground();
}

void	
CMFCPracticeDialogDlg::ShowBackground()
{
	CWnd* pWnd = GetDlgItem(IDC_WHITE_BOARD);

	CClientDC dc(pWnd/*this*/);
	CBrush	brush, *pOldBrush;
	CPen	pen, *pOldPen;

	// Prepare pen and brush
	brush.CreateSolidBrush(RGB(255, 255, 255));
	pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	// Apply
	pOldBrush = (CBrush*)dc.SelectObject(&brush);
	pOldPen = (CPen*)dc.SelectObject(&pen);

	// Draw
	int width	= m_rect.Width();
	int height	= m_rect.Height();
	dc.Rectangle( 0, 0, width, height );

	// Restore old pen and brush
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

void	
CMFCPracticeDialogDlg::DrawRectangle(int aPosX, int aPosY, bool abRemoveTrace )
{
	CWnd* pWnd = GetDlgItem( IDC_WHITE_BOARD );

	CClientDC dc(pWnd /*this*/);
	CBrush	brush,	*pOldBrush;  				
	CPen	pen,	*pOldPen;       			

	if ( abRemoveTrace )
	{
		// Prepare pen and brush
		CBrush	brushWB;
		CPen	penWB;
		brushWB.CreateSolidBrush(RGB(255, 255, 255));
		penWB.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

		pOldBrush	= (CBrush*)dc.SelectObject(&brushWB);
		pOldPen		= (CPen*)dc.SelectObject(&penWB);

		dc.Rectangle(0+m_posLastX, 0+m_posLastY, 20+m_posLastX, 20+m_posLastY);
	}
		
	// Prepare pen and brush
	brush.CreateSolidBrush(RGB(255, 255, 0));  	
	pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));

	// Apply
	pOldBrush	= (CBrush*)dc.SelectObject(&brush); 
	pOldPen		= (CPen*)dc.SelectObject(&pen);

	// Draw
	dc.Rectangle(0+aPosX, 0+aPosY, 20+aPosX, 20+aPosY);

	m_posLastX = aPosX;
	m_posLastY = aPosY;

	// Restore old pen and brush
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);			
}

void CMFCPracticeDialogDlg::OnBnClickedLeft()
{
	m_posX -= 2;
	if ( m_posX < 0 )
		m_posX = 0;
	DrawRectangle( m_posX, m_posY );
}


void CMFCPracticeDialogDlg::OnBnClickedUp()
{
	m_posY -= 2;
	if ( m_posY < 0 )
		m_posY = 0;
	DrawRectangle(m_posX, m_posY);
}


void CMFCPracticeDialogDlg::OnBnClickedDown()
{
	m_posY += 2;
	if (m_rect.Height() < m_posY)
		m_posY = m_rect.Height();
	DrawRectangle(m_posX, m_posY);
}


void CMFCPracticeDialogDlg::OnBnClickedRight()
{
	m_posX += 2;
	if (m_rect.Width() < m_posY)
		m_posY = m_rect.Width();
	DrawRectangle(m_posX, m_posY);
}
