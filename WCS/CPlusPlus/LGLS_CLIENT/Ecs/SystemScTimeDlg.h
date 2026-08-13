#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"
#include "EcsDoc.h"
#include "NtnChart.h"
#include "FontManagerDialog.h"
#include "Ecs.h"
#include "TGroupBox.h"


// CSystemScTimeDlg 대화 상자입니다.

class CSystemScTimeDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CSystemScTimeDlg)

public:
	enum EN_TAB_IDX 
	{ 
		EN_TAB_IDX_CV = 0, EN_TAB_IDX_SC = 1
	};
	CString GetTabIdxEnumToCString(EN_TAB_IDX enTabIdx);

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	CSystemScTimeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CSystemScTimeDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);
	virtual ~CSystemScTimeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SYSTEM_SCTIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;

	CSkinButton m_btnAgingType;
	CSkinButton m_btnQrySelect;
	CComboBoxWrapper m_drbtnAgingType;
	CTGroupBox m_grpUsageCondition;
	CTGroupBox m_grpUsageCondtion;
	CTabCtrl* m_tabUsage;

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
		CNtnChart m_ntnChart;
		afx_msg void OnBnClickedOk();
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnBnClickedSystemSctimeCancel();
		void InitilizeChart(EN_TAB_IDX nTabIndex);
		void UpdateChart();


public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	afx_msg void OnCbnSelchangeDrpbxAgingType();
	afx_msg void OnBnClickedBtnQrySelect();
	void InitializeResource(int pnTabIndex);
	afx_msg void OnBnClickedBtnAgingType();
};
