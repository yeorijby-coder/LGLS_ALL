#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"
#include "EcsDoc.h"
// CUserCreateUserDlg 대화 상자입니다.

class CUserCreateUserDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CUserCreateUserDlg)

public:
	CUserCreateUserDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
		CUserCreateUserDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserCreateUserDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USER_CREATEUSER };

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
	DECLARE_MESSAGE_MAP()

public:
	CStatic m_lblUSER_ID;
	CStatic m_lblUSER_PW;
	CStatic m_lblUSER_NM;
	CStatic m_lblGRP_CD;
	CStatic m_lblREMARKS;

	CEdit m_edtUSER_ID;
	CEdit m_edtUSER_PW;
	CEdit m_edtUSER_NM;
	CComboBoxWrapper m_comboGRP_CD;
	CEdit m_edtREMARKS;

	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void InitializeControlLanguage();
	void FillComboBox();
	afx_msg void OnBnClickedUserCreateruserOk();
	afx_msg void OnBnClickedUserCreateruserCancel();
};
