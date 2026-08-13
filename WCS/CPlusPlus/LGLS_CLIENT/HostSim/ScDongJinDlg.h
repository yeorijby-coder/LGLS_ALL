#if !defined(AFX_SCDONGJINDLG_H__82EC4452_9354_4803_9006_1E5FD1A48415__INCLUDED_)
#define AFX_SCDONGJINDLG_H__82EC4452_9354_4803_9006_1E5FD1A48415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScDongJinDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScDongJinDlg dialog

class CScDongJin;
class CScInfo;
class CScDongJinDlg : public CDialog
{
// Construction
public:
	CScDongJinDlg(CWnd* pParent = NULL);									// standard constructor
	CScDongJinDlg(CScDongJin* pSc, int nEquipNum, CWnd* pParent = NULL);  

public:
	CScInfo* m_pScInfo;
	CScDongJin* m_pSc;
	int m_nEquipNum;

public:
// Dialog Data
	//{{AFX_DATA(CScDongJinDlg)
	enum { IDD = IDD_SC_CTRL1 };
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
	BOOL	m_bR2RSuspend;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScDongJinDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScDongJinDlg)
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
	afx_msg void OnCheckSuspendRacktorack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDONGJINDLG_H__82EC4452_9354_4803_9006_1E5FD1A48415__INCLUDED_)
