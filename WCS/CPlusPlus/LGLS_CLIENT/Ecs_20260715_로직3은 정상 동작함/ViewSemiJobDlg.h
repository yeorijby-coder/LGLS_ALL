#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"
#include "EcsDoc.h"

#include "fpspread1.h"
//#include "SpreadSheet.h"

// CViewSemiJobDlg 대화 상자입니다.

class CViewSemiJobDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CViewSemiJobDlg)

public:
	CViewSemiJobDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CViewSemiJobDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewSemiJobDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEW_SEMIJOB };


public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	CFpspread1 m_pSpdSemiJob;

	DECLARE_MESSAGE_MAP()


public:
	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedViewSemijobOk();
	afx_msg void OnBnClickedViewSemijobCancel();

	void InitializeUSER_MST();
	void FillSpreadColumn(int nColIdx, CString strColumnName);
	void FillSpreadRow(int nColIdx, int nRowIdx, CString strValue);
	int GetListSEMI_JOB(CStringList &strListUSER_GRP);
};
