#pragma once
#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "XColorPickerXP.h"

// CConfigStatus 대화 상자입니다.

class CConfigStatus : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CConfigStatus)

public:
	CConfigStatus(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CConfigStatus(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다
	virtual ~CConfigStatus();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIG_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;
	 
	int m_nLang;

public:
	HICON m_hIcon;
	BOOL m_bInitialized;
	//CComboBoxWrapper m_btnScSto;
	CTGroupBox m_grpCvJobStatus;
	CTGroupBox m_grpCvStatus;
	CTGroupBox m_grpRailStatus;
	CTGroupBox m_grpScStatus;
	CTGroupBox m_grpStatus;
	CStaticTransparent m_lblCvSto;
	CStaticTransparent m_lblCvRet;
	CStaticTransparent m_lblCvMove;
	CStaticTransparent m_lblCvRtr;
	CStaticTransparent m_lblCvAta;
	CStaticTransparent m_lblCvWtw;

	CStaticTransparent m_lblCvStoReady;
	CStaticTransparent m_lblCvRetReady;
	CStaticTransparent m_lblCvStoHs;
	CStaticTransparent m_lblCvRetHs;
	CStaticTransparent m_lblCvSuspend;

	CStaticTransparent m_lblErr;
	CStaticTransparent m_lblManual;
	CStaticTransparent m_lblDisConnect;
	CStaticTransparent m_lblCvWcPass;
	CStaticTransparent m_lblSearch;

	CStaticTransparent m_lblRailSto;
	CStaticTransparent m_lblRailRet;
	CStaticTransparent m_lblRailStoret;
	CStaticTransparent m_lblRailErr;
	CStaticTransparent m_lblRailItnLugg;

	CListBox m_listCvSto;


	CXColorPickerXP m_btnAutoSto	;	
	CXColorPickerXP m_btnAutoRet	;	
	CXColorPickerXP m_btnAutoMove	;	
	CXColorPickerXP m_btnAutoA2A	;	
	CXColorPickerXP m_btnAutoW2W	;	
	CXColorPickerXP m_btnAutoR2R	;	

	CXColorPickerXP m_btnStnSto		;
	CXColorPickerXP m_btnStnRet		;
	CXColorPickerXP m_btnHsSto		;
	CXColorPickerXP m_btnHsRet		;
	CXColorPickerXP m_btnSuspend	;

	CXColorPickerXP m_btnErr		;	
	CXColorPickerXP m_btnManual		;	
	CXColorPickerXP m_btnDisConnect	;	
	CXColorPickerXP m_btnWcPass		;
	CXColorPickerXP m_btnCvSearch	;
	
	CXColorPickerXP m_btnScRailSto	;
	CXColorPickerXP m_btnScRailRet	;
	CXColorPickerXP m_btnScRailAll	;
	CXColorPickerXP m_btnScRailErr	;
	CXColorPickerXP m_btnScRailInvk	;
	// [LGLS 2026-07-19] 반자동 작업 색상
	CTGroupBox m_grpSemiStatus;
	CStaticTransparent m_lblSemiSto;
	CStaticTransparent m_lblSemiRet;
	CStaticTransparent m_lblSemiMove;
	CStaticTransparent m_lblSemiRtr;
	CStaticTransparent m_lblSemiAta;
	CXColorPickerXP m_btnSemiSto;
	CXColorPickerXP m_btnSemiRet;
	CXColorPickerXP m_btnSemiMove;
	CXColorPickerXP m_btnSemiRtr;
	CXColorPickerXP m_btnSemiAta;
	//CSkinButton
	
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();

public :
	void LoadColor();
	CComboBoxWrapper m_cbxCvSto;
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedInitColor();
};
