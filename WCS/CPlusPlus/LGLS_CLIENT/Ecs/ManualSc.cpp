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
}

void CManualSc::PostNcDestroy()
{
	if (m_pDoc != NULL) m_pDoc->m_pManualSc = NULL;
	CSkinDialog::PostNcDestroy();
	delete this;
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

// [LGLS 2026-09-03] 호기 콤보에서 크레인 번호(1~5)를 얻는다 : 항목데이터 우선, 없으면 문자열 앞자리
static int LglsScIdxOf(CComboBoxWrapper& cbx)
{
	int nSel = cbx.GetCurSel();
	if (nSel < 0) return 0;
	int d = (int)cbx.GetItemData(nSel);
	if (d >= 1 && d <= 5) return d;
	CString str; cbx.GetWindowText(str); str.Trim();
	int n = _ttoi(str);
	if (n >= 901) n -= 900;
	return (n >= 1 && n <= 5) ? n : 0;
}

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
	// [LGLS] SC 1~5호기 보장: DB(COMMON_CODE)에 일부 호기만 등록된 경우 소스에서 보강
	{
		bool bHasSc[6] = { false, false, false, false, false, false };
		int nScCnt = m_cbxScManualScNo.GetCount();
		for (int i = 0; i < nScCnt; i++) { int d = (int)m_cbxScManualScNo.GetItemData(i); if (d >= 1 && d <= 5) bHasSc[d] = true; }
		for (int nSc = 1; nSc <= 5; nSc++)
		{
			if (!bHasSc[nSc])
			{
				CString strScAdd; strScAdd.Format(_T("%d호기"), nSc);
				int idxAdd = m_cbxScManualScNo.AddString(strScAdd);
				m_cbxScManualScNo.SetItemData(idxAdd, nSc);
			}
		}
		m_cbxScManualScNo.SetCurSel(0);
	}
	CLib::BindCombo(m_cbxScManualJobTyp, _T("SC_JOB_TYP"), m_pDoc, (int)pEn, FALSE);
	m_cbxScManualScNo.GetWindowText(strCOMBO_SC_NO);
	strSC_NO.Format(_T("%03s"), strCOMBO_SC_NO);
	//CLib::GetComBoBoxData(m_cbxScManualScNo, strSC_NO, 10);
	// [LGLS 2026-09-03] H/S 는 SC_HS_DEF 대신 CV_DATA(해당 크레인 측 C/V 트랙)에서 채운다
	CLib::BindCombo_SC_HS_TRACK(m_cbxStartPosFork1, m_pDoc, (int)pEn, LglsScIdxOf(m_cbxScManualScNo));
	CLib::BindCombo_SC_HS_TRACK(m_cbxDestPosFork1,  m_pDoc, (int)pEn, LglsScIdxOf(m_cbxScManualScNo));

	m_edtManualScStartPosFk1.EnableMask(_T("dd ddd dd"),_T("__-___-__"), _T(' '));
	m_edtManualScStartPosFk1.SetWindowText(_T("01-001-01"));
	m_edtManualScStartPosFk1.SetReadOnly(TRUE);   // [LGLS] TextBox ReadOnly
	m_edtManualScStartPosFk1.EnableWindow(TRUE);

	m_edtManualScDestPosFk1.EnableMask(_T("dd ddd dd"),_T("__-___-__"), _T(' '));
	m_edtManualScDestPosFk1.SetWindowText(_T("01-002-01"));
	m_edtManualScDestPosFk1.SetReadOnly(TRUE);   // [LGLS] TextBox ReadOnly
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
	m_strScTitleBase = strValue;
	{ CString scSel; m_cbxScManualScNo.GetWindowText(scSel); if(!scSel.IsEmpty()) strValue.Format(_T("%s - %s"), m_strScTitleBase, scSel); }
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("agingtyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_MANUAL_SC_GRP_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scno"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_MANUAL_SC_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_MANUAL_SC_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_START_TRACK_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_DEST_TRACK_FORK1, strValue); 

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startloc"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_START_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destloc"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_DEST_FORK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("order"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(	IDC_BTN_SC_MANUAL_SAVE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("info"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(	IDC_GRP_SC_MANUAL_INFO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("hsinfo"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(	IDC_GRP_SC_HS_INFO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\"), _T("dlg_scmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork1"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(	IDC_GRP_SC_FORK1, strValue);
		
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

// [LGLS 2026-09-05] Vehicle 반송 좌표 인코딩
//   IO_TASK cThread_SCH.VehPortLoc / VehCellLoc 과 같은 규약을 쓴다.
//     포트 : ("00", "00", 트랙 % 100)
//     셀   : (홀수뱅크 "01" / 짝수뱅크 "02", Bay % 100, Level % 100)
static void VehPortLoc(LPCTSTR szTrack, CString& a, CString& b, CString& c)
{
	int t = _ttoi(szTrack);
	a = _T("00");
	b = _T("00");
	c.Format(_T("%02d"), t % 100);
}

static void VehCellLoc(LPCTSTR szBank, LPCTSTR szBay, LPCTSTR szLev,
                       CString& a, CString& b, CString& c)
{
	int bank = _ttoi(szBank);
	int bay  = _ttoi(szBay);
	int lev  = _ttoi(szLev);
	a = (bank % 2 == 1) ? _T("01") : _T("02");
	b.Format(_T("%02d"), bay % 100);
	c.Format(_T("%02d"), lev % 100);
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

	// [LGLS 2026-09-05] VehThread 가 실제로 소비하는 좌표를 함께 만든다.
	//   종전에는 START_/DEST_*_FK1_OD 만 기록해 지시가 설비에 나가지 않았고,
	//   OD_RQ_YN='Y' 만 남아 그 크레인이 물렸다(자동/수동 모두 불가).
	CString strF1, strF2, strF3, strT1, strT2, strT3;
	if (strJobTyp == _T("1"))          // 입고 : 라인 포트 -> 셀
	{
		VehPortLoc(strStartHsFork1, strF1, strF2, strF3);
		VehCellLoc(strDestLocBankFork1, strDestLocBayFork1, strDestLocLevFork1, strT1, strT2, strT3);
	}
	else if (strJobTyp == _T("2"))     // 출고 : 셀 -> 라인 포트
	{
		VehCellLoc(strStartLocBankFork1, strStartLocBayFork1, strStartLocLevFork1, strF1, strF2, strF3);
		VehPortLoc(strDestHsFokr1, strT1, strT2, strT3);
	}
	else                               // 그 외(3) : 포트 -> 포트
	{
		VehPortLoc(strStartHsFork1, strF1, strF2, strF3);
		VehPortLoc(strDestHsFokr1, strT1, strT2, strT3);
	}


	// [LGLS 2026-09-03] 가드 : 스케줄러가 이미 낸 지시(OD_RQ_YN=Y)를 수동 지시가 덮어쓰면
	//   그 작업이 영영 완료되지 않는다(RGV 3059 실측 사례). 크레인도 같은 규칙.
	{
		CString strChk;
		strChk.Format(_T(" SELECT ") + m_pDoc->NVL + _T("(OD_RQ_YN,'N') AS OD_RQ_YN, ") + m_pDoc->NVL + _T("(PALLET_ID_OD,'') AS PALLET_ID_OD ")
			_T(" FROM SC_DATA_LGLS WHERE WH_TYP = '%s' AND MC_NO = '%s' "), (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strScNo);
		int nCnt = 0; CString strErr;
		_RecordsetPtr pRs = m_pDoc->GetSelectQryRecordsetPtr_DLG(strChk, nCnt, strErr);
		if (nCnt > 0)
		{
			CRecordSetWrap rsw(pRs); rsw.MoveFirst();
			CString strRq = rsw.GetItem(_T("OD_RQ_YN"));     strRq.Trim();
			CString strPl = rsw.GetItem(_T("PALLET_ID_OD")); strPl.Trim();
			if (strRq == _T("Y"))
			{
				CString strMsg;
				strMsg.Format(_T("이 크레인에 진행 중인 지시(파레트 %s)가 있습니다. 먼저 SC 상태창의 [지시 삭제] 로 정리한 뒤 지시하세요."), (LPCTSTR)strPl);
				AfxMessageBox(strMsg);
				return;
			}
		}
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

	strSql = _T(" UPDATE SC_DATA_LGLS													 \n");
	strSql+= _T("    SET OD_RQ_YN = 'Y'												 \n");
	strSql+= _T("    ,JOB_TYP_OD          = '") + strJobTyp + _T("'					 \n");
	strSql+= _T("    ,LUGG_NO_FK1_OD      = '9999'									 \n");
	// [LGLS 2026-09-05] 아래 8개가 WCS_TASK_CV(VehThread.ConsumeCommands)가 읽는 값이다.
	//   TRANSFER_REQUEST_OD 가 트리거이고, 나머지가 실제 반송 좌표다.
	strSql+= _T("    ,PALLET_ID_OD       = '9999'                                   \n");
	strSql+= _T("    ,FROM_01_OD         = '") + strF1 + _T("'                                   \n");
	strSql+= _T("    ,FROM_02_OD         = '") + strF2 + _T("'                                   \n");
	strSql+= _T("    ,FROM_03_OD         = '") + strF3 + _T("'                                   \n");
	strSql+= _T("    ,TO_01_OD           = '") + strT1 + _T("'                                   \n");
	strSql+= _T("    ,TO_02_OD           = '") + strT2 + _T("'                                   \n");
	strSql+= _T("    ,TO_03_OD           = '") + strT3 + _T("'                                   \n");
	strSql+= _T("    ,TRANSFER_REQUEST_OD = 'Y'                                   \n");
	strSql+= _T("    ,COMPLETE_RD        = '0'                                   \n");
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
	strSql+= _T("    AND MC_NO = '") + strScNo + _T("' ");
	strSql+= _T("    AND OD_RQ_YN = 'N' ");   // [LGLS 2026-09-03] 진행 중 지시 덮어쓰기 방지(경쟁 상황 방어)

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
	strSql += _T("  FROM SC_DATA_LGLS									\n");
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

	strSql.Format(_T(" UPDATE SC_DATA_LGLS	                                        \n")
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
	// [LGLS] SC 1~5호기 보장 (재바인딩 시에도 동일)
	{
		bool bHasSc[6] = { false, false, false, false, false, false };
		int nScCnt = m_cbxScManualScNo.GetCount();
		for (int i = 0; i < nScCnt; i++) { int d = (int)m_cbxScManualScNo.GetItemData(i); if (d >= 1 && d <= 5) bHasSc[d] = true; }
		for (int nSc = 1; nSc <= 5; nSc++)
		{
			if (!bHasSc[nSc]) { CString s2; s2.Format(_T("%d호기"), nSc); int ix = m_cbxScManualScNo.AddString(s2); m_cbxScManualScNo.SetItemData(ix, nSc); }
		}
	}
	OnCbnSelchangeCmbScManualScNo();
}


void CManualSc::OnCbnSelchangeCmbScManualScNo()
{
	//UpdateData(TRUE);

	//EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	//SetFontNation((int)pEn);

	CLib::BindCombo(m_cbxScManualJobTyp, _T("SC_JOB_TYP"), m_pDoc, 0, FALSE);
	{ CString scSel; m_cbxScManualScNo.GetWindowText(scSel); CString t; if(scSel.IsEmpty()) t=m_strScTitleBase; else t.Format(_T("%s - %s"), m_strScTitleBase, scSel); SetWindowText(t); }
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

	// [LGLS 2026-09-03] H/S 는 CV_DATA 기준(SC_HS_DEF 미사용)
	CLib::BindCombo_SC_HS_TRACK(m_cbxStartPosFork1, m_pDoc, 0, LglsScIdxOf(m_cbxScManualScNo));
	CLib::BindCombo_SC_HS_TRACK(m_cbxDestPosFork1,  m_pDoc, 0, LglsScIdxOf(m_cbxScManualScNo));

	
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
	DestroyWindow();
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