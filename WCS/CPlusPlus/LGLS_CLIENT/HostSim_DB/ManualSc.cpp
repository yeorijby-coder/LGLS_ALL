// ManualSc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "ManualSc.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CManualSc 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualSc, CDialog)

CManualSc::CManualSc(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CManualSc::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CManualSc::CManualSc(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent)
	: CSkinDialog(CManualSc::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pDB = pDb;
}
CManualSc::~CManualSc()
{
	m_pDoc->m_pManualSc = NULL;
	this->DestroyWindow();
}

void CManualSc::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBL_SC_MANUAL_SC_GRP_NO,	m_lblScManualScGrpNo);
	DDX_Control(pDX, IDC_LBL_SC_MANUAL_SC_NO,		m_lblScManualScNo);
	DDX_Control(pDX, IDC_LBL_SC_MANUAL_SC_JOB_TYP,  m_lblScManaulJobTyp);
	DDX_Control(pDX, IDC_LBL_SC_START_TRACK_FORK1,	m_lblScManaulStartHsFork1);
	DDX_Control(pDX, IDC_LBL_SC_DEST_TRACK_FORK1,	m_lblScManaulDestHsFork1);
	DDX_Control(pDX, IDC_LBL_SC_START_FORK1,		m_lblScManaulStartPos);
	DDX_Control(pDX, IDC_LBL_SC_DEST_FORK1,			m_lblScManaulStartPos2);

	
	DDX_Control(pDX, IDC_CMB_SC_MANUAL_SC_GRP_NO,	m_cbxScManualScGrpNo);
	DDX_Control(pDX, IDC_CMB_SC_MANUAL_SC_NO,		m_cbxScManualScNo);
	DDX_Control(pDX, IDC_CMB_SC_MANUAL_JOB_TYP,		m_cbxScManualJobTyp);
	DDX_Control(pDX, IDC_CMB_SC_START_TRACK_FORK1,	m_cbxStartPosFork1);
	DDX_Control(pDX, IDC_CMB_SC_DEST_TRACK_FORK1,	m_cbxDestPosFork1);
	
	DDX_Control(pDX, IDC_MASK_EDIT_START_LOC_FORK1, m_edtManualScStartPosFk1);

	DDX_Control(pDX, IDC_MASK_EDIT_DEST_LOC_FORK1,  m_edtManualScDestPosFk1);

	DDX_Control(pDX, IDC_GRP_SC_MANUAL_INFO,		   m_grpScInfo);
	DDX_Control(pDX, IDC_GRP_SC_HS_INFO,			   m_grpScHsInfo);
	DDX_Control(pDX, IDC_GRP_SC_FORK1,				   m_grpScManaulFork1);

	DDX_Control(pDX, IDC_BTN_SC_MANUAL_SAVE,		   m_btnScManualSave);
	
}


BEGIN_MESSAGE_MAP(CManualSc, CSkinDialog)
	ON_CBN_SELCHANGE(IDC_CMB_SC_MANUAL_SC_NO, &CManualSc::OnCbnSelchangeCmbScManualScNo)
	ON_BN_CLICKED(IDC_BTN_SC_MANUAL_SAVE, &CManualSc::OnBnClickedBtnScManualSave)
	ON_CBN_SELCHANGE(IDC_CMB_SC_MANUAL_SC_GRP_NO, &CManualSc::OnCbnSelchangeCmbScManualScGrpNo)
	ON_CBN_SELCHANGE(IDC_CMB_SC_MANUAL_JOB_TYP, &CManualSc::OnCbnSelchangeCmbScManualJobTyp)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CManualSc::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	CString strSC_GRP_NO;
	CString strCOMBO_SC_NO;
	CString strSC_NO;
	

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		RelocationControls();
		m_bInitialized = TRUE;		
	}

	CLib::BindCombo(m_cbxScManualScGrpNo, _T("SC_GRP_NO"), m_pDoc, (int)pEn, FALSE);
	strSC_GRP_NO = m_cbxScManualScGrpNo.GetItemKey(m_cbxScManualScGrpNo.GetCurSel());
	//CLib::BindCombo_SC_NO(m_cbxScManualScNo, _T("CELL_SC_NO"), m_pDoc, (int)pEn, strSC_GRP_NO, _T(""));
	CLib::BindCombo_SC_NO(m_cbxScManualScNo, _T("SC_NO"), m_pDoc, (int)pEn, strSC_GRP_NO);
	CLib::BindCombo(m_cbxScManualJobTyp, _T("SC_JOB_TYP"), m_pDoc, (int)pEn, FALSE);
	m_cbxScManualScNo.GetWindowText(strCOMBO_SC_NO);
	strSC_NO.Format(_T("%03s"), strCOMBO_SC_NO);
	//CLib::GetComBoBoxData(m_cbxScManualScNo, strSC_NO, 10);
	CLib::BindCombo_SC_HS_DEF(m_cbxStartPosFork1, m_pDoc, (int)pEn, strSC_NO, _T(""));
	CLib::BindCombo_SC_HS_DEF(m_cbxDestPosFork1, m_pDoc, (int)pEn, strSC_NO,  _T(""));

	m_edtManualScStartPosFk1.EnableMask(_T("dd ddd dd"),_T("__-___-__"), _T(' '));
	m_edtManualScStartPosFk1.SetWindowText(_T("01-001-01"));
	m_edtManualScStartPosFk1.EnableWindow(TRUE);

	m_edtManualScDestPosFk1.EnableMask(_T("dd ddd dd"),_T("__-___-__"), _T(' '));
	m_edtManualScDestPosFk1.SetWindowText(_T("01-002-01"));
	m_edtManualScDestPosFk1.EnableWindow(TRUE);

	m_strScNo = _T("");
	m_cbxStartPosFork1.EnableWindow(FALSE);
	m_cbxDestPosFork1.EnableWindow(FALSE);

	m_edtManualScStartPosFk1.EnableWindow(FALSE);
	m_edtManualScDestPosFk1.EnableWindow(FALSE);

	InitializeResource(pEn);
	return TRUE;  
}

void CManualSc::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnScManualSave.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScManualSave.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

}

void CManualSc::InitializeResource(int pnLang)
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

void CManualSc::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("agingtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_MANUAL_SC_GRP_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_MANUAL_SC_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_MANUAL_SC_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_START_TRACK_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_DEST_TRACK_FORK1, strValue); 

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_START_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_DEST_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("order"), (int)m_enLang);
	SetDlgItemText(	IDC_BTN_SC_MANUAL_SAVE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("info"), (int)m_enLang);
	SetDlgItemText(	IDC_GRP_SC_MANUAL_INFO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("hsinfo"), (int)m_enLang);
	SetDlgItemText(	IDC_GRP_SC_HS_INFO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork1"), (int)m_enLang);
	SetDlgItemText(	IDC_GRP_SC_FORK1, strValue);
		
}

void CManualSc::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;

	m_btnScManualSave.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScManualSave.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);
	m_btnScManualSave.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnScManualSave.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);	
}



BOOL CManualSc::IsInvalidStoData()
{
	CString strStartHsFork1 = _T("");
	CString strStartHsFork2 = _T("");

	CString strStartLocFork1 = _T("");
	CString strStartLocFork2 = _T("");
	CString strDestLocFork1 = _T("");
	CString strDestLocFork2 = _T("");

	UpdateData(TRUE);

	m_cbxStartPosFork1.GetWindowText(strStartHsFork1);

	m_edtManualScStartPosFk1.GetWindowText(strStartLocFork1);

	m_edtManualScDestPosFk1.GetWindowText(strDestLocFork1);

	return TRUE;

}

BOOL CManualSc::IsInvalidRetData()
{
	CString strDestHsFork1 = _T("");
	CString strDestHsFork2 = _T("");

	CString strStartLocFork1 = _T("");
	CString strStartLocFork2 = _T("");
	CString strDestLocFork1 = _T("");
	CString strDestLocFork2 = _T("");

	UpdateData(TRUE);

	m_cbxDestPosFork1.GetWindowText(strDestHsFork1);

	m_edtManualScStartPosFk1.GetWindowText(strStartLocFork1);

	m_edtManualScDestPosFk1.GetWindowText(strDestLocFork1);

	
	return TRUE;
}
BOOL CManualSc::IsInvalidRetToRetData()
{
	CString strScNo,strScGrpNo = _T("");
	CString strStartHsFork1 = _T("");
	CString strStartHsFork2 = _T("");
	CString strDestHsFork1 = _T("");
	CString strDestHsFork2 = _T("");

	UpdateData(TRUE);
	m_cbxScManualScGrpNo.GetWindowText(strScGrpNo);
	m_cbxScManualScNo.GetWindowText(strScNo);
	m_cbxStartPosFork1.GetWindowText(strStartHsFork1);
	m_cbxDestPosFork1.GetWindowText(strDestHsFork1);

	int nScNo = m_cbxScManualScNo.GetItemData(m_cbxScManualScNo.GetCurSel());
	strScNo = CConvert::ToString(nScNo);

	if (strScGrpNo == _T("PCH"))
	{
		if ( strStartHsFork1 ==_T("") || strDestHsFork1 == _T("") ||
			strStartHsFork2 == _T("") || strDestHsFork2 == _T(""))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("포크1 홈스탠드 정보를 체크하시오")));
			return FALSE;
		}
	}
	else
	{
		if ( strStartHsFork1 ==_T("") || strDestHsFork1 == _T(""))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("포크1 정보를 체크하시오")));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CManualSc::IsInvalidRackToRackData()
{
	CString strScGrpNo, strScNo = _T("");
	CString strStartLocFork1 = _T("");
	CString strStartLocFork2 = _T("");
	CString strDestLocFork1 = _T("");
	CString strDestLocFork2 = _T("");

	UpdateData(TRUE);
	m_cbxScManualScGrpNo.GetWindowText(strScGrpNo);
	m_cbxScManualScNo.GetWindowText(strScNo);

	m_edtManualScStartPosFk1.GetWindowText(strStartLocFork1);

	m_edtManualScDestPosFk1.GetWindowText(strDestLocFork1);

	if (strScGrpNo == _T("PCH"))
	{
		if ( strStartLocFork1 ==_T("") || strDestLocFork1 == _T("") ||
			strStartLocFork2 == _T("") || strDestLocFork2 == _T(""))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("포크1 홈스탠드 정보를 체크하시오")));
			return FALSE;
		}
	}
	else
	{
		if ( strStartLocFork1 ==_T("") || strDestLocFork1 == _T(""))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("포크1 정보를 체크하시오")));
			return FALSE;
		}
	}
	return TRUE;
}

void CManualSc::OnBnClickedBtnScManualSave()
{
	CString strSql = _T("");
	CString strScGrpNo = _T("");
	CString strScNo = _T("");
	CString strJobTyp = _T("");
	CString strStartHsFork1 = _T("");
	CString strDestHsFokr1 = _T("");
	CString strStartLocFork1 = _T("");
	CString strStartLocBankFork1 = _T("");
	CString strStartLocBayFork1 = _T("");
	CString strStartLocLevFork1 = _T("");
	CString strDestLocFork1 = _T("");
	CString strDestLocBankFork1 = _T("");
	CString strDestLocBayFork1 = _T("");
	CString strDestLocLevFork1 = _T("");
	int nScNo;

	UpdateData(TRUE);
	m_cbxScManualScGrpNo.GetWindowText(strScGrpNo);
	m_cbxScManualScNo.GetWindowText(strScNo);
	m_cbxScManualJobTyp.GetWindowText(strJobTyp);

	strStartHsFork1 = m_cbxStartPosFork1.GetItemKey(m_cbxStartPosFork1.GetCurSel());
	strDestHsFokr1 = m_cbxDestPosFork1.GetItemKey(m_cbxDestPosFork1.GetCurSel());

	m_edtManualScStartPosFk1.GetWindowText(strStartLocFork1);
	m_edtManualScDestPosFk1.GetWindowText(strDestLocFork1);

	//0100101
	strStartLocBankFork1 = strStartLocFork1.Mid(0,2);
	strStartLocBayFork1 = strStartLocFork1.Mid(2,3);
	strStartLocLevFork1 = strStartLocFork1.Mid(5,2);
	strDestLocBankFork1 = strDestLocFork1.Mid(0,2);
	strDestLocBayFork1 = strDestLocFork1.Mid(2,3);
	strDestLocLevFork1 = strDestLocFork1.Mid(5,2);


	nScNo = m_cbxScManualScNo.GetItemData(m_cbxScManualScNo.GetCurSel());
	strScNo = CConvert::ToString(nScNo);

	strJobTyp = m_cbxScManualJobTyp.GetItemKey(m_cbxScManualJobTyp.GetCurSel());
	strJobTyp.Trim();

	if (strJobTyp == _T("0")) {AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업을 선택하세요"))); return;}

	if (strScGrpNo == _T("") || 
		strScNo     == _T("") ||
		strJobTyp   == _T("")) 
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("포크1 정보를 체크하시오")));
		return;
	}

	if (IsInvalidStoData() == FALSE) { return; }


	if (SelectScStatus() == FALSE) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업중입니다"))) + _T(" ") + m_pDoc->GetMsgLangDef( _T("종료 혹은 삭제 후 진행하세요.")); return;}

	if (strJobTyp == _T("1"))
	{
		strStartLocBankFork1 = _T("0");
		strStartLocBayFork1 = _T("0");
		strStartLocLevFork1 = _T("0");
		strDestHsFokr1 = _T("0");
	}
	else if (strJobTyp == _T("2"))
	{
		strStartHsFork1 = _T("0");
		strDestLocBankFork1 = _T("0");
		strDestLocBayFork1 = _T("0");
		strDestLocLevFork1 = _T("0");
	}
	else if (strJobTyp == _T("3"))
	{
		strStartLocBankFork1 = _T("0");
		strStartLocBayFork1 = _T("0");
		strStartLocLevFork1 = _T("0");
		strDestLocBankFork1 = _T("0");
		strDestLocBayFork1 = _T("0");
		strDestLocLevFork1 = _T("0");
	}


	m_pDoc->BeginTrans_DLG();

	CString strLOG_LUGG_NO = _T("9999");
	CString strLOG_BOTTOM_TRAY = _T("");
	CString strLOG_TOP_TRAY = _T("");
	CString strLOG_MSG = _T("");
	strLOG_MSG.Format(_T("MANUAL SC JOB -> JOB_TYP : %s , START POS : %s , START LOCATION : %02s-%03s-%02s , DEST POS : %s , DEST LOCATION : %02s-%03s-%02s , SC_NO : %s")
					, strJobTyp, strStartHsFork1, strStartLocBankFork1, strStartLocBayFork1, strStartLocLevFork1, strDestHsFokr1, strDestLocBankFork1, strDestLocBayFork1, strDestLocLevFork1, strScNo );
	if (!m_pDoc->GetQueryInsertClientLog(_T("CManualSc"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	strSql = _T(" UPDATE SC_DATA													 \n");
	strSql+= _T("    SET OD_RQ_YN = 'Y'												 \n");
	strSql+= _T("    ,JOB_TYP_OD          = '") + strJobTyp + _T("'					 \n");
	strSql+= _T("    ,LUGG_NO_FK1_OD      = '9999'									 \n");
	strSql+= _T("    ,START_BANK_FK1_OD   = '") + strStartLocBankFork1 + _T("'		 \n");
	strSql+= _T("    ,START_BAY_FK1_OD    = '") + strStartLocBayFork1 + _T("'		 \n");
	strSql+= _T("    ,START_LEVEL_FK1_OD  = '") + strStartLocLevFork1 + _T("'		 \n");
	strSql+= _T("    ,START_HSPOS_FK1_OD  = '") + strStartHsFork1 + _T("'			 \n");
	strSql+= _T("    ,DEST_BANK_FK1_OD    = '") + strDestLocBankFork1 + _T("'		 \n");
	strSql+= _T("    ,DEST_BAY_FK1_OD     = '") + strDestLocBayFork1 + _T("'		 \n");
	strSql+= _T("    ,DEST_LEVEL_FK1_OD   = '") + strDestLocLevFork1 + _T("'		 \n");
	strSql+= _T("    ,DEST_HSPOS_FK1_OD   = '") + strDestHsFokr1 + _T("'			 \n");
	strSql+= _T("    ,USE_FK_OD           ='0'								\n");
	strSql+= _T("    ,WRITE_CONTINUE_OD   ='0'								\n");
	strSql+= _T("    ,WRITE_FLAG_OD       ='0'								\n");
	strSql+= _T("    ,USER_COMMAND_OD     ='0'								\n");
	strSql+= _T("  WHERE WH_TYP = '") + m_pDoc->m_WH_TYP + _T("'            \n");
	strSql+= _T("    AND MC_NO = '") + strScNo + _T("'"                        );

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;

}
//SC 상태 체크
BOOL CManualSc::SelectScStatus()
{
	CString strSql = _T("");
	CString strScNo = _T("");



	UpdateData(TRUE);
	int nScNo = m_cbxScManualScNo.GetItemData(m_cbxScManualScNo.GetCurSel());
	strScNo = CConvert::ToString(nScNo);

	strSql = _T("");
	strSql += _T("SELECT *											\n");
	strSql += _T("  FROM SC_DATA									\n");
	strSql += _T(" WHERE WH_TYP = '") + m_pDoc->m_WH_TYP + _T("'	\n");
	strSql += _T("   AND MC_NO = '") + strScNo + _T("'				\n");
	strSql += _T("   AND ITN_LUGG_FK1 = '0'							\n");

	int nSelCnt = m_pDoc->GetSelectQryCnt_DLG(strSql);

	if (nSelCnt < 1)
	{
		return FALSE;
	}

	return TRUE;

}


BOOL CManualSc::UpdateScData()
{
	CString strSql = _T("");
	CString strScNo = _T("");

	UpdateData(TRUE);
	m_cbxScManualScNo.GetWindowText(strScNo);

	int nScNo = m_cbxScManualScNo.GetItemData(m_cbxScManualScNo.GetCurSel());
	strScNo = CConvert::ToString(nScNo);

	strSql.Format(_T(" UPDATE SC_DATA	                                        \n")
		_T("    SET LUGG_NO_FK1_OD = '0'								\n")
		_T("      , START_BANK_FK1_OD = '0'							\n")
		_T("	    , START_BAY_FK1_OD = '0'							\n")
		_T("	    , START_LEVEL_FK1_OD = '0'							\n")
		_T("      , START_HSPOS_FK1_OD = '0'   						\n")
		_T("	    , DEST_BANK_FK1_OD = '0'							\n")
		_T("	    , DEST_BAY_FK1_OD = '0'								\n")
		_T("	    , DEST_LEVEL_FK1_OD = '0'							\n")
		_T("	    , DEST_HSPOS_FK1_OD = '0'							\n")
		_T("	    , LUGG_NO_FK2_OD = '0'		                        \n")
		_T("      , START_BANK_FK2_OD = '0'							\n")
		_T("	    , START_BAY_FK2_OD = '0'							\n")
		_T("	    , START_LEVEL_FK2_OD = '0'							\n")
		_T("      , START_HSPOS_FK2_OD =  '0'							\n")
		_T("	    , DEST_BANK_FK2_OD = '0'		                    \n")
		_T("	    , DEST_BAY_FK2_OD = '0'		                        \n")
		_T("	    , DEST_LEVEL_FK2_OD = '0'	                        \n")
		_T("	    , DEST_HSPOS_FK2_OD = '0'                           \n")
		_T("	    , WRITE_CONTINUE_OD = '0'                           \n")
		_T("	    , WRITE_FLAG_OD = '0'		                        \n")
		_T("	    , USER_COMMAND_OD = '0'		                        \n")
		_T("		, JOB_TYP_OD = '0'									\n")
		_T("	    , ITN_LUGG_FK1 = 0									\n")
		_T("	    , ITN_LUGG_FK2 = 0									\n")
		_T("  WHERE WH_TYP = '%s'										\n")
   		_T("    AND SC_NO = '%s'										  "), m_pDoc->m_WH_TYP, strScNo);

 	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;

}

void CManualSc::OnCbnSelchangeCmbScManualScGrpNo()
{
	//m_cbxScManualScNo
	CString strSC_GRP_NO;
	//EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;

	strSC_GRP_NO = m_cbxScManualScGrpNo.GetItemKey(m_cbxScManualScGrpNo.GetCurSel());

 	//CLib::GetComBoBoxData(m_cbxScManualScGrpNo, strSC_GRP_NO, 10);
 	//CLib::BindCombo_CELL_SC_NO(m_cbxScManualScNo, _T("SC_NO"), m_pDoc, (int)pEn, strSC_GRP_NO, _T(""));
	CLib::BindCombo_SC_NO(m_cbxScManualScNo, _T("SC_NO"), m_pDoc, 0, strSC_GRP_NO);
	OnCbnSelchangeCmbScManualScNo();
}


void CManualSc::OnCbnSelchangeCmbScManualScNo()
{
	//UpdateData(TRUE);

	//EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	//SetFontNation((int)pEn);

	CLib::BindCombo(m_cbxScManualJobTyp, _T("SC_JOB_TYP"), m_pDoc, 0, FALSE);
	OnCbnSelchangeCmbScManualJobTyp();

}


void CManualSc::OnCbnSelchangeCmbScManualJobTyp()
{

	//EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	//SetFontNation((int)pEn);

	CString strScNo;
	CString strCOMBO_SC_NO;
	CString strJobTyp,strScJobTyp=_T("");

	//CLib::GetComBoBoxData(m_cbxScManualScNo, strScNo, 10);
	m_cbxScManualScNo.GetWindowText(strCOMBO_SC_NO);
	strScNo.Format(_T("%03s"), strCOMBO_SC_NO);
	
	strScJobTyp = m_cbxScManualJobTyp.GetItemKey(m_cbxScManualJobTyp.GetCurSel());

	CLib::BindCombo_SC_HS_DEF(m_cbxStartPosFork1, m_pDoc, 0, strScNo, strScJobTyp); //(int)pEn
	CLib::BindCombo_SC_HS_DEF(m_cbxDestPosFork1, m_pDoc, 0, strScNo, strScJobTyp);

	
	UpdateData(TRUE);

	if (strScJobTyp == _T("0"))
	{
		m_cbxStartPosFork1.EnableWindow(FALSE);
		m_cbxDestPosFork1.EnableWindow(FALSE);

		m_edtManualScStartPosFk1.EnableWindow(FALSE);
		m_edtManualScDestPosFk1.EnableWindow(FALSE);
	}
	else if (strScJobTyp == _T("1"))
	{
		m_cbxStartPosFork1.EnableWindow(TRUE);
		m_cbxDestPosFork1.EnableWindow(FALSE);

		m_edtManualScStartPosFk1.EnableWindow(FALSE);
		m_edtManualScDestPosFk1.EnableWindow(TRUE);
	}
	else if (strScJobTyp == _T("2"))
	{
		m_cbxStartPosFork1.EnableWindow(FALSE);
		m_cbxDestPosFork1.EnableWindow(TRUE);

		m_edtManualScStartPosFk1.EnableWindow(TRUE);
		m_edtManualScDestPosFk1.EnableWindow(FALSE);
	}
	else if (strScJobTyp == _T("3"))
	{
		m_cbxStartPosFork1.EnableWindow(TRUE);
		m_cbxDestPosFork1.EnableWindow(TRUE);

		m_edtManualScStartPosFk1.EnableWindow(FALSE);
		m_edtManualScDestPosFk1.EnableWindow(FALSE);
	}
	else if (strScJobTyp == _T("4"))
	{
		m_cbxStartPosFork1.EnableWindow(FALSE);
		m_cbxDestPosFork1.EnableWindow(FALSE);

		m_edtManualScStartPosFk1.EnableWindow(TRUE);
		m_edtManualScDestPosFk1.EnableWindow(TRUE);
	}
}

void CManualSc::OnClose()
{
	m_pDoc->m_pManualSc=NULL;
	delete this;
}


//bool CManualSc::BindCombo_SC_NO(CComboBoxWrapper& cbx, CString strCDX_CD, CEcsDoc *pDoc, int nEN_LANG, CString strAGING_TYP, CString strSC_NO)
//{
//	if(pDoc	== NULL)							return false;
//	if(pDoc->IsConnectDB() == FALSE) return FALSE;
//	CString srtAAAa;
//	srtAAAa.Format(_T("%%")); 
//	CStringList strList;
//	CString strSql;
//	CString strCCD_CD, strCCD_NM_VALUE;
//	int nRowCnt = 0, j=0;
//	CString strMessage;
//	cbx.ResetContent();
//
//
//	strSql.Format(_T("    SELECT AGING_TYP,CCD_CD, CCD_NM_KOR, COALESCE(CCD_NM_ENG,CCD_NM_KOR) AS CCD_NM_ENG 								\n")
//		_T("			        , COALESCE(CCD_NM_HUN,CCD_NM_KOR) AS CCD_NM_HUN, COALESCE(CCD_NM_CHIN,CCD_NM_KOR) AS CCD_NM_CHIN					\n")
//		_T("			     FROM COMMON_CODE CC JOIN (SELECT AGING_TYP,CELL_SC_NO FROM CELL_MST GROUP BY AGING_TYP,CELL_SC_NO) CM  	\n")
//		_T("	  		       ON CC.CCD_CD = CM.CELL_SC_NO  																			\n")
//		_T("			    WHERE CC.CCD_CD_YN = 'Y'																					\n")
//		_T("			      AND CC.CDX_CD = '%s'																						\n")
//		_T("			      AND CC.WH_TYP LIKE '%s%%'																					\n")
//		_T("			      AND CM.AGING_TYP LIKE '%s%%'  																			\n")
//		_T("				  AND CM.CELL_SC_NO LIKE '%%%s'  																			\n")
//		_T("		     ORDER BY CC.CCD_EPR_ORD																							 	\n")
//		, strCDX_CD, pDoc->m_WH_TYP, strAGING_TYP, strSC_NO.Mid(2,1));
//
//	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
//	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
//
//	pRsw->MoveFirst(); 
//	cbx.SetItemDataEx(0, 0);
//	cbx.AddString(_T("ALL"));
//	for(int i = 0; i < nRowCnt; i++)
//	{
//		strCCD_CD = pRsw->GetItem(_T("CCD_CD"));
//
//		if (nEN_LANG == 0)
//			strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_KOR"));
//		else if (nEN_LANG == 1)
//			strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_ENG"));
//		else if (nEN_LANG == 2)
//			strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_HUN"));
//		else if (nEN_LANG == 3)
//			strCCD_NM_VALUE = pRsw->GetItem(_T("CCD_NM_CHIN"));
//
//		cbx.AddString(strCCD_NM_VALUE);
//		cbx.SetItemDataEx(i + 1, strCCD_NM_VALUE);
//
//		pRsw->MoveNext();
//	}
//
//	cbx.SetCurSel(0);
//	delete pRsw;
//	return true;
//}