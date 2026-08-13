// ConfigSub3Page.cpp : implementation file

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "ConfigSub3Page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigSub3Page property page

IMPLEMENT_DYNCREATE(CConfigSub3Page, CPropertyPage)

CConfigSub3Page::CConfigSub3Page() : CPropertyPage(CConfigSub3Page::IDD)
{
	//{{AFX_DATA_INIT(CConfigSub3Page)
	//}}AFX_DATA_INIT
}

CConfigSub3Page::CConfigSub3Page(CEcsDoc* pDoc, CConfig* pConfig) : CPropertyPage(CConfigSub3Page::IDD)
{
	m_pDoc = pDoc;
	m_pConfig = pConfig;
}

CConfigSub3Page::~CConfigSub3Page()
{
}

void CConfigSub3Page::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigSub3Page)
	DDX_Control(pDX, ID_BUTTON_SAVE, m_btnSave);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfigSub3Page, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigSub3Page)
	ON_BN_CLICKED(ID_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_CHECK_MODE, OnCheckMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigSub3Page message handlers

BOOL CConfigSub3Page::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	// 온습도 정보
	CWnd::SetDlgItemText(IDC_EDIT_TEMP_MAX, m_pConfig->m_strTempMax);
	CWnd::SetDlgItemText(IDC_EDIT_TEMP_MIN, m_pConfig->m_strTempMin);
	CWnd::SetDlgItemText(IDC_EDIT_HUM_MAX, m_pConfig->m_strHumMax);
	CWnd::SetDlgItemText(IDC_EDIT_HUM_MIN, m_pConfig->m_strHumMin);
// Update : 2014.06.27 - 입고포트 이동제어
//	CWnd::SetDlgItemText(IDC_EDIT_START_TIME, m_pConfig->m_strStartTime);
// Update : 2014.06.08 - 물류흐름이 원활하지 않은 경우 상온 Aging, 충방전에서 출고된 Tray를 물류 Loop line으로 투입하지 않음.
	// 물류흐름 제어 정보
	CWnd::SetDlgItemText(IDC_EDIT_LOOP_TRACK_COUNT, m_pConfig->m_strLoopTrackCount);
	CWnd::SetDlgItemText(IDC_EDIT_CAPACITY, m_pConfig->m_strCapacity);
	CWnd::CheckDlgButton(IDC_CHECK_MODE, _ttoi(m_pConfig->m_strMode));
	CWnd::CheckDlgButton(IDC_CHECK_AGING_01, _ttoi(m_pConfig->m_strAging01));
	CWnd::CheckDlgButton(IDC_CHECK_AGING_02, _ttoi(m_pConfig->m_strAging02));
	CWnd::CheckDlgButton(IDC_CHECK_AGING_03, _ttoi(m_pConfig->m_strAging03));
	CWnd::CheckDlgButton(IDC_CHECK_AGING_04, _ttoi(m_pConfig->m_strAging04));
	CWnd::CheckDlgButton(IDC_CHECK_AGING_05, _ttoi(m_pConfig->m_strAging05));
	CWnd::CheckDlgButton(IDC_CHECK_AGING_06, _ttoi(m_pConfig->m_strAging06));
	CWnd::CheckDlgButton(IDC_CHECK_CDE_1, _ttoi(m_pConfig->m_strCde01));
	CWnd::CheckDlgButton(IDC_CHECK_CDE_2, _ttoi(m_pConfig->m_strCde02));

	if (CWnd::IsDlgButtonChecked(IDC_CHECK_MODE) > 0)	// Auto
	{
		CWnd::GetDlgItem(IDC_CHECK_AGING_01)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_02)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_03)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_04)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_05)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_06)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_CDE_1)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_CDE_2)->EnableWindow(FALSE);
	}
	else	// Manual
	{
		CWnd::GetDlgItem(IDC_CHECK_AGING_01)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_02)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_03)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_04)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_05)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_06)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_CDE_1)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_CDE_2)->EnableWindow(TRUE);
	}
//---------------------------------------------------------------------------------------
	INIT_BUTTON(m_btnSave, IDI_SAVE, 18, 18, _T("현재 설정된 값으로 저장"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigSub3Page::OnButtonSave()
{
	if (AfxMessageBox(_T("변경된 내용을 저장 하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;

	// 온습도 정보	
	CWnd::GetDlgItemText(IDC_EDIT_TEMP_MAX, m_pConfig->m_strTempMax);
	CWnd::GetDlgItemText(IDC_EDIT_TEMP_MIN, m_pConfig->m_strTempMin);
	CWnd::GetDlgItemText(IDC_EDIT_HUM_MAX, m_pConfig->m_strHumMax);
	CWnd::GetDlgItemText(IDC_EDIT_HUM_MIN, m_pConfig->m_strHumMin);
	// Update : 2014.06.27 - 입고포트 이동제어
	//	CWnd::GetDlgItemText(IDC_EDIT_START_TIME, m_pConfig->m_strStartTime);
	// Update : 2014.06.08 - 물류흐름이 원활하지 않은 경우 상온 Aging, 충방전에서 출고된 Tray를 물류 Loop line으로 투입하지 않음.
		// 물류흐름 제어 정보
	CWnd::GetDlgItemText(IDC_EDIT_LOOP_TRACK_COUNT, m_pConfig->m_strLoopTrackCount);
	CWnd::GetDlgItemText(IDC_EDIT_CAPACITY, m_pConfig->m_strCapacity);
	m_pConfig->m_strMode.Format(_T("%d"), CWnd::IsDlgButtonChecked(IDC_CHECK_MODE));
	if (CWnd::IsDlgButtonChecked(IDC_CHECK_MODE) > 0)	// Auto
	{
		m_pConfig->m_strAging01 = _T("0");
		m_pConfig->m_strAging02 = _T("0");
		m_pConfig->m_strAging03 = _T("0");
		m_pConfig->m_strAging04 = _T("0");
		m_pConfig->m_strAging05 = _T("0");
		m_pConfig->m_strAging06 = _T("0");
		m_pConfig->m_strCde01 = _T("0");
		m_pConfig->m_strCde02 = _T("0");
	}
	else	// Manual
	{
		m_pConfig->m_strAging01.Format(_T("%d"), CWnd::IsDlgButtonChecked(IDC_CHECK_AGING_01));
		m_pConfig->m_strAging02.Format(_T("%d"), CWnd::IsDlgButtonChecked(IDC_CHECK_AGING_02));
		m_pConfig->m_strAging03.Format(_T("%d"), CWnd::IsDlgButtonChecked(IDC_CHECK_AGING_03));
		m_pConfig->m_strAging04.Format(_T("%d"), CWnd::IsDlgButtonChecked(IDC_CHECK_AGING_04));
		m_pConfig->m_strAging05.Format(_T("%d"), CWnd::IsDlgButtonChecked(IDC_CHECK_AGING_05));
		m_pConfig->m_strAging06.Format(_T("%d"), CWnd::IsDlgButtonChecked(IDC_CHECK_AGING_06));
		m_pConfig->m_strCde01.Format(_T("%d"), CWnd::IsDlgButtonChecked(IDC_CHECK_CDE_1));
		m_pConfig->m_strCde02.Format(_T("%d"), CWnd::IsDlgButtonChecked(IDC_CHECK_CDE_2));
	}
	//------------------------------------------------------------------------------------------

	m_pConfig->SaveConfig4();
}

void CConfigSub3Page::SetChildWndFont()
{
	CFont font;
	font.CreateFont (10, 0, 0, 0, 100, FALSE, 0, 0, 1, 0, 0, 0, 0, _T("System") );

	CWnd* pChildWnd = GetWindow(GW_CHILD);
	while(pChildWnd)
	{
		pChildWnd->SetFont(&font);
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CConfigSub3Page::OnCheckMode() 
{
	if (CWnd::IsDlgButtonChecked(IDC_CHECK_MODE) > 0)	// Auto
	{
		CWnd::GetDlgItem(IDC_CHECK_AGING_01)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_02)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_03)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_04)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_05)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_06)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_CDE_1)->EnableWindow(FALSE);
		CWnd::GetDlgItem(IDC_CHECK_CDE_2)->EnableWindow(FALSE);
	}
	else	// Manual
	{
		CWnd::GetDlgItem(IDC_CHECK_AGING_01)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_02)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_03)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_04)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_05)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_AGING_06)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_CDE_1)->EnableWindow(TRUE);
		CWnd::GetDlgItem(IDC_CHECK_CDE_2)->EnableWindow(TRUE);
	}
}