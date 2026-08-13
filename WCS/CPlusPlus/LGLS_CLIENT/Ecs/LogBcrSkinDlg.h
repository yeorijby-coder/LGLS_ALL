#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "NtnChart.h"

// CLogBcr 대화 상자입니다.

class CLogBcrSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CLogBcrSkinDlg)

public:
	CLogBcrSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CLogBcrSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogBcrSkinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG_BCR };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	EN_LANG m_nLang;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(EN_LANG pnLANG);
	void RelocationControls();

	void FillSpreadColumn(CStringArray& strCols);

	void InitializeSpread(int nRowCheck, BOOL bSearch);
	int SetSpeadData(int nRowCheck, BOOL bSearch);
	int SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetColumnText(int nColIdx, int nRowIdx, CString strColumnName);
	void SetMaxRows(int pRowCnt);
	void SetColWidth(int pColCnt, int pColSize);
	void SetMaxCols(int pMaxCol);

public:
	CString GetQrySelect(int nRowCheck, BOOL bSearch);

public:
	CStaticTransparent m_lblTrayBottom;
	CStaticTransparent m_lblTrayTop;
	CStaticTransparent m_lblBcrMcNo;
	CStaticTransparent m_lblBcrNo;
	CStaticTransparent m_lblFromDate;
	CStaticTransparent m_lblToDate;
	CStaticTransparent m_lblLogBcrResult;
	CStaticTransparent m_lblLogBcrCnt;

	CEdit m_edtTrayBottom;
	CEdit m_edtTrayTop;
	CEdit m_edtBcrMcNo;
	CEdit m_edtBcrNo;
	
	CDateTimeCtrl m_dtInsStartD;
	CDateTimeCtrl m_dtInsStartT;
	CDateTimeCtrl m_dtInsEndD;
	CDateTimeCtrl m_dtInsEndT;

	CTGroupBox m_grpLogBcrSearch;
	CTGroupBox m_grpLogBcrSearch2;

	CSkinButton m_btnLogBcrSearch;

	//CFpspread1 m_SpLogBcr;
	CSpreadSheet m_SpreadSheet;

	HICON m_hIcon;
	BOOL m_bInitialized;

	CComboBoxWrapper m_cbxRowCnt;
	CSkinButton m_btnPre;
	CSkinButton m_btnNext;

	int m_pPreRowCnt;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg LRESULT OnLglsHdrSort(WPARAM wParam, LPARAM lParam);	// [LGLS]
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnBnClickedBtnLogBcrSearch();
	afx_msg void OnClose();
	
	afx_msg void OnSetfocusCmbRowCnt();
	afx_msg void OnBnClickedNextRow();
	afx_msg void OnBnClickedPreRow();
	CComboBoxWrapper m_cbxLogBcrWhTyp;
	CStaticTransparent m_lblLogBcrWhTyp;
	CComboBoxWrapper m_cbxLogBcrWeightReadSta;
	CStaticTransparent m_m_lblLogBcrWeightReadSta;
};
