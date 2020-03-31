// SungsuTCPServer.h : main header file for the SUNGSUTCPSERVER application
//

#if !defined(AFX_SUNGSUTCPSERVER_H__D94735AE_7F98_43CB_B306_7C8E3D0EF99F__INCLUDED_)
#define AFX_SUNGSUTCPSERVER_H__D94735AE_7F98_43CB_B306_7C8E3D0EF99F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSungsuTCPServerApp:
// See SungsuTCPServer.cpp for the implementation of this class
//

class CSungsuTCPServerApp : public CWinApp
{
public:
	CSungsuTCPServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSungsuTCPServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSungsuTCPServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUNGSUTCPSERVER_H__D94735AE_7F98_43CB_B306_7C8E3D0EF99F__INCLUDED_)
