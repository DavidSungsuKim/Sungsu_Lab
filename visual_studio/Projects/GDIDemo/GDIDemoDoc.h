// GDIDemoDoc.h : interface of the CGDIDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDIDEMODOC_H__49190380_96B9_4882_B42E_745D12DF7D96__INCLUDED_)
#define AFX_GDIDEMODOC_H__49190380_96B9_4882_B42E_745D12DF7D96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGDIDemoDoc : public CDocument
{
protected: // create from serialization only
	CGDIDemoDoc();
	DECLARE_DYNCREATE(CGDIDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDIDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGDIDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGDIDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIDEMODOC_H__49190380_96B9_4882_B42E_745D12DF7D96__INCLUDED_)
