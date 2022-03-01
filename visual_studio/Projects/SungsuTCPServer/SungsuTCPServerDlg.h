// SungsuTCPServerDlg.h : header file
//

#if !defined(AFX_SUNGSUTCPSERVERDLG_H__BA84C79A_AE50_4550_8D63_E2FDA385CA24__INCLUDED_)
#define AFX_SUNGSUTCPSERVERDLG_H__BA84C79A_AE50_4550_8D63_E2FDA385CA24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TCPServer.h"

/////////////////////////////////////////////////////////////////////////////
// CSungsuTCPServerDlg dialog

class CSungsuTCPServerDlg : public CDialog
{
// Construction
public:
	CSungsuTCPServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSungsuTCPServerDlg)
	enum { IDD = IDD_SUNGSUTCPSERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSungsuTCPServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSungsuTCPServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnTest();
	afx_msg void OnBtnOpen();
	afx_msg void OnBtnClose();
	afx_msg void OnBtnSendMsg();
	afx_msg void OnBtnSendMsg2();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	// UI
	bool	m_bSendRun;


	void	InitControl();

private:
	CTCPServer	m_TcpServer;

	CWnd*		m_pWndTest;
	CWnd*		m_pWndOpen;
	CWnd*		m_pWndClose;
	CWnd*		m_pWndSendMsg;
	CWnd*		m_pWndSendMsg2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUNGSUTCPSERVERDLG_H__BA84C79A_AE50_4550_8D63_E2FDA385CA24__INCLUDED_)
