#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "NtnChart.h"

// CLogEqpSkinDlg 대화 상자입니다.

class CLogEqpSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CLogEqpSkinDlg)

public:
	CLogEqpSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CLogEqpSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogEqpSkinDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG_EQP};

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
	void GetEQPLogSelect();
	void FillSpreadRow(int nColIdx, int nRowIdx, CString strValue);

public:
	CString GetQrySelect();

public:
	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	CSkinButton m_btnSearch;

	CTGroupBox m_grpLogEqpSearch;
	CTGroupBox m_grpLogEqpSearch2;

	CStaticTransparent m_lblEqpMstCnt;//SK

	CStaticTransparent m_lblWhTyp;
	CStaticTransparent m_lblTrayBottom;
	CStaticTransparent m_lblTrayTop;
	CStaticTransparent m_lblLuggNo;
	CStaticTransparent m_lblRequestId;
	CStaticTransparent m_lblFromDate;
	CStaticTransparent m_lblToDate;
	CStaticTransparent m_lblFromTrack;
	CStaticTransparent m_lblToTrack;

	CStaticTransparent m_lblEqpLogWhTyp; //SK
	CStaticTransparent m_lblEqpLogEqpTyp;
	CComboBoxWrapper m_cbxEqpLogWhTyp;
	CComboBoxWrapper m_cbxEqpLogEqpTyp;
	CStaticTransparent m_lblEQPMstResult;
			  
	CStaticTransparent  m_lblEqpKorLog;
	CEdit				m_cbxEqpKorLog;

	CEdit m_edtTrayBottom;
	CEdit m_edtTrayTop;
	CEdit m_edtLuggNo;
	CEdit m_edtRequestId;
	CEdit m_edtFromTrack;
	CEdit m_edtToTrack;
			

	CDateTimeCtrl m_dtpFrom;
	CDateTimeCtrl m_dtpFromTime;
	CDateTimeCtrl m_dtpTo;
	CDateTimeCtrl m_dtpToTime;

	CFpspread1 m_spdLog;

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
	afx_msg LRESULT OnLglsHdrSort(WPARAM wParam, LPARAM lParam);	// [LGLS]
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogIoSearch();
	void FillSpreadColumn(CStringArray& strCols);
};
