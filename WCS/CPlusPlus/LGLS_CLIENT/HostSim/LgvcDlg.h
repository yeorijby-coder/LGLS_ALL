#if !defined(AFX_LGVCDLG_H__536586CA_CF19_49CE_9003_C632DEAA0D62__INCLUDED_)
#define AFX_LGVCDLG_H__536586CA_CF19_49CE_9003_C632DEAA0D62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LgvcDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLgvcDlg dialog

class CLgv;
class CLgvcDlg : public CDialog
{
// Construction
public:
	CLgvcDlg(CWnd* pParent = NULL);								// standard constructor
	CLgvcDlg(CLgv* pLgv, int nLgvNum, CWnd* pParent = NULL);   // standard constructor

public:
	CLgv* m_pLgv;
	int m_nSelectLgv;
// Dialog Data
	//{{AFX_DATA(CLgvcDlg)
	enum { IDD = IDD_LGVC_CTRL };
//	CListCtrl	m_lstJob;
	BUTTON_TYPE	m_btnRgvStop;
	BUTTON_TYPE	m_btnTRR;
	BUTTON_TYPE	m_btnAGQ;
	BUTTON_TYPE	m_btnOK;
	BOOL	m_bSuspend;
	CButtonST	m_btnDoor1Open;
	CButtonST	m_btnDoor1Close;
	CButtonST	m_btnDoor1OpenAccept;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLgvcDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLgvcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRequestTransport();
	afx_msg void OnButtonRequestAgvStatus();
	afx_msg void OnButtonRequestSuspend();
	afx_msg void OnCheckSuspend();
//	afx_msg void OnButtonDoor1Open();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LGVCDLG_H__536586CA_CF19_49CE_9003_C632DEAA0D62__INCLUDED_)
