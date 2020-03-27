// GDIDemoDoc.cpp : implementation of the CGDIDemoDoc class
//

#include "stdafx.h"
#include "GDIDemo.h"

#include "GDIDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGDIDemoDoc

IMPLEMENT_DYNCREATE(CGDIDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CGDIDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CGDIDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIDemoDoc construction/destruction

CGDIDemoDoc::CGDIDemoDoc()
{
	// TODO: add one-time construction code here

}

CGDIDemoDoc::~CGDIDemoDoc()
{
}

BOOL CGDIDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGDIDemoDoc serialization

void CGDIDemoDoc::Serialize(CArchive& ar)
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
// CGDIDemoDoc diagnostics

#ifdef _DEBUG
void CGDIDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGDIDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGDIDemoDoc commands
