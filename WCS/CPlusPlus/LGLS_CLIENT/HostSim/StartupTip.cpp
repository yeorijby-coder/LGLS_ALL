// StartupTip.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "StartupTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CStartupTip

CStartupTip::CStartupTip()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_colorBack = RGB(255,255,255);

	m_nLeftPane = 50;
	m_colorLeftPane = RGB(132,130,132);
	m_nVspace = 30;
	m_nVsize = 2;

	m_nTipLen = 80;

	m_nLines = 0;
	for(int i=0; i<10; i++)
		memset(&Tips[i].Text[0], 0, 100);

	m_pfontOld = NULL;


	m_strTitle = _T("Alarm...");
	m_strHeader = _T("");
	m_strContent = _T("");
	m_listHead.RemoveAll();

	m_fontTitle.CreateFont(22, 0,0,0, FW_BOLD, 0,0,0, DEFAULT_CHARSET, 0,0, DEFAULT_QUALITY, FF_ROMAN, _T("Times New Roman"));
	m_fontHead.CreateFont(18, 0,0,0, FW_BOLD, 0,0,0, DEFAULT_CHARSET, 0,0, DEFAULT_QUALITY, FF_ROMAN, _T("Arial"));
//	m_fontContent.CreateFont(8, 0,0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET, 0,0, DEFAULT_QUALITY, FF_ROMAN, "System");
	m_fontContent.CreateFont(16,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,0,0,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS, _T("Arial"));


	m_colorTitle = BLACK;
	m_colorHead = SITE;

	m_nOffY = 0;
}

CStartupTip::~CStartupTip()
{
}


BEGIN_MESSAGE_MAP(CStartupTip, CStatic)
	//{{AFX_MSG_MAP(CStartupTip)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartupTip message handlers

void CStartupTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	dc.SetBkMode(TRANSPARENT);

	DrawLeftPane(&dc);
	DrawTopTitle(&dc);
	DrawSubTitle(&dc);
	//DrawContent(&dc);
	DrawMultiLine(&dc);

	if ( m_pfontOld != NULL )
		dc.SelectObject(m_pfontOld);

	m_pfontOld = NULL;

	// Do not call CStatic::OnPaint() for painting messages
}

void CStartupTip::DrawLeftPane(CPaintDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
	rect.right = m_nLeftPane;
	pDC->FillSolidRect(rect, m_colorLeftPane);

	// Draw icon in LeftPane
	if (m_hIcon != NULL)
		DrawIcon(pDC->m_hDC, (m_nLeftPane/2)-16, 6, m_hIcon);
}

void CStartupTip::DrawTopTitle(CPaintDC *pDC)
{
	CRect rect;

	GetClientRect(rect);
	rect.left = m_nLeftPane;
	rect.top = m_nVspace;
	rect.bottom = m_nVsize + m_nVspace;
	pDC->FillSolidRect(rect, m_colorLeftPane);

	pDC->SetTextColor(m_colorTitle);
	if ( m_pfontOld == NULL )
		m_pfontOld = pDC->SelectObject(&m_fontTitle);
	else
		pDC->SelectObject(&m_fontTitle);

	pDC->TextOut(m_nLeftPane+5, 5, m_strTitle);
}

void CStartupTip::DrawSubTitle(CPaintDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
	
	int nOffX = m_nLeftPane+10;
	m_nOffY = m_nVsize + m_nVspace + 5;

	pDC->SetTextColor(m_colorHead);
	if ( m_pfontOld == NULL )
		m_pfontOld = pDC->SelectObject(&m_fontHead);
	else
		pDC->SelectObject(&m_fontHead);

	for (POSITION pos = m_listHead.GetHeadPosition(); pos != NULL; )
	{
		CString strHead = m_listHead.GetNext(pos);
		pDC->TextOut(nOffX, m_nOffY, strHead);
		CSize size = pDC->GetTextExtent(strHead);
		m_nOffY += size.cy;
	}
}

void CStartupTip::DrawContent(CPaintDC *pDC)
{
	CRect rect;
	GetClientRect(rect);

	CSize size = pDC->GetTextExtent(m_strHeader);
	int nOffX = m_nLeftPane+10 + 10;
	int nOffY = m_nOffY + size.cy;

	rect.left = m_nLeftPane+10;
	rect.top = nOffY;
	rect.right = rect.right-5;


	pDC->SetTextColor(RGB(0, 0, 0));
	if ( m_pfontOld == NULL )
		m_pfontOld = pDC->SelectObject(&m_fontContent);
	else
		pDC->SelectObject(&m_fontContent);

/*
	for (POSITION pos = m_listContent.GetHeadPosition(); pos != NULL; )
	{
		CString strContent = m_listContent.GetNext(pos);
		pDC->ExtTextOut(nOffX, nOffY, ETO_CLIPPED, rect, strContent, strContent.GetLength(), NULL);
		CSize size = pDC->GetTextExtent(strContent);
		nOffY += size.cy;
	}
*/
}

void CStartupTip::DrawMultiLine(CPaintDC *pDC)
{

	CRect rect;
	GetClientRect(rect);

	CSize size = pDC->GetTextExtent(m_strHeader);
	int nOffX = m_nLeftPane+10 + 10;
	int nOffY = m_nOffY + size.cy;

	rect.left = m_nLeftPane+10;
	rect.top = nOffY;
	rect.right = rect.right-5;

	pDC->SetTextColor(RGB(0, 0, 0));
	if ( m_pfontOld == NULL )
		m_pfontOld = pDC->SelectObject(&m_fontContent);
	else
		pDC->SelectObject(&m_fontContent);

	for(int i=0; i<m_nLines; i++)
	{
		int nLen = strlen(&Tips[i].Text[0]);
		if ( nLen == 0)
			continue;
		pDC->ExtTextOut(nOffX, nOffY, ETO_CLIPPED, rect, (LPCTSTR)&Tips[i].Text[0], nLen, NULL);
		CSize size = pDC->GetTextExtent(Tips[i].Text);
		nOffY += size.cy;
	};
}

BOOL CStartupTip::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, m_colorBack);

	return 0;
	return CStatic::OnEraseBkgnd(pDC);
}

void CStartupTip::SetBgColor(COLORREF color)
{
	m_colorBack = color;
}

void CStartupTip::SetLeftPaneSize(int c)
{
	m_nLeftPane = c;
}

void CStartupTip::SetLeftPaneColor(COLORREF color)
{
	m_colorLeftPane = color;	
}

void CStartupTip::SetIcon(UINT nIconId)
{
	m_hIcon = AfxGetApp()->LoadIcon( nIconId );
	Invalidate(TRUE);
}

void CStartupTip::ResetTipText()
{
	m_listHead.RemoveAll();
	for(int i=0; i<10; i++)
		memset(&Tips[i].Text[0], 0, 100);

	Invalidate(true);
}

void CStartupTip::SetTipText(CString strHead, CString strContent)
{
	m_listHead.RemoveAll();

	if ( strHead.IsEmpty() || strContent.IsEmpty() )
		return;

	for ( ; ; )
	{
		strHead.TrimLeft('\\');
		if ( strHead.IsEmpty() )
			break;
		
		int i = strHead.Find('\\', 0);
		if ( i == -1 )
		{
			m_listHead.AddTail(strHead);
			break;
		}
		else
		{
			m_listHead.AddTail(strHead.Mid(0, i));
			strHead.Delete(0, i);
		}
	}

	SetTipText(strContent);
	Invalidate(true);
}

void CStartupTip::SetTipText(LPCTSTR lpText)
{
	CString str = lpText;

	char szWord[100];
	memset(szWord,0,100);

	m_nLines=0;
	int i = 0;
	for(i=0; i<10; i++)
		memset(&Tips[i].Text[0], 0, 100);

	i=GetWord((LPCTSTR)str, (LPTSTR)&szWord[0]);
	int u=0;  // letter counter in string
	int p=0;  // string counter
	while (i>0)
	{
		int k = strlen(&szWord[0]);
		u = u+k+1;
		if (u < m_nTipLen)
		{
			strcat(&Tips[p].Text[0], &szWord[0]);
			strcat(&Tips[p].Text[0], " ");
		}
		else
		{
			if (p<9)
			{
				p++;
				u=k+1;
				strcat(&Tips[p].Text[0], &szWord[0]);
				strcat(&Tips[p].Text[0], " ");
			};
		};

		int j = str.GetLength();
		str=str.Right(j-i);
		memset(szWord, 0, 100);
		i=GetWord((LPCTSTR)str, (LPTSTR)&szWord[0]);
	};

	if (u>0) m_nLines=p+1;

	Invalidate(TRUE);
}


int CStartupTip::GetWord(LPCTSTR in_str, LPTSTR lpWord)
{
	int i=0;
	int j=0;
	LPCTSTR lpStr;

	lpStr=in_str;

	// detect leading spaces
	while ((*lpStr==' '))
	{
		i++;
		lpStr++;
	};
	if (*lpStr=='\0') return 0;

	// copy symbols to word buffer until space or EOL
	while ((*lpStr!=' ') && (*lpStr!='\0'))
	{
		lpWord[j]=*lpStr;
		j++;
		i++;
		lpStr++;
	};

	return i;
}

void CStartupTip::SetMaxLen(int n)
{
	if (n<1) return;

	if (n>100)
		m_nTipLen = 100;
	else
		m_nTipLen = n;
}
