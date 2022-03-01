
// MFCPracticeDialogDlg.h : header file
//

#pragma once


// CMFCPracticeDialogDlg dialog
class CMFCPracticeDialogDlg : public CDialogEx
{
// Construction
public:
	CMFCPracticeDialogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCPRACTICEDIALOG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


protected:
	int		m_posX;
	int		m_posY;
	int		m_posLastX;
	int		m_posLastY;
	CRect	m_rect;

public:
	void	Initialize		( );
	void	ShowBackground	( );
	void	DrawRectangle	( int aPosX, int aPosY, bool abRemoveTrace = true );

	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedRight();
};
