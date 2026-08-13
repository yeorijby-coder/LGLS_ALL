#pragma once

#include "Resource.h"

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"

//#include "fpspread1.h"
//#include "SpreadSheet.h"
#include "StaticTransparent.h"

#include "TGroupBox.h"

#include "FontManagerDialog.h"
#include "ComboBoxWrapper.h"

#include "ExtLabel.h"

// CViewJobListDlg 대화 상자입니다.

class CViewJobListDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CViewJobListDlg)

public:
	CViewJobListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CViewJobListDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewJobListDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEW_JOBLIST1 };
	enum { enComboWhTyp = 1, enComboStartPos = 2, enComboDestPos = 3, enComboJobTyp = 4, enComboJobStauts = 5 };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	int m_nLang;

public:
	CString m_strWhTypCode;
	CString m_strStartPosCode;
	CString m_strDestPosCode;
	CString m_strJobTypCode;
	CString m_strJobStatusCode;
	int m_nActiveRow;
	bool bFirst;

	CString m_strJOB_TYP;


public:
//	CFpspread1 m_pSpreadMain;
	CSpreadSheet m_SpreadSheet;

public:
	CTrackInfo* m_pTrackInfo;	

public:
	//ddx
	CStaticTransparent m_lblWhType;
	CComboBoxWrapper  m_cmbWhTyp;
	CStaticTransparent m_lblLuggNum;
	CEdit	   m_edtLuggNum;
	CStaticTransparent m_lblStartPos;
	CStaticTransparent m_lblSelectCnt;
	CComboBoxWrapper  m_cmbStartPos;
	CStaticTransparent m_lblDestPos;
	CComboBoxWrapper  m_cmbDestPos;
	CComboBoxWrapper  m_cmbJobTyp;
	CStaticTransparent m_lblJobTyp;
	CStaticTransparent m_lblJobStatus;
	CComboBoxWrapper  m_cmbJobStatus;
	CStaticTransparent m_lblBcrTop;
	CEdit	   m_edtBcrTop;
	CStaticTransparent m_lblBcrBottom;
	CEdit	   m_edtBcrBottom;
	CStaticTransparent m_lblSpdMainCnt;
	CSkinButton m_btnJobDelete;
	CSkinButton m_btnJobDataClear;
	CSkinButton m_btnJobSearch;
	CSkinButton m_btnJobUpdate;
	CTGroupBox m_grpSearch;
	CTGroupBox m_grpSearch2;
	CTGroupBox m_grpEdit;
	CStaticTransparent m_lblJobStatus2;
	CComboBoxWrapper  m_cmbJobStatus2;
	CStaticTransparent m_lblBcrTop2;
	CEdit	   m_edtBcrTop2;
	CStaticTransparent m_lblBcrBottom2;
	CEdit	   m_edtBcrBottom2;

	CStaticTransparent m_lblJobPriority;
	CStaticTransparent m_lblProductSize;
	CComboBoxWrapper m_cbxJobPriority;
	CComboBoxWrapper m_cbxProductSize;
	CSkinButton m_btnJobCopy;

	
	CSkinButton m_btnJobCvComplete;
	CSkinButton m_btnJobScComplete;


//
//	CMFCMaskedEdit	   m_edtLocation1;
//	CMFCMaskedEdit	   m_edtLocation2;
//	CMFCMaskedEdit	   m_edtStoDate1;
//	CMFCMaskedEdit	   m_edtStoDate2;
//
//	CEdit	   m_edtProdectSize;
//	CEdit	   m_edtRemarks;
//	CEdit      m_edtDurationTime;
//

//
//
///*
//	CSkinButton m_btnJobInsert;*/
//	CSkinButton m_btnLocationTo;
//	CSkinButton m_btnStoDateTo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCommandRangeButtonEvent(UINT nID);
	afx_msg LRESULT OnUpdateSpread(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP();
	DECLARE_EVENTSINK_MAP();

public:
	//CSkinButton m_btnOk; //20190306
	//CSkinButton m_btnCancel;
	//CStaticTransparent m_lblLocation;
	//CStaticTransparent m_lblStoDate;
	//CStaticTransparent m_lblProductSize;
	//CStaticTransparent m_lblRemarks;
	//CStaticTransparent m_lblSelect;
	//CStaticTransparent m_lblDurationTime;
	//CStaticTransparent m_lblJobMstResult;
	//
		

	//CSkinButton m_btnSearch;
	//CSkinButton m_btnManualAdd;
	//CSkinButton m_btnManualDelete;
	//CSkinButton m_btnDataClear;
	//CSkinButton m_btnCopy;



	void InitializeJOB_LIST(BOOL bSearch);
	void FillSpreadColumn(int nColIdx, CString strColumnName);
	void FillSpreadRow(int nColIdx, int nRowIdx, CString strValue);
	//int GetListJOB_LIST(CStringList &strListUSER_GRP, BOOL bSearch);
	CString GetJobMstQry(BOOL bSearch = FALSE);
	CString SetJobMstQry();


	afx_msg void OnBnClickedJobSearch();
	afx_msg void OnBnClickedJobLocationTo();
	afx_msg void OnBnClickedJobStodateTo();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedJobInsert();
	afx_msg void OnBnClickedJobDelete();
	afx_msg void OnBnClickedJobCopy();
	afx_msg void OnBnClickedJobDataClear();

	void UpdateJob();
	void DeleteJob();
	void DataInputCheck();
	void ClearInputData();
	void InitializeControlLanguage();
	void CreateSemiJob();
	void InvalidateComboBoxData(CComboBox* pComboBox, CString pTemp);
	void FillComboBoxWhtyp();
	void FillComboBoxStartPos();
	void FillComboBoxDestPos();
	void FillComboBoxJobTyp();
	void FillComboBoxJobStatus();


	//void BindCombo(CComboBox& cbx, CString strCDX_CD);

	afx_msg void OnBnClickedJobRtvMove();
	afx_msg void OnBnClickedJobMove1();
	afx_msg void OnBnClickedJobMove2();
	afx_msg void OnBnClickedJobMove3();
	afx_msg void OnBnClickedJobSto1();
	afx_msg void OnBnClickedJobSto2();
	afx_msg void OnBnClickedJobSto3();
	afx_msg void OnBnClickedJobSto4();


	void ShapTestInsertJobMst(CString strStartPos, CString strDestPos, CString strJobTyp);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedViewJoblistCancel();

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void UpdateSpreadSheet();
	void MoveLocation();
	void MoveStoDate();
	void FillCopyJob(int nColIdx, int nRowIdx, CString &strValue);
	CString FillCopyJob(int nColIdx, int nRowIdx);
	void BindCombo(CComboBox& cbx, CString strCDX_CD);
	void SetSpreadColumn();
	CString GetQrySelect_LUGG_MST();
	void SetBindCombo_DEST_POS_DEF(CComboBoxWrapper& cbx);

	void ClickSpread(long Col, long Row);
	LRESULT OnSpreadLClick(WPARAM wParam, LPARAM lParam);
	
	//스프레드 값 채우기 함수
protected:
	void InitializeSpread(BOOL bSearch);
	int SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetColumnText(int nColIdx, int nRowIdx, CString strColumnName);
	void SetMaxRows(int pRowCnt);
	int SetSpeadData(BOOL bSearch);
	void SetColWidth(int pColCnt, int pColSize);
	void SetMaxCols(int pMaxCol);
	CString GetQrySelect_Main(BOOL bSearch);
	CString CViewJobListDlg::GetColumnName(CString pColNm);
	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	void SetItemReSize();
	void GetItemID(CStringArray& strArr);
	void ReSizing(int nID);
	
//	afx_msg void OnBnClickedJobUpdate();
	CREATESTRUCT cs;

	void CopyJob();
	void JobComplete(CString pJOB_STATUS);

	BOOL m_bFirstStart;
};
