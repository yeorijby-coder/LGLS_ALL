// SystemScTime.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "SystemScTimeDlg.h"
#include "afxdialogex.h"


// CSystemScTimeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSystemScTimeDlg, CSkinDialog)

CSystemScTimeDlg::CSystemScTimeDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CSystemScTimeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bInitialized = FALSE;
	m_pDoc = pDoc;
}

CSystemScTimeDlg::CSystemScTimeDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CSystemScTimeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CSystemScTimeDlg::~CSystemScTimeDlg()
{
	this->DestroyWindow();
}

void CSystemScTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_SYSTEM_SCTIME_OK, m_btnOk);
	DDX_Control(pDX, ID_SYSTEM_SCTIME_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_AGING_TYPE, m_btnAgingType);
	DDX_Control(pDX, IDC_BTN_QRY_SELECT, m_btnQrySelect);
	DDX_Control(pDX, IDC_DRPBX_AGING_TYPE, m_drbtnAgingType);
}


BEGIN_MESSAGE_MAP(CSystemScTimeDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_SYSTEM_SCTIME_OK, &CSystemScTimeDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_SYSTEM_SCTIME_CANCEL, &CSystemScTimeDlg::OnBnClickedSystemSctimeCancel)
	ON_CBN_SELCHANGE(IDC_DRPBX_AGING_TYPE, &CSystemScTimeDlg::OnCbnSelchangeDrpbxAgingType)
	ON_BN_CLICKED(IDC_BTN_QRY_SELECT, &CSystemScTimeDlg::OnBnClickedBtnQrySelect)
	ON_BN_CLICKED(IDC_BTN_AGING_TYPE, &CSystemScTimeDlg::OnBnClickedBtnAgingType)
END_MESSAGE_MAP()



BOOL CSystemScTimeDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	m_tabUsage = (CTabCtrl *)GetDlgItem(IDC_TAB_USAGE);
	if(m_tabUsage == NULL)
	{
		return FALSE;
	}


	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		RelocationControls();
		RedrawImage();
		RenameResource(pEn);

		InitilizeChart(EN_TAB_IDX_CV);
		UpdateChart();
		m_bInitialized = TRUE;		
	}
	 
	InitializeResource(0);
	CWnd::SetTimer(1,3000,NULL);
	//Invalidate(TRUE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSystemScTimeDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\"), _T("dlg_sctime"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\"), _T("dlg_sctime"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectcondition"), (int)m_enLang);
	SetDlgItemText(IDC_GBX_USAGE_SELCONDIT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\"), _T("dlg_sctime"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("condition"), (int)m_enLang);
	SetDlgItemText(IDC_GBX_USAGE_CONDITION, strValue);

}

void CSystemScTimeDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\128x32\\");

	CString strExtension = _T(".png");
 	m_btnAgingType.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
 	m_btnAgingType.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("wh"), strExtension)), NULL, 5, 5);

	m_btnQrySelect.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnQrySelect.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("select"), strExtension)), NULL, 5, 5);

}



void CSystemScTimeDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

HCURSOR CSystemScTimeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSystemScTimeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CSkinDialog::OnPaint();
	}

}

void CSystemScTimeDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

// 	x = rc.Width() - 20 - size.cx;
// 	y = rc.Height() - 10 - size.cy;
// 	m_btnCancel.MoveWindow(x, y, size.cx, size.cy, TRUE);
// 
// 	x -= size.cx + 10;
// 	m_btnOk.MoveWindow(x, y, size.cx, size.cy, TRUE);
// 	m_btnOk.Invalidate();

	CRect rc2;
	m_btnAgingType.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnAgingType.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnQrySelect.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnQrySelect.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

 	m_drbtnAgingType.GetWindowRect(&rc2);
 	ScreenToClient(&rc2);
 	m_drbtnAgingType.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);



}


// CSystemScTimeDlg 메시지 처리기입니다.


void CSystemScTimeDlg::OnBnClickedOk()
{
	CSkinDialog::OnOK();
}


void CSystemScTimeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateChart();
	CSkinDialog::OnTimer(nIDEvent);
}


void CSystemScTimeDlg::OnBnClickedSystemSctimeCancel()
{
	CSkinDialog::OnCancel();
}

void CSystemScTimeDlg::InitilizeChart(EN_TAB_IDX nTabIndex)
{
	CRect rect;
// 	TCITEM* pTabItem = NULL;
// 	if(m_tabUsage->GetItem(nTabIndex, pTabItem))
// 	{
// 		int a = 0;
// 	}
	CStatic * fram = (CStatic *)m_tabUsage;
	if(fram == NULL)
	{
		return;
	}
	fram->GetWindowRect(rect);
	ScreenToClient(&rect);
	m_ntnChart.m_nXCnt = 10;
	m_ntnChart.Create(this,&rect);
	m_ntnChart.ShowWindow(SW_SHOW);
	m_ntnChart.UpdateWindow();
}

void CSystemScTimeDlg::UpdateChart()
{
	CString strSql;
	CStringList listSql;
	strSql.Format(_T(" SELECT SD.SC_NO, COUNT(1) as CNT")
				  _T("	 FROM SC_DATA SD, (SELECT WH_TYP, START_POS, JOB_STATUS ")
				  _T("						 FROM JOB_MST ")
				  _T("						WHERE JOB_STATUS = '%d') JM ")
				  _T("  WHERE SD.WH_TYP = JM.WH_TYP ")
				  _T("    AND SD.SC_NO = JM.START_POS ")
				  _T("  GROUP BY SD.SC_NO"), 10);

	int nCnt = 0;// m_pDB->SelectQry(strSql,listSql);
	if(!(listSql.GetCount() >= 0)){
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("업데이트에 실패했습니다")));
		return;
	}

	REAL test[100];
	ZeroMemory(test,sizeof(REAL) * nCnt);

	int nIdxValue = -1;
	for (POSITION pos = listSql.GetHeadPosition(); pos != NULL;)
	{
		CString strTemp = listSql.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);

		if ( strTemp.IsEmpty() ) continue;
		if(StrFieldName == _T("CNT"))
		{
			nIdxValue++;
			test[nIdxValue] = CConvert::ToInt(strValue);
		}
	}

	m_ntnChart.m_nXCnt = nCnt;
	m_ntnChart.SetData(test, nCnt);
	m_ntnChart.UpdateWindow();
}


void CSystemScTimeDlg::OnCbnSelchangeDrpbxAgingType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int a = 0;
}


void CSystemScTimeDlg::OnBnClickedBtnQrySelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CSystemScTimeDlg::InitializeResource(int pnTabIndex)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\16x16\\");
	CString strExtension = _T(".png");
	
	CImageList imageList;
	imageList.Create(16, 16, TRUE/*bMask*/,	1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cv"), strExtension)));
	imageList.Add(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("sc"), strExtension)));
	m_tabUsage->SetImageList(&imageList);
	imageList.Detach();

	m_tabUsage->InsertItem((int)EN_TAB_IDX_CV, GetTabIdxEnumToCString(EN_TAB_IDX_CV), (int)EN_TAB_IDX_CV);
	m_tabUsage->InsertItem((int)EN_TAB_IDX_SC, GetTabIdxEnumToCString(EN_TAB_IDX_SC), (int)EN_TAB_IDX_SC);


	m_grpUsageCondition.SubclassDlgItem(IDC_GBX_USAGE_SELCONDIT, this);
	m_grpUsageCondition.SetFontSubclass(this->GetFont());
	m_grpUsageCondtion.SubclassDlgItem(IDC_GBX_USAGE_CONDITION, this);
	m_grpUsageCondition.SetFontSubclass(this->GetFont());
}


CString CSystemScTimeDlg::GetTabIdxEnumToCString(EN_TAB_IDX enTabIdx)
{
	CString strReturn = _T("CV");
	if(enTabIdx == EN_TAB_IDX_CV){ return _T("CV");}
	if(enTabIdx == EN_TAB_IDX_SC){ return _T("SC");}
	return strReturn;
}


void CSystemScTimeDlg::OnBnClickedBtnAgingType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int a = 0;
}
