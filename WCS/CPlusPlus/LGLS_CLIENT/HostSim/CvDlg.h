#if !defined(AFX_CVDLG_H__75FE0FD1_A729_4B8B_B91C_4A39E9AB4E06__INCLUDED_)
#define AFX_CVDLG_H__75FE0FD1_A729_4B8B_B91C_4A39E9AB4E06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCvDlg dialog

class CEcsDoc;
class CTrackInfo;
class CCvCmdMsg;
class CCvDlg : public CDialog
{
// Construction
public:
	CCvDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CEcsDoc* m_pDoc;
	CTrackInfo* m_pTrack;

public:
	int  m_nLuggNum;
	int  m_nJobType;
	int  m_nDestPos;
	BYTE m_ucGenCode;
	int  m_nSize;
	BOOL m_bClone;

protected:
	void SetComboBox(CComboBox& rComboBox, DWORD dwItemData);
	void SetJobType(int nJobType) { SetComboBox(m_cbxJobType, nJobType); }
	void SetDestTrack(int nDestTrack) { SetComboBox(m_cbxDestTrack, nDestTrack); }
	void SetGenCode(BYTE ucGenCode) { SetComboBox(m_cbxGenCode, ucGenCode); }
	void SetEtcValue();
	void WriteTrack(int nLuggNum, int nJobType, int nDestPos, BYTE ucGenCode, int nSize);
	void WriteWord(int nAddress, WORD wWord);
	BOOL UpdateTrackInfo(BOOL bSaveObject = TRUE);

public:
// Dialog Data
	//{{AFX_DATA(CCvDlg)
	enum { IDD = IDD_CV_CTRL1 };
	CButtonST	m_btnStatusStoStation;
	CButtonST	m_btnStatusScStoHS;
	CButtonST	m_btnStatusScRetHS;
	CButtonST	m_btnStatusRtvDepartHS;
	CButtonST	m_btnStatusRtvArriveHS;
	CButtonST	m_btnStatusRetStation;
	CButtonST	m_btnStatusLfDepartHS;
	CButtonST	m_btnStatusLfArriveHS;
	CButtonST	m_btnStatusLgvDepartHS;
	CButtonST	m_btnStatusLgvArriveHS;
	CButtonST	m_btnStatusLgvOutStatus;
	CButtonST	m_btnStatusLgvInRequest;
	CButtonST	m_btnStatusCvCheckPos;
	CButtonST	m_btnSensorProduct1;
	CButtonST	m_btnSensorScInterlock;
	CButtonST	m_btnSensorRtvInterlock;
	CButtonST	m_btnSensorLgvInterlock;
	CButtonST	m_btnSensorLfInterlock;
	CComboBox	m_cbxJobType;
	CComboBox	m_cbxDestTrack;
	CComboBox	m_cbxGenCode;
	BUTTON_TYPE	m_btnWrite;
	BUTTON_TYPE	m_btnStatusReport;
	BUTTON_TYPE	m_btnRefresh;
	BUTTON_TYPE	m_btnPaste;
	BUTTON_TYPE	m_btnDelete;
	BUTTON_TYPE	m_btnCut;
	BUTTON_TYPE	m_btnCopy;
	BUTTON_TYPE	m_btnOK;
	BOOL		m_bSuspend;
	BOOL		m_bDeadLock;
	int			m_nDestTrack;
	BOOL		m_bReqError;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCvDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonWrite();
	afx_msg void OnButtonCopy();
	afx_msg void OnButtonPaste();
	afx_msg void OnButtonCut();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonStatusReport();
	afx_msg void OnCheckSuspend();
	afx_msg void OnCheckDeadLock();
	afx_msg void OnSelchangeComboDestTrack();
//	afx_msg void OnButtonWrite2();
	afx_msg void OnCheckError();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVDLG_H__75FE0FD1_A729_4B8B_B91C_4A39E9AB4E06__INCLUDED_)
