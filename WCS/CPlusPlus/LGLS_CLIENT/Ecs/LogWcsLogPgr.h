#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "NtnChart.h"
// CLogWcsLogPgr 대화 상자입니다.

class CLogWcsLogPgr : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CLogWcsLogPgr)

public:
	CLogWcsLogPgr(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CLogWcsLogPgr(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogWcsLogPgr();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG_WCS_LOG_PGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	int m_nActiveRow;

public:
	EN_LANG m_nLang;

public:
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(EN_LANG pnLANG);

protected:

	void InitializeSpread(int nRowCheck, BOOL bSearch);
	int SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetColumnText(int nColIdx, int nRowIdx, CString strColumnName);
	void SetMaxRows(int pRowCnt);
	int SetSpeadData(int nRowCheck, BOOL bSearch);
	void SetColWidth(int pColCnt, int pColSize);
	void SetMaxCols(int pMaxCol);
	CString GetQrySelect_Main(int nRowCheck, BOOL bSearch);

protected: 
	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	afx_msg LRESULT OnLglsHdrSort(WPARAM wParam, LPARAM lParam);	// [LGLS]
	DECLARE_MESSAGE_MAP()
public:
	//CFpspread1 m_pSpreadMain;
	CSpreadSheet m_SpreadSheet;

	CSkinButton m_btnWcsLogSearch;
	CComboBoxWrapper m_cbxRowCnt;
	CComboBoxWrapper m_cbxWcsLogPgrNm;
	CComboBoxWrapper m_cbxWcsLogWhTyp;
	CDateTimeCtrl m_dtpFrom;
	CDateTimeCtrl m_dtpFromTime;
	CDateTimeCtrl m_dtpTo;
	CDateTimeCtrl m_dtpToTime;
	CEdit m_edtWcsLogDestpos;
	CEdit m_edtWcsLogMessage;
	CEdit m_edtWcsLogStartPos;
	CTGroupBox m_grpWcsSearch;
	CTGroupBox m_grpWcsSearch2;
	CStaticTransparent m_lblWcsLogCnt;
	CStaticTransparent m_lblWcsLogDestPos;
	CStaticTransparent m_lblWcsLogFromDate;
	CStaticTransparent m_lblWcsLogLuggNo;
	CStaticTransparent m_lblWcsLogMessage;
	CStaticTransparent m_lblWcsLogPgrNm;
	CStaticTransparent m_lblWcsLogResult;
	CStaticTransparent m_lblWcsLogStartPos;
	CStaticTransparent m_lblWcsLogToDate;
	CStaticTransparent m_lblWcsLogWhTyp;
	CEdit m_edtWcsLogLuggNo;


	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnWcsLogSearch();
};
