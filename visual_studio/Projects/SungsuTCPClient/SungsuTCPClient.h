// SungsuTCPClient.h : main header file for the SUNGSUTCPCLIENT application
//

#if !defined(AFX_SUNGSUTCPCLIENT_H__FA5C61A9_2B55_4ABD_9460_6146616A5DAF__INCLUDED_)
#define AFX_SUNGSUTCPCLIENT_H__FA5C61A9_2B55_4ABD_9460_6146616A5DAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSungsuTCPClientApp:
// See SungsuTCPClient.cpp for the implementation of this class
//

class CSungsuTCPClientApp : public CWinApp
{
public:
	CSungsuTCPClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSungsuTCPClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSungsuTCPClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUNGSUTCPCLIENT_H__FA5C61A9_2B55_4ABD_9460_6146616A5DAF__INCLUDED_)
