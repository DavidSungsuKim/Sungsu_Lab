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
	
	// 0.05�ʸ��� WM_TIMER �޽����� �߻��ϵ��� ����
	SetTimer(0, 50, NULL);
}

void CBounceView::OnDestroy() 
{
	CView::OnDestroy();
	
	// Ÿ�̸Ӹ� ����
	KillTimer(0);
}

#define R 20
#define STEP 5

void CBounceView::OnTimer(UINT nIDEvent) 
{
	CClientDC dc(this);
	static int nX=R, nY=R;				// ���� ���� ��ġ
	static int nCX=STEP, nCY=STEP;		// ���� �̵� ����

	CRect rect;							// Ŭ���̾�Ʈ ������ ũ��
	GetClientRect(&rect);				// Ŭ���̾�Ʈ ������ ũ�⸦ ����

	// ���� ���� ����� ���� ��� �귯�ø� ����
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Ellipse(nX-R,nY-R,nX+R,nY+R);	// �� �׸���

	// x ��ǥ ����
	if(nX < R) nCX = STEP;
	else if(nX > rect.Width()-R) nCX = -STEP;
	nX += nCX;
	
	//y ��ǥ ����
	if(nY < R) nCY = STEP;
	else if(nY > rect.Height()-R) nCY = -STEP;
	nY += nCY;
	
	// ���� ���� �׸��� ���� ������ �귯�� ����
	CBrush brush, *pOldBrush;
	brush.CreateSolidBrush(RGB(255,0,0));

	// DC�� ������ �귯�� ����
	pOldBrush = (CBrush *)dc.SelectObject(&brush);
	dc.Ellipse(nX-R,nY-R,nX+R,nY+R);	// �� �׸���
	dc.SelectObject(pOldBrush);			// DC ����
	CView::OnTimer(nIDEvent);
}
