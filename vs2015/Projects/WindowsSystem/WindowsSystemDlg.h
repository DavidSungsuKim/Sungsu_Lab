
// WindowsSystemDlg.h : header file
//

#pragma once

#include "WindowsSys.h"


// CWindowsSystemDlg dialog
class CWindowsSystemDlg : public CDialogEx
{
// Construction
public:
	CWindowsSystemDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWSSYSTEM_DIALOG };
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
public:
	CWindowsSys	m_WinSys;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
