#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"

// CRevSkinDlg 대화 상자입니다.

class CRevSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CRevSkinDlg)

public:
	CRevSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CRevSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRevSkinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKIN_REV_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	LRESULT OnMessageSwitch(WPARAM wParam, LPARAM lParam);

public:
	CTrackInfo* m_pTrackInfo;
	CEcsDoc* m_pDoc;
	EN_LANG m_nLang;

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	afx_msg void OnClose();
	afx_msg void OnEnterSizeMove();
	afx_msg void OnExitSizeMove();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	virtual BOOL OnInitDialog();

	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(EN_LANG pnLANG);
	void InvalidateTrackData(EN_LANG pLang);
	void InvalidateLangControl( EN_LANG enLangTemp );

	CSkinButton m_btnClampBackwardErr;
	CSkinButton m_btnClampForwardErr;
	CSkinButton m_btnClampForwardSensor;
	CSkinButton m_btnDrivBackwardErr;
	CSkinButton m_btnDrivForwardErr;
	CSkinButton m_btnElevAscErr;
	CSkinButton m_btnElevDescErr; 

	CEdit m_edtRevDrivPosDataPickup;
	CEdit m_edtRevDrivPosDataReverse;
	CEdit m_EdtRevElevPosData1;
	CEdit m_edtElevPosData2;
	CEdit m_edtRevElevPosDataAsc;
	CEdit m_edtRevNo;
	CEdit m_edtRevSensorDataHeight;

	CTGroupBox m_grpRevPosData;
	CTGroupBox m_grpRevPosValue;
	CTGroupBox m_grpRevPosItem;
	CTGroupBox m_grpRevStatus;

	CStaticTransparent m_lblRevDrivPosDataPickup;
	CStaticTransparent m_lblRevDrivPosDatareverse;
	CStaticTransparent m_lblRevElevPosData1;
	CStaticTransparent m_lblRevElevPosData2;
	CStaticTransparent m_lblRevElevPosDataAsc;
	CStaticTransparent m_lblRevSensorDataHeight;
	
	CButton m_chkAutoSel;
	afx_msg void OnBnClickedChkAutoSel();
	BOOL m_blAutoSel;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CSkinButton m_btnClampBackwardSensor;
	CSkinButton m_btnDrivBackwardPos;
	CSkinButton m_btnDrivForwardPos;
};
