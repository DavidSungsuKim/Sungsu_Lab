// BounceView.cpp : implementation of the CBounceView class
//

#include "stdafx.h"
#include "Bounce.h"

#include "BounceDoc.h"
#include "BounceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBounceView

IMPLEMENT_DYNCREATE(CBounceView, CView)

BEGIN_MESSAGE_MAP(CBounceView, CView)
	//{{AFX_MSG_MAP(CBounceView)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBounceView construction/destruction

CBounceView::CBounceView()
{
	// TODO: add construction code here

}

CBounceView::~CBounceView()
{
}

BOOL CBounceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBounceView drawing

void CBounceView::OnDraw(CDC* pDC)
{
	CBounceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CBounceView printing

BOOL CBounceView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBounceView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBounceView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBounceView diagnostics

#ifdef _DEBUG
void CBounceView::AssertValid() const
{
	CView::AssertValid();
}

void CBounceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBounceDoc* CBounceView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBounceDoc)));
	return (CBounceDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBounceView message handlers

void CBounceView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// 0.05초마다 WM_TIMER 메시지가 발생하도록 설정
	SetTimer(0, 50, NULL);
}

void CBounceView::OnDestroy() 
{
	CView::OnDestroy();
	
	// 타이머를 꺼줌
	KillTimer(0);
}

#define R 20
#define STEP 5

void CBounceView::OnTimer(UINT nIDEvent) 
{
	CClientDC dc(this);
	static int nX=R, nY=R;				// 현재 공의 위치
	static int nCX=STEP, nCY=STEP;		// 공의 이동 방향

	CRect rect;							// 클라이언트 영역의 크기
	GetClientRect(&rect);				// 클라이언트 영역의 크기를 얻음

	// 이전 공을 지우기 위해 흰색 브러시를 선택
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Ellipse(nX-R,nY-R,nX+R,nY+R);	// 원 그리기

	// x 좌표 변경
	if(nX < R) nCX = STEP;
	else if(nX > rect.Width()-R) nCX = -STEP;
	nX += nCX;
	
	//y 좌표 변경
	if(nY < R) nCY = STEP;
	else if(nY > rect.Height()-R) nCY = -STEP;
	nY += nCY;
	
	// 새로 공을 그리기 위해 빨간색 브러시 생성
	CBrush brush, *pOldBrush;
	brush.CreateSolidBrush(RGB(255,0,0));

	// DC에 빨간색 브러시 선택
	pOldBrush = (CBrush *)dc.SelectObject(&brush);
	dc.Ellipse(nX-R,nY-R,nX+R,nY+R);	// 원 그리기
	dc.SelectObject(pOldBrush);			// DC 복원
	CView::OnTimer(nIDEvent);
}
