#pragma once
#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"

// CManualSc 대화 상자입니다.

class CManualSc : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CManualSc)

public:
	CManualSc(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CManualSc(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent = NULL);   // 표준 생성자입니다
	virtual ~CManualSc();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANAUL_SC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

public:

	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;
	

	EN_LANG m_nLang;

	CString m_strScNo;
	CString m_strScFork;
	
public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(int pnTabIndex);

public:
	CStaticTransparent m_lblScManualScGrpNo;
	CStaticTransparent m_lblScManualScNo;
	CStaticTransparent m_lblScManaulJobTyp;
	CStaticTransparent m_lblScManaulStartHsFork1;
	CStaticTransparent m_lblScManaulDestHsFork1;
	CStaticTransparent m_lblScManaulStartPos;
	CStaticTransparent m_lblScManaulStartPos2;

	CComboBoxWrapper m_cbxScManualScGrpNo;
	CComboBoxWrapper m_cbxScManualScNo;
	CComboBoxWrapper m_cbxScManualJobTyp;
	CComboBoxWrapper m_cbxStartPosFork1;
	CComboBoxWrapper m_cbxDestPosFork1;
	CComboBoxWrapper m_pAging;

	CSkinButton m_btnScManualSave;

	CTGroupBox m_grpScInfo;
	CTGroupBox m_grpScHsInfo;
	CTGroupBox m_grpScManaulFork1;

	CMFCMaskedEdit m_edtManualScStartPosFk1;

	CMFCMaskedEdit m_edtManualScDestPosFk1;

public:
	BOOL IsInvalidStoData();
	BOOL IsInvalidRetData();
	BOOL IsInvalidRetToRetData();
	BOOL IsInvalidRackToRackData();

	BOOL SelectScStatus();
	BOOL UpdateScData();


	afx_msg void OnCbnSelchangeCmbScManualScNo();
	afx_msg void OnBnClickedBtnScManualSave();
	afx_msg void OnCbnSelchangeCmbScManualScGrpNo();
	afx_msg void OnCbnSelchangeCmbScManualJobTyp();
	afx_msg void OnClose();
};
