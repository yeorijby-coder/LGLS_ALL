#include "StdAfx.h"
#include "ManyLang.h"


CManyLang::CManyLang(CWnd* pWnd, int pnLang)
{
	m_pWnd = pWnd;
	m_nLang = pnLang;
	m_pFont = new CFont();
}


CManyLang::~CManyLang(void)
{
	delete m_pFont;
}

void CManyLang::SetFont(CWnd* pWnd, int pnLang)
{
	if(pWnd == NULL){return;}
	CFont* pFont = new CFont();
	pFont->CreateFont(
		10, // nHeight 
		8, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		0, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_DONTCARE, // nPitchAndFamily 
		_T("SimSun") // lpszFacename 
		); 
	//SetFont(pFont, TRUE);
	//SendMessageToDescendants(WM_SETFONT, (WPARAM)pFont->GetSafeHandle(), 1, TRUE, FALSE);
}


