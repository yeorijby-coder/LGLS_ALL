// PanelJobDlg.h : [LGLS 2026-09-01] 전체 작업(JOB_MST) 도킹 판넬 (작업구분 탭 필터)
#pragma once
#include "resource.h"

class CEcsDoc;

class CPanelJobDlg : public CDialog
{
public:
	CPanelJobDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_PANEL_JOB };

	CEcsDoc*  m_pDoc;
	CTabCtrl  m_tabTyp;
	CListCtrl m_list;

	void Refresh();

protected:
	CString TypFilter();    // 현재 탭의 JOB_TYP IN (...) 조건

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnListClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTabChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
