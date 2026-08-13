// JobOfflineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "JobOfflineDlg.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJobOfflineDlg dialog


CJobOfflineDlg::CJobOfflineDlg(CEcsDoc* pDoc, CWnd* pParent /* = NULL */)
	: CDialog(CJobOfflineDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pDoc->m_pJob != NULL);
	DEBUGER_ASSERT_VALID(m_pDoc->m_pLog != NULL);

	//{{AFX_DATA_INIT(CJobOfflineDlg)
	m_bAutoJob = FALSE;
	//}}AFX_DATA_INIT
}


void CJobOfflineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJobOfflineDlg)
	DDX_Control(pDX, IDC_STATIC_QNT, m_stcQty);
	DDX_Control(pDX, IDC_STATIC_QTY_GROUP, m_stcQtyGroup);
	DDX_Control(pDX, IDC_COMBO_GEN_CODE, m_cbxGenCode);		// 수량으로 사용 
	DDX_Control(pDX, IDC_TAB_JOB_TYPE, m_tabJobType);
	DDX_Control(pDX, IDC_COMBO_START_WAREHOUSE, m_cbxStartWH);
	DDX_Control(pDX, IDC_COMBO_DEST_WAREHOUSE, m_cbxDestWH);
	DDX_Control(pDX, IDC_COMBO_START_STATION, m_cbxStartStn);
	DDX_Control(pDX, IDC_COMBO_DEST_STATION, m_cbxDestStn);
	DDX_Control(pDX, IDC_EDIT_START_LOCATION, m_edtStartLoc);
	DDX_Control(pDX, IDC_EDIT_DEST_LOCATION, m_edtDestLoc);
	DDX_Control(pDX, IDC_BUTTON_CREATE, m_btnCreate);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Check(pDX, IDC_CHECK_AUTO, m_bAutoJob);
	DDX_Check(pDX, IDC_CHECK_BIG, m_bBig);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJobOfflineDlg, CDialog)
	//{{AFX_MSG_MAP(CJobOfflineDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_JOB_TYPE, OnSelchangeTabJobType)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, OnButtonCreate)
	ON_CBN_SELCHANGE(IDC_COMBO_START_WAREHOUSE, OnSelchangeComboStartWarehouse)
	ON_CBN_SELCHANGE(IDC_COMBO_DEST_WAREHOUSE, OnSelchangeComboDestWarehouse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJobOfflineDlg message handlers

BOOL CJobOfflineDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int w = 32, h = 32;
	INIT_BUTTON(m_btnCreate, IDI_RUN, w, h, _T("반자동작업 생성"));
	INIT_BUTTON(m_btnOK, IDI_EXIT, w, h, _T("창닫기"));

	CImageList imageList;
	imageList.Create(32, 32, TRUE, 1, 1);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_CLASSES2));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_CLASSES3));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_CLASSES4));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_CLASSES5));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_CLASSES6));
	m_tabJobType.SetImageList(&imageList);
	imageList.Detach();
	
	///////////////////////////////////////////////////////////////////////////////////////

	m_tabJobType.InsertItem(0, _T("입고"), 0);
	m_tabJobType.InsertItem(1, _T("출고"), 1);
	m_tabJobType.InsertItem(2, _T("랙투랙"), 2);
	m_tabJobType.InsertItem(3, _T("이동"), 3);
	m_tabJobType.InsertItem(4, _T("호기이동"), 4);

	///////////////////////////////////////////////////////////////////////////////////////

	CLib::SetComboBoxWarehouse(m_cbxStartWH);
	m_cbxStartWH.SetCurSel(0);
	UpdateStationInfo(m_cbxStartWH.GetItemData(m_cbxStartWH.GetCurSel()), m_cbxStartStn);

	CLib::SetComboBoxWarehouse(m_cbxDestWH);
	m_cbxDestWH.SetCurSel(0);
	UpdateStationInfo(m_cbxDestWH.GetItemData(m_cbxDestWH.GetCurSel()), m_cbxDestStn);

	
//	m_cbxGenCode.AddString(_T("없음"));
//	m_cbxGenCode.SetItemData(0, 0);
	CLib::SetComboBoxQty(m_cbxGenCode);
	m_cbxGenCode.SetCurSel(0);

	///////////////////////////////////////////////////////////////////////////////////////

	m_edtStartLoc.LimitText(CLib::enLengthLocation);
	m_edtDestLoc.LimitText(CLib::enLengthLocation);

	LRESULT lResult;
	OnSelchangeTabJobType(NULL, &lResult);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJobOfflineDlg::OnSelchangeTabJobType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_cbxGenCode.ResetContent();
	CLib::SetComboBoxQty(m_cbxGenCode);
	m_cbxGenCode.SetCurSel(0);
	m_stcQty.EnableWindow(FALSE);
	m_stcQtyGroup.EnableWindow(FALSE);
	m_cbxGenCode.EnableWindow(FALSE);		// 수량으로 사용 
	
	switch (m_tabJobType.GetCurSel())
	{
	case enSemiSto:
		{
			m_cbxStartStn.EnableWindow(TRUE);
			m_cbxDestStn.EnableWindow(FALSE);

			m_edtStartLoc.EnableWindow(FALSE);
			m_edtDestLoc.EnableWindow(TRUE);
		}
		break;
	case enSemiRet:
		{
			m_cbxStartStn.EnableWindow(FALSE);
			m_cbxDestStn.EnableWindow(TRUE);

			m_edtStartLoc.EnableWindow(TRUE);
			m_edtDestLoc.EnableWindow(FALSE);
		}
		break;
	case enSemiR2R:
		{
			m_cbxStartStn.EnableWindow(FALSE);
			m_cbxDestStn.EnableWindow(FALSE);

			m_edtStartLoc.EnableWindow(TRUE);
			m_edtDestLoc.EnableWindow(TRUE);
		}
		break;
	case enSemiMove:
		{
			m_cbxStartStn.EnableWindow(TRUE);
			m_cbxDestStn.EnableWindow(TRUE);

			m_edtStartLoc.EnableWindow(FALSE);
			m_edtDestLoc.EnableWindow(FALSE);

			// 이동일때 수량을 입력가능하게 함!
			m_stcQty.EnableWindow(TRUE);
			m_stcQtyGroup.EnableWindow(TRUE);
			m_cbxGenCode.EnableWindow(TRUE);		// 수량으로 사용 
		}
		break;

	case enSemiW2W:
		{
			m_cbxStartStn.EnableWindow(FALSE);
			m_cbxDestStn.EnableWindow(FALSE);

			m_edtStartLoc.EnableWindow(TRUE);
			m_edtDestLoc.EnableWindow(TRUE);
		}
		break;

	default:
		DEBUGER_ASSERT_VALID(FALSE);
		break;
	}

	*pResult = 0;
}

void CJobOfflineDlg::OnButtonCreate() 
{
	int nLuggNum = m_pDoc->m_pJob->GenerateOfflineLuggNum();
	if (nLuggNum == 0)
	{
		AfxMessageBox(_T("작업번호 생성 실패!"));
		return;
	}

	UpdateData(TRUE);
	CJobItem oJobItem(m_pDoc);
	CJobItem* pJobItem = NULL;
	CStationInfo* pStartStation = NULL;
	CStationInfo* pDestStation = NULL;
	CString strLog, strStartLoc, strDestLoc;
	m_edtStartLoc.GetWindowText(strStartLoc);
	m_edtDestLoc.GetWindowText(strDestLoc);

	if (m_cbxStartWH.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("출발창고를 선택해주세요!"));
		return;
	}
	int nStartWH = m_cbxStartWH.GetItemData(m_cbxStartWH.GetCurSel());

	if (m_cbxDestWH.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("도착창고를 선택해주세요!"));
		return;
	}
	int nDestWH = m_cbxDestWH.GetItemData(m_cbxDestWH.GetCurSel());

	switch (m_tabJobType.GetCurSel())
	{
	case enSemiSto:
		{
			if (m_cbxStartStn.GetCurSel() == CB_ERR)
			{
				AfxMessageBox(_T("출발지를 선택해주세요!"));
				return;
			}

			if ((CLib::IsValidLocation(nDestWH, strDestLoc) == FALSE) || (CLib::GetStackerNum(nDestWH, strDestLoc) == 0))
			{
				AfxMessageBox(_T("LOCATION을 확인해주세요!"));
				return;
			}

			if (CLib::IsDisableLocation(nDestWH, strDestLoc))
			{
				AfxMessageBox(_T("금지된 LOCATION 입니다. 확인해주세요!"));
				return;
			}

			pStartStation = (CStationInfo*)m_cbxStartStn.GetItemDataPtr(m_cbxStartStn.GetCurSel());
			pDestStation = m_pDoc->GetScStationInfo(nDestWH, strDestLoc);
			DEBUGER_ASSERT_VALID(pStartStation != NULL);
			DEBUGER_ASSERT_VALID(pDestStation != NULL);

			if (pStartStation->m_pTrack->IsStoStationReady() == FALSE)
			{
				if (AfxMessageBox(_T("출발지 작업대 상태가 준비되지 않았습니다. 계속 진행하시겠습니까?"), MB_OKCANCEL) != IDOK)
					return;
			}

			if (pJobItem = m_pDoc->m_pJob->FetchReservedJobByStartStation(pStartStation->m_strID))
			{
				if (AfxMessageBox(_T("해당 출발지에 진행중인 작업이 존재합니다. 계속 진행하시겠습니까?\n\n") + pJobItem->GetLogString(), MB_OKCANCEL) != IDOK)
					return;
			}

			// TEST
//			nLuggNum = 2003;
//			m_bAutoJob = TRUE;

			oJobItem.m_nLuggNum		= nLuggNum;
			oJobItem.m_nJobType		= (m_bAutoJob) ? enJobTypeAutoSto : enJobTypeSemiSto;
			oJobItem.m_nStartWH		= nStartWH;
			oJobItem.m_strStartStn	= pStartStation->m_strID;
			oJobItem.m_nDestWH		= nDestWH;
			oJobItem.m_strDestStn	= pDestStation->m_strID;
			oJobItem.m_strDestLoc	= strDestLoc;

			oJobItem.m_nSize		= 0;			// 제품 크기
//			// 입고시 도착 Location의 Level이 10 이상일경우 Size가 1로 들어갈것!
			if((oJobItem.m_strDestLoc != "") && (_ttoi(oJobItem.m_strDestLoc.Mid(5,2)) > 10))
				oJobItem.m_nSize	= 1;
		}
		break;

	case enSemiRet:
		{
			if ((CLib::IsValidLocation(nStartWH, strStartLoc) == FALSE) || (CLib::GetStackerNum(nStartWH, strStartLoc) == 0))
			{
				AfxMessageBox(_T("LOCATION을 확인해주세요!"));
				return;
			}

			if (CLib::IsDisableLocation(nStartWH, strStartLoc))
			{
				AfxMessageBox(_T("금지된 LOCATION 입니다. 확인해주세요!"));
				return;
			}

			if (m_cbxDestStn.GetCurSel() == CB_ERR)
			{
				AfxMessageBox(_T("도착지를 선택해주세요!"));
				return;
			}

			pStartStation = m_pDoc->GetScStationInfo(nStartWH, strStartLoc);
			pDestStation = (CStationInfo*)m_cbxDestStn.GetItemDataPtr(m_cbxDestStn.GetCurSel());
			DEBUGER_ASSERT_VALID(pStartStation != NULL);
			DEBUGER_ASSERT_VALID(pDestStation != NULL);

			oJobItem.m_nLuggNum		= nLuggNum;
			oJobItem.m_nJobType		= (m_bAutoJob) ? enJobTypeAutoRet : enJobTypeSemiRet;
			oJobItem.m_nStartWH		= nStartWH;
			oJobItem.m_strStartStn	= pStartStation->m_strID;
			oJobItem.m_strStartLoc	= strStartLoc;
			oJobItem.m_nDestWH		= nDestWH;
			oJobItem.m_strDestStn	= pDestStation->m_strID;

			oJobItem.m_nSize		= 0;			// 제품 크기
//			// 입고시 도착 Location의 Level이 10 이상일경우 Size가 1로 들어갈것!
			if((oJobItem.m_strDestLoc != "") && (_ttoi(oJobItem.m_strDestLoc.Mid(4,2)) > 10))
				oJobItem.m_nSize	= 1;
		}
		break;

	case enSemiR2R:
		{
			if (m_cbxStartWH.GetCurSel() != m_cbxDestWH.GetCurSel())
			{
				AfxMessageBox(_T("출발창고와 도착창고를 같게 선택해주세요!"));
				return;
			}

			BOOL bDouble = FALSE;
			if ((CLib::IsValidLocation(nStartWH, strStartLoc) == FALSE) || (CLib::GetStackerNum(nStartWH, strStartLoc, bDouble) == 0))
			{
				AfxMessageBox(_T("출발지 LOCATION을 확인해주세요!"));
				return;
			}

			if (CLib::IsDisableLocation(nStartWH, strStartLoc))
			{
				AfxMessageBox(_T("금지된 출발지 LOCATION 입니다. 확인해주세요!"));
				return;
			}

			if ((CLib::IsValidLocation(nDestWH, strDestLoc) == FALSE) || (CLib::GetStackerNum(nDestWH, strDestLoc, bDouble) == 0))
			{
				AfxMessageBox(_T("도착지 LOCATION을 확인해주세요!"));
				return;
			}

			if (CLib::IsDisableLocation(nDestWH, strDestLoc))
			{
				AfxMessageBox(_T("금지된 도착지 LOCATION 입니다. 확인해주세요!"));
				return;
			}

			if (CLib::GetStackerNum(nStartWH, strStartLoc, bDouble) != CLib::GetStackerNum(nDestWH, strDestLoc, bDouble))
			{
				AfxMessageBox(_T("출발지와 도착지의 LOCATION이 동일호기의 S/C작업이 아닙니다. 확인해주세요!"));
				return;
			}

			if ( strStartLoc == strDestLoc)
			{
				AfxMessageBox(_T("출발지의 LOCATION과 도착지의 LOCATION이 같습니다. 확인해 주세요!"));
				return;
			}
			
			pStartStation = m_pDoc->GetScStationInfo(nStartWH, strStartLoc);
			pDestStation = m_pDoc->GetScStationInfo(nDestWH, strDestLoc);
			DEBUGER_ASSERT_VALID(pStartStation != NULL);
			DEBUGER_ASSERT_VALID(pDestStation != NULL);

			oJobItem.m_nLuggNum		= nLuggNum;
			oJobItem.m_nJobType		= (m_bAutoJob) ? enJobTypeAutoR2R : enJobTypeSemiR2R;
			oJobItem.m_nStartWH		= nStartWH;
			oJobItem.m_strStartStn	= pStartStation->m_strID;
			oJobItem.m_strStartLoc	= strStartLoc;
			oJobItem.m_nDestWH		= nDestWH;
			oJobItem.m_strDestStn	= pDestStation->m_strID;
			oJobItem.m_strDestLoc	= strDestLoc;

			oJobItem.m_nSize		= 0;			// 제품 크기
//			// 입고시 도착 Location의 Level이 10 이상일경우 Size가 1로 들어갈것!
			if((oJobItem.m_strDestLoc != "") && (_ttoi(oJobItem.m_strDestLoc.Mid(4,2)) > 10))
				oJobItem.m_nSize	= 1;
		}
		break;
	case enSemiMove:
		{
			if (m_cbxStartStn.GetCurSel() == CB_ERR)
			{
				AfxMessageBox(_T("출발지를 선택해주세요!"));
				return;
			}

			if (m_cbxDestStn.GetCurSel() == CB_ERR)
			{
				AfxMessageBox(_T("도착지를 선택해주세요!"));
				return;
			}

			if (m_cbxGenCode.GetCurSel() == CB_ERR)
			{
				AfxMessageBox(_T("수량을 선택해주세요!"));
				return;
			}

			int nQty = m_cbxGenCode.GetItemData(m_cbxGenCode.GetCurSel());


			pStartStation = (CStationInfo*)m_cbxStartStn.GetItemDataPtr(m_cbxStartStn.GetCurSel());
			pDestStation = (CStationInfo*)m_cbxDestStn.GetItemDataPtr(m_cbxDestStn.GetCurSel());
			DEBUGER_ASSERT_VALID(pStartStation != NULL);
			DEBUGER_ASSERT_VALID(pDestStation != NULL);

			if ( pStartStation->m_strID == pDestStation->m_strID)
			{
				AfxMessageBox(_T("출발지와 도착지의 Station 번호가 같습니다. 확인해 주세요!"));
				return;
			}

			CString strTemp;
			strTemp.Format(_T("%d개 작업을 생성하시겠습니까?"), nQty);
			if (AfxMessageBox(strTemp, MB_OKCANCEL) != IDOK)
				return;

			int i = 0;
			for(i = 0 ; i < nQty ; i++)
			{
				CJobItem oMoveJobItem(m_pDoc);
				int nMoveLuggNum = m_pDoc->m_pJob->GenerateOfflineLuggNum();
				if (nMoveLuggNum == 0)
				{
					strTemp.Format(_T("이동 작업번호 %d/%d 생성 실패!"), i+1, nQty);
					AfxMessageBox(strTemp);
					return;
				}

				oMoveJobItem.m_nLuggNum		= nMoveLuggNum;
				oMoveJobItem.m_nJobType		= (m_bAutoJob) ? enJobTypeAutoMove : enJobTypeSemiMove;
				oMoveJobItem.m_nStartWH		= nStartWH;
				oMoveJobItem.m_strStartStn	= pStartStation->m_strID;
				oMoveJobItem.m_nDestWH		= nDestWH;
				oMoveJobItem.m_strDestStn	= pDestStation->m_strID;

				oMoveJobItem.m_nSize		= 0;			// 제품 크기
	//			// 입고시 도착 Location의 Level이 10 이상일경우 Size가 1로 들어갈것!
				if((oMoveJobItem.m_strDestLoc != "") && (_ttoi(oMoveJobItem.m_strDestLoc.Mid(4,2)) > 10))
					oMoveJobItem.m_nSize	= 1;

				if ((pJobItem = m_pDoc->m_pJob->Add(oMoveJobItem)) == NULL)
				{
					strTemp.Format(_T("작업 %d/%d 생성 실패!"), i + 1, nQty);
					AfxMessageBox(oMoveJobItem.GetJobTypeString() + strTemp);
					return;
				}

				strLog.Format(_T("%s 작업 %d/%d 생성"), oMoveJobItem.GetJobTypeString(), i + 1, nQty);
				m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, strLog, _T("CJobOfflineDlg::OnButtonCreate"), pJobItem);

				if (m_pDoc->m_pJob->Invoke(pJobItem) == FALSE)
				{
					strTemp.Format(_T("작업 %d/%d 지시 실패!"), i + 1, nQty);
					AfxMessageBox(oMoveJobItem.GetJobTypeString() + _T(" 작업지시 실패!"));
					return;
				}
			}
			
			// 여기서 완료해야만함! 
			return;
		}
		break;

	case enSemiW2W:
		{
//			if (m_cbxStartWH.GetCurSel() == m_cbxDestWH.GetCurSel())
//			{
//				AfxMessageBox(_T("출발창고와 도착창고를 다르게 선택해주세요!"));
//				return;
//			}

			if ((CLib::IsValidLocation(nStartWH, strStartLoc) == FALSE) || (CLib::GetStackerNum(nStartWH, strStartLoc) == 0))
			{
				AfxMessageBox(_T("출발지 LOCATION을 확인해주세요!"));
				return;
			}

			if (CLib::IsDisableLocation(nStartWH, strStartLoc))
			{
				AfxMessageBox(_T("금지된 출발지 LOCATION 입니다. 확인해주세요!"));
				return;
			}

			if ((CLib::IsValidLocation(nDestWH, strDestLoc) == FALSE) || (CLib::GetStackerNum(nDestWH, strDestLoc) == 0))
			{
				AfxMessageBox(_T("도착지 LOCATION을 확인해주세요!"));
				return;
			}

			if (CLib::IsDisableLocation(nDestWH, strDestLoc))
			{
				AfxMessageBox(_T("금지된 도착지 LOCATION 입니다. 확인해주세요!"));
				return;
			}

			if (CLib::GetStackerNum(nStartWH, strStartLoc) == CLib::GetStackerNum(nDestWH, strDestLoc))
			{
				AfxMessageBox(_T("출발지와 도착지의 LOCATION이 동일호기의 S/C작업입니다. 확인해주세요!"));
				return;
			}

			pStartStation = m_pDoc->GetScStationInfo(nStartWH, strStartLoc);
			pDestStation = m_pDoc->GetScStationInfo(nDestWH, strDestLoc);
			DEBUGER_ASSERT_VALID(pStartStation != NULL);
			DEBUGER_ASSERT_VALID(pDestStation != NULL);

			oJobItem.m_nLuggNum		= nLuggNum;
			oJobItem.m_nJobType		= (m_bAutoJob) ? enJobTypeAutoW2W : enJobTypeSemiW2W;
			oJobItem.m_nStartWH		= nStartWH;
			oJobItem.m_strStartStn	= pStartStation->m_strID;
			oJobItem.m_strStartLoc	= strStartLoc;
			oJobItem.m_nDestWH		= nDestWH;
			oJobItem.m_strDestStn	= pDestStation->m_strID;
			oJobItem.m_strDestLoc	= strDestLoc;

			oJobItem.m_nSize		= 0;			// 제품 크기
//			// 입고시 도착 Location의 Level이 10 이상일경우 Size가 1로 들어갈것!
			if((oJobItem.m_strDestLoc != "") && (_ttoi(oJobItem.m_strDestLoc.Mid(4,2)) > 10))
				oJobItem.m_nSize	= 1;
		}
		break;

	default:
		DEBUGER_ASSERT_VALID(FALSE);
		break;
	}

	if (AfxMessageBox(_T("작업을 생성하시겠습니까?"), MB_OKCANCEL) != IDOK)
		return;

	if ((pJobItem = m_pDoc->m_pJob->Add(oJobItem)) == NULL)
	{
		AfxMessageBox(oJobItem.GetJobTypeString() + _T(" 작업생성 실패!"));
		return;
	}

	strLog.Format(_T("%s 작업생성"), oJobItem.GetJobTypeString());
	m_pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, strLog, _T("CJobOfflineDlg::OnButtonCreate"), pJobItem);

	if (m_pDoc->m_pJob->Invoke(pJobItem) == FALSE)
	{
		AfxMessageBox(oJobItem.GetJobTypeString() + _T(" 작업지시 실패!"));
//		return;
	}
}

void CJobOfflineDlg::UpdateStationInfo(int nWarehouse, CComboBox& cbxStation)
{
	if (!CJobItem::IsValidWarehouse(nWarehouse))
	{
		AfxMessageBox(_T("유효하지 않은 창고번호입니다!"));
		return;
	}

	CString strTemp;
	cbxStation.ResetContent();
	CStationInfo* pStation = NULL;
	for (int i=0; i<m_pDoc->m_pStationInfos.GetSize(); ++i)
	{
		pStation = m_pDoc->m_pStationInfos[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		if (pStation->GetWarehouseNo() != nWarehouse)
			continue;

		strTemp.Format(_T("[%d] %s"), pStation->GetTrackDestination(), pStation->m_strName);

		switch (cbxStation.GetDlgCtrlID())
		{
		case IDC_CBX_DEPART:
			if ((pStation->m_enKind == CStationInfo::enStoStation) || (pStation->m_enKind == CStationInfo::enArvStation))
			{
				m_cbxStartStn.AddString(strTemp);
				m_cbxStartStn.SetItemDataPtr(m_cbxStartStn.GetCount()-1, (void*)pStation);
			}
			break;

		case IDC_CBX_ARRIVE:
			if ((pStation->m_enKind == CStationInfo::enRetStation) || (pStation->m_enKind == CStationInfo::enArvStation))
			{
				m_cbxDestStn.AddString(strTemp);
				m_cbxDestStn.SetItemDataPtr(m_cbxDestStn.GetCount()-1, (void*)pStation);
			}
			break;

		default:
			DEBUGER_ASSERT_VALID(FALSE);
			return;
		}
	}
}

void CJobOfflineDlg::OnSelchangeComboStartWarehouse() 
{
	if (m_cbxStartWH.GetCurSel() == CB_ERR)
		return;

	UpdateStationInfo(m_cbxStartWH.GetItemData(m_cbxStartWH.GetCurSel()), m_cbxStartStn);
}

void CJobOfflineDlg::OnSelchangeComboDestWarehouse() 
{
	if (m_cbxDestWH.GetCurSel() == CB_ERR)
		return;

	UpdateStationInfo(m_cbxDestWH.GetItemData(m_cbxDestWH.GetCurSel()), m_cbxDestStn);
}
