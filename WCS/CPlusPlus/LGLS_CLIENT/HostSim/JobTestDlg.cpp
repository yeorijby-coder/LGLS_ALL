// JobTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "JobTestDlg.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJobTestDlg dialog


CJobTestDlg::CJobTestDlg(CEcsDoc* pDoc, CWnd* pParent /* = NULL */)
	: CDialog(CJobTestDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);

	m_strLocation = "";
	m_strStation = "";
}


void CJobTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJobTestDlg)
	DDX_Control(pDX, IDC_COMBO_STATION, m_cbxStation);
	DDX_Control(pDX, IDC_EDIT_LOCATION, m_edtLocation);
	DDX_Control(pDX, IDC_EDIT_STATION, m_edtStation);
	DDX_Control(pDX, IDOK, m_btnCreate);
	DDX_Control(pDX, IDCANCEL, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJobTestDlg, CDialog)
	//{{AFX_MSG_MAP(CJobTestDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJobTestDlg message handlers

BOOL CJobTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int w = 32, h = 32;
	INIT_BUTTON(m_btnCreate, IDI_EXIT, w, h, _T("닫   기"));
	INIT_BUTTON(m_btnOK, IDI_RUN, w, h, _T("시   작"));

	UpdateStationInfo(1, m_cbxStation);

	m_edtLocation.LimitText(CLib::enLengthLocation);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJobTestDlg::UpdateStationInfo(int nWarehouse, CComboBox& cbxStation)
{
	if (!CJobItem::IsValidWarehouse(nWarehouse))
	{
		AfxMessageBox(_T("유효하지 않은 창고번호입니다!"));
		return;
	}

	cbxStation.ResetContent();
	CStationInfo* pStation = NULL;
	for (int i=0; i<m_pDoc->m_pStationInfos.GetSize(); ++i)
	{
		pStation = m_pDoc->m_pStationInfos[i];
		DEBUGER_ASSERT_VALID(pStation != NULL);

		if (pStation->GetWarehouseNo() != nWarehouse)
			continue;

		int nTemp = cbxStation.GetDlgCtrlID();

		switch (cbxStation.GetDlgCtrlID())
		{
		case IDC_CBX_DEPART:
			if ((pStation->m_enKind == CStationInfo::enStoStation) || (pStation->m_enKind == CStationInfo::enArvStation))
			{
				m_cbxStation.AddString(pStation->m_strName);
				m_cbxStation.SetItemDataPtr(m_cbxStation.GetCount()-1, (void*)pStation);
			}
			break;

		case IDC_CBX_ARRIVE:
			if ((pStation->m_enKind == CStationInfo::enRetStation) || (pStation->m_enKind == CStationInfo::enArvStation))
			{
				m_cbxStation.AddString(pStation->m_strName);
				m_cbxStation.SetItemDataPtr(m_cbxStation.GetCount()-1, (void*)pStation);
			}
			break;

		default:
			DEBUGER_ASSERT_VALID(FALSE);
			return;
		}
	}
}


void CJobTestDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CJobTestDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CStationInfo* pStation = NULL;
	CStationInfo* pLocation = NULL;

	CString strLocation;
	
	m_edtLocation.GetWindowText(strLocation);

//	if(CLib::IsValidLocation(1, strLocation)==FALSE)
//	{
//		AfxMessageBox(_T("Location을 바르게 입력해주세요"));
//		return;
//	}

	int nTemp = m_cbxStation.GetCurSel();
	if (nTemp == -1)
	{
		AfxMessageBox(_T("Station 정보를 선택해 주세요!"));
		return;
	}

	pStation = (CStationInfo*)m_cbxStation.GetItemDataPtr(m_cbxStation.GetCurSel());
//	pLocation = m_pDoc->GetScStationInfo(1, strLocation);

	if (pStation == NULL)
	{
		AfxMessageBox(_T("Station 정보를 선택해 주세요!"));
		return;
	}

//	if (pLocation == NULL)
//	{
//		AfxMessageBox(_T("Location 정보를 가져오지 못했습니다."));
//		return;
//	}

	m_strStation = pStation->m_strID;
//	m_strLocation = strLocation;

	
	CDialog::OnCancel();
}
