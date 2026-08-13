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
	, m_bPortSeq(FALSE)
{
	//{{AFX_DATA_INIT(CPortEthernetDlg)
	m_bConnect = FALSE;
	m_bRetry = FALSE;
	m_nPort1 = 0;
	m_nPort2 = 0;
	m_nPort3 = 0;
	m_nPort4 = 0;
	m_nPort5 = 0;
	m_nPort6 = 0;
	m_nPort7 = 0;
	m_nPort8 = 0;
	m_nPort9 = 0;
	m_nPort10 = 0;
	m_nPort11 = 0;
	m_nPort12 = 0;
	m_nPort13 = 0;
	m_nPort14 = 0;
	m_nPort15 = 0;
	m_nPort16 = 0;
	m_nPort17 = 0;
	m_nPort18 = 0;
	m_strDevice = _T("");
	m_bWriteLog = FALSE;
	m_dwRecvTimeout = 0;
	//}}AFX_DATA_INIT

	m_pPort = pPort;
	ASSERT(m_pPort != NULL);


	/*
	m_SpreadSheet.m_nSorting = SS_USERCOLACTION_DEFAULT;
	//m_SpreadSheet.m_bAutoResizeCol = true;
	m_SpreadSheet.m_wGridType = SS_GRID_HORIZONTAL|SS_GRID_VERTICAL|SS_GRID_SOLID;
	m_SpreadSheet.m_rectMargin = CRect(0, 35, 0, 0);

	HFONT hFontH = CreateFont(20,0,0,0,700,0,0,0,0,0,0,0,0,_T("Tahoma"));
	HFONT hFontB = CreateFont(12,0,0,0,FW_HEAVY,0,0,0,0,0,0,0,0,_T("굴림"));
	m_SpreadSheet.AddSheet("", this, hFontH, hFontB);
	m_SpreadSheet.AddColHead("항  목", 14);
	m_SpreadSheet.AddColHead("설  정", 20);
	m_SpreadSheet.AddColHead("선  택", 10);
	m_SpreadSheet.AddColHead("확  인", 12);
	m_SpreadSheet.AddColHead("기  타", 300);
	//*/
}


void CPortEthernetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortEthernetDlg)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Check(pDX, IDC_CHK_CONNECT, m_bConnect);
	DDX_Check(pDX, IDC_CHK_RETRY, m_bRetry);
	DDX_Text(pDX, IDC_EDT_PORT1, m_nPort1);
	DDX_Text(pDX, IDC_EDT_PORT2, m_nPort2);
	DDX_Text(pDX, IDC_EDT_PORT3, m_nPort3);
	DDX_Text(pDX, IDC_EDT_PORT4, m_nPort4);
	DDX_Text(pDX, IDC_EDT_PORT5, m_nPort5);
	DDX_Text(pDX, IDC_EDT_PORT6, m_nPort6);
	DDX_Text(pDX, IDC_EDT_PORT7, m_nPort7);
	DDX_Text(pDX, IDC_EDT_PORT8, m_nPort8);
	DDX_Text(pDX, IDC_EDT_PORT9, m_nPort9);
	DDX_Text(pDX, IDC_EDT_PORT10, m_nPort10);
	DDX_Text(pDX, IDC_EDT_PORT11, m_nPort11);
	DDX_Text(pDX, IDC_EDT_PORT12, m_nPort12);
	DDX_Text(pDX, IDC_EDT_PORT13, m_nPort13);
	DDX_Text(pDX, IDC_EDT_PORT14, m_nPort14);
	DDX_Text(pDX, IDC_EDT_PORT15, m_nPort15);
	DDX_Text(pDX, IDC_EDT_PORT16, m_nPort16);
	DDX_Text(pDX, IDC_EDT_PORT17, m_nPort17);
	DDX_Text(pDX, IDC_EDT_PORT18, m_nPort18);
	DDX_Text(pDX, IDC_EDT_NAME, m_strDevice);
	DDX_Check(pDX, IDC_CHK_WRITE_LOG, m_bWriteLog);
	DDX_Text(pDX, IDC_EDT_RECV_TIMEOUT, m_dwRecvTimeout);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHK_PORT_SEQ, m_bPortSeq);
}


BEGIN_MESSAGE_MAP(CPortEthernetDlg, CDialog)
	//{{AFX_MSG_MAP(CPortEthernetDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_PORT_SEQ, &CPortEthernetDlg::OnBnClickedChkPortSeq)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortEthernetDlg message handlers

BOOL CPortEthernetDlg::OnInitDialog() 
{
	m_strDevice		= m_pPort->m_strDevice;
	m_nPort1		= m_pPort->m_nPort[0];
	m_nPort2		= m_pPort->m_nPort[1];
	m_nPort3		= m_pPort->m_nPort[2];
	m_nPort4		= m_pPort->m_nPort[3];
	m_nPort5		= m_pPort->m_nPort[4];
	m_nPort6		= m_pPort->m_nPort[5];
	m_nPort7		= m_pPort->m_nPort[6];
	m_nPort8		= m_pPort->m_nPort[7];
	m_nPort9		= m_pPort->m_nPort[8];
	m_nPort10		= m_pPort->m_nPort[9];
	m_nPort11		= m_pPort->m_nPort[10];
	m_nPort12		= m_pPort->m_nPort[11];
	m_nPort13		= m_pPort->m_nPort[12];
	m_nPort14		= m_pPort->m_nPort[13];
	m_nPort15		= m_pPort->m_nPort[14];
	m_nPort16		= m_pPort->m_nPort[15];
	m_nPort17		= m_pPort->m_nPort[16];
	m_nPort18		= m_pPort->m_nPort[17];
	m_bConnect		= m_pPort->m_bConnect;
	m_bRetry		= m_pPort->m_bRetry;
	m_bWriteLog		= m_pPort->m_bWriteLog;
	m_dwRecvTimeout	= m_pPort->m_dwRecvTimeout;

	CDialog::OnInitDialog();

	SetDlgItemText(IDC_IPADDRESS, m_pPort->m_strIP);

	INIT_BUTTON(m_btnOK, IDI_OK, 24, 24, _T("저장후 닫음"));
	INIT_BUTTON(m_btnCancel, IDI_CANCEL, 24, 24, _T("저장하지 않고 닫음"));
	




//	m_SpreadSheet.Create();
//	m_SpreadSheet.SetStatic(1, SS_ALLROWS);			// 6.0에선 되는데...
//	m_SpreadSheet.SetStatic(1, 1);
//	m_SpreadSheet.SetColMerge(1);

//	m_staticTitle.SetWindowText("DETAIL INFORMATION");
//	m_staticTitle.SetTransparent(TRUE);
//	m_staticTitle.SetFont(_T("Arial Black"));
//	m_staticTitle.SetTextColor(RGB(255, 102, 0));
//	m_staticTitle.SetFontSize(30, TRUE);

	return TRUE;
}

void CPortEthernetDlg::OnOK() 
{
	if (UpdateData(TRUE) == FALSE)
		return;

	GetDlgItemText(IDC_IPADDRESS, m_pPort->m_strIP);

	m_pPort->m_strDevice	= m_strDevice;
	m_pPort->m_nPort[0]		= m_nPort1;
	m_pPort->m_nPort[1]		= m_nPort2;
	m_pPort->m_nPort[2]		= m_nPort3;
	m_pPort->m_nPort[3]		= m_nPort4;
	m_pPort->m_nPort[4]		= m_nPort5;
	m_pPort->m_nPort[5]		= m_nPort6;
	m_pPort->m_nPort[6]		= m_nPort7;
	m_pPort->m_nPort[7]		= m_nPort8;
	m_pPort->m_nPort[8]		= m_nPort9;
	m_pPort->m_nPort[9]		= m_nPort10;
	m_pPort->m_nPort[10]	= m_nPort11;
	m_pPort->m_nPort[11]	= m_nPort12;
	m_pPort->m_nPort[12]	= m_nPort13;
	m_pPort->m_nPort[13]	= m_nPort14;
	m_pPort->m_nPort[14]	= m_nPort15;
	m_pPort->m_nPort[15]	= m_nPort16;
	m_pPort->m_nPort[16]	= m_nPort17;
	m_pPort->m_nPort[17]	= m_nPort18;
	m_pPort->m_bConnect		= m_bConnect;		
	m_pPort->m_bRetry		= m_bRetry;
	m_pPort->m_bWriteLog	= m_bWriteLog;
	m_pPort->m_dwRecvTimeout= m_dwRecvTimeout;

	if (m_bPortSeq == TRUE)
	{
		int nPort1 = m_nPort1;
		for (int i = 1 ; i < PLC_CONN_PORT_CNT ; i++)		
		{
			m_pPort->m_nPort[i] = ++nPort1;
		}
	}


	CDialog::OnOK();
}


void CPortEthernetDlg::OnBnClickedChkPortSeq()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (UpdateData(TRUE) == FALSE)
		return;

	// 체크가 되면 1번 포트를 제외하고 나머지 포트는 모두 비활성화 된다.
	BOOL bEnable = TRUE;
	if (m_bPortSeq == TRUE)
	{
		bEnable = FALSE;
	}

	// 1번 포트를 제외해야 하므로 1부터 시작
	for (int i = 1 ; i < PLC_CONN_PORT_CNT ; i++)		
	{
		ENABLE_WND(IDC_EDT_PORT1 + i, bEnable);
	}
}
	