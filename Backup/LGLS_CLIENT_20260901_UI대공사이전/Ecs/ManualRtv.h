#pragma once
#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"



// CManualRtv 대화 상자입니다.

class CManualRtv : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CManualRtv)

public:
	CManualRtv(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CManualRtv(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent = NULL);   // 표준 생성자입니다
	virtual ~CManualRtv();

	enum { IDD = IDD_MANAUL_RTV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedChkRtvFork1();
	afx_msg void OnBnClickedChkRtvFork2();
	afx_msg void OnBnClickedBtnRtvManualSave();
	afx_msg void OnBnClickedChkRtvFork12();

public:

	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

	EN_LANG m_nLang;

	CString m_strRtvFork;

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(int pnTabIndex);


public:
	CStaticTransparent m_lblRtvManaulRtvNo;
	CStaticTransparent m_lblRtvManaulDepFork1;
	CStaticTransparent m_lblRtvManaulDepFork2;
	CStaticTransparent m_lblRtvManaulArrFork1;
	CStaticTransparent m_lblRtvManaulArrFork2;

	CComboBoxWrapper m_cbxRtvNo;
	CComboBoxWrapper m_cbxDepFork1;
	CComboBoxWrapper m_cbxDepFork2;
	CComboBoxWrapper m_cbxArrFork1;
	CComboBoxWrapper m_cbxArrFork2;

	CButton m_btnFork1;
	CButton m_btnFork2;
	CButton m_btnFork1Fork2;

	CSkinButton m_btnRtvManualSave;

	CTGroupBox m_grpRtvInfo;
	CTGroupBox m_grpRtvManaulFork1;
	CTGroupBox m_grpRtvManaulFork2;
	CTGroupBox m_grpRtvManualCommand;
};
