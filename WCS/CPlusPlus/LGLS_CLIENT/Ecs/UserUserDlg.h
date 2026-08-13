#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "fpspread1.h"	// [LGLS 2026-08-05] 예전엔 EcsDoc.h -> ViewRackDlg.h 를 타고 들어왔다
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "NtnChart.h"
// CUserUserDlg 대화 상자입니다.

class CUserUserDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CUserUserDlg)

public:
	CUserUserDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CUserUserDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserUserDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USER_USER };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	EN_LANG m_nLang;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	afx_msg void OnBnClickedBtnInsert();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnDelete();

protected:
	afx_msg LRESULT OnLglsHdrSort(WPARAM wParam, LPARAM lParam);	// [LGLS]
	DECLARE_MESSAGE_MAP()

public:
	CStaticTransparent m_lblUserId_S;
	CEdit m_edtUserId_S;
	CStaticTransparent m_lblUserNm_S;
	CEdit m_edtUserNm_S;
	CStaticTransparent m_lblGrpNm_S;
	CComboBoxWrapper m_cbxGrpCd_S;
	CStaticTransparent m_lblRemarks_S;
	CEdit m_edtRemarks_S;

	CStaticTransparent m_lblUserId_I;
	CEdit m_edtUserId_I;
	CStaticTransparent m_lblUserNm_I;
	CEdit m_edtUserNm_I;
	CStaticTransparent m_lblGrpNm_I;
	CComboBoxWrapper m_cbxGrpCd_I;
	CStaticTransparent m_lblRemarks_I;
	CEdit m_edtRemarks_I;
	CStaticTransparent m_lblPW_I;
	CEdit m_edtPW_I;
/*
	CStaticTransparent m_lblUserId_U;
	CEdit m_edtUserId_U;
	CStaticTransparent m_lblUserNm_U;
	CEdit m_edtUserNm_U;
	CStaticTransparent m_lblGrpNm_U;
	CComboBoxWrapper m_cbxGrpCd_U;
	CStaticTransparent m_lblRemarks_U;
	CEdit m_edtRemarks_U;*/

	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	CSkinButton m_btnDelete;
	CSkinButton m_btnSearch;
	CSkinButton m_btnInsert;

	CTGroupBox m_grp1;
	CTGroupBox m_grp2;
	CTGroupBox m_grp3;

	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void InitializeControlLanguage();
	void RedrawImage();
	void RenameResource(EN_LANG enLang);
	void DeleteUserMst();
	void InitializeResource(EN_LANG enLang);
	void InsertUserMst();
	//스프레드 값 채우기 함수
public:
	CFpspread1 m_pSpreadMain;
	CStaticTransparent m_lblSpdMainResult;//SK
	CStaticTransparent m_lblSpdMainCnt;


protected:
	void InitializeSpread(int nRowCheck, BOOL bSearch);
	int SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetColumnText(int nColIdx, int nRowIdx, CString strColumnName);
	void SetMaxRows(int pRowCnt);
	int SetSpeadData(int nRowCheck, BOOL bSearch);
	void SetColWidth(int pColCnt, int pColSize);
	void SetMaxCols(int pMaxCol);
	CString GetQrySelect_Main(int nRowCheck, BOOL bSearch);

public:
	int m_nActiveRow;

	DECLARE_EVENTSINK_MAP()
	void ClickSpread(long Col, long Row);
	afx_msg void OnClose();
};
