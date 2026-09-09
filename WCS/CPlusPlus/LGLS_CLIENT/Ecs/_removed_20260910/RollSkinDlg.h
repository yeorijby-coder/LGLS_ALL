#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"

// CRollSkinDlg 대화 상자입니다.

class CRollSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CRollSkinDlg)

public:
	CRollSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CRollSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRollSkinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKIN_ROLL_CTRL };

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
	CSkinButton m_btnClampforwardErr;
	CSkinButton m_btnCrushPaperSensor;
	CSkinButton m_btnDrivBackwardErr;
	CSkinButton m_btnDrivBackwardPos;
	CSkinButton m_btnDrivForwardErr;
	CSkinButton m_btnDrivForwardPos;
	CSkinButton m_btnElevAscErr;
	CSkinButton m_btnElevDescErr;
	CSkinButton m_btnPaperBlockSensor1;
	CSkinButton m_btnPaperBlockSensor2;
	CSkinButton m_btnPaperBlockSensor3;
	CSkinButton m_btnPaperBlockSensor4;
	CSkinButton m_btnPaperFullSensor;
	CButton m_chkAutoSel;

	CEdit m_edtRollClampPosDataBackward;
	CEdit m_edtRollClampPosDataForward;
	CEdit m_edtRollElevPosDataAsc;
	CEdit m_edtRollElevPosDataPickup;
	CEdit m_edtRollElevPosDataAscRolling;
	CEdit m_edtRollNo;
	CEdit m_edtRollSensorDataHeight;

	CStaticTransparent m_lblRollClampPosDataBackward;
	CStaticTransparent m_lblRollClampPosDataForward;
	CStaticTransparent m_lblRollElevPosDataAsc;
	CStaticTransparent m_lblRollElevPosDataPickup;
	CStaticTransparent m_lblRollElevPosDataRolling;
	CStaticTransparent m_lblRollSensorDataHeight;

	CTGroupBox m_grpRollPosData;
	CTGroupBox m_grpRollPosItem;
	CTGroupBox m_grpRollPosValue;
	CTGroupBox m_grpRollStatus;
	CTGroupBox m_grpTransStatus;

	
	afx_msg void OnBnClickedChkAutoSel();
	BOOL m_blAutoSel;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
