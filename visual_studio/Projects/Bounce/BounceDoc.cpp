// BounceDoc.cpp : implementation of the CBounceDoc class
//

#include "stdafx.h"
#include "Bounce.h"

#include "BounceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBounceDoc

IMPLEMENT_DYNCREATE(CBounceDoc, CDocument)

BEGIN_MESSAGE_MAP(CBounceDoc, CDocument)
	//{{AFX_MSG_MAP(CBounceDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBounceDoc construction/destruction

CBounceDoc::CBounceDoc()
{
	// TODO: add one-time construction code here

}

CBounceDoc::~CBounceDoc()
{
}

BOOL CBounceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBounceDoc serialization

void CBounceDoc::Serialize(CArchive& ar)
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
// CBounceDoc diagnostics

#ifdef _DEBUG
void CBounceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBounceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBounceDoc commands
