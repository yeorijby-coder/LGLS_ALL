#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"
#include "EcsDoc.h"
#include "FontManagerDialog.h"

// CSystemLoginDlg 대화 상자입니다.

class CSystemLoginDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CSystemLoginDlg)

public:
	CSystemLoginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CSystemLoginDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSystemLoginDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SYSTEM_LOGIN };

public:
	CEcsDoc* m_pDoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	CMap<CString, LPCTSTR, CPermission*, CPermission*> m_pUserInfo;
	//폼명				권한

	CSkinButton m_btnOk;
	CSkinButton m_btnLock;
	CSkinButton m_btnCancel;
	CEdit m_edtId;
	CEdit m_edtPw;

	HICON m_hIcon;
	BOOL m_bInitialized;
	BOOL m_blLogYn;

	void RelocationControls();

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	void OnBnClickedOkSU();
	afx_msg void OnBnClickedCancel();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedLock();
	//void InitilizeNationFont( int pnLang );

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	bool InitializeUser();
	
};
