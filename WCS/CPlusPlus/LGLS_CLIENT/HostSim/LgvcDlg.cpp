// LgvcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "LgvcDlg.h"
#include "EcsDoc.h"
#include "LgvManualDlg.h"
#include "Lgv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLgvcDlg dialog

CLgvcDlg::CLgvcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLgvcDlg::IDD, pParent)
{

	m_pLgv = NULL;
	m_nSelectLgv = 0;
	//{{AFX_DATA_INIT(CLgvcDlg)
	//}}AFX_DATA_INIT
}


CLgvcDlg::CLgvcDlg(CLgv* pLgv, int nLgvNum, CWnd* pParent /*=NULL*/)
	: CDialog(CLgvcDlg::IDD, pParent)
{

	m_pLgv = pLgv;
	m_nSelectLgv = nLgvNum;
	//{{AFX_DATA_INIT(CLgvcDlg)
	//}}AFX_DATA_INIT
}


void CLgvcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLgvcDlg)
//	DDX_Control(pDX, IDC_LIST_LGV_JOB, m_lstJob);
	DDX_Control(pDX, IDC_BUTTON_REQUEST_SUSPEND, m_btnRgvStop);
	DDX_Control(pDX, IDC_BUTTON_REQUEST_TRANSPORT, m_btnTRR);
	DDX_Control(pDX, IDC_BUTTON_REQUEST_AGV_STATUS, m_btnAGQ);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Check(pDX, IDC_CHECK_SUSPEND, m_bSuspend);
	DDX_Control(pDX, IDC_BUTTON_DOOR1_OPEN, m_btnDoor1Open);
	DDX_Control(pDX, IDC_BUTTON_DOOR1_CLOSE, m_btnDoor1Close);
	DDX_Control(pDX, IDC_BUTTON_DOOR1_OPEN_ACCEPT, m_btnDoor1OpenAccept);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLgvcDlg, CDialog)
	//{{AFX_MSG_MAP(CLgvcDlg)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_TRANSPORT, OnButtonRequestTransport)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_AGV_STATUS, OnButtonRequestAgvStatus)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_SUSPEND, OnButtonRequestSuspend)
	ON_BN_CLICKED(IDC_CHECK_SUSPEND, OnCheckSuspend)
//	ON_BN_CLICKED(IDC_BUTTON_DOOR1_OPEN, OnButtonDoor1Open)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLgvcDlg message handlers

LRESULT CLgvcDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{	
	case WM_USER_REFRESH_NOTIFY:
	{
	CLgvInfo* pInfo = m_pLgv->m_pInfo;
	DEBUGER_ASSERT_VALID(pInfo != NULL);
//			DEBUGER_ASSERT_VALID(pInfo->m_pCvInfo != NULL);

	SetDlgItemText(IDC_EDIT_CLIENT_STATUS, m_pLgv->IsConnect() ? _T("접속됨") : _T("접속 종료"));
	SetDlgItemText(IDC_EDIT_SERVER_STATUS, m_pLgv->IsConnect() ? m_pLgv->m_pThreadSocket->GetPeerInfo() : _T("없음"));
	SetDlgItemText(IDC_EDIT_CV_STATUS, pInfo->m_pCvInfo->m_pEquipment->IsConnect() ? _T("통신연결") : _T("연결끊김"));
	CInterfaceThreadSk* pSocket = (CInterfaceThreadSk*)m_pLgv->m_pThreadSocket;
	
	//			SetDlgItemInt(IDC_EDIT_SEQNO_RECV, pSocket ? pSocket->m_nSeqRecv : -1);
	//			SetDlgItemInt(IDC_EDIT_SEQNO_SEND, pSocket ? pSocket->m_nSeqSend : -1);
	SetDlgItemText(IDC_EDIT_MESSAGE_RECV, pSocket ? pSocket->m_strSend : _T(""));
	SetDlgItemText(IDC_EDIT_MESSAGE_SEND, pSocket ? pSocket->m_strRecv : _T(""));



//		int nNum = pInfo->m_pEquipment->m_nNumber;
		BOOL bDoor = (pInfo->m_pCvInfo == NULL) ? FALSE : pInfo->m_pCvInfo->IsDoorOpenReq(0);
		// TEST
//		bDoor = FALSE;

				
		int nIcon = bDoor ? IDI_ICON_LEDON : IDI_ICON_LEDOFF;
		m_btnDoor1Open.SetIcon(nIcon);

//		bDoor = (pInfo->m_pCvInfo == NULL) ? FALSE : pInfo->m_pCvInfo->IsDoorCloseResponse(0);
		// TEST
//		bDoor = FALSE;
	//
		nIcon = !bDoor ? IDI_ICON_LEDON : IDI_ICON_LEDOFF;
		m_btnDoor1Close.SetIcon(nIcon);
		

		bDoor = (pInfo->m_pCvInfo == NULL) ? FALSE : pInfo->m_pCvInfo->IsDoorOpenAccept(0);
		nIcon = bDoor ? IDI_ICON_LEDON : IDI_ICON_LEDOFF;
		m_btnDoor1OpenAccept.SetIcon(nIcon);
//		m_btnDoor2Open.SetIcon(pInfo->IsDoorOpenRequest(1) ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
//		m_btnDoor2Close.SetIcon(pInfo->IsDoorCloseRequest(1) ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
		
	//	UpdateList();
	}
	break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CLgvcDlg::OnInitDialog() 
{
	DEBUGER_ASSERT_VALID(m_pLgv != NULL);

	m_pLgv->m_pEquipDlg = this;

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	int w = 16, h = 16;
	INIT_BUTTON(m_btnTRR, IDI_ICON_MANUAL, w, h, _T("LGV 작업지시 요청 (TRR)"));
	INIT_BUTTON(m_btnAGQ, IDI_BUTTERFLY, w, h, _T("AGV 상태갱신 요청 (AGQ)"));
	INIT_BUTTON(m_btnOK, IDI_EXIT, 32, 32, _T("다이얼로그 닫기"));
	INIT_BUTTON(m_btnRgvStop, IDI_LAMP1, w, h, _T("열기요청"));
//	INIT_BUTTON(m_btnDoor1Close, IDI_LAMP1, w, h, "닫기요청");
//	INIT_BUTTON(m_btnDoor1Open, IDI_LAMP1, w, h, "열기요청");

	CString strLgv;
	strLgv.Format(_T("RGVC #%d"), m_nSelectLgv);
	SetWindowText(strLgv);

 	m_bSuspend =  m_pLgv->m_pInfo->m_bSuspend;
	UpdateData(FALSE);
	
	WindowProc(WM_USER_REFRESH_NOTIFY, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CLgvcDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pLgv->m_pEquipDlg = NULL;
	return CDialog::DestroyWindow();
}

void CLgvcDlg::OnButtonRequestTransport() 
{
	// TODO: Add your control notification handler code here
	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	CLgvManualDlg dlg(m_pLgv, this);
	dlg.DoModal();
	
}

void CLgvcDlg::OnButtonRequestAgvStatus() 
{
	// TODO: Add your control notification handler code here
	DEBUGER_ASSERT_VALID(m_pLgv != NULL);
	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo != NULL);

	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}
//	SetDlgItemText(IDC_EDIT_CV_STATUS, pInfo->m_pCvInfo->m_pEquipment->IsConnect() ? _T("통신연결") : _T("연결끊김"));

	if (m_pLgv->m_pInfo->m_pCvInfo->m_pEquipment->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("CV PLC와 통신이 연결되지 않았습니다."));
		return;
	}
	
	if (m_pLgv->m_pInfo->m_pCvInfo->IsDoorOpen(0) == TRUE)
	{
		AfxMessageBox(_T("FENCE가 열려 있습니다. \nFENCE가 열려 있는 상태에서는 RGV를 기동할 수 없습니다."));
		return;
	} 
	
	if (AfxMessageBox(_T("RGV를 기동 하시겠습니까?"), MB_YESNO) != IDYES)
		return;

//	TCITEM tcItem;
//	m_tabAgv.GetItem(m_tabAgv.GetCurSel(), &tcItem);

	CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CLgvCmdMsg::enRgvStop;
	pCmdMsg->m_strValues.SetSize(1);
	pCmdMsg->m_strValues[0].Format(_T("%d"), FALSE);
	m_pLgv->SetCmdMsg(pCmdMsg);		
	
}

void CLgvcDlg::OnButtonRequestSuspend() 
{
	// TODO: Add your control notification handler code here
	DEBUGER_ASSERT_VALID(m_pLgv != NULL);
	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo != NULL);

	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		UpdateData(FALSE);
		return;
	}

	if (m_pLgv->m_pInfo->m_pCvInfo->m_pEquipment->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("CV PLC와 통신이 연결되지 않았습니다."));
		return;
	}
	
	if (AfxMessageBox(_T("RGV를 정지 하시겠습니까?"), MB_YESNO) != IDYES)
		return;

	CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CLgvCmdMsg::enRgvStop;
	pCmdMsg->m_strValues.SetSize(1);
	pCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
	m_pLgv->SetCmdMsg(pCmdMsg);
	
}

void CLgvcDlg::OnCheckSuspend() 
{
	// TODO: Add your control notification handler code here
	DEBUGER_ASSERT_VALID(m_pLgv != NULL);
	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo != NULL);

	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		UpdateData(FALSE);
		return;
	}

	if (m_pLgv->m_pInfo->m_pCvInfo->m_pEquipment->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("CV PLC와 통신이 연결되지 않았습니다."));
		return;
	}
	
	if (!UpdateData())
		return;

	CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CLgvCmdMsg::enCmdSuspend;
	pCmdMsg->m_strValues.SetSize(1);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_bSuspend);
	m_pLgv->SetCmdMsg(pCmdMsg);

}

/*
void CLgvcDlg::OnButtonDoor1Open() 
{
	// TODO: Add your control notification handler code here
	
}
*/