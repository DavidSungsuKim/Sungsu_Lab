
// SungsuWorks2015Dlg.h : header file
//

#pragma once

//#include "..//..//Source/SwDataStructureStudy.h"
#include	"SwDataStructureStudy.h"

// CSungsuWorks2015Dlg dialog
class CSungsuWorks2015Dlg : public CDialogEx
{
// Construction
public:
	CSungsuWorks2015Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUNGSUWORKS2015_DIALOG };
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
	afx_msg void OnBnClickedButton1();

public:
	CSwDataStructureStudy	m_DataStructure;
};
