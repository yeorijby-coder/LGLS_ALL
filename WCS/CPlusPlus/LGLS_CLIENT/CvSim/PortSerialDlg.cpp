// PortSerialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "PortSerialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortSerialDlg dialog


CPortSerialDlg::CPortSerialDlg(CPortSerial* pPort, CWnd* pParent /*=NULL*/)
	: CDialog(CPortSerialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortSerialDlg)
	m_strDevice = _T("");
	m_strComName = _T("");
	m_nBaudRate = 0;
	m_nParity = 0;
	m_nBitPerChar = 0;
	m_dwReadTimeout = 0;
	m_nStopBits = 0;
	//}}AFX_DATA_INIT

	m_pPort = pPort;
	ASSERT(m_pPort != NULL);
}


void CPortSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortSerialDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDT_DEVICE, m_strDevice);
	DDX_Text(pDX, IDC_EDT_COMNAME, m_strComName);
	DDX_Text(pDX, IDC_EDT_BAUD_RATE, m_nBaudRate);
	DDX_Text(pDX, IDC_EDT_PARITY, m_nParity);
	DDX_Text(pDX, IDC_EDT_BIT_PER_CHAR, m_nBitPerChar);
	DDX_Text(pDX, IDC_EDT_READ_TIMEOUT, m_dwReadTimeout);
	DDX_Text(pDX, IDC_EDT_STOP_BITS, m_nStopBits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortSerialDlg, CDialog)
	//{{AFX_MSG_MAP(CPortSerialDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortSerialDlg message handlers

BOOL CPortSerialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_strDevice		= m_pPort->m_strDevice;
	m_strComName	= m_pPort->m_strComName;
	m_nBaudRate		= m_pPort->m_nBaudRate;
	m_nParity		= m_pPort->m_nParity;
	m_nStopBits		= m_pPort->m_nStopBits;
	m_nBitPerChar	= m_pPort->m_nBitsPerChar;
	m_dwReadTimeout	= m_pPort->m_dwReadTimeOut;

	UpdateData(FALSE);

	INIT_BUTTON(m_btnOK, IDI_OK, 24, 24, _T("저장후 닫음"));
	INIT_BUTTON(m_btnCancel, IDI_CANCEL, 24, 24, _T("저장하지 않고 닫음"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPortSerialDlg::OnOK() 
{
	if (UpdateData(TRUE) == FALSE)
		return;

	m_pPort->m_strDevice	 = m_strDevice;	
	m_pPort->m_strComName	 = m_strComName;
	m_pPort->m_nBaudRate	 = m_nBaudRate;	
	m_pPort->m_nParity		 = m_nParity;	
	m_pPort->m_nStopBits	 = m_nStopBits;	
	m_pPort->m_nBitsPerChar	 = m_nBitPerChar;
	m_pPort->m_dwReadTimeOut = m_dwReadTimeout;

	CDialog::OnOK();
}