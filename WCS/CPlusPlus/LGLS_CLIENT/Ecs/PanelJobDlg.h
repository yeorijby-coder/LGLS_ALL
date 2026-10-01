// PanelJobDlg.h : [LGLS 2026-09-01] 전체 작업(JOB_MST) 도킹 판넬 (작업구분 탭 필터)
//   [LGLS 2026-10-01] 구 ECS 하단 반송 판넬과 같게 셋으로 나눈다 (사용자 지시)
//     왼쪽  : 우선순위 (값, ▲ ▼, 반송조정)
//     가운데: 작업 목록 (탭 필터)
//     오른쪽: 선택 작업의 상세 - ECS번호/작업번호 + 단계(SEQ/디바이스/시작/도착/상태) + [완료처리]
#pragma once
#include "resource.h"
#include <afxtempl.h>
class CEcsDoc;

class CPanelJobDlg : public CDialog
{
public:
	CPanelJobDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_PANEL_JOB };
	CEcsDoc*  m_pDoc;
	CTabCtrl  m_tabTyp;
	CListCtrl m_list;
	CButton   m_chkAuto;	// [LGLS 2026-09-10] 자동 갱신 (오른쪽 위)

	// [LGLS 2026-10-01] 구 ECS 판넬의 왼쪽/오른쪽 칸 (런타임 생성)
	CStatic   m_lblPriTitle;
	CStatic   m_lblPriVal;
	CButton   m_btnPriUp;
	CButton   m_btnPriDn;
	CButton   m_btnTransfer;
	CStatic   m_lblEcs;
	CStatic   m_lblEcsVal;
	CStatic   m_lblJob;
	CStatic   m_lblJobVal;
	CListCtrl m_listSeq;
	CButton   m_btnComplete;

	// 목록 한 줄의 원시값 (화면 글자는 이름표라 코드가 따로 필요하다)
	struct ROW {
		CString lugg, typCd, staCd, startPos, startLoc, destPos, destLoc, hs, sc, pri, lot;
	};
	CArray<ROW, ROW&> m_arRow;
	CString m_strSelLugg;

	void Refresh();
protected:
	CString TypFilter();    // 현재 탭의 JOB_TYP IN (...) 조건
	void BuildOldEcsControls();
	void FillDetail(int nRow);
	void BuildSeqRows(const ROW& r);
	BOOL ExecUpdate(CString strSql, CString strLogMsg, CString strLuggNo);
	int  FindRow(const CString& strLugg);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTabChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPriUp();
	afx_msg void OnPriDown();
	afx_msg void OnTransferCtl();
	afx_msg void OnComplete();
	DECLARE_MESSAGE_MAP()
};
