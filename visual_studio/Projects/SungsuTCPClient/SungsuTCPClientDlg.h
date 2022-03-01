// SungsuTCPClientDlg.h : header file
//

#if !defined(AFX_SUNGSUTCPCLIENTDLG_H__B74A5FE8_217A_4499_9BFB_8322801C7352__INCLUDED_)
#define AFX_SUNGSUTCPCLIENTDLG_H__B74A5FE8_217A_4499_9BFB_8322801C7352__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TCPClient.h"

/////////////////////////////////////////////////////////////////////////////
// CSungsuTCPClientDlg dialog

class CSungsuTCPClientDlg : public CDialog
{
// Construction
public:
	CSungsuTCPClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSungsuTCPClientDlg)
	enum { IDD = IDD_SUNGSUTCPCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSungsuTCPClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSungsuTCPClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnTest();
	afx_msg void OnBtnOpen();
	afx_msg void OnBtnSendMsg();
	afx_msg void OnBtnClose();
	afx_msg void OnBtnClearRecvMsg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// UI
	CString				m_strRecvMsg;
	int					m_maxRecvRow;
	int					m_maxRecvCol;
	int					m_curRecvRow;

	// Msg Control
	int					m_numMsgReceived;

	// Task
	HANDLE				m_hTask;

	// UI
	void				InitControl();
	void				DisplayRecvMessage(const char* newMsg);

	// Task
	void				InitTask();
	void				Task();
	static UINT WINAPI	TaskProc(LPVOID pParam);

private:
	CTCPClient	m_TcpClient;

	CWnd*		m_pWndTest;
	CWnd*		m_pWndOpen;
	CWnd*		m_pWndClose;
	CWnd*		m_pWndSendMsg;

	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUNGSUTCPCLIENTDLG_H__B74A5FE8_217A_4499_9BFB_8322801C7352__INCLUDED_)
