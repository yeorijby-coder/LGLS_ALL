#if !defined(AFX_LGVDLG_H__A87EC669_97CA_43A6_BFBF_C712F4154BB9__INCLUDED_)
#define AFX_LGVDLG_H__A87EC669_97CA_43A6_BFBF_C712F4154BB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LgvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLgvDlg dialog

class CLgv;
class CLgvDlg : public CDialog
{
// Construction
public:
	CLgvDlg(CWnd* pParent = NULL);								// standard constructor
	CLgvDlg(CLgv* pLgv, int nCarNum, CWnd* pParent = NULL);   

//protected:
public:
	CLgv* m_pLgv;
	int m_nSelectCar;

protected:
	void UpdateList();
	void ReportDoorStatus(int nDoorID, BOOL bOpenStatus);

public:
// Dialog Data
	//{{AFX_DATA(CLgvDlg)
	enum { IDD = IDD_LGV_CTRL1 };
	CListCtrl	m_lstJob;
	CTabCtrl	m_tabAgv;
	BUTTON_TYPE	m_btnReset;
	BUTTON_TYPE	m_btnRgvStop;
	BUTTON_TYPE	m_btnTRR;
	BUTTON_TYPE	m_btnDEL;
	BUTTON_TYPE	m_btnTRQ;
	BUTTON_TYPE	m_btnAGQ;
	BUTTON_TYPE	m_btnOK;
	BOOL	m_bSuspend;
	BUTTON_TYPE	m_btnDoor1Report;
	CButtonST	m_btnDoor2Open;
	CButtonST	m_btnDoor2Close;
	BUTTON_TYPE	m_btnDoor2Report;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLgvDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLgvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRequestReset();
	afx_msg void OnButtonRequestTransport();
	afx_msg void OnButtonRequestDelete();
	afx_msg void OnButtonRequestStatus();
	afx_msg void OnButtonRequestAgvStatus();
	afx_msg void OnButtonDoor1Report();
	afx_msg void OnButtonDoor2Report();
	afx_msg void OnCheckSuspend();
	afx_msg void OnButtonDoor1Open();
	afx_msg void OnButtonDoor2Open();
	afx_msg void OnButtonDoor1Close();
	afx_msg void OnButtonDoor2Close();
	afx_msg void OnButtonRequestSuspend();
//	afx_msg void OnButtonDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LGVDLG_H__A87EC669_97CA_43A6_BFBF_C712F4154BB9__INCLUDED_)
