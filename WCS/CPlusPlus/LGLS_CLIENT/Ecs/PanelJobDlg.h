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
	CButton   m_btnSplit;		// [LGLS 2026-10-01] 가로보기/세로보기 전환
	int       m_nSplitMode;		// 1 가로 / 2 세로 (ini JOB_PANEL_SPLIT 에서 읽고, 단추로 바꾸면 ini 에도 쓴다)
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
	// [LGLS 2026-10-01] 목록|상세 사이 분할선 (끌어서 상세 칸 폭 조절)
	int   m_nSplitR;
	int   m_nSplitB;			// 세로 쌓기일 때 아래 칸 높이(끌어서 정한 값)
	BOOL  m_bSplitVert;		// 지금 분할선이 가로줄(세로 쌓기)인가
	int   m_nRcLeftW;			// 리소스의 왼쪽 칸 폭
	CRect m_rcRcRight[6];		// 리소스의 오른쪽 머리줄 상대 위치 (ECS라벨/값/작업라벨/값/완료처리/SEQ표)
	BOOL  m_bDragSplit;
	CRect m_rcSplit;

	void Refresh();
protected:
	CString TypFilter();    // 현재 탭의 JOB_TYP IN (...) 조건
	void BuildOldEcsControls();
	void FillDetail(int nRow);
	void BuildSeqRows(const ROW& r);
	CMap<int, int, int, int> m_mapTrackCv;	// 트랙(3자리) → C/V 번호
	void LoadTrackCvMap();
	int  CvOfTrack(int nTrk);
	int  NeighborTrack(int nTrk);
	CString PortText(int nTrk);
	CString CvDevText(int nTrk);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
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
	afx_msg void OnSplitToggle();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};
