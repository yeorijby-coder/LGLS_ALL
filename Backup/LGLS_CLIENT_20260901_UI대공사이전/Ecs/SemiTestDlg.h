#pragma once
#include "SkinDialog.h"
#include "SkinButton.h"
#include "EcsDoc.h"

// [LGLS 2026-08-13] 반자동 TEST 창 (HOST_SIM 역할 재현)
//   행마다: 출발지/도착지 + SC1~5 체크 → 체크된 크레인을 순환하며
//   입고(출발지→크레인의 다음 로케이션, 셀 순서로 +1) → 완료되면 그 로케이션에서
//   도착지로 출고 → 완료되면 다음 크레인으로... 무한 반복.
//   ★반자동 원칙: HOST 와 무관 - 완료 보고 없이 JOB_MST 에서 삭제만 한다.
//   ECS가 PLC에 적은 값(SC_DATA_LGLS *_OD)과 PLC가 준 값(*_RD) 변화를 로그로 표시.
class CSemiTestDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CSemiTestDlg)
public:
	CSemiTestDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);
	virtual ~CSemiTestDlg();
	enum { IDD = IDD_LGLS_SEMI_TEST };
	enum { ROWS = 5, CRANES = 5 };

	CEcsDoc* m_pDoc;
	BOOL m_bRun;

	struct SemiRow
	{
		int     nState;                 // 0=대기, 1=입고 진행, 2=출고 진행
		int     nCrane;                 // 마지막 사용 크레인(0~4)
		CString strLugg;                // 진행 중 작업번호
		CString strBank, strBay, strLev;// 진행 중 로케이션
		CString strScNo;                // 진행 중 SC ('901'~)
		CString strLastCell[CRANES];    // 크레인별 마지막 CELL_NO(+1 기준)
		int     nSeq;
	};
	SemiRow m_row[ROWS];
	CString m_strLastSta[ROWS];	// 상태전이 로그용 직전 JOB_STATUS
	CString m_strScSnapOd[CRANES];      // SC별 지시(OD) 스냅샷
	CString m_strScSnapRd[CRANES];      // SC별 보고(RD) 스냅샷
	CMapStringToString m_mapCvSnap;     // 작업대(MC_NO)별 RD 스냅샷

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);	// [LGLS 2026-08-13] 스킨 버튼 크기 고정
	DECLARE_MESSAGE_MAP()

	CSkinButton m_btnStart;
	CSkinButton m_btnStop;
	CSkinButton m_btnClose;
	CListCtrl   m_lstLog;		// [LGLS 2026-08-13] 컬럼형 로그

	// [LGLS 2026-08-13] 컬럼형 로그: 로직번호(1~5, 0=공통)/동작/작업번호/시스템/함수명/내용
	void AddLog(int nLogic, LPCTSTR szAct, LPCTSTR szLugg, LPCTSTR szSys, LPCTSTR szFunc, const CString& strMsg);
	void InitLogList();
	BOOL RowActive(int r);
	BOOL IsLocMode();               // [LGLS 2026-08-19] 로케이션 직접 입력 모드 여부(라디오)
	BOOL ResolveLocRow(int r);      // [LGLS 2026-08-19] 행의 로케이션 입력을 CELL_MST 로 확정
	BOOL PickNextCrane(int r, int& nCrane);
	BOOL NextCell(int r, int nCrane);
	BOOL InsertJob(int r, BOOL bSto);
	int  CheckJob(int r);               // 1=완료, 0=진행, -1=행 소멸
	void DeleteJob(int r);              // ★반자동 원칙: HOST 보고 없이 삭제
	void StepRow(int r);
	void TraceSc();
	void TraceCv();
	void SetRowStat(int r, LPCTSTR s);
	void RenameByIni();
};
