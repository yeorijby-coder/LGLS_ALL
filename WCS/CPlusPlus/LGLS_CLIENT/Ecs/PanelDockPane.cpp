// PanelDockPane.cpp : [LGLS 2026-09-01] 다이얼로그를 담는 도킹 판넬
#include "stdafx.h"
#include "PanelDockPane.h"

CPanelDockPane::CPanelDockPane()
{
	m_pDlg = NULL;
	m_nIDD = 0;
}

BEGIN_MESSAGE_MAP(CPanelDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CPanelDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pDlg != NULL && m_nIDD != 0)
	{
		if (!m_pDlg->Create(m_nIDD, this))
			return -1;
		m_pDlg->ShowWindow(SW_SHOW);
	}
	return 0;
}

void CPanelDockPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_pDlg != NULL && ::IsWindow(m_pDlg->m_hWnd))
		m_pDlg->MoveWindow(0, 0, cx, cy);
}
