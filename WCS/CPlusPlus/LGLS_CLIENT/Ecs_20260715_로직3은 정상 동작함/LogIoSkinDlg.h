#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "NtnChart.h"

// CLogIoSkinDlg 대화 상자입니다.

class CLogIoSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CLogIoSkinDlg)

public:
	CLogIoSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CLogIoSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogIoSkinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG_IO };

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

	CStaticTransparent m_lblWhTyp;
	CStaticTransparent m_lblTrayBottom;
	CStaticTransparent m_lblTrayTop;
	CStaticTransparent m_lblLuggNo;
	CStaticTransparent m_lblRequestId;
	CStaticTransparent m_lblFromDate;
	CStaticTransparent m_lblToDate;
	CStaticTransparent m_lblFromTrack;
	CStaticTransparent m_lblToTrack;
	CStaticTransparent m_lblFromLocation;
	CStaticTransparent m_lblToLocation;
	CStaticTransparent m_lblJobTyp;
	
	CEdit m_edtTrayBottom;
	CEdit m_edtTrayTop;
	CEdit m_edtLuggNo;
	CEdit m_edtRequestId;
	CEdit m_edtFromTrack;
	CEdit m_edtToTrack;
	CEdit m_edtFromLocation;
	CEdit m_edtToLocation;

	CDateTimeCtrl m_dtpFrom;
	CDateTimeCtrl m_dtpFromTime;
	CDateTimeCtrl m_dtpTo;
	CDateTimeCtrl m_dtpToTime;

	CComboBoxWrapper m_cbxIOLogWhTyp;
	CComboBoxWrapper m_cbxJobTyp;


	CTGroupBox m_grp1;
	CTGroupBox m_grp2;

	//CFpspread1 m_spdLog;
	CSpreadSheet m_SpreadSheet;

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
	afx_msg void OnBnClickedBtnLogIoSearch();
	void FillSpreadColumn(CStringArray& strCols);


	//스프레드 값 채우기 함수
protected:

	//CFpspread1 m_pSpreadMain;
	CStaticTransparent m_lblSpdMainCnt;
	CStaticTransparent m_lblSpdMain;

	void InitializeSpread(int nRowCheck, BOOL bSearch);
	int SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetColumnText(int nColIdx, int nRowIdx, CString strColumnName);
	void SetMaxRows(int pRowCnt);
	int SetSpeadData(int nRowCheck, BOOL bSearch);
	void SetColWidth(int pColCnt, int pColSize);
	void SetMaxCols(int pMaxCol);
	CString GetQrySelect_Main(int nRowCheck, BOOL bSearch);

	//ROW 수 조절
protected:
	CSkinButton m_btnPre;
	CSkinButton m_btnNext;
	CComboBoxWrapper m_cbxRowCnt;
	int m_pPreRowCnt;

	void OnBnClickedPreRow();
	void OnBnClickedNextRow();

};
