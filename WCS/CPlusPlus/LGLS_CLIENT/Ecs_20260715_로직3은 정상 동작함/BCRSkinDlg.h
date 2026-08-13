#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"

// CBCRSkinDlg 대화 상자입니다.

class CBCRSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CBCRSkinDlg)

public:
	CBCRSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CBCRSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBCRSkinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKIN_BCR_CRTL };

public:
	CEcsDoc* m_pDoc;	
	EN_LANG m_nLang;

public:
	CCV_DATA* m_pCV_DATA;
	CBCR_MST* m_pBCR_MST;

protected:
	HICON m_hIcon;
	BOOL m_bInitialized;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	void RelocationControls();
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(EN_LANG m_enLang);

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

public:	
	LRESULT OnMessageSwitch(WPARAM wParam, LPARAM lParam);
	void InvalidateBcrData(EN_LANG pLang);
	void InvalidateLangControl( EN_LANG enLangTemp );
public:
	BOOL IsValidataEditData(CString pBcrTop, CString pBcrBottom);

public:
	//label
	CStaticTransparent m_lblBcrLevel;

	CStaticTransparent m_lblBcrBottom;
	CStaticTransparent m_lblBcrTop;
	CStaticTransparent m_lblBcrSta;

	CStaticTransparent m_lblBcrTrackNo;
	CStaticTransparent m_lblBcrSuspend;

	//

	//edit
	CEdit m_edtBcrNo;
	CEdit m_edtBcrLevel;

	CEdit m_edtBcrBottom;
	CEdit m_edtBcrTop;
	CEdit m_edtBcrSta;

	CEdit m_edtBcrTrackNo;
	CEdit m_edtBcrNoreadCnt;
	CEdit m_edtBcrSuspend;
	//

	//button
	CSkinButton m_btnBcrReadBarcode;
	CSkinButton m_btnBcrWriteManual;
	CSkinButton m_btnBcrSuspend;
	CSkinButton m_btnBcrMesReport;
	CSkinButton m_btnBcrActive;
	//
	CTGroupBox m_grpBcrCommand;
	CTGroupBox m_grpBcrReport;
	CTGroupBox m_grpBcrReportItem;
	CTGroupBox m_grpBcrReportValue;
	CTGroupBox m_grpBcrTrackStatus;
	CTGroupBox m_grpBcrTrackStatusItem;
	CTGroupBox m_grpBcrTrackStatusValue;
	CTGroupBox m_grpBcrErrorInfomation;
	CTGroupBox m_grpBcrSuspend;
	//


	afx_msg void OnBnClickedBtnBcrReadBarcode();
	afx_msg void OnBnClickedBtnBcrWriteManual();
	afx_msg void OnBnClickedBtnBcrSuspend();
	afx_msg void OnBnClickedBtnBcrMesReport();
	afx_msg void OnBnClickedBtnActive();
	CString GetQrySelectBCR_STATUS_CCD( CBCR_MST* pBCR_MST );
};
