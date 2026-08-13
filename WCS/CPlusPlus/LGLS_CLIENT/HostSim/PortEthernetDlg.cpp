// PortEthernetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "PortEthernetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortEthernetDlg dialog


CPortEthernetDlg::CPortEthernetDlg(CPortEthernet* pPort, CWnd* pParent /* = NULL */)
	: CDialog(CPortEthernetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortEthernetDlg)
	m_bConnect = FALSE;
	m_bRetry = FALSE;
	m_nPort = 0;
	m_strDevice = _T("");
	m_bWriteLog = FALSE;
	m_dwRecvTimeout = 0;
	//}}AFX_DATA_INIT

	m_pPort = pPort;
	ASSERT(m_pPort != NULL);
}


void CPortEthernetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortEthernetDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Check(pDX, IDC_CHK_CONNECT, m_bConnect);
	DDX_Check(pDX, IDC_CHK_RETRY, m_bRetry);
	DDX_Text(pDX, IDC_EDT_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDT_NAME, m_strDevice);
	DDX_Check(pDX, IDC_CHK_WRITE_LOG, m_bWriteLog);
	DDX_Text(pDX, IDC_EDT_RECV_TIMEOUT, m_dwRecvTimeout);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortEthernetDlg, CDialog)
	//{{AFX_MSG_MAP(CPortEthernetDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortEthernetDlg message handlers

BOOL CPortEthernetDlg::OnInitDialog() 
{
	m_strDevice		= m_pPort->m_strDevice;
	m_nPort			= m_pPort->m_nPort;
	m_bConnect		= m_pPort->m_bConnect;
	m_bRetry		= m_pPort->m_bRetry;
	m_bWriteLog		= m_pPort->m_bWriteLog;
	m_dwRecvTimeout	= m_pPort->m_dwRecvTimeout;

	CDialog::OnInitDialog();

	SetDlgItemText(IDC_IPADDRESS, m_pPort->m_strIP);

	INIT_BUTTON(m_btnOK, IDI_OK, 24, 24, _T("저장후 닫음"));
	INIT_BUTTON(m_btnCancel, IDI_CANCEL, 24, 24, _T("저장하지 않고 닫음"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPortEthernetDlg::OnOK() 
{
	if (UpdateData(TRUE) == FALSE)
		return;

	GetDlgItemText(IDC_IPADDRESS, m_pPort->m_strIP);

	m_pPort->m_strDevice	= m_strDevice;
	m_pPort->m_nPort		= m_nPort;
	m_pPort->m_bConnect		= m_bConnect;		
	m_pPort->m_bRetry		= m_bRetry;
	m_pPort->m_bWriteLog	= m_bWriteLog;
	m_pPort->m_dwRecvTimeout= m_dwRecvTimeout;

	CDialog::OnOK();
}
