// CvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "CvDlg.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCvDlg dialog


CCvDlg::CCvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCvDlg::IDD, pParent)
{
	m_pTrack = NULL;
	
	m_nLuggNum	= 0;
	m_nJobType	= 0;
	m_nDestPos	= 0;
	m_ucGenCode = 0;
	m_nSize		= 0;
	m_bClone	= FALSE;

	//{{AFX_DATA_INIT(CCvDlg)
	m_bSuspend = FALSE;
	m_bDeadLock = FALSE;
	m_nDestTrack = 0;
	//}}AFX_DATA_INIT
}


void CCvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCvDlg)
	DDX_Control(pDX, IDC_BUTTON_RESET_FIRE_ALARM, m_btnResetFireAlarm);
	DDX_Control(pDX, IDC_BUTTON_STATUS_STO_STATION, m_btnStatusStoStation);
	DDX_Control(pDX, IDC_BUTTON_STATUS_SC_STO_HS, m_btnStatusScStoHS);
	DDX_Control(pDX, IDC_BUTTON_STATUS_SC_RET_HS, m_btnStatusScRetHS);
	DDX_Control(pDX, IDC_BUTTON_STATUS_RET_STATION, m_btnStatusRetStation);
	DDX_Control(pDX, IDC_BUTTON_SENSOR_PRODUCT1, m_btnSensorProduct1);
	DDX_Control(pDX, IDC_BUTTON_SENSOR_SC_INTERLOCK, m_btnSensorScInterlock);
	DDX_Control(pDX, IDC_COMBO_JOB_TYPE, m_cbxJobType);
	DDX_Control(pDX, IDC_COMBO_DEST_TRACK, m_cbxDestTrack);
	DDX_Control(pDX, IDC_COMBO_GEN_CODE, m_cbxGenCode);
	DDX_Control(pDX, IDC_BUTTON_WRITE, m_btnWrite);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BUTTON_PASTE, m_btnPaste);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_CUT, m_btnCut);
	DDX_Control(pDX, IDC_BUTTON_COPY, m_btnCopy);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Check(pDX, IDC_CHECK_SUSPEND, m_bSuspend);
	DDX_Check(pDX, IDC_CHECK_DEADLOCK, m_bDeadLock);
	DDX_Text(pDX, IDC_EDIT_DEST_TRACK, m_nDestTrack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCvDlg, CDialog)
	//{{AFX_MSG_MAP(CCvDlg)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, OnButtonPaste)
	ON_BN_CLICKED(IDC_BUTTON_CUT, OnButtonCut)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_CHECK_SUSPEND, OnCheckSuspend)
	ON_BN_CLICKED(IDC_CHECK_DEADLOCK, OnCheckDeadLock)
	ON_CBN_SELCHANGE(IDC_COMBO_DEST_TRACK, OnSelchangeComboDestTrack)
	ON_BN_CLICKED(IDC_BUTTON_RESET_FIRE_ALARM, OnClickedResetFireAlarm)
	ON_BN_CLICKED(IDC_BUTTON_FIRE_ALARM, OnClickedFireAlarm)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCvDlg message handlers

BOOL CCvDlg::OnInitDialog() 
{
	if ((m_pTrack == NULL) || (m_pTrack->m_pEquipment == NULL) || (m_pDoc == NULL))
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return FALSE;
	}

	m_pTrack->m_pEquipment->m_pEquipDlg = this;
	m_bSuspend = m_pTrack->m_bSuspend;
	m_bDeadLock = m_pTrack->m_bDeadLock;
	BOOL bMDInit = (m_pTrack->m_wMotorData) && (m_pTrack->IsMDInitReady()); 

	CDialog::OnInitDialog();

	SetDlgItemText(IDC_EDIT_TRACK_NUM, m_pTrack->GetTrackString());
	GetDlgItem(IDC_CHECK_SUSPEND)->EnableWindow(m_pTrack->IsNotInvokeTrack() == FALSE);
//	GetDlgItem(IDC_CHECK_DEADLOCK)->EnableWindow(m_pDoc->GetDeadLockTrack(m_pTrack->m_nNumber) == m_pTrack);
//	GetDlgItem(IDC_EDIT_SIZE_CODE)->EnableWindow(FALSE);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_cbxJobType.AddString(_T("없음"));
	m_cbxJobType.SetItemData(0, 0);
	CLib::SetComboBoxJobType(m_cbxJobType);

//	m_cbxGenCode.AddString(_T("없음"));
//	m_cbxGenCode.SetItemData(0, 0);
//	CLib::SetComboBoxGenCode(m_cbxGenCode);

	m_cbxDestTrack.AddString(_T("없음"));
	m_cbxDestTrack.SetItemData(0, 0);

	CStationInfo* pStation = NULL;
	for (int i=0; i<m_pDoc->m_pStationInfos.GetSize(); ++i)
	{
		pStation = m_pDoc->m_pStationInfos[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		CString strTemp;
		int nTemp;
		
		switch (pStation->m_enKind)
		{
		case CStationInfo::enScStation:
			nTemp = pStation->GetTrackDestination();
			strTemp.Format(_T("[%04d] %s"), nTemp, pStation->m_strName);

			m_cbxDestTrack.AddString(strTemp);
			m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, nTemp);
			break;

		case CStationInfo::enRetStation:
		case CStationInfo::enArvStation:
			nTemp = pStation->m_pTrack->m_nNumber;
			strTemp.Format(_T("[%04d] %s"), nTemp, pStation->m_strName);

			m_cbxDestTrack.AddString(strTemp);
			m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, nTemp);
			break;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int w = 20, h = 20;
	INIT_BUTTON(m_btnWrite, IDI_SAVE, w, h, _T("트랙정보 쓰기"));
	INIT_BUTTON(m_btnCopy, IDI_ICON_COPY, w, h, _T("트랙정보 복사"));
	INIT_BUTTON(m_btnPaste, IDI_ICON_PASTE, w, h, _T("트랙정보 붙이기"));
	INIT_BUTTON(m_btnCut, IDI_ICON_CUT, w, h, _T("트랙정보 자르기"));
	INIT_BUTTON(m_btnDelete, IDI_CANCEL, w, h, _T("트랙정보 삭제"));
	INIT_BUTTON(m_btnRefresh, IDI_UPDATE, w, h, _T("트랙정보 갱신"));
//	INIT_BUTTON(m_btnStatusReport, IDI_RUN, w, h, _T("상태보고"));
	INIT_BUTTON(m_btnResetFireAlarm, IDI_RESET, w, h, _T("화재알람리셋"));
	INIT_BUTTON(m_btnOK, IDI_EXIT, w, h, _T("닫기"));

	WindowProc(WM_USER_REFRESH_NOTIFY, 0, m_pTrack->m_nNumber);
	WindowProc(WM_USER_REFRESH_NOTIFY, 1, m_pTrack->m_nNumber);
	// 진행시간
	if (m_pTrack->m_nDestPos != 0 && m_pTrack->IsProductSensing() == TRUE)
	{
		COleDateTimeSpan dtp = COleDateTime::GetCurrentTime() - m_pTrack->m_tOperStart;
		if (dtp.GetDays() > 99)		dtp.SetDateTimeSpan(0, 0, 0, 0);
		CString strDate = _T("");
		strDate.Format(_T("%02d일 %02d:%02d:%02d"), 
			dtp.GetDays(), dtp.GetHours(), dtp.GetMinutes(), dtp.GetSeconds());
		CWnd::SetDlgItemText(IDC_EDIT_JOB_WAIT_TIME, strDate);
	}
	// 진행시간 설정 타이머
	CWnd::SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCvDlg::DestroyWindow() 
{
	CWnd::KillTimer(1);
	m_pTrack->m_pEquipment->m_pEquipDlg = NULL;
	
	return CDialog::DestroyWindow();
}

LRESULT CCvDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_USER_REFRESH_NOTIFY:
		{
			if (wParam && lParam == m_pTrack->m_nNumber)
			{
				//m_nLuggNum	= (int)GetDlgItemInt(IDC_EDIT_LUGG_NUM);
				//m_nJobType	= (int)m_cbxJobType.GetItemData(m_cbxJobType.GetCurSel());
				//m_nDestPos	= m_nDestTrack;
				//m_ucGenCode	= (BYTE)m_cbxGenCode.GetItemData(m_cbxGenCode.GetCurSel());

				//if (m_nLuggNum != m_pTrack->m_nLuggNum)
					SetDlgItemInt(IDC_EDIT_LUGG_NUM, m_pTrack->m_nLuggNum);
				//if (m_nJobType != m_pTrack->m_nJobType)
					SetJobType(m_pTrack->m_nJobType);
				//if (m_ucGenCode != m_pTrack->m_ucGenCode)
//					SetGenCode(m_pTrack->m_ucGenCode);
				//if (m_nDestPos != m_pTrack->m_nDestPos)
					SetDestTrack(m_pTrack->m_nDestPos);
// @@@@@
//				SetDlgItemText(IDC_EDIT_SIZE, (m_pTrack->m_wReserved3 == 0) ? "小" : "大");
//				SetDlgItemInt(IDC_EDIT_SIZE_CODE, m_pTrack->m_wReserved3);
				m_nSize = (int)m_pTrack->m_wReserved3;

				m_nDestTrack = m_pTrack->m_nDestPos;
				UpdateData(FALSE);

				int nSelIndex = m_cbxDestTrack.GetCurSel();
				BOOL bEnable = ((nSelIndex == CB_ERR) || (nSelIndex == 0)) ? TRUE : FALSE;
				GetDlgItem(IDC_EDIT_DEST_TRACK)->EnableWindow(bEnable);
			}
			else
			{
				CString strMode = m_pTrack->IsAutoMode() ? _T("자동") : _T("수동");
				SetDlgItemText(IDC_EDIT_TRACK_MODE, strMode);

				CJobItem* pJobItem = m_pDoc->m_pJob->Find(m_pTrack->m_nLuggNum);
				SetDlgItemText(IDC_EDIT_JOB_START_STN, pJobItem ? pJobItem->GetStartStationString() : _T(""));
				SetDlgItemText(IDC_EDIT_JOB_DEST_STN, pJobItem ? pJobItem->GetDestStationString() : _T(""));
				SetDlgItemText(IDC_EDIT_JOB_STATUS, pJobItem ? pJobItem->GetJobStatusString() : _T(""));
				SetDlgItemText(IDC_EDIT_JOB_PRODUCT, pJobItem ? pJobItem->m_strProductID : _T(""));
				SetDlgItemText(IDC_EDIT_SIZE, (pJobItem) ? pJobItem->GetTrayTypeString() : _T(""));
				SetDlgItemText(IDC_EDIT_CV_STATUS, m_pTrack->GetErrorString());

				m_nSize = (pJobItem) ? pJobItem->m_nSize : 0;

				m_btnSensorProduct1.SetIcon(m_pTrack->IsProductSensing() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusStoStation.SetIcon(m_pTrack->IsStoStationReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusRetStation.SetIcon(m_pTrack->IsRetStationReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnSensorScInterlock.SetIcon(m_pTrack->IsSensorScInterlock() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusScStoHS.SetIcon(m_pTrack->IsScStoHsReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusScRetHS.SetIcon(m_pTrack->IsScRetHsReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				// 진행시간
				if (m_pTrack->m_nDestPos == 0 && m_pTrack->IsProductSensing() == FALSE)
					CWnd::SetDlgItemText(IDC_EDIT_JOB_WAIT_TIME, _T(""));
			}
		}
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CCvDlg::OnTimer(UINT nIDEvent) 
{
	if (m_pTrack->m_nDestPos != 0 && m_pTrack->IsProductSensing() == TRUE)
	{
		COleDateTimeSpan dtp = COleDateTime::GetCurrentTime() - m_pTrack->m_tOperStart;
		if (dtp.GetDays() > 99)		dtp.SetDateTimeSpan(0, 0, 0, 0);
		CString strDate = _T("");
		strDate.Format(_T("%02d일 %02d:%02d:%02d"), 
			dtp.GetDays(), dtp.GetHours(), dtp.GetMinutes(), dtp.GetSeconds());
		CWnd::SetDlgItemText(IDC_EDIT_JOB_WAIT_TIME, strDate);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CCvDlg::SetComboBox(CComboBox& rComboBox, DWORD dwItemData)
{
	int nIndex = -1;
	for (int i=0; i<rComboBox.GetCount(); ++i)
	{
		if (rComboBox.GetItemData(i) == dwItemData)
		{
			nIndex = i;
			break;
		}
	}
	rComboBox.SetCurSel(nIndex);
}

void CCvDlg::WriteTrack(int nLuggNum, int nJobType, int nDestPos, BYTE ucGenCode, int nSize)
{
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);
	DEBUGER_ASSERT_VALID(m_pTrack->m_pEquipment != NULL);

	if (m_pTrack->m_pEquipment->IsConnect(0) == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCmdMsg::enCmdTrackWrite;
	pCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeTrackWrite);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_pTrack->m_nNumber);
	pCmdMsg->m_strValues[1].Format(_T("%d"), nLuggNum);
	pCmdMsg->m_strValues[2].Format(_T("%d"), nJobType);
	pCmdMsg->m_strValues[3].Format(_T("%d"), nDestPos);
	pCmdMsg->m_strValues[4].Format(_T("%d"), ucGenCode);
	pCmdMsg->m_strValues[5].Format(_T("%d"), nSize);
	m_pTrack->m_pEquipment->SetCmdMsg(pCmdMsg);
}

void CCvDlg::WriteWord(int nAddress, WORD wWord)
{
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);
	DEBUGER_ASSERT_VALID(m_pTrack->m_pEquipment != NULL);

	if (m_pTrack->m_pEquipment->IsConnect(0) == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCmdMsg::enCmdWriteWord;
	pCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeWriteWord);
	pCmdMsg->m_strValues[0].Format(_T("%d"), nAddress);
	pCmdMsg->m_strValues[1].Format(_T("%d"), wWord);
	m_pTrack->m_pEquipment->SetCmdMsg(pCmdMsg);
}

BOOL CCvDlg::UpdateTrackInfo(BOOL bSaveObject /* = TRUE */)
{
	if (bSaveObject)
	{
		if (!UpdateData(TRUE))
			return FALSE;

		m_nLuggNum	= (int)GetDlgItemInt(IDC_EDIT_LUGG_NUM);
		m_nJobType	= (int)m_cbxJobType.GetItemData(m_cbxJobType.GetCurSel());
		m_nDestPos	= m_nDestTrack;
//		m_ucGenCode	= (BYTE)m_cbxGenCode.GetItemData(m_cbxGenCode.GetCurSel());
//		m_nSize		= (int)GetDlgItemInt(IDC_EDIT_SIZE_CODE);
	}
	else
	{
		SetDlgItemInt(IDC_EDIT_LUGG_NUM, m_nLuggNum);
		SetJobType(m_nJobType);
		SetGenCode(m_ucGenCode);
		SetDestTrack(m_nDestPos);
		m_nDestTrack = m_nDestPos;
//		SetDlgItemInt(IDC_EDIT_SIZE_CODE, m_nSize);
//		SetDlgItemText(IDC_EDIT_SIZE, (m_nSize==0) ? "小" : "大");
		UpdateData(FALSE);
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCvDlg::OnButtonWrite() 
{

	DEBUGER_ASSERT_VALID(m_pTrack != NULL);

	if (UpdateTrackInfo(TRUE) == FALSE)
		return;

	if (AfxMessageBox(_T("트랙에 해당 데이터를 WRITE 하시겠습니까?"), MB_YESNO) != IDYES)
		return;

	WriteTrack(m_nLuggNum, m_nJobType, m_nDestPos, m_ucGenCode, m_nSize);
	CString strLog;
	strLog.Format(_T("%s 수동 데이터 쓰기 (작업번호=%d, 작업종류=%s, 완료위치=%d)"), 
		m_pTrack->GetTrackString(), m_nLuggNum, m_pTrack->GetJobTypeString(), m_nDestPos);
	m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_CV, strLog, _T("CCvDlg::OnButtonWrite"));
}

void CCvDlg::OnButtonCopy() 
{
	if (UpdateTrackInfo(TRUE) == FALSE)
		return;
	
	m_bClone = TRUE;
	m_btnPaste.EnableWindow(m_bClone);
}

void CCvDlg::OnButtonPaste() 
{
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);

	if (m_bClone == FALSE)
		return;

	m_bClone = FALSE;
	m_btnPaste.EnableWindow(m_bClone);

	WriteTrack(m_nLuggNum, m_nJobType, m_nDestPos, m_ucGenCode, m_nSize);
}

void CCvDlg::OnButtonCut() 
{
	if (UpdateTrackInfo(TRUE) == FALSE)
		return;

	m_bClone = TRUE;
	m_btnPaste.EnableWindow(m_bClone);

	WriteTrack(0, 0, 0, 0, 0);
}

void CCvDlg::OnButtonDelete() 
{
	if (AfxMessageBox(_T("트랙정보를 삭제하시겠습니까?"), MB_YESNO) != IDYES)
		return;

	WriteTrack(0, 0, 0, 0, 0);
}

void CCvDlg::OnButtonRefresh() 
{
	WindowProc(WM_USER_REFRESH_NOTIFY, 0, m_pTrack->m_nNumber);
	WindowProc(WM_USER_REFRESH_NOTIFY, 1, m_pTrack->m_nNumber);
}

void CCvDlg::OnCheckSuspend() 
{
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);

	if (m_pTrack->m_pEquipment->IsConnect(0) == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		UpdateData(FALSE);
		return;
	}

	if (!UpdateData())
		return;

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCmdMsg::enCmdTrackSuspend;
	pCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeTrackSuspend);
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_pTrack->m_nNumber);
	pCmdMsg->m_strValues[1].Format(_T("%d"), m_bSuspend);
//	pCmdMsg->m_strValues[2].Format(_T("%d"), m_pTrack->m_nCvPlcNum);
	m_pTrack->m_pEquipment->SetCmdMsg(pCmdMsg);
}

void CCvDlg::OnCheckDeadLock() 
{
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);

	if (m_pTrack->m_pEquipment->IsConnect(0) == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		UpdateData(FALSE);
		return;
	}

	if (!UpdateData())
		return;

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCmdMsg::enCmdTrackDeadLock;
	pCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeTrackDeadLock);	
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_pTrack->m_nNumber);
	pCmdMsg->m_strValues[1].Format(_T("%d"), m_bDeadLock);
	m_pTrack->m_pEquipment->SetCmdMsg(pCmdMsg);
}

void CCvDlg::OnSelchangeComboDestTrack() 
{
	GetDlgItem(IDC_EDIT_DEST_TRACK)->EnableWindow(m_cbxDestTrack.GetCurSel() == 0);
	m_nDestTrack = (int)m_cbxDestTrack.GetItemData(m_cbxDestTrack.GetCurSel());
	UpdateData(FALSE);
}

//=============================================================================
//	Desc	: 화재발생 알람 수동 발생
//	Date	:
//	Update	: 
//=============================================================================
void CCvDlg::OnClickedFireAlarm() 
{
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);
	DEBUGER_ASSERT_VALID(m_pTrack->m_pEquipment != NULL);

	if (m_pTrack->m_pEquipment->IsConnect(0) == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCvCmdMsg::enCmdFireAlarm;
	pCmdMsg->m_strValues.SetSize(CCvCmdMsg::enSizeFireAlarm);
	pCmdMsg->m_strValues[0].Format(_T("%d"), 457);
	pCmdMsg->m_strValues[1].Format(_T("%d"), 1);
	m_pTrack->m_pEquipment->SetCmdMsg(pCmdMsg);
}

//=============================================================================
//	Desc	: 화재발생 후 알람 수동 리셋
//	Date	:
//	Update	: 
//=============================================================================
void CCvDlg::OnClickedResetFireAlarm() 
{
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);
	DEBUGER_ASSERT_VALID(m_pTrack->m_pEquipment != NULL);

	if (m_pTrack->m_pEquipment->IsConnect(0) == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		return;
	}

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCvCmdMsg::enCmdFireAlarm;
	pCmdMsg->m_strValues.SetSize(CCvCmdMsg::enSizeFireAlarm);
	pCmdMsg->m_strValues[0].Format(_T("%d"), 457);
	pCmdMsg->m_strValues[1].Format(_T("%d"), 0);
	m_pTrack->m_pEquipment->SetCmdMsg(pCmdMsg);
}
