#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"

// CManualLogin 대화 상자입니다.

class CManualLogin : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CManualLogin)

public:
	CManualLogin(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CManualLogin(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent = NULL);   // 표준 생성자입니다
	virtual ~CManualLogin();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSkinButton m_btnManualLoginOk;
	CEdit m_edtManualLoginPw;

	afx_msg void OnBnClickedBtnManualLoginOk();

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;
	 
	int m_nLang;

public:
	HICON m_hIcon;
	BOOL m_bInitialized;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CString GetQry_Main();
	void RedrawImage();
};
