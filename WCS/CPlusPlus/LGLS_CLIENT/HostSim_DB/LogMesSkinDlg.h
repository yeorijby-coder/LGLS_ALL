#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "NtnChart.h"
// CLogMesSkinDlg 대화 상자입니다.

class CLogMesSkinDlg : public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CLogMesSkinDlg)

public:
	CLogMesSkinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CLogMesSkinDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogMesSkinDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG_MES };

public:
	CURMDBAccess* m_pDB;
	CEcsDoc* m_pDoc;

public:
	int m_nActiveRow;

public:
	EN_LANG m_nLang;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
	void InitializeResource(EN_LANG pnLANG);

public:
	CString GetQrySelect();

public:
	CSkinButton m_btnOk;
	CSkinButton m_btnCancel;
	CSkinButton m_btnSearch;

	CStaticTransparent m_lblWhTyp;
	CStaticTransparent m_lblLuggNo;
	CStaticTransparent m_lblToDate;
	CStaticTransparent m_lblFromDate;


	CEdit m_edtTrayBottom;
	CEdit m_edtTrayTop;
	CEdit m_edtLuggNo;
	CEdit m_edtRequestId;
	CEdit m_edtFromTrack;
	CEdit m_edtToTrack;

	CStaticTransparent m_lblMESMessage;
	CEdit			   m_edtMESMessage;
	CStaticTransparent m_lblMESMessage2;
	CEdit			   m_edtMESMessage2;
	CStaticTransparent m_lblMESMessage3;
	CEdit			   m_edtMESMessage3;




	CEdit              m_edtMESDetailLog;

	CStaticTransparent m_lblMESLogWhTyp;
	CComboBoxWrapper m_cbxMESLogWhTyp;

	CComboBoxWrapper m_cbxMesLogDirection;
	CComboBoxWrapper m_cbxMesLogHostCmd;
	CStaticTransparent m_lblMesLogDirection;
	CStaticTransparent m_lblMesLogHostCmd;

	CDateTimeCtrl m_dtpFrom;
	CDateTimeCtrl m_dtpFromTime;
	CDateTimeCtrl m_dtpTo;
	CDateTimeCtrl m_dtpToTime;
	

	CTGroupBox m_grpMesLogSearch;
	CTGroupBox m_grpMesLogSearch2;
	CTGroupBox m_grpMesLogMessage;

	//void GetMESLogSelect();


	CTGroupBox m_grpMesLogMessage2;
	CEdit	   m_edtMESDetailLog2;

	



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	HICON m_hIcon;
	BOOL m_bInitialized;
	void RelocationControls();

	CString GetQrySelect(int nLANG, CString strWH_TYP, CString strLUGG_NO, CString strMessage, CString strMESMessage2, CString strMESMessage3, CString strdtTo);



	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);




	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnLogMesSearch();
	afx_msg void OnBnClickedSpread();

	//void FillSpreadColumn(CStringArray& strCols);	
	void FillSpreadColumn(int nColIdx, CString strColumnName);
	void FillSpreadRow(int nColIdx, int nRowIdx, CString strValue); //SK
	afx_msg void OnSpdLogIo();
	afx_msg void OnUpdateSpdLogIo(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetfocusCmbRowCnt();
	DECLARE_EVENTSINK_MAP()
	void ClickSpread(long Col, long Row);




	//스프레드 값 채우기 함수
public:
		//CFpspread1 m_pSpreadMain;
		CSpreadSheet m_SpreadSheet;
		CStaticTransparent m_lblSpdMainResult;//SK
		CStaticTransparent m_lblSpdMainCnt;

protected:

	void InitializeSpread(int nRowCheck, BOOL bSearch);
	int SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql);
	void SetColumnText(int nColIdx, int nRowIdx, CString strColumnName);
	void SetMaxRows(int pRowCnt);
	int SetSpeadData(int nRowCheck, BOOL bSearch);
	void SetColWidth(int pColCnt, int pColSize);
	void SetMaxCols(int pMaxCol);
	CString GetQrySelect_Main(int nRowCheck, BOOL bSearch);

	//ROW 수 조절
protected:
	CSkinButton m_btnPre;
	CSkinButton m_btnNext;
	CComboBoxWrapper m_cbxRowCnt;
	int m_pPreRowCnt;

	void OnBnClickedPreRow();
	void OnBnClickedNextRow();

};
