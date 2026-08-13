#pragma once

#include "SkinDialog.h"
#include "SkinButton.h"
#include "afxwin.h"
#include "EcsDoc.h"
#include "StaticTransparent.h"
#include "FontManagerDialog.h"
#include "TGroupBox.h"
#include "FireParms.h"

class CFireMessageDlg :	public CSkinDialog, CFontManagerDialog
{
	DECLARE_DYNAMIC(CFireMessageDlg)


public:
	enum { IDD = IDD_FIRE_MESSAGE };


public:
	CFireMessageDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CFireMessageDlg(CEcsDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	~CFireMessageDlg();

public:
	CEcsDoc* m_pDoc;

public:
	int m_nLang;

public:
	CEdit m_edtMessage;
	CEdit m_edtRemarks;
	CStaticTransparent m_lblRemark;
	CSkinButton m_btnStop;
	CFpspread1 m_spd;

public:
	CRect m_rect;

private:
	BOOL m_bFlag;//ERASE BACKGEND FLAG  R T R T
	BOOL m_bBeep;

public:
	void ShowDlg();
	void HideDlg();

public:
	void BeepOff();
	void BeepOn(CString pstrControlId);
	
public:
	CMap<CString, LPCTSTR, CFireParms*, CFireParms*>* m_mapViewKey;
	CMap<CString, LPCTSTR, CFireParms*, CFireParms*>* m_mapView;

public:
	void KeySwap();

public:
	LRESULT OnMessageStopStructure(WPARAM wParam, LPARAM lParam);

public:
	void RenameResource( EN_LANG m_enLang = EN_ENG );
	void RedrawImage();
public:
	DECLARE_MESSAGE_MAP()
	void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnStop();

public:
	void GetCellNo();
	void FillSpreadRow(int nColIdx, int nRowIdx, CString strValue);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnExitSizeMove();
	CString GetColumnText( CString pstrColName , int pnLang);
	CString GetIniText( CString pstrKey, int pnLang );
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ChangeFont();
	//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

