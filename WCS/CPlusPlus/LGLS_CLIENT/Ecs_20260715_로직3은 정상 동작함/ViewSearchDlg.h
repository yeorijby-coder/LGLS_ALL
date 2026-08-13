#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"
#include "EcsDoc.h"
#include "FontManagerDialog.h"

// CViewSearchDlg 대화 상자입니다.

class CViewSearchDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CViewSearchDlg)

public:
	CViewSearchDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CViewSearchDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewSearchDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEW_SEARCH };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;
	
public:
	int m_nLang;

public:
	CEdit m_edtSeach;
	CStaticTransparent m_lblSearchTyp;

	CTGroupBox m_grpSearchBox;

	CComboBoxWrapper m_cmbSearchTyp;

	CSkinButton m_btnSeach;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetSearchCv();
	DECLARE_MESSAGE_MAP()


public:
	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);


public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	CString GetTrackNo(CString pTemp, CString pChkName);
	void BindCombo(CComboBoxWrapper& cbx);
};
