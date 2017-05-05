
// SungsuWorks2015.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSungsuWorks2015App:
// See SungsuWorks2015.cpp for the implementation of this class
//

class CSungsuWorks2015App : public CWinApp
{
public:
	CSungsuWorks2015App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSungsuWorks2015App theApp;