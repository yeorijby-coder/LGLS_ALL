#pragma once
#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "NtnChart.h"

// CLogEqpJobHisSkinDlg 대화 상자입니다.

class CLogEqpJobHisSkinDlg : public CSkinDialog,  CFontManagerDialog
{
	DECLARE_DYNAMIC(CLogEqpJobHisSkinDlg)

public:
	CLogEqpJobHisSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CLogEqpJobHisSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogEqpJobHisSkinDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG_EQP_JOB_HIS };

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
	CStaticTransparent m_lblFROM_LOCATION;
	CStaticTransparent m_lblTO_LOCATION;
	CStaticTransparent m_lblFromDate;
	CStaticTransparent m_lblToDate;
	CStaticTransparent m_lblFromTrack;
	CStaticTransparent m_lblToTrack;

	CEdit m_edtTrayBottom;
	CEdit m_edtTrayTop;
	CEdit m_edtLuggNo;
	CEdit m_edtFROM_LOCATION;
	CEdit m_edtTO_LOCATION;	
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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogSearch();
	void FillSpreadColumn(CStringArray& strCols);
};
