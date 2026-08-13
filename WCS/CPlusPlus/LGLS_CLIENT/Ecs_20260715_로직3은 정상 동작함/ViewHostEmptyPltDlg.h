#pragma once
#include "Resource.h"

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "URMDBAccess.h"

//#include "fpspread1.h"
//#include "SpreadSheet.h"
#include "StaticTransparent.h"

#include "TGroupBox.h"

#include "FontManagerDialog.h"
#include "ComboBoxWrapper.h"

// CViewHostEmptyPltDlg 대화 상자입니다.

class CViewHostEmptyPltDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CViewHostEmptyPltDlg)

public:
	CViewHostEmptyPltDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CViewHostEmptyPltDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewHostEmptyPltDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEW_HOST_EMPTY_PLT };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	int m_nLang;
	int m_nActiveRow;

public:
	//CFpspread1 m_pSpreadMain;
	CSpreadSheet m_SpreadSheet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSkinButton m_btnHostEmptyPltDelete;
	CSkinButton m_btnHostEmptyPltRequest;
	CSkinButton m_btnHostEmptyPltSearch;
	CComboBoxWrapper m_cbxHostEmptyPltKind;
	CComboBoxWrapper m_cbxHostEmptyPltStatus;
	CComboBoxWrapper m_cbxHostEmptyPltStn;
	CComboBoxWrapper m_cbxHostEmptyPltWhTyp;
	CComboBoxWrapper m_cbxRowCnt;
	CDateTimeCtrl m_dtpFrom;
	CDateTimeCtrl m_dtpFromTime;
	CDateTimeCtrl m_dtpTo;
	CDateTimeCtrl m_dtpToTime;
	CEdit m_edtHostEmptyPltLuggNo;
	CStaticTransparent m_lblHostEmptyPltCnt;
	CTGroupBox m_grpHostEmpty;
	CTGroupBox m_grpHostEmpty2;



public:
	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();

public:
	void InitializeSpread(int nRowCheck, BOOL bSearch);
	int SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetColumnText(int nColIdx, int nRowIdx, CString strColumnName);
	void SetMaxRows(int pRowCnt);
	int SetSpeadData(int nRowCheck, BOOL bSearch);
	void SetColWidth(int pColCnt, int pColSize);
	void SetMaxCols(int pMaxCol);
	void RedrawImage();
	CString GetQrySelect_Main(int nRowCheck, BOOL bSearch);
	CStatic m_lblHostEmptyPltFromDate;
	CStatic m_lblHostEmptyPltKind;
	CStatic m_lblHostEmptyPltLuggNo;
	CStatic m_lblHostEmptyPltResult;
	CStatic m_lblHostEmptyPltStatus;
	CStatic m_lblHostEmptyPltStn;
	CStatic m_lblHostEmptyPltToDate;
	CStatic m_lblHostEmptyPltWhTyp;

	CString m_strSTN;
	CString m_strKIND;
	CString m_strSTATUS;

	afx_msg void OnBnClickedBtnHostEmptyPltSearch();
	afx_msg void OnBnClickedBtnHostEmptyPltDelete();
	afx_msg void OnBnClickedBtnHostEmptyPltRequest();
	LRESULT OnSpreadLClick(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
