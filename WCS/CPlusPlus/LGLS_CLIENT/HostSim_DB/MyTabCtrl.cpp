// MyTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"
#include "MyTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl

CMyTabCtrl::CMyTabCtrl()
{
	m_PageColor			= RGB(226,233,252);
	m_BackColor			= RGB(226,233,252);
	m_UnSelectedColor	= PALE_GRAY;			//RGB(183,198,222);
	m_SelectedColor		= BLUE;					//RGB(226,233,252);
	m_ErrorColor		= RED;					//RGB(226,233,252);
	m_SelectFontColor	= RGB(0,0,0);
	m_UnSelectFontColor = RGB(0,0,0);

	for (int i = 0 ; i < ECS_ALL_TAB_COUNT ; i++) 
	{
		m_nErrorTab[i] = 0;
	}

}

CMyTabCtrl::~CMyTabCtrl()
{

}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CMyTabCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl message handlers
//void CMyTabCtrl::Refresh()
//{
//	OnPaint();
//}

void CMyTabCtrl::SetColor(COLORREF pSelectColor, COLORREF pUnSelectColor, COLORREF pBackColor, COLORREF pPageColor, COLORREF pSelectFontColor, COLORREF pUnSelectFontColor)
{
	m_PageColor			= pPageColor;
	m_BackColor			= pBackColor;
	m_UnSelectedColor	= pUnSelectColor;
	m_SelectedColor		= pSelectColor;
	m_SelectFontColor	= pSelectFontColor;
	m_UnSelectFontColor = pUnSelectFontColor;
}

void CMyTabCtrl::DrawItemBorder(LPDRAWITEMSTRUCT lpdis, int nDraw)
{
//	BOOL bSelected = nDraw;//(lpdis->itemID == (UINT)GetCurSel());
	BOOL bBackTabs = TRUE;

	CRect rItem(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC);
	CBrush pBrush;
	COLORREF m_Color;

	CString sTemp;

	if(nDraw == 1)		
		m_Color = m_ErrorColor;
	else if(nDraw == 2)	
		m_Color = m_SelectedColor;
	else				
		m_Color = m_UnSelectedColor;

	pBrush.CreateSolidBrush(m_Color);
	pDC->FillRect(rItem, &pBrush);
	pBrush.DeleteObject();
	if(nDraw != 0)
	{
		fnDrawLine(pDC, RGB(255,255,255), CPoint(rItem.left+2,rItem.top),CPoint(rItem.right-1,rItem.top), 1);
		fnDrawLine(pDC, RGB(255,255,255), CPoint(rItem.left,rItem.top+2),CPoint(rItem.left,rItem.bottom), 1);
		fnDrawLine(pDC, RGB(255,255,255), CPoint(rItem.left+2,rItem.top),CPoint(rItem.left,rItem.top+2), 1);
		fnDrawLine(pDC, RGB(66,65,66), CPoint(rItem.right-1,rItem.bottom-2),CPoint(rItem.right-1,rItem.top), 1);
		fnDrawLine(pDC, RGB(66,65,66), CPoint(rItem.right,rItem.bottom-2),CPoint(rItem.right,rItem.top+1), 1);
		fnDrawLine(pDC, RGB(130,132,130), CPoint(rItem.right-1,rItem.bottom-2),CPoint(rItem.right-1,rItem.top+1), 1);
		m_Color = m_SelectFontColor;
	}
	else
	{
		fnDrawLine(pDC, RGB(255,255,255), CPoint(rItem.left+2,rItem.top),CPoint(rItem.right-2,rItem.top), 1);
		fnDrawLine(pDC, RGB(255,255,255), CPoint(rItem.left,rItem.top+2),CPoint(rItem.left,rItem.bottom), 1);
		fnDrawLine(pDC, RGB(255,255,255), CPoint(rItem.left+2,rItem.top),CPoint(rItem.left,rItem.top+2), 1);
		fnDrawLine(pDC, RGB(66,65,66), CPoint(rItem.right-1,rItem.bottom-1),CPoint(rItem.right-1,rItem.top+1), 1);
		fnDrawLine(pDC, RGB(66,65,66), CPoint(rItem.right-2,rItem.bottom-1),CPoint(rItem.right-2,rItem.top), 1);
		fnDrawLine(pDC, RGB(130,132,130), CPoint(rItem.right-2,rItem.bottom-1),CPoint(rItem.right-2,rItem.top+1), 1);
		m_Color = m_UnSelectFontColor;
	}
	pDC->SetBkMode(TRANSPARENT);

	TC_ITEM     tci;
	tci.mask        = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText     = sTemp.GetBuffer(100);
	tci.cchTextMax  = 99;
	GetItem(lpdis->itemID, &tci);
	sTemp.ReleaseBuffer();
	CFont* oldFont = fnSetFont(pDC,12,0,FALSE,FALSE,FALSE,"±¼¸²Ã¼");
	LOGFONT pLogFont;
	font.GetLogFont(&pLogFont);
	int y = ((rItem.bottom - rItem.top - pLogFont.lfHeight) / 2) + 1;
	rItem.top += y;
	rItem.left += 6;
	rItem.bottom = rItem.top + pLogFont.lfHeight;

	pDC->SetTextColor(m_Color);
	pDC->DrawText(sTemp, rItem, DT_NOPREFIX | DT_LEFT);
	font.DeleteObject();
	
}

void CMyTabCtrl::DrawMainBorder(LPDRAWITEMSTRUCT lpdis)
{
	CRect rBorder(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC);

	CBrush pBrush;
	pBrush.CreateSolidBrush(m_PageColor);
	pDC->FillRect(rBorder, &pBrush);
	pBrush.DeleteObject();

	fnDrawLine(pDC, RGB(255,255,255), CPoint(rBorder.left,rBorder.top),CPoint(rBorder.right,rBorder.top), 1);
	fnDrawLine(pDC, RGB(255,255,255), CPoint(rBorder.left,rBorder.top),CPoint(rBorder.left,rBorder.bottom), 1);
	fnDrawLine(pDC, RGB(130,132,130), CPoint(rBorder.left+1,rBorder.bottom-2),CPoint(rBorder.right-2,rBorder.bottom-2), 1);
	fnDrawLine(pDC, RGB(130,132,130), CPoint(rBorder.right-2,rBorder.bottom-2),CPoint(rBorder.right-2,rBorder.top+1), 1);
	fnDrawLine(pDC, RGB(66,65,66), CPoint(rBorder.left,rBorder.bottom-1),CPoint(rBorder.right-1,rBorder.bottom-1), 1);
	fnDrawLine(pDC, RGB(66,65,66), CPoint(rBorder.right-1,rBorder.bottom-1),CPoint(rBorder.right-1,rBorder.top), 1);
}

void CMyTabCtrl::fnDrawLine(CDC* pDC, COLORREF pColor, CPoint pStartPoint, CPoint pEndPoint, int pLineWidth)
{
	CPen pPen, *pOldPen;
	pPen.CreatePen(PS_SOLID, pLineWidth, pColor);
	pOldPen = pDC->SelectObject(&pPen);

	pDC->MoveTo(pStartPoint.x, pStartPoint.y);
	pDC->LineTo(pEndPoint.x, pEndPoint.y);

	pDC->SelectObject(pOldPen);
	pPen.DeleteObject();
}

CFont* CMyTabCtrl::fnSetFont(CDC *pDC,int pszHeight, int pszWidth, BOOL pszBold, BOOL pszItalic, BOOL pszUnderLine, char *pszFontName)
{
	int m_Width;
	if(pszBold)		m_Width = FW_BOLD;
	else			m_Width = FW_NORMAL;
	font.CreateFont(pszHeight,pszWidth,
					0,0,
					m_Width,
					pszItalic,
					pszUnderLine,
					0,
					ANSI_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					DEFAULT_PITCH,
					CA2CT(pszFontName));
	CFont *oldFont = pDC->SelectObject(&font);

	return oldFont;
}

void CMyTabCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	int nTab = GetItemCount();
	int nSel = GetCurSel();

	if(!nTab)	
		return;

	DRAWITEMSTRUCT dis;

	dis.CtlType = ODT_TAB;
	dis.CtlID = GetDlgCtrlID();
	dis.hwndItem = GetSafeHwnd();
	dis.hDC = dc.GetSafeHdc();
	dis.itemAction = ODA_DRAWENTIRE;

	CRect rect, pageRect;
	GetClientRect(&dis.rcItem);
	pageRect = dis.rcItem;
	AdjustRect(FALSE, pageRect);
	dis.rcItem.top = pageRect.top - 2;

//	fnCaptureBack(&dc);

	DrawMainBorder(&dis);
	while(nTab--)
	{
		if(nTab != nSel)
		{
			dis.itemID = nTab;
			dis.itemState = 0;

			GetItemRect(nTab, &dis.rcItem);
//			dis.rcItem.bottom = 2;
			DrawItemBorder(&dis, m_nErrorTab[nTab]);
		}
		else
		{
			dis.itemID = nSel;
			dis.itemState = ODS_SELECTED;

			GetItemRect(nSel, &dis.rcItem);
			dis.rcItem.left -= 2;
			dis.rcItem.right += 1;
			dis.rcItem.bottom += 1;
			dis.rcItem.top -= 2;
			DrawItemBorder(&dis, m_nErrorTab[nTab]);
		}
	}	
	// Do not call CTabCtrl::OnPaint() for painting messages
}
