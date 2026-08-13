#if !defined(AFX_SCPAIRDLG_H__82EC4452_9354_4803_9006_1E5FD1A48415__INCLUDED_)
#define AFX_SCPAIRDLG_H__82EC4452_9354_4803_9006_1E5FD1A48415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScPairDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScPairDlg dialog

class CScPair;
class CScPairDlg : public CDialog
{
// Construction
public:
	CScPairDlg(CScPair* pSc, CWnd* pParent = NULL);   // standard constructor

protected:
	CScPair* m_pSc;
	
public:
// Dialog Data
	//{{AFX_DATA(CScPairDlg)
	enum { IDD = IDD_SC_PAIR_CTRL };
	BUTTON_TYPE	m_btnComplete;
	BUTTON_TYPE	m_btnEmergency;
	BUTTON_TYPE	m_btnActive;
	BUTTON_TYPE	m_btnStop;
	BUTTON_TYPE	m_btnReset;
	BUTTON_TYPE	m_btnDelete;
	BUTTON_TYPE	m_btnHome;
	BUTTON_TYPE	m_btnDualStore;
	BUTTON_TYPE	m_btnManual;
	BUTTON_TYPE	m_btnOK;
	BOOL	m_bRetSuspend;
	BOOL	m_bStoSuspend;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScPairDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScPairDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonComplete();
	afx_msg void OnButtonEmergency();
	afx_msg void OnButtonActive();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonErrorReset();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonCallToHome();
	afx_msg void OnButtonDualStore();
	afx_msg void OnButtonManual();
	afx_msg void OnCheckSuspentStore();
	afx_msg void OnCheckSuspendRetrieve();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCPAIRDLG_H__82EC4452_9354_4803_9006_1E5FD1A48415__INCLUDED_)
