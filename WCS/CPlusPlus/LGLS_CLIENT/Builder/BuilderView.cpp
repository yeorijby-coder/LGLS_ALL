// BuilderView.cpp : implementation of the CBuilderView class
//

#include "stdafx.h"
#include "Builder.h"

#include "BuilderDoc.h"
#include "BuilderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBuilderView

IMPLEMENT_DYNCREATE(CBuilderView, CFormView)

BEGIN_MESSAGE_MAP(CBuilderView, CFormView)
	//{{AFX_MSG_MAP(CBuilderView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuilderView construction/destruction

CBuilderView::CBuilderView()
	: CFormView(CBuilderView::IDD)
{
	//{{AFX_DATA_INIT(CBuilderView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CBuilderView::~CBuilderView()
{
}

void CBuilderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBuilderView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CBuilderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CBuilderView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CBuilderDoc* pDoc = GetDocument();
	pDoc->m_buildManager.OnInitialUpdate(this);
}

/////////////////////////////////////////////////////////////////////////////
// CBuilderView printing

BOOL CBuilderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBuilderView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBuilderView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CBuilderView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CBuilderView diagnostics

#ifdef _DEBUG
void CBuilderView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBuilderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CBuilderDoc* CBuilderView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBuilderDoc)));
	return (CBuilderDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBuilderView message handlers

void CBuilderView::OnDraw(CDC* pDC) 
{
	CBuilderDoc* pDoc = GetDocument();
	pDoc->m_buildManager.OnDraw(this, pDC);
}

void CBuilderView::OnSize(UINT nType, int cx, int cy) 
{
	CBuilderDoc* pDoc = GetDocument();
	pDoc->m_buildManager.OnSize(this, nType, cx, cy);

	CFormView::OnSize(nType, cx, cy);
}

void CBuilderView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CBuilderDoc* pDoc = GetDocument();
	pDoc->m_bMoveFlag = TRUE;
	pDoc->m_PrevPoint = point;
	pDoc->m_tChecktime = CTime::GetCurrentTime();
	pDoc->m_buildManager.OnButtonDown(this, nFlags, point);
 
	CFormView::OnLButtonDown(nFlags, point);
}

void CBuilderView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CBuilderDoc* pDoc = GetDocument();
	pDoc->m_bMoveFlag = FALSE;
	pDoc->m_buildManager.OnButtonUp(this, nFlags, point);

	CFormView::OnLButtonUp(nFlags, point);
}

void CBuilderView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
// 	CFPSpreadTestDlg dlg;
// 	dlg.DoModal();
	
	CFormView::OnLButtonDblClk(nFlags, point);
}

void CBuilderView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CBuilderDoc* pDoc = GetDocument();
	pDoc->m_buildManager.OnKeyDown(this, nChar, nRepCnt, nFlags);

	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBuilderView::OnFileOpen() 
{
	CBuilderDoc* pDoc = GetDocument();
	pDoc->m_buildManager.OnFileOpen(this);
}

void CBuilderView::OnFileSave() 
{
	CBuilderDoc* pDoc = GetDocument();
	pDoc->m_buildManager.OnFileSave(this);
}

void CBuilderView::OnFileSaveAs() 
{
	OnFileSave();
}

void CBuilderView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CBuilderView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBuilderDoc* pDoc = GetDocument();
	pDoc->m_buildManager.OnMouseWheel(this, nFlags, zDelta, pt);

	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CBuilderView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBuilderDoc* pDoc = GetDocument();
	pDoc->m_buildManager.OnOriginalSize(this);

	CFormView::OnRButtonUp(nFlags, point);
}


void CBuilderView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBuilderDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CTimeSpan tTimeSpan;
	tTimeSpan = CTime::GetCurrentTime() - pDoc->m_tChecktime;
	if (tTimeSpan.GetTotalSeconds() < 1)
		return;

	if (pDoc->m_bMoveFlag == TRUE)
	{
		if (tTimeSpan.GetTotalSeconds() < 1)
			return;

		if (pDoc->m_PrevPoint != point)
		{
			if (pDoc->m_PrevPoint.x > point.x)
			{
				if (pDoc->m_PrevPoint.x - point.x > 3)
				{
					pDoc->m_buildManager.OnKeyDown(this, VK_LEFT, 0, 0);			// VK_RIGHT
					pDoc->m_PrevPoint.x = point.x;
				}
			}
			else if(pDoc->m_PrevPoint.x < point.x)
			{
				if (point.x - pDoc->m_PrevPoint.x > 3)
				{
					pDoc->m_buildManager.OnKeyDown(this, VK_RIGHT, 0, 0);			// VK_LEFT
					pDoc->m_PrevPoint.x = point.x;
				}
			}
			if (pDoc->m_PrevPoint.y > point.y)
			{
				if (pDoc->m_PrevPoint.y - point.y > 5)
				{
					pDoc->m_buildManager.OnKeyDown(this, VK_UP, 0, 0);				// VK_DOWN
					pDoc->m_PrevPoint.y = point.y;
				}
			}
			else if(pDoc->m_PrevPoint.y < point.y)
			{
				if (point.y - pDoc->m_PrevPoint.y > 5)
				{
					pDoc->m_buildManager.OnKeyDown(this, VK_DOWN, 0, 0);			// VK_UP
					pDoc->m_PrevPoint.y = point.y;
				}
			}
		}
	}

	CFormView::OnMouseMove(nFlags, point);
}
