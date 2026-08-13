// LogEqpErrHisSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "LogEqpErrHisSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"

// CLogEqpErrHisSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogEqpErrHisSkinDlg, CSkinDialog)

	CLogEqpErrHisSkinDlg::CLogEqpErrHisSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogEqpErrHisSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CLogEqpErrHisSkinDlg::CLogEqpErrHisSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogEqpErrHisSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CLogEqpErrHisSkinDlg::~CLogEqpErrHisSkinDlg()
{
	m_pDoc->m_pLogEqpSkinDlg = NULL;
	this->DestroyWindow();
}

BOOL CLogEqpErrHisSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);

}
void CLogEqpErrHisSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_EQP_HIS_SEARCH,              m_btnSearch);
	DDX_Control(pDX, IDOK2,								 m_btnOk);
	DDX_Control(pDX, IDCANCEL2,							 m_btnCancel);

	DDX_Control(pDX, IDC_LBL_WH_TYP,					 m_lblWH_TYP);
	DDX_Control(pDX, IDC_LBL_TRAY_BOTTOM,                m_lblTRAY_BOTTOM);
	DDX_Control(pDX, IDC_LBL_TRAY_TOP,					 m_lblTRAY_TOP);
	DDX_Control(pDX, IDC_LBL_LUGG_NO,					 m_lblLUGG_NO);
	DDX_Control(pDX, IDC_LBL_FROM_DATE,					 m_lblFROM_DATE);
	DDX_Control(pDX, IDC_LBL_TO_DATE,					 m_lblTO_DATE);
	DDX_Control(pDX, IDC_LBL_EQP_NO,					 m_lblEQP_NO);
	DDX_Control(pDX, IDC_LBL_EQP_TYP,					 m_lblEQP_TYP);
	DDX_Control(pDX, IDC_LBL_EQP_HIS_RESULT,             m_lblSEL_RESULT);

	DDX_Control(pDX, IDC_EDT_TRAY_BOTTOM,			     m_edtTrayBottom);
	DDX_Control(pDX, IDC_EDT_TRAY_TOP,					 m_edtTrayTop);
	DDX_Control(pDX, IDC_EDT_LUGG_NO,					 m_edtLUGG_NO);
	DDX_Control(pDX, IDC_EDT_EQP_NO,				     m_edtEQP_NO);



	DDX_Control(pDX, IDC_DTP_FROM_DATE,					 m_dtpFrom);
	DDX_Control(pDX, IDC_DTP_FROM_TIME,					 m_dtpFromTime);
	DDX_Control(pDX, IDC_DTP_TO_DATE,					 m_dtpTo);
	DDX_Control(pDX, IDC_DTP_TO_TIME,					 m_dtpToTime);

	DDX_Control(pDX, IDC_CMB_WH_TYP,					 m_cbxWH_TYP);
	DDX_Control(pDX, IDC_CMB_EQP_TYP,					 m_cbxEQP_TYP);

	DDX_Control(pDX, IDC_LBL_EQP_HIS_CNT,                m_lblSpdMainCnt);
	//DDX_Control(pDX, IDC_SPD_LOG_ERR_HIS,				 m_pSpreadMain);

	DDX_Control(pDX, IDC_CBX_ROW_CNT_EQP_HIS,			 m_cbxRowCnt);
	DDX_Control(pDX, IDC_BTN_PRE_EQP_HIS,				 m_btnPre);
	DDX_Control(pDX, IDC_BTN_NEXT_EQP_HIS,				 m_btnNext);

	DDX_Control(pDX, IDC_GRP_EQP_ERR_HIS_SEARCH,				 m_grp1);
	DDX_Control(pDX, IDC_GRP_EQP_ERR_HIS_SEARCH2,				 m_grp2);


	DDX_Control(pDX, IDC_LBL_EQP_ERR_CD, m_lblEqpErrCd);
	DDX_Control(pDX, IDC_LBL_EQP_ERR_MSG, m_lblEqpErrMsg);
	DDX_Control(pDX, IDC_EDT_EQP_ERR_CD, m_edtEqpErrCd);
	DDX_Control(pDX, IDC_EDT_EQP_ERR_MSG, m_edtEqpErrMsg);
}


BEGIN_MESSAGE_MAP(CLogEqpErrHisSkinDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_EQP_HIS_SEARCH, &CLogEqpErrHisSkinDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_PRE_EQP_HIS, &CLogEqpErrHisSkinDlg::OnBnClickedPreRow)
	ON_BN_CLICKED(IDC_BTN_NEXT_EQP_HIS, &CLogEqpErrHisSkinDlg::OnBnClickedNextRow)
	ON_CBN_SELCHANGE(IDC_CBX_ROW_CNT_EQP_HIS, &CLogEqpErrHisSkinDlg::OnSetfocusCmbRowCnt)
END_MESSAGE_MAP()
// CLogEqpErrHisSkinDlg 메시지 처리기입니다.

void CLogEqpErrHisSkinDlg::OnClose()
{
	m_pDoc->m_pLogEqpErrHis = NULL;
	delete this;
}

HCURSOR CLogEqpErrHisSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLogEqpErrHisSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

void CLogEqpErrHisSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}


void CLogEqpErrHisSkinDlg::RelocationControls()
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


BOOL CLogEqpErrHisSkinDlg::OnInitDialog()
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

	CLib::BindCombo(m_cbxWH_TYP, _T("WH_TYP"),m_pDoc, (int)pEn, FALSE);
	CLib::BindCombo(m_cbxEQP_TYP, _T("EQP_TYP"), m_pDoc ,(int)pEn, TRUE);
	CLib::BindCombo(m_cbxRowCnt, _T("ROW_CNT"), m_pDoc ,(int)pEn, FALSE);


	CTime tToDate = CTime::GetTickCount();
	CTimeSpan span(1,0,0,0); // (d,h,m,s)
	//tToDate -= span;
	int nYear = tToDate.GetYear();
	int nMonth = tToDate.GetMonth();
	int nDay = tToDate.GetDay();
	int nHour = tToDate.GetHour();
	int nMin = tToDate.GetMinute();
	int nSec = tToDate.GetSecond();

	CString strStartTime;
	strStartTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), nYear, nMonth, nDay, 00, 00, 00);
	COleDateTime tStartTime;

	tStartTime = CLib::ConvertCStringToCOleDateTime(strStartTime);

	CString strToTime;
	strToTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), nYear, nMonth, nDay, 23, 59, 59);
	COleDateTime tstrToTime;

	tstrToTime = CLib::ConvertCStringToCOleDateTime(strToTime);

	m_dtpFrom.SetTime(tStartTime);
	m_dtpFromTime.SetTime(tStartTime);

	m_dtpTo.SetTime(tstrToTime);
	m_dtpToTime.SetTime(tstrToTime);

	m_dtpFromTime.SetFormat(_T("HH:mm:ss"));
	m_dtpToTime.SetFormat(_T("HH:mm:ss"));


	InitializeResource(pEn);

	#pragma region 스프레드 초기화 
	//IDC_CUSTOM1 라는 컨트롤에서 Rect 값을 가져와서 스프레드에 적용 
	// 상대 좌표 구하기
	CRect rect;							// 윈도우 기준 절대 좌표 
	GetDlgItem(IDC_STATIC_SPREAD)->GetWindowRect(&rect);

	CRect rectTemp = rect;				// 다이얼 로그 영역안의 컨트롤(rect)의 상대좌표 
	ScreenToClient(&rectTemp);
	 
	CRect rtTemp;
	::GetWindowRect(this->m_hWnd, &rtTemp);
	
	CRect rectMargin = CRect(rectTemp.left, rectTemp.top, rtTemp.right-rect.right, rtTemp.bottom - rect.bottom);

//	m_SpreadSheet.m_rectWnd = rectTemp;//CRect(0, 0, 0, 0);
	m_SpreadSheet.m_nSorting = SS_USERCOLACTION_DEFAULT;
	m_SpreadSheet.m_bAutoResizeCol = true;
	m_SpreadSheet.m_colorBaseBack = WHITE;
	m_SpreadSheet.m_wGridType = SS_GRID_HORIZONTAL | SS_GRID_VERTICAL | SS_GRID_SOLID;

	m_SpreadSheet.m_rectMargin = rectMargin;

	HFONT hFontH = CreateFont(15,0,0,0,700,0,0,0,0,0,0,0,0,_T("System"));
	HFONT hFontB = CreateFont(15,0,0,0,FW_BOLD,0,0,0,0,0,0,0,0,_T("System"));

	m_SpreadSheet.AddSheet(_T(""), this, hFontH, hFontB);
	//m_SpreadSheet.AddColHead(_T(" "), 10);
	m_SpreadSheet.AddColHead(_T("창고 타입"), 8);
	m_SpreadSheet.AddColHead(_T("설비 종류"), 8);
	m_SpreadSheet.AddColHead(_T("설비 번호"), 8);
	m_SpreadSheet.AddColHead(_T("에러 발생 시간"), 18);
	m_SpreadSheet.AddColHead(_T("에러 코드"), 8);
	m_SpreadSheet.AddColHead(_T("에러 메시지"), 30);
	m_SpreadSheet.AddColHead(_T("작업 번호"), 8);
	m_SpreadSheet.AddColHead(_T("비고"), 10);
	m_SpreadSheet.AddColHead(_T("출발 위치"), 10);
	m_SpreadSheet.AddColHead(_T("도착 위치"), 10);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

#pragma endregion

	InitializeSpread(0, TRUE);

	return TRUE;
}

void CLogEqpErrHisSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\128x32\\");

	CString strExtension = _T(".png");
	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnOk.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("ok"), strExtension)), NULL, 5, 5);
	m_btnCancel.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cancel"), strExtension)), NULL, 5, 5);
	m_btnSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);

}

void CLogEqpErrHisSkinDlg::RenameResource( EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	SetWindowText(strValue);
		
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)enLang);
	SetDlgItemText(IDC_LBL_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqptyp"), (int)enLang);
	SetDlgItemText(IDC_LBL_EQP_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("traytop"), (int)enLang);
	SetDlgItemText(IDC_LBL_TRAY_TOP, strValue);
			
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("luggno"), (int)enLang);
	SetDlgItemText(IDC_LBL_LUGG_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpno"), (int)enLang);
	SetDlgItemText(IDC_LBL_EQP_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("traybottom"), (int)enLang);
	SetDlgItemText(IDC_LBL_TRAY_BOTTOM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromdate"), (int)enLang);
	SetDlgItemText(IDC_LBL_FROM_DATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("todate"), (int)enLang);
	SetDlgItemText(IDC_LBL_TO_DATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	SetDlgItemText(IDC_BTN_EQP_HIS_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	SetDlgItemText(IDC_LBL_EQP_HIS_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)enLang);
	SetDlgItemText(IDC_GRP_EQP_ERR_HIS_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)enLang);
	SetDlgItemText(IDC_GRP_EQP_ERR_HIS_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqperrcd"), (int)enLang);
	SetDlgItemText(IDC_LBL_EQP_ERR_CD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqphislog\\"), _T("dlg_eqphislog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqperrmsg"), (int)enLang);
	SetDlgItemText(IDC_LBL_EQP_ERR_MSG, strValue);
}

void CLogEqpErrHisSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RenameResource(nEN_LANG);
	RedrawImage();
}

void CLogEqpErrHisSkinDlg::OnBnClickedBtnSearch()
{
	if(m_pDoc == NULL) { return; };
	InitializeSpread(0, TRUE);
}


CString CLogEqpErrHisSkinDlg::GetQrySelect()
{
	if(m_pDoc == NULL){ return _T(""); };
	CString strWH_TYP = m_pDoc->m_WH_TYP;
	CString strTRAY_BOTTOM, strTRAY_TOP, strLUGG_NO, strREQUEST_ID, strEQP_NO = _T("");
	CString strdtFrom, strdtTo;
	CTime tDATE_FROM, tDATE_TO, tTIME_FROM, tTIME_TO;
	m_edtTrayBottom.GetWindowText(strTRAY_BOTTOM);
	m_edtTrayTop.GetWindowText(strTRAY_TOP);
	m_edtEQP_NO.GetWindowText(strEQP_NO);
	m_edtLUGG_NO.GetWindowText(strLUGG_NO);
	m_dtpFrom.GetTime(tDATE_FROM);
	m_dtpFromTime.GetTime(tTIME_FROM);
	m_dtpTo.GetTime(tDATE_TO);
	m_dtpToTime.GetTime(tTIME_TO);

	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tDATE_FROM, tTIME_FROM);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);

	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	strSql += CRLF + _T(" SELECT * ");
	strSql += CRLF + _T("   FROM EQP_ERR_HIS ");
	strSql += CRLF + _T("  WHERE WH_TYP =  ") + CLib::Quot(strWH_TYP);
	strSql += CRLF + _T("    AND ( ERROR_DT >= TO_DATE('") + strdtFrom + _T("', 'MM/DD/YYYY HH24:MI:SS')"); //    + CLib::Quot(_T("TO_DATE('{0}','MM/DD/YYYY HH12:MI:SS')"))
	strSql += CRLF + _T("    AND   ERROR_DT <=  TO_DATE('") + strdtTo + _T("', 'MM/DD/YYYY HH24:MI:SS') )");
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
	if(strEQP_NO != _T(""))
	{
		strSql += CRLF + _T("    AND EQP_NO =  ") + CLib::Quot(strEQP_NO);
	}
	return strSql;
}


////////SPREAD 값채우기
void CLogEqpErrHisSkinDlg::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	//정렬 가능
	//m_pSpreadMain.SetUserColAction(1);

	//m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);

	m_SpreadSheet.PrepareLoadSpread();

	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_lblSpdMainCnt.SetWindowText(CConvert::ToString(nRowCnt));
	m_SpreadSheet.FinishLoadSpread();

}

void CLogEqpErrHisSkinDlg::SetMaxRows(int pRowCnt)
{
	//m_pSpreadMain.SetMaxRows(pRowCnt);
}

void CLogEqpErrHisSkinDlg::SetColWidth(int pColCnt, int pColSize)
{
	//m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

int CLogEqpErrHisSkinDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
{
	CLib::GetColumnName(pStrArrColName, pStrSql);
	nColIdx = pStrArrColName.GetCount();
	int nStartCol = 0;
	if (pStrArrColName[0].Right(5) == _T("_HIDE"))
	{
		nStartCol = - 1;
	}

	//SetMaxCols(nColIdx + nStartCol);

	CString strTemp = _T("");
	CStringArray arrColRename;
	CLib::RenameColumn(pStrArrColName, m_pDoc->m_enLang, arrColRename);
	//int nNewColIdx = 0;
	for(int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
	{
		int nSize = arrColRename[nIdxCol].GetLength();

		strTemp = arrColRename[nIdxCol];

		if (strTemp.GetLength() > 5 && strTemp.Right(5) == _T("_HIDE"))
		{
			nSize = 0;
			if (nIdxCol == 0)
				nSize = 3;

			continue;
		}
		//++nNewColIdx;
		pStrArrColSize.Add(CConvert::ToString(nSize));
	}

	//nStartCol = 1;
	for(int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
	{
		CString asd = arrColRename[nIdxCol];
		
		strTemp = arrColRename[nIdxCol];

		if (strTemp.GetLength() > 5 && strTemp.Right(5) == _T("_HIDE"))
			continue;

		SetColumnText(nIdxCol + nStartCol, 0, arrColRename[nIdxCol]);
		
	}
	//*/

	//nColIdx = nNewColIdx;

	return 0;
}

void CLogEqpErrHisSkinDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

int CLogEqpErrHisSkinDlg::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
{
	CStringList strList;
	CStringArray arrColName;
	CStringArray arrColSize;
	int nLANG = m_pDoc->m_enLang;
	int nColSize = -1;
	int nRowCnt = -1;
	CString strMessage = _T("");

	CString strSql = GetQrySelect_Main(nRowCheck, bSearch);

	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp); 

	if(nRowCnt < 0)
	{
		if(pRsw != NULL)
		{
			delete pRsw;	
		}
	}
	else
	{	
		// arrColName과 arrColSize의 갯수가 다름!! Hide되어있는 Column을 가져오느라 어쩔수 없음~
		int nIdxStart = SetHeadColumn(arrColName, arrColSize, nColSize, strSql);

		//SetMaxRows(nRowCnt);

		pRsw->MoveFirst();

		for( int nIdxRow = 1; nIdxRow <= nRowCnt; nIdxRow++ )
		{
			m_SpreadSheet.SetCurrentRow();
			for (int nIdxCol = nIdxStart; nIdxCol < nColSize; nIdxCol++)		
			{
				CString strColValue = pRsw->GetItem(arrColName[nIdxCol]);

				strColValue += _T("    ");
				//if (strColValue.GetLength() < 6)
				//	continue;

				int nPreSize = CConvert::ToInt(arrColSize[nIdxCol]);

				if ((strColValue.GetLength()*1.5 > nPreSize) && (nPreSize != 0))
				{
					int nSize = strColValue.GetLength()*1.5;
					arrColSize[nIdxCol]= CConvert::ToString(nSize);
				}
				SetColumnText(nIdxCol, nIdxRow, strColValue);				// 번역됨
				//m_SpreadSheet.SetData(nIdxCol, nIdxRow, strColValue);		// 번역안됨

				// 짝수마다 색깔을 다르게 칠할것!
				if (nIdxRow % 2 == 0)
				{
					m_SpreadSheet.SetColor(nIdxCol+1, nIdxRow, m_pDoc->m_pConfig->m_clrSPREAD_COLOR, BLACK);
				}
				else
				{
					m_SpreadSheet.SetColor(nIdxCol+1, nIdxRow, SPREAD_COLOR_NONE, BLACK);					
				}

			}
			pRsw->MoveNext();
		}

		if (pRsw != NULL)
		{
			delete pRsw;
		}

		for (int nIdxCol = 0; nIdxCol < nColSize; nIdxCol++)
		{
			int nSize =CConvert::ToInt(arrColSize[nIdxCol]);

			//SetColWidth(nIdxCol, nSize);
			m_SpreadSheet.SetColWidth(nIdxCol + 1, nSize);
		}
	}
	return nRowCnt;
}

void CLogEqpErrHisSkinDlg::SetMaxCols(int pMaxCol)
{
	//m_pSpreadMain.SetMaxCols(pMaxCol);
}

CString CLogEqpErrHisSkinDlg::GetQrySelect_Main(int nRowCheck,BOOL bSearch)
{
	int nLANG = m_pDoc->m_enLang;
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	CString strSqlWhTypCommonCode = CLib::GetCommonCode(strSql, _T("WH_TYP"), m_pDoc, nLANG);

	CString strTRAY_BOTTOM, strTRAY_TOP, strLUGG_NO, strREQUEST_ID, strEQP_NO, strEQP_ERR_CD, strEQP_ERR_MSG = _T("");
	CString strdtFrom, strdtTo;
	CTime tDATE_FROM, tDATE_TO, tTIME_FROM, tTIME_TO;
	CString strWH_TYP = m_cbxWH_TYP.GetItemKey(m_cbxWH_TYP.GetCurSel());
	CString strEQP_TYP = m_cbxEQP_TYP.GetItemKey(m_cbxEQP_TYP.GetCurSel());

	m_edtTrayBottom.GetWindowText(strTRAY_BOTTOM);
	m_edtTrayTop.GetWindowText(strTRAY_TOP);
	m_edtEQP_NO.GetWindowText(strEQP_NO);
	m_edtLUGG_NO.GetWindowText(strLUGG_NO);
	m_dtpFrom.GetTime(tDATE_FROM);
	m_dtpFromTime.GetTime(tTIME_FROM);
	m_dtpTo.GetTime(tDATE_TO);
	m_dtpToTime.GetTime(tTIME_TO);
	m_edtEqpErrCd.GetWindowText(strEQP_ERR_CD);
	m_edtEqpErrMsg.GetWindowText(strEQP_ERR_MSG);

	strTRAY_BOTTOM.Trim();
	strTRAY_TOP.Trim();
	strEQP_NO.Trim();
	strLUGG_NO.Trim();
	strEQP_ERR_CD.Trim();
	strEQP_ERR_MSG.Trim();

	CString strRowCnt;
	int nRowCnt;	
	int nRowSum;
	strRowCnt = m_cbxRowCnt.GetItemKey(m_cbxRowCnt.GetCurSel()); 
	nRowCnt = CConvert::ToInt(strRowCnt);
	if (nRowCheck == 0) { nRowSum = nRowCnt; m_pPreRowCnt = 0; }
	if (nRowCheck < 0) { nRowSum = m_pPreRowCnt - nRowCnt; }
	if (nRowCheck > 0) { nRowSum = m_pPreRowCnt + nRowCnt; }
	if (nRowSum == 0) { nRowSum = nRowCnt; m_pPreRowCnt = 0; }

	CString strPreRowCnt = CConvert::ToString(nRowSum - nRowCnt + 1);
	CString strNextRowCnt = CConvert::ToString(nRowSum);
	m_pPreRowCnt = nRowSum;

	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tDATE_FROM, tTIME_FROM);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);

	
	strSql = CRLF +  _T("SELECT ") + m_pDoc->NVL + _T("(CD_WH_TYP.CCD_NM_KOR, '10') AS WH_TYP							    ");
	strSql += CRLF + _T("      ,EEH.EQP_TYP AS EQP_TYP																	    ");
	strSql += CRLF + _T("      ,EEH.EQP_NO AS EQP_NO																	    ");
	strSql += CRLF + _T("      ,EEH.ERROR_DT AS ERROR_DT																    ");
	strSql += CRLF + _T("      ,EEH.EQP_ERR_CD AS EQP_ERR_CD															    ");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(EEM.MSG_KOR, EEH.EQP_ERR_CD) AS EQP_ERR_MSG						    ");
	//strSql += CRLF + _T("      ,EEH.BCR_BOTTOM AS BCR_BOTTOM															    ");
	//strSql += CRLF + _T("      ,EEH.BCR_TOP AS BCR_TOP																    ");
	strSql += CRLF + _T("      ,EEH.LUGG_NO AS LUGG_NO																	    ");
	strSql += CRLF + _T("      ,EEH.REMARK AS REMARK																	    ");
	strSql += CRLF + _T("      ,CASE WHEN JMH.START_LOCATION = '00-000-00' THEN JM.START_LOCATION ELSE JMH.START_LOCATION END AS START_LOCATION  ");
	strSql += CRLF + _T("      ,CASE WHEN JMH.DEST_LOCATION = '00-000-00' THEN JM.DEST_LOCATION ELSE JMH.DEST_LOCATION END AS DEST_LOCATION ");
	//strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(JM.START_LOCATION, '') AS START_LOCATION						    ");
	//strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(JM.DEST_LOCATION, '') AS DEST_LOCATION						        ");
	strSql += CRLF + _T("  FROM EQP_ERR_HIS EEH LEFT OUTER JOIN EQP_ECD_MST EEM												");
	strSql += CRLF + _T("                                    ON EEH.EQP_TYP = EEM.EQP_TYP									");
	strSql += CRLF + _T("                                   AND EEH.EQP_ERR_CD = EEM.EQP_ERR_CD								");
	strSql += CRLF + _T("                       LEFT OUTER JOIN COMMON_CODE CD_WH_TYP										");
	strSql += CRLF + _T("                                    ON CD_WH_TYP.CDX_CD = 'WH_TYP'									");
	strSql += CRLF + _T("                                   AND CD_WH_TYP.CCD_CD = EEH.WH_TYP								");
	strSql += CRLF + _T("                       LEFT OUTER JOIN JOB_MST JM													");
	strSql += CRLF + _T("                                    ON EEH.LUGG_NO = JM.LUGG_NO									");
	strSql += CRLF + _T("                                   AND JM.JOB_TYP IN ('1', '2')									");
	strSql += CRLF + _T("                       LEFT OUTER JOIN JOB_MST_HIS JMH												");
	strSql += CRLF + _T("                                    ON EEH.LUGG_NO = JMH.LUGG_NO									");
	strSql += CRLF + _T("                                   AND JMH.JOB_TYP IN ('1', '2')									");
	strSql += CRLF + _T(" WHERE EEH.WH_TYP = ") + CLib::Quot(strWH_TYP);
	strSql += CRLF + _T("   AND TO_CHAR(EEH.ERROR_DT , 'YYYYMMDDHH24MISS') BETWEEN '") + strdtFrom + _T("' AND '") + strdtTo + _T("'");

	if(strEQP_TYP != _T("ALL"))
	{
		strSql += CRLF + _T("    AND EEH.EQP_TYP = ") + CLib::Quot(strEQP_TYP);
	}
	//if(strTRAY_BOTTOM != _T(""))
	//{
	//	strSql += CRLF + _T("    AND EEH.BCR_BOTTOM =  ") + CLib::Quot(strTRAY_BOTTOM);
	//}
	//if(strTRAY_TOP != _T(""))
	//{
	//	strSql += CRLF + _T("    AND EEH.BCR_TOP =  ") + CLib::Quot(strTRAY_TOP);
	//}
	if(strLUGG_NO != _T(""))
	{
		strSql += CRLF + _T("    AND EEH.LUGG_NO LIKE  '%") + strLUGG_NO + _T("%'");
	}
	if(strEQP_NO != _T(""))
	{
		strSql += CRLF + _T("    AND EEH.EQP_NO =  ") + CLib::Quot(strEQP_NO);
	}
	if(strEQP_ERR_CD != _T(""))
	{
		strSql += CRLF + _T("    AND EEH.EQP_ERR_CD LIKE '%") + strEQP_ERR_CD + _T("%'");
	}
	if(strEQP_ERR_MSG != _T(""))
	{
		strSql += CRLF + _T("    AND EEM.MSG_KOR LIKE '%") + strEQP_ERR_MSG + _T("%'");
	}
#if ORACLE
	strSql += CRLF + _T(" AND ROWNUM <=	") + strNextRowCnt;
#endif

	strSql += CRLF + _T(" ORDER BY EEH.ERROR_DT DESC ");

#if POSTGRESQL
	if (strRowCnt != _T("ALL"))
		strSql += CRLF + _T(" LIMIT	") + strNextRowCnt;
#endif

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CLogEqpErrHisSkinDlg::OnBnClickedPreRow()
{
	InitializeSpread(-1, TRUE);
}

void CLogEqpErrHisSkinDlg::OnBnClickedNextRow() 
{
	InitializeSpread(1, TRUE);
}

void CLogEqpErrHisSkinDlg::OnSetfocusCmbRowCnt()
{
	m_pPreRowCnt = 0;
}