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
	
	m_nLuggNum = 0;
	m_nJobType = 0;
	m_nDestPos = 0;
	m_ucGenCode = 0;
	m_bClone = FALSE;

	//{{AFX_DATA_INIT(CCvDlg)
	m_bSuspend = FALSE;
	m_bDeadLock = FALSE;
	m_nDestTrack = 0;
	m_bReqError = FALSE;
	//}}AFX_DATA_INIT
}


void CCvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCvDlg)
	DDX_Control(pDX, IDC_BUTTON_STATUS_STO_STATION, m_btnStatusStoStation);
	DDX_Control(pDX, IDC_BUTTON_STATUS_SC_STO_HS, m_btnStatusScStoHS);
	DDX_Control(pDX, IDC_BUTTON_STATUS_SC_RET_HS, m_btnStatusScRetHS);
	DDX_Control(pDX, IDC_BUTTON_STATUS_RTV_DEPART_HS, m_btnStatusRtvDepartHS);
	DDX_Control(pDX, IDC_BUTTON_STATUS_RTV_ARRIVE_HS, m_btnStatusRtvArriveHS);
	DDX_Control(pDX, IDC_BUTTON_STATUS_RET_STATION, m_btnStatusRetStation);
	DDX_Control(pDX, IDC_BUTTON_STATUS_LIFTER_DEPART_HS, m_btnStatusLfDepartHS);
	DDX_Control(pDX, IDC_BUTTON_STATUS_LIFTER_ARRIVE_HS, m_btnStatusLfArriveHS);
	DDX_Control(pDX, IDC_BUTTON_STATUS_LGV_DEPART_HS, m_btnStatusLgvDepartHS);
	DDX_Control(pDX, IDC_BUTTON_STATUS_LGV_ARRIVE_HS, m_btnStatusLgvArriveHS);
	DDX_Control(pDX, IDC_BUTTON_STATUS_LGV_OUT_REQUEST, m_btnStatusLgvOutStatus);
	DDX_Control(pDX, IDC_BUTTON_STATUS_LGV_IN_REQUEST, m_btnStatusLgvInRequest);
	DDX_Control(pDX, IDC_BUTTON_STATUS_CV_CHECK_POS, m_btnStatusCvCheckPos);
	DDX_Control(pDX, IDC_BUTTON_SENSOR_PRODUCT1, m_btnSensorProduct1);
	DDX_Control(pDX, IDC_BUTTON_SENSOR_SC_INTERLOCK, m_btnSensorScInterlock);
	DDX_Control(pDX, IDC_BUTTON_SENSOR_RTV_INTERLOCK, m_btnSensorRtvInterlock);
	DDX_Control(pDX, IDC_BUTTON_SENSOR_LGV_INTERLOCK, m_btnSensorLgvInterlock);
	DDX_Control(pDX, IDC_BUTTON_SENSOR_LIFTER_INTERLOCK, m_btnSensorLfInterlock);
	DDX_Control(pDX, IDC_COMBO_JOB_TYPE, m_cbxJobType);
	DDX_Control(pDX, IDC_COMBO_DEST_TRACK, m_cbxDestTrack);
	DDX_Control(pDX, IDC_COMBO_GEN_CODE, m_cbxGenCode);
	DDX_Control(pDX, IDC_BUTTON_WRITE, m_btnWrite);
	DDX_Control(pDX, IDC_BUTTON_STATUS_REPORT, m_btnStatusReport);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BUTTON_PASTE, m_btnPaste);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_CUT, m_btnCut);
	DDX_Control(pDX, IDC_BUTTON_COPY, m_btnCopy);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Check(pDX, IDC_CHECK_SUSPEND, m_bSuspend);
	DDX_Check(pDX, IDC_CHECK_DEADLOCK, m_bDeadLock);
	DDX_Text(pDX, IDC_EDIT_DEST_TRACK, m_nDestTrack);
	DDX_Check(pDX, IDC_CHECK_ERROR, m_bReqError);
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
	ON_BN_CLICKED(IDC_BUTTON_STATUS_REPORT, OnButtonStatusReport)
	ON_BN_CLICKED(IDC_CHECK_SUSPEND, OnCheckSuspend)
	ON_BN_CLICKED(IDC_CHECK_DEADLOCK, OnCheckDeadLock)
	ON_CBN_SELCHANGE(IDC_COMBO_DEST_TRACK, OnSelchangeComboDestTrack)
//	ON_BN_CLICKED(IDC_BUTTON_WRITE2, OnButtonWrite2)
	ON_BN_CLICKED(IDC_CHECK_ERROR, OnCheckError)
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

	CDialog::OnInitDialog();

	SetDlgItemText(IDC_EDIT_TRACK_NUM, m_pTrack->GetTrackString());
	GetDlgItem(IDC_CHECK_SUSPEND)->EnableWindow(m_pTrack->IsNotInvokeTrack() == FALSE);
	GetDlgItem(IDC_CHECK_ERROR)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_DEADLOCK)->EnableWindow(FALSE);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	m_cbxJobType.ResetContent();
	m_cbxJobType.AddString(_T("없음"));
	m_cbxJobType.SetItemData(0, 0);
	CLib::SetComboBoxJobType(m_cbxJobType);

	//int i=1; 
	m_cbxGenCode.ResetContent();
	m_cbxGenCode.AddString(_T("없음"));
	m_cbxGenCode.SetItemData(0, 0);
	CLib::SetComboBoxGenCode(m_cbxGenCode);

	m_cbxDestTrack.ResetContent();
	m_cbxDestTrack.AddString(_T("없음"));
	m_cbxDestTrack.SetItemData(0, 0);

	CString strTemp;
	int nSelectPlcNum = m_pTrack->m_nNumber / 1000;
	int nDestListPlcNum = 0;
	CStationInfo* pStation = NULL;
	for (int i=0; i<m_pDoc->m_pStationInfos.GetSize(); ++i)
	{
		pStation = m_pDoc->m_pStationInfos[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		strTemp.Format(_T("[%d] %s"), pStation->GetTrackDestination(), pStation->m_strName);
		switch (pStation->m_enKind)
		{
		case CStationInfo::enScStation:
//			strTemp.Format("[%d] %s", pStation->GetTrackDestination(), pStation->m_strName);
			m_cbxDestTrack.AddString(strTemp);
			m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, pStation->GetTrackDestination());
			break;

		case CStationInfo::enRetStation:
		case CStationInfo::enArvStation:
			nDestListPlcNum = pStation->m_pTrack->m_nNumber / 1000;
//			strTemp.Format("[%d] %s", pStation->GetTrackDestination(), pStation->m_strName);
			
			if (nSelectPlcNum == nDestListPlcNum)
			{
//				if (nSelectPlcNum == 3)
//				{
//					// 같은 Part 일때 
//					int nSelectPartNum = int(m_pTrack->m_nNumber / 100) % 10;
//					int nDestListPartNum = int(pStation->m_pTrack->m_nNumber / 100) % 10;
//
//					if (nSelectPartNum == nDestListPartNum)
//					{
//						m_cbxDestTrack.AddString(strTemp);
//						m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, pStation->m_pTrack->m_nNumber);
//					}
//					else if(pStation->m_pTrack->m_nNumber == 3502)
//					{
//						m_cbxDestTrack.AddString(strTemp);
//						m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, pStation->m_pTrack->m_nNumber);
//					}
//				}
//				else
//				{
					m_cbxDestTrack.AddString(strTemp);
					m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, pStation->m_pTrack->m_nNumber);
//				}
			}
			break;
		}
	}

// 	m_cbxDestTrack.AddString(_T("TR301"));
// 	m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, 301);
// 	m_cbxDestTrack.AddString(_T("TR003"));
// 	m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, 3);
// 	m_cbxDestTrack.AddString(_T("TR001"));
// 	m_cbxDestTrack.SetItemData(m_cbxDestTrack.GetCount()-1, 1);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int w = 20, h = 20;
	INIT_BUTTON(m_btnWrite, IDI_SAVE, w, h, _T("트랙정보 쓰기"));
	INIT_BUTTON(m_btnCopy, IDI_ICON_COPY, w, h, _T("트랙정보 복사"));
	INIT_BUTTON(m_btnPaste, IDI_ICON_PASTE, w, h, _T("트랙정보 붙이기"));
	INIT_BUTTON(m_btnCut, IDI_ICON_CUT, w, h, _T("트랙정보 자르기"));
	INIT_BUTTON(m_btnDelete, IDI_CANCEL, w, h, _T("트랙정보 삭제"));
	INIT_BUTTON(m_btnRefresh, IDI_UPDATE, w, h, _T("트랙정보 갱신"));
	INIT_BUTTON(m_btnStatusReport, IDI_RUN, w, h, _T("상태보고"));
	INIT_BUTTON(m_btnOK, IDI_EXIT, w, h, _T("닫기"));

//	// SC 입고HS는 안전상에 이유로 WRITE, PASTE, COPY, CUT 금지
//	BOOL bScStoHS = m_pTrack->GetScStoHS() ? TRUE : FALSE;
//	m_btnCut.EnableWindow(!bScStoHS);
//	m_btnCopy.EnableWindow(!bScStoHS);
//	m_btnWrite.EnableWindow(!bScStoHS);
//	m_btnPaste.EnableWindow(m_bClone && !bScStoHS);
//	m_btnStatusReport.EnableWindow(m_pTrack->m_bReportTrack);
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch(m_pTrack->m_nNumber) 
	{
	case 3503:			GetDlgItem(IDC_CHECK_ERROR)->EnableWindow(TRUE);			GetDlgItem(IDC_BUTTON_STATUS_REPORT)->EnableWindow(TRUE);		break;		
	case 3117:
	case 3217:
	case 3317:			GetDlgItem(IDC_CHECK_ERROR)->EnableWindow(TRUE);			GetDlgItem(IDC_CHECK_DEADLOCK)->EnableWindow(TRUE);	// break;		// break하지 않음!!
	case 3119:
	case 3219:
	case 3319:
	case 3411:	
	case 2203:
	case 2214:
	case 2103:	
	case 2210:
	case 2229:
	case 2236:
	case 2119:
	case 2118:
	case 1102:
	case 1103:
	case 1109:		
	case 1205:
	case 3111:
	case 3112:
	case 3211:
	case 3212:
	case 3311:
	case 3312:
	case 3404:
	case 3409:
	case 3502:			GetDlgItem(IDC_BUTTON_STATUS_REPORT)->EnableWindow(TRUE);		break;
	default:			GetDlgItem(IDC_BUTTON_STATUS_REPORT)->EnableWindow(FALSE);		break;
	}
	
	WindowProc(WM_USER_REFRESH_NOTIFY, 0, 0);
	WindowProc(WM_USER_REFRESH_NOTIFY, 1, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCvDlg::SetEtcValue()
{
	CString strTemp1 = "", strTemp2 = "기타정보 :", strTemp3 = "작업요청", strTemp4 = "", strTemp5 = "무게";
	if (m_pTrack->m_nNumber == 2210 || 
		m_pTrack->m_nNumber == 2229 || 
		m_pTrack->m_nNumber == 2236 || 
		m_pTrack->m_nNumber == 1103 || 
		m_pTrack->m_nNumber == 2203 || 
		m_pTrack->m_nNumber == 2119 || 
		m_pTrack->m_nNumber == 2118 )
	{
		if (m_pTrack->m_strEtcValue == "" || m_pTrack->m_strEtcValue == "0")
		{
			strTemp1 = "Before Produce...";
		}
		else
		{
			strTemp1 = m_pTrack->m_strEtcValue;
		}

		strTemp2 = "PLT 정보 :";
		GetDlgItem(IDC_EDIT_REQ_ERR)->ShowWindow(FALSE);
	}
	else if (m_pTrack->m_nNumber == 3117 || m_pTrack->m_nNumber == 3217 || m_pTrack->m_nNumber == 3317)
	{
		if (m_pTrack->m_strEtcValue == "" || m_pTrack->m_strEtcValue == "0")
		{
			strTemp1 = "Before Weight Checking!";
		}
		else
		{
			strTemp1 = m_pTrack->m_strEtcValue;
		}

		strTemp2 = "중량정보 :";
		strTemp3 = "WC에러보고";
		strTemp4 = CConvert::ToString(m_pTrack->m_wReserved3);

		GetDlgItem(IDC_EDIT_REQ_ERR)->ShowWindow(TRUE);
	}
	else if (m_pTrack->m_nNumber == 3503)
	{
//		if (m_pTrack->m_strEtcValue == "" || m_pTrack->m_strEtcValue == "0")
//		{
//			strTemp1 = "Before Weight Checking!";
//		}
//		else
//		{
//			strTemp1 = m_pTrack->m_strEtcValue;
//		}

//		strTemp2 = "중량정보 :";
		strTemp3 = "SIZE보고";
//		strTemp4 = CConvert::ToString(m_pTrack->m_wReserved3);
		strTemp5 = "에러해제";				SetDlgItemText(IDC_CHECK_ERROR, strTemp5);
		strTemp5 = "SISE CHECKER";			// 밑에서 적용됨		

		GetDlgItem(IDC_EDIT_REQ_ERR)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_ERROR)->ShowWindow(TRUE);
	}
	else
		GetDlgItem(IDC_EDIT_REQ_ERR)->ShowWindow(TRUE);

	SetDlgItemText(IDC_EDIT_SIZE, strTemp1);
	SetDlgItemText(IDC_STATIC_ETC, strTemp2);
	SetDlgItemText(IDC_BUTTON_STATUS_REPORT, strTemp3);
	SetDlgItemText(IDC_EDIT_REQ_ERR, strTemp4);
	SetDlgItemText(IDC_STATIC_GROUP, strTemp5);	
}

BOOL CCvDlg::DestroyWindow() 
{
	m_pTrack->m_pEquipment->m_pEquipDlg = NULL;
	
	return CDialog::DestroyWindow();
}

LRESULT CCvDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_USER_REFRESH_NOTIFY:
		{
			if (wParam)
			{
				SetDlgItemInt(IDC_EDIT_LUGG_NUM, m_pTrack->m_nLuggNum);
				SetJobType(m_pTrack->m_nJobType);
				SetGenCode(m_pTrack->m_ucGenCode);
				SetDestTrack(m_pTrack->m_nDestPos);
				SetEtcValue();

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
//=====================================================================================================================================
//				//출고대 끝단 정보 표현 
//=====================================================================================================================================
//				// 원본 
//				SetDlgItemText(IDC_EDIT_JOB_START_STN, pJobItem ? pJobItem->GetStartStationString() : _T(""));
//				SetDlgItemText(IDC_EDIT_JOB_DEST_STN, pJobItem ? pJobItem->GetDestStationString() : _T(""));
//				SetDlgItemText(IDC_EDIT_JOB_STATUS, pJobItem ? pJobItem->GetJobStatusString() : _T(""));
//				SetDlgItemText(IDC_EDIT_JOB_PRODUCT, pJobItem ? pJobItem->m_strWeight : _T(""));
//				SetDlgItemText(IDC_EDIT_JOB_PRODUCT3, pJobItem ? pJobItem->m_strProductID : _T(""));	
//				SetDlgItemText(IDC_EDIT_CV_STATUS, m_pTrack->GetErrorString());
//				
//				// 수정본 
				int nIndex = (m_pTrack->m_nLuggNum > 0 && m_pTrack->m_nLuggNum < 10000) ? m_pTrack->m_nLuggNum : 0;
				SetDlgItemText(IDC_EDIT_JOB_START_STN, pJobItem ? pJobItem->GetStartStationString() : m_pDoc->m_strDepartPos[nIndex]);
				SetDlgItemText(IDC_EDIT_JOB_DEST_STN, pJobItem ? pJobItem->GetDestStationString()	: m_pDoc->m_strArrivePos[nIndex]);
				SetDlgItemText(IDC_EDIT_JOB_PRODUCT, pJobItem ? pJobItem->m_strWeight				: m_pDoc->m_strWeightVal[nIndex]);
				SetDlgItemText(IDC_EDIT_JOB_PRODUCT3, pJobItem ? pJobItem->m_strProductID			: m_pDoc->m_strProductID[nIndex]);	
				SetDlgItemText(IDC_EDIT_JOB_STATUS, pJobItem ? pJobItem->GetJobStatusString()		: _T(""));
				SetDlgItemText(IDC_EDIT_CV_STATUS, m_pTrack->GetErrorString());
//-------------------------------------------------------------------------------------------------------------------------------------

				m_btnSensorProduct1.SetIcon(m_pTrack->IsProductSensing() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnSensorScInterlock.SetIcon(m_pTrack->m_bDeadLock ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnSensorRtvInterlock.SetIcon(m_pTrack->IsSensorRtvInterlock() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnSensorLgvInterlock.SetIcon(m_pTrack->IsSensorLgvInterlock() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnSensorLfInterlock.SetIcon(m_pTrack->IsSensorLfInterlock() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);

				m_btnStatusCvCheckPos.SetIcon(m_pTrack->IsCvCheckPosReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusLgvInRequest.SetIcon(m_pTrack->IsLgvInRequest() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusLgvOutStatus.SetIcon(m_pTrack->IsLgvOutRequest() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusStoStation.SetIcon(m_pTrack->IsStoStationReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusRetStation.SetIcon(m_pTrack->IsRetStationReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusScStoHS.SetIcon(m_pTrack->IsScStoHsReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusScRetHS.SetIcon(m_pTrack->IsScRetHsReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusRtvDepartHS.SetIcon(m_pTrack->IsRtvDepartHsReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusRtvArriveHS.SetIcon(m_pTrack->IsRtvArriveHsReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusLfDepartHS.SetIcon(m_pTrack->IsLfDepartHsReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusLfArriveHS.SetIcon(m_pTrack->IsLfArriveHsReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusLgvDepartHS.SetIcon(m_pTrack->IsLgvDepartHsReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
				m_btnStatusLgvArriveHS.SetIcon(m_pTrack->IsLgvArriveHsReady() ? IDI_ICON_LEDON : IDI_ICON_LEDOFF);
			}
		}
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
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

	if (m_pTrack->m_pEquipment->IsConnect() == FALSE)
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

	if (m_pTrack->m_pEquipment->IsConnect() == FALSE)
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
// 		if (m_cbxJobType.GetCurSel() <= 0)
// 		{
// 			AfxMessageBox(_T("작업구분을 선택해주세요."));
// 			return FALSE;
// 		}
// 
// 		if (m_cbxDestTrack.GetCurSel() <= 0)
// 		{
// 			AfxMessageBox(_T("도착위치를 선택해주세요."));
// 			return FALSE;
// 		}
// 
// 		if (m_cbxGenCode.GetCurSel() <= 0)
// 		{
// 			AfxMessageBox(_T("GEN 정보를 선택해주세요."));
// 			return FALSE;
// 		}

		if (!UpdateData(TRUE))
			return FALSE;

		m_nLuggNum	= (int)GetDlgItemInt(IDC_EDIT_LUGG_NUM);
		m_nJobType	= (int)m_cbxJobType.GetItemData(m_cbxJobType.GetCurSel());
		m_nDestPos	= m_nDestTrack;
		m_ucGenCode	= (BYTE)m_cbxGenCode.GetItemData(m_cbxGenCode.GetCurSel());
		m_nSize		= (int)GetDlgItemInt(IDC_EDIT_SIZE_CODE);
	}
	else
	{
		SetDlgItemInt(IDC_EDIT_LUGG_NUM, m_nLuggNum);
		SetJobType(m_nJobType);
		SetGenCode(m_ucGenCode);
		SetDestTrack(m_nDestPos);
		m_nDestTrack = m_nDestPos;
		SetDlgItemInt(IDC_EDIT_SIZE_CODE, m_nSize);
		SetDlgItemText(IDC_EDIT_SIZE, (m_nSize==0) ? _T("小") : _T("大"));
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

	if(m_pDoc !=NULL && m_pTrack != NULL)
	{
		CString strLog;
		strLog.Format(_T("트랙 잘라내기 [%d]"), m_pTrack->m_nNumber);
		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_CV, strLog, _T("CCvDlg::OnButtonCut"));
	}	

	WriteTrack(0, 0, 0, 0, 0);
}

void CCvDlg::OnButtonDelete() 
{
	if (AfxMessageBox(_T("트랙정보를 삭제하시겠습니까?"), MB_YESNO) != IDYES)
		return;

	if(m_pDoc !=NULL && m_pTrack != NULL)
	{
		CString strLog;
		strLog.Format(_T("트랙 삭제 [%d]"), m_pTrack->m_nNumber);
		m_pDoc->WriteLog(LOG_TYPE_DEBUG, LOG_POS_CV, strLog, _T("CCvDlg::OnButtonDelete"));
	}

	WriteTrack(0, 0, 0, 0, 0);	
}

void CCvDlg::OnButtonRefresh() 
{
	WindowProc(WM_USER_REFRESH_NOTIFY, 0, 0);
	WindowProc(WM_USER_REFRESH_NOTIFY, 1, 0);
}

// 작업요청 버튼으로 변경
void CCvDlg::OnButtonStatusReport() 
{
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);

	CString strTemp;
	CString strJobType;
	int nJobType = 0;
	switch(m_pTrack->m_nNumber) 
	{
	case 2210:
	case 2229:
	case 2236:
	case 2119:
	case 2118:
	case 2203:
	case 1103:
	case 1102:
	case 1109:		nJobType = 1;		strJobType = _T("입고 작업요청");			break;
	case 1205:
	case 3111:
	case 3112:
	case 3211:
	case 3212:
	case 3311:
	case 3312:
	case 3404:
	case 3409:
	case 3502:		nJobType = 2;		strJobType = _T("출고 작업요청");			break;
	case 3119:
	case 3219:
	case 3319:
	case 3411:		nJobType = 3;		strJobType = _T("공파레트 입고요청");		break;
	case 2214:
	case 2103:		nJobType = 4;		strJobType = _T("공파레트 출고요청");		break;
	case 3117:
	case 3217:
	case 3317:		nJobType = 5;		strJobType = _T("중량이상 에러 보고");		break;
	case 3503:		nJobType = 6;		strJobType = _T("SIZE CHECKER 에러 해제");	break;
	default:																		return;
	}

	strTemp.Format(_T("%s  하시겠습니까?"), strJobType);

	if (AfxMessageBox(strTemp, MB_YESNO) != IDYES)
		return;

	if ((m_pDoc->m_pHostCl == NULL) || (m_pDoc->m_pHostCl->IsConnect() == FALSE))
	{
		AfxMessageBox(_T("IMS CLIENT와 연결이 끊어졌습니다. 재접속후 시도해주세요!"));
		return;
	}

	if (nJobType == 1)
	{
		CStationInfo* pStation = m_pTrack->GetStoStation();
		if (pStation == NULL)
			return;

		m_pDoc->m_pHostCl->StoJobRequest(_ttoi(pStation->m_strID), m_pTrack->m_nJobQty, m_pTrack->m_strEtcValue, TRUE);

	}
	else if (nJobType == 2)
	{
		CStationInfo* pStation = m_pTrack->GetArvStation();
		if (pStation == NULL)
			return;

		m_pDoc->m_pHostCl->RetJobRequest(_ttoi(pStation->m_strID), m_pTrack->m_nLuggNum, TRUE);
	}
	else if (nJobType == 3)
	{
		CStationInfo* pStation = m_pTrack->GetArvStation();
		if (pStation == NULL)
			return;

		m_pDoc->m_pHostCl->JobRequest(_ttoi(pStation->m_strID), 1, TRUE);
	}
	else if (nJobType == 4)
	{
		CStationInfo* pStation = m_pTrack->GetRetStation();
		if (pStation == NULL)
			return;

		m_pDoc->m_pHostCl->JobRequest(_ttoi(pStation->m_strID), 2, TRUE);
	}
	else if (nJobType == 5 || nJobType == 6)
	{
		CStationInfo* pStation = m_pTrack->GetArvStation();
		if (pStation == NULL)
			return;

		m_pDoc->m_pHostCl->Error(1, m_pTrack->m_nNumber, 5, m_pTrack->m_wErrorCode, m_pTrack->m_nLuggNum, "", m_pTrack->m_strEtcValue, TRUE);
	}

//	m_pTrack->StatusReport(TRUE);
}

void CCvDlg::OnCheckSuspend() 
{
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);

	if (m_pTrack->m_pEquipment->IsConnect() == FALSE)
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
	m_pTrack->m_pEquipment->SetCmdMsg(pCmdMsg);
}

void CCvDlg::OnCheckDeadLock() 
{
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);

	if (m_pTrack->m_pEquipment->IsConnect() == FALSE)
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
/*
void CCvDlg::OnButtonWrite2() 
{
	// TODO: Add your control notification handler code here
}
*/
void CCvDlg::OnCheckError() 
{
	// TODO: Add your control notification handler code here
	DEBUGER_ASSERT_VALID(m_pTrack != NULL);

	if (m_pTrack->m_pEquipment->IsConnect() == FALSE)
	{
		AfxMessageBox(_T("통신이 연결되지 않았습니다."));
		UpdateData(FALSE);
		return;
	}

	if (!UpdateData())
		return;

	CCmdMsg* pCmdMsg = new CCmdMsg;
	DEBUGER_ASSERT_VALID(pCmdMsg != NULL);
	pCmdMsg->m_nCommand = CCmdMsg::enCmdTrackDeadLockCheck;
	pCmdMsg->m_strValues.SetSize(CCmdMsg::enSizeTrackDeadLockCheck);	
	pCmdMsg->m_strValues[0].Format(_T("%d"), m_pTrack->m_nNumber);
	pCmdMsg->m_strValues[1].Format(_T("%d"), m_bReqError);	
	m_pTrack->m_pEquipment->SetCmdMsg(pCmdMsg);
}
