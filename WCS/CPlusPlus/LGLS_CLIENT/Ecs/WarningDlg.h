// WarningDlg.h : header file
//
// [LGLS 2026-08-22] 작업 체류(설비 무응답) 경고창.
//   TASK 는 서버에서 돌아 사람이 보지 못하므로, 운전자가 보는 Client 에 띄운다.
//   원본 CWarningDlg 는 CLog / CDialogResize / CStartupTip / CFooButton 에 의존했는데
//   그 네 클래스의 구현이 프로젝트에 없어(헤더만 존재) 빌드 대상에서도 빠져 있었다.
//   → MFC 표준(CDialog / CListCtrl / CStatic)만 쓰도록 다시 쓰고, 경고 원천은
//     JOB_MST 를 직접 조회하는 방식으로 바꾼다.

#pragma once

#include "Resource.h"

class CEcsDoc;

class CWarningDlg : public CDialog
{
public:
	CWarningDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);
	virtual ~CWarningDlg();

public:
	CEcsDoc* m_pDoc;

	enum { IDD = IDD_WARNING_DLG };
	enum { TIMER_SCAN = 7301 };

	// 체류 판정 기준(초). Ecs.ini [USER] JOB_STALL_WARN_SEC, 기본 300
	int  m_nStallSec;
	// 자동 표시 억제(STOP 버튼)
	BOOL m_bMute;
	// 이미 알린 작업 : "작업번호|상태" 목록 (같은 상태로 머무는 동안 1회만 알림)
	CStringArray m_arrNotified;
	// 목록 순회 위치 (|<< << >> >>| 버튼). -1 = 선택 없음
	int m_nCursor;

public:
	void ScanStalledJobs();
	void ScanAlarmLogs();          // WCS_CLIENT_LOG 의 미확인 알람(ALARM_YN='Y')을 띄운다
	void PumpupAlarm(LPCTSTR lpszWinId, LPCTSTR lpszLugg, LPCTSTR lpszMessage);  // 즉시 표시(구 CLog::PumpupAlarm 대응)
	void ShowRow(int nIndex);      // 지정 행을 상단 팁에 표시하고 선택
	void AddRow(LPCTSTR lpszTime, LPCTSTR lpszLugg, LPCTSTR lpszStatus,
	            LPCTSTR lpszIdle, LPCTSTR lpszRoute);

protected:
	CListCtrl m_ctlList;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonPrev();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonLast();
	afx_msg void OnButtonShow();
	DECLARE_MESSAGE_MAP()
};
