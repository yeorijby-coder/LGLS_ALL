#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"

// CRtvSkinDlg 대화 상자입니다.

class CRtvSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CRtvSkinDlg)

public:
	// [LGLS 2026-08-01] 설비 통신상태 패널(IDC_RTVV_*)을 런타임 생성 - rc 에 컨트롤이 없어 값이 안 보이던 문제
	void BuildVehStatusPanel();
	CPtrArray m_arVehCtrl;
	CPtrArray m_arLglsCtrl;		// [LGLS 2026-08-01] 적재 용기/제품 정보 행(확대·축소와 무관하게 항상 표시)
	CSkinButton m_btnVehZoom;	// [LGLS 2026-08-01] [확대]/[축소] 토글 버튼
	CSkinButton m_btnRtvResend;	// [LGLS 2026-08-12] 지시 재전송(원 대화상자)
	BOOL    m_bVehExpanded;	// TRUE=확대(상태 패널 표시)
	int     m_nVehBaseH;	// 축소 상태 창 높이
	int     m_nVehPanelH;	// 상태 패널 높이
	void CompactJobStatusArea();	// [LGLS 2026-08-01] 빈 공간 제거(사용자 지시 사진)
	void SetVehPanelExpanded(BOOL bExpand);
	afx_msg void OnBnClickedVehZoom();
	afx_msg void OnBnClickedBtnRtvManual();   // [LGLS 2026-09-03] 수동지시
	CRtvSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CRtvSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRtvSkinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKIN_RTV_CTRL };

	enum { 
		EN_BtnRtvConfirm = 1,
		EN_BtnRtvEmergency = 2,
		EN_BtnRtvActive = 3,
		EN_BtnRtvStop = 4,
		EN_BtnRtvErrReset = 5,
		EN_BtnRtvFk1Delete = 6,
		EN_BtnRtvFk2Delete = 7,
		EN_BtnRtvFk1Fk2Delete = 8,
		EN_BtnRtvCanel = 9,
		EN_BtnRtvCallToHome = 10,
		EN_BtnRtvSave = 11
	};

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;
	
	EN_LANG m_nLang;

public:
	CRtv* m_Rtv;
	CRTV_DATA* m_pRTV_DATA;


public:
	void GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void MakeGroupBoxesTransparent();	// [LGLS 2026-08-05] 그룹박스 캡션 배경 제거
	void RenameRuntimeLabels();	// [LGLS 2026-08-05] 런타임 생성 라벨 다국어
	void RedrawImage();
	//void InitializeResource(int pnTabIndex);
	void InitializeResource(EN_LANG pnLANG);


	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);   // [LGLS 2026-07-22] 실시간 갱신
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	CButton m_chkRtvFork1;
	CButton m_chkRtvFork2;
	CButton m_chkRtvFork1Fork2;

	CString m_strRtvFork;

public:	
	LRESULT OnMessagSwitch(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	//Label
	CStaticTransparent m_lblLuggNo;
	CStaticTransparent m_lblStartPos;
	CStaticTransparent m_lblDestPos;
	CStaticTransparent m_lblJobTyp;
	CStaticTransparent m_lblRtvMode;
	CStaticTransparent m_lblComplete;
	CStaticTransparent m_lblRtvProdLoad;
	CStaticTransparent m_lblRtvStatus;
	CStaticTransparent m_lblActive;
	CStaticTransparent m_lblHorizontalPos;
	CStaticTransparent m_lblJisangMode;
	CStaticTransparent m_lblJobLuggNo;
	CStaticTransparent m_lblJobStartPos;
	CStaticTransparent m_lblJobDestPos;
	CStaticTransparent m_lblJobJobTyp;
	CStaticTransparent m_lblJobJobStatus;

	
	//
	
	//Edit
	CEdit m_edtRtvNo;
	CEdit m_edtJobNo;
	CEdit m_edtRtvMode;
	CEdit m_edtComplete;
	CEdit m_edtRtvProdLoad;
	CEdit m_edtRtvStatus;
	CEdit m_edtActive;
	CEdit m_edtHorizontalPos;
	CEdit m_edtJisangMode; 

	CEdit m_edtJobJobNo;
	CEdit m_edtJobJobStatus;
	CEdit m_edtJobProdInfo;
	CEdit m_edtDiagnosis;
	CEdit m_edtRtvSuspend;
	//

	//combo
	CComboBoxWrapper m_cbxRtvStartPos;
	CComboBoxWrapper m_cbxRtvDestPos;
	CComboBoxWrapper m_cbxRtvJobTyp;


	CComboBoxWrapper m_cbxRtvJobStartPos;
	CComboBoxWrapper m_cbxRtvJobDestPos;
	CComboBoxWrapper m_cbxRtvJobJobTyp;

	CTGroupBox m_grpRtvStatus;
	CTGroupBox m_grpRtvStatusItem;
	CTGroupBox m_grpRtvStatusFk1;
	CTGroupBox m_grpRtvStatusCommand;

	CTGroupBox m_grpRtvStatusItem3;
	CTGroupBox m_grpRtvStatusRtv;
	CTGroupBox m_grpRtvJobStatus;
	CTGroupBox m_grpRtvJobStatusItem;
	CTGroupBox m_grpRtvJobStatusFk1;
	CTGroupBox m_grpRtvCommandRightBottom;

	CTGroupBox m_grpRtvErrorInfomation;
	CTGroupBox m_grpRtvSuspend;

	//Button
	CSkinButton m_btnRtvComplete;
	CSkinButton m_btnRtvEstop;
	CSkinButton m_btnRtvActive;
	CSkinButton m_btnRtvStop;
	CSkinButton m_btnRtvResetError;
	CSkinButton m_btnRtvDelete;
	CSkinButton m_btnRtvCallToHome;
	CSkinButton m_btnRtvManual;
	CSkinButton m_btnRtvSuspend;
	//

public:
	void SetButtonScImage( CButton& pButton, CString pFieldName, int pValue );

	void InvalidateRtvData(EN_LANG pLang);
	void InvalidateLangControl( EN_LANG enLangTemp );
	void UpdateRtvData(int pBntJob);

	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnRtvComplete();
	afx_msg void OnBnClickedBtnRtvEstop();
	afx_msg void OnBnClickedBtnRtvActive();
	afx_msg void OnBnClickedBtnRtvStop();
	afx_msg void OnBnClickedBtnRtvResetError();
	afx_msg void OnBnClickedBtnRtvDelete();
	afx_msg void OnBnClickedBtnRtvCallToHome();
	afx_msg void OnBnClickedBtnRtvSuspend();
	afx_msg void OnBnClickedChkRtvFork1();
	afx_msg void OnBnClickedChkRtvFork2();
	afx_msg void OnBnClickedChkRtvFork12();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedRtvvResend();
	afx_msg void OnBnClickedRtvvOk();
	void SetLed(int nId, CString strVal);
	HBRUSH m_brLedOn;
	HBRUSH m_brLedOff;
	HBRUSH m_brStatus;
	CString GetQrySelectJOB_MST( CRTV_DATA* pRTV_DATA );
	CString GetQrySelectRTV_STATUS_CCD( CRTV_DATA* pRTV_DATA );
};
