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
	
	// 0.1�ʸ��� WM_TIMER �޽����� �߻��ϵ��� ����
	SetTimer(0, 100, NULL);
}

void CGDIDemoView::OnDestroy() 
{
	CView::OnDestroy();
	
	// Ÿ�̸Ӹ� ����
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
    CClientDC dc(this);  					// ȭ�� ����� ���� DC Ŭ����
    CRect rect;	      						// ȭ�� ũ�⸦ ��� ���� ����
    CBrush brush, *pOldBrush;  				// �귯�� ������Ʈ
    CPen pen, *pOldPen;       				// �� ������Ʈ

    int  x1,y1,x2,y2,x3,y3,x4,y4;  			// ȭ�� ��ǥ
    int  r,g,b;                  			// �÷�
    int  nObject;               			// �׷��� Ÿ��
	
    GetClientRect(rect);  					// ȭ���� ũ�⸦ ��� �Լ�

    r = rand() % 255;    					// �÷��� �����ϰ� ����
    g = rand() % 255;
    b = rand() % 255;
    brush.CreateSolidBrush(RGB(r,g,b));  	// �귯�� ����
    pOldBrush = (CBrush *)dc.SelectObject(&brush); // DC�� �귯�� ����

    r = rand() % 255;						// �÷��� �����ϰ� ����
    g = rand() % 255;
    b = rand() % 255;
    pen.CreatePen(PS_SOLID, 5, RGB(r,g,b));	// �� ����
    pOldPen = (CPen *)dc.SelectObject(&pen);// DC�� �� ����

    x1 = rand() % rect.right;				// ��ǥ�� �����ϰ� ����
    y1 = rand() % rect.bottom;
    x2 = rand() % rect.right;
    y2 = rand() % rect.bottom;
    x3 = rand() % rect.right;
    y3 = rand() % rect.bottom;
    x4 = rand() % rect.right;
    y4 = rand() % rect.bottom;

    nObject = rand() % 6;   				// �׷��� Ÿ���� �����ϰ� ����

    switch(nObject)
    {
        case OBJ_LINE:    					// ���� �׸���
            dc.MoveTo(x1,y1);
            dc.LineTo(x2,y2);
            break;

			case OBJ_RECTANGLE:  			// �簢�� �׸���
            dc.Rectangle(x1,y1,x2,y2);
            break;

        case OBJ_ELLIPSE:					// Ÿ�� �׸���
            dc.Ellipse(x1,y1,x2,y2);
            break;

        case OBJ_ROUNDRECT:					// �ձ� �簢�� �׸���
            dc.RoundRect(x1,y1,x2,y2,x3,y3);
            break;

        case OBJ_CHORD:						// �� �׸���
            dc.Chord(x1,y1,x2,y2,x3,y3,x4,y4);
            break;

        case OBJ_PIE:						// ���� �׸���
            dc.Pie(x1,y1,x2,y2,x3,y3,x4,y4);
            break;
    }

    dc.SelectObject(pOldPen);				// DC ����
    dc.SelectObject(pOldBrush);				// DC ����

    CView::OnTimer(nIDEvent);
}
