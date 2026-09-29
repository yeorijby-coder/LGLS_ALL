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

	// [LGLS 2026-09-29] 항목마다 [숨김] 체크 - 체크하면 메인 범례 판넬에서 뺀다 (사용자 지시).
	//   순서는 Config 의 g_szLegendHideKey / m_bLEGEND_HIDE 와 같다.
	CButton	m_chkHide[12];
	static const UINT	m_nChkHideId[12];

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
	CTGroupBox m_grpScStatus;
	CTGroupBox m_grpStatus;
	CStaticTransparent m_lblCvSto;
	CStaticTransparent m_lblCvRet;
	CStaticTransparent m_lblCvMove;

	CStaticTransparent m_lblCvStoReady;
	CStaticTransparent m_lblCvRetReady;
	CStaticTransparent m_lblCvSuspend;

	CStaticTransparent m_lblErr;
	CStaticTransparent m_lblManual;
	CStaticTransparent m_lblSearch;


	CListBox m_listCvSto;


	CXColorPickerXP m_btnAutoSto	;	
	CXColorPickerXP m_btnAutoRet	;	
	CXColorPickerXP m_btnAutoMove	;	

	CXColorPickerXP m_btnStnSto		;
	CXColorPickerXP m_btnStnRet		;
	CXColorPickerXP m_btnSuspend	;

	CXColorPickerXP m_btnErr		;	
	CXColorPickerXP m_btnManual		;	
	CXColorPickerXP m_btnCvSearch	;
	
	// [LGLS 2026-07-19] 반자동 작업 색상
	CTGroupBox m_grpSemiStatus;
	CStaticTransparent m_lblSemiSto;
	CStaticTransparent m_lblSemiRet;
	CStaticTransparent m_lblSemiMove;
	CXColorPickerXP m_btnSemiSto;
	CXColorPickerXP m_btnSemiRet;
	CXColorPickerXP m_btnSemiMove;
	//CSkinButton
	
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();

public :
	void LoadColor();
	CComboBoxWrapper m_cbxCvSto;
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedInitColor();
};
