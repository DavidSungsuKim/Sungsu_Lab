// ROP2Doc.h : interface of the CROP2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROP2DOC_H__1B27C551_2E0B_43AE_8341_80E29B82AF07__INCLUDED_)
#define AFX_ROP2DOC_H__1B27C551_2E0B_43AE_8341_80E29B82AF07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CROP2Doc : public CDocument
{
protected: // create from serialization only
	CROP2Doc();
	DECLARE_DYNCREATE(CROP2Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CROP2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CROP2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CROP2Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROP2DOC_H__1B27C551_2E0B_43AE_8341_80E29B82AF07__INCLUDED_)
