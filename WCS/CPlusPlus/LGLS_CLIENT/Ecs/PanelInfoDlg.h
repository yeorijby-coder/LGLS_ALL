// PanelInfoDlg.h : [LGLS 2026-09-01] 상세정보 도킹 판넬 (CV/SC/RTV/작업 탭)
//   항목/값/설정/확인/구ECS주소/실제주소/기록시명칭 7열. 탭 선택에 따라 판넬 캡션이 바뀐다.
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
	CComboBox m_cmbUnit;      // 설비 탭의 호기(트랙) 선택
	CListCtrl m_list;
	CString   m_strJobNo;     // 작업 탭에 표시할 작업번호

	void Refresh();
	void SetJob(CString strLuggNo);

protected:
	// 행 위 오버레이 컨트롤 (설정/확인 열)
	CComboBox m_cmbStatus;    // 작업 탭 : 작업상태
	CButton   m_btnStatus;
	CComboBox m_cmbPri;       // 작업 탭 : 우선순위
	CButton   m_btnPri;
	CEdit     m_edtCvJob;     // CV 탭 : 지시 작업번호
	CButton   m_btnCvWrite;
	CButton   m_btnForce;     // SC/RTV 탭 : 강제완료
	CButton   m_btnCvDelete;  // CV 탭 하단 : 지시 삭제

	CStringArray m_arStatusCd;   // 작업상태 콤보의 코드값

	void UpdateTitle();
	void FillUnits();
	void HideOverlays();
	void PlaceOverCell(CWnd* pCtrl, int nRow, int nCol, BOOL bShow);
	BOOL ExecUpdate(CString strSql, CString strLogMsg, CString strLuggNo, CString strWidId);

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTabChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUnitChanged();
	afx_msg void OnBtnStatus();
	afx_msg void OnBtnPri();
	afx_msg void OnBtnCvWrite();
	afx_msg void OnBtnCvDelete();
	afx_msg void OnBtnForce();
	DECLARE_MESSAGE_MAP()
};
