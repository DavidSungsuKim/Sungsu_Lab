// ROP2View.cpp : implementation of the CROP2View class
//

#include "stdafx.h"
#include "ROP2.h"

#include "ROP2Doc.h"
#include "ROP2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CROP2View

IMPLEMENT_DYNCREATE(CROP2View, CView)

BEGIN_MESSAGE_MAP(CROP2View, CView)
	//{{AFX_MSG_MAP(CROP2View)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CROP2View construction/destruction

CROP2View::CROP2View()
{
	// TODO: add construction code here

}

CROP2View::~CROP2View()
{
}

BOOL CROP2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CROP2View drawing

void CROP2View::OnDraw(CDC* pDC)
{
	CROP2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CROP2View printing

BOOL CROP2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CROP2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CROP2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CROP2View diagnostics

#ifdef _DEBUG
void CROP2View::AssertValid() const
{
	CView::AssertValid();
}

void CROP2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CROP2Doc* CROP2View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CROP2Doc)));
	return (CROP2Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CROP2View message handlers

void CROP2View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_Pt1 = m_Pt2 = point;
	
	CView::OnLButtonDown(nFlags, point);
}

void CROP2View::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(nFlags & MK_LBUTTON)
	{
		CClientDC dc(this);
		
		//레스터 오퍼레이션을 R2_XORPEN으로 설정
		dc.SetROP2(R2_XORPEN);	

		//흰색 펜을 선택
		dc.SelectStockObject(WHITE_PEN); 

		//이전에 그려진 선을 지웁니다.
		dc.MoveTo(m_Pt1);
		dc.LineTo(m_Pt2);
		
		//새로 선을 그립니다.
		dc.MoveTo(m_Pt1);
		dc.LineTo(point);

		//현재 점을 다음에 지워질 점으로 저장해 둡니다.
		m_Pt2 = point;
	}
	
	CView::OnMouseMove(nFlags, point);
}
