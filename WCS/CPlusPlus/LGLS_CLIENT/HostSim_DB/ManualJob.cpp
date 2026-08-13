// ManualJob.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "ManualJob.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"
#include "afxwin.h"


// CManualJob 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualJob, CSkinDialog)

CManualJob::CManualJob(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CManualJob::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CManualJob::CManualJob(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent)
	: CSkinDialog(CManualJob::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pDB = pDb;
}
CManualJob::~CManualJob()
{
	m_pDoc->m_pManualJob = NULL;
	m_pDoc->m_blManualLogin = FALSE;
	m_pDoc->m_pManualLogin = NULL;
	this->DestroyWindow();
}


void CManualJob::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GRP_MANUAL_JOB_SEARCH,		    m_grpManualJobSearch);

	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_WH_TYP,         m_lblManualJobWhTyp);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_STARTPOS,       m_lblManualJobStartPos);
	DDX_Control(pDX, IDC_MANUAL_JOB_LBL_DESTPOS,        m_lblManualJobDestPos);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_JOBTYP,         m_lblManualJobJobTyp);
	//DDX_Control(pDX, IDC_LBL_MANUAL_JOB_PRODUCTSIZE,    m_lblManualJobProductSize);
	//DDX_Control(pDX, IDC_LBL_MANUAL_JOB_DURATION_TIME,  m_lblManualJobDurationTime);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_REMARKS,        m_lblManualJobRemark);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_LOCATION,       m_lblManualJobLocation);

	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_WHTYPE,		m_cbxManualJobWhTyp);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_STARTPOS,     m_cbxManualJobStartPos);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_DESTPOS,      m_cbxManualJobDestPos);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_EDIT_JOB_TYP, m_cbxManualJobJobTyp);

	DDX_Control(pDX, IDC_EDIT_MANUAL_JOB_START_LOCATION,   m_edtManualJobStartLocation);
	DDX_Control(pDX, IDC_EDIT_MANUAL_JOB_DEST_LOCATION, m_edtManualJobDestLocation);
	DDX_Control(pDX, IDC_EDIT_MANUAL_JOB_REMARKS,		m_edtManualJobRemark);

	DDX_Control(pDX, IDC_MASK_EDIT_MANUAL_JOB_CELL_NO_FR,   m_maskedtManualJobLocationFr);
	DDX_Control(pDX, IDC_MASK_EDIT_MANUAL_JOB_CELL_NO_TO,	m_maskedtManualJobLocationTo);


	DDX_Control(pDX, ID_BTN_MANUL_JOB_INSERT,			m_btnManualJobAdd);
	DDX_Control(pDX, ID_BTN_MANUAL_JOB_CLEAR,			m_btnManualJobDataClear);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_LOCATION_TO,    m_lblManualJobLocation_To);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_PRODUCT_SIZE, m_cbxManualJobProductSize);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_PRODUCT_SIZE, m_lblManualJobProductSize);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_CNT, m_cbxManualJobCnt);
	DDX_Control(pDX, IDC_COMBO_MANUAL_JOB_PRIORITY, m_cbxManualJobPriority);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_CNT, m_lblManualJobCnt);
	DDX_Control(pDX, IDC_LBL_MANUAL_JOB_PRIORITY, m_lblManualJobPriority);
}


BEGIN_MESSAGE_MAP(CManualJob, CSkinDialog)
	ON_COMMAND_RANGE(ID_BTN_MANUL_JOB_INSERT, ID_BTN_MANUAL_JOB_STODATE_FRTO, &CManualJob::OnCommandRangeButtonEvent)
	ON_BN_CLICKED(IDCANCEL, &CManualJob::OnBnClickedCancel)
	ON_BN_CLICKED(ID_BTN_MANUL_JOB_INSERT, &CManualJob::OnBnClickedBtnManulJobInsert)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BTN_MANUAL_JOB_CLEAR, &CManualJob::OnBnClickedBtnManualJobClear)
	ON_CBN_SELCHANGE(IDC_COMBO_MANUAL_JOB_EDIT_JOB_TYP, &CManualJob::OnSelchangeComboManualJobEditJobTyp)
	ON_CBN_SELCHANGE(IDC_COMBO_MANUAL_JOB_WHTYPE, &CManualJob::OnCbnSelchangeComboManualJobWhtype)
END_MESSAGE_MAP()

void CManualJob::OnClose()
{
	m_pDoc->m_pManualJob = NULL;
	m_pDoc->m_pManualLogin = NULL;
	m_pDoc->m_blManualLogin = FALSE;
	delete this;
}

// CManualJob 메시지 처리기입니다.
BOOL CManualJob::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn, 0);

	CLib::SetBindCombo_DEST_POS_DEF(m_cbxManualJobStartPos, m_pDoc);
	CLib::SetBindCombo_DEST_POS_DEF(m_cbxManualJobDestPos, m_pDoc);

	CLib::BindCombo(m_cbxManualJobWhTyp, _T("WH_TYP"), m_pDoc,(int)pEn, FALSE);
	CLib::BindCombo(m_cbxManualJobJobTyp, _T("JOB_TYP"), m_pDoc,(int)pEn, TRUE);
	CLib::BindCombo(m_cbxManualJobProductSize, _T("PRODUCT_SIZE"), m_pDoc,(int)pEn, TRUE);
	CLib::BindCombo(m_cbxManualJobPriority, _T("JOB_PRIORITY"), m_pDoc,(int)pEn, FALSE);
	CLib::BindCombo(m_cbxManualJobCnt, _T("JOB_CNT"), m_pDoc,(int)pEn, FALSE);

	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		//RelocationControls();
		RedrawImage();
		RenameResource(pEn);

		m_bInitialized = TRUE;		

		//InitializeResource(0);
	}

	m_edtManualJobStartLocation.SetWindowText(_T("01-001-01"));
	m_edtManualJobDestLocation.SetWindowText(_T("01-002-01"));

	m_maskedtManualJobLocationFr.EnableMask(_T("dd ddd dd"), _T("__-___-__"), _T(' '));
	m_maskedtManualJobLocationFr.SetWindowText(_T("01-001-01"));

	m_maskedtManualJobLocationTo.EnableMask(_T("dd ddd dd"), _T("__-___-__"), _T(' '));
	m_maskedtManualJobLocationTo.SetWindowText(_T("01-002-01"));


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManualJob::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}

void CManualJob::OnCommandRangeButtonEvent(UINT nID)
{
	switch(nID)
	{
	case ID_BTN_MANUL_JOB_INSERT:
		{
			OnBnClickedBtnManulJobInsert();
			break;
		}
	case ID_BTN_MANUAL_JOB_CLEAR:
		{
			OnBnClickedBtnManualJobClear();
			break;
		}   
	case ID_BTN_MANUAL_JOB_LOCATION_FRTO:
		{
			break;
		}    
	case ID_BTN_MANUAL_JOB_STODATE_FRTO:
		{
			break;
		}   
	default:
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("정의되지 않은 버튼 이벤트")) + CConvert::ToString((int)nID));
		}
	}
}

void CManualJob::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("completereport"), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUL_JOB_COMP, strValue);
	
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("delete"), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUAL_JOB_CELL_DELETE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manualadd "), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUL_JOB_INSERT, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manualdelete "), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUAL_JOB_DELETE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("datereset "), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUAL_JOB_CLEAR, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("copy"), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUAL_JOB_COPY, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_JOBTYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("productsize"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_PRODUCT_SIZE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("strageloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_LOCATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectcnt"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("racktime"), (int)m_enLang);
//	SetDlgItemText(IDC_LBL_MANUAL_JOB_DURATION_TIME, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_STARTPOS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_MANUAL_JOB_LBL_DESTPOS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("remark"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_REMARKS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobstartdt"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_STODATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("serach"), (int)m_enLang);
	SetDlgItemText(ID_BTN_MANUAL_JOB_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("update"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_UPDATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_MANUAL_JOB_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\"), _T("dlg_jobinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_MANUAL_JOB_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("location"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_LOCATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("locationto"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_LOCATION_TO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobpriority"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_PRIORITY, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\"), _T("dlg_jobmanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobcnt"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_MANUAL_JOB_CNT, strValue);

	//strValue = CLib::GetIniStringFromPath(strFullPath, _T("sign"), (int)m_enLang);
	//SetDlgItemText(IDOK, strValue);
}

void CManualJob::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;


	m_btnManualJobAdd.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnManualJobAdd.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("copy"), strExtension)), NULL, 5, 5);
	m_btnManualJobAdd.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnManualJobAdd.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnManualJobDataClear.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnManualJobDataClear.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cut"), strExtension)), NULL, 5, 5);
	m_btnManualJobDataClear.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnManualJobDataClear.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);
}

void CManualJob::OnBnClickedBtnManulJobInsert()
{
	CString strSql = _T("");
	CString strWhTyp = _T("");
	CString strStartPos = _T("");
	CString strStartLocation = _T("");
	CString strDestPos = _T("");
	CString strDestLocation = _T("");
	CString strJobTyp = _T("");
	CString strRemarks = _T("");
	CString strStartLocBankFork1 = _T("");
	CString strStartLocBayFork1 = _T("");
	CString strStartLocLevFork1 = _T("");
	CString strDestLocBankFork1 = _T("");
	CString strDestLocBayFork1 = _T("");
	CString strDestLocLevFork1 = _T("");
	CString strProductSize = _T("");
	CString strJobPriority = _T("");
	CString strJobCnt = _T("");
	CString strScNo = _T("");
	CString strCellUseYn = _T("");
	CString strCellScPltJobTyp = _T("");
	CString strCvScPltJobTyp = _T("");
	int nCnt = 0;

	CStringList strJobMstList;

	UpdateData(TRUE);
	strWhTyp = m_pDoc->m_WH_TYP;
	m_cbxManualJobStartPos.GetWindowText(strStartPos);
	m_cbxManualJobDestPos.GetWindowText(strDestPos);
	m_cbxManualJobJobTyp.GetWindowText(strJobTyp);

	m_edtManualJobRemark.GetWindowText(strRemarks);
	m_maskedtManualJobLocationFr.GetWindowText(strStartLocation);
	m_maskedtManualJobLocationTo.GetWindowText(strDestLocation);
	m_cbxManualJobProductSize.GetWindowText(strProductSize);

	//0100101
	strStartLocBankFork1 = strStartLocation.Mid(0,2);
	strStartLocBayFork1 = strStartLocation.Mid(2,3);
	strStartLocLevFork1 = strStartLocation.Mid(5,2);
	strDestLocBankFork1 = strDestLocation.Mid(0,2);
	strDestLocBayFork1 = strDestLocation.Mid(2,3);
	strDestLocLevFork1 = strDestLocation.Mid(5,2);

	if(AfxMessageBox(m_pDoc->GetMsgLangDef(_T("입력한 작업을 생성하시겠습니까?")), MB_YESNO) == IDYES)
    {
		//창고구분
		strWhTyp = m_cbxManualJobWhTyp.GetItemKey(m_cbxManualJobWhTyp.GetCurSel());
		if (m_cbxManualJobWhTyp.GetCurSel() == CB_ERR || strWhTyp == _T("ALL"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("창고를 선택해주세요.")));
			return ;
		}

		//작업구분
		strJobTyp = m_cbxManualJobJobTyp.GetItemKey(m_cbxManualJobJobTyp.GetCurSel());
		if (m_cbxManualJobJobTyp.GetCurSel() == CB_ERR || strJobTyp == _T("ALL"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업구분을 선택해주세요.")));
			return ;
		}

		//출발위치
		strStartPos = m_cbxManualJobStartPos.GetItemKey(m_cbxManualJobStartPos.GetCurSel());
		
		//도착지
		strDestPos = m_cbxManualJobDestPos.GetItemKey(m_cbxManualJobDestPos.GetCurSel());
		if (strDestPos == _T("108"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 도착지는 출고할 수 없습니다.")));
			return ;
		}

		//PULP 단수 체크
		strProductSize = m_cbxManualJobProductSize.GetItemKey(m_cbxManualJobProductSize.GetCurSel());
		if (m_cbxManualJobProductSize.GetCurSel() == CB_ERR || strProductSize == _T("ALL"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("PULP 단수를 선택해주세요.")));
			return ;
		}

		//우선순위 체크
		strJobPriority = m_cbxManualJobPriority.GetItemKey(m_cbxManualJobPriority.GetCurSel());
		if (strJobPriority == _T(""))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("우선순위를 선택해주세요.")));
			return ;
		}

		//작업개수 체크
		strJobCnt = m_cbxManualJobCnt.GetItemKey(m_cbxManualJobCnt.GetCurSel());
		if (strJobCnt == _T(""))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업개수를 선택해주세요.")));
			return ;
		}


		//로케이션 체크
		if(strJobTyp == "1")
		{
			if(strDestLocation.GetLength() > 0)
			{
				if(strDestLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착 로케이션이 다릅니다.")));
					return ;
				}
			}

			if (strStartPos == _T("ALL"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발지를 선택해주세요.")));
					return ;
			}

			//입고때 출발 로케이션 00-000-00
			strStartLocBankFork1 = _T("0");
			strStartLocBayFork1 = _T("0");
			strStartLocLevFork1 = _T("0");

			//로케이션에 따른 SC 구하기.
			GetQrySelectCELLMST(strDestLocBankFork1, strDestLocBayFork1, strDestLocLevFork1, strScNo, strCellUseYn, strCellScPltJobTyp, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 도착 로케이션 입니다.")));
				return ;
			}
			strDestPos = strScNo;

			//해당 작업대의 PLT 정보 구하기
			GetQrySelectCV_DATA(strStartPos, strCvScPltJobTyp, nCnt);
			if (nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업대의 대한 정보가 없습니다.")));
				return ;
			}

			if(strCellScPltJobTyp != strCvScPltJobTyp)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("선택한 작업대와 선택한 로케이션은 PLT 정보가 다릅니다.")));
				return ;
			}

		}
		else if(strJobTyp == "2")
		{
			if(strStartLocation.GetLength() > 0)
			{
				if(strStartLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션이 다릅니다.")));
					return ;
				}
			}

			if (strDestPos == _T("ALL"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지를 선택해주세요.")));
					return ;
			}

			//출고때 도착 로케이션 00-000-00
			strDestLocBankFork1 = _T("0");
			strDestLocBayFork1 = _T("0");
			strDestLocLevFork1 = _T("0");

			//로케이션에 따른 SC 구하기.
			GetQrySelectCELLMST(strStartLocBankFork1, strStartLocBayFork1, strStartLocLevFork1, strScNo, strCellUseYn, strCellScPltJobTyp, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 출발 로케이션 입니다.")));
				return ;
			}
			strStartPos = strScNo;

			//해당 작업대의 PLT 정보 구하기
			GetQrySelectCV_DATA(strDestPos, strCvScPltJobTyp, nCnt);
			if (nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업대의 대한 정보가 없습니다.")));
				return ;
			}

			if(strCellScPltJobTyp != strCvScPltJobTyp)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("선택한 작업대와 선택한 로케이션은 PLT 정보가 다릅니다.")));
				return ;
			}
		}
		else if(strJobTyp == "3")
		{
			strStartLocBankFork1 = _T("0");
			strStartLocBayFork1 = _T("0");
			strStartLocLevFork1 = _T("0");
			strDestLocBankFork1 = _T("0");
			strDestLocBayFork1 = _T("0");
			strDestLocLevFork1 = _T("0");
		}
		else if(strJobTyp == "4")
		{
			if(strStartLocation.GetLength() > 0)
			{
				if(strStartLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션이 다릅니다.")));
					return ;
				}
			}

			if(strDestLocation.GetLength() > 0)
			{
				if(strDestLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착 로케이션이 다릅니다.")));
					return ;
				}
			}

			//출발, 도착 로케이션 확인
			GetQrySelectCELLMST(strStartLocBankFork1, strStartLocBayFork1, strStartLocLevFork1, strScNo, strCellUseYn, strCellScPltJobTyp, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 출발 로케이션 입니다.")));
				return ;
			}

			strStartPos = strScNo;

			GetQrySelectCELLMST(strDestLocBankFork1, strDestLocBayFork1, strDestLocLevFork1, strScNo, strCellUseYn, strCvScPltJobTyp, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 도착 로케이션 입니다.")));
				return ;
			}

			strDestPos = strScNo;

			if(strStartPos != strDestPos)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션과 도착 로케이션이 동일한 SC가 아닙니다.")));
				return ;
			}

			if(strCellScPltJobTyp != strCvScPltJobTyp)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션과 도착 로케이션의 PLT 정보가 다릅니다.")));
				return ;
			}

		}
		else if(strJobTyp == "5")
		{
			if(strStartLocation.GetLength() > 0)
			{
				if(strStartLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션이 다릅니다.")));
					return ;
				}
			}

			if(strDestLocation.GetLength() > 0)
			{
				if(strDestLocation.GetLength() != 7)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착 로케이션이 다릅니다.")));
					return ;
				}
			}

			//출발, 도착 로케이션 확인
			GetQrySelectCELLMST(strStartLocBankFork1, strStartLocBayFork1, strStartLocLevFork1, strScNo, strCellUseYn, strCellScPltJobTyp, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 출발 로케이션 입니다.")));
				return ;
			}

			strStartPos = strScNo;

			GetQrySelectCELLMST(strDestLocBankFork1, strDestLocBayFork1, strDestLocLevFork1, strScNo, strCellUseYn, strCvScPltJobTyp, nCnt);
			if (strCellUseYn == "N" || nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("사용할 수 없는 도착 로케이션 입니다.")));
				return ;
			}

			strDestPos = strScNo;

			if (strStartPos == strDestPos)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션과 도착 로케이션의 SC가 동일한 호기입니다.")));
				return ;
			}

			if(strCellScPltJobTyp != strCvScPltJobTyp)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 로케이션과 도착 로케이션의 PLT 정보가 다릅니다.")));
				return ;
			}
		}
		else if(strJobTyp == "6")
		{
			if (strStartPos == _T("ALL"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발지를 선택해주세요.")));
					return ;
			}
			if (strDestPos == _T("ALL"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지를 선택해주세요.")));
					return ;
			}


			GetQrySelectCV_DATA(strStartPos, strCellScPltJobTyp, nCnt);
			if (nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 작업대의 대한 정보가 없습니다.")));
				return ;
			}

			GetQrySelectCV_DATA(strDestPos, strCvScPltJobTyp, nCnt);
			if (nCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착 작업대의 대한 정보가 없습니다.")));
				return ;
			}

			if(strCellScPltJobTyp != strCvScPltJobTyp)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발 작업대와 도착 작업대의 PLT 정보가 다릅니다.")));
				return ;
			}

			strStartLocBankFork1 = _T("0");
			strStartLocBayFork1 = _T("0");
			strStartLocLevFork1 = _T("0");
			strDestLocBankFork1 = _T("0");
			strDestLocBayFork1 = _T("0");
			strDestLocLevFork1 = _T("0");
		}

		if (strStartPos == _T("")){ strStartPos = _T("0");}
		if (strDestPos == _T("")){ strDestPos = _T("0");}
		if (strJobTyp == _T("")){ strJobTyp = _T("0");}
		if (strProductSize == _T("")){ strProductSize = _T("0");}

		strWhTyp.Trim();
		strStartPos.Trim();
		strStartLocation.Trim();
		strDestPos.Trim();
		strDestLocation.Trim();
		strJobTyp.Trim();
		strProductSize.Trim();
		strJobPriority.Trim();
		strJobCnt.Trim();

		int nJobCnt = CConvert::ToInt(strJobCnt);

		//작업대 이동일 때만 작업개수. 그 외에는 1건
		if(strJobTyp != "6")
		{
			nJobCnt = 1;
		}

		m_pDoc->BeginTrans_DLG();

		for(int i = 0; i < nJobCnt; i++)
		{
			
			strSql.Format(_T(" INSERT INTO JOB_MST     (  WH_TYP				\n")
			_T("					                , LUGG_NO					\n")
			_T("					                , START_POS					\n")
			_T("					                , START_LOCATION			\n")
			_T("                                    , DEST_POS					\n")
			_T("                                    , DEST_LOCATION				\n")
			_T("                                    , PRODUCT_SIZE				\n")
			_T("                                    , JOB_TYP					\n")
			_T("                                    , JOB_STATUS				\n")
			_T("                                    , JOB_PRIORITY				\n")
			_T("                                    , INS_DT					\n")
			_T("                                    , INS_USER_ID				\n")
			_T("                                    , REMARKS 					\n")
			_T("                                    , WC_STEP )					\n")
			_T("                            VALUES ('%s'						\n")
			_T("                                   , nextval('manual_seq')		\n") 
			_T("                                   , '%s'						\n")
			_T("                                   , '%02s-%03s-%02s'			\n")
			_T("                                   , '%s'						\n")
			_T("                                   , '%02s-%03s-%02s'			\n")
			_T("                                   , '%s'						\n")
			_T("                                   ,'%s'						\n")
			_T("								   , '99'						\n") //작업상태 '신규' 고정
			_T("								   , '%s'						\n") //우선순위
			_T("                                   , ") + m_pDoc->SYSDATE + _T(" \n") 
			_T("                                   , '%s'						 \n")
			_T("                                   , '%s' 						 \n")
			_T("                                   , '0' )						 ")  , strWhTyp, strStartPos, strStartLocBankFork1, strStartLocBayFork1, strStartLocLevFork1, strDestPos, strDestLocBankFork1, strDestLocBayFork1, strDestLocLevFork1, strProductSize, strJobTyp, strJobPriority, m_pDoc->m_strId, strRemarks);
			

			BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == FALSE || isSuccess < 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
				m_pDoc->RollbackTrans_DLG();
				return;
			}
		}
		
		CString strStartLoc = strStartLocBankFork1 + _T("-") + strStartLocBayFork1 + _T("-") + strStartLocLevFork1;
		CString strDestLoc = strDestLocBankFork1 + _T("-") + strDestLocBayFork1 + _T("-") + strDestLocLevFork1;
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("수동 작업 지시 -> 출발지 : %s , 출발로케이션 : %s , 도착지 : %s, 도착로케이션 : %s, PULP 단수 : %s, 작업구분 : %s, 작업개수 : %s"), strStartPos, strStartLoc, strDestPos, strDestLoc, strProductSize, strJobTyp, strJobCnt);
		
		if (!m_pDoc->GetQueryInsertClientLog(_T("CManualJob"), _T(""), _T(""), _T(""), strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}

		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SUCCESS")));
		refresh_Chk();
		m_pDoc->CommitTrans_DLG();
	}
}

CString CManualJob::GetJobMstQry(BOOL bSearch)
{
	CString strSql = _T("");

	strSql.Format(_T(" SELECT '' as Chk										\n")
					_T("      , JMT.WH_TYP									\n") 
		_T("      , COM1.CCD_NM_KOR as 창고구분					\n")
		_T("		, JMT.LUGG_NO as 작업번호						\n")
		_T("		, JMT.START_POS as 출발지						\n")
		_T("		, JMT.START_LOCATION as 출발랙					\n")
		_T("		, JMT.DEST_POS as 도착지						\n")
		_T("		, JMT.DEST_LOCATION as 도착랙					\n")
		_T("		, JMT.TURN as 턴여부							\n")
		_T("		, JMT.PRODUCT_ID as 제품번호					\n")
		_T("		, JMT.PRODUCT_SIZE as 제품크기					\n")
		_T("		, JMT.JOB_TYP									\n")
		_T("		, COM2.CCD_NM_KOR as 작업구분					\n")
		_T("		, JMT.BCR_TOP as 상단바코드						\n")
		_T("		, JMT.BCR_BOTTOM as 하단바코드					\n")
		_T("		, JMT.MES_WRITE_YN as MES보고여부				\n")
		_T("		, JMT.JOB_STATUS								\n")
		_T("		, COM3.CCD_NM_KOR as 작업상태					\n")
		_T("		, JMT.JOB_PRIORITY as 우선순위					\n")
		_T("		, JMT.MES_ERROR_CD as MES에러					\n")
		_T("		, JMT.OD_LAST_PAGE as 지시프로그램				\n")
		_T("		, JMT.OD_LAST_USER as 지시자					\n")
		_T("		, JMT.JOB_START_DT as 최초구동시간				\n")
		_T("      , JMT.DURATION_TIME as 랙지속시간				\n")
		_T("		, JMT.UPD_DT as 수정시간						\n")
		_T("		, JMT.UPD_USER_ID as 수정자						\n")
		_T("		, JMT.INS_DT as 생성시간						\n")
		_T("		, JMT.INS_USER_ID as 생성자						\n")
		_T("   FROM JOB_MST JMT									\n")
		_T("		LEFT OUTER JOIN (SELECT CCD_CD					\n")
		_T("							  , CCD_NM_KOR				\n")
		_T("						   FROM COMMON_CODE				\n")
		_T("						  WHERE CDX_CD = 'WH_TYP' 		\n")
		_T("						)COM1							\n")
		_T("					 ON JMT.WH_TYP = COM1.CCD_CD 		\n")
		_T("		LEFT OUTER JOIN (SELECT CCD_CD					\n")
		_T("							  , CCD_NM_KOR				\n")
		_T("						   FROM COMMON_CODE				\n")
		_T("						  WHERE CDX_CD = 'JOB_TYP' 		\n")
		_T("						)COM2							\n")
		_T("					 ON JMT.JOB_TYP = COM2.CCD_CD 		\n") 
		_T("		LEFT OUTER JOIN (SELECT CCD_CD					\n")	
		_T("							  , CCD_NM_KOR				\n")
		_T("							FROM COMMON_CODE			\n")	
		_T("						   WHERE CDX_CD = 'JOB_STATUS' 	\n")
		_T("						 )COM3							\n")	
		_T("				     ON JMT.JOB_STATUS = COM3.CCD_CD	\n")    
		_T("  WHERE 1 = 1 \n"));				    

	CString strTemp = _T("");
	strTemp.Trim();

	m_cbxManualJobWhTyp.GetWindowText(strTemp);
	if (strTemp != "")
	{
		strSql += _T("AND COM1.CCD_NM_KOR = '") + strTemp + _T("' \n");
	}
	m_cbxManualJobStartPos.GetWindowText(strTemp);
	if (strTemp != "")
	{		
		strSql += _T("AND JMT.START_POS ='") + strTemp + _T("' \n");	
	}
	m_cbxManualJobDestPos.GetWindowText(strTemp);
	if (strTemp != "")
	{		
		strSql += _T("AND JMT.DEST_POS ='") + strTemp + _T("' \n");	
	}
	m_cbxManualJobJobTyp.GetWindowText(strTemp);
	if (strTemp != "")
	{		
		strSql += _T("AND COM2.CCD_NM_KOR ='") + strTemp + _T("' \n");	
	}
	m_edtManualJobDestLocation.GetWindowText(strTemp);
	if (strTemp != "")
	{		
		strSql += _T("AND JMT.DURATION_TIME ='") + strTemp + _T("' \n");	
	}

	strSql += _T("   ORDER BY JMT.LUGG_NO DESC \n");

	return strSql;
}

CString CManualJob::GetQryUpdate( CString strWH_TYP, CString strLugg_No, CString strJopTyp)
{

	CString strSql = _T("");
	CString CRLF = _T("\r\n");


	strSql = _T("");
	strSql += CRLF + _T("UPDATE JOB_MST");
	strSql += CRLF + _T("   SET JOB_STATUS ='29'");
	strSql += CRLF + _T(" WHERE WH_TYP = '") + strWH_TYP + _T("'");
	strSql += CRLF + _T("   AND  LUGG_NO = '") + strLugg_No + _T("'");
	strSql += CRLF + _T("   AND JOB_TYP IN ('1','2','4')");
	strSql += CRLF + _T("   AND (JOB_STATUS > 20 AND JOB_STATUS < 30)");

	return strSql;
}

BEGIN_EVENTSINK_MAP(CManualJob, CSkinDialog)
END_EVENTSINK_MAP()

CString CManualJob::GetQryDelete( CString strWH_TYP, CString strLugg_No)
{

	CString strSql = _T("");
	CString CRLF = _T("\r\n");


	strSql = _T("");
	strSql += CRLF + _T("DELETE FROM JOB_MST");
	strSql += CRLF + _T(" WHERE WH_TYP = '") + strWH_TYP + _T("'");
	strSql += CRLF + _T("   AND LUGG_NO = '") + strLugg_No + _T("'");

	return strSql;
}

CString CManualJob::GetQryCellByJobTyp( CString strWH_TYP, CString strSC_NO, CString strLOCATION, CString strJOB_TYP, CString strJOB_STATUS)
{

	CString strSql = _T("");
	CString CRLF = _T("\r\n");

	if(strJOB_TYP == "1")
	{
		strSql = _T("");
		strSql += CRLF + _T("UPDATE CELL_MST");
		strSql += CRLF + _T("   SET CELL_STA = '0'");
		strSql += CRLF + _T(" WHERE WH_TYP = '") + strWH_TYP + _T("'");
		strSql += CRLF + _T("   AND CELL_STA <> '2'");
		strSql += CRLF + _T("   AND CELL_NO = '") + strLOCATION.Trim() + _T("'");
		strSql += CRLF + _T("   AND CELL_SC_NO = '") + strSC_NO.Trim() + _T("'");

	}
	else if(strJOB_TYP == "2")
	{
		if(CConvert::ToInt(strJOB_STATUS) > 20)
		{
			strSql = _T("");
			strSql += CRLF + _T("UPDATE CELL_MST");
			strSql += CRLF + _T("   SET CELL_STA = '2'");
			strSql += CRLF + _T(" WHERE WH_TYP = '") + strWH_TYP + _T("'");
			strSql += CRLF + _T("   AND CELL_STA <> '2'");
			strSql += CRLF + _T("   AND CELL_NO = '") + strLOCATION.Trim() + _T("'");
			strSql += CRLF + _T("   AND CELL_SC_NO = '") + strSC_NO.Trim() + _T("'");

		}
	}


	return strSql;
}

CString CManualJob::GetQryJobMst()
{
	CString strSql = _T("");
	CString strTemp = _T("");	
	CString strWH_TYP = _T("");
	CString strLUGG_NO = _T("");
	CString strJOB_TYP = _T("");
	CString strJOB_STATUS = _T("");
	CString strSTART_LOC = _T("");
	CString strDEST_LOC = _T("");
	CString strSTART_POS = _T("");
	CString strDEST_POS = _T("");
	CString strBCR_TOP = _T("");
	CString strBCR_BOTTOM = _T("");
	CString strREMARKS = _T("");
	CTime tDATE_FROM, tDATE_TO, tTIME_FROM, tTIME_TO;
	CString strdtFrom, strdtTo;

	m_cbxManualJobWhTyp.GetWindowText(strWH_TYP);
    m_cbxManualJobJobTyp.GetWindowText(strJOB_TYP);
	m_edtManualJobStartLocation.GetWindowText(strSTART_LOC);
	m_edtManualJobDestLocation.GetWindowText(strDEST_LOC);
	m_cbxManualJobStartPos.GetWindowText(strSTART_POS);
    m_cbxManualJobDestPos.GetWindowText(strDEST_POS);
	m_edtManualJobRemark.GetWindowText(strREMARKS);
   
	strWH_TYP = m_cbxManualJobWhTyp.GetItemKey(m_cbxManualJobWhTyp.GetCurSel());
	strJOB_TYP = m_cbxManualJobJobTyp.GetItemKey(m_cbxManualJobJobTyp.GetCurSel());
	strSTART_POS = m_cbxManualJobStartPos.GetItemKey(m_cbxManualJobStartPos.GetCurSel());
	strDEST_POS = m_cbxManualJobDestPos.GetItemKey(m_cbxManualJobDestPos.GetCurSel());
	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tDATE_FROM, tTIME_FROM);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);

	

	if (strJOB_TYP == _T("1"))
	{
		strSTART_LOC = _T("00-000-00");
	}
	
	if (strJOB_TYP == _T("2"))
	{
		strDEST_LOC = _T("00-000-00");
	}

	strWH_TYP.Trim();
	strLUGG_NO.Trim();
	strJOB_TYP.Trim();
	strJOB_STATUS.Trim();
	strSTART_LOC.Trim();
	strDEST_LOC.Trim();
	strSTART_POS.Trim();
	strDEST_POS.Trim();
	strBCR_TOP.Trim();
	strBCR_BOTTOM.Trim();

	
	strSql.Format(_T(" SELECT '' AS CheckBox																					    \n")	
				  _T("		, ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, ' ') AS WH_TYP														\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(JM.LUGG_NO, ' ') AS LUGG_NO																	\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR, ' ') AS JOB_TYP														\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(CCD_JOB_STATUS.CCD_NM_KOR, JM.JOB_STATUS) AS JOB_STATUS										\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(JM.START_POS, ' ') AS START_POS																\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(JM.START_LOCATION, ' ') AS START_LOCATION													\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(JM.DEST_POS, ' ') AS DEST_POS																\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(JM.DEST_LOCATION, ' ') AS DEST_LOCATION														\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(JM.BCR_TOP, ' ') AS BCR_TOP																	\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(JM.BCR_BOTTOM, ' ') AS BCR_BOTTOM															\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(JM.REMARKS, ' ') AS REMARKS											        				\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(TO_CHAR(JM.JOB_START_DT, 'MM/DD/YYYY HH24:MI:SS'), '00-00-0000 00:00:00') AS JOB_START_DT	\n")	
				  _T("		, ") + m_pDoc->NVL + _T("(TO_CHAR(JM.INS_DT, 'MM/DD/YYYY HH24:MI:SS'), '00-00-0000 00:00:00') AS INS_DT				\n")	
				  _T("   FROM JOB_MST JM LEFT OUTER JOIN COMMON_CODE CCD_JOB_STATUS													\n")	
				  _T("				                  ON CCD_JOB_STATUS.CDX_CD = 'JOB_STATUS'										\n")	
				  _T("				                 AND JM.JOB_STATUS = CCD_JOB_STATUS.CCD_CD										\n")	
				  _T("                   LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP														\n")
				  _T("								  ON CCD_WH_TYP.CDX_CD = 'WH_TYP'												\n")	
				  _T("								 AND JM.WH_TYP = CCD_WH_TYP.CCD_CD												\n")	
				  _T("                   LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP													\n")
				  _T("								  ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP'												\n")	
				  _T("								 AND JM.JOB_TYP = CCD_JOB_TYP.CCD_CD											\n")
				  _T("	WHERE JM.WH_TYP = '%s'																						\n")
				  _T("    AND TO_CHAR(JM.JOB_START_DT , 'YYYYMMDDHH24MISS') BETWEEN '") + strdtFrom + _T("' AND '") + strdtTo + _T("'"), strWH_TYP);
	 
	
	if (strLUGG_NO != "")
	{		
		strSql += _T("AND JM.LUGG_NO =  '") + strLUGG_NO + _T("' \n");	
	}

	if (strBCR_TOP != "")
	{		
		strSql += _T("AND JM.BCR_TOP =  '") + strBCR_TOP + _T("' \n");	
	}
	
	if (strBCR_BOTTOM != "")
	{		
		strSql += _T("AND JM.BCR_BOTTOM =  '") + strBCR_BOTTOM + _T("' \n");	
	}

	if ((strSTART_POS != "") && (strSTART_POS != _T("ALL")))
	{	
		strSql += _T("AND JM.START_POS ='") + strSTART_POS + _T("' \n");	
	}
	
	if ((strDEST_POS != "") && (strDEST_POS != _T("ALL")))
	{		
		strSql += _T("AND JM.DEST_POS ='") + strDEST_POS + _T("' \n");	
	}
	
	if ((strJOB_TYP != "") && (strJOB_TYP != _T("ALL")))
	{	
		strSql += _T("AND JM.JOB_TYP ='") + strJOB_TYP + _T("' \n");	
	}
	
	if ((strJOB_STATUS != "") && (strJOB_STATUS != _T("ALL")))
	{	
		strSql += _T("AND JM.JOB_STATUS ='") + strJOB_STATUS + _T("' \n");	
	}

   	strSql += _T("   ORDER BY JM.LUGG_NO DESC \n");


   return strSql;
}

void CManualJob::refresh_Chk()
{
	m_cbxManualJobJobTyp.SetCurSel(0);
	m_cbxManualJobStartPos.SetCurSel(0);
	m_cbxManualJobDestPos.SetCurSel(0);
	m_cbxManualJobProductSize.SetCurSel(0);
	m_cbxManualJobPriority.SetCurSel(0);
	m_cbxManualJobCnt.SetCurSel(0);

	m_edtManualJobStartLocation.SetWindowText(_T("01-001-01")); //출발 롴
	m_edtManualJobDestLocation.SetWindowText(_T("01-002-01")); //도착 롴
	m_maskedtManualJobLocationFr.EnableMask(_T("dd ddd dd"), _T("__-___-__"), _T(' '));
	m_maskedtManualJobLocationFr.SetWindowText(_T("01-001-01"));

	m_maskedtManualJobLocationTo.EnableMask(_T("dd ddd dd"), _T("__-___-__"), _T(' '));
	m_maskedtManualJobLocationTo.SetWindowText(_T("01-002-01"));

	m_edtManualJobRemark.SetWindowText(_T(""));
}


void CManualJob::OnBnClickedBtnManualJobClear()
{
	refresh_Chk();
}


void CManualJob::OnSelchangeComboManualJobEditJobTyp()
{
	//EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	//SetFontNation((int)pEn);

	CString strJobTyp =_T("");
	
	UpdateData(TRUE);

	strJobTyp = m_cbxManualJobJobTyp.GetItemKey(m_cbxManualJobJobTyp.GetCurSel());
	if (strJobTyp == _T("0"))
	{
		m_maskedtManualJobLocationFr.EnableWindow(FALSE);
		m_maskedtManualJobLocationTo.EnableWindow(FALSE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(FALSE);
	}
	else if (strJobTyp == _T("1"))
	{
		m_maskedtManualJobLocationFr.EnableWindow(FALSE);
		m_maskedtManualJobLocationTo.EnableWindow(TRUE);
		m_cbxManualJobStartPos.EnableWindow(TRUE);
		m_cbxManualJobDestPos.EnableWindow(FALSE);
	}
	else if (strJobTyp == _T("2"))
	{
		m_maskedtManualJobLocationFr.EnableWindow(TRUE);
		m_maskedtManualJobLocationTo.EnableWindow(FALSE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(TRUE);
	}
	else if (strJobTyp == _T("3"))
	{
		m_maskedtManualJobLocationFr.EnableWindow(FALSE);
		m_maskedtManualJobLocationTo.EnableWindow(FALSE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(FALSE);
	}
	else if (strJobTyp == _T("4"))
	{
		m_maskedtManualJobLocationFr.EnableWindow(TRUE);
		m_maskedtManualJobLocationTo.EnableWindow(TRUE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(FALSE);
	}
	else if (strJobTyp == _T("5"))
	{
		m_maskedtManualJobLocationFr.EnableWindow(TRUE);
		m_maskedtManualJobLocationTo.EnableWindow(TRUE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(FALSE);
	}
	else if (strJobTyp == _T("6"))
	{
		m_maskedtManualJobLocationFr.EnableWindow(FALSE);
		m_maskedtManualJobLocationTo.EnableWindow(FALSE);
		m_cbxManualJobStartPos.EnableWindow(TRUE);
		m_cbxManualJobDestPos.EnableWindow(TRUE);
	}
	else
	{
		m_maskedtManualJobLocationFr.EnableWindow(FALSE);
		m_maskedtManualJobLocationTo.EnableWindow(FALSE);
		m_cbxManualJobStartPos.EnableWindow(FALSE);
		m_cbxManualJobDestPos.EnableWindow(FALSE);
	}
}


void CManualJob::OnCbnSelchangeComboManualJobWhtype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

CString CManualJob::GetQrySelectCELLMST(CString strBANK, CString strBAY, CString strLEV, CString& strSC_NO, CString& strCELL_USE_YN, CString& strSC_PLT_JOB_TYP,int& nCnt )
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql += CRLF + _T("SELECT SC_NO, CELL_USE_YN, SC_PLT_JOB_TYP ");
	strSql += CRLF + _T("  FROM CELL_MST ");
	strSql += CRLF + _T(" WHERE WH_TYP = ") + CLib::Quot(m_pDoc->m_WH_TYP);
	strSql += CRLF + _T("   AND BANK = ") + CLib::Quot(strBANK);
	strSql += CRLF + _T("   AND BAY = ") + CLib::Quot(strBAY);
	strSql += CRLF + _T("   AND LEV = ") + CLib::Quot(strLEV);
	
	
	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 


	strSC_NO = pRsw->GetItem(_T("SC_NO"));
	strCELL_USE_YN = pRsw->GetItem(_T("CELL_USE_YN"));
	strSC_PLT_JOB_TYP = pRsw->GetItem(_T("SC_PLT_JOB_TYP"));
	nCnt = nRowCnt;

	delete pRsw;
	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CManualJob::GetQrySelectCV_DATA(CString strMC_NO, CString& strSC_PLT_JOB_TYP, int& nCnt )
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql += CRLF + _T("SELECT SC_PLT_JOB_TYP ");
	strSql += CRLF + _T("  FROM CV_DATA ");
	strSql += CRLF + _T(" WHERE WH_TYP = ") + CLib::Quot(m_pDoc->m_WH_TYP);
	strSql += CRLF + _T("   AND MC_NO = ") + CLib::Quot(strMC_NO);
	
	
	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 

	strSC_PLT_JOB_TYP = pRsw->GetItem(_T("SC_PLT_JOB_TYP"));
	nCnt = nRowCnt;

	delete pRsw;
	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}