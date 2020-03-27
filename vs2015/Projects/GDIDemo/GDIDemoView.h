// GDIDemoView.h : interface of the CGDIDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDIDEMOVIEW_H__4B88C27B_AAFE_47C3_9D85_0ABE209257AF__INCLUDED_)
#define AFX_GDIDEMOVIEW_H__4B88C27B_AAFE_47C3_9D85_0ABE209257AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGDIDemoView : public CView
{
protected: // create from serialization only
	CGDIDemoView();
	DECLARE_DYNCREATE(CGDIDemoView)

// Attributes
public:
	CGDIDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDIDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGDIDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGDIDemoView)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GDIDemoView.cpp
inline CGDIDemoDoc* CGDIDemoView::GetDocument()
   { return (CGDIDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIDEMOVIEW_H__4B88C27B_AAFE_47C3_9D85_0ABE209257AF__INCLUDED_)
