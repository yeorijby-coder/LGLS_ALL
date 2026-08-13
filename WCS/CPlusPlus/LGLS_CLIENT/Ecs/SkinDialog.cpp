// SkinDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SkinDialog.h"

#include "Ecs.h"

#define MARGIN_BOX			10

#define MARGIN_MAXIMIZED	4

// CSkinDialog dialog
CSkinDialog::CSkinDialog(UINT nResourceID, CWnd* pParent /*=NULL*/)
	: CDialog(nResourceID, pParent)
{
	//{{AFX_DATA_INIT(CShapeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_CtlColorBrush.CreateSolidBrush(Global.GetRGB(IDX_RGB_BACKGROUND));

	m_nClickedBox	= CLICKED_BOX_NONE;

	m_bTracking		= FALSE;

	m_bDraging		= FALSE;
	m_bSizeCursor	= FALSE;

	m_bActive		= FALSE;

	m_cfText		= RGB(0,0,0);
	m_cfMask		= RGB(255,0,255);

	m_hBmpCapL		= NULL;
	m_hBmpCapM		= NULL;
	m_hBmpCapR		= NULL;
	m_hBmpFrmL		= NULL;
	m_hBmpFrmLB		= NULL;
	m_hBmpFrmB		= NULL;
	m_hBmpFrmR		= NULL;
	m_hBmpFrmRB		= NULL;

	m_hBmpMinimize	= NULL;
	m_hBmpMaximize	= NULL;
	m_bDisableMaximize = FALSE;
	m_bResizeReady = FALSE;
	m_szClientInit = CSize(0,0);
	m_hBmpRestore	= NULL;
	m_hBmpExit		= NULL;

	m_hIcon			= NULL;

	memset(&m_sizeCapL, 0x00, sizeof(SIZE));
	memset(&m_sizeCapM, 0x00, sizeof(SIZE));
	memset(&m_sizeCapR, 0x00, sizeof(SIZE));
	memset(&m_sizeFrmL, 0x00, sizeof(SIZE));
	memset(&m_sizeFrmLB, 0x00, sizeof(SIZE));
	memset(&m_sizeFrmB, 0x00, sizeof(SIZE));
	memset(&m_sizeFrmRB, 0x00, sizeof(SIZE));
	memset(&m_sizeFrmR, 0x00, sizeof(SIZE));
	memset(&m_sizeBox, 0x00, sizeof(SIZE));
}


CSkinDialog::~CSkinDialog()
{
	m_CtlColorBrush.DeleteObject();
	m_rgnWnd.DeleteObject();
	this->DestroyWindow();
}


void CSkinDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSkinDialog, CDialog)
	ON_MESSAGE(WM_LGLS_SAVELAYOUT, &CSkinDialog::OnLglsSaveLayout)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_NCPAINT()	
	ON_WM_NCACTIVATE()
	ON_WM_NCCALCSIZE()
	ON_WM_CTLCOLOR()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_ENTERSIZEMOVE, OnEnterSizeMove)
	ON_MESSAGE(WM_EXITSIZEMOVE, OnExitSizeMove)
	ON_WM_ACTIVATE()	
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSkinDialog message handlers

HBRUSH CSkinDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(pWnd->IsKindOf(RUNTIME_CLASS(CStatic)))
	{
		return NULL;
	}

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	// TODO:  Change any attributes of the DC here

	if( pWnd->IsKindOf(RUNTIME_CLASS(CButton)) )
	{
		return (HBRUSH)m_CtlColorBrush;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CSkinDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	m_CtlColorBrush.DeleteObject();
	m_rgnWnd.DeleteObject();
}

void CSkinDialog::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CDialog::OnNcPaint() for painting messages
	if( GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_CAPTION )
	{
		DrawFrame();
	}
}

BOOL CSkinDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// [LGLS 2026-08-12] ï¿½ï¿½Å² NC ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(OnNcCalcSize)ï¿½ï¿½ Ç¥ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Óºï¿½ï¿½ï¿½ ï¿½Î²ï¿½ï¿½ï¿½
	//   Å¬ï¿½ï¿½ï¿½Ì¾ï¿½Æ®ï¿½ï¿½ ï¿½ï¿½ï¿½Ò½ï¿½ ï¿½ï¿½ï¿½ï¿½ Å©ï¿½âº¸ï¿½ï¿½ ï¿½Û¾ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½/ï¿½Æ·ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ ï¿½ß¸ï¿½).
	//   ï¿½Î²ï¿½ ï¿½ï¿½ï¿½Ì¸ï¿½Å­ Ã¢ï¿½ï¿½ Å°ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ Å¬ï¿½ï¿½ï¿½Ì¾ï¿½Æ® Å©ï¿½â¸¦ ï¿½Ç»ì¸°ï¿½ï¿½.
	if( GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_CAPTION )
	{
		int nDx = 0, nDy = 0;
		SIZE size = Global.GetBitmapSize(IDX_BMP_CAPTION_CENTER);
		if( size.cy ) nDy += size.cy - (GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYFRAME));
		size = Global.GetBitmapSize(IDX_BMP_FRAME_LEFT);
		if( size.cx ) nDx += size.cx - GetSystemMetrics(SM_CXFRAME);
		size = Global.GetBitmapSize(IDX_BMP_FRAME_RIGHT);
		if( size.cx ) nDx += size.cx - GetSystemMetrics(SM_CXFRAME);
		size = Global.GetBitmapSize(IDX_BMP_FRAME_BOTTOM);
		if( size.cy ) nDy += size.cy - GetSystemMetrics(SM_CXFRAME);	// OnNcCalcSize ï¿½ï¿½ ï¿½ï¿½ï¿½Ï½ï¿½ ï¿½ï¿½ï¿½ï¿½
		if( nDx < 0 ) nDx = 0;
		if( nDy < 0 ) nDy = 0;
		if( nDx > 0 || nDy > 0 )
		{
			CRect rcWin; GetWindowRect(&rcWin);
			SetWindowPos(NULL, 0, 0, rcWin.Width() + nDx, rcWin.Height() + nDy,
			             SWP_NOMOVE | SWP_NOZORDER);
		}
	}
	if( !m_bDisableMaximize )
	{
		ModifyStyle(0, WS_THICKFRAME);	// [LGLS] ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ Ã¢(SC/RTV/CV ï¿½ï¿½ï¿½ï¿½)
		PostMessage(WM_LGLS_SAVELAYOUT, 0, 0);	// ï¿½ï¿½ï¿½ ï¿½Ê±ï¿½È­ ï¿½Ï·ï¿½ ï¿½ï¿½ ï¿½Ê±ï¿½ ï¿½ï¿½ï¿½Ì¾Æ¿ï¿½ ï¿½ï¿½ï¿½ï¿½
	}
	else
	{
		// [LGLS] SC/RTV/CV: ï¿½Ö¼ï¿½È­/ï¿½Ö´ï¿½È­ ï¿½ï¿½Æ° ï¿½ï¿½ï¿½ï¿½(ï¿½Ö´ï¿½È­/ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ã¼ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½×¸ï¿½ ï¿½Ü»ï¿½ ï¿½ï¿½ï¿½ï¿½)
		ModifyStyle(WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0);
		SetWindowPos(NULL, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	return TRUE;
}

LRESULT CSkinDialog::OnLglsSaveLayout(WPARAM, LPARAM)
{
	if( m_bDisableMaximize ) return 0;
	CRect rcClient; GetClientRect(&rcClient);
	if( rcClient.Width() > 0 && rcClient.Height() > 0 && GetWindow(GW_CHILD) != NULL )
	{
		m_szClientInit = rcClient.Size();
		m_vResizeHwnd.clear(); m_vResizeRect.clear();
		for( CWnd* pC = GetWindow(GW_CHILD); pC != NULL; pC = pC->GetWindow(GW_HWNDNEXT) )
		{
			CRect rc; pC->GetWindowRect(&rc); ScreenToClient(&rc);
			m_vResizeHwnd.push_back(pC->GetSafeHwnd());
			m_vResizeRect.push_back(rc);
		}
		m_bResizeReady = TRUE;
	}
	return 0;
}

void CSkinDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	if( GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_CAPTION )
	{
		CRect rc;
		GetWindowRect(&rc);

		int width = rc.Width()+1;
		int height = rc.Height()+1;

		int x = 0, y = 0;
		int round = 14;
		CRgn rgnA, rgnB;

		if( nType == SIZE_MAXIMIZED )
		{
			x = MARGIN_MAXIMIZED;
			y = MARGIN_MAXIMIZED;
			width -= MARGIN_MAXIMIZED;
			height -= MARGIN_MAXIMIZED;
		}

		// ï¿½ï¿½Ã¼ ï¿½ï¿½ï¿½ï¿½
		if( (HRGN)m_rgnWnd != NULL )
			m_rgnWnd.DeleteObject();
		m_rgnWnd.CreateRectRgn( x, y, width, height );

		// left-topï¿½ß¶ó³»±ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
		rgnA.CreateEllipticRgn(x, y, round, round);
		rgnB.CreateRectRgn(x, y, round/2, round/2);
		rgnA.CombineRgn(&rgnB, &rgnA, RGN_DIFF);

		// left-topï¿½ï¿½ï¿½ï¿½ ï¿½ß¶ï¿½
		m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnA, RGN_DIFF);
		rgnA.DeleteObject();
		rgnB.DeleteObject();

		// right-topï¿½ß¶ó³»±ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
		rgnA.CreateEllipticRgn(width-round, y, width, round);
		rgnB.CreateRectRgn(width-round/2, y, width, round/2);
		rgnA.CombineRgn(&rgnB, &rgnA, RGN_DIFF);

		// right-topï¿½ï¿½ï¿½ï¿½ ï¿½ß¶ï¿½
		m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnA, RGN_DIFF);
		rgnA.DeleteObject();
		rgnB.DeleteObject();

		// left-bottomï¿½ß¶ó³»±ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
//		rgnA.CreateEllipticRgn(x, height-round, round, height);
//		rgnB.CreateRectRgn(x, height-round/2, round/2, height);
//		rgnA.CombineRgn(&rgnB, &rgnA, RGN_DIFF);

		// left-bottomï¿½ï¿½ï¿½ï¿½ ï¿½ß¶ï¿½
//		m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnA, RGN_DIFF);
//		rgnA.DeleteObject();
//		rgnB.DeleteObject();

		// right-bottomï¿½ß¶ó³»±ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
//		rgnA.CreateEllipticRgn(width-round, height-round, width, height);
//		rgnB.CreateRectRgn(width-round/2, height-round/2, width, height);
//		rgnA.CombineRgn(&rgnB, &rgnA, RGN_DIFF);

		// right-bottomï¿½ï¿½ï¿½ï¿½ ï¿½ß¶ï¿½
//		m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnA, RGN_DIFF);
//		rgnA.DeleteObject();
//		rgnB.DeleteObject();

		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		SetWindowRgn((HRGN)m_rgnWnd, TRUE);
	}

	// [LGLS] ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ã¢: ï¿½Ú½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ä¡ (ï¿½Ê±ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ OnLglsSaveLayout)
	if( !m_bDisableMaximize && nType != SIZE_MINIMIZED && m_bResizeReady )
	{
		CRect rcClient; GetClientRect(&rcClient);
		if( m_szClientInit.cx > 0 && m_szClientInit.cy > 0 )
		{
			double sx = (double)rcClient.Width()  / (double)m_szClientInit.cx;
			double sy = (double)rcClient.Height() / (double)m_szClientInit.cy;
			for( size_t _i = 0; _i < m_vResizeHwnd.size(); _i++ )
			{
				if( !::IsWindow(m_vResizeHwnd[_i]) ) continue;
				CWnd* pC = CWnd::FromHandle(m_vResizeHwnd[_i]);
				if( pC == NULL ) continue;
				CRect r = m_vResizeRect[_i];
				CRect nr( (int)(r.left*sx), (int)(r.top*sy), (int)(r.right*sx), (int)(r.bottom*sy) );
				pC->MoveWindow(&nr);
			}
			Invalidate();
		}
	}

	// [LGLS 2026-08-13] ë¦¬ì‚¬ì´ì¦ˆ í›„ ìŠ¤í‚¨ í”„ë ˆìž„ì„ ë‹¤ì‹œ ê·¸ë¦°ë‹¤.
	//   ê¸°ì¡´ì—ëŠ” í´ë¼ì´ì–¸íŠ¸ë§Œ ê°±ì‹ í•´ì„œ, ì»¤ì§„ ë¹„í´ë¼ì´ì–¸íŠ¸(ìº¡ì…˜/í…Œë‘ë¦¬) ì˜ì—­ì—
	//   ì´ì „ í™”ë©´ ìž”ìƒ(ê²€ì€ ì ì„ )ì´ ë‚¨ì•˜ë‹¤.
	if( (GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_CAPTION) && nType != SIZE_MINIMIZED )
	{
		RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
		DrawFrame();
	}
}

BOOL CSkinDialog::OnNcActivate(BOOL bActive)
{
	// TODO: Add your message handler code here and/or call default

	if( GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_CAPTION )
	{
		DrawFrame();
		return TRUE;
	}
	else
	{
		return CDialog::OnNcActivate(bActive);
	}
}

void CSkinDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: Add your message handler code here and/or call default

	if( GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_CAPTION )
	{
		int revision=0;
		
		// Recalculate top
		SIZE size = Global.GetBitmapSize(IDX_BMP_CAPTION_CENTER);
		if( size.cy )
		{
			revision = size.cy - (GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYFRAME));
			lpncsp->rgrc[0].top += revision;
		}

		// Recalculate left
		size = Global.GetBitmapSize(IDX_BMP_FRAME_LEFT);
		if( size.cx )
		{
			revision = size.cx - GetSystemMetrics(SM_CXFRAME);
			lpncsp->rgrc[0].left += revision;
		}

		// Recalculate right
		size = Global.GetBitmapSize(IDX_BMP_FRAME_RIGHT);
		if( size.cx )
		{
			revision = size.cx - GetSystemMetrics(SM_CXFRAME);
			lpncsp->rgrc[0].right -= revision;
		}

		// Recalculate bottom
		size = Global.GetBitmapSize(IDX_BMP_FRAME_BOTTOM);
		if( size.cy )
		{
			revision = size.cy - GetSystemMetrics(SM_CXFRAME);
			lpncsp->rgrc[0].bottom -= revision;
		}
	}

	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CSkinDialog::DrawFrame()
{
	if( !(GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_CAPTION) )
		return;

	int x=0,y=0,x2=0,y2=0,cx=0,cy=0;
	CRect rc;
	GetWindowRect(&rc);
	BOOL bIconic = IsIconic();
	BOOL bZoomed = IsZoomed();

	//Non-Client ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½Â´ï¿½.
	CWindowDC dc(this);	

	// È£È¯ï¿½ï¿½ ï¿½Ö´ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
	CDC BufferDC, tmpDC;
	BufferDC.CreateCompatibleDC(&dc);
	tmpDC.CreateCompatibleDC(&dc);

	// ï¿½Þ¸ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	CBitmap bmpBuffer, *pOldBitmap1;
	HGDIOBJ hgdiobj;

	// È£È¯ï¿½ï¿½ ï¿½Ö´ï¿½ ï¿½Þ¸ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï¿ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	bmpBuffer.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	pOldBitmap1 = (CBitmap *)BufferDC.SelectObject(&bmpBuffer);

	// [LGLS 2026-08-13] ¹öÆÛ ÀüÃ¼¸¦ ÇÁ·¹ÀÓ Å×µÎ¸®»öÀ¸·Î ¼±Ã¤¿ò.
	//   Windows 10 ÆÐµðµå º¸´õ ¶§¹®¿¡ ½ÇÁ¦ ºñÅ¬¶óÀÌ¾ðÆ®°¡ ½ºÅ² ºñÆ®¸Êº¸´Ù ¸î px µÎ²¨¿ö,
	//   ºñÆ®¸ÊÀÌ ¸ø µ¤´Â Æ´¿¡ ¸®»çÀÌÁî µå·¡±× ÀÜ»ó(Á¡¼±)ÀÌ ³²¾Ò´Ù. ¼±Ã¤¿òÀ¸·Î Æ´À» ±ÕÀÏÇÏ°Ô °¡¸°´Ù.
	BufferDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(0, 0, 0));

	// Left Ä¸ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	x = 0;
	y = 0;
	cx = m_sizeCapL.cx;
	cy = m_sizeCapL.cy;
	hgdiobj = tmpDC.SelectObject( m_hBmpCapL );
	BufferDC.BitBlt(x, y, cx, cy, &tmpDC, 0, 0, SRCCOPY);

	// Center Ä¸ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	x += cx;
	cx = rc.Width() - m_sizeCapL.cx - m_sizeCapR.cx;
	cy = m_sizeCapM.cy;
	tmpDC.SelectObject( m_hBmpCapM );
	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, m_sizeCapM.cx, m_sizeCapM.cy, SRCCOPY);

	// Right Ä¸ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	x += cx;
	cx = m_sizeCapR.cx;
	cy = m_sizeCapR.cy;
	tmpDC.SelectObject( m_hBmpCapR );
	BufferDC.BitBlt(x, y, cx, cy, &tmpDC, 0, 0, SRCCOPY);

	// Left ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	x = 0;
	y = m_sizeCapL.cy;
	cx = m_sizeFrmL.cx;
	cy = rc.Height() - m_sizeCapL.cy - m_sizeFrmB.cy;
	tmpDC.SelectObject( m_hBmpFrmL );
	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, m_sizeFrmL.cx, m_sizeFrmL.cy, SRCCOPY);

	// Left-Bottom ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	y += cy;
	cx = m_sizeFrmLB.cx;
	cy = m_sizeFrmLB.cy;
	tmpDC.SelectObject( m_hBmpFrmLB );
	BufferDC.BitBlt(x, y, cx, cy, &tmpDC, 0, 0, SRCCOPY);

	// Bottom ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	x += cx;
	cx = rc.Width() - m_sizeFrmL.cx - m_sizeFrmR.cx;
	cy = m_sizeFrmB.cy;
	tmpDC.SelectObject( m_hBmpFrmB );
	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, m_sizeFrmB.cx, m_sizeFrmB.cy, SRCCOPY);	

	// Right ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	x = rc.Width()-m_sizeFrmR.cx;
	y = m_sizeCapR.cy;
	cx = m_sizeFrmR.cx;
	cy = rc.Height() - m_sizeCapR.cy - m_sizeFrmRB.cy;
	tmpDC.SelectObject( m_hBmpFrmR );
	BufferDC.StretchBlt(x, y, cx, cy, &tmpDC, 0, 0, m_sizeFrmR.cx, m_sizeFrmR.cy, SRCCOPY);

	// Right-Bottom ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	x = rc.Width()-m_sizeFrmRB.cx;
	y += cy;
	cx = m_sizeFrmRB.cx;
	cy = m_sizeFrmRB.cy;
	tmpDC.SelectObject( m_hBmpFrmRB );
	BufferDC.BitBlt(x, y, cx, cy, &tmpDC, 0, 0, SRCCOPY);	

	if( !bIconic )
	{
		if( bZoomed )
			y = MARGIN_MAXIMIZED;
		else
			y = 0;

		// Close Box ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
		tmpDC.SelectObject(m_hBmpExit);
		x = rc.Width()-MARGIN_BOX-m_sizeBox.cx;
		BufferDC.BitBlt(x, y, m_sizeBox.cx, m_sizeBox.cx, &tmpDC, 0, 0, SRCCOPY);

		if( !m_bDisableMaximize )
		{
		if( !bZoomed )
		{
			// Maximize Box ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
			tmpDC.SelectObject(m_hBmpMaximize);
			x = x-m_sizeBox.cx;			
			BufferDC.BitBlt(x, y, m_sizeBox.cx, m_sizeBox.cx, &tmpDC, 0, 0, SRCCOPY);
		}
		}
		else
		{
			// Restore Box ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
			tmpDC.SelectObject(m_hBmpRestore);
			x = x-m_sizeBox.cx;			
			BufferDC.BitBlt(x, y, m_sizeBox.cx, m_sizeBox.cx, &tmpDC, 0, 0, SRCCOPY);
		}

		// Minimize Box ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
		tmpDC.SelectObject(m_hBmpMinimize);
		x = x-m_sizeBox.cx;		
		BufferDC.BitBlt(x, y, m_sizeBox.cx, m_sizeBox.cx, &tmpDC, 0, 0, SRCCOPY);
	}
	else
	{
		// Close Box ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
		tmpDC.SelectObject(m_hBmpExit);
		x = rc.Width()-MARGIN_BOX-m_sizeBox.cx;
		BufferDC.BitBlt(x, 0, m_sizeBox.cx, m_sizeBox.cx, &tmpDC, 0, 0, SRCCOPY);//m_sizeBox.cx, m_sizeBox.cx, m_cfMask);

		// Maximize Box ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
		tmpDC.SelectObject(m_hBmpMaximize);
		x = x-m_sizeBox.cx;		
		BufferDC.BitBlt(x, 0, m_sizeBox.cx, m_sizeBox.cx, &tmpDC, 0, 0, SRCCOPY);//m_sizeBox.cx, m_sizeBox.cx, m_cfMask);

		// Restore Box ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
		tmpDC.SelectObject(m_hBmpRestore);
		x = x-m_sizeBox.cx;		
		BufferDC.BitBlt(x, 0, m_sizeBox.cx, m_sizeBox.cx, &tmpDC, 0, 0, SRCCOPY);//m_sizeBox.cx, m_sizeBox.cx, m_cfMask);
	}

	// ï¿½Þ¸ï¿½ DC ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	tmpDC.SelectObject(hgdiobj);
	tmpDC.DeleteDC();

	// ï¿½Ø½ï¿½Æ® ï¿½ï¿½ï¿½ï¿½å¸¦ ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½)
	BufferDC.SetBkMode(TRANSPARENT);

	// ï¿½âº» ï¿½Û²ï¿½ï¿½ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	CFont *pOldFont = (CFont *)BufferDC.SelectObject(GetFont());

	// ï¿½Ø½ï¿½Æ®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	BufferDC.SetTextColor(m_cfText);

	// ï¿½ï¿½ï¿½Ú¿ï¿½ï¿½ï¿½ ï¿½ï¿½Âµï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ç¥ ï¿½ï¿½ï¿½ï¿½
	BufferDC.SetTextAlign(TA_LEFT|TA_TOP);


	
	if( !bIconic )
	{
		// ï¿½Ø½ï¿½Æ®ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½
		CString str;
		GetWindowText(str);
		BufferDC.TextOut(15, 10, str);

		// Å¬ï¿½ï¿½ï¿½Ì¾ï¿½Æ® ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ñ´ï¿½.
		// [LGLS 2026-08-13] ºñÆ®¸Ê Å©±â ±âÁØ °¡Á¤ ´ë½Å '½ÇÁ¦' Å¬¶óÀÌ¾ðÆ® ÁÂÇ¥·Î Á¦¿Ü.
		//   ÆÐµðµå º¸´õ¸¸Å­ ¾î±ß³ª ÀÖ´ø Æ´ÀÌ ÀÌÁ¦ ÇÁ·¹ÀÓ ¼±Ã¤¿òÀ¸·Î µ¤ÀÎ´Ù.
		CRect rcCliScr;
		GetClientRect(&rcCliScr);
		ClientToScreen(&rcCliScr);
		x  = rcCliScr.left   - rc.left;
		y  = rcCliScr.top    - rc.top;
		x2 = rcCliScr.right  - rc.left;
		y2 = rcCliScr.bottom - rc.top;
		dc.ExcludeClipRect(x, y, x2, y2);
	}
	else
	{
		// ï¿½Ø½ï¿½Æ®ï¿½ï¿½ ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½ï¿½ï¿½
		CString str;
		GetWindowText(str);

		if( str.GetLength() > 4 )
			str = str.Left(4) + _T("...");

		BufferDC.TextOut(15, (rc.Height()-dc.GetTextExtent(str).cy)/2, str);
	}

	// ï¿½Þ¸ï¿½ DCï¿½ï¿½ ï¿½×·ï¿½ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ È­ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½	
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &BufferDC, 0, 0, SRCCOPY);

	// ï¿½Þ¸ï¿½ DC ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	//BufferDC.SelectObject(pOldFont);
	BufferDC.SelectObject(pOldBitmap1);
	BufferDC.DeleteDC();
}

BOOL CSkinDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest == HTBOTTOM)
	{
		int a =0;
	}

	if( nHitTest == HTBOTTOM	|| nHitTest == HTTOP || 
		nHitTest == HTLEFT		|| nHitTest == HTRIGHT ||
		nHitTest == HTBOTTOMLEFT|| nHitTest == HTBOTTOMRIGHT ||
		nHitTest == HTTOPLEFT	|| nHitTest == HTTOPRIGHT )
		m_bSizeCursor = TRUE;
	else
		m_bSizeCursor = FALSE;

	if( m_nClickedBox != CLICKED_BOX_NONE )
		return TRUE;

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CSkinDialog::OnEnterSizeMove( WPARAM wParam, LPARAM lParam)
{
	if( m_bSizeCursor )
		return 0L;

	if( !m_bDraging )
	{
		m_bDraging = TRUE;
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
		Global.SetLayeredWindowAttributes(m_hWnd, 0, 176, LWA_ALPHA);
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}

	m_bTracking = FALSE;
	CDialog::OnEnterSizeMove();
    return 0L;
}


LRESULT CSkinDialog::OnExitSizeMove( WPARAM wParam, LPARAM lParam)
{
	if( m_bDraging )
	{
		m_bDraging = FALSE;
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}

	m_bTracking = FALSE;
	CDialog::OnExitSizeMove();
    return 0L;
}

void CSkinDialog::SetCtlColor(COLORREF crCtlColor)
{
	m_CtlColorBrush.DeleteObject();
	m_CtlColorBrush.CreateSolidBrush(crCtlColor);
	RedrawWindow();
}

void CSkinDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	m_bActive = (nState ? TRUE : FALSE);

	if( GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_CAPTION )
	{
		if( m_bActive )
		{
			//m_hIcon = Global.GetIcon(IDX_ICON_SKIN,ICON24);
			m_cfText = Global.GetRGB(IDX_RGB_FONT_CAPTION);
		}
		else
		{
			//m_hIcon = Global.GetIcon(IDX_ICON_SKIN_INACTIVE,ICON24);
			m_cfText = Global.GetRGB(IDX_RGB_FONT_CAPTION_INACTIVE);
		}

		DrawFrame();
	}
	else
	{
		CDialog::OnActivate(nState, pWndOther, bMinimized);
	}

	// TODO: Add your message handler code here
	
}

BOOL CSkinDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(	pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4 ||
		pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	else if(pMsg->wParam == VK_ESCAPE)
	{
		::ShowWindow(m_hWnd, SW_HIDE); 
		return TRUE;
	}
	//
	/*else if(pMsg->wParam == VK_LEFT)
	{
		if (GET_KEYDOWN_EVENT_PASS() == TRUE)
		{
			return true;
		}

		CRect Rect;
		GetWindowRect(&Rect); 
		SetWindowPos(&wndTop, Rect.left - 15, Rect.top, 
			Rect.Width(), Rect.Height(), 
			SWP_SHOWWINDOW);
		return TRUE;
	}
	else if(pMsg->wParam == VK_RIGHT)
	{
		if (GET_KEYDOWN_EVENT_PASS() == TRUE)
		{
			return true;
		}

		CPoint pt;
		::GetCursorPos(&pt);
		HDC h_dc = ::GetDC(m_hWnd); 
		HGDIOBJ h_bitmap = ::GetCurrentObject(h_dc,OBJ_BITMAP);
		BITMAP bmp_info;
		::GetObject(h_bitmap, sizeof(BITMAP), &bmp_info);
		int a =0;
  		CRect Rect;
  		GetWindowRect(&Rect); 
  		SetWindowPos(&wndTop, Rect.left + 15, Rect.top, 
  			Rect.Width(), Rect.Height(), 
  			SWP_SHOWWINDOW);
  		return TRUE;
	}
	else if(pMsg->wParam == VK_UP)
	{
		if (GET_KEYDOWN_EVENT_PASS() == TRUE)
		{
			return true;
		}

		CRect Rect;
		GetWindowRect(&Rect); 
		SetWindowPos(&wndTop, Rect.left, Rect.top - 15, 
			Rect.Width(), Rect.Height(), 
			SWP_SHOWWINDOW);
		return TRUE;
	}
	else if(pMsg->wParam == VK_DOWN)
	{
		if (GET_KEYDOWN_EVENT_PASS() == TRUE)
		{
			return true;
		}

		CRect Rect;
		GetWindowRect(&Rect); 
		SetWindowPos(&wndTop, Rect.left, Rect.top + 15, 
			Rect.Width(), Rect.Height(), 
			SWP_SHOWWINDOW);
		return TRUE;
	}*/
	else if( pMsg->message == WM_NCLBUTTONDOWN ||	// None Client ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ì½º ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Æ°ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½
			 pMsg->message == WM_NCMOUSEMOVE ||		// None Client ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ì½ºï¿½ï¿½ ï¿½Ìµï¿½ï¿½ï¿½ ï¿½ï¿½
			 pMsg->message == WM_LBUTTONUP ||		// None Client ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Æ´Ñ°ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ì½º ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Æ°ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½
			 pMsg->message == WM_NCLBUTTONUP ||		// None Client ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ì½º ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Æ°ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½
			 pMsg->message == WM_NCMOUSELEAVE )		// None Client ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Æ´Ñ°ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ì½ºï¿½ï¿½ ï¿½Ìµï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½
	{
		if( GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_CAPTION )
		{
			CRect rc;
			GetWindowRect(&rc);

			CRect rcExit, rcMaximize, rcMinimize;

			int x=0,y=0,cx=0,cy=0;

			SIZE sizeBox = Global.GetBitmapSize(IDX_BMP_BOX_EXIT);

			BOOL bZoomed = IsZoomed();				// ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ì°¡ ï¿½Ö´ï¿½È­ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½..
			BOOL bIconic = IsIconic();				// ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ì°¡ ï¿½Ö¼ï¿½È­ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½..
			BOOL bLButton = KEYDOWN(VK_LBUTTON);	// ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ì½º ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Æ°ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö´ï¿½ï¿½ï¿½..

			y = rc.top+MARGIN_MAXIMIZED;

			rcExit.left = rc.right-MARGIN_BOX-sizeBox.cx;
			rcExit.top = y;
			rcExit.right = rcExit.left+sizeBox.cx;
			rcExit.bottom = rcExit.top+sizeBox.cy;

			rcMaximize.left = rcExit.left-sizeBox.cx;
			rcMaximize.top = y;
			rcMaximize.right = rcMaximize.left+sizeBox.cx;
			rcMaximize.bottom = rcMaximize.top+sizeBox.cy;

			rcMinimize.left = rcMaximize.left-sizeBox.cx;
			rcMinimize.top = y;
			rcMinimize.right = rcMinimize.left+sizeBox.cx;
			rcMinimize.bottom = rcMinimize.top+sizeBox.cy;

			if( m_bDisableMaximize )	// [LGLS] ï¿½Ö´ï¿½È­ ï¿½ï¿½ï¿½ï¿½: ï¿½Ö¼ï¿½È­ï¿½ï¿½ ï¿½Ö´ï¿½È­ ï¿½Ú¸ï¿½ï¿½ï¿½, ï¿½Ö´ï¿½È­ Å¬ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
			{
				rcMinimize.left = rcMaximize.left;
				rcMinimize.right = rcMaximize.right;
				rcMaximize.SetRectEmpty();
			}

			switch( pMsg->message )
			{
			case WM_NCLBUTTONDOWN :
				if( rcExit.PtInRect(pMsg->pt) )
				{
					SetCapture();
					m_nClickedBox = CLICKED_BOX_EXIT;
					m_hBmpExit = Global.GetBitmap(IDX_BMP_BOX_EXIT_ON);
					DrawFrame();
					return TRUE;
				}
				else if( rcMaximize.PtInRect(pMsg->pt) )
				{
					SetCapture();
					m_nClickedBox = CLICKED_BOX_MAXIMIZE;
					if( IsZoomed() )
						m_hBmpRestore = Global.GetBitmap(IDX_BMP_BOX_RESTORE_ON);
					else
						m_hBmpMaximize = Global.GetBitmap(IDX_BMP_BOX_MAXMIZE_ON);
					DrawFrame();
					return TRUE;
				}
				else if( rcMinimize.PtInRect(pMsg->pt) )
				{
					SetCapture();
					m_nClickedBox = CLICKED_BOX_MINIMIZE;
					if( bIconic )
						m_hBmpRestore = Global.GetBitmap(IDX_BMP_BOX_RESTORE_ON);
					else
						m_hBmpMinimize = Global.GetBitmap(IDX_BMP_BOX_MINIMIZE_ON);
					DrawFrame();
					return TRUE;
				}
				else
				{
					m_nClickedBox = CLICKED_BOX_NONE;

					if( bZoomed )
						return TRUE;
				}
				break;

			case WM_NCMOUSELEAVE :
				m_bTracking = FALSE;
				if( m_nClickedBox == CLICKED_BOX_NONE )
				{					
					m_hBmpMinimize = Global.GetBitmap(IDX_BMP_BOX_MINIMIZE);
					m_hBmpMaximize = Global.GetBitmap(IDX_BMP_BOX_MAXMIZE);
					m_hBmpRestore = Global.GetBitmap(IDX_BMP_BOX_RESTORE);
					m_hBmpExit = Global.GetBitmap(IDX_BMP_BOX_EXIT);
					m_nClickedBox = CLICKED_BOX_NONE;					
					DrawFrame();
				}
				break;

			case WM_NCMOUSEMOVE :				
				if( !m_bTracking )
				{	// ï¿½ï¿½ï¿½ì½ºï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ì¸¦ ï¿½ï¿½ï¿½î³ªï¿½ï¿½ WM_NCMOUSELEAVE ï¿½Ìºï¿½Æ®ï¿½ï¿½ ï¿½ß»ï¿½ï¿½Ïµï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
					m_bTracking = TRUE;
					TRACKMOUSEEVENT t = { sizeof(TRACKMOUSEEVENT), TME_NONCLIENT|TME_LEAVE, m_hWnd, 0 };
					::TrackMouseEvent(&t);					
				}

				if( m_nClickedBox != CLICKED_BOX_NONE )
					break;
				
				m_hBmpMinimize = Global.GetBitmap(IDX_BMP_BOX_MINIMIZE);
				m_hBmpMaximize = Global.GetBitmap(IDX_BMP_BOX_MAXMIZE);
				m_hBmpRestore = Global.GetBitmap(IDX_BMP_BOX_RESTORE);
				m_hBmpExit = Global.GetBitmap(IDX_BMP_BOX_EXIT);				
				
				if( rcExit.PtInRect(pMsg->pt) )
				{
					m_hBmpExit = Global.GetBitmap(IDX_BMP_BOX_EXIT_OVER);
				}
				else if( rcMaximize.PtInRect(pMsg->pt) )
				{
					if( IsZoomed() )
						m_hBmpRestore = Global.GetBitmap(IDX_BMP_BOX_RESTORE_OVER);
					else
						m_hBmpMaximize = Global.GetBitmap(IDX_BMP_BOX_MAXMIZE_OVER);
				}
				else if( rcMinimize.PtInRect(pMsg->pt) )
				{
					if( bIconic )
						m_hBmpRestore = Global.GetBitmap(IDX_BMP_BOX_RESTORE_OVER);
					else
						m_hBmpMinimize = Global.GetBitmap(IDX_BMP_BOX_MINIMIZE_OVER);
				}				
				DrawFrame();
				break;

			case WM_LBUTTONUP :
			case WM_NCLBUTTONUP :
				if( m_nClickedBox != CLICKED_BOX_NONE )
					ReleaseCapture();

				if( rcExit.PtInRect(pMsg->pt) )
				{
					if( m_nClickedBox == CLICKED_BOX_EXIT )
					{
						PostMessage(WM_CLOSE, 0, 0);
					}
				}
				else if( rcMaximize.PtInRect(pMsg->pt) )
				{
					if( m_nClickedBox == CLICKED_BOX_MAXIMIZE )
					{
						if( IsZoomed() )
							ShowWindow(SW_RESTORE);
						//else
						//	ShowWindow(SW_MAXIMIZE);
					}
				}
				else if( rcMinimize.PtInRect(pMsg->pt) )
				{
					if( m_nClickedBox == CLICKED_BOX_MINIMIZE )
					{
						if( bIconic )
							ShowWindow(SW_RESTORE);
						else
							ShowWindow(SW_MINIMIZE);
					}
				}
				else
				{					
					m_hBmpMinimize = Global.GetBitmap(IDX_BMP_BOX_MINIMIZE);
					m_hBmpMaximize = Global.GetBitmap(IDX_BMP_BOX_MAXMIZE);
					m_hBmpRestore = Global.GetBitmap(IDX_BMP_BOX_RESTORE);
					m_hBmpExit = Global.GetBitmap(IDX_BMP_BOX_EXIT);
					DrawFrame();
				}

				m_nClickedBox = CLICKED_BOX_NONE;
				break;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CSkinDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default		

	lpMMI->ptMinTrackSize.x = 240;								// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö¼ï¿½ ï¿½Êºï¿½ 
	lpMMI->ptMinTrackSize.y = 100;								// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö¼ï¿½ ï¿½ï¿½ï¿½ï¿½

#if 0
	/*--------------------------------------------------------------------------------------------
	ï¿½Æ·ï¿½ ï¿½Ò½ï¿½ï¿½ï¿½ Main ï¿½ï¿½ï¿½ï¿½ï¿½ È­ï¿½ï¿½ï¿½ï¿½ ï¿½Û¾ï¿½ Ç¥ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½/ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Û¾ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½î¸¦ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ APIï¿½ï¿½ 
	ï¿½ï¿½ï¿½ï¿½Ï¿ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´ï¿½È­ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡/ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Õ´Ï´ï¿½.
	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Í¸ï¿½ ï¿½ï¿½ï¿½ï¿½Ñ´Ù¸ï¿½ ï¿½Æ·ï¿½ï¿½ï¿½ APIï¿½Æ´ï¿½ ï¿½ï¿½ï¿½ï¿½Íºï¿½ ï¿½Û¾ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ APIï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Ø¾ï¿½ ï¿½Õ´Ï´ï¿½.
	ï¿½×¸ï¿½ï¿½ï¿½ ï¿½Ø´ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ò¼ÓµÇ´ï¿½ ï¿½ï¿½ï¿½ï¿½Í´ï¿½ ï¿½Þ¶ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½Ö½ï¿½ï¿½Ï´ï¿½.
	ï¿½Ì·ï¿½ ï¿½ï¿½ï¿½ï¿½ Ã³ï¿½ï¿½ï¿½Ø¾ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ç·ï¿½ ï¿½ì¸®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ È£ï¿½ï¿½Ç´ï¿½ 
	OnCreateï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Å¸ï¿½Ï¸ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ï¿ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ø°ï¿½ï¿½Ï°Ú½ï¿½ï¿½Ï´ï¿½.
	--------------------------------------------------------------------------------------------*/
	CRect rc;	
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);	

	lpMMI->ptMaxPosition.x = -MARGIN_MAXIMIZED;					// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´ï¿½È­ ï¿½ï¿½ Xï¿½ï¿½ ï¿½ï¿½Ç¥
	lpMMI->ptMaxPosition.y = -MARGIN_MAXIMIZED;					// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´ï¿½È­ ï¿½ï¿½ Yï¿½ï¿½ ï¿½Â·ï¿½
	lpMMI->ptMaxSize.x = rc.Width()+MARGIN_MAXIMIZED*2;			// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´ï¿½È­ ï¿½ï¿½ ï¿½Êºï¿½
	lpMMI->ptMaxSize.y = rc.Height()+MARGIN_MAXIMIZED*2;		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´ï¿½È­ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	lpMMI->ptMaxTrackSize.x = rc.Width()+MARGIN_MAXIMIZED*2;	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´ï¿½È­ ï¿½ï¿½ ï¿½Êºï¿½
	lpMMI->ptMaxTrackSize.y = rc.Height()+MARGIN_MAXIMIZED*2;	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´ï¿½È­ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
#endif

	CDialog::OnGetMinMaxInfo(lpMMI);
}

int CSkinDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ã½ï¿½ï¿½ï¿½ ï¿½Ú½ï¿½ï¿½ï¿½ Ç¥ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ê±ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Å¸ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	// ï¿½ï¿½ï¿½ Maximize Ã³ï¿½ï¿½ï¿½ï¿½ ï¿½Ï±ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ò½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ã½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Å¸ï¿½ï¿½ï¿½ï¿½ Enable ï¿½ï¿½ï¿½Ñ¾ï¿½ ï¿½ï¿½
	ModifyStyle( WS_SYSMENU, 0);	
	 
	// TODO:  Add your specialized creation code here
	m_cfText = Global.GetRGB(IDX_RGB_FONT_CAPTION);
	m_cfMask = Global.GetRGB(IDX_RGB_MASK);

	// ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Â´ï¿½.
	m_sizeCapL		= Global.GetBitmapSize(IDX_BMP_CAPTION_LEFT);
	m_sizeCapM		= Global.GetBitmapSize(IDX_BMP_CAPTION_CENTER);
	m_sizeCapR		= Global.GetBitmapSize(IDX_BMP_CAPTION_RIGHT);
	m_sizeFrmL		= Global.GetBitmapSize(IDX_BMP_FRAME_LEFT);
	m_sizeFrmLB		= Global.GetBitmapSize(IDX_BMP_FRAME_LEFT_BOTTOM);
	m_sizeFrmB		= Global.GetBitmapSize(IDX_BMP_FRAME_BOTTOM);
	m_sizeFrmRB		= Global.GetBitmapSize(IDX_BMP_FRAME_RIGHT_BOTTOM);
	m_sizeFrmR		= Global.GetBitmapSize(IDX_BMP_FRAME_RIGHT);
	m_sizeBox		= Global.GetBitmapSize(IDX_BMP_BOX_EXIT);

	// ï¿½ï¿½ ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ ï¿½Úµï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Â´ï¿½.
	m_hBmpCapL		= Global.GetBitmap(IDX_BMP_CAPTION_LEFT);
	m_hBmpCapM		= Global.GetBitmap(IDX_BMP_CAPTION_CENTER);
	m_hBmpCapR		= Global.GetBitmap(IDX_BMP_CAPTION_RIGHT);
	m_hBmpFrmL		= Global.GetBitmap(IDX_BMP_FRAME_LEFT);
	m_hBmpFrmLB		= Global.GetBitmap(IDX_BMP_FRAME_LEFT_BOTTOM);
	m_hBmpFrmB		= Global.GetBitmap(IDX_BMP_FRAME_BOTTOM);
	m_hBmpFrmR		= Global.GetBitmap(IDX_BMP_FRAME_RIGHT);
	m_hBmpFrmRB		= Global.GetBitmap(IDX_BMP_FRAME_RIGHT_BOTTOM);

	m_hBmpMinimize	= Global.GetBitmap(IDX_BMP_BOX_MINIMIZE);
	m_hBmpMaximize	= Global.GetBitmap(IDX_BMP_BOX_MAXMIZE);
	m_hBmpRestore	= Global.GetBitmap(IDX_BMP_BOX_RESTORE);
	m_hBmpExit		= Global.GetBitmap(IDX_BMP_BOX_EXIT);

	//
	m_hIcon			= Global.GetIcon(IDX_ICON_SKIN,ICON24);			

	return 0;
}

void CSkinDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, Global.GetRGB(IDX_RGB_BACKGROUND));
}

bool CSkinDialog::GET_KEYDOWN_EVENT_PASS()
{
	return FALSE;
	int nID = GetFocus()->GetDlgCtrlID();
	CWnd* cWnd = GetDlgItem(nID);
	if (cWnd == NULL)
	{
		return FALSE;
	}
	CString strTemp = _T("");

	LPWSTR lpwSTR = (LPWSTR)(LPCWSTR)strTemp;
	::GetClassName(cWnd->m_hWnd, lpwSTR, 100);

	strTemp=(LPWSTR)(LPCWSTR)_T("Edit");
	if (lpwSTR == strTemp)
	{
		int a =0;
		return TRUE;
	}
	strTemp=(LPWSTR)(LPCWSTR)_T("SPR32AU70_SpreadSheet");
	if(lpwSTR == strTemp)
	{
		int a = 0;
		return TRUE;
	}
	else
		return FALSE;
}