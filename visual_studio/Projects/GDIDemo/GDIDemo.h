// GDIDemo.h : main header file for the GDIDEMO application
//

#if !defined(AFX_GDIDEMO_H__8FADFBCC_BD13_4255_B3D3_30D70F3F9335__INCLUDED_)
#define AFX_GDIDEMO_H__8FADFBCC_BD13_4255_B3D3_30D70F3F9335__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGDIDemoApp:
// See GDIDemo.cpp for the implementation of this class
//

class CGDIDemoApp : public CWinApp
{
public:
	CGDIDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDIDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGDIDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIDEMO_H__8FADFBCC_BD13_4255_B3D3_30D70F3F9335__INCLUDED_)
