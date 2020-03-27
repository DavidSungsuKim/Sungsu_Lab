// ROP2Doc.cpp : implementation of the CROP2Doc class
//

#include "stdafx.h"
#include "ROP2.h"

#include "ROP2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CROP2Doc

IMPLEMENT_DYNCREATE(CROP2Doc, CDocument)

BEGIN_MESSAGE_MAP(CROP2Doc, CDocument)
	//{{AFX_MSG_MAP(CROP2Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CROP2Doc construction/destruction

CROP2Doc::CROP2Doc()
{
	// TODO: add one-time construction code here

}

CROP2Doc::~CROP2Doc()
{
}

BOOL CROP2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CROP2Doc serialization

void CROP2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CROP2Doc diagnostics

#ifdef _DEBUG
void CROP2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CROP2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CROP2Doc commands
