// JobDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "JobDlg.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJobDlg dialog


CJobDlg::CJobDlg(CEcsDoc* pDoc, CTrackInfo* pTrack, CWnd* pParent /* = NULL */)
	: CDialog(CJobDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_pTrack = pTrack;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pDoc->m_pJob != NULL);
	DEBUGER_ASSERT_VALID(m_pDoc->m_pLog != NULL);
	
	//{{AFX_DATA_INIT(CJobDlg)
	m_bDestPos = FALSE;
	m_bNextCv = FALSE;
	m_bNextCv2 = FALSE;
	m_bTwinCheck = FALSE;
	//}}AFX_DATA_INIT
	m_itemindex = 0;
//	m_pDestinations = NULL;
}


void CJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJobDlg)
	DDX_Control(pDX, IDC_COMBO_DEST_PLC, m_cbxDestPlc);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_ADD,  m_btnAdd);
	DDX_Control(pDX, IDC_BTN_ADD2, m_btnAdd2);
	DDX_Control(pDX, IDC_LIST_JOB, m_lstJobList);
	DDX_Check(pDX, IDC_CHECK_DEST_POS, m_bDestPos);
	DDX_Check(pDX, IDC_CHECK_NEXT_CV, m_bNextCv);
	DDX_Check(pDX, IDC_CHECK_NEXT_CV2, m_bNextCv2);
	DDX_Check(pDX, IDC_CHECK_CENTER, m_bCenter);
	DDX_Check(pDX, IDC_CHECK_CENTER_NEXT, m_bCenterNext);
	DDX_Check(pDX, IDC_CHECK_CENTER_DEST, m_bCenterDest);
	DDX_Check(pDX, IDC_CHECK_CROSS, m_bCross);
	DDX_Check(pDX, IDC_CHECK_CROSS_TRACK, m_bCrossTrack);
	DDX_Check(pDX, IDC_CHECK_CROSS_DEST, m_bCrossDest);
	DDX_Control(pDX, IDC_COMBO_DEST_FLOOR, m_cbxDestFloor);
	DDX_Check(pDX, IDC_CHECK_TWIN, m_bTwinCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJobDlg, CDialog)
	//{{AFX_MSG_MAP(CJobDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAddOfflineJob)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_JOB, OnDblclkListJob)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_DEST_POS, OnCheckDestPos)
	ON_BN_CLICKED(IDC_CHECK_NEXT_CV, OnCheckNextCv)
	ON_BN_CLICKED(IDC_CHECK_NEXT_CV2, OnCheckNextCv2)
	ON_BN_CLICKED(IDC_BTN_ADD2, OnBtnAdd2)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_CHECK_CENTER_NEXT, OnCheckCenterNext)
	ON_BN_CLICKED(IDC_CHECK_CENTER_DEST, OnCheckCenterDest)
	ON_BN_CLICKED(IDC_CHECK_CROSS_TRACK, OnCheckCrossTrack)
	ON_BN_CLICKED(IDC_CHECK_CENTER, OnCheckCenter)
	ON_BN_CLICKED(IDC_CHECK_CROSS, OnCheckCross)
	ON_BN_CLICKED(IDC_BTN_RETRY, OnBtnRetry)
	ON_BN_CLICKED(IDC_BTN_ADD3, OnBtnAdd3)
	ON_CBN_EDITCHANGE(IDC_COMBO_DEST_FLOOR, OnEditchangeComboDestFloor)
	ON_BN_CLICKED(IDC_CHECK_CROSS_DEST, OnCheckCrossDest)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_JOB, &CJobDlg::OnNMClickListJob)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_JOB, &CJobDlg::OnLvnKeydownListJob)
	ON_BN_CLICKED(IDOK, &CJobDlg::OnBnClickedOk)
//	ON_WM_KEYDOWN()
//ON_WM_KEYDOWN()
ON_BN_CLICKED(IDC_BTN_RETRY2, &CJobDlg::OnBnClickedBtnRetry2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJobDlg message handlers

BOOL CJobDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CDialog::CenterWindow();

	int w = 16, h = 16;
	INIT_BUTTON(m_btnAdd, IDI_ICON_MANUAL, w, h, _T("등록"));
	INIT_BUTTON(m_btnAdd2, IDI_ICON_MANUAL, w, h, _T("등록"));
	INIT_BUTTON(m_btnOK, IDI_EXIT, w, h, _T("창닫기"));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	enum EN_STN_KIND { enKindArvStation, enKindStoStation, enKindRetStation, enKindCraneStation };

	CImageList imageList;
	imageList.Create(16, 16, TRUE, 1, 1);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_JOB_ALL));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_JOB_PLAN));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_JOB_RESULT));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_JOB_COMPLETE));
	m_lstJobList.SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	const int size			= 2;
	UINT	mask			= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	int		fmt				= LVCFMT_LEFT;
	int		cx[size]		= {300, 80};
	LPTSTR	pszText[size]	= {_T("목적지:[번호]목적지코드"), _T("다음트랙")};
	for (int i=0; i<size; ++i)
		m_lstJobList.InsertColumn(i, (LPCTSTR)pszText[i], fmt, cx[i]);
	m_lstJobList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_lstJobList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CString strTemp;

	SET(IDC_EDT_USE_DEST_TABLE, _T(" "));

	m_bUseDT = FALSE;

	if (m_pTrack != NULL)
	{
		strTemp.Format(_T("%04d"), m_pTrack->m_nNumber);
		SET(IDC_EDT_SET_TRACK, strTemp);
		strTemp.Format(_T("%04d"), m_pTrack->m_nCenterNext);
		SET(IDC_EDIT_CENTER_NEXT,strTemp);
		strTemp.Format(_T("%04d"), m_pTrack->m_nCenterDest);
		SET(IDC_EDIT_CENTER_DEST,strTemp);
		strTemp.Format(_T("%04d"), m_pTrack->m_nCrossTrack);
		SET(IDC_EDIT_CROSS_TRACK,strTemp);
		strTemp.Format(_T("%04d"), m_pTrack->m_nCrossDest);
		SET(IDC_EDIT_CROSS_DEST,strTemp);
		m_bCenter = m_pTrack->m_bCenter;
		m_bCross = m_pTrack->m_bCross;
		m_bTwinCheck = m_pTrack->m_bTwinCheck;
		m_pDoc->m_nCenterNextPlcNum = m_pTrack->m_nCenterNextPlcNum;
		m_pDoc->m_nCrossPlcNum = m_pTrack->m_nCrossPlcNum;
	//	m_pDoc->m_nHSTrack = m_pTrack->m_nHSTrack;
		
		if (m_pDoc->m_pDeviceMaps[m_pTrack->m_nCvPlcNum - 1]->m_bUseSeparatelyETC == TRUE)
		{
			m_bUseDT = TRUE;
			
			SET(IDC_EDT_USE_DEST_TABLE, _T("목적지 Table 사용함!"));
		}
	}
	else
	{
		SET(IDC_EDT_SET_TRACK, _T("0"));
	}
	
	//int nDestFloor = CLib::GetTrackNumByFloor(m_pTrack->m_nDestPos);

	m_cbxDestFloor.AddString(_T("단"));
	m_cbxDestFloor.SetItemData(0, 0);
	
	for(int j=0; j<3; j++)
	{
		strTemp.Format(_T("%1d단"), j + 1);
		m_cbxDestFloor.AddString(strTemp);
		m_cbxDestFloor.SetItemData(m_cbxDestFloor.GetCount()-1,0);		
	}

	m_cbxDestPlc.AddString(_T("PLC"));
	m_cbxDestPlc.SetItemData(0, 0);

	for(int k=0; k<10; k++)
	{
		CString str;
		str.Format(_T("%d"), k+1);
		m_cbxDestPlc.AddString(str);
		m_cbxDestPlc.SetItemData(m_cbxDestPlc.GetCount()-1, 0);
	}

	
	UpdateData(FALSE);

	UpdateList();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CJobDlg::UpdateList()
{
	CString strSetTr, strDestPos, strNextTr;
	int		nSetTr, nDestPos, nNextTr;

	GET(IDC_EDT_SET_TRACK, strSetTr);
	nSetTr = _ttoi(strSetTr);
	GET(IDC_EDT_DEST_POS, strDestPos);
	nDestPos = _ttoi(strDestPos);
	GET(IDC_EDT_NEXT_TRACK, strNextTr);
	nNextTr = _ttoi(strNextTr);

	if (m_pTrack == NULL)
		return;

	int nLen = m_pTrack->m_nStationArray.GetSize();
	//int nLen = m_itemindex;

	int i = 0, j = 0, nImage = enKindCraneStation;
	UINT uMask = LVIF_TEXT | LVIF_IMAGE;

	CStationInfo* pStation = NULL;
	CTrackHS* pHs = NULL;

	m_lstJobList.DeleteAllItems();

	CDestination* pDestination = NULL;
	for (int k=0; k<nLen; k++)
	{
		pStation = m_pDoc->m_pStationInfos[k];
		CString strStation;


		switch (pStation->m_enKind)
		{
		case CStationInfo::enArvStation:	nImage = enKindArvStation;		break;
		case CStationInfo::enScStation:		nImage = enKindCraneStation;	break;
		case CStationInfo::enRetStation:	nImage = enKindRetStation;		break;
		case CStationInfo::enStoStation:	//nImage = enKindStoStation;		break;
		default:															continue;
		}
		
		if (pStation->m_enKind == CStationInfo::enScStation)
		{
			//strStation.Format(_T("[%05s] %05d"), pStation->m_strID, m_pTrack->m_nStationArray[k]);
			strStation.Format(_T("[%s] %05s"), pStation->m_strName, pStation->m_strID);
			m_lstJobList.InsertItem(uMask, i, strStation, 0, 0, nImage, 0);

			strStation = CConvert::ToString(m_pTrack->m_nNextTrArray[k]);
			m_lstJobList.SetItem(i, ++j, uMask, strStation, nImage, 0, 0, 0);
		}
		else
		{
			strStation.Format(_T("[TR #%03d] %05d"), pStation->m_pTrack->m_nNumber, pStation->m_pTrack->m_nDestCode);
			m_lstJobList.InsertItem(uMask, i, strStation, 0, 0, nImage, 0);

			strStation = CConvert::ToString(m_pTrack->m_nNextTrArray[k]);
			m_lstJobList.SetItem(i, ++j, uMask, strStation, nImage, 0, 0, 0);
		}
		i++;
		j=0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CJobItem* CJobDlg::GetSelectJobItem()
{
	POSITION pos = m_lstJobList.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return NULL;

	int nIndex = m_lstJobList.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex >= m_lstJobList.GetItemCount()))
		return NULL;

	return (CJobItem*)m_lstJobList.GetItemData(nIndex);
}

void CJobDlg::OnBtnAddOfflineJob() 
{
	CString strDestPos, strNextTr;
	int nDestPos, nNextTr;

	GET(IDC_EDT_NEXT_TRACK, strNextTr);
	nNextTr = _ttoi(strNextTr);
	if (nNextTr <= 0)
		return;

	GET(IDC_EDT_DEST_POS, strDestPos);
	nDestPos = _ttoi(strDestPos);
	if (nDestPos <= 0)
		return;

	CStationInfo* pStation = NULL;

	int nLen = m_pTrack->m_nStationArray.GetSize();
	for (int i=0; i<nLen; i++)
	{
		pStation = m_pDoc->m_pStationInfos[i];

		int nStationArrayMember = m_pTrack->m_nStationArray[i];

		if (nStationArrayMember == nDestPos)
		{
			m_pTrack->m_nNextTrArray[i] = nNextTr;
			m_pTrack->m_nNextPlcArray[i] = m_pDoc->m_nNextPlcNum;
		}
		else if (pStation != NULL)
		{
			if (pStation->m_enKind == CStationInfo::enRetStation && pStation->m_pTrack->m_nDestCode == nDestPos)
			{
				m_pTrack->m_nNextTrArray[i] = nNextTr;
				m_pTrack->m_nNextPlcArray[i] = m_pDoc->m_nNextPlcNum;
				m_pTrack->m_nDestPosArray[i] = nDestPos;
			}
			else if (_ttoi(pStation->m_strID) == nDestPos)
			{
				m_pTrack->m_nDestPosArray[i] = nDestPos;
				m_pTrack->m_nNextTrArray[i] = nNextTr;
				m_pTrack->m_nNextPlcArray[i] = m_pDoc->m_nNextPlcNum;
			}
		}
	}

	UpdateList();
}

void CJobDlg::OnBtnAdd2()
{
	CString strDestPos, strNextTr;
	int nNextTr;

	GET(IDC_EDT_NEXT_TRACK2, strNextTr);
	nNextTr = _ttoi(strNextTr);
	if (nNextTr <= 0)
		return;

	//int nLen = m_pTrack->m_nStationArray.GetSize();
	int nLen = m_itemindex; //list에서 선택한 로우 값
	if(nLen < 0)
	{
		return;
	}

	m_pTrack->m_nNextTrArray[nLen] = nNextTr;
	m_pTrack->m_nNextPlcArray[nLen] = m_pDoc->m_nNextPlcNum2;

	UpdateList();
}

void CJobDlg::OnBtnSave()
{
	m_pTrack->m_nKind = 1;
	m_pTrack->m_nNextPlcNum = m_pDoc->m_nNextPlcNum;
	m_pTrack->m_nNextPlcNum2 = m_pDoc->m_nNextPlcNum2;

	UpdateData(TRUE);
	
	CString strCenterNext, strCenterDest, strCrossTrack, strCrossDest;
	int nCenterNext, nCenterDest, nCrossTrack, nCrossDest, nCenterNextPlcNum, nCrossPlcNum;

	if (m_bCenter)
	{
		GET(IDC_EDIT_CENTER_NEXT,strCenterNext);
		nCenterNext = _ttoi(strCenterNext);
		if (nCenterNext <= 0) return;
		GET(IDC_EDIT_CENTER_DEST,strCenterDest);
		nCenterDest = _ttoi(strCenterDest);
		if (nCenterDest <= 0) return;
		nCenterNextPlcNum = m_pDoc->m_nCenterNextPlcNum;
	}
	else
	{
		nCenterNext = 0;
		nCenterDest = 0;
		nCenterNextPlcNum = 0;
	}

	if (m_bCross)
	{
		GET(IDC_EDIT_CROSS_TRACK,strCrossTrack);
		nCrossTrack = _ttoi(strCrossTrack);
		GET(IDC_EDIT_CROSS_DEST,strCrossDest);
		nCrossDest = _ttoi(strCrossDest);
		if (nCrossTrack <= 0 || nCrossDest <= 0) return;
		nCrossPlcNum = m_pDoc->m_nCrossPlcNum;
	}
	else
	{
		nCrossTrack = 0;
		nCrossPlcNum = 0;
		nCrossDest = 0; //추가
	}
	
	m_pTrack->m_nNextCv = 0;
	m_pTrack->m_nNextPlcNum = 0;
	m_pTrack->m_bCenter = m_bCenter;
	m_pTrack->m_nCenterNextPlcNum = nCenterNextPlcNum;
	m_pTrack->m_nCenterNext = nCenterNext;
	m_pTrack->m_nCenterDest = nCenterDest;
	m_pTrack->m_bCross = m_bCross;
	m_pTrack->m_nCrossTrack = nCrossTrack;
	m_pTrack->m_nCrossDest = nCrossDest;
	m_pTrack->m_nCrossPlcNum = nCrossPlcNum;

	m_pTrack->SaveXML();

	m_pTrack->m_bModified = TRUE;
	m_pTrack->InvokeControl(m_pTrack->m_pTrackCtrl);
	
}

void CJobDlg::OnDblclkListJob(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
}

void CJobDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
		UpdateList();
	
	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////////
// <MODALESS DIALOG> 구현시 반드시 구현해야함

void CJobDlg::PostNcDestroy() 
{
	m_pDoc->m_pJobDlg = NULL;
	delete this;
}

void CJobDlg::OnClose() 
{
	DestroyWindow();
//	CDialog::OnClose();
}

void CJobDlg::OnOK() 
{
	DestroyWindow();
//	CDialog::OnOK();
}

void CJobDlg::OnCheckDestPos() 
{
	UpdateData(TRUE);

	if (m_bDestPos)
	{
		m_bNextCv = FALSE;
		m_bNextCv2 = FALSE;

		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		m_bCrossDest = FALSE;
	}

	UpdateData(FALSE);
	
}

void CJobDlg::OnCheckNextCv() 
{
	UpdateData(TRUE);
	
	if (m_bNextCv)
	{
		m_bDestPos = FALSE;
		m_bNextCv2 = FALSE;

		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		m_bCrossDest = FALSE;
	}

	UpdateData(FALSE);
	
}

void CJobDlg::OnCheckNextCv2() 
{
	UpdateData(TRUE);
	
	if (m_bNextCv2)
	{
		m_bNextCv = FALSE;
		m_bDestPos = FALSE;

		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		m_bCrossDest = FALSE;
	}

	UpdateData(FALSE);
	
}

void CJobDlg::OnCheckCenterNext() 
{
	UpdateData(TRUE);
	
	if (m_bCenterNext)
	{
		m_bNextCv = FALSE;
		m_bDestPos = FALSE;
		m_bNextCv2 = FALSE;

		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		m_bCrossDest = FALSE;
	}

	UpdateData(FALSE);
	
}

void CJobDlg::OnCheckCenterDest() 
{
	UpdateData(TRUE);
	
	if (m_bCenterDest)
	{
		m_bNextCv = FALSE;
		m_bDestPos = FALSE;
		m_bNextCv2 = FALSE;

		m_bCenterNext = FALSE;
		m_bCrossTrack = FALSE;
		m_bCrossDest = FALSE;
	}

	UpdateData(FALSE);
	
}

void CJobDlg::OnCheckCrossTrack() 
{
	UpdateData(TRUE);
	
	if (m_bCrossTrack)
	{
		m_bNextCv = FALSE;
		m_bDestPos = FALSE;
		m_bNextCv2 = FALSE;

		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossDest = FALSE;
	}

	UpdateData(FALSE);
	
}

void CJobDlg::OnCheckCenter() 
{
	UpdateData(TRUE);
	
	if (m_bCenter)
	{
		m_bNextCv = FALSE;
		m_bDestPos = FALSE;
		m_bNextCv2 = FALSE;

		m_bCenterNext = TRUE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		m_bCrossDest = FALSE;
	}

	UpdateData(FALSE);
	
}

void CJobDlg::OnCheckCross() 
{
	UpdateData(TRUE);
	
	if (m_bCross)
	{
		m_bNextCv = FALSE;
		m_bDestPos = FALSE;
		m_bNextCv2 = FALSE;

		m_bCrossTrack = TRUE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossDest = FALSE;
	}

	UpdateData(FALSE);
	
}

void CJobDlg::OnBtnRetry() 
{
	//int nLen = m_pTrack->m_nStationArray.GetSize();
	int nLen = m_itemindex;
	
	if(nLen < 0)
	{
		return;
	}

	m_pTrack->m_nNextTrArray[nLen] = 0; //다음트랙
	m_pTrack->m_nNextPlcArray[nLen] = 0; //다음PLC
	
	//for (int i=0; i<nLen; i++)
	//{
	//	m_pTrack->m_nNextTrArray[i] = 0; //다음트랙
	//	m_pTrack->m_nNextPlcArray[i] = 0; //다음PLC
	//}

	UpdateList();
	
}

void CJobDlg::OnBtnAdd3() 
{
	CString strDestFloor, strDestPos, strNextTr;
	int nNextTr;
	int nFloor;
	int nFloorSel, nPlcSel =0;
	nFloorSel = m_cbxDestFloor.GetCurSel();
	nPlcSel = m_cbxDestPlc.GetCurSel();

	if(nFloorSel > 0)
	{
		GET(IDC_EDT_NEXT_TRACK, strNextTr);
		nNextTr = _ttoi(strNextTr);
		if (nNextTr <= 0)
			return;

		int nLen = m_pTrack->m_nStationArray.GetSize();
		
		for (int i=0; i<nLen; i++)
		{
			int nStation = m_pTrack->m_nStationArray[i];
			if ((nFloor = m_pTrack->m_nFloorArray[i]) == nFloorSel)
			{
				m_pTrack->m_nNextTrArray[i] = nNextTr;
				m_pTrack->m_nNextPlcArray[i] = m_pDoc->m_nNextPlcNum;
			}
		}

		UpdateList();
	}
	else if(nPlcSel > 0)
	{
		GET(IDC_EDT_NEXT_TRACK, strNextTr);
		nNextTr = _ttoi(strNextTr);
		if (nNextTr <= 0)
			return;

		int nLen = m_pTrack->m_nStationArray.GetSize();
	
		for (int i=0; i<nLen; i++)
		{
			int nPlc = m_pTrack->m_nStationArray[i]/1000;
			int nStation = m_pTrack->m_nStationArray[i];
			if (nPlc == nPlcSel)
			{
				m_pTrack->m_nNextTrArray[i] = nNextTr;
				m_pTrack->m_nNextPlcArray[i] = m_pDoc->m_nNextPlcNum;
			}
		}

		UpdateList();

	}
	
}

void CJobDlg::OnEditchangeComboDestFloor() 
{
	// TODO: Add your control notification handler code here
	
}

void CJobDlg::OnCheckCrossDest() 
{
	UpdateData(TRUE);
	
	if (m_bCrossDest)
	{
		m_bNextCv = FALSE;
		m_bDestPos = FALSE;
		m_bNextCv2 = FALSE;

		m_bCrossTrack = FALSE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
	}

	UpdateData(FALSE);
	
}


void CJobDlg::OnNMClickListJob(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_itemindex = pNMListView->iItem;

	*pResult = 0;
}


void CJobDlg::OnLvnKeydownListJob(NMHDR *pNMHDR, LRESULT *pResult)
{	
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	/*NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_itemindex = pNMListView->iItem;*/

	*pResult = 0;
}


void CJobDlg::OnBnClickedOk()
{
	DestroyWindow();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialog::OnOK();
}


BOOL CJobDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(pMsg->message == WM_KEYDOWN)
	{
	  if(pMsg->wParam == VK_ESCAPE) // ESC키 눌릴 시
	  {
		DestroyWindow();
	    return TRUE;
	  }
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CJobDlg::OnBnClickedBtnRetry2()
{
	int nLen = m_pTrack->m_nStationArray.GetSize();
	
	if(nLen < 0)
	{
		return;
	}
	
	for (int i=0; i<nLen; i++)
	{
		m_pTrack->m_nNextTrArray[i] = 0; //다음트랙
		m_pTrack->m_nNextPlcArray[i] = 0; //다음PLC
	}

	UpdateList();
}
