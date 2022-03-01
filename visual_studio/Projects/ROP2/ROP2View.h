// ROP2View.h : interface of the CROP2View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROP2VIEW_H__8E0DAD3F_FE82_4289_8216_1BB111C6A683__INCLUDED_)
#define AFX_ROP2VIEW_H__8E0DAD3F_FE82_4289_8216_1BB111C6A683__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CROP2View : public CView
{
protected: // create from serialization only
	CROP2View();
	DECLARE_DYNCREATE(CROP2View)

// Attributes
public:
	CROP2Doc* GetDocument();
	CPoint m_Pt1, m_Pt2;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CROP2View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CROP2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CROP2View)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ROP2View.cpp
inline CROP2Doc* CROP2View::GetDocument()
   { return (CROP2Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROP2VIEW_H__8E0DAD3F_FE82_4289_8216_1BB111C6A683__INCLUDED_)
