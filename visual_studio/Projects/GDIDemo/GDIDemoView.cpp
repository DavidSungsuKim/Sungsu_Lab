// GDIDemoView.cpp : implementation of the CGDIDemoView class
//

#include "stdafx.h"
#include "GDIDemo.h"

#include "GDIDemoDoc.h"
#include "GDIDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGDIDemoView

IMPLEMENT_DYNCREATE(CGDIDemoView, CView)

BEGIN_MESSAGE_MAP(CGDIDemoView, CView)
	//{{AFX_MSG_MAP(CGDIDemoView)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIDemoView construction/destruction

CGDIDemoView::CGDIDemoView()
{
	// TODO: add construction code here

}

CGDIDemoView::~CGDIDemoView()
{
}

BOOL CGDIDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGDIDemoView drawing

void CGDIDemoView::OnDraw(CDC* pDC)
{
	CGDIDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGDIDemoView printing

BOOL CGDIDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDIDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDIDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGDIDemoView diagnostics

#ifdef _DEBUG
void CGDIDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CGDIDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDIDemoDoc* CGDIDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDIDemoDoc)));
	return (CGDIDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGDIDemoView message handlers

void CGDIDemoView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// 0.1초마다 WM_TIMER 메시지가 발생하도록 설정
	SetTimer(0, 100, NULL);
}

void CGDIDemoView::OnDestroy() 
{
	CView::OnDestroy();
	
	// 타이머를 꺼줌
	KillTimer(0);
}

#define OBJ_LINE			0
#define OBJ_RECTANGLE	1
#define OBJ_ELLIPSE		2
#define OBJ_ROUNDRECT	3
#define OBJ_CHORD		4
#define OBJ_PIE			5

void CGDIDemoView::OnTimer(UINT nIDEvent) 
{
    CClientDC dc(this);  					// 화면 출력을 위한 DC 클래스
    CRect rect;	      						// 화면 크기를 얻기 위한 변수
    CBrush brush, *pOldBrush;  				// 브러시 오브젝트
    CPen pen, *pOldPen;       				// 펜 오브젝트

    int  x1,y1,x2,y2,x3,y3,x4,y4;  			// 화면 좌표
    int  r,g,b;                  			// 컬러
    int  nObject;               			// 그래픽 타입
	
    GetClientRect(rect);  					// 화면의 크기를 얻는 함수

    r = rand() % 255;    					// 컬러를 랜덤하게 설정
    g = rand() % 255;
    b = rand() % 255;
    brush.CreateSolidBrush(RGB(r,g,b));  	// 브러시 생성
    pOldBrush = (CBrush *)dc.SelectObject(&brush); // DC에 브러시 선택

    r = rand() % 255;						// 컬러를 랜덤하게 설정
    g = rand() % 255;
    b = rand() % 255;
    pen.CreatePen(PS_SOLID, 5, RGB(r,g,b));	// 펜 생성
    pOldPen = (CPen *)dc.SelectObject(&pen);// DC에 펜 선택

    x1 = rand() % rect.right;				// 좌표를 랜덤하게 설정
    y1 = rand() % rect.bottom;
    x2 = rand() % rect.right;
    y2 = rand() % rect.bottom;
    x3 = rand() % rect.right;
    y3 = rand() % rect.bottom;
    x4 = rand() % rect.right;
    y4 = rand() % rect.bottom;

    nObject = rand() % 6;   				// 그래픽 타입을 랜덤하게 설정

    switch(nObject)
    {
        case OBJ_LINE:    					// 라인 그리기
            dc.MoveTo(x1,y1);
            dc.LineTo(x2,y2);
            break;

			case OBJ_RECTANGLE:  			// 사각형 그리기
            dc.Rectangle(x1,y1,x2,y2);
            break;

        case OBJ_ELLIPSE:					// 타원 그리기
            dc.Ellipse(x1,y1,x2,y2);
            break;

        case OBJ_ROUNDRECT:					// 둥근 사각형 그리기
            dc.RoundRect(x1,y1,x2,y2,x3,y3);
            break;

        case OBJ_CHORD:						// 현 그리기
            dc.Chord(x1,y1,x2,y2,x3,y3,x4,y4);
            break;

        case OBJ_PIE:						// 파이 그리기
            dc.Pie(x1,y1,x2,y2,x3,y3,x4,y4);
            break;
    }

    dc.SelectObject(pOldPen);				// DC 복원
    dc.SelectObject(pOldBrush);				// DC 복원

    CView::OnTimer(nIDEvent);
}
