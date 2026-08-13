#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"
#include "EcsDoc.h"


#include "fpspread1.h"
//#include "SpreadSheet.h"

// CUserGroupDlg 대화 상자입니다.

class CUserGroupDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CUserGroupDlg)

public:
	CUserGroupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
		CUserGroupDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserGroupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USER_GROUP };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedBtnDelete2();
	afx_msg void OnBnClickedBtnSearch2();
	DECLARE_MESSAGE_MAP()

public:
	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	CSkinButton m_btnDelete;
	CSkinButton m_btnSearch;

	CStaticTransparent m_lblGrpNm;
	CStaticTransparent m_lblGroupDesc;
	CStaticTransparent m_lblRemarks;
	CStaticTransparent m_lblResult;
	CStaticTransparent m_lblCnt;

	CComboBoxWrapper m_cbxGrpCd;
	CEdit m_edtGrpDesc;
	CEdit m_edtRemarks;

	CButton	   m_chkCheckAll;


	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CFpspread1 m_pUSER_GRP;

	void InitializeControlLanguage();
	void InitializeUSER_GRP(BOOL bSearch);
	int GetListUSER_GRP(CStringList &strListUSER_GRP, BOOL bSearch);
	void FillSpreadColumn(int nColIdx, CString strColumnName);
	void FillSpreadRow(int nColIdx, int nRowIdx, CString strValue);
	void FillComboBoxGrpCd();
	void DeleteUserMst();

};
