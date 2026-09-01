// PanelJobDlg.h : [LGLS 2026-09-01] 전체 작업(JOB_MST) 도킹 판넬
//   구 SPL EcsSv CPanelJobDlg 를 LGLS 인프라(CListCtrl + DB 직조회)로 재구현.
#pragma once
#include "resource.h"

class CEcsDoc;

class CPanelJobDlg : public CDialog
{
public:
	CPanelJobDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_PANEL_JOB };

	CEcsDoc*  m_pDoc;
	CListCtrl m_list;

	void Refresh();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}          // Enter/Esc 로 판넬이 닫히지 않게
	virtual void OnCancel() {}

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
