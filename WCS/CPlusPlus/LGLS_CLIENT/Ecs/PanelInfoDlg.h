// PanelInfoDlg.h : [LGLS 2026-09-01] 각종 정보(CV/SC/RTV/작업) 도킹 판넬
//   구 SPL EcsSv CPanelInfoDlg 를 LGLS 인프라(CTabCtrl + CListCtrl + DB 직조회)로 재구현.
#pragma once
#include "resource.h"

class CEcsDoc;

class CPanelInfoDlg : public CDialog
{
public:
	CPanelInfoDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_PANEL_INFO };

	CEcsDoc*  m_pDoc;
	CTabCtrl  m_tab;
	CListCtrl m_list;
	CString   m_strJobNo;     // 작업 탭에 표시할 작업번호(작업 판넬 선택 연동)

	void Refresh();
	void SetJob(CString strLuggNo);   // 작업 탭으로 전환 + 해당 작업 상세

protected:
	void RebuildColumns();

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTabChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
