#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"
#include "EcsDoc.h"
#include "FontManagerDialog.h"
// CSystemConfigDlg 대화 상자입니다.

class CSystemConfigDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CSystemConfigDlg)

public:
	CSystemConfigDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CSystemConfigDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSystemConfigDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SYSTEM_CONFIG };

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
	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedSystemConfigOk();
	afx_msg void OnBnClickedSystemConfigCancel();

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
};
