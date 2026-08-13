#include "StdAfx.h"
#include "FontManagerDialog.h"
#include "Ecs.h"



CFontManagerDialog::CFontManagerDialog(void)
{
	m_pFont = NULL;
	m_pParentDlgWnd = NULL;
}

CFontManagerDialog::CFontManagerDialog(CWnd* pWnd)
{
	m_pFont = NULL;
	m_pParentDlgWnd = pWnd;
}


CFontManagerDialog::~CFontManagerDialog(void)
{
	//if(m_pFont != NULL)
	//{
	//	delete m_pFont;
	//}
}

BOOL CFontManagerDialog::InitializeFontManager(CWnd* pWnd)
{
	m_pFont = NULL;
	m_pParentDlgWnd = pWnd;
	if(m_pParentDlgWnd == NULL){ return FALSE; }
	return TRUE;
}

BOOL CFontManagerDialog::InitializeFontManager()
{
	m_pFont = NULL;
	if(m_pParentDlgWnd == NULL){ return FALSE; }
	return TRUE;
}

int CFontManagerDialog::GetLangDefaultWidth(int pnLang)
{
	//int n = nFontSizeDefaultWidth_KOR;
	if(pnLang == 0){ return nFontSizeDefaultWidth_KOR;};
	if(pnLang == 1){ return nFontSizeDefaultWidth_ENG;};
	if(pnLang == 2){ return nFontSizeDefaultWidth_HUN;};
	if(pnLang == 3){ return nFontSizeDefaultWidth_CHIN;};
	return 8;
}

int CFontManagerDialog::GetLangDefaultHeight(int pnLang)
{
	if(pnLang == 0){ return nFontSizeDefaultHeight_KOR;};
	if(pnLang == 1){ return nFontSizeDefaultHeight_ENG;};
	if(pnLang == 2){ return nFontSizeDefaultHeight_HUN;};
	if(pnLang == 3){ return nFontSizeDefaultHeight_CHIN;};
	return 11;
}

BOOL CFontManagerDialog::SetFontNation(int pnLang, int pnWidth, int pnHeight, BOOL pbRedraw, BOOL pbMessage,  CWnd* pWnd)
{													//= true		//= true		// = 8			// = 13		// = NULL
	CWnd* wndTemp = (pWnd == NULL) ? m_pParentDlgWnd : pWnd;
	if(m_pFont != NULL){ delete m_pFont; };
	if(wndTemp == NULL){ return FALSE; };
	m_pFont = new CFont();
	m_pFont->CreateFont( 
		(pnWidth == 0) ? GetLangDefaultHeight(pnLang) : pnWidth, // nHeight 
		(pnWidth == 0) ?  GetLangDefaultWidth(pnLang) : pnWidth, // nwidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_DONTCARE, // nPitchAndFamily 
		//strFontPath
		Global.GetFontName(pnLang)
		); 
	
	wndTemp->SetFont(m_pFont, TRUE);
	if(pbMessage == TRUE)
	{
		wndTemp->SendMessageToDescendants(WM_SETFONT, (WPARAM)m_pFont->GetSafeHandle(), 1, TRUE, FALSE);
	}

	//@@@@
	//delete m_pFont;
	return TRUE;
}