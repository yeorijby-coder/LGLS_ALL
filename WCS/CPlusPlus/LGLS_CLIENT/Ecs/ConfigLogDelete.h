#pragma once
#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "NtnChart.h"

// CConfigLogDelete 대화 상자입니다.

class CConfigLogDelete : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CConfigLogDelete)

public:
	CConfigLogDelete(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CConfigLogDelete(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfigLogDelete();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONFIG_LOG_DELETE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CEdit m_edtConfigLogDeleteDay;
	CTGroupBox m_grpConfigLogDelete;
	CStaticTransparent m_lblConfigLogDeleteDay;
	CSkinButton m_btnConfigLogDeleteUpdate;
	int m_nActiveRow;

	CString m_strTABLE_NAME;
	CString m_strGrpCaptionBase;   // [LGLS 2026-09-03] 그룹 제목 원문(선택 로그명 표시용)

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	EN_LANG m_nLang;

	HICON m_hIcon;
	BOOL m_bInitialized;
	//CFpspread1 m_pSpreadMain;
	CSpreadSheet m_SpreadSheet;
	void RedrawImage();
	void InitializeResource(EN_LANG pnLANG);
	void RelocationControls();
	void InitializeSpread(int nRowCheck, BOOL bSearch);
	int SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetColumnText(int nColIdx, int nRowIdx, CString strColumnName);
	void SetMaxRows(int pRowCnt);
	int SetSpeadData(int nRowCheck, BOOL bSearch);
	void SetColWidth(int pColCnt, int pColSize);
	void SetMaxCols(int pMaxCol);
	CString GetQrySelect_Main(int nRowCheck, BOOL bSearch);
	
	afx_msg void OnBnClickedBtnConfigLogDeleteUpdate();
	LRESULT OnSpreadLClick(WPARAM wParam, LPARAM lParam);
};
