#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"

// CWcSkinDlg 대화 상자입니다.

class CWcSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CWcSkinDlg)

public:
	CWcSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CWcSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWcSkinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKIN_WC_CTRL };

	enum {	EN_BtnPass = 1, 
		    EN_BtnRead = 2, 
			EN_BtnWrite = 3
		  };

public:
	CEcsDoc* m_pDoc;
	CURMDBAccess* m_pDB;
	CWc* m_Wc;
	CWC_DATA * m_pWC_DATA;

public:
	EN_LANG m_nLang;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	virtual BOOL OnInitDialog();

	

	DECLARE_MESSAGE_MAP()
public:

	CStaticTransparent m_lblWcRcvVal;
	CStaticTransparent m_lblWcReadSta;
	CStaticTransparent m_lblWcWriteVal;
	CStaticTransparent m_lblWcByPass;

	CSkinButton m_btnWcActive;
	CSkinButton m_btnWcPass;
	CSkinButton m_btnWcRead;
	CSkinButton m_btnWcSuspend;
	CSkinButton m_btnWcWrite;

	CEdit m_edtWcErrCode;
	CEdit m_edtWcNo;
	CEdit m_edtWcRcvVal;
	CEdit m_edtWcReadSta;
	CEdit m_edtWcSuspend;
	CMFCMaskedEdit m_edtWcWriteVal;
	CEdit m_edtWcByPass;

	CTGroupBox m_grpWcErrorInformation;
	CTGroupBox m_grpWcSuspend;
	CTGroupBox m_grpWcWcStatus;
	CTGroupBox m_grpWcStatusCommand;
	CTGroupBox m_grpWcStatusItem;
	CTGroupBox m_grpWcStatusValue;

	CButton m_chkWcByPass;
	CSkinButton m_btnWcWriteManual;

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(EN_LANG pnLANG);
	LRESULT OnMessageSwitch(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnClose();
	afx_msg void OnEnterSizeMove();
	afx_msg void OnExitSizeMove();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void InvalidateWcData(EN_LANG pLang);

	afx_msg void OnBnClickedBtnWcPass();
	afx_msg void OnBnClickedBtnWcRead();
	afx_msg void OnBnClickedBtnWcWrite();
	afx_msg void OnBnClickedBtnWcActive();
	afx_msg void OnBnClickedBtnWcSuspend();

	void UpdateWcSuspend(CString pSusTyp);
	void UpdateWeightRcvVal(CString pRcvVal, CString EventTyp);
	afx_msg void OnClickedChkWcBypass();
	afx_msg void OnBnClickedBtnWcWriteManual();
	void SelCommonCode(CString pCCD_CD, CString pCMD, CString& pCCD_NM_KOR);
};
