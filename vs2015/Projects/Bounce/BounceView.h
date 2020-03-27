// BounceView.h : interface of the CBounceView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNCEVIEW_H__AA096736_5CF7_43B5_84E0_D48C42B30AAF__INCLUDED_)
#define AFX_BOUNCEVIEW_H__AA096736_5CF7_43B5_84E0_D48C42B30AAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBounceView : public CView
{
protected: // create from serialization only
	CBounceView();
	DECLARE_DYNCREATE(CBounceView)

// Attributes
public:
	CBounceDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBounceView)
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
	virtual ~CBounceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBounceView)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BounceView.cpp
inline CBounceDoc* CBounceView::GetDocument()
   { return (CBounceDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOUNCEVIEW_H__AA096736_5CF7_43B5_84E0_D48C42B30AAF__INCLUDED_)
