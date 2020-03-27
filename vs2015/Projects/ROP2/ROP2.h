// ROP2.h : main header file for the ROP2 application
//

#if !defined(AFX_ROP2_H__BB6EED43_BD78_4D52_9033_8FE87C63A0E0__INCLUDED_)
#define AFX_ROP2_H__BB6EED43_BD78_4D52_9033_8FE87C63A0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CROP2App:
// See ROP2.cpp for the implementation of this class
//

class CROP2App : public CWinApp
{
public:
	CROP2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CROP2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CROP2App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROP2_H__BB6EED43_BD78_4D52_9033_8FE87C63A0E0__INCLUDED_)
