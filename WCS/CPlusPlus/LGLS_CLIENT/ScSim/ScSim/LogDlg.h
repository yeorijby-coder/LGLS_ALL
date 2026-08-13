#ifndef __LOGDLG_H__
#define __LOGDLG_H__

#include "DialogResize.h"

class CEcsDoc;

class CLogDlg : public CDialogResize
{
public:
	CLogDlg(CEcsDoc *pDoc, CLog *pLog, CWnd* pParent = NULL);

protected:
	CEcsDoc* m_pDoc;
	CLog* m_pLog;

	CSortContext Context;

protected:
	CStatusBar m_wndStatusBar;
    CToolBar m_wndToolBar;

public:
	void Loading();
	void PrintTitle(CDC* pDC, int nPageNum);
	void SetSxButton();

	void SetRange();

	void SetupOutlookBar();
	void SetStatusBarPane();

	BOOL CreateBarWindow();

public:
// Dialog Data
	//{{AFX_DATA(CLogDlg)
	enum { IDD = IDD_LOG };
	//}}AFX_DATA

	CEditEx     m_edtContext;
	CFooButton  m_btnSort;
	CReportCtrl m_ctlReport;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(CLogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboPosition();
	afx_msg void OnLogFile();
	afx_msg void OnLogExit();
	afx_msg void OnLogSaveas();
	afx_msg void OnClickListOutlook(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRefreshLog();
	afx_msg void OnKeydownListOutlook(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSort();
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_DLGRESIZE_MAP;
};

#endif
