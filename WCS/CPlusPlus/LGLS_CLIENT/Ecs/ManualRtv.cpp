// ManualRtv.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "ManualRtv.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CManualRtv 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualRtv, CSkinDialog)
CManualRtv::CManualRtv(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CManualRtv::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CManualRtv::CManualRtv(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent)
	: CSkinDialog(CManualRtv::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pDB = pDb;
}
CManualRtv::~CManualRtv()
{
}

void CManualRtv::PostNcDestroy()
{
	if (m_pDoc != NULL) m_pDoc->m_pManualRtv = NULL;
	CSkinDialog::PostNcDestroy();
	delete this;
}

void CManualRtv::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBL_RTV_MANUAL_RTV_NO,		 m_lblRtvManaulRtvNo);
	DDX_Control(pDX, IDC_LBL_RTV_DEP_FORK1,			 m_lblRtvManaulDepFork1);
	DDX_Control(pDX, IDC_LBL_RTV_DEP_FORK2,			 m_lblRtvManaulDepFork2);
	DDX_Control(pDX, IDC_LBL_RTV_ARR_FORK1,			 m_lblRtvManaulArrFork1);
	DDX_Control(pDX, IDC_LBL_RTV_ARR_FORK2,			 m_lblRtvManaulArrFork2);

	DDX_Control(pDX, IDC_BTN_RTV_MANUAL_SAVE,		 m_btnRtvManualSave);

	DDX_Control(pDX, IDC_CMB_RTV_MANUAL_RTV_NO,		m_cbxRtvNo);
	DDX_Control(pDX, IDC_CMB_RTV_ARR_FORK1,			m_cbxArrFork1);
	DDX_Control(pDX, IDC_CMB_RTV_ARR_FORK2,			m_cbxArrFork2);
	DDX_Control(pDX, IDC_CMB_RTV_DEP_FORK1,			m_cbxDepFork1);
	DDX_Control(pDX, IDC_CMB_RTV_DEP_FORK2,			m_cbxDepFork2);

	// [LGLS 2026-09-03] 1 FORK 체크박스 제거(포크1 고정) - DDX_Control(pDX, IDC_CHK_RTV_FORK1,				m_btnFork1);
	DDX_Control(pDX, IDC_CHK_RTV_FORK2,				m_btnFork2);
	DDX_Control(pDX, IDC_CHK_RTV_FORK12,			m_btnFork1Fork2);

	DDX_Control(pDX, IDC_GRP_RTV_MANUAL_INFO,		m_grpRtvInfo);
	DDX_Control(pDX, IDC_GRP_RTV_FORK1,				m_grpRtvManaulFork1);
	DDX_Control(pDX, IDC_GRP_RTV_FORK2,				m_grpRtvManaulFork2);
	// [LGLS 2026-09-03] 명령 그룹박스를 리소스에서 뺐다(DDX 어설션 방지) - DDX_Control(pDX, IDC_GRP_RTV_MANUAL_COMMAND,	m_grpRtvManualCommand);
}


BEGIN_MESSAGE_MAP(CManualRtv, CSkinDialog)
	ON_BN_CLICKED(IDCANCEL, &CManualRtv::OnBnClickedCancel)
	//	ON_WM_CLOSE()
	ON_WM_CLOSE()
	// [LGLS 2026-09-03] 1 FORK 체크박스 제거 - ON_BN_CLICKED(IDC_CHK_RTV_FORK1, &CManualRtv::OnBnClickedChkRtvFork1)
	ON_BN_CLICKED(IDC_CHK_RTV_FORK2, &CManualRtv::OnBnClickedChkRtvFork2)
	ON_BN_CLICKED(IDC_BTN_RTV_MANUAL_SAVE, &CManualRtv::OnBnClickedBtnRtvManualSave)
	ON_BN_CLICKED(IDC_CHK_RTV_FORK12, &CManualRtv::OnBnClickedChkRtvFork12)
END_MESSAGE_MAP()

BOOL CManualRtv::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		RelocationControls();
		m_bInitialized = TRUE;		
	}
	// [LGLS 2026-09-03] RGV 는 싱글 포크 - 포크1 고정, 1 FORK 체크박스와 명령 그룹은 화면에서 뺐다
	m_strRtvFork = _T("1");
	CLib::BindCombo_RTV_NO(m_cbxRtvNo,  m_pDoc,(int)pEn);
	m_cbxRtvNo.SetCurSel(0);
	// [LGLS 2026-09-03] 출발지/도착지를 트랙번호로 표시(구 COMMON_CODE 1~6 대신 실제 RGV 정차 트랙)
	CLib::BindCombo_RTV_TRACK(m_cbxArrFork1, m_pDoc,(int)pEn);
	CLib::BindCombo_RTV_TRACK(m_cbxArrFork2, m_pDoc,(int)pEn);
	CLib::BindCombo_RTV_TRACK(m_cbxDepFork1, m_pDoc,(int)pEn);
	CLib::BindCombo_RTV_TRACK(m_cbxDepFork2, m_pDoc,(int)pEn);
	m_cbxArrFork1.EnableWindow(TRUE);
	m_cbxDepFork1.EnableWindow(TRUE);


	// [LGLS] RTV는 Fork1만 사용 ? Fork2 컨트롤 숨김, Fork1 고정
	m_lblRtvManaulDepFork2.ShowWindow(SW_HIDE);
	m_lblRtvManaulArrFork2.ShowWindow(SW_HIDE);
	m_cbxArrFork2.ShowWindow(SW_HIDE);
	m_cbxDepFork2.ShowWindow(SW_HIDE);
	m_btnFork2.ShowWindow(SW_HIDE);
	m_btnFork1Fork2.ShowWindow(SW_HIDE);
	m_grpRtvManaulFork2.ShowWindow(SW_HIDE);
	// [LGLS 2026-09-03] 1 FORK 체크박스는 리소스에서 뺐다(포크1 고정) - 창 열 때 어설션 원인
	if (::IsWindow(m_btnFork1.GetSafeHwnd())) m_btnFork1.SetCheck(1);
	m_cbxArrFork1.EnableWindow(TRUE);
	m_cbxDepFork1.EnableWindow(TRUE);

	InitializeResource(pEn);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManualRtv::OnBnClickedCancel()
{
	//OnClose();
}

void CManualRtv::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnRtvManualSave.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvManualSave.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

}

void CManualRtv::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;

	m_btnRtvManualSave.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvManualSave.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);
	m_btnRtvManualSave.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnRtvManualSave.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);	
}

void CManualRtv::InitializeResource(int pnLang)
{	//
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

	RenameResource((EN_LANG)pnLang);
	RedrawImage();
}

void CManualRtv::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	{ CString rtvSel; m_cbxRtvNo.GetWindowText(rtvSel); if(!rtvSel.IsEmpty()){ CString tt; tt.Format(_T("%s - %s"), strValue, rtvSel); strValue=tt; } }
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rtvno"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_MANUAL_RTV_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_DEP_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_ARR_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_DEP_FORK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_ARR_FORK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("order"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_RTV_MANUAL_SAVE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("info"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_MANUAL_INFO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork1"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork2"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_FORK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtvmanual\\"), _T("dlg_rtvmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	// [LGLS 2026-09-03] 명령 그룹박스 제거 - if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_MANUAL_COMMAND, strValue);
}

void CManualRtv::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//m_pDoc->m_pRtvSkinManualDlg = NULL;
	DestroyWindow();
}


void CManualRtv::OnBnClickedChkRtvFork1()
{

	UpdateData(TRUE);

	if (::IsWindow(m_btnFork1.GetSafeHwnd()) && m_btnFork1.GetCheck() == 1)
	{
		m_strRtvFork = _T("1");
		//Enable 처리
		m_cbxArrFork1.EnableWindow(TRUE);
		m_cbxDepFork1.EnableWindow(TRUE);
		m_cbxArrFork2.EnableWindow(FALSE);
		m_cbxDepFork2.EnableWindow(FALSE);
		m_btnFork2.EnableWindow(FALSE);
		m_btnFork1Fork2.EnableWindow(FALSE);
	}else{
		m_strRtvFork = _T("0");
		m_cbxArrFork1.EnableWindow(FALSE);
		m_cbxDepFork1.EnableWindow(FALSE);
		m_btnFork2.EnableWindow(TRUE);
		m_btnFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}


void CManualRtv::OnBnClickedChkRtvFork2()
{
	//나중
	UpdateData(TRUE);

	if (m_btnFork2.GetCheck() == 1)
	{
		m_strRtvFork = _T("2");
		m_cbxArrFork2.EnableWindow(TRUE);
		m_cbxDepFork2.EnableWindow(TRUE);
		m_cbxArrFork1.EnableWindow(FALSE);
		m_cbxDepFork1.EnableWindow(FALSE);
		if (::IsWindow(m_btnFork1.GetSafeHwnd())) m_btnFork1.EnableWindow(FALSE);
		m_btnFork1Fork2.EnableWindow(FALSE);
	}else{
		m_strRtvFork = _T("0");
		m_cbxArrFork2.EnableWindow(FALSE);
		m_cbxDepFork2.EnableWindow(FALSE);
		if (::IsWindow(m_btnFork1.GetSafeHwnd())) m_btnFork1.EnableWindow(TRUE);
		m_btnFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}

void CManualRtv::OnBnClickedChkRtvFork12()
{
	UpdateData(TRUE);

	if (m_btnFork1Fork2.GetCheck() == 1)
	{
		m_strRtvFork = _T("3");
		m_cbxArrFork2.EnableWindow(TRUE);
		m_cbxDepFork2.EnableWindow(TRUE);
		m_cbxArrFork1.EnableWindow(TRUE);
		m_cbxDepFork1.EnableWindow(TRUE);
		if (::IsWindow(m_btnFork1.GetSafeHwnd())) m_btnFork1.EnableWindow(FALSE);
		m_btnFork2.EnableWindow(FALSE);
	}else{
		m_strRtvFork = _T("0");
		m_cbxArrFork1.EnableWindow(FALSE);
		m_cbxDepFork1.EnableWindow(FALSE);
		m_cbxArrFork2.EnableWindow(FALSE);
		m_cbxDepFork2.EnableWindow(FALSE);
		if (::IsWindow(m_btnFork1.GetSafeHwnd())) m_btnFork1.EnableWindow(TRUE);
		m_btnFork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}

void CManualRtv::OnBnClickedBtnRtvManualSave()
{
	CString strSql=_T("");
	CString strRtvNo = _T("");
	CString strArrFork1=_T("");
	CString strArrFork2=_T("");
	CString strDepFork1=_T("");
	CString strDepFork2=_T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;

	UpdateData(TRUE);

	m_cbxRtvNo.GetWindowText(strRtvNo);
	// [LGLS 2026-09-03] 표시는 "103 (이름)" 이므로 지시값은 항목키(트랙번호)에서 가져온다.
	strArrFork1 = m_cbxArrFork1.GetItemKey(m_cbxArrFork1.GetCurSel());
	strArrFork2 = m_cbxArrFork2.GetItemKey(m_cbxArrFork2.GetCurSel());
	strDepFork1 = m_cbxDepFork1.GetItemKey(m_cbxDepFork1.GetCurSel());
	strDepFork2 = m_cbxDepFork2.GetItemKey(m_cbxDepFork2.GetCurSel());
	strArrFork1.Trim(); strArrFork2.Trim(); strDepFork1.Trim(); strDepFork2.Trim();

	strRtvNo = m_cbxRtvNo.GetItemKey(m_cbxRtvNo.GetCurSel());

	strRtvNo.Trim();

	if (m_strRtvFork == _T("0")){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("포크를 선택하세요"))); return;}
	if (strRtvNo == _T("")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("RTV 호기를 선택하세요"))); return;}

	m_pDoc->BeginTrans_DLG();

	if (m_strRtvFork == _T("1"))
	{
		if (strDepFork1 == _T("")){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발지 포크1을 선택하세요"))); return;}
		if (strArrFork1 == _T("")){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지 포크1을 선택하세요"))); return;}

		strSql.Format(_T(" UPDATE RTV_DATA_LGLS						\n")
			_T("    SET DEPART_TRACK = '%s'		\n")
			_T("      , ARRIVE_TRACK = '%s'		\n")
			_T("      , JOB_TYP_OD = '3'              \n")
			_T("      , LUGG_OD = '9998'     \n")
			_T("	  , OD_RQ_YN = 'Y'			    \n")
			_T("  WHERE WH_TYP = '%s'					\n")
			_T("    AND RTV_NO = '%s'			"), strDepFork1, strArrFork1, strWhTyp, strRtvNo);

	}else if(m_strRtvFork == _T("2")){

		if (strDepFork2 == _T("")){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발지 포크2을 선택하세요"))); return;}
		if (strArrFork2 == _T("")){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지 포크2을 선택하세요"))); return;}

		strSql.Format(_T(" UPDATE RTV_DATA_LGLS						\n")
			_T("    SET DEPART_TRACK = '%s'		\n")
			_T("      , ARRIVE_TRACK = '%s'		\n")
			_T("      , JOB_TYP_OD = '3'              \n")
			_T("      , LUGG_OD = '9998'     \n")
			_T("	  , OD_RQ_YN = 'Y'			    \n")
			_T("  WHERE WH_TYP = '%s'					\n")
			_T("    AND RTV_NO = '%s'			"), strDepFork2, strArrFork2, strWhTyp, strRtvNo);
	}else{

		if (strDepFork1 == _T("")){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발지 포크1을 선택하세요"))); return;}
		if (strArrFork1 == _T("")){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지 포크1을 선택하세요"))); return;}
		if (strDepFork2 == _T("")){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발지 포크2을 선택하세요"))); return;}
		if (strArrFork2 == _T("")){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지 포크2을 선택하세요"))); return;}

		strSql.Format(_T(" UPDATE RTV_DATA_LGLS						\n")
			_T("    SET DEPART_TRACK = '%s'		\n")
			_T("      , ARRIVE_TRACK = '%s'		\n")
			_T("      , JOB_TYP_OD = '3'              \n")
			_T("      , LUGG_OD = '9998'     \n")
			_T("	  , OD_RQ_YN = 'Y'			    \n")
			_T("  WHERE WH_TYP = '%s'					\n")
			_T("    AND RTV_NO = '%s'			"), strDepFork1, strArrFork1, strWhTyp, strRtvNo /* [LGLS] 단일포크: 포크1 좌표 사용 */);
	}


	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		
		CString strLOG_LUGG_NO = _T("");
		CString strLOG_BOTTOM_TRAY = _T("");
		CString strLOG_TOP_TRAY = _T("");
		CString strLOG_MSG = _T("");

		if(m_strRtvFork == _T("1"))
		{
			strLOG_LUGG_NO = _T("9998");
			strLOG_BOTTOM_TRAY = _T("");
			strLOG_TOP_TRAY = _T("");
			strLOG_MSG = _T("");
			strLOG_MSG.Format(_T("MANUAL RTV JOB -> RTV_NO : %s , DEPARTHS_NO : %s , ARRIVEHS_NO : %s , USE_FK : 0"), strRtvNo, strDepFork1, strArrFork1);
			if (!m_pDoc->GetQueryInsertClientLog(_T("CManualRtv"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
			{
				m_pDoc->RollbackTrans_DLG();
				return;
			}
		}
		else if (m_strRtvFork != _T("2"))
		{
			strLOG_LUGG_NO = _T("9999");
			strLOG_BOTTOM_TRAY = _T("");
			strLOG_TOP_TRAY = _T("");
			strLOG_MSG = _T("");
			strLOG_MSG.Format(_T("MANUAL RTV JOB -> RTV_NO : %s , DEPARTHS_NO : %s , ARRIVEHS_NO : %s , USE_FK : 2"), strRtvNo, strDepFork2, strArrFork2);
			if (!m_pDoc->GetQueryInsertClientLog(_T("CManualRtv"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
			{
				m_pDoc->RollbackTrans_DLG();
				return;
			}
		}
		else
		{
			strLOG_LUGG_NO = _T("9998");
			strLOG_BOTTOM_TRAY = _T("");
			strLOG_TOP_TRAY = _T("");
			strLOG_MSG = _T("");
			strLOG_MSG.Format(_T("MANUAL RTV JOB -> RTV_NO : %s , DEPARTHS_NO : %s , ARRIVEHS_NO : %s , USE_FK : 1"), strRtvNo, strDepFork1, strArrFork1);
			if (!m_pDoc->GetQueryInsertClientLog(_T("CManualRtv"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
			{
				m_pDoc->RollbackTrans_DLG();
				return;
			}

			strLOG_LUGG_NO = _T("9999");
			strLOG_BOTTOM_TRAY = _T("");
			strLOG_TOP_TRAY = _T("");
			strLOG_MSG = _T("");
			strLOG_MSG.Format(_T("MANUAL RTV JOB -> RTV_NO : %s , DEPARTHS_NO : %s , ARRIVEHS_NO : %s , USE_FK : 2"), strRtvNo, strDepFork2, strArrFork2);
			if (!m_pDoc->GetQueryInsertClientLog(_T("CManualRtv"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
			{
				m_pDoc->RollbackTrans_DLG();
				return;
			}
		}
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));	
	return;
}



