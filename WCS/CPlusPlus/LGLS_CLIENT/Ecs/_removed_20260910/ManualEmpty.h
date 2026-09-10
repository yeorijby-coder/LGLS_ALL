#pragma once
#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"


// CManualEmpty 대화 상자입니다.

class CManualEmpty : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CManualEmpty)

public:
	CManualEmpty(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CManualEmpty(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent = NULL);   // 표준 생성자입니다
	virtual ~CManualEmpty();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_EMPTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;
	 
	int m_nLang;

public:
	HICON m_hIcon;
	BOOL m_bInitialized;

public:
	CStaticTransparent m_lblManualEmptyWhTyp;
	CStaticTransparent m_lblManualEmptyKind;
	CStaticTransparent m_lblManualEmptyStartPos;
	CStaticTransparent m_lblManualEmptyDestPos;
	CComboBoxWrapper m_cbxManualEmptyWhTyp;
	CTGroupBox m_grpManualEmptySearch;
	CComboBoxWrapper m_cbxManualEmptyKind;
	CComboBoxWrapper m_cbxManualEmptyStartPos;
	CComboBoxWrapper m_cbxManualEmptyDestPos;
	CSkinButton m_btnManualEmptyClear;
	CSkinButton m_btnManualEmptyInsert;

public:
	void RelocationControls();
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnManualEmptyClear();
	void DataClear();
	afx_msg void OnBnClickedBtnManualEmptyInsert();
	afx_msg void OnCbnSelchangeComboManualEmptyKind();
};
