#pragma once
#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"


// CManualJob 대화 상자입니다.

class CManualJob : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CManualJob)

public:
	CManualJob(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CManualJob(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent = NULL);   // 표준 생성자입니다
	virtual ~CManualJob();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANAUL_JOB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;
	 
	int m_nLang;

public:
	HICON m_hIcon;
	BOOL m_bInitialized;

public:
	CStaticTransparent m_lblManualJobWhTyp;
	CStaticTransparent m_lblManualJobJobNo;
	CStaticTransparent m_lblManualJobStartPos;
	CStaticTransparent m_lblManualJobDestPos;
	CStaticTransparent m_lblManualJobJobTyp;
	CStaticTransparent m_lblManualJobStatus;
	CStaticTransparent m_lblManaulJobBcrTop;
	CStaticTransparent m_lblManualJobBcrBottom;
	//CStaticTransparent m_lblManualJobProductSize;
	//CStaticTransparent m_lblManualJobDurationTime;
	CStaticTransparent m_lblManualJobRemark;
	CStaticTransparent m_lblManualJobLocation;
	CStaticTransparent m_lblManualJobLocation_To;
	//CStaticTransparent m_btnManualJobDataClear;
	
	CTGroupBox m_grpManualJobSearch;

	CSkinButton m_btnManualJobAdd;
	CSkinButton m_btnManualJobDataClear;

	CComboBoxWrapper m_cbxManualJobWhTyp;
	CComboBoxWrapper m_cbxManualJobStartPos;
	CComboBoxWrapper m_cbxManualJobDestPos;
	CComboBoxWrapper m_cbxManualJobJobTyp;
	CComboBoxWrapper m_cbxManualJobJobStatus;
	
	CEdit m_edtManualJobJobNo;
	CEdit m_edtManualJobBcrTop;
	CEdit m_edtManualJobBcrBottom;
	CEdit m_edtManualJobStartLocation;
	CEdit m_edtManualJobDestLocation;
	CEdit m_edtManualJobRemark;
	
	CMFCMaskedEdit m_maskedtManualJobLocationFr;
	CMFCMaskedEdit m_maskedtManualJobLocationTo;

	CComboBoxWrapper m_cbxManualJobProductSize;
	CStaticTransparent m_lblManualJobProductSize;

	
	CComboBoxWrapper m_cbxManualJobCnt;
	CComboBoxWrapper m_cbxManualJobPriority;
	CStaticTransparent m_lblManualJobCnt;
	CStaticTransparent m_lblManualJobPriority;


public:
	void RelocationControls();
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void OnBnClickCompleteReport();

	CString GetJobMstQry(BOOL bSearch = FALSE);
	CString SetJobMstQry();
	CString GetQryUpdate(CString strWH_TYP, CString strLugg_No, CString strJopTyp);
	CString GetQryCellByJobTyp( CString strWH_TYP, CString strSC_NO, CString strLOCATION, CString strJOB_TYP, CString strJOB_STATUS);
	CString GetQryDelete( CString strWH_TYP, CString strLugg_No);
	CString GetQryJobMst();

	void refresh_Chk();
	CString GetQrySelectCELLMST(CString strBANK, CString strBAY, CString strLEV, CString& strSC_NO, CString& strCELL_USE_YN, CString& strSC_PLT_JOB_TYP, int& strRowCnt);
	CString GetQrySelectCV_DATA(CString strMC_NO, CString& strSC_PLT_JOB_TYP, int& nCnt );


	afx_msg void OnCommandRangeButtonEvent(UINT nID);
	afx_msg void OnBnClickedBtnManulJobInsert();
	afx_msg void OnClose();
	

public:
	DECLARE_EVENTSINK_MAP()

	afx_msg void OnBnClickedBtnManualJobClear();
	afx_msg void OnSelchangeComboManualJobEditJobTyp();
	afx_msg void OnCbnSelchangeComboManualJobWhtype();
};
