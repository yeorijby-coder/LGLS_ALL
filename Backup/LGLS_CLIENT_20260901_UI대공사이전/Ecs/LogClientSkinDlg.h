#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "NtnChart.h"


// CLogClientSkinDlg 대화 상자입니다.

class CLogClientSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CLogClientSkinDlg)

public:
	CLogClientSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CLogClientSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogClientSkinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG_CLIENT };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	EN_LANG m_nLang;

	HICON m_hIcon;
	BOOL m_bInitialized;

public:
	CStaticTransparent m_lblClientLogWhTyp; 
	CStaticTransparent m_lblClientLogBcrBottom;
	CStaticTransparent m_lblClientLogBcrTop;
	CStaticTransparent m_lblClientLogLuggNo;
	CStaticTransparent m_lblClientLogUserId;
	CStaticTransparent m_lblClientLogUserIp;  
	CStaticTransparent m_lblClientLogRemark;
	CStaticTransparent m_lblClientLogRemark2;
	CStaticTransparent m_lblClientLogRemark3;
	CStaticTransparent m_lblClientLogInsFrDt;
	CStaticTransparent m_lblClientLogInsToDt; 
	CStaticTransparent m_lblClientLogResult;
	CStaticTransparent m_lblClientLogCnt;


	CTGroupBox m_grpClientSearch;
	CTGroupBox m_grpClientSearch2;
	 
	CEdit m_edtClientLogBcrBottom;
	CEdit m_edtClientLogBcrTop;
	CEdit m_edtClientLogLuggNo;
	CEdit m_edtClientLogUserId;
	CEdit m_edtClientLogUserIp; 
	CEdit m_edtClientLogMessage;
	CEdit m_edtClientLogMessage2;
	CEdit m_edtClientLogMessage3;
	
	CMFCMaskedEdit m_maskedtClientLogStartLoc;
	CMFCMaskedEdit m_maskedtClientLogDestLoc;
	
	CComboBoxWrapper m_cbxClientLogWhTyp; 

	CSkinButton m_btnClientLogSearch;
	
	
	CDateTimeCtrl m_dtpFrom;
	CDateTimeCtrl m_dtpFromTime;
	CDateTimeCtrl m_dtpTo;
	CDateTimeCtrl m_dtpToTime;

	//CFpspread1 m_pSpreadMain;
	CSpreadSheet m_SpreadSheet;
	CStaticTransparent m_lblSpdMainCnt;

	CSkinButton m_btnPre;
	CSkinButton m_btnNext;
	CComboBoxWrapper m_cbxRowCnt;
	int m_pPreRowCnt;
	CComboBoxWrapper m_cbxClientLogPgrNm;
	CStaticTransparent m_lblClientLogPgrNm;




public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(EN_LANG pnLANG);
	void RelocationControls();

	void FillSpreadColumn(CStringArray& strCols);

public:
	CString GetQrySelect();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg LRESULT OnLglsHdrSort(WPARAM wParam, LPARAM lParam);	// [LGLS]
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnLogClinetSearch();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnClose();



	//스프레드 값 채우기 함수
protected:
	void InitializeSpread(int nRowCheck, BOOL bSearch);
	int SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetColumnText(int nColIdx, int nRowIdx, CString strColumnName);
	void SetMaxRows(int pRowCnt);
	int SetSpeadData(int nRowCheck, BOOL bSearch);
	void SetColWidth(int pColCnt, int pColSize);
	void SetMaxCols(int pMaxCol);
	CString GetQrySelect_Main(int nRowCheck, BOOL bSearch);


	void OnBnClickedPreRow();
	void OnBnClickedNextRow();
	afx_msg void OnSetfocusCmbRowCnt();
public:
	afx_msg void OnBnClickedGrpLogClientSearch();
};
