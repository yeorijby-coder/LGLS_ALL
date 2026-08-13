// LogEqpJobHisSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "LogEqpJobHisSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CLogEqpJobHisSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogEqpJobHisSkinDlg, CSkinDialog)

	CLogEqpJobHisSkinDlg::CLogEqpJobHisSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogEqpJobHisSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CLogEqpJobHisSkinDlg::CLogEqpJobHisSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogEqpJobHisSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CLogEqpJobHisSkinDlg::~CLogEqpJobHisSkinDlg()
{
	m_pDoc->m_pLogJobHis = NULL;
	this->DestroyWindow();
}

BOOL CLogEqpJobHisSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);

}
void CLogEqpJobHisSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_LOG_IO_SEARCH,              m_btnSearch);
	DDX_Control(pDX, IDOK,								 m_btnOk);
	DDX_Control(pDX, IDCANCEL,							 m_btnCancel);

	DDX_Control(pDX, IDC_LBL_WH_TYP,					 m_lblWhTyp);
	DDX_Control(pDX, IDC_LBL_TRAY_BOTTOM,                m_lblTrayBottom);
	DDX_Control(pDX, IDC_LBL_TRAY_TOP,					 m_lblTrayTop);
	DDX_Control(pDX, IDC_LBL_LUGG_NO,					 m_lblLuggNo);
	DDX_Control(pDX, IDC_LBL_FROM_LOCATION,              m_lblFROM_LOCATION);
	DDX_Control(pDX, IDC_LBL_TO_LOCATION,                m_lblTO_LOCATION);
	DDX_Control(pDX, IDC_LBL_FROM_DATE,					 m_lblFromDate);
	DDX_Control(pDX, IDC_LBL_TO_DATE,					 m_lblToDate);
	DDX_Control(pDX, IDC_LBL_FROM_TRACK,                 m_lblFromTrack);
	DDX_Control(pDX, IDC_LBL_TO_TRACK,					 m_lblToTrack);

	DDX_Control(pDX, IDC_EDT_TRAY_BOTTOM,			     m_edtTrayBottom);
	DDX_Control(pDX, IDC_EDT_TRAY_TOP,					 m_edtTrayTop);
	DDX_Control(pDX, IDC_EDT_LUGG_NO,					 m_edtLuggNo);
	DDX_Control(pDX, IDC_EDT_FROM_LOCATION,				 m_edtFROM_LOCATION);
	DDX_Control(pDX, IDC_EDT_TO_LOCATION,				 m_edtTO_LOCATION);
	DDX_Control(pDX, IDC_EDT_FROM_TRACK,				 m_edtFromTrack);
	DDX_Control(pDX, IDC_EDT_TO_TRACK,					 m_edtToTrack);

	DDX_Control(pDX, IDC_DTP_FROM_DATE,					 m_dtpFrom);
	DDX_Control(pDX, IDC_DTP_FROM_TIME,					 m_dtpFromTime);
	DDX_Control(pDX, IDC_DTP_TO_DATE,					 m_dtpTo);
	DDX_Control(pDX, IDC_DTP_TO_TIME,					 m_dtpToTime);

	DDX_Control(pDX, IDC_SPD_LOG_JOB_HIS,					 m_spdLog);
}


BEGIN_MESSAGE_MAP(CLogEqpJobHisSkinDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_LOG_IO_SEARCH, &CLogEqpJobHisSkinDlg::OnBnClickedBtnLogSearch)
END_MESSAGE_MAP()
// CLogEqpJobHisSkinDlg 메시지 처리기입니다.

void CLogEqpJobHisSkinDlg::OnClose()
{
	m_pDoc->m_pLogJobHis = NULL;
	delete this;
}

HCURSOR CLogEqpJobHisSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLogEqpJobHisSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

void CLogEqpJobHisSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}


void CLogEqpJobHisSkinDlg::RelocationControls()
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


BOOL CLogEqpJobHisSkinDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	DateTime_SetFormat(m_dtpFromTime.m_hWnd, TEXT("hh:mm:ss tt"));
	DateTime_SetFormat(m_dtpToTime.m_hWnd, TEXT("hh:mm:ss tt"));
	m_dtpFromTime.Invalidate();
	m_dtpToTime.Invalidate();
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{	
		RelocationControls();
		m_bInitialized = TRUE;	
	}
	InitializeResource(pEn);

	OnBnClickedBtnLogSearch();

	return TRUE;
}

void CLogEqpJobHisSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\128x32\\");

	CString strExtension = _T(".png");
	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnOk.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("ok"), strExtension)), NULL, 5, 5);
	m_btnCancel.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cancel"), strExtension)), NULL, 5, 5);
	m_btnSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);

}

void CLogEqpJobHisSkinDlg::RenameResource( EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	SetDlgItemText(IDC_BTN_LOG_IO_SEARCH, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ok"), (int)enLang);
	SetDlgItemText(IDOK, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cancel"), (int)enLang);
	SetDlgItemText(IDCANCEL, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)enLang);
	SetDlgItemText(IDC_LBL_WH_TYP, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("traybottom"), (int)enLang);
	SetDlgItemText(IDC_LBL_TRAY_BOTTOM, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("traytop"), (int)enLang);
	SetDlgItemText(IDC_LBL_TRAY_TOP, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("luggno"), (int)enLang);
	SetDlgItemText(IDC_LBL_LUGG_NO, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromlocation"), (int)enLang);
	SetDlgItemText(IDC_LBL_FROM_LOCATION, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("tolocation"), (int)enLang);
	SetDlgItemText(IDC_LBL_TO_LOCATION, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromdate"), (int)enLang);
	SetDlgItemText(IDC_LBL_FROM_DATE, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("todate"), (int)enLang);
	SetDlgItemText(IDC_LBL_TO_DATE, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromtrack"), (int)enLang);
	SetDlgItemText(IDC_LBL_FROM_TRACK, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobhislog\\"), _T("dlg_jobhislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("totrack"), (int)enLang);
	SetDlgItemText(IDC_LBL_TO_TRACK, strValue);

}

void CLogEqpJobHisSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RedrawImage();
	RenameResource(nEN_LANG);
}

CString CLogEqpJobHisSkinDlg::GetQrySelect()
{
	if(m_pDoc == NULL){ return _T(""); };
	CString strWH_TYP = m_pDoc->m_WH_TYP;
	CString strTRAY_BOTTOM, strTRAY_TOP, strLUGG_NO, strTRACK_START, strTRACK_DEST, strFROM_LOCATION, strTO_LOCATION = _T("");
	CString strdtFrom, strdtTo;
	CString strTimeFrom, strTimeTo;
	CTime tDATE_FROM, tDATE_TO, tTIME_FROM, tTIME_TO;
	m_edtTrayBottom.GetWindowText(strTRAY_BOTTOM);
	m_edtTrayTop.GetWindowText(strTRAY_TOP);
	m_edtLuggNo.GetWindowText(strLUGG_NO);
	m_edtFROM_LOCATION.GetWindowText(strFROM_LOCATION);
	m_edtTO_LOCATION.GetWindowText(strTO_LOCATION);
	m_edtFromTrack.GetWindowText(strTRACK_START);
	m_edtToTrack.GetWindowText(strTRACK_DEST);
	m_dtpFrom.GetTime(tDATE_FROM);
	m_dtpFromTime.GetTime(tTIME_FROM);
	m_dtpTo.GetTime(tDATE_TO);
	m_dtpToTime.GetTime(tTIME_TO);

	CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);
	strdtFrom = tDATE_FROM.Format(_T("%Y%m%d"));
	strdtTo = tDATE_TO.Format(_T("%Y%m%d"));
	strTimeFrom = tTIME_FROM.Format("%H%M%S");
	strTimeTo = tTIME_TO.Format("%H%M%S");

	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	strSql += CRLF + _T(" SELECT * ");
	strSql += CRLF + _T("   FROM JOB_MST_HIS ");
	strSql += CRLF + _T("  WHERE WH_TYP =  ") + CLib::Quot(strWH_TYP);
	strSql += CRLF + _T("    AND CONCAT(INS_DATE, INS_TIME) >= ") + CLib::Quot(strdtFrom+strTimeFrom);
	strSql += CRLF + _T("    AND CONCAT(INS_DATE, INS_TIME) <= ") + CLib::Quot(strdtTo+strTimeTo);
	// 	strSql += CRLF + _T("    AND ( INS_DT >= TO_DATE('") + strdtFrom + _T("', 'MM/DD/YYYY HH24:MI:SS')"); //    + CLib::Quot(_T("TO_DATE('{0}','MM/DD/YYYY HH12:MI:SS')"))
	// 	strSql += CRLF + _T("    AND   INS_DT <=  TO_DATE('") + strdtTo + _T("', 'MM/DD/YYYY HH24:MI:SS') )");
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
	if(strFROM_LOCATION != _T(""))
	{
		strSql += CRLF + _T("    AND START_LOCATION =  ") + CLib::Quot(strFROM_LOCATION);
	}
	if(strTO_LOCATION != _T(""))
	{
		strSql += CRLF + _T("    AND DEST_LOCATION =  ") + CLib::Quot(strTO_LOCATION);
	}
	if(strTRACK_START != _T(""))
	{
		strSql += CRLF + _T("    AND START_POS =  ") + CLib::Quot(strTRACK_START);
	}
	if(strTRACK_DEST != _T(""))
	{
		strSql += CRLF + _T("    AND DEST_POS =  ") + CLib::Quot(strTRACK_DEST);
	}
	return strSql;
}

void CLogEqpJobHisSkinDlg::OnBnClickedBtnLogSearch()
{
	if(m_pDoc == NULL) { return; };

	CString strSql = GetQrySelect();
	if(strSql == _T("")){ return; }

	CString strMessage = _T("");
	int nRowCnt, nColCnt = -1;
	_RecordsetPtr pRsptr  = m_pDB->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	CStringArray strCols;
	strCols.Add(_T("WH_TYP"));
	strCols.Add(_T("LUGG_NO"));
	strCols.Add(_T("START_POS"));
	strCols.Add(_T("START_LOCATION"));
	strCols.Add(_T("DEST_POS"));
	strCols.Add(_T("DEST_LOCATION"));
	strCols.Add(_T("JOB_TYP"));
	strCols.Add(_T("BCR_TOP"));
	strCols.Add(_T("BCR_BOTTOM"));
	strCols.Add(_T("JOB_STATUS"));
	strCols.Add(_T("REMARKS"));
	strCols.Add(_T("TRAY_TYP"));
	strCols.Add(_T("TRAY_LEV"));
	strCols.Add(_T("SC_NO"));
	strCols.Add(_T("INS_DATE"));
	strCols.Add(_T("INS_TIME"));
	nColCnt = strCols.GetSize();
	FillSpreadColumn(strCols);
	m_spdLog.SetMaxRows(nRowCnt);
	pRsw->MoveFirst();
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

void CLogEqpJobHisSkinDlg::FillSpreadColumn(CStringArray& strCols)
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
	m_spdLog.put_ColWidth(2, 45);
}
