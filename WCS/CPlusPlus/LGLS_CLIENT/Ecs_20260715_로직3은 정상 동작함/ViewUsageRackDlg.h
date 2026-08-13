#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"
#include "NtnChart.h"
#include "FontManagerDialog.h"
#include "Ecs.h"
#include "TGroupBox.h"
#include "EcsDoc.h"

#define AGING_1G_MAX_BAY  22
#define AGING_2G_MAX_BAY  23
#define AGING_3G_MAX_BAY  80
#define AGING_4G_MAX_BAY  22


// CSystemScTimeDlg 대화 상자입니다.

class CViewUsageRackDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CViewUsageRackDlg)

public:
	enum EN_TAB_IDX 
	{ 
		EN_TAB_IDX_CV = 0, EN_TAB_IDX_SC = 1
	};

	enum EN_AGING{};

	CString GetTabIdxEnumToCString(EN_TAB_IDX enTabIdx);

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	CFpspread1 m_pSpdCellList;
//	CSpreadSheet m_p;

public:
	CViewUsageRackDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CViewUsageRackDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);
	virtual ~CViewUsageRackDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEW_USAGE_RACK };

public:
	CString		m_pAging;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CSkinButton m_btnOk;
	
	CSkinButton m_btnSelect; //CSkinButton
	CSkinButton m_btnCancel;
	
	CSkinButton m_btnAgingType;
	CSkinButton m_btnQrySelect;
	CComboBoxWrapper m_drbtnAgingType;
	CTGroupBox m_grpUsageCondition;
	CTGroupBox m_grpUsageCondtion;
	CTGroupBox m_grpUsageSelect;
	CTGroupBox m_grpUsageInform;
	CTGroupBox m_grpUsagePreview;
	CTGroupBox m_grpUsageSearch;
	CButton m_rdoUsage;
	CButton m_rdoFire;


	CTabCtrl* m_tabUsage;

	UINT m_nRadio;
	
	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSelChangeCmbWhTyp();
	afx_msg void OnSelChangeCmbAging();

public:
		CNtnChart m_ntnChart;
		afx_msg void OnBnClickedOk();
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnBnClickedSystemSctimeCancel();
		void UpdateChart();
		void InitializeRACK_LIST(BOOL bSearch);
		CString GetJobMstQry(BOOL bSearch);
		void FillSpreadColumn(int nColIdx, CString strColumnName);
		void FillSpreadRow(int nColIdx, int nRowIdx, CString strValue);
		void FillSpread(int nColIdx, int nRowIdx, CString strValue);

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	afx_msg void OnBnClickedBtnQrySelect();
	void InitializeResource(EN_LANG pnLANG);
	afx_msg void OnBnClickedBtnAgingType();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnSelect();
	CMFCButton m_mbtnSelect;
	LRESULT OnRefreshDialog(WPARAM wParam, LPARAM lParam);
	CComboBoxWrapper m_cbxWhTyp;
//	CComboBoxWrapper m_cbxAging;
	CComboBoxWrapper m_cbxDestPos;
	CComboBoxWrapper m_cbxAging;
	CComboBoxWrapper m_cbxBank;
	CStaticTransparent m_lblWhTyp;
	CStaticTransparent m_lblUsageCell;
	CStaticTransparent m_lblSumTray;
	CStaticTransparent m_lblRsvSto;
	CStaticTransparent m_lblRsvRet;
	CStaticTransparent m_lblPossibleSto;
	CStaticTransparent m_lblPossibleRet;
	CStaticTransparent m_lblNormal;
	CStaticTransparent m_lblEmptyTray;
	CStaticTransparent m_lblEmptyErr;
	CStaticTransparent m_lblEmptyCell;
	CStaticTransparent m_lblDualErr;
	CStaticTransparent m_lblCellTyp;
	CStaticTransparent m_lblCellStatus;
	CStaticTransparent m_lblCellCnt;
	CStaticTransparent m_lblBank;
	CStaticTransparent m_lblBanSto;
	CStaticTransparent m_lblBanRet;
	CStaticTransparent m_lblAging;
	CStaticTransparent m_lblBnk;
	CStaticTransparent m_lblAcTray;
	CStaticTransparent m_lblBanUser;
	CStaticTransparent m_lblBanPhb;
};
