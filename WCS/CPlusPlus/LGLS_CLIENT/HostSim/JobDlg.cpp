// JobDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "JobDlg.h"
#include "JobItemDlg.h"
#include "JobOfflineDlg.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJobDlg dialog


CJobDlg::CJobDlg(CEcsDoc* pDoc, CWnd* pParent /* = NULL */)
	: CDialog(CJobDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	DEBUGER_ASSERT_VALID(m_pDoc->m_pJob != NULL);
	DEBUGER_ASSERT_VALID(m_pDoc->m_pLog != NULL);
	
	//{{AFX_DATA_INIT(CJobDlg)
	m_bAutoRefresh = FALSE;
	m_nFindLuggNum = 0;
	m_uElapse = 1000;
	//}}AFX_DATA_INIT

	m_bPairSelected = FALSE;
}


void CJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJobDlg)
	DDX_Control(pDX, IDC_CBX_FILTER_IDLE_JOB, m_cbxFilterIdleJob);
	DDX_Control(pDX, IDC_CBX_FILTER_DEST_STATION, m_cbxFilterDestStn);
	DDX_Control(pDX, IDC_CBX_FILTER_START_STATION, m_cbxFilterStartStn);
	DDX_Control(pDX, IDC_CBX_FILTER_JOB_TYPE, m_cbxFilterJobType);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_COPY, m_btnCopy);
	DDX_Control(pDX, IDC_BTN_PAIR, m_btnPair);
	DDX_Control(pDX, IDC_BTN_ARRIVE, m_btnArrive);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_COMPLETE, m_btnComplete);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BTN_FIND, m_btnFind);
	DDX_Control(pDX, IDC_TAB_JOB, m_tabJobKind);
	DDX_Control(pDX, IDC_LIST_JOB, m_lstJobList);
	DDX_Check(pDX, IDC_CHK_AUTO_REFRESH, m_bAutoRefresh);
	DDX_Text(pDX, IDC_EDT_LUGG_NUM, m_nFindLuggNum);
	DDX_Text(pDX, IDC_EDT_REFRESH_TIME, m_uElapse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJobDlg, CDialog)
	//{{AFX_MSG_MAP(CJobDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_FIND, OnBtnFindFromLuggNum)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAddOfflineJob)
	ON_BN_CLICKED(IDC_BTN_COMPLETE, OnBtnComplete)
	ON_BN_CLICKED(IDC_BTN_ARRIVE, OnBtnArrive)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_BN_CLICKED(IDC_CHK_AUTO_REFRESH, OnChkAutoRefresh)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_JOB, OnDblclkListJob)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_JOB, OnSelchangeTabJob)
	ON_CBN_SELCHANGE(IDC_CBX_FILTER_JOB_TYPE, OnSelchangeCbxFilterJobType)
	ON_CBN_SELCHANGE(IDC_CBX_FILTER_START_STATION, OnSelchangeCbxFilterStartStation)
	ON_CBN_SELCHANGE(IDC_CBX_FILTER_DEST_STATION, OnSelchangeCbxFilterDestStation)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_COPY, OnBtnCopy)
	ON_BN_CLICKED(IDC_BTN_PAIR, OnBtnPair)
	ON_CBN_SELCHANGE(IDC_CBX_FILTER_IDLE_JOB, OnSelchangeCbxFilterIdleJob)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJobDlg message handlers

BOOL CJobDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CDialog::CenterWindow();

	int w = 16, h = 16;
	INIT_BUTTON(m_btnFind, IDI_FIND, w, h, _T("작업번호로 찾기"));
	INIT_BUTTON(m_btnRefresh, IDI_ICON_RETURN, w, h, _T("작업목록 최신정보로 갱신"));
	INIT_BUTTON(m_btnAdd, IDI_ICON_MANUAL, w, h, _T("반자동 작업 추가"));
	INIT_BUTTON(m_btnComplete, IDI_ICON_COMPLETE, w, h, _T("SC 작업 완료보고"));
	INIT_BUTTON(m_btnCancel, IDI_CANCEL, w, h, _T(""));
	INIT_BUTTON(m_btnArrive, IDI_RUN, w, h, _T(""));
	INIT_BUTTON(m_btnCopy, IDI_ICON_COPY, w, h, _T(""));
	INIT_BUTTON(m_btnPair, IDI_ICON_COPY, w, h, _T(""));
	INIT_BUTTON(m_btnOK, IDI_EXIT, w, h, _T("창닫기"));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	CImageList imageList;
	imageList.Create(16, 16, TRUE, 1, 1);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_JOB_ALL));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_JOB_NEW));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_JOB_PROGRESS));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_JOB_COMPLETE));
	imageList.Add(AfxGetApp()->LoadIcon(IDI_JOB_ERROR));
//	imageList.Add(AfxGetApp()->LoadIcon(IDI_JOB_PLAN));
	m_lstJobList.SetImageList(&imageList, LVSIL_SMALL);
	m_tabJobKind.SetImageList(&imageList);
	imageList.Detach();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	m_tabJobKind.InsertItem(0, _T("전체"), 0);
	m_tabJobKind.InsertItem(1, _T("신규"), 1);
	m_tabJobKind.InsertItem(2, _T("진행"), 2);
	m_tabJobKind.InsertItem(3, _T("완료"), 3);
	m_tabJobKind.InsertItem(4, _T("에러"), 4);
//	m_tabJobKind.InsertItem(5, _T("미사용"), 5);
	m_tabJobKind.SetCurSel(0);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int i = 0;
	const int size			= 10;
	UINT	mask			= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	int		fmt				= LVCFMT_LEFT;
	int		cx[size]		= {80, 100, 160, 160, 110, 80, 40, 200, 80, 160};
	LPTSTR	pszText[size]	= {_T("작업번호"), _T("작업구분"), _T("출발위치"), _T("도착위치"), _T("진행상태"), _T("RGVC"), _T("CAR"), _T("RGV 상태"), _T("설비사용"), _T("작업지시시간")};
	for (i=0; i<size; ++i)
		m_lstJobList.InsertColumn(i, (LPCTSTR)pszText[i], fmt, cx[i]);
	m_lstJobList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_lstJobList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	UpdateList();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_cbxFilterJobType.AddString(_T("전체"));
	m_cbxFilterJobType.SetItemData(0, 0);
	m_cbxFilterJobType.SetCurSel(0);
	CLib::SetComboBoxJobType(m_cbxFilterJobType);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_cbxFilterStartStn.AddString(_T("전체"));
	m_cbxFilterStartStn.SetItemDataPtr(0, NULL);

	m_cbxFilterDestStn.AddString(_T("전체"));
	m_cbxFilterDestStn.SetItemDataPtr(0, NULL);

	m_cbxFilterIdleJob.AddString(_T("전체"));
	m_cbxFilterIdleJob.SetItemData(0, 0);
	m_cbxFilterIdleJob.AddString(_T("미사용"));
	m_cbxFilterIdleJob.SetItemData(1, 1);
	m_cbxFilterIdleJob.AddString(_T("사용"));
	m_cbxFilterIdleJob.SetItemData(2, 2);
	m_cbxFilterIdleJob.SetCurSel(0);

	CStationInfo* pStation = NULL;
	for (i=0; i<m_pDoc->m_pStationInfos.GetSize(); ++i)
	{
		pStation = m_pDoc->m_pStationInfos[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		switch (pStation->m_enKind)
		{
		case CStationInfo::enScStation:
		case CStationInfo::enArvStation:
			m_cbxFilterStartStn.AddString(pStation->m_strName);
			m_cbxFilterStartStn.SetItemDataPtr(m_cbxFilterStartStn.GetCount()-1, pStation);
			m_cbxFilterDestStn.AddString(pStation->m_strName);
			m_cbxFilterDestStn.SetItemDataPtr(m_cbxFilterDestStn.GetCount()-1, pStation);
			break;

		case CStationInfo::enStoStation:
			m_cbxFilterStartStn.AddString(pStation->m_strName);
			m_cbxFilterStartStn.SetItemDataPtr(m_cbxFilterStartStn.GetCount()-1, pStation);
			break;

		case CStationInfo::enRetStation:
			m_cbxFilterDestStn.AddString(pStation->m_strName);
			m_cbxFilterDestStn.SetItemDataPtr(m_cbxFilterDestStn.GetCount()-1, pStation);
			break;

		}
	}
	m_cbxFilterStartStn.SetCurSel(0);
	m_cbxFilterDestStn.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJobDlg::UpdateList()
{
	CJobItem* pJobItem = NULL;
	CJob* pJob = m_pDoc->m_pJob;
	DEBUGER_ASSERT_VALID(pJob != NULL);

	int i = 0, j = 0, nImage = 0;
	int nFilterJobKind = m_tabJobKind.GetCurSel();
	int nFilterJobType = (m_cbxFilterJobType.GetCurSel() > 0) ? m_cbxFilterJobType.GetItemData(m_cbxFilterJobType.GetCurSel()) : 0;
	CStationInfo* pFilterStartStn = (m_cbxFilterStartStn.GetCurSel() > 0) ? (CStationInfo*)m_cbxFilterStartStn.GetItemDataPtr(m_cbxFilterStartStn.GetCurSel()) : NULL;
	CStationInfo* pFilterDestStn = (m_cbxFilterDestStn.GetCurSel() > 0) ? (CStationInfo*)m_cbxFilterDestStn.GetItemDataPtr(m_cbxFilterDestStn.GetCurSel()) : NULL;
	UINT uMask = LVIF_TEXT | LVIF_IMAGE;

	int nFilterIdleJob = (m_cbxFilterIdleJob.GetCurSel() > 0) ? m_cbxFilterIdleJob.GetItemData(m_cbxFilterIdleJob.GetCurSel()) : 0;

//	CSingleLock SingleLock(&pJob->m_csSyncJobList);
//	SingleLock.Lock();

	CString strTemp;
	strTemp.Format(_T("%d Pair"), m_pDoc->m_nPairLuggNum);
	strTemp = (m_bPairSelected) ? strTemp : "(Pair) 선택";
	SetDlgItemText(IDC_BTN_PAIR, strTemp);

	m_lstJobList.DeleteAllItems();
	for (POSITION pos=pJob->GetHeadPosition(); pos!=NULL; j=0)
	{
		pJobItem = (CJobItem*)pJob->GetNext(pos);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		switch (pJobItem->m_nJobStatus)
		{
		case enJobStatusNew:
		case enJobStatusCvNew:
		case enJobStatusScRequest:
			nImage = enKindNew;
			if ((nFilterJobKind != enKindAll) && (nFilterJobKind != enKindNew))
				continue;
			break;

		case enJobStatusComplete:
		case enJobStatusArrived:
		case enJobStatusCancel:
			nImage = enKindComplete;
			if ((nFilterJobKind != enKindAll) && (nFilterJobKind != enKindComplete))
				continue;
			break;

		case enJobStatusErrorEmptyRetrieve:
		case enJobStatusErrorDualStore:
			nImage = enKindError;
			if ((nFilterJobKind != enKindAll) && (nFilterJobKind != enKindError))
				continue;
			break;

		default:
			nImage = enKindProgress;
			if ((nFilterJobKind != enKindAll) && (nFilterJobKind != enKindProgress))
				continue;
			break;
		}

		if ((nFilterJobType > 0) && (nFilterJobType != pJobItem->m_nJobType))
			continue;

		if ((pFilterStartStn != NULL) && (pFilterStartStn->m_strID != pJobItem->m_strStartStn))
			continue;

		if ((pFilterDestStn != NULL) && (pFilterDestStn->m_strID != pJobItem->m_strDestStn))
			continue;

//		if (nFilterJobKind == enKindNotUsed)
//		{
//			if (m_pDoc->IsIdleJob(pJobItem->m_nLuggNum) == FALSE)
//				continue;
//		}

		// 미사용:1, 사용:2
		if (nFilterIdleJob > 0)
		{
			int nIdleJob = (m_pDoc->IsIdleJob(pJobItem->m_nLuggNum)) ? 1 : 2;
			if (nFilterIdleJob != nIdleJob)
				continue;
		}
		
		m_lstJobList.InsertItem(uMask, i, CConvert::ToString(pJobItem->m_nLuggNum), 0, 0, nImage, 0);
		m_lstJobList.SetItem(i, ++j, uMask, pJobItem->GetJobTypeString(), nImage, 0, 0, 0);
		m_lstJobList.SetItem(i, ++j, uMask, pJobItem->GetStartStationString(), nImage, 0, 0, 0);
		m_lstJobList.SetItem(i, ++j, uMask, pJobItem->GetDestStationString(), nImage, 0, 0, 0);
		m_lstJobList.SetItem(i, ++j, uMask, pJobItem->GetJobStatusString(), nImage, 0, 0, 0);
//============================================================================================================
//		2011.01.24 날짜 및 시간 추가	
//============================================================================================================		CConvert::ToString(pJobItem->m_nRgvcNum)
		m_lstJobList.SetItem(i, ++j, uMask, (pJobItem->m_nJobStatus == enJobStatusRtvInvoke) ? ((pJobItem->m_nRgvcNum == 1) ? _T("STORE"): (pJobItem->m_nRgvcNum == 2) ? _T("RETRIEVE"): _T("0")) : _T(""), nImage, 0, 0, 0);
		m_lstJobList.SetItem(i, ++j, uMask, (pJobItem->m_nJobStatus == enJobStatusRtvInvoke) ? CConvert::ToString(pJobItem->m_nVehicleID) : _T("0") , nImage, 0, 0, 0);
		m_lstJobList.SetItem(i, ++j, uMask, (pJobItem->m_nJobStatus == enJobStatusRtvInvoke) ? pJobItem->GetRgvStatusString() : _T("") , nImage, 0, 0, 0);
 		m_lstJobList.SetItem(i, ++j, uMask, m_pDoc->IsIdleJob(pJobItem->m_nLuggNum) ? _T("미사용") : _T("사용"), nImage, 0, 0, 0);
		m_lstJobList.SetItem(i, ++j, uMask, pJobItem->GetDateTime(), nImage, 0, 0, 0);
//------------------------------------------------------------------------------------------------------------
		m_lstJobList.SetItemData(i++, (LPARAM)pJobItem);
	}

	CString strValue;
	strValue.Format(_T("%d / %d"), m_lstJobList.GetItemCount(), pJob->GetCount());
	SetDlgItemText(IDC_EDT_COUNT, strValue);
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
	int nJobCount = m_pDoc->m_pJob->GetCount();
	
	CJobOfflineDlg dlg(m_pDoc, this);
	dlg.DoModal();

	if (m_pDoc->m_pJob->GetCount() != nJobCount)
		UpdateList();
}

void CJobDlg::OnBtnComplete() 
{
	CJobItem* pJobItem = GetSelectJobItem();
	if (pJobItem == NULL)
	{
		AfxMessageBox(_T("목록에서 작업을 선택해주세요!"));
		return;
	}

	if (pJobItem->IsValidJobItem() == FALSE)
	{
		AfxMessageBox(_T("서버와 싱크가 맞지 않습니다. \n이미 완료 처리된 작업일수 있습니다. \n최신정보를 클릭해주세요"));
		return;
	}

	if (pJobItem->m_pDoc == NULL) pJobItem->m_pDoc = m_pDoc;
	CJobItemDlg dlg(pJobItem, CJobItemDlg::enComplete, this);
	if (dlg.DoModal() == IDOK)
		UpdateList();
}

void CJobDlg::OnBtnArrive() 
{
	CJobItem* pJobItem = GetSelectJobItem();
	if (pJobItem == NULL)
	{
		AfxMessageBox(_T("목록에서 작업을 선택해주세요!"));
		return;
	}

	if (pJobItem->IsValidJobItem() == FALSE)
	{
		AfxMessageBox(_T("서버와 싱크가 맞지 않습니다. \n이미 도착 보고된 작업일수 있습니다. \n최신정보를 클릭해주세요"));
		return;
	}

	if (pJobItem->m_pDoc == NULL) pJobItem->m_pDoc = m_pDoc;
	CJobItemDlg dlg(pJobItem, CJobItemDlg::enArrive, this);
	if (dlg.DoModal() == IDOK)
		UpdateList();
}

void CJobDlg::OnBtnCancel() 
{
	CJobItem* pJobItem = GetSelectJobItem();
	if (pJobItem == NULL)
	{
		AfxMessageBox(_T("목록에서 작업을 선택해주세요!"));
		return;
	}

	if (pJobItem->IsValidJobItem() == FALSE)
	{
		AfxMessageBox(_T("서버와 싱크가 맞지 않습니다. \n이미 삭제된 작업일수 있습니다. \n최신정보를 클릭해주세요"));
		return;
	}

	if (pJobItem->m_pDoc == NULL) pJobItem->m_pDoc = m_pDoc;
	CJobItemDlg dlg(pJobItem, CJobItemDlg::enCancel, this);
	if (dlg.DoModal() == IDOK)
		UpdateList();
}

void CJobDlg::OnDblclkListJob(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CJobItem* pJobItem = GetSelectJobItem();
	if (pJobItem == NULL)
	{
		AfxMessageBox(_T("목록에서 작업을 선택해주세요!"));
		return;
	}

	if (pJobItem->IsValidJobItem() == FALSE)
	{
		AfxMessageBox(_T("서버와 싱크가 맞지 않습니다. \n이미 삭제된 작업일수 있습니다. \n최신정보를 클릭해주세요"));
		return;
	}
	
	if (pJobItem->m_pDoc == NULL) pJobItem->m_pDoc = m_pDoc;
	CJobItemDlg dlg(pJobItem, CJobItemDlg::enModify, this);
	if (dlg.DoModal() == IDOK)
		UpdateList();
	
	*pResult = 0;
}

void CJobDlg::OnBtnCopy() 
{
	CJobItem* pJobItem = GetSelectJobItem();
	if (pJobItem == NULL)
	{
		AfxMessageBox(_T("목록에서 작업을 선택해주세요!"));
		return;
	}

	if (pJobItem->IsValidJobItem() == FALSE)
	{
		AfxMessageBox(_T("서버와 싱크가 맞지 않습니다. \n이미 삭제된 작업일수 있습니다. \n최신정보를 클릭해주세요"));
		return;
	}

	CStationInfo* pStation = m_pDoc->GetStationInfo(pJobItem->m_strDestStn);
	if (pStation == NULL)
	{
		AfxMessageBox(_T("도착위치가 유효하지 않은 STATION입니다. 확인해주세요!"));
		return;
	}

	m_pDoc->m_oCvDlg.m_nLuggNum		= pJobItem->m_nLuggNum;
	m_pDoc->m_oCvDlg.m_nJobType		= pJobItem->m_nJobType;
	m_pDoc->m_oCvDlg.m_nDestPos		= pStation->GetTrackDestination();
	m_pDoc->m_oCvDlg.m_ucGenCode	= pJobItem->m_ucGenCode;
	m_pDoc->m_oCvDlg.m_nSize		= pJobItem->m_nSize;
	m_pDoc->m_oCvDlg.m_bClone		= TRUE;

	m_pDoc->m_oCvDlg.m_btnPaste.EnableWindow(TRUE);

	AfxMessageBox(_T("트랙정보를 복사하였습니다. 트랙에 붙여넣기를 하세요!"));
}

void CJobDlg::OnBtnFindFromLuggNum() 
{
	if (UpdateData(TRUE) == FALSE)
		return;

	CJobItem* pJobItem = NULL;
	for (int i=0; i<m_lstJobList.GetItemCount(); ++i)
	{
		pJobItem = (CJobItem*)m_lstJobList.GetItemData(i);
		DEBUGER_ASSERT_VALID(pJobItem != NULL);

		if (pJobItem->m_nLuggNum == m_nFindLuggNum)
		{
			m_lstJobList.SetItemState(i, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
			m_lstJobList.EnsureVisible(i, FALSE);
			m_lstJobList.SetFocus();
			return;
		}
	}

	AfxMessageBox(_T("해당 작업번호가 존재하지 않습니다."));
}

void CJobDlg::OnBtnRefresh() 
{
	UpdateList();
}

void CJobDlg::OnChkAutoRefresh() 
{
	if (UpdateData(TRUE) == FALSE)
		return;

	if (!m_bAutoRefresh) KillTimer(1);
	else SetTimer(1, m_uElapse, NULL);

	GetDlgItem(IDC_EDT_REFRESH_TIME)->EnableWindow(!m_bAutoRefresh);
	GetDlgItem(IDC_BTN_REFRESH)->EnableWindow(!m_bAutoRefresh);
	UpdateData(FALSE);
}

void CJobDlg::OnTimer(UINT_PTR  nIDEvent) 
{
	if (nIDEvent == 1)
		UpdateList();
	
	CDialog::OnTimer(nIDEvent);
}

void CJobDlg::OnSelchangeTabJob(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateList();
	
	*pResult = 0;
}

void CJobDlg::OnSelchangeCbxFilterJobType() 
{
	UpdateList();
}

void CJobDlg::OnSelchangeCbxFilterStartStation() 
{
	UpdateList();
}

void CJobDlg::OnSelchangeCbxFilterDestStation() 
{
	UpdateList();
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

void CJobDlg::OnCancel()
{
	DestroyWindow();
//	CDialog::OnCancel();
}

void CJobDlg::OnOK() 
{
	DestroyWindow();
//	CDialog::OnOK();
}

void CJobDlg::OnBtnPair() 
{
	// TODO: Add your control notification handler code here
	CJobItem* pJobItem = GetSelectJobItem();
	if (pJobItem == NULL)
	{
		AfxMessageBox(_T("목록에서 작업을 선택해주세요!"));
		return;
	}

	if(m_bPairSelected == FALSE)
	{
		m_pDoc->m_nPairLuggNum = pJobItem->m_nLuggNum;
		m_bPairSelected = TRUE;
		OnBtnRefresh(); 
	}
	else
	{
		m_bPairSelected = FALSE;
		CJobItem* pPairJobItem = m_pDoc->m_pJob->Find(m_pDoc->m_nPairLuggNum);
		if (pPairJobItem == NULL)
		{
			return;
		}
		else
		{
			pPairJobItem->m_nPairLuggNo = pJobItem->m_nLuggNum;
			pJobItem->m_nPairLuggNo = m_pDoc->m_nPairLuggNum;
			OnBtnRefresh(); 
		}
	}
}

void CJobDlg::OnSelchangeCbxFilterIdleJob() 
{
	// TODO: Add your control notification handler code here
	UpdateList();	
}
