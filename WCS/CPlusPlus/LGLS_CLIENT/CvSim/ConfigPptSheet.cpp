// ConfigPptSheet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#include "EcsDoc.h"
#include "ConfigPptSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigPptSheet

IMPLEMENT_DYNAMIC(CConfigPptSheet, CPropertySheet)

CConfigPptSheet::CConfigPptSheet(CEcsDoc *pDoc, CConfig *pConfig, LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage), m_Page1(pDoc, pConfig), m_Page2(pDoc, pConfig), m_Page4(pDoc, pConfig)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	AddPage(&m_Page1);
	AddPage(&m_Page2);
//	AddPage(&m_Page4);

//	m_strLogoFile = pDoc->m_pConfig->GetSideLogoFile();
}

CConfigPptSheet::~CConfigPptSheet()
{
}

BEGIN_MESSAGE_MAP(CConfigPptSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CConfigPptSheet)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigPptSheet message handlers

BOOL CConfigPptSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	return bResult;
}

BOOL CConfigPptSheet::OnEraseBkgnd(CDC* pDC) 
{
	CPropertySheet::OnEraseBkgnd(pDC);
/*
	CRect rect;
	GetClientRect(rect);

	CBrush newBrush(RED);
	CBrush *pOldBrush = pDC->SelectObject(&newBrush);

	pDC->FillRect(&rect, &newBrush);

	if ( pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
*/
	return TRUE;
}