#if !defined(AFX_PORTSERIALDLG_H__493A2A91_AAE7_4752_9A69_6FC9C1CC6886__INCLUDED_)
#define AFX_PORTSERIALDLG_H__493A2A91_AAE7_4752_9A69_6FC9C1CC6886__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortSerialDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPortSerialDlg dialog

#include "PortObject.h"

class CPortSerialDlg : public CDialog
{
// Construction
public:
	CPortSerialDlg(CPortSerial* pPort, CWnd* pParent = NULL);   // standard constructor

private:
	CPortSerial* m_pPort;
	
public:
// Dialog Data
	//{{AFX_DATA(CPortSerialDlg)
	enum { IDD = IDD_PORT_SERIAL };
	BUTTON_TYPE	m_btnCancel;
	BUTTON_TYPE	m_btnOK;
	CString	m_strDevice;
	CString	m_strComName;
	int		m_nBaudRate;
	int		m_nParity;
	int		m_nBitPerChar;
	DWORD	m_dwReadTimeout;
	int		m_nStopBits;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortSerialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPortSerialDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTSERIALDLG_H__493A2A91_AAE7_4752_9A69_6FC9C1CC6886__INCLUDED_)
