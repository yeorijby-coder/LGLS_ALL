#if !defined(AFX_PORTETHERNETDLG_H__DAA0C85E_8AAD_4CD7_BB5E_C8F22CCEC0CD__INCLUDED_)
#define AFX_PORTETHERNETDLG_H__DAA0C85E_8AAD_4CD7_BB5E_C8F22CCEC0CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortEthernetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPortEthernetDlg dialog

#include "PortObject.h"

class CPortEthernetDlg : public CDialog
{
// Construction
public:
	CPortEthernetDlg(CPortEthernet* pPort, CWnd* pParent = NULL);   // standard constructor

private:
	CPortEthernet* m_pPort;
	
public:
// Dialog Data
	//{{AFX_DATA(CPortEthernetDlg)
	enum { IDD = IDD_PORT_ETHERNET };
	BUTTON_TYPE	m_btnOK;
	BUTTON_TYPE	m_btnCancel;
	BOOL	m_bConnect;
	BOOL	m_bRetry;
	int		m_nPort1;
	int		m_nPort2;
	int		m_nPort3;
	int		m_nPort4;
	int		m_nPort5;
	int		m_nPort6;
	int		m_nPort7;
	int		m_nPort8;
	int		m_nPort9;
	int		m_nPort10;
	int		m_nPort11;
	int		m_nPort12;
	int		m_nPort13;
	int		m_nPort14;
	int		m_nPort15;
	int		m_nPort16;
	int		m_nPort17;
	int		m_nPort18;
	CString	m_strDevice;
	BOOL	m_bWriteLog;
	DWORD	m_dwRecvTimeout;
	//}}AFX_DATA

//	CSpreadSheet m_SpreadSheet;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortEthernetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPortEthernetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChkPortSeq();
	// 포트번호를 연속적으로 사용할때... 1번 포트만 활성화 되고 나머지는 비활성화 된다.
	BOOL m_bPortSeq;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTETHERNETDLG_H__DAA0C85E_8AAD_4CD7_BB5E_C8F22CCEC0CD__INCLUDED_)
