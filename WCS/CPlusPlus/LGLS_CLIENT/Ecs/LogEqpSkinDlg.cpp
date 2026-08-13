// LogIoSkinDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Ecs.h"
#include "LogEqpSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CLogEqpSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogEqpSkinDlg, CSkinDialog)

	CLogEqpSkinDlg::CLogEqpSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogEqpSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CLogEqpSkinDlg::CLogEqpSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogEqpSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CLogEqpSkinDlg::~CLogEqpSkinDlg()
{
	m_pDoc->m_pLogEqpSkinDlg = NULL;
	this->DestroyWindow();
}

BOOL CLogEqpSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);

}
void CLogEqpSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_LOG_IO_SEARCH,              m_btnSearch);
	DDX_Control(pDX, IDOK,								 m_btnOk);
	DDX_Control(pDX, IDCANCEL,							 m_btnCancel);

	//DDX_Control(pDX, IDC_LBL_WH_TYP,					 m_lblWhTyp);
	DDX_Control(pDX, IDC_LBL_TRAY_BOTTOM,                m_lblTrayBottom);
	DDX_Control(pDX, IDC_LBL_TRAY_TOP,					 m_lblTrayTop);
	DDX_Control(pDX, IDC_LBL_LUGG_NO,					 m_lblLuggNo);
	DDX_Control(pDX, IDC_LBL_REQUEST_ID,                 m_lblRequestId);
	DDX_Control(pDX, IDC_LBL_FROM_DATE,					 m_lblFromDate);
	DDX_Control(pDX, IDC_LBL_TO_DATE,					 m_lblToDate);
	DDX_Control(pDX, IDC_LBL_FROM_TRACK,                 m_lblFromTrack);
	DDX_Control(pDX, IDC_LBL_TO_TRACK,					 m_lblToTrack);

	DDX_Control(pDX, IDC_EDT_TRAY_BOTTOM,			     m_edtTrayBottom);
	DDX_Control(pDX, IDC_EDT_TRAY_TOP,					 m_edtTrayTop);
	DDX_Control(pDX, IDC_EDT_LUGG_NO,					 m_edtLuggNo);
	DDX_Control(pDX, IDC_EDT_REQUEST_ID,				 m_edtRequestId);
	DDX_Control(pDX, IDC_EDT_FROM_TRACK,				 m_edtFromTrack);
	DDX_Control(pDX, IDC_EDT_TO_TRACK,					 m_edtToTrack);

	DDX_Control(pDX, IDC_DTP_FROM_DATE,					 m_dtpFrom);
	DDX_Control(pDX, IDC_DTP_FROM_TIME,					 m_dtpFromTime);
	DDX_Control(pDX, IDC_DTP_TO_DATE,					 m_dtpTo);
	DDX_Control(pDX, IDC_DTP_TO_TIME,					 m_dtpToTime);


	DDX_Control(pDX, IDC_SPD_LOG_EQP,					 m_spdLog);

	DDX_Control(pDX, IDC_LBL_EQP_LOG_WH_TYP,		m_lblEqpLogWhTyp);
	DDX_Control(pDX, IDC_LBL_EQP_LOG_EQP_TYP,		m_lblEqpLogEqpTyp);
	DDX_Control(pDX, IDC_CBX_EQP_LOG_WH_TYP,		m_cbxEqpLogWhTyp);
	DDX_Control(pDX, IDC_CBX_EQP_LOG_EQP_TYP,		m_cbxEqpLogEqpTyp);
	
	DDX_Control(pDX, IDC_LBL_EQP_KOR_LOG,		m_lblEqpKorLog);
	DDX_Control(pDX, IDC_EDT_EQP_KOR_LOG,		m_cbxEqpKorLog);

	DDX_Control(pDX, IDC_LBL_EQP_MST_RESULT,		m_lblEQPMstResult);
	
	DDX_Control(pDX, IDC_GRP_LOG_EQP_SEARCH,		m_grpLogEqpSearch);
	DDX_Control(pDX, IDC_GRP_LOG_EQP_SEARCH2,		m_grpLogEqpSearch2);

	DDX_Control(pDX, IDC_LBL_EQP_MST_CNT,			m_lblEqpMstCnt);

}



BEGIN_MESSAGE_MAP(CLogEqpSkinDlg, CSkinDialog)
	ON_MESSAGE(SSM_CLICK, &CLogEqpSkinDlg::OnLglsHdrSort)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_LOG_IO_SEARCH, &CLogEqpSkinDlg::OnBnClickedBtnLogIoSearch)
END_MESSAGE_MAP()

// [LGLS] 그리드 헤더 클릭 정렬
LRESULT CLogEqpSkinDlg::OnLglsHdrSort(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if(pCell != NULL && pCell->Row == 0 && pCell->Col >= 1)
		m_spdLog.SortByHeaderColumn(pCell->Col);
	return 0;
}
// CLogEqpSkinDlg 메시지 처리기입니다.

void CLogEqpSkinDlg::OnClose()
{
	m_pDoc->m_pLogEqpSkinDlg = NULL;
	delete this;
}

HCURSOR CLogEqpSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLogEqpSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

void CLogEqpSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}


void CLogEqpSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE sizeWideLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_WIDELARGE);

	CRect rc2;
	m_btnOk.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnOk.MoveWindow(rc2.left - 30, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCancel.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCancel.MoveWindow(rc2.left - 15, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnSearch.MoveWindow(rc2.left - 10, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);
}


BOOL CLogEqpSkinDlg::OnInitDialog()
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

	CLib::BindCombo(m_cbxEqpLogWhTyp, _T("WH_TYP"), m_pDoc, (int)pEn);
	CLib::BindCombo(m_cbxEqpLogEqpTyp, _T("EQP_TYP"), m_pDoc, (int)pEn);

	InitializeResource(pEn);

	OnBnClickedBtnLogIoSearch();

	{ CString _tt; GetWindowText(_tt); if(_tt.Find(_T("[")) < 0) SetWindowText(_tt + _T(" [WCS_LOG_PGR]")); }	// [LGLS] 제목에 조회 테이블명
	return TRUE;
}

void CLogEqpSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\128x32\\");

	CString strExtension = _T(".png");
	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnOk.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("ok"), strExtension)), NULL, 5, 5);
	m_btnCancel.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cancel"), strExtension)), NULL, 5, 5);
	m_btnSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);

}

void CLogEqpSkinDlg::RenameResource( EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_LOG_IO_SEARCH, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ok"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDOK, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cancel"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDCANCEL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("logkor"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_KOR_LOG, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectcnt"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_MST_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_LOG_WH_TYP, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqptyp"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_LOG_EQP_TYP, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("traybottom"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_TRAY_BOTTOM, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("traytop"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_TRAY_TOP, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("luggno"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LUGG_NO, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("requestid"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_REQUEST_ID, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromdate"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_FROM_DATE, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("todate"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_TO_DATE, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromtrack"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_FROM_TRACK, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqplog\\"), _T("dlg_eqplog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("totrack"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_TO_TRACK, strValue);
}

void CLogEqpSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RedrawImage();
	RenameResource(nEN_LANG);
}

CString CLogEqpSkinDlg::GetQrySelect()
{
	if(m_pDoc == NULL){ return _T(""); };
	CString strWH_TYP = m_pDoc->m_WH_TYP;
	CString strTRAY_BOTTOM, strTRAY_TOP, strLUGG_NO, strREQUEST_ID, strTRACK_START, strTRACK_DEST = _T("");
	CString strdtFrom, strdtTo;
	CTime tDATE_FROM, tDATE_TO, tTIME_FROM, tTIME_TO;
	m_edtTrayBottom.GetWindowText(strTRAY_BOTTOM);
	m_edtTrayTop.GetWindowText(strTRAY_TOP);
	m_edtLuggNo.GetWindowText(strLUGG_NO);
	m_edtRequestId.GetWindowText(strREQUEST_ID);
	m_edtFromTrack.GetWindowText(strTRACK_START);
	m_edtToTrack.GetWindowText(strTRACK_DEST);
	m_dtpFrom.GetTime(tDATE_FROM);
	m_dtpFromTime.GetTime(tTIME_FROM);
	m_dtpTo.GetTime(tDATE_TO);
	m_dtpToTime.GetTime(tTIME_TO);



	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tDATE_FROM, tTIME_FROM);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);

	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	strSql += CRLF + _T(" SELECT * ");
	strSql += CRLF + _T("   FROM WCS_LOG_PGR ");
	strSql += CRLF + _T("  WHERE WH_TYP =  ") + CLib::Quot(strWH_TYP);
	strSql += CRLF + _T("    AND PGR_NM = 'EQP_TASK' ");
	strSql += CRLF + _T("    AND ( INS_DT >= CONVERT(DATETIME, STUFF(STUFF(STUFF('") + strdtFrom + _T("',13,0,':'),11,0,':'),9,0,' '))");	// [LGLS]
	strSql += CRLF + _T("    AND   INS_DT <=  CONVERT(DATETIME, STUFF(STUFF(STUFF('") + strdtTo + _T("',13,0,':'),11,0,':'),9,0,' ')) )");	// [LGLS]
	if(strTRAY_BOTTOM != _T(""))
	{
		strSql += CRLF + _T("    AND BCR_BOTTOM =  ") + CLib::Quot(strTRAY_BOTTOM);
	}
	if(strTRAY_TOP != _T(""))
	{
		strSql += CRLF + _T("    AND BCR_TOP =  ") + CLib::Quot(strTRAY_TOP);
	}
	if(strLUGG_NO != _T(""))
	{
		strSql += CRLF + _T("    AND LUGG_NO =  ") + CLib::Quot(strLUGG_NO);
	}
	if(strREQUEST_ID != _T(""))
	{
		strSql += CRLF + _T("    AND RQ_INS_ID =  ") + CLib::Quot(strREQUEST_ID);
	}
	if(strTRACK_START != _T(""))
	{
		strSql += CRLF + _T("    AND TRACK_FROM =  ") + CLib::Quot(strTRACK_START);
	}
	if(strTRACK_DEST != _T(""))
	{
		strSql += CRLF + _T("    AND TRACK_TO =  ") + CLib::Quot(strTRACK_DEST);
	}
	return strSql;
}

void CLogEqpSkinDlg::GetEQPLogSelect()
{

	CString strTRAY_BOTTOM, strTRAY_TOP, strLUGG_NO, strREQUEST_ID, strTRACK_START, strTRACK_DEST = _T("");
	CString strdtFrom, strdtTo;
	CString strEqp_typ;
	CString strKorLog;
	CTime tDATE_FROM, tDATE_TO, tTIME_FROM, tTIME_TO;
	m_edtTrayBottom.GetWindowText(strTRAY_BOTTOM);
	m_edtTrayTop.GetWindowText(strTRAY_TOP);
	m_edtLuggNo.GetWindowText(strLUGG_NO);
	m_edtRequestId.GetWindowText(strREQUEST_ID);
	m_edtFromTrack.GetWindowText(strTRACK_START);
	m_edtToTrack.GetWindowText(strTRACK_DEST);
	m_dtpFrom.GetTime(tDATE_FROM);
	m_dtpFromTime.GetTime(tTIME_FROM);
	m_dtpTo.GetTime(tDATE_TO);
	m_dtpToTime.GetTime(tTIME_TO);

	m_cbxEqpKorLog.GetWindowText(strKorLog);

	//SK
	int nWH_TYP = m_cbxEqpLogWhTyp.GetItemData(m_cbxEqpLogWhTyp.GetCurSel());
	CString strWH_TYP = CConvert::ToString(nWH_TYP);
	m_cbxEqpLogEqpTyp.GetWindowText(strEqp_typ);

	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tDATE_FROM, tTIME_FROM);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);


	// lCol,lRow,  lCol2,  lRow2, bDataOnly
	m_spdLog.ClearRange(0, 0, -1, -1, TRUE);
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	int nRowCnt = -1, nIdxRow = 0;
	CString strMessage = _T("");

	strSql += CRLF + _T("SELECT LOG.WH_TYP															 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(EMST.PLC_NO,       '0') as PLC_NO				 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.INS_DT,	   ") + m_pDoc->SYSDATE + _T(") as INS_DT				 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.LOG_SEQ,	   '0') as LOG_SEQ				 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.LUGG_NO,	   '0') as LUGG_NO				 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.BCR_BOTTOM,    '0') as BCR_BOTTOM			 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.BCR_TOP,	   '0') as BCR_TOP				 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.PGR_NM,		   '0') as PGR_NM			 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.LOG_KOR,       '0') as LOG_KOR			 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.TRACK_FROM,    '0') as TRACK_FROM			 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.TRACK_TO,      '0') as TRACK_TO			 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.JOB_STA,       '0') as JOB_STA			 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.RQ_INS_ID,	   '0') as RQ_INS_ID		 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.RQ_INS_DT, ") + m_pDoc->SYSDATE + _T(") as RQ_INS_DT			 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.EQP_TYP,	   '0') as EQP_TYP				 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.SPARE1,	       '0') as SPARE1			 ");
	strSql += CRLF + _T(",		") + m_pDoc->NVL + _T("(LOG.SPARE2,        '0') as SPARE2				 ");	
	strSql += CRLF + _T("	  FROM  WCS_LOG_PGR LOG INNER JOIN EQP_MST EMST			 ");
	strSql += CRLF + _T("		ON  LOG.EQP_TYP = EMST.EQP_TYP						 ");
	strSql += CRLF + _T("		AND LOG.WH_TYP = EMST.WH_TYP						 ");
	strSql += CRLF + _T("	  WHERE LOG.WH_TYP =  ") + CLib::Quot(strWH_TYP);	
	strSql += CRLF + _T("       AND ( INS_DT >= CONVERT(DATETIME, STUFF(STUFF(STUFF('") + strdtFrom + _T("',13,0,':'),11,0,':'),9,0,' '))");	// [LGLS]
	strSql += CRLF + _T("       AND   INS_DT <=  CONVERT(DATETIME, STUFF(STUFF(STUFF('") + strdtTo + _T("',13,0,':'),11,0,':'),9,0,' ')) )");	// [LGLS]
	if(strEqp_typ != _T(""))
	{
		strSql += CRLF + _T("    AND LOG.EQP_TYP =  ") + CLib::Quot(strEqp_typ);
	}	
	if(strKorLog != _T(""))
	{
		strSql += CRLF + _T("    AND LOG.LOG_KOR LIKE '%" +strKorLog + _T("%'"));
	}
	if(strLUGG_NO != _T(""))
	{
		strSql += CRLF + _T("    AND LOG.LUGG_NO =  ") + CLib::Quot(strLUGG_NO);
	}
	if(strTRAY_BOTTOM != _T(""))
	{
		strSql += CRLF + _T("    AND LOG.BCR_BOTTOM LIKE  '%" +strTRAY_BOTTOM + _T("%'"));
	}	
	if(strTRAY_TOP != _T(""))
	{
		strSql += CRLF + _T("    AND LOG.BCR_TOP LIKE  '%" +strTRAY_TOP + _T("%'"));
	}
	if(strREQUEST_ID != _T(""))
	{
		strSql += CRLF + _T("    AND LOG.RQ_INS_ID =  ") + CLib::Quot(strREQUEST_ID);
	}
	if(strTRACK_START != _T(""))
	{
		strSql += CRLF + _T("    AND LOG.TRACK_FROM =  ") + CLib::Quot(strTRACK_START);
	}
	if(strTRACK_DEST != _T(""))
	{
		strSql += CRLF + _T("    AND LOG.TRACK_TO =  ") + CLib::Quot(strTRACK_DEST);
	}
#if ORACLE
	strSql += CRLF + _T("    AND ROWNUM <= 10000									");	
#endif

	strSql += CRLF + _T(" ORDER BY LOG.INS_DT DESC									");		

#if POSTGRESQL
	strSql += CRLF + _T("    LIMIT 10000											");	
#elif MSSQL
	CLib::ApplyTopN(strSql, _T("10000"));	// [LGLS] SQL2008: TOP 10000 instead of LIMIT
#endif
	

	_RecordsetPtr pRsptr = NULL; //m_pDoc->m_pUrmDBAccess->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 

	m_spdLog.SetMaxRows(nRowCnt);
	m_lblEqpMstCnt.SetWindowText(CConvert::ToString(nRowCnt));
	if(nRowCnt < 1)
	{
		delete pRsw;
		m_lblEqpMstCnt.SetWindowText(CConvert::ToString(0));
		return;
	}
	

	for(int i = 0; i < nRowCnt; i++)
	{
		FillSpreadRow(0, nIdxRow , pRsw->GetItem (_T("WH_TYP")));
		FillSpreadRow(1, nIdxRow , pRsw->GetItem(_T("PLC_NO")));
		FillSpreadRow(2, nIdxRow , pRsw->GetItem (_T("INS_DT")));
		FillSpreadRow(3, nIdxRow , pRsw->GetItem (_T("LOG_SEQ")));
		FillSpreadRow(4, nIdxRow , pRsw->GetItem (_T("LUGG_NO")));
		FillSpreadRow(5, nIdxRow , pRsw->GetItem (_T("BCR_BOTTOM")));
		FillSpreadRow(6, nIdxRow , pRsw->GetItem (_T("BCR_TOP")));
		FillSpreadRow(7, nIdxRow , pRsw->GetItem (_T("PGR_NM")));
		FillSpreadRow(8, nIdxRow , pRsw->GetItem (_T("LOG_KOR")));
		FillSpreadRow(9, nIdxRow , pRsw->GetItem (_T("TRACK_FROM")));
		FillSpreadRow(10, nIdxRow , pRsw->GetItem (_T("TRACK_TO")));
		FillSpreadRow(11, nIdxRow , pRsw->GetItem(_T("JOB_STA")));
		FillSpreadRow(12, nIdxRow , pRsw->GetItem(_T("RQ_INS_ID")));
		FillSpreadRow(13, nIdxRow , pRsw->GetItem(_T("RQ_INS_DT")));
		FillSpreadRow(14, nIdxRow , pRsw->GetItem(_T("EQP_TYP")));
		FillSpreadRow(15, nIdxRow , pRsw->GetItem(_T("SPARE1")));
		FillSpreadRow(16, nIdxRow , pRsw->GetItem(_T("SPARE2")));

		nIdxRow++; //안해주면 행 한줄만 보임

		//FillSpreadRowp    Rsw->GetItem(_T("CCD_CD"));
		//strCCD_NM_KOR = pRsw->GetItem(_T("CCD_NM_KOR"));
		//cbx.AddString(strCCD_NM_KOR);
		//cbx.SetItemData(j++, CConvert::ToInt(strCCD_CD));

		pRsw->MoveNext();
	}
}


void CLogEqpSkinDlg::OnBnClickedBtnLogIoSearch()
{
	if(m_pDoc == NULL) { return; };

	CString strSql = GetQrySelect();
	if(strSql == _T("")){ return; }

	CString strMessage = _T("");
	int nRowCnt, nColCnt = -1;
	_RecordsetPtr pRsptr  = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	CStringArray strCols;
	strCols.Add(_T("WH_TYP"));
	strCols.Add(_T("PLC_NO"));
	strCols.Add(_T("INS_DT"));
	strCols.Add(_T("LOG_SEQ"));
	strCols.Add(_T("LUGG_NO"));
	strCols.Add(_T("BCR_BOTTOM"));
	strCols.Add(_T("BCR_TOP"));
	strCols.Add(_T("PGR_NM"));
	strCols.Add(_T("LOG_KOR"));
	strCols.Add(_T("TRACK_FROM"));
	strCols.Add(_T("TRACK_TO"));
	strCols.Add(_T("JOB_STA"));
	strCols.Add(_T("RQ_INS_ID"));
	strCols.Add(_T("RQ_INS_DT"));
	strCols.Add(_T("EQP_TYP"));
	strCols.Add(_T("SPARE1"));
	strCols.Add(_T("SPARE2"));
	nColCnt = strCols.GetSize();
	FillSpreadColumn(strCols);
	m_spdLog.SetMaxRows(nRowCnt);
	pRsw->MoveFirst();

	GetEQPLogSelect();

	for(int nIdxRow = 0; nIdxRow < nRowCnt; nIdxRow++)
	{
		for(int nIdxCol = 0; nIdxCol < nColCnt; nIdxCol++)
		{
			m_spdLog.SetText(nIdxCol + 1, nIdxRow + 1, variant_t(m_pDoc->m_pLang->GetLangValue(pRsw->GetItem(strCols[nIdxCol]), m_nLang)));
		}
		pRsw->MoveNext();
	}

	delete pRsw;
}

void CLogEqpSkinDlg::FillSpreadColumn(CStringArray& strCols)
{
	m_spdLog.SetBackColor(WHITE);
	int nColCnt = strCols.GetSize();
	m_spdLog.SetMaxCols(nColCnt);
	CString strValue = _T("");
	for(int nIdxCol = 0; nIdxCol < nColCnt; nIdxCol++)
	{
		strValue.Format(_T("%s\r"), strCols.GetAt(nIdxCol));
		m_spdLog.SetText(nIdxCol + 1, 0, variant_t(strValue));
		m_spdLog.put_ColWidth(nIdxCol + 1, strValue.GetLength() * 1.5);
	}
	//m_spdLog.put_ColWidth(2, 45);
}

void CLogEqpSkinDlg::FillSpreadRow(int nColIdx, int nRowIdx, CString strValue)
{
	CString aaa = _T("");
	aaa.Format(_T("%s\r"), strValue);
	m_spdLog.SetText(nColIdx + 1, nRowIdx + 1, variant_t(aaa));
}



