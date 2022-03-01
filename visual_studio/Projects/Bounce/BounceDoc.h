// BounceDoc.h : interface of the CBounceDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNCEDOC_H__FF71458D_F4AE_4CB3_A08A_5F2BD1866F02__INCLUDED_)
#define AFX_BOUNCEDOC_H__FF71458D_F4AE_4CB3_A08A_5F2BD1866F02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBounceDoc : public CDocument
{
protected: // create from serialization only
	CBounceDoc();
	DECLARE_DYNCREATE(CBounceDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBounceDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBounceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBounceDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOUNCEDOC_H__FF71458D_F4AE_4CB3_A08A_5F2BD1866F02__INCLUDED_)
