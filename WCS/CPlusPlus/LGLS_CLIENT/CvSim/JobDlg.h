#if !defined(AFX_JOBDLG_H__C6E1F470_07A7_4A3B_889B_759A42ADE31F__INCLUDED_)
#define AFX_JOBDLG_H__C6E1F470_07A7_4A3B_889B_759A42ADE31F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JobDlg.h : header file
//
#include "Trackinfo.h"
/////////////////////////////////////////////////////////////////////////////
// CJobDlg dialog

class CDestinationArray;
class CEcsDoc;
class CJobItem;
class CJobDlg : public CDialog
{
// Construction
public:
	CJobDlg(CEcsDoc* pDoc, CTrackInfo* pTrack, CWnd* pParent = NULL);   // standard constructor
	enum EN_KIND { enKindAll, enKindNew, enKindProgress, enKindComplete, enKindError, enKindSchedule, enKindResult };
	enum EN_STN_KIND { enKindArvStation, enKindStoStation, enKindRetStation, enKindCraneStation };

protected:
	CEcsDoc* m_pDoc;

protected:
	void UpdateList();
	CJobItem* GetSelectJobItem();

public:
// Dialog Data
	//{{AFX_DATA(CJobDlg)
	enum { IDD = IDD_JOB };
	CComboBox	m_cbxDestPlc;
	BUTTON_TYPE	m_btnOK;
	BUTTON_TYPE	m_btnAdd;
	BUTTON_TYPE	m_btnAdd2;
	CListCtrl	m_lstJobList;
	BOOL		m_bAutoRefresh;
	BOOL		m_bDestPos;
	BOOL		m_bNextCv;
	BOOL		m_bNextCv2;
	BOOL		m_bCenter;
	BOOL		m_bCenterNext;
	BOOL		m_bCenterDest;
	BOOL		m_bCross;
	BOOL		m_bCrossTrack;
	BOOL		m_bCrossDest;
	CComboBox	m_cbxDestFloor;
	BOOL		m_bTwinCheck;
	//}}AFX_DATA

//	CDestinationArray* m_pDestinations;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJobDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJobDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnBtnAddOfflineJob();
	afx_msg void OnBtnComplete();
	afx_msg void OnBtnArrive();
	afx_msg void OnBtnCancel();
	afx_msg void OnDblclkListJob(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCbxFilterStartStation();
	afx_msg void OnSelchangeCbxFilterDestStation();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnCopy();
	afx_msg void OnCheckDestPos();
	afx_msg void OnCheckNextCv();
	afx_msg void OnCheckNextCv2();
	afx_msg void OnBtnAdd2();
	afx_msg void OnBtnSave();
	afx_msg void OnCheckCenterNext();
	afx_msg void OnCheckCenterDest();
	afx_msg void OnCheckCrossTrack();
	afx_msg void OnCheckCenter();
	afx_msg void OnCheckCross();
	afx_msg void OnBtnRetry();
	afx_msg void OnBtnAdd3();
	afx_msg void OnEditchangeComboDestFloor();
	afx_msg void OnCheckCrossDest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CTrackInfo* m_pTrack;
	afx_msg void OnNMClickListJob(NMHDR *pNMHDR, LRESULT *pResult);
	int m_itemindex;
	BOOL m_bUseDT;			//DestTable 사용유무;
	afx_msg void OnLvnKeydownListJob(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnRetry2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOBDLG_H__C6E1F470_07A7_4A3B_889B_759A42ADE31F__INCLUDED_)
