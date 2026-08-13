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
#include "Equipment.h"

class CPortEthernetDlg : public CDialog
{
// Construction
public:
//	CPortEthernetDlg(CPortEthernet* pPort, CWnd* pParent = NULL);   // standard constructor
	CPortEthernetDlg(CEquipment * pEquipment, CWnd* pParent = NULL);   // standard constructor

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
	CString	m_strDevice;
	BOOL	m_bWriteLog;
	DWORD	m_dwRecvTimeout;
	//}}AFX_DATA

	CEquipment * m_pEquipment;

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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTETHERNETDLG_H__DAA0C85E_8AAD_4CD7_BB5E_C8F22CCEC0CD__INCLUDED_)
