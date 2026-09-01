// PanelVehDlg.h : [LGLS 2026-09-01] Crane & Vehicle 반송 현황 판넬 (구 ECS 메인화면 표 대응)
#pragma once
#include "resource.h"

class CEcsDoc;

class CPanelVehDlg : public CDialog
{
public:
	CPanelVehDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_PANEL_VEH };

	CEcsDoc*  m_pDoc;
	CListCtrl m_list;

	void Refresh();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};
