
// SerialCommDlg.h : header file
//

#pragma once

#include "ShinbongRober.h"

// CSerialCommDlg dialog
class CSerialCommDlg : public CDialogEx
{
// Construction
public:
	CSerialCommDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALCOMM_DIALOG };
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

	CShinbongRober		m_Rober;

	void		InitDialog();


public:
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedOpenPort();
	afx_msg void OnBnClickedSendPackets();
	afx_msg void OnBnClickedSendSteering();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedClosePort();
	int m_steeringValue;
	afx_msg void OnBnClickedSendMainMotor();


};
