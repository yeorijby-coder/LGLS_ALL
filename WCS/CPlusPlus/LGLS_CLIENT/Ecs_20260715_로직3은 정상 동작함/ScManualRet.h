#pragma once
#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "resource.h"
#include "Lang.h"
#include "ComboBoxWrapper.h"

// CScManualRet 대화 상자입니다.

class CScManualRet : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CScManualRet)

public:
	CScManualRet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CScManualRet(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다
	virtual ~CScManualRet();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SC_MANUAL_RET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	void RelocationControls();

	DECLARE_MESSAGE_MAP()

public:
	HICON m_hIcon;
	BOOL m_bInitialized;

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;
	EN_LANG m_nLang;

	CSC_DATA* m_pSC_DATA;

	CString m_strWhTyp;
	CString m_strPlcNo;
	CString m_strScNo;

	CString m_strRadioButton;

public:
	void RenameResource(EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(int pnTabIndex);

public:
	int m_radStoHs;
	CSkinButton m_btnScManualRet;
	CTGroupBox m_grpPosition;
	CComboBoxWrapper m_cbxDestPos;
	CStaticTransparent m_lblDestPos;

	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnScManualRet();
	afx_msg void OnRadioButton(UINT value);

	BOOL StoHsDataDelete(CString& pLuggNo, CString& pPulpSensorRd, CString& pHsMcNo);
	BOOL ScDataDelete();
	BOOL JobMstDelete(CString pLuggNo, CString& strProductSize);
	BOOL JobMstInsert(CString pHsMcNo, CString& pLuggNo, CString& strProductSize, CString strDestPos);
	BOOL CvDataUpdate(CString pMcNo, CString pLuggNo, CString pProductSize, CString strDestPos);

	BOOL RetHsDataSelect(CString& strHS_MC_NO, CString& strSensor0DataRd);
	BOOL JobMstUpdate(CString pLuggNo, CString& strProductSize);
};
