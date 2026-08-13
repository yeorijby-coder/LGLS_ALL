// ExtLabel.cpp : implementation file
//

#include "stdafx.h"

#include "ExtLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtLabel

CExtLabel::CExtLabel()
{
	m_colorBack = BLACK;
	m_colorText = BLUE;

	m_bTransparent = FALSE;
	m_bFont3DIntaglio = FALSE;
	m_bFont3DShadow = TRUE;
	m_bOutLine = FALSE;

	InitFont();

	m_bUseBitmap = FALSE;
	m_pBMPArray = NULL;
	m_nNumOfBitmap = 0;
	m_nBmpNoToDraw = 0;
}

CExtLabel::~CExtLabel()
{
	m_Font.DeleteObject();

	// Bitmap
	if(m_pBMPArray != NULL)
		delete [] m_pBMPArray;

	m_pBMPArray = NULL;
}


BEGIN_MESSAGE_MAP(CExtLabel, CStatic)
	//{{AFX_MSG_MAP(CExtLabel)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtLabel message handlers


BOOL CExtLabel::OnEraseBkgnd(CDC* pDC) 
{
//	RECT rc;
//	GetClientRect(&rc);
//	pDC->FillSolidRect(&rc, RGB(255, 0, 0));

//	return 0;
	pDC;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CExtLabel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect( &rcClient );

	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcClient);
	dc.SelectClipRgn(&rgn);
	
	CBrush brush;
	CBrush* pOldbrush;
	brush.CreateSolidBrush(m_colorBack);
	pOldbrush = (CBrush*)dc.SelectObject(&brush);
	
	CPen pen;
	CPen* pOldPen;
	pen.CreatePen(PS_SOLID, 1, BLACK);
	pOldPen = (CPen*)dc.SelectObject(&pen);

	if(!m_bTransparent)
	{
		if(m_bOutLine)
		{
			dc.Rectangle(&rcClient);
		}
		else
			dc.FillSolidRect( &rcClient, m_colorBack );
	}
	else
		dc.FillSolidRect( &rcClient, GetSysColor(COLOR_3DFACE));

//	else
//		nOldBkMode = dc.SetBkMode(TRANSPARENT);
	
	
	if(m_bUseBitmap)
	{
		DrawBitmapOnBackGround( &dc, m_nBmpNoToDraw);
	}
//	dc.FillSolidRect( &rcClient, GetSysColor(COLOR_3DFACE) );

	//////////////////////////////////////////////////////////////////////////
	// paint text
	//////////////////////////////////////////////////////////////////////////
	
	CString strText;
	GetWindowText(strText);

	DWORD style = GetStyle();
	DWORD dwFlags = 0;
				
	switch (style & SS_TYPEMASK)
	{
	case SS_RIGHT: 
		dwFlags = DT_RIGHT | DT_WORDBREAK; 
		break; 
	case SS_CENTER: 
		dwFlags = SS_CENTER | DT_WORDBREAK;
		break;
	case SS_LEFTNOWORDWRAP: 
		dwFlags = DT_LEFT; 
		break;
	default: // treat other types as left
	case SS_LEFT: 
		dwFlags = DT_LEFT | DT_WORDBREAK; 
		break;
	}	
	// Added to expand tabs...
	if(strText.Find(_T('\t')) != -1)
		dwFlags |= DT_EXPANDTABS;
	// If the text centered make an assumtion that
	// the will want to center verticly as well
	if (style & SS_CENTERIMAGE){
		dwFlags = DT_CENTER;
		// Apply 
		if (strText.Find(_T("\r\n")) == -1){
			dwFlags |= DT_VCENTER;
			// And because DT_VCENTER only works with single lines
			dwFlags |= DT_SINGLELINE; 
		}
	}	
	dwFlags |= (DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);


	COLORREF clrOldText;
	INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
	
	m_Font.DeleteObject();
//	InitFont();
	m_Font.CreateFontIndirect(&m_logfont);
	CFont * pOldFont = dc.SelectObject(&m_Font);

	CRect rc;
			
	if(m_bFont3DShadow)
	{
		rc = rcClient;
		rc.top = 0;
		if (strText.Find(_T("-")) == -1)
			rc.left += 2;
		else
			rc.left += 3;
		
		clrOldText = dc.SetTextColor( GetSysColor(COLOR_3DSHADOW) );
		dc.DrawText(strText,rc,dwFlags);
	}

	if(m_bFont3DIntaglio)
	{
		rc = rcClient;
		rc.top -= 1;
		rc.left += 3;
		clrOldText = dc.SetTextColor( WHITE );
		dc.DrawText(strText,rc,dwFlags);
	}
	
	rc = rcClient;
	rc.left += 1;
	rc.top -= 2;
	dc.SetTextColor( m_colorText );
	dc.DrawText(strText,rc,dwFlags);
	
	dc.SelectObject(pOldbrush);
	dc.SelectObject(pOldPen);
	dc.SelectObject( pOldFont );
	m_Font.DeleteObject();
	dc.SetBkMode( nOldBkMode );
	dc.SetTextColor( clrOldText );
}


/////////////////////////////////////////
//
void CExtLabel::SetBackColor(COLORREF color)
{
	m_colorBack = color;

	UpdateSurface();
}

void CExtLabel::SetTextColor(COLORREF color)
{
	m_colorText = color;

	UpdateSurface();
}

void CExtLabel::SetTransparent(BOOL bSW)
{
	m_bTransparent = bSW;
}

void CExtLabel::SetFontShadow(BOOL bShadow)
{
	m_bFont3DShadow = bShadow;
}

void CExtLabel::SetFontIntaglio(BOOL bIntaglio)
{
	m_bFont3DIntaglio = bIntaglio;
}

void CExtLabel::UseOutLine(BOOL bSW)
{
	m_bOutLine = bSW;
}

void CExtLabel::InitFont()
{
	memset(&m_logfont,0,sizeof(LOGFONT));
	m_logfont.lfQuality = PROOF_QUALITY;
	m_logfont.lfHeight = 25;
	m_logfont.lfWeight = 700;
	lstrcpy(m_logfont.lfFaceName, _T("Arial Black"));
}

void CExtLabel::SetFont(const CString& strFont, BYTE byCharSet /* = ANSI_CHARSET*/)
{
	m_logfont.lfCharSet = byCharSet;

	_tcscpy(m_logfont.lfFaceName,strFont);
	
	ReconstructFont();	
	UpdateSurface();	
}

void CExtLabel::SetFontSize(int textsize, BOOL bold)
{
	const int POINTS_PER_INCH = 72;
	int nDot;
	CClientDC dc(this);

	nDot = (textsize * dc.GetDeviceCaps(LOGPIXELSY)) / POINTS_PER_INCH;

	m_logfont.lfHeight = nDot;
	
	if(bold)
		m_logfont.lfWeight = FW_BOLD;
	else
		m_logfont.lfWeight = FW_NORMAL;

	ReconstructFont();
	UpdateSurface();
}

void CExtLabel::SetFontAngle(long angle)
{
	m_logfont.lfOrientation = angle;
	m_logfont.lfEscapement = angle;

	ReconstructFont();
	UpdateSurface();
//	Invalidate();
}

void CExtLabel::ReconstructFont()
{
	m_Font.DeleteObject();
	BOOL bCreated = m_Font.CreateFontIndirect(&m_logfont);
}

void CExtLabel::UpdateSurface()
{
	CRect rc;
	GetClientRect(rc);
	RedrawWindow();
}

void CExtLabel::InitBitmapArray(int numofbitmap)
{
	m_bUseBitmap = TRUE;
	m_nNumOfBitmap = numofbitmap;
	m_pBMPArray = new CBitmap[m_nNumOfBitmap];
}

void CExtLabel::SetBmpNoToDraw(int BmpNo)
{
	m_nBmpNoToDraw = BmpNo;
}

void CExtLabel::LoadBitmapFromRes(UINT ResName, int bitmapNo)
{
	(m_pBMPArray+bitmapNo-1)->Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(ResName), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	
	BITMAP	bitmapbits;
	(m_pBMPArray+bitmapNo-1)->GetBitmap(&bitmapbits);

	m_ImageSize.cy=bitmapbits.bmHeight;
	m_ImageSize.cx=bitmapbits.bmWidth;
}

void CExtLabel::DrawBitmapOnBackGround(CDC *pDC, int numofbitmap)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap *pOB1;
	pOB1 = (CBitmap *)MemDC.SelectObject((m_pBMPArray+numofbitmap-1));
	
	pDC->BitBlt( 0, 0, m_ImageSize.cx, m_ImageSize.cy, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOB1);
}


///////////////////////////////////////
//
CColor::CColor()
{
	m_colorBack = 0;
	m_colorText = 0;
}

CColor::CColor(COLORREF colorB, COLORREF colorT)
{
	m_colorBack = colorB;
	m_colorText = colorT;
}

