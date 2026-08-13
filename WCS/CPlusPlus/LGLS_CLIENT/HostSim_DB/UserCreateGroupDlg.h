#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"
#include "EcsDoc.h"
// CUserCreateGroupDlg 대화 상자입니다.

class CUserCreateGroupDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CUserCreateGroupDlg)

public:
	CUserCreateGroupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
		CUserCreateGroupDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserCreateGroupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USER_CREATEGROUP };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

	CStaticTransparent m_lblGrpCd;
	CStaticTransparent m_lblGrpDesc;
	CStaticTransparent m_lblRemarks;

	CEdit m_edtGrpCd;
	CEdit m_edtGrpDesc;
	CEdit m_edtRemarks;




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
	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void InitializeControlLanguage();
	afx_msg void OnBnClickedUserCreategroupOk();
	afx_msg void OnBnClickedUserCreategroupCancel();
};
