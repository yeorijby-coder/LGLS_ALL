// LgvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "LgvDlg.h"
#include "EcsDoc.h"
#include "LgvManualDlg.h"
#include "Lgv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLgvDlg dialog

CLgvDlg::CLgvDlg(CWnd* pParent /* = NULL */)
	: CDialog(CLgvDlg::IDD, pParent)
{
	m_pLgv = NULL;
	m_nSelectCar = 0;

	//{{AFX_DATA_INIT(CLgvDlg)
	m_bSuspend = FALSE;
	//}}AFX_DATA_INIT
}


CLgvDlg::CLgvDlg(CLgv* pLgv, int nCarNum, CWnd* pParent /* = NULL */)
	: CDialog(CLgvDlg::IDD, pParent)
{
	m_pLgv = pLgv;
	DEBUGER_ASSERT_VALID(m_pLgv != NULL);
	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo != NULL);
//	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo->m_pCvInfo != NULL);
	m_nSelectCar = nCarNum;

	//{{AFX_DATA_INIT(CLgvDlg)
	m_bSuspend = m_pLgv->m_pInfo->m_bSuspend;
	//}}AFX_DATA_INIT
}


void CLgvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLgvDlg)
	DDX_Control(pDX, IDC_LIST_LGV_JOB, m_lstJob);
//	DDX_Control(pDX, IDC_TAB_AGV, m_tabAgv);
	DDX_Control(pDX, IDC_BUTTON_REQUEST_RESET, m_btnReset);
	DDX_Control(pDX, IDC_BUTTON_REQUEST_SUSPEND, m_btnRgvStop);
	DDX_Control(pDX, IDC_BUTTON_REQUEST_TRANSPORT, m_btnTRR);
	DDX_Control(pDX, IDC_BUTTON_REQUEST_DELETE, m_btnDEL);
	DDX_Control(pDX, IDC_BUTTON_REQUEST_STATUS, m_btnTRQ);
	DDX_Control(pDX, IDC_BUTTON_REQUEST_AGV_STATUS, m_btnAGQ);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Check(pDX, IDC_CHECK_SUSPEND, m_bSuspend);
	DDX_Control(pDX, IDC_BUTTON_DOOR1_REPORT, m_btnDoor1Report);
	DDX_Control(pDX, IDC_BUTTON_DOOR2_OPEN, m_btnDoor2Open);
	DDX_Control(pDX, IDC_BUTTON_DOOR2_CLOSE, m_btnDoor2Close);
	DDX_Control(pDX, IDC_BUTTON_DOOR2_REPORT, m_btnDoor2Report);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLgvDlg, CDialog)
	//{{AFX_MSG_MAP(CLgvDlg)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_RESET, OnButtonRequestReset)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_TRANSPORT, OnButtonRequestTransport)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_DELETE, OnButtonRequestDelete)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_STATUS, OnButtonRequestStatus)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_AGV_STATUS, OnButtonRequestAgvStatus)
	ON_BN_CLICKED(IDC_BUTTON_DOOR1_REPORT, OnButtonDoor1Report)
	ON_BN_CLICKED(IDC_BUTTON_DOOR2_REPORT, OnButtonDoor2Report)
	ON_BN_CLICKED(IDC_CHECK_SUSPEND, OnCheckSuspend)
	ON_BN_CLICKED(IDC_BUTTON_DOOR1_OPEN, OnButtonDoor1Open)
	ON_BN_CLICKED(IDC_BUTTON_DOOR2_OPEN, OnButtonDoor2Open)
	ON_BN_CLICKED(IDC_BUTTON_DOOR1_CLOSE, OnButtonDoor1Close)
	ON_BN_CLICKED(IDC_BUTTON_DOOR2_CLOSE, OnButtonDoor2Close)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST_SUSPEND, OnButtonRequestSuspend)
//	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLgvDlg message handlers

BOOL CLgvDlg::OnInitDialog() 
{
	DEBUGER_ASSERT_VALID(m_pLgv != NULL);
	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo != NULL);
	m_bSuspend = m_pLgv->m_pInfo->m_bSuspend;

	m_pLgv->m_pEquipDlg = this;
	
	CDialog::OnInitDialog();

	int w = 16, h = 16;
	INIT_BUTTON(m_btnReset, IDI_ICON_RETURN, w, h, _T("수신 시퀀스번호 RESET"));
	INIT_BUTTON(m_btnTRR, IDI_ICON_MANUAL, w, h, _T("LGV 작업지시 요청 (TRR)"));
	INIT_BUTTON(m_btnDEL, IDI_CANCEL, w, h, _T("LGV 작업삭제 요청 (DEL)"));
	INIT_BUTTON(m_btnTRQ, IDI_BUTTERFLY, w, h, _T("작업상태 갱신 요청 (TRQ)"));
	INIT_BUTTON(m_btnAGQ, IDI_BUTTERFLY, w, h, _T("AGV 상태갱신 요청 (AGQ)"));
	INIT_BUTTON(m_btnDoor1Report, IDI_LAMP1, w, h, _T("DOOR1 상태보고 (HOS)"));
//	INIT_BUTTON(m_btnDoor2Report, IDI_LAMP1, w, h, "DOOR2 상태보고 (HOS)");
	INIT_BUTTON(m_btnOK, IDI_EXIT, 32, 32, _T("다이얼로그 닫기"));
//	INIT_BUTTON(m_btnDoor1Close, IDI_LAMP1, w, h, "닫기요청");
//	INIT_BUTTON(m_btnDoor1Open, IDI_LAMP1, w, h, "열기요청");
	INIT_BUTTON(m_btnRgvStop, IDI_LAMP1, w, h, _T("열기요청"));

	CString strLgvCar;
	strLgvCar.Format(_T("RGV #%d"), m_nSelectCar);
	SetWindowText(strLgvCar);

//	DDX_Control(pDX, IDC_BUTTON_DOOR1_CLOSE, m_btnDoor1Close);
//	DDX_Control(pDX, IDC_BUTTON_DOOR1_REPORT, m_btnDoor1Report);
	//////////////////////////////////////////////////////////////////////////

//	for (int i=0; i<m_pLgv->m_pInfo->m_oLgvVehicles.GetSize(); ++i)
//	{
//		CLgvVehicle* pAgv = m_pLgv->m_pInfo->m_oLgvVehicles[i];
//		m_tabAgv.InsertItem(TCIF_TEXT|TCIF_PARAM, i, pAgv->GetAgvString(), 0, pAgv->m_nVehicleID);
//		DEBUGER_ASSERT_VALID(pAgv != NULL);
//	}
//	if(m_nSelectCar < 6)
//		m_tabAgv.SetCurSel(m_nSelectCar-1);
//	else
//		m_tabAgv.SetCurSel(m_nSelectCar-6);
//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	const int size			= 6;
//	UINT	mask			= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
//	int		fmt				= LVCFMT_LEFT;
//	int		cx[size]		= {80, 80, 80, 80, 170, 200};
//	LPTSTR	pszText[size]	= {_T("작업번호"), _T("차량번호"), _T("출발위치"), _T("도착위치"), _T("작업상태"), _T("진행상태")};
//	for (int i=0; i<size; ++i)
//		m_lstJob.InsertColumn(i, (LPCTSTR)pszText[i], fmt, cx[i]);
//	m_lstJob.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
//	m_lstJob.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	UpdateList();

	
	//////////////////////////////////////////////////////////////////////////

//	int nDoorCount = m_pLgv->m_pInfo->m_pCvInfo->GetDoorCount();
//	GetDlgItem(IDC_BUTTON_DOOR1_OPEN)->EnableWindow(nDoorCount > 0);
//	GetDlgItem(IDC_BUTTON_DOOR1_CLOSE)->EnableWindow(nDoorCount > 0);
//	GetDlgItem(IDC_BUTTON_DOOR1_REPORT)->EnableWindow(nDoorCount > 0);
//	GetDlgItem(IDC_BUTTON_DOOR2_OPEN)->EnableWindow(nDoorCount > 1);
//	GetDlgItem(IDC_BUTTON_DOOR2_CLOSE)->EnableWindow(nDoorCount > 1);
//	GetDlgItem(IDC_BUTTON_DOOR2_REPORT)->EnableWindow(nDoorCount > 1);

//	SetWindowText(m_pLgv->GetDeviceName());
	WindowProc(WM_USER_REFRESH_NOTIFY, 0, 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CLgvDlg::DestroyWindow() 
{
	m_pLgv->m_pEquipDlg = NULL;
	
	return CDialog::DestroyWindow();
}

LRESULT CLgvDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CString strTemp;
	switch (message)
	{
	case WM_USER_REFRESH_NOTIFY:
		{
			CLgvInfo* pInfo = m_pLgv->m_pInfo;
			DEBUGER_ASSERT_VALID(pInfo != NULL);
//			DEBUGER_ASSERT_VALID(pInfo->m_pCvInfo != NULL);

//			SetDlgItemText(IDC_EDIT_SERVER_STATUS, m_pLgv->m_pListener ? _T("구동") : _T("종료"));
//			SetDlgItemText(IDC_EDIT_CLIENT_STATUS, m_pLgv->m_pSocket ? m_pLgv->m_pSocket->GetPeerInfo() : _T("없음"));
//			SetDlgItemText(IDC_EDIT_CV_STATUS, pInfo->m_pCvInfo->m_pEquipment->IsConnect() ? _T("통신연결") : _T("연결끊김"));
			
			CInterfaceThreadSk* pSocket = (CInterfaceThreadSk*)m_pLgv->m_pThreadSocket;
//			SetDlgItemInt(IDC_EDIT_SEQNO_RECV, pSocket ? pSocket->m_nSeqRecv : -1);
//			SetDlgItemInt(IDC_EDIT_SEQNO_SEND, pSocket ? pSocket->m_nSeqSend : -1);
//			SetDlgItemText(IDC_EDIT_MESSAGE_RECV, pSocket ? pSocket->m_strRecv : _T(""));
//			SetDlgItemText(IDC_EDIT_MESSAGE_SEND, pSocket ? pSocket->m_strSend : _T(""));

		//	TCITEM tcItem;
		//	m_tabAgv.GetItem(m_tabAgv.GetCurSel(), &tcItem);
		
			CLgvVehicle* pAgv = pInfo->GetLgvVehicle(m_nSelectCar);
			if (pAgv)
			{
				
				SetDlgItemText(IDC_EDIT_AGV_MODE, pAgv->GetModeString());
				SetDlgItemInt(IDC_EDIT_AGV_POSITION, pAgv->m_nCurPosition);
//				SetDlgItemText(IDC_EDIT_AGV_STATUS, pAgv->GetActivityString());
				SetDlgItemText(IDC_EDIT_LOAD_STATUS, pAgv->GetLoadStatusString());
				SetDlgItemText(IDC_EDIT_BATTERY_STATUS, pAgv->GetVehicleStatusString());
//				SetDlgItemText(IDC_EDIT_BATTERY_CHARGE, pAgv->GetBatteryChargeString());
				SetDlgItemInt(IDC_EDIT_AGV_TRANPORT_ID, pAgv->m_nLuggNum);
				CJobItem* pJobItem = m_pLgv->m_pDoc->m_pJob->Find(pAgv->m_nLuggNum);
				if (pJobItem != NULL)
				{
					strTemp.Format(_T("[ST %03d] TR# %04d"), pJobItem->m_nTempPickPos, pJobItem->m_nTempDepartTrackNum);
					SetDlgItemText(IDC_EDIT_FROM, strTemp);

					strTemp.Format(_T("[ST %03d] TR# %04d"), pJobItem->m_nTempDropPos, pJobItem->m_nTempArriveTrackNum);
					SetDlgItemText(IDC_EDIT_TO, strTemp);

					SetDlgItemText(IDC_EDIT_RGV_JOB_START_POS, pJobItem->GetStartStationString());
					SetDlgItemText(IDC_EDIT_RGV_JOB_DEST_POS, pJobItem->GetDestStationString());
					SetDlgItemText(IDC_EDIT_RGV_JOB_PRODUCT, pJobItem->m_strProductID);
					SetDlgItemText(IDC_EDIT_RGV_JOB_WEIGHT, pJobItem->m_strWeight);
					SetDlgItemText(IDC_EDIT_RGV_JOB_STATUS, pJobItem->GetRgvStatusString());
				}
				SetDlgItemInt(IDC_EDIT_ERROR_CODE, pAgv->m_nErrorCode);
				SetDlgItemText(IDC_EDIT_AGV_ERROR, pAgv->GetErrorString());
				
			}

			UpdateList();
		}
		break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CLgvDlg::UpdateList()
{
	/*
	int i = 0, j = 0, nImage = 0;
	CLgvJobItem* pLgvJobItem = NULL;
	CLgvJobList& rJobList = m_pLgv->m_pInfo->m_oLgvJobList;

	m_lstJob.DeleteAllItems();
	for (POSITION pos=rJobList.GetHeadPosition(); pos!=NULL; j=0)
	{
		pLgvJobItem = (CLgvJobItem*)rJobList.GetNext(pos);
		DEBUGER_ASSERT_VALID(pLgvJobItem != NULL);

		m_lstJob.InsertItem(LVIF_TEXT, i, CConvert::ToString(pLgvJobItem->m_nLuggNum), 0, 0, nImage, 0);
		m_lstJob.SetItem(i, ++j, LVIF_TEXT, CConvert::ToString(pLgvJobItem->m_nVehicleID), nImage, 0, 0, 0);
		m_lstJob.SetItem(i, ++j, LVIF_TEXT, pLgvJobItem->GetPickPosString(), nImage, 0, 0, 0);
		m_lstJob.SetItem(i, ++j, LVIF_TEXT, pLgvJobItem->GetDropPosString(), nImage, 0, 0, 0);
//		m_lstJob.SetItem(i, ++j, LVIF_TEXT, CJobItem::GetGenString((BYTE)pLgvJobItem->m_nCrateSize), nImage, 0, 0, 0);
//		m_lstJob.SetItem(i, ++j, LVIF_TEXT, CConvert::ToString(pLgvJobItem->m_nPriority), nImage, 0, 0, 0);
		m_lstJob.SetItem(i, ++j, LVIF_TEXT, pLgvJobItem->GetActivityString(), nImage, 0, 0, 0);
		m_lstJob.SetItem(i, ++j, LVIF_TEXT, pLgvJobItem->GetStatusString(), nImage, 0, 0, 0);
		m_lstJob.SetItemData(i++, (LPARAM)pLgvJobItem->m_nLuggNum);
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////

void CLgvDlg::OnButtonRequestReset() 
{
	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	if (AfxMessageBox(_T("시퀀스번호를 RESET 하시겠습니까?"), MB_YESNO) != IDYES)
		return;

//	CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
//	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
//	pCmdMsg->m_nCommand = CLgvCmdMsg::enReset;
//	m_pLgv->SetCmdMsg(pCmdMsg);		
}

void CLgvDlg::OnButtonRequestTransport() 
{
	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	CLgvManualDlg dlg(m_pLgv, this);
	dlg.DoModal();
}

void CLgvDlg::OnButtonRequestDelete() 
{
	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	POSITION pos = m_lstJob.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("작업을 선택해주세요!"));
		return;
	}

	if (AfxMessageBox(_T("작업삭제요청을 하시겠습니까?"), MB_YESNO) != IDYES)
		return;

	CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CLgvCmdMsg::enDEL;
	pCmdMsg->m_strValues.SetSize(CLgvCmdMsg::enSizeDEL);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_lstJob.GetItemData(m_lstJob.GetNextSelectedItem(pos)));
	m_pLgv->SetCmdMsg(pCmdMsg);		
}

void CLgvDlg::OnButtonRequestStatus() 
{
	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	POSITION pos = m_lstJob.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("작업을 선택해주세요!"));
		return;
	}

	if (AfxMessageBox(_T("작업상태요청을 하시겠습니까?"), MB_YESNO) != IDYES)
		return;

	CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CLgvCmdMsg::enTRQ;
	pCmdMsg->m_strValues.SetSize(CLgvCmdMsg::enSizeTRQ);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_lstJob.GetItemData(m_lstJob.GetNextSelectedItem(pos)));
	m_pLgv->SetCmdMsg(pCmdMsg);		
}

void CLgvDlg::OnButtonRequestAgvStatus() 
{
	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
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
void CLgvDlg::OnButtonRequestSuspend() 
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

	if (AfxMessageBox(_T("RGV를 정지 하시겠습니까?"), MB_YESNO) != IDYES)
		return;

	CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CLgvCmdMsg::enRgvStop;
	pCmdMsg->m_strValues.SetSize(1);
	pCmdMsg->m_strValues[0].Format(_T("%d"), TRUE);
	m_pLgv->SetCmdMsg(pCmdMsg);
	
}

void CLgvDlg::ReportDoorStatus(int nDoorID, BOOL bOpenStatus)
{
	DEBUGER_ASSERT_VALID(m_pLgv != NULL);
	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo->m_pCvInfo != NULL);
	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo->m_pCvInfo->m_pEquipment != NULL);

	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	if (AfxMessageBox(_T("상태보고를 하시겠습니까?"), MB_YESNO) != IDYES)
		return;

	CLgvCmdMsg* pCmdMsg = new CLgvCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CLgvCmdMsg::enHOS;
	pCmdMsg->m_strValues.SetSize(CLgvCmdMsg::enSizeHOS);
	pCmdMsg->m_strValues[0].Format(_T("%d"), nDoorID);
	pCmdMsg->m_strValues[1].Format(_T("%d"), bOpenStatus);
	m_pLgv->SetCmdMsg(pCmdMsg);	

	CString strLog;
	strLog.Format(_T("%s DOOR%d 상태보고 [STATUS=%s]"), m_pLgv->GetDeviceName(), nDoorID, bOpenStatus ? _T("OPEN") : _T("CLOSE"));
	m_pLgv->WriteLog(LOG_TYPE_EVENT, strLog, _T("CLgvDlg::ReportDoorStatus"));
}

void CLgvDlg::OnButtonDoor1Report() 
{
//	if (m_pLgv->m_pInfo->m_pCvInfo->m_pEquipment->IsConnect() == FALSE)
//	{
//		AfxMessageBox(_T("CV가 통신이 연결되지 않아 DOOR 상태를 알 수 없습니다. CV 통신연결후 다시 시도해주세요!"));
//		return;
//	}
//
//	ReportDoorStatus(1, m_pLgv->m_pInfo->m_pCvInfo->IsDoorOpen(0));
}

void CLgvDlg::OnButtonDoor2Report() 
{
//	if (m_pLgv->m_pInfo->m_pCvInfo->m_pEquipment->IsConnect() == FALSE)
//	{
//		AfxMessageBox(_T("CV가 통신이 연결되지 않아 DOOR 상태를 알 수 없습니다. CV 통신연결후 다시 시도해주세요!"));
//		return;
//	}
//
//	ReportDoorStatus(2, m_pLgv->m_pInfo->m_pCvInfo->IsDoorOpen(1));
}

void CLgvDlg::OnButtonDoor1Open() 
{
//	if (m_pLgv->m_pInfo->m_pCvInfo->IsDoorOpen(0) == FALSE)
//	{
//		if (AfxMessageBox(_T("현재 DOOR CLOSE 상태입니다. 계속 진행하시겠습니까?"), MB_YESNO) != IDYES)
//			return;
//	}

//	// C/V에 DoorOpen 요청 - test용
//	CCvCmdMsg* pCmdMsg = new CCvCmdMsg;
//	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
//	pCmdMsg->m_nCommand = CCvCmdMsg::enCmdDoorOpen;
//	pCmdMsg->m_strValues.SetSize(CCvCmdMsg::enSizeDoor);
//	pCmdMsg->m_strValues[0].Format(_T("%d"), 0);
//	m_pLgv->m_pInfo->m_pCvInfo->m_pEquipment->SetCmdMsg(pCmdMsg);
//	::Sleep(100);

//	if (AfxMessageBox(_T("DOOR1 OPEN되었다고 보고하시겠습니까? 반드시 DOOR 상태를 확인한 후 OPEN인 경우에만 사용하세요."), MB_YESNO) != IDYES)
//		return;

//	ReportDoorStatus(1, TRUE);
}

void CLgvDlg::OnButtonDoor2Open() 
{
//	if (m_pLgv->m_pInfo->m_pCvInfo->IsDoorOpen(1) == FALSE)
//	{
//		if (AfxMessageBox(_T("현재 DOOR CLOSE 상태입니다. 계속 진행하시겠습니까?"), MB_YESNO) != IDYES)
//			return;
//	}
//
//	// C/V에 DoorOpen 요청 - test용
//	CCvCmdMsg* pCmdMsg = new CCvCmdMsg;
//	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
//	pCmdMsg->m_nCommand = CCvCmdMsg::enCmdDoorOpen;
//	pCmdMsg->m_strValues.SetSize(CCvCmdMsg::enSizeDoor);
//	pCmdMsg->m_strValues[0].Format(_T("%d"), 1);
//	m_pLgv->m_pInfo->m_pCvInfo->m_pEquipment->SetCmdMsg(pCmdMsg);
//	::Sleep(100);

//	if (AfxMessageBox(_T("DOOR2 OPEN되었다고 보고하시겠습니까? 반드시 DOOR 상태를 확인한 후 OPEN인 경우에만 사용하세요."), MB_YESNO) != IDYES)
//		return;
//
//	ReportDoorStatus(2, TRUE);
}

void CLgvDlg::OnButtonDoor1Close() 
{
//	// C/V에 DoorOpen 요청 - test용
//	CCvCmdMsg* pCmdMsg = new CCvCmdMsg;
//	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
//	pCmdMsg->m_nCommand = CCvCmdMsg::enCmdDoorClose;
//	pCmdMsg->m_strValues.SetSize(CCvCmdMsg::enSizeDoor);
//	pCmdMsg->m_strValues[0].Format(_T("%d"), 0);
//	m_pLgv->m_pInfo->m_pCvInfo->m_pEquipment->SetCmdMsg(pCmdMsg);
//	::Sleep(100);

//	if (AfxMessageBox(_T("DOOR1 CLOSE되었다고 보고하시겠습니까? 반드시 DOOR 상태를 확인한 후 CLOSE인 경우에만 사용하세요."), MB_YESNO) != IDYES)
//		return;

//	ReportDoorStatus(1, FALSE);
}

void CLgvDlg::OnButtonDoor2Close() 
{
//	// C/V에 DoorOpen 요청 - test용
//	CCvCmdMsg* pCmdMsg = new CCvCmdMsg;
//	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
//	pCmdMsg->m_nCommand = CCvCmdMsg::enCmdDoorClose;
//	pCmdMsg->m_strValues.SetSize(CCvCmdMsg::enSizeDoor);
//	pCmdMsg->m_strValues[0].Format(_T("%d"), 1);
//	m_pLgv->m_pInfo->m_pCvInfo->m_pEquipment->SetCmdMsg(pCmdMsg);
//	::Sleep(100);

//	if (AfxMessageBox(_T("DOOR2 CLOSE되었다고 보고하시겠습니까? 반드시 DOOR 상태를 확인한 후 CLOSE인 경우에만 사용하세요."), MB_YESNO) != IDYES)
//		return;

//	ReportDoorStatus(2, FALSE);
}

void CLgvDlg::OnCheckSuspend() 
{
	DEBUGER_ASSERT_VALID(m_pLgv != NULL);
	DEBUGER_ASSERT_VALID(m_pLgv->m_pInfo != NULL);

	if (m_pLgv->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		UpdateData(FALSE);
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
void CLgvDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	
}
*/
