#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "ScManualRet.h"


// CScSkinDlg 대화 상자입니다.

class CScSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CScSkinDlg)

public:
	// [LGLS 2026-08-01] 설비 통신상태 패널(IDC_SCV_*)을 런타임 생성 - rc 에 컨트롤이 없어 값이 안 보이던 문제
	void BuildVehStatusPanel();
	void RebuildVehStatusPanel();	// [LGLS 2026-09-01] 호기 전환 시 주소 라벨 재생성
	void CompactForkStatusRow();		// [LGLS] 포크 상태 행 제거 + 아래 행 끌어올림
	CPtrArray m_arVehCtrl;
	CPtrArray m_arLglsCtrl;		// [LGLS 2026-08-01] 적재 용기/제품 정보 행(확대·축소와 무관하게 항상 표시)
	CSkinButton m_btnVehZoom;	// [LGLS 2026-08-01] [확대]/[축소] 토글 버튼
	CSkinButton m_btnScResend;	// [LGLS 2026-08-12] 지시 재전송(원 대화상자)
	BOOL    m_bVehExpanded;	// TRUE=확대(상태 패널 표시)
	BOOL    m_bForkRowCompacted;   // [LGLS 2026-09-02] 포크 행 압축 1회 가드
	int     m_nVehBaseH;	// 축소 상태 창 높이
	int     m_nVehPanelH;	// 상태 패널 높이
	void SetVehPanelExpanded(BOOL bExpand);
	afx_msg void OnBnClickedVehZoom();
	CScSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CScSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CScSkinDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKIN_SC_CTRL };

	enum { 
		EN_BtnScConfirm = 1,
		EN_BtnScEmergency = 2,
		EN_BtnScActive = 3,
		EN_BtnScStop = 4,
		EN_BtnScErrReset = 5,
		EN_BtnScFk1Delete = 6,
		EN_BtnScFk2Delete = 7,
		EN_BtnScFk1Fk2Delete = 8,
		EN_BtnScCanel = 9,
		EN_BtnScCallToHome = 10,
		EN_BtnScSave = 11
	};


public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	CSc* m_Sc;
	CSC_DATA * m_pSC_DATA;
	CScManualRet* m_pScManualRet;

	CString m_strScFork;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);   // [LGLS 2026-07-22] 실시간 갱신
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void MakeGroupBoxesTransparent();	// [LGLS 2026-08-05] 그룹박스 캡션 배경 제거
	void RenameRuntimeLabels();	// [LGLS 2026-08-05] 런타임 생성 라벨 다국어
	void RedrawImage();
	//void InitializeResource(int pnTabIndex);
	void InitializeResource(EN_LANG nEN_LANG);
	void GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode);

public:
	CSkinButton m_btnScOk;
	CSkinButton m_btnScCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	EN_LANG m_nLang;

public:	
	LRESULT OnMessagSwitch(WPARAM wParam, LPARAM lParam);
	//CSTATIC
	CStaticTransparent m_lblScJobNo;
	CStaticTransparent m_lblScJobTyp;
	CStaticTransparent m_lblScStartPos;
	CStaticTransparent m_lblScDestPos;

	CStaticTransparent m_lblScRcMode;
	CStaticTransparent m_lblScScMode;
	CStaticTransparent m_lblScForkPos;
	CStaticTransparent m_lblScForkPos2;
	CStaticTransparent m_lblScJobStatus;
	CStaticTransparent m_lblScRcStatus;
	CStaticTransparent m_lblScScStatus;
	CStaticTransparent m_lblScHorizontalPos;
	CStaticTransparent m_lblScVerticalPos;
	CStaticTransparent m_lblScProdLoad;

	CStaticTransparent m_lblScJobJobNo;
	CStaticTransparent m_lblScJobJobTyp;
	CStaticTransparent m_lblScJobStartPos;
	CStaticTransparent m_lblScJobDestPos;
	CStaticTransparent m_lbScJobJobStatus;
	CStaticTransparent m_lblScJobBcrTop;
	CStaticTransparent m_lblScJobBcrBottom;

	CStaticTransparent m_lblScSuspend;

	//GRP
	//SCSTATUS
	CTGroupBox m_grpScScStauts;
	CTGroupBox m_grpScScStautsItem1;
	CTGroupBox m_grpScScStatusValue1;
	CTGroupBox m_grpScScStautsItem2;
	CTGroupBox m_grpScScStatusValue2;
	CTGroupBox m_grpScScStautsItem3;
	CTGroupBox m_grpScScStatusValue3;
	CTGroupBox m_grpScScStautsItem4;
	CTGroupBox m_grpScScStautsItem5;
	CTGroupBox m_grpScScStatusValue4;
	CTGroupBox m_grpScScStatusValue5;

	CTGroupBox m_grpScScStautsCommand;
	//--SCSTATUS

	//JOBSTATUS
	CTGroupBox m_grpScJobStatus;
	CTGroupBox m_grpScJobStatusItem1;
	CTGroupBox m_grpScJobStatusValue1;
	CTGroupBox m_grpScJobStatusItem2;
	CTGroupBox m_grpScJobStatusValue2;
	CTGroupBox m_grpScJobStatusCommand;
	CTGroupBox m_grpBottom;
	

	CTGroupBox m_grpScJobStatus2;
	CTGroupBox m_grpScSuspend;
	//--JOBSTATUS
	//--GRP

	//--CSTATIC


	//SC_NM
	CEdit m_edtScNo;
	//--SC_NM

	//SC_NO
	CEdit m_edtScNo2;
	//--SC_NO



	//JOBSTATUS
	CEdit m_edtScJobNo;

	CComboBoxWrapper m_cbxScJobTyp;
	CComboBoxWrapper m_cbxScStartPos;
	CComboBoxWrapper m_cbxScDestPos;
	//--JOBSTATUS

	//SCSTATUS
	CEdit m_edtScRcMode;
	CEdit m_edtScScMode;
	CEdit m_edtScForkPos;
	CEdit m_edtScJobStatus;
	CEdit m_edtScRcStatus;
	CEdit m_edtScScStatus;
	CEdit m_edtScHorizontalPos;
	CEdit m_edtScVerticalPos;
	CEdit m_edtScProdLoad;
	//--SCSTATUS

	//JOBSTATUS
	CEdit m_edtScJobJobNo;
	CEdit m_edtScJobBcrTop;
	CEdit m_edtScJobBcrBottom;
	CEdit m_edtScSuspend;
	CEdit m_edtScErrCode;

	CComboBoxWrapper m_cbxScJobJobTyp;
	CComboBoxWrapper m_cbxScJobStartPos;
	CComboBoxWrapper m_cbxScJobDestPos;
	CComboBoxWrapper m_cbxScJobJobStatus;

	
	CEdit m_edtScJobDestLoc;
	CEdit m_edtScJobStartLoc;
	CStaticTransparent m_lblScJobDestLoc;
	CStaticTransparent m_lblScJobStartLoc;

	//--JOBSTATUS

	//COMMAND
	CSkinButton m_btnScConfirm;
	CSkinButton m_btnScEmergency;
	CSkinButton m_btnScActive;
	CSkinButton m_btnScStop;
	CSkinButton m_btnScErrorReset;
	CSkinButton m_btnScDelete;
	CSkinButton m_btnScCallToHome;
	CSkinButton m_btnScManual;
	CSkinButton m_btnScStoSuspend;
	CSkinButton m_btnScRetSuspend;
	CSkinButton m_btnScAllSuspend;
	CSkinButton m_btnDuplSto;

	CButton m_chkScFork1;
	CButton m_chkScFork2;
	CButton m_chkScFork1Fork2;

	BOOL    m_ChkSuspend;
	CSkinButton m_btnScManualRet;
	CEdit m_edtScItnLuggNo;
	CStaticTransparent m_lblItnLuggNo;
	//--COMMAND

public:
	CString m_strScSuspend;

public:
	void SetButtonScImage( CButton& pButton, CString pFieldName, int pValue );

	void InvalidateScData(EN_LANG pLang);
	void InvalidateLangControl( EN_LANG enLangTemp );
	void UpdateScData(int pBntJob);
	void GetCommonCode(CString pWH_TYP, CString pSC_NO);
	CString GetSelectQry(CString pWH_TYP, CString pSC_NO);
	CString GetUpdateQry(CString pWH_TYP, CString pPLC_NO, CString pSC_NO, CString pSuspned);


	afx_msg void OnBnClickedBtnScConfirm();
	afx_msg void OnBnClickedBtnScEmergency();
	afx_msg void OnBnClickedBtnScActive();
	afx_msg void OnBnClickedBtnScStop();
	afx_msg void OnBnClickedBtnScErrorReset();
	afx_msg void OnBnClickedBtnScCallToHome();
	afx_msg void OnBnClickedBtnScManual();

	afx_msg void OnBnClickedBtnScOk();
	afx_msg void OnBnClickedBtnScCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnScDelte();
	afx_msg void OnBnClickedBtnCvStoSuspend();
	afx_msg void OnBnClickedBtnScRetSuspend();
	afx_msg void OnBnClickedBtnScAllSuspend();
	afx_msg void OnBnClickedChkScFork1();
	afx_msg void OnBnClickedChkScFork2();
	afx_msg void OnBnClickedScRtvFork12();
	afx_msg void OnBnClickedScDuplicationSto();
	CString GetQrySelectJOB_MST_FK1( CSC_DATA * pSC_DATA );
	CString GetQrySelectJOB_MST_FK2( CSC_DATA * pSC_DATA );
	CString GetQrySelectSC_STATUS_CCD( CSC_DATA* pSC_DATA );
	afx_msg void OnBnClickedBtnScManualRet();

	BOOL RetHsDataSelect(CString& strHS_MC_NO, CString& strSensor0DataRd);
	BOOL JobMstInsert(CString pHsMcNo, CString& pLuggNo, CString& strProductSize);
	BOOL ScDataDelete();
	BOOL JobMstDelete(CString pLuggNo, CString& strProductSize);
	BOOL CvDataUpdate(CString pMcNo, CString pLuggNo, CString pProductSize);
	void SetScStatus(CString pCMD, CString pSC_SUSPEND);
	void SelCommonCode(CString pCCD_CD, CString pCMD, CString& pCCD_NM_KOR);
	afx_msg void OnBnClickedScvResend();
	afx_msg void OnBnClickedScvOk();
	void SetLed(int nId, CString strVal);
	HBRUSH m_brLedOn;
	HBRUSH m_brLedOff;
	HBRUSH m_brStatus;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
