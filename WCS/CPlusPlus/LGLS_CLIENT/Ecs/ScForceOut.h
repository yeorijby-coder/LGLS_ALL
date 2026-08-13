#pragma once
#include "afxwin.h"
#include "JobItem.h"
#include "EcsDoc.h"

// CScForceOut 대화 상자입니다.
class CJobItem;
class CScPair;
class CScInfo;
class CScForceOut : public CDialogEx
{
	DECLARE_DYNAMIC(CScForceOut)

public:
	CScForceOut(CScPair* pSc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CScForceOut();

	CScPair * m_pSc;
	
	CScInfo * m_pInfo;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SC_PORCE_OUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
//	int CreateSemiMoveJob(CTrackHS* pScStoHS, int nArriveTrack, int nScRetHsPos);
	void WriteTrack(int nTrack, int nLuggNum);

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	BUTTON_TYPE m_btnOK;
	BUTTON_TYPE m_btnCancel;
	BOOL m_bPair;
	BOOL m_b2Fork;
	CComboBoxWrapper m_cbxArriveHS;
	afx_msg void OnBnClickedCheckPair();
	afx_msg void OnBnClickedCheck2fork();
	CComboBoxWrapper m_cbxDepartHS;

};
