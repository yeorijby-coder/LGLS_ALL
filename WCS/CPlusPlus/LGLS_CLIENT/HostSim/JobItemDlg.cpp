// JobItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "JobItemDlg.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJobItemDlg dialog


CJobItemDlg::CJobItemDlg(CJobItem* pJobItem, EN_KIND enKind, CWnd* pParent /* = NULL */)
	: CDialog(CJobItemDlg::IDD, pParent)
{
	m_enKind = enKind;
	m_pJobItem = pJobItem;
	DEBUGER_ASSERT_VALID(m_pJobItem != NULL);
	DEBUGER_ASSERT_VALID_LOG(m_pJobItem->m_pDoc != NULL, m_pJobItem->GetLogString());

	//{{AFX_DATA_INIT(CJobItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CJobItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJobItemDlg)
	DDX_Control(pDX, IDC_COMBO_JOB_STATUS, m_cbxJobStatus);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJobItemDlg, CDialog)
	//{{AFX_MSG_MAP(CJobItemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJobItemDlg message handlers

BOOL CJobItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	DEBUGER_ASSERT_VALID(m_pJobItem != NULL);

	if (m_pJobItem->m_pDoc == NULL)
	{
		AfxMessageBox(_T("CEcsDoc Pointer NULL!"));
		return FALSE;
	}
	
	int w = 32, h = 32;
	INIT_BUTTON(m_btnCancel, IDI_CANCEL3, w, h, _T("저장하지 않고 종료"));
	INIT_BUTTON(m_btnOK, IDI_OK3, w, h, _T("저장후 종료"));

	switch (m_enKind)
	{
	case enModify:		m_btnOK.SetWindowText(_T("수        정"));	break;
	case enComplete:	m_btnOK.SetWindowText(_T("강 제  완 료"));	break;
	case enCancel:		m_btnOK.SetWindowText(_T("작 업  취 소"));	break;
	case enArrive:		m_btnOK.SetWindowText(_T("도 착  완 료"));	break;
	default:			DEBUGER_ASSERT_VALID(FALSE);	return FALSE;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SetDlgItemInt(IDC_EDIT_LUGG_NUM, m_pJobItem->m_nLuggNum);
	SetDlgItemText(IDC_EDIT_JOB_TYPE, m_pJobItem->GetJobTypeString());

	SetDlgItemText(IDC_EDIT_START_WH, m_pJobItem->GetStartWarehouseString());
	SetDlgItemText(IDC_EDIT_START_STN, m_pJobItem->GetStartStationString());
//	SetDlgItemText(IDC_EDIT_START_LOC, m_pJobItem->m_strStartLoc);

	SetDlgItemText(IDC_EDIT_DEST_WH, m_pJobItem->GetDestWarehouseString());
	SetDlgItemText(IDC_EDIT_DEST_STN, m_pJobItem->GetDestStationString());
//	SetDlgItemText(IDC_EDIT_DEST_LOC, m_pJobItem->m_strDestLoc);

	SetDlgItemText(IDC_EDIT_UPDATE_TIME, m_pJobItem->m_tTime.Format(_T("%Y-%m-%d %H:%M:%S")));
	SetDlgItemText(IDC_EDIT_DEPART_HS, m_pJobItem->GetDepartTrackString());
	SetDlgItemText(IDC_EDIT_ARRIVE_HS, m_pJobItem->GetArriveTrackString());

	SetDlgItemText(IDC_EDIT_GEN_CODE, m_pJobItem->GetGenString());
	SetDlgItemText(IDC_EDIT_PROD_INFO, m_pJobItem->m_strWeight);
	CString strTemp, strTemp1;
	strTemp = (m_pJobItem->m_strRouteStn == _T("215")) ? _T("중량체커#1") : ((m_pJobItem->m_strRouteStn == _T("216")) ? _T("중량체커#2") : ((m_pJobItem->m_strRouteStn == _T("217")) ? _T("중량체커#3") : _T("")));
	strTemp1.Format(_T("[%s] %s"),m_pJobItem->m_strRouteStn, strTemp);
	SetDlgItemText(IDC_EDIT_CRATE_KIND, strTemp1);
	SetDlgItemText(IDC_EDIT_COMPANY, m_pJobItem->m_strProductID);
	strTemp.Format(_T("%d"), m_pJobItem->m_nPairLuggNo);
	SetDlgItemText(IDC_EDIT_PAIR_LUGG, (m_pJobItem->m_nPairLuggNo == 0) ? _T("없음") : strTemp);				// SIZE로 사용


	GetDlgItem(IDC_EDIT_LINE_INFO)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CRATE_STATUS)->EnableWindow(FALSE);
		
	if(m_pJobItem->m_nJobStatus == enJobStatusRtvInvoke && m_pJobItem->m_nVehicleID != 0)
	{		
		CLgv* pRgv = NULL;
		int nRgvNum = 0;
		if (m_pJobItem->m_nVehicleID > 0 && m_pJobItem->m_nVehicleID <= 5)
		{
			nRgvNum = 1;
		}
		else if (m_pJobItem->m_nVehicleID > 5 && m_pJobItem->m_nVehicleID <= 13)
		{
			nRgvNum = 2;
		}

		pRgv = (CLgv*) m_pJobItem->m_pDoc->GetEquipment(CEquipment::enRGV, nRgvNum);

		if (pRgv != NULL)
		{
			CLgvVehicle* pVehicle = (CLgvVehicle*)pRgv->m_pInfo->GetLgvVehicle(m_pJobItem->m_nVehicleID);

			if (pVehicle != NULL)
			{
				SetDlgItemInt(IDC_EDIT_LINE_INFO, m_pJobItem->m_nVehicleID);				// RGV 번호
				SetDlgItemText(IDC_EDIT_CRATE_STATUS, m_pJobItem->GetRgvStatusString());				// RGV 상태

				GetDlgItem(IDC_EDIT_LINE_INFO)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_CRATE_STATUS)->EnableWindow(TRUE);	
			}
		}
	}

	CString strBcrInfo;
	strBcrInfo.Format(_T("%s"), m_pJobItem->m_strBarcode);
	SetDlgItemText(IDC_EDIT_BARCODE, strBcrInfo);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (int i=0; i<enJobStatusSize; ++i)
		m_cbxJobStatus.AddString(CJobItem::GetJobStatusString(i));
	m_cbxJobStatus.SetCurSel(m_pJobItem->m_nJobStatus);
	m_cbxJobStatus.EnableWindow(m_enKind == enModify);
	

	
	//	IDC_STATIC_LOGO
	DrawBitmap();



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJobItemDlg::DrawBitmap() 
{
	CClientDC WinDC(this);		// 윈도우 DC를 얻음
	CDC MemDC;  
	BITMAP bmpInfo;  

	// dc에 대해 메모리DC를 만들어 준다.  
	MemDC.CreateCompatibleDC( &WinDC );  
	CBitmap bmp;  
	CBitmap* pOldBmp = NULL;  

	// 비트맵 로드  
	bmp.LoadBitmap( IDB_LOGO_USER );  

	// 비트맵에 정보를 저장  
	bmp.GetBitmap( &bmpInfo );  

	// 메모리DC에 저장  
	pOldBmp = MemDC.SelectObject( &bmp );

	CRect rect;
	GetDlgItem(IDC_STATIC_LOGO)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	// 메모리 Dc에 있는 비트맵을 화면 DC로 복사, 출력  
	WinDC.StretchBlt( rect.left, rect.top, bmpInfo.bmWidth/2, bmpInfo.bmHeight/2,  
					&MemDC, rect.left, rect.top, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY ); 

	// MemDC를 이전값으로 복원  
	MemDC.SelectObject( pOldBmp ); 
}

void CJobItemDlg::OnOK() 
{
	CEcsDoc* pDoc = m_pJobItem->m_pDoc;
	DEBUGER_ASSERT_VALID(m_pJobItem != NULL);
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	CString strTemp;
	switch (m_enKind)
	{
	case enModify:
		{
			if (AfxMessageBox(_T("에러등 특이상황 발생시에만 사용하세요!\n\n수정 하시겠습니까?"), MB_OKCANCEL) != IDOK)
				return;

			if (m_cbxJobStatus.GetCurSel() == CB_ERR)
			{
				AfxMessageBox(_T("작업진행상태를 설정해주세요!"));
				return;
			}

			int nPairLugg = (int)GetDlgItemInt(IDC_EDIT_PAIR_LUGG);
			if (nPairLugg < 0)
				return;


			int nArriveTrack = GET_INT(IDC_EDIT_ARRIVE_HS);
			int nRouteStn = GET_INT(IDC_EDIT_CRATE_KIND);
			strTemp.Format(_T("%03d"), nRouteStn);
			m_pJobItem->m_nArriveTrackNum = nArriveTrack;
			m_pJobItem->m_strRouteStn = strTemp;

			GET(IDC_EDIT_PROD_INFO, strTemp);
			m_pJobItem->m_strWeight = strTemp;
			m_pJobItem->SetJobStatus(m_cbxJobStatus.GetCurSel());
			m_pJobItem->m_nPairLuggNo = nPairLugg;
			pDoc->m_pJob->Backup();
		}
		break;

	case enComplete:
		{
			if (AfxMessageBox(_T("작업은 완료되었으나 어떤 원인에 의해 IMS에 완료보고가 안된 경우에만 사용하세요!\n\n완료보고를  하시겠습니까?"), MB_OKCANCEL) != IDOK)
				return;

			if (m_pJobItem->GetJobPattern() == enJobPatternMove)
			{
				AfxMessageBox(_T("이동작업은 완료보고 할 수 없습니다!"));
				return;
			}

			if ((pDoc->m_pHostCl == NULL) || (pDoc->m_pHostCl->IsConnect() == FALSE))
			{
				AfxMessageBox(_T("IMS와 연결이 해제되었습니다. 연결후 재시도하세요!"));
				return;
			}

			BYTE ucScJobType = 0;
			switch (m_pJobItem->GetJobPattern())
			{
			case enJobPatternSto:
			case enJobPatternR2R:
				ucScJobType = CScInfo::enStore;
				break;

			case enJobPatternRet:
			case enJobPatternPR:
				ucScJobType = CScInfo::enRetrieve;
//				m_pJobItem->m_nJobStatus = enJobStatusCvNew;
				m_pJobItem->m_nJobStatus = enJobStatusScComplete;
				break;

			case enJobPatternW2W:
				if (AfxMessageBox(_T("도착창고 완료입니까?"), MB_YESNO) == IDYES) ucScJobType = CScInfo::enStore;
				else ucScJobType = CScInfo::enRetrieve;
				break;

			default:
				DEBUGER_ASSERT_VALID(FALSE);
				return;
			}

			pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, _T("강제완료"), _T("CJobItemDlg::OnOK"), m_pJobItem);
			pDoc->m_pJob->Complete(m_pJobItem, ucScJobType, TRUE);
			pDoc->m_pJob->Backup();
		}
		break;

	case enArrive:
		{
			if (AfxMessageBox(_T("작업은 완료되었으나 어떤 원인에 의해 IMS에 도착보고가 안된 경우에만 사용하세요.\n\n도착보고를 하시겠습니까?"), MB_YESNO) != IDYES)
				return;

			if ((m_pJobItem->GetJobPattern() != enJobPatternRet) && 
				(m_pJobItem->GetJobPattern() != enJobPatternPR) && 
				(m_pJobItem->GetJobPattern() != enJobPatternMove))
			{
				AfxMessageBox(_T("출고작업과 피킹출고작업과 이동작업을 제외한 작업은 도착보고를 할 수 없습니다!"));
				return;
			}

			if ((pDoc->m_pHostCl == NULL) || (pDoc->m_pHostCl->IsConnect() == FALSE))
			{
				AfxMessageBox(_T("IMS와 연결이 해제되었습니다. 연결후 재시도하세요!"));
				return;
			}

			pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, _T("작업도착보고"), _T("CJobItemDlg::OnOK"), m_pJobItem);
			pDoc->m_pJob->Arrive(m_pJobItem, TRUE);
			pDoc->m_pJob->Backup();
		}
		break;

	case enCancel:	
		{
			if (pDoc->IsIdleJob(m_pJobItem->m_nLuggNum) == FALSE)
			{
				if (AfxMessageBox(_T("설비에서 사용중인 작업입니다. 삭제하시겠습니까?"), MB_YESNO) != IDYES)
					return;
			}
			
			if (m_pJobItem->IsOfflineJobType())
			{
				pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, _T("작업삭제"), _T("CJobItemDlg::OnOK"), m_pJobItem);
				pDoc->m_pJob->Remove(m_pJobItem);
				break;
			}

//			if (AfxMessageBox(_T("IMS에 보고하지 않고 강제로 삭제하시겠습니까?"), MB_YESNO) == IDYES)
//			{
				pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, _T("강제작업삭제"), _T("CJobItemDlg::OnOK"), m_pJobItem);
				pDoc->m_pJob->Remove(m_pJobItem);
				pDoc->m_pJob->Backup();
//				break;
//			}
//
//			if ((pDoc->m_pHostCl == NULL) || (pDoc->m_pHostCl->IsConnect() == FALSE))
//			{
//				AfxMessageBox(_T("IMS와 연결이 해제되었습니다. 연결후 재시도하세요!"));
//				return;
//			}
//
//			pDoc->WriteLog(LOG_TYPE_EVENT, LOG_POS_HOST, _T("작업삭제보고"), _T("CJobItemDlg::OnOK"), m_pJobItem);
//			pDoc->m_pJob->Cancel(m_pJobItem, TRUE);
//			pDoc->m_pJob->Backup();
		}
		break;

	default:
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}
	
	CDialog::OnOK();
}
