#pragma once
#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "NtnChart.h"



class CLogEqpErrHisSkinDlg : public CSkinDialog,  CFontManagerDialog
{
	DECLARE_DYNAMIC(CLogEqpErrHisSkinDlg)

public:
	CLogEqpErrHisSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CLogEqpErrHisSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogEqpErrHisSkinDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG_EQP_ERR_HIS };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	EN_LANG m_nLang;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(EN_LANG pnLANG);

public:
	CString GetQrySelect();

public:
	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	CSkinButton m_btnSearch;

	CStaticTransparent m_lblWH_TYP;
	CStaticTransparent m_lblTRAY_TOP;
	CStaticTransparent m_lblTRAY_BOTTOM;
	CStaticTransparent m_lblLUGG_NO;
	CStaticTransparent m_lblFROM_DATE;
	CStaticTransparent m_lblTO_DATE;
	CStaticTransparent m_lblEQP_NO;
	CStaticTransparent m_lblEQP_TYP;
	CStaticTransparent m_lblSEL_RESULT;

	CEdit m_edtTrayBottom;
	CEdit m_edtTrayTop;
	CEdit m_edtLUGG_NO;
	CEdit m_edtEQP_NO;

	CDateTimeCtrl m_dtpFrom;
	CDateTimeCtrl m_dtpFromTime;
	CDateTimeCtrl m_dtpTo;
	CDateTimeCtrl m_dtpToTime;

	CComboBoxWrapper m_cbxWH_TYP;
	CComboBoxWrapper m_cbxEQP_TYP;

	CSkinButton m_btnPre;
	CSkinButton m_btnNext;
	CComboBoxWrapper m_cbxRowCnt;
	int m_pPreRowCnt;


	CTGroupBox m_grp1;
	CTGroupBox m_grp2;

	//CFpspread1 m_pSpreadMain;
	CSpreadSheet m_SpreadSheet;
	CStaticTransparent m_lblSpdMainCnt;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogSearch();
	void FillSpreadColumn(CStringArray& strCols);
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedOk2();



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
	CStatic m_lblEqpErrCd;
	CStatic m_lblEqpErrMsg;
	CEdit m_edtEqpErrCd;
	CEdit m_edtEqpErrMsg;
};