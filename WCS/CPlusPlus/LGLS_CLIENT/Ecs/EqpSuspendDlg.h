#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"

//#include "fpspread1.h"
//#include "SpreadSheet.h"

#include "ExtLabel.h"

#include "FontManagerDialog.h"

// CScSuspendDlg 대화 상자입니다.

class CEqpSuspendDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CEqpSuspendDlg)

public:
	CEqpSuspendDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CEqpSuspendDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEqpSuspendDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_EQP_SUSPEND };

public:
	CEcsDoc* m_pDoc;

public:
	CString m_strScEqpSuspend;
	CString m_strOtherEqpSuspend;


public:
	EN_LANG m_nLang;

	//CFpspread1 m_pSpEQPSUSPEND;
	CSpreadSheet m_SpreadSheet;
	long m_nActiveRow;

	CStaticTransparent m_lblEqpSuspendWhTyp;
	CStaticTransparent m_lblEqpSuspendEqpTyp;
	CStaticTransparent m_lblEqpSuspendUseYn;
	CStaticTransparent m_lblEqpSuspnedConnectedYn;
	CStaticTransparent m_lblEqpSuspendEqp;
	CStaticTransparent m_lblEqpSuspend;
	CStaticTransparent m_lblEqpSuspendResult;
	CStaticTransparent m_lblEqpSuspendCnt;

	CStaticTransparent m_lblEqpPLC_IP;
	CStaticTransparent m_lblEqpPLC_PORT_FROM;
	CStaticTransparent m_lblEqpPLC_PORT_TO;
	CStaticTransparent m_lblEqpUSE_YN;
	CStaticTransparent m_lblEqpSUSPEND;

	CEdit m_edtPLC_IP;

	CEdit m_edtPLC_PORT_FROM;
	CEdit m_edtPLC_PORT_TO;

	CComboBoxWrapper m_cbxUSE_YN;
	CComboBoxWrapper m_cbxEQP_SUSPEND;



	CComboBoxWrapper m_cbxEqpSuspendWhTyp;
	CComboBoxWrapper m_cbxEqpSuspendUseYn;
	CComboBoxWrapper m_cbxEqpSuspendEqpTyp;
	CComboBoxWrapper m_cbxEqpSusepndConnectedYn;
	CComboBoxWrapper m_cbxEqpSuspendEqpSuspend;
	CComboBoxWrapper m_cbxEqpSuspendEqpSuspend2;

	//CEdit m_edtEqpSuspendEqpNo;
	//CEdit m_edtEqpSuspendEqpGrpNo;
	//CEdit m_edtEqpSuspendPlcIp;
	//CEdit m_edtEqpSusepndPlcPortFr;
	//CEdit m_edtEqpSusepndPlcPortTo;

	CTGroupBox m_grpEqpEqpState;
	CTGroupBox m_grpEqpSuspend;
	CTGroupBox m_grpEqpSuspend2;

	CSkinButton m_btnEqpSuspend;
	CSkinButton m_btnEqpSuspendSearch;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnEqpSuspendSearch();
	afx_msg void OnBnClickedBtnEqpSuspendSuspend();
	afx_msg void OnBnClickedEqpSuspendCheckAll();
	afx_msg void OnClose();

	afx_msg LRESULT OnLglsHdrSort(WPARAM wParam, LPARAM lParam);	// [LGLS]
	DECLARE_MESSAGE_MAP()

	void InitializeControlLanguage();
	void InitializeSC_DATA(BOOL bSearch);
	void FillSpreadColumn(int nColIdx, CString strColumnName);
	void FillSpreadRow(int nColIdx, int nRowIdx, CString strValue);
	void FillComboBoxWhtyp();
	void FillComboBoxScSuspend();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void InitializeResource(EN_LANG pnLANG);
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();

	CString GetQrySelect( int nLang,CString strWH_TYP, CString strEQP_TYP, CString strEQP_SUSPEND, CString strEQP_NO
		, CString strEQP_GRP_NO, CString strPLC_IP, CString strPLC_PORT_FR, CString strPLC_PORT_TO
		, CString strCONNECTED_YN, CString strRETRY_YN, CString strUSE_YN);
	CString GetQryUpdate( CString strWH_TYP, CString strEQP_TYP, CString strPLC_NO, CString strEQP_NO );
	afx_msg void OnBnClickedChkScActive();
	afx_msg void OnBnClickedChkScStoSuspend();
	afx_msg void OnBnClickedChkRetSuspend();
	afx_msg void OnBnClickedChkScAllSuspend();
	afx_msg void OnBnClickedChkOtherEqpActive();
	afx_msg void OnBnClickedChkOtherSuspend();
	afx_msg void OnCbnSelchangeCbxEqpSuspendEqpTyp();
	DECLARE_EVENTSINK_MAP()
	void ClickEqpSuspend(long Col, long Row);
	CString GetQrySelect_Main();
	int SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetMaxRows(int pRowCnt);
	void SetMaxCols(int pMaxCol);
	void SetColumnText(int nColIdx, int nRowIdx, CString strColumnName);
	void SetColWidth(int pColCnt, int pColSize);
};
