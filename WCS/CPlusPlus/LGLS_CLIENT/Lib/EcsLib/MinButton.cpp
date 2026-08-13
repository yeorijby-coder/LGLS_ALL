// MinButton.cpp : implementation file
//

#include "stdafx.h"
#include "MinButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMinButton

CMinButton::CMinButton()
{
	m_bDefault = FALSE;
	m_nOldState = 0;
	m_nOldAction = 0;

	m_nTextOffsetFromImage = 8;
	m_bUseOffset = TRUE;

	memset(&m_logFont,0, sizeof(LOGFONT));
	m_nTextHeight = 0;
	m_bTextBold = FALSE;
}

CMinButton::~CMinButton()
{
}

BEGIN_MESSAGE_MAP(CMinButton, CButton)
	//{{AFX_MSG_MAP(CMinButton)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMinButton message handlers

inline void CMinButton::Redraw()
{
	if( m_hWnd != NULL ) Invalidate();
}

BOOL CMinButton::SetDefaultButton( BOOL bState )
{
	CDialog *pDialog = (CDialog *)GetOwner();
	ASSERT( pDialog->IsKindOf( RUNTIME_CLASS( CDialog ) ) );

	pDialog->SetDefID( GetDlgCtrlID() );

	BOOL bPrevious = m_bDefault;
	m_bDefault = bState;

	Redraw();

	// Return previous state
	return bPrevious;
}

int CMinButton::SetTextOffset( int nPixels ) 
{ 
	int nPrevious = m_nTextOffsetFromImage;

	//m_bUseOffset = TRUE; 
	//m_nTextOffsetFromImage = nPixels; 

	Redraw();

	return nPrevious;
}

CPoint CMinButton::SetTextPos( CPoint p ) 
{ 
	CPoint pointPrevious = m_pointText;

	m_bUseOffset = FALSE; 
	m_pointText = p; 

	Redraw();

	return pointPrevious;
}

//	Centering a point helper function
void CMinButton::CheckPointForCentering( CPoint &p, int nWidth, int nHeight )
{
	CRect	rectControl;
	GetClientRect( rectControl );

	if( p.x == MINBUTTON_CENTER )
		p.x = ( ( rectControl.Width() - nWidth ) >> 1 );
	if( p.y == MINBUTTON_CENTER )
		p.y = ( ( rectControl.Height() - nHeight ) >> 1 );
}

void CMinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	CRect	rectControl( lpDrawItemStruct->rcItem );
	
	UINT	nOffset = 1;			// For position adjustment of a pushed button
	UINT	nFrameStyle=0;
	BOOL	bDRAWFOCUSONLY = FALSE;	// Optimize a bit
	int		nStateFlag;				// Normal or Disabled
	
	UINT	nNewState = lpDrawItemStruct->itemState;
	UINT	nNewAction = lpDrawItemStruct->itemAction;

	//	Find out what state the control and set some drawing flags 
	//	according to the state.
	if ( nNewState & ODS_SELECTED)
	{
		nFrameStyle = DFCS_PUSHED;
		nOffset += 1;
	}

	if( nNewState & ODS_DISABLED )
		nStateFlag = DSS_DISABLED;
	else
		nStateFlag = DSS_NORMAL;

	// If only the focus is changing, don't redraw the whole control
	if (nNewAction == ODA_FOCUS )
		bDRAWFOCUSONLY = TRUE;

	if( !bDRAWFOCUSONLY )
	{
		//	Draw button frame
		pDC->DrawFrameControl(&rectControl, DFC_BUTTON, DFCS_BUTTONPUSH | nFrameStyle); //DFCS_BUTTONPUSH | nFrameStyle);DFCS_FLAT


		CRect rect;
		rect = rectControl;
		rect.right -=4;
		rect.bottom -=4;

		CPoint pt(2,2);
		m_GradationRect.GetDC(pDC);
		m_GradationRect.SetDrawArea(pt, rect);
		m_GradationRect.SetGradationType(m_Gradation_Type);
		m_GradationRect.Draw();


		//	Get control text
		CString		strTitle;
		this->GetWindowText(strTitle);

		//	Draw Text
		if ( !strTitle.IsEmpty() )
		{
			CPoint pt;
			CSize sizeText;
			int nOffsetFixY;

			CString strTmp;
			CFont *pOldFont = NULL;

			if( m_nTextHeight )
			{
				m_nTextHeight =   
				m_logFont.lfHeight = m_nTextHeight;

				//m_logFont.lfWidth = 11;
				m_logFont.lfEscapement = 0;
				m_logFont.lfOrientation = 0;

				if( m_bTextBold ) m_logFont.lfWeight = FW_BOLD;
				else m_logFont.lfWeight = FW_NORMAL;

				m_logFont.lfItalic = FALSE;
				m_logFont.lfUnderline = FALSE;
				m_logFont.lfStrikeOut = FALSE;
				m_logFont.lfCharSet = DEFAULT_CHARSET;
				m_logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
				m_logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
				m_logFont.lfQuality = PROOF_QUALITY;//DEFAULT_QUALITY;
	//			m_logFont.lfPitchAndFamily = FIXED_PITCH;			//DEFAULT_PITCH
				m_logFont.lfPitchAndFamily = DEFAULT_PITCH;

				m_newFont.DeleteObject();
				if( m_newFont.CreateFontIndirect(&m_logFont) )
					pOldFont = pDC->SelectObject(&m_newFont);
			}

			//if( strTitle.Find('\r\n') == -1 )
			if( strTitle.Find(_T("\r\n")) == -1 )
			{
				sizeText = pDC->GetTextExtent(strTitle);

				if(m_bUseOffset)
				{
					pt.x = MINBUTTON_CENTER;
					pt.y = MINBUTTON_CENTER; 
				}
				else
					pt = m_pointText;

				//	If we are centering the text vertically, it looks best of we
				//	center based on the height of the text, then move it up 1 more pixel
				nOffsetFixY = pt.y == 0 ? -1 : 0;

				CheckPointForCentering( pt, sizeText.cx, sizeText.cy );

				pt.Offset( nOffset, nOffset + nOffsetFixY );

				pDC->SetTextColor(COLORREF(m_textColor));
				pDC->SetBkMode (TRANSPARENT);

				pDC->DrawState( pt, CSize(0,0), strTitle, DST_PREFIXTEXT|nStateFlag, TRUE, 0, (CBrush*)NULL );
			}
			else
			{
				char sbuf[100];
				char str[4][50];
				int len[4],i;
				char* p;
				char* ptr = NULL;
				int cnt=0;

				for(i=0;i<4;i++) len[i]=0;

				//strcpy_s(sbuf, strTitle);		// JBY
				strcpy(sbuf, CT2A(strTitle));
				//_tcscpy( sbuf , strTitle );

				p = strtok_s(sbuf, "\n", &ptr);
				while( p != NULL )
				{
					strcpy_s(str[cnt],p);
					len[cnt] = strlen(str[cnt]);
					p = strtok_s(NULL, "\n", &ptr);
					cnt++;
					if(cnt>3) break;
				}

				CString MyStr;
				CSize MyStrSize;
						
				MyStr = str[0];
				MyStrSize = pDC->GetTextExtent(MyStr);

				if(m_bUseOffset)
					pt.y = ((rectControl.bottom - rectControl.top) - (MyStrSize.cy * cnt) ) / 2;
				else
					pt.y = m_pointText.y;

				for(i=0;i<4;i++)
				{
					if(len[i])
					{
						MyStr = str[i];
						MyStrSize = pDC->GetTextExtent(MyStr);
						
						pt.x = ((rectControl.right - rectControl.left) / 2) - (MyStrSize.cx / 2);

						pDC->SetTextColor(COLORREF(m_textColor));
						pDC->SetBkMode (TRANSPARENT);
						
						pDC->DrawState( pt, CSize(0,0), MyStr, DST_PREFIXTEXT|nStateFlag, TRUE, 0, (CBrush*)NULL );
					}
					pt.y += MyStrSize.cy;
				}
			}

			if( m_nTextHeight )
			{
				if(pOldFont) pDC->SelectObject(pOldFont);
				m_newFont.DeleteObject();
			}
		}
	} // End !focus only

	//	Draw focus rectange
/*	if( !( nNewState & ODS_DISABLED ) )	// Make sure it's not disabled
	{
		// Redraw the focus if:
		//		1. There is a change in focus state 
		// OR	2. The entire control was just redrawn and Focus is set
		if( ( nNewState && ODS_FOCUS ) ^ ( m_nOldState && ODS_FOCUS ) ||
			( !bDRAWFOCUSONLY && ( nNewState & ODS_FOCUS ) ) )
		{
			#define FOCUSOFFSET 3
			CRect rect( rectControl );

			// As control gets smaller, decrease focus size
			int nDeflate = min( FOCUSOFFSET,
								min( rect.Width(), rect.Height() ) >> 2 );
			rect.DeflateRect( nDeflate, nDeflate);
			pDC->DrawFocusRect(&rect);
		}
	}*/

	m_nOldAction = nNewAction;
	m_nOldState = nNewState;

	if( pDC ) ReleaseDC(pDC);
}

CMinButton& CMinButton::SetBkColor(COLORREF crBkgnd, COLORREF crBkgndHigh , UINT mode, int tpercent)
{

	m_GradationRect.SetBkColor( crBkgnd, crBkgndHigh, mode, tpercent);
	
	Redraw();

	return *this;
}

CMinButton& CMinButton::SetFontName(const CString& strFont, BYTE byCharSet /* Default = ANSI_CHARSET */)
{	
	m_logFont.lfCharSet = byCharSet;
	//strcpy_s(m_logFont.lfFaceName, strFont);		// JBY
	strcpy(CT2A(m_logFont.lfFaceName), CT2A(strFont));
	
//	ReconstructFont();
	m_newFont.DeleteObject();
	BOOL bCreated = m_newFont.CreateFontIndirect(&m_logFont);

//	UpdateSurface();
	Redraw();

	return *this;
}

void CMinButton::SetTextSize(int textsize, BOOL bold)
{
	const int POINTS_PER_INCH = 72;
	int nDot;
	CClientDC dc(this);

	nDot = (textsize * dc.GetDeviceCaps(LOGPIXELSY)) / POINTS_PER_INCH;

	m_nTextHeight = nDot;
	m_bTextBold = bold;

	Redraw();
}
