#pragma once

#include "Resource.h"
#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"
#include "afxmaskededit.h"
#include "fpspread1.h"
//#include "SpreadSheet.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "ComboBoxWrapper.h"


// CViewRackDlg 대화 상자입니다.

class CViewRackDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CViewRackDlg)

public:
	CViewRackDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CViewRackDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewRackDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEW_RACK };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	int m_nLang;
	CString m_pAging;
	CString m_strWH_TYP;
	CString m_pCellScNo;
	CString m_pScNo;
	int m_nActiveRow;

public:
	CFpspread1 m_pSpreadMain;

public:
	//STATIC
	CStaticTransparent m_lblCellWhTyp;
	CStaticTransparent m_lblCellCellScNo;
	CStaticTransparent m_lblCellScNo;
	CStaticTransparent m_lblCellNo;
	CStaticTransparent m_lblCellSta;
	CStaticTransparent m_lblCellUseDef;
	CStaticTransparent m_lblCellUseYn;
	CStaticTransparent m_lblCellTyp;
	CStaticTransparent m_lblAgingDt;
	CStaticTransparent m_lblBcrTop;
	CStaticTransparent m_lblBcrBottom;
	CStaticTransparent m_lblDestPos;
	CStaticTransparent m_lblRemark;
	CStaticTransparent m_lblCellUserDef2;
	CStaticTransparent m_lblCellSta2;
	CStaticTransparent m_lblRackBcrTop2;
	CStaticTransparent m_lblRackBcrBottom2;
	CStaticTransparent m_lblRemarks3;
	CStaticTransparent m_lblSpdMainCnt;
	//-----STATIC

	//EDIT
	CEdit m_editCellNoFr;
	CEdit m_editCellNoTo;
	CEdit m_edtAgingStartDt;
	CEdit m_edtAgingEndDt;
	CEdit m_edtTopTray;
	CEdit m_edtBottmTray;
	CEdit m_edtRemarks;
	CEdit m_edtBottomTray2;
	CEdit m_edtTopTray2;
	CEdit m_edtRemarks2;
	//----EDIT

	//BUTTON
	//CSkinButton m_btnCellNoFrTo;
	//CSkinButton m_btnAgingDtFrTo;
	CSkinButton m_btnCellNoFrTo;
	CSkinButton m_btnAgingDtFrTo;
	CSkinButton m_btnRetireve;
	CSkinButton m_btnSearch;
	CSkinButton m_btnCellRet;
	CSkinButton m_btnCellUseDefEdit;
	//----BUTTON

	//COMBO
	CComboBoxWrapper m_cmbWhTyp;
	CComboBoxWrapper m_cmbCellScNo;
	CComboBoxWrapper m_cmbScNo;
	CComboBoxWrapper m_cmbCellUseDef;
	CComboBoxWrapper m_cmbCellSta;
	CComboBoxWrapper m_cmbCellSta2;
	CComboBoxWrapper m_cmbAgingTyp;
	CComboBoxWrapper m_cmbDestPos;

	CComboBoxWrapper m_cmbCellUseDef2;
	
	//----COMBO

	//DATETIME
	CDateTimeCtrl m_dtAgingStartD;
	CDateTimeCtrl m_dtAgingStartT;
	CDateTimeCtrl m_dtAgingEndD;
	CDateTimeCtrl m_dtAgingEndT;
	//----DATETIME

	CMFCMaskedEdit m_MaskEditCellNoFr;
	CMFCMaskedEdit m_MaskEditCellNoTo;

	CButton	   m_chkCheckAll;

	CTGroupBox m_grpCellSearch;
	CTGroupBox m_grpCellSearch2;
	CTGroupBox m_grpCellMst;
	CTGroupBox m_grpCellEdit;


	/////////////////////샵 테스트////////////////////
	CStaticTransparent m_lblCellMstResult;
	//////////////////////////////////////////////////

public:

	enum {	 enWH_TYP = 2, enWH_TYP_NM = 3, enCELL_SC_NO = 4, enSC_NO = 5, enCELL_NO = 6, enCELL_STA = 7
		   , enCELL_STA_NM = 8,  enCELL_USE_DEF , enCELL_USE_DEF_NM 
           , enAGING_TYP , enAGING_TYP_NM 
		   , enAGING_START_DT , AGING_END_DT , enBOTTOM_TRAY, enTOP_TRAY 
		   , enREMARKS , enINS_USER_ID , enINS_DT , enUPD_USER_ID , enUPD_DT 

	     };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();
	void InitializeResource(EN_LANG pnLANG);

	virtual BOOL OnInitDialog();

	void InitializeControlLanguage();
	void FillSpreadColumn(int nColIdx, CString strColumnName);
	void FillSpreadRow(int nColIdx, int nRowIdx, CString strValue);
	void ClearInputData();
	
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	CString GetQryInsert(CString strWH_TYP, CString strSC_NO, CString strCELL_NO,  CString strDEST_POS, CString strBOTTOM_TRAY, CString strTOP_TRAY);
	CString GetQryUpdate(CString strWH_TYP, CString strCELL_SC_NO, CString strCELL_NO, CString strCELL_USE_DEF, CString strUP_CELL_STA, CString strUP_BOTTOM_TRAY, CString strUP_TOP_TRAY, CString strUP_REMARK);

public:
	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnCellNoFrTo();
	afx_msg void OnBnClickedBtnAgingDtFrTo();
	afx_msg void OnClose();
	afx_msg void OnEnChangeMaskEditCellNoFr();
	afx_msg void OnDtnDatetimechangeDtAgingEndT(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnCellSta();
	afx_msg void OnBnClickedBtnPltUpdate();
	afx_msg void OnBnClickedBtnPltDelete();
	afx_msg void OnBnClickedViewRackCancel();

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedLblPltNo();
	afx_msg void OnBnClickedBtnCellRet();
	afx_msg void OnBnClickedBtnCellUseDef();
	afx_msg void OnSetfocusCmbScNo();
	afx_msg void OnSetfocusCmbCellScNo();
	afx_msg void OnSetfocusCmbAingTyp();

	virtual HRESULT accHitTest(long xLeft, long yTop, VARIANT *pvarChild);


	DECLARE_EVENTSINK_MAP();
	void BlockSelectedCellMst(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	void ClickSpread(long Col, long Row);

	
//스프레드 값 채우기 함수
protected:
	void InitializeSpread(BOOL bSearch);
	void SetHeadColumn_VerChk(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetColumnText_VerChk(int nColIdx, int nRowIdx, CString strColumnName);
	void SetMaxRows(int pRowCnt);
	int SetSpeadData(BOOL bSearch);
	void SetColWidth(int pColCnt, int pColSize);
	void SetMaxCols(int pMaxCol);
	CString GetQrySelect_Main(BOOL bSearch = FALSE);
	
};
