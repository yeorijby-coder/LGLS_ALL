// PanelDockPane.h : [LGLS 2026-09-01] 다이얼로그를 담는 도킹 판넬(CDockablePane)
//   참고: 구 SPL EcsSv 의 CDockingBar(CSizingControlBar) 방식을 MFC Feature Pack 으로 재구현.
#pragma once

class CPanelDockPane : public CDockablePane
{
public:
	CPanelDockPane();

	CDialog* m_pDlg;      // 판넬 안에 채울 자식 다이얼로그 (Create 전에 지정)
	UINT     m_nIDD;      // 그 다이얼로그의 템플릿 ID

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};
