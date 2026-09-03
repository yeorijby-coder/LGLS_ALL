// LogIoSkinDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Ecs.h"
#include "LogMesSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CLogMesSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogMesSkinDlg, CSkinDialog)

	CLogMesSkinDlg::CLogMesSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogMesSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CLogMesSkinDlg::CLogMesSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogMesSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CLogMesSkinDlg::~CLogMesSkinDlg()
{
	m_pDoc->m_pLogMesSkinDlg = NULL;
	this->DestroyWindow();
}

BOOL CLogMesSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);

}
void CLogMesSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_SPD_LOG_MES,					 m_pSpreadMain);
	DDX_Control(pDX, IDC_LBL_MES_MST_CNT,				 m_lblSpdMainCnt);


	DDX_Control(pDX, IDC_BTN_LOG_IO_SEARCH,              m_btnSearch);
	DDX_Control(pDX, IDOK,								 m_btnOk);
	DDX_Control(pDX, IDCANCEL,							 m_btnCancel);

	//	DDX_Control(pDX, IDC_LBL_WH_TYP,					 m_lblWhTyp);
	DDX_Control(pDX, IDC_LBL_MES_TO_DATE,				 m_lblToDate);
	DDX_Control(pDX, IDC_LBL_MES_FROM_DATE,				 m_lblFromDate);

	DDX_Control(pDX, IDC_DTP_TO_DATE,					 m_dtpTo);
	DDX_Control(pDX, IDC_DTP_TO_TIME,					 m_dtpToTime);
	DDX_Control(pDX, IDC_DTP_FROM_DATE,					 m_dtpFrom);
	DDX_Control(pDX, IDC_DTP_FROM_TIME,					 m_dtpFromTime);


	DDX_Control(pDX, IDC_LBL_MES_MST_RESULT,			 m_lblSpdMainResult);

	DDX_Control(pDX, IDC_LBL_MES_MESSAGE3,               m_lblMESMessage3);
	DDX_Control(pDX, IDC_EDT_MES_MESSAGE3,			     m_edtMESMessage3);

	DDX_Control(pDX, IDC_EDIT_MES_LOG,			     m_edtMESDetailLog);

	DDX_Control(pDX, IDC_GRP_MES_SEARCH,		 m_grpMesLogSearch);
	DDX_Control(pDX, IDC_GRP_MES_SEARCH2,		 m_grpMesLogSearch2);
	DDX_Control(pDX, IDC_STATIC_MES_LOG,		 m_grpMesLogMessage);

	DDX_Control(pDX, IDC_LBL_MES_LOG_WH_TYP,		m_lblMESLogWhTyp);
	DDX_Control(pDX, IDC_CBX_MES_LOG_WH_TYP,		m_cbxMESLogWhTyp);

	DDX_Control(pDX, IDC_CBX_ROW_CNT,		m_cbxRowCnt);
	DDX_Control(pDX, IDC_BTN_PRE,		m_btnPre);
	DDX_Control(pDX, IDC_BTN_NEXT,		m_btnNext);

	DDX_Control(pDX, IDC_STATIC_MES_LOG2,		 m_grpMesLogMessage2);
	DDX_Control(pDX, IDC_EDIT_MES_LOG2,			 m_edtMESDetailLog2);

	DDX_Control(pDX, IDC_CBX_MES_LOG_DIRECTION, m_cbxMesLogDirection);
	DDX_Control(pDX, IDC_CBX_MES_LOG_HOST_CMD, m_cbxMesLogHostCmd);
	DDX_Control(pDX, IDC_LBL_MES_LOG_DIRECTION, m_lblMesLogDirection);
	DDX_Control(pDX, IDC_LBL_MES_LOG_HOST_CMD, m_lblMesLogHostCmd);
	DDX_Control(pDX, IDC_LBL_MES_JOB_DEFINE, m_lblJobDefine);
	DDX_Control(pDX, IDC_CBX_MES_JOB_DEFINE, m_cbxJobDefine);
	DDX_Control(pDX, IDC_LBL_MES_LUGG_NO, m_lblHostLuggNo);
	DDX_Control(pDX, IDC_EDT_MES_LUGG_NO, m_edtHostLuggNo);
	DDX_Control(pDX, IDC_LBL_MES_STN_NO, m_lblStnNo);
	DDX_Control(pDX, IDC_EDT_MES_STN_NO, m_edtStnNo);
}


BEGIN_MESSAGE_MAP(CLogMesSkinDlg, CSkinDialog)
	ON_MESSAGE(SSM_CLICK, &CLogMesSkinDlg::OnLglsHdrSort)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SPD_LOG_IO, &CLogMesSkinDlg::OnBnClickedSpread)
	ON_BN_CLICKED(IDC_BTN_LOG_IO_SEARCH, &CLogMesSkinDlg::OnBnClickedBtnLogMesSearch)
	ON_BN_CLICKED(IDC_BTN_PRE, &CLogMesSkinDlg::OnBnClickedPreRow)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CLogMesSkinDlg::OnBnClickedNextRow)
	ON_COMMAND(IDC_SPD_LOG_IO, &CLogMesSkinDlg::OnSpdLogIo)
	ON_UPDATE_COMMAND_UI(IDC_SPD_LOG_IO, &CLogMesSkinDlg::OnUpdateSpdLogIo)
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_CBX_ROW_CNT, &CLogMesSkinDlg::OnSetfocusCmbRowCnt)
END_MESSAGE_MAP()

// [LGLS] 그리드 헤더 클릭 정렬
LRESULT CLogMesSkinDlg::OnLglsHdrSort(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if(pCell != NULL && pCell->Row == 0 && pCell->Col >= 1)
		m_SpreadSheet.SortByHeaderColumn(pCell->Col);
	return 0;
}


BEGIN_EVENTSINK_MAP(CLogMesSkinDlg, CSkinDialog)
	ON_EVENT(CLogMesSkinDlg, IDC_SPD_LOG_MES, 5, CLogMesSkinDlg::ClickSpread, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()
// CLogMesSkinDlg 메시지 처리기입니다.

void CLogMesSkinDlg::OnSetfocusCmbRowCnt()
{
	m_pPreRowCnt = 0;
}

void CLogMesSkinDlg::OnClose()
{
	m_pDoc->m_pLogMesSkinDlg = NULL;
	delete this;
}

HCURSOR CLogMesSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLogMesSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

void CLogMesSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}


void CLogMesSkinDlg::RelocationControls()
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


BOOL CLogMesSkinDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	CTime tStartD, tStartT, tEndD, tEndT;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{	
		RelocationControls();
		m_bInitialized = TRUE;	
	}

	CLib::BindCombo(m_cbxMESLogWhTyp, _T("WH_TYP"),m_pDoc, (int)pEn, FALSE);
	CLib::BindCombo(m_cbxMesLogDirection, _T("DIRECTION"),m_pDoc, (int)pEn, TRUE);
	CLib::BindCombo(m_cbxMesLogHostCmd, _T("HOST_CMD"),m_pDoc, (int)pEn, TRUE);
	// [LGLS 2026-08-23] 작업구분 : 명세서 Job Define (1 입고 / 2 출고)
	{
		int nIdx;
		m_cbxJobDefine.ResetContent();
		nIdx = m_cbxJobDefine.AddString(_T("ALL"));   m_cbxJobDefine.SetItemDataEx(nIdx, _T("ALL"));
		nIdx = m_cbxJobDefine.AddString(_T("1:입고")); m_cbxJobDefine.SetItemDataEx(nIdx, _T("1"));
		nIdx = m_cbxJobDefine.AddString(_T("2:출고")); m_cbxJobDefine.SetItemDataEx(nIdx, _T("2"));
		m_cbxJobDefine.SetCurSel(0);
	}
	CLib::BindCombo(m_cbxRowCnt, _T("ROW_CNT"), m_pDoc ,(int)pEn, FALSE);
	
	InitializeResource(pEn);
	m_pPreRowCnt = 0;

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
	m_SpreadSheet.AddColHead(_T("창고 타입"), 10);
	m_SpreadSheet.AddColHead(_T("보고유형"), 10);
	m_SpreadSheet.AddColHead(_T("송수신자"), 10);
	m_SpreadSheet.AddColHead(_T("메세지"), 25);
	m_SpreadSheet.AddColHead(_T("추가 시간"), 19);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

#pragma endregion

	InitializeSpread(0, TRUE);

	{ CString _tt; GetWindowText(_tt); if(_tt.Find(_T("[")) < 0) SetWindowText(_tt + _T(" [MES_IF_LOG]")); }	// [LGLS] 제목에 조회 테이블명
	return TRUE;
}

void CLogMesSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\128x32\\");

	CString strExtension = _T(".png");
	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnOk.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("ok"), strExtension)), NULL, 5, 5);
	m_btnCancel.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cancel"), strExtension)), NULL, 5, 5);
	m_btnSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);

}

void CLogMesSkinDlg::RenameResource(EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_LOG_IO_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ok"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDOK, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cancel"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDCANCEL, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MES_LOG_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("direction"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MES_LOG_DIRECTION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("hostcmd"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MES_LOG_HOST_CMD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("message"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MES_MESSAGE3, strValue);	

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("result"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MES_MST_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("todate"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MES_TO_DATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromdate"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MES_FROM_DATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_MES_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_MES_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("detail"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_STATIC_MES_LOG, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_meslog\\"), _T("dlg_meslog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("mrcdmsg"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_STATIC_MES_LOG2, strValue);


}

void CLogMesSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RedrawImage();
	RenameResource(nEN_LANG);
}


CString CLogMesSkinDlg::GetQrySelect(int nLANG, CString strWH_TYP, CString strLUGG_NO, CString strMesMessage, CString strMesMessage2, CString strMesMessage3, CString strdtTo)
{
	int nRowCnt = 0;
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	CString strSqlWhTypCommonCode = CLib::GetCommonCode(strSql, _T("WH_TYP"), m_pDoc, nLANG);
	CString strSrTypCommonCode = CLib::GetCommonCode(strSql, _T("SR_TYPE"), m_pDoc, nLANG);
	nRowCnt = m_cbxRowCnt.GetItemData(m_cbxRowCnt.GetCurSel()); 
	CString strRowCnt = CConvert::ToString(nRowCnt);

	
	strSql += CRLF + _T("SELECT MES.WH_TYP					");
	strSql += CRLF + _T(",	   COM1.CCD_NM_KOR AS WH_TYP_NM");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.LOG_DATE,		'0') as LOG_DATE					 ");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.LOG_TIME,		'0') as LOG_TIME					 ");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(CCD_MES_CMD.CCD_NM_KOR,		    '0') as MES_CMD						 ");
	strSql += CRLF + _T(",	   COM2.CCD_NM_KOR AS SR_TYPE_NM ");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.MESSAGE,			'0') as MESSAGE					");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.LUGG_NO,			'0') as LUGG_NO					");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.BCR_BOTTOM,		'0') as BCR_BOTTOM				");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.BCR_TOP,			'0') as BCR_TOP					");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.REMARKS,			'0') as REMARKS					");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.INS_DT,			") + m_pDoc->SYSDATE + _T(") as INS_DT				");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.INS_USER_ID,		'0') as INS_USER_ID				");
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.UPD_DT,			") + m_pDoc->SYSDATE + _T(") as UPD_DT				");	
	strSql += CRLF + _T(",     ") + m_pDoc->NVL + _T("(MES.UPD_USER_ID,		'0') as UPD_USER_ID				");
	strSql += CRLF + _T(" FROM MES_IF_LOG MES									");
	strSql += CRLF + _T("	   LEFT OUTER JOIN (") + strSqlWhTypCommonCode + _T(") COM1 ");
	strSql += CRLF + _T("		            ON MES.WH_TYP = COM1.CCD_CD 			    ");
	strSql += CRLF + _T("	   LEFT OUTER JOIN (") + strSrTypCommonCode + _T(") COM2    ");
	strSql += CRLF + _T("		            ON MES.SR_TYPE = COM2.CCD_CD 			    ");	
	strSql += CRLF + _T("      LEFT OUTER JOIN COMMON_CODE CCD_MES_CMD ");
	strSql += CRLF + _T("              ON CCD_MES_CMD.CDX_CD = 'MES_CMD' ");
	strSql += CRLF + _T("             AND CCD_MES_CMD.CCD_CD = MES.MES_CMD ");
	strSql += CRLF + _T("             AND CCD_MES_CMD.WH_TYP LIKE ") + CLib::QuotLikeLR(strSqlWhTypCommonCode);										
	strSql += CRLF + _T("WHERE (MES.INS_DT < CONVERT(DATETIME, STUFF(STUFF(STUFF('") + strdtTo + _T("',13,0,':'),11,0,':'),9,0,' ')) AND MES.INS_DT > DATEADD(DAY, -1, CONVERT(DATETIME, STUFF(STUFF(STUFF('") + strdtTo + _T("',13,0,':'),11,0,':'),9,0,' '))))");	// [LGLS] Oracle date-diff -> DATEADD window
	strSql += CRLF + _T("	   AND WH_TYP =  ") + CLib::Quot(strWH_TYP);
	strSql += CRLF + _T("      AND (MES.MESSAGE LIKE '%") + strMesMessage + _T("%' AND MES.MESSAGE LIKE '%") + strMesMessage2 + _T("%' AND MES.MESSAGE LIKE '%") + strMesMessage3 + _T("%')");
#if ORACLE
	strSql += CRLF + _T(" AND ROWNUM <=	") + strRowCnt;
#endif

	strSql += CRLF + _T("ORDER BY MES.INS_DT DESC	   							    ");

#if POSTGRESQL
	if (strRowCnt != _T("ALL"))
		strSql += CRLF + _T(" LIMIT	") + strRowCnt;
#elif MSSQL
	if (strRowCnt != _T("ALL"))	// [LGLS] SQL2008: inject TOP n instead of LIMIT
		CLib::ApplyTopN(strSql, strRowCnt);	// [LGLS]
#endif


	return strSql;
}

void CLogMesSkinDlg::OnBnClickedBtnLogMesSearch()
{
	UpdateData(TRUE);

	if(m_pDoc == NULL) { return; };
	InitializeSpread(0, TRUE);

	//int nLANG  = m_pDoc->m_enLang;
	//CString strMesMessage, strMesMessage2, strMesMessage3 = _T("");
	//CString strLUGG_NO = _T("");
	//CString strdtFrom, strdtTo;	
	//CTime tDATE_TO, tTIME_TO;

	//m_dtpTo.GetTime(tDATE_TO);
	//m_dtpToTime.GetTime(tTIME_TO);
	//strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);
	//int nWH_TYP = m_cbxMESLogWhTyp.GetItemData(m_cbxMESLogWhTyp.GetCurSel());
	//CString strWH_TYP = CConvert::ToString(nWH_TYP);

	//m_edtLuggNo.GetWindowText(strLUGG_NO);
	//m_edtMESMessage.GetWindowText(strMesMessage);
	//m_edtMESMessage2.GetWindowText(strMesMessage2);
	//m_edtMESMessage3.GetWindowText(strMesMessage3);
	//m_dtpTo.GetTime(tDATE_TO);
	//m_dtpToTime.GetTime(tTIME_TO);

	//CString strSql = _T("");	
	//m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);


	//strSql = GetQrySelect(nLANG, strWH_TYP, strLUGG_NO, strMesMessage, strMesMessage2, strMesMessage3, strdtTo);

	//CStringArray arrColName;

	////한글
	//if(nLANG == 0)
	//{
	//	arrColName.Add(_T("창고구분(H)"));
	//	arrColName.Add(_T("창고구분"));
	//	arrColName.Add(_T("날짜"));
	//	arrColName.Add(_T("시간"));
	//	arrColName.Add(_T("MES_CMD"));
	//	arrColName.Add(_T("송수신상태"));
	//	arrColName.Add(_T("로그내용"));
	//	arrColName.Add(_T("작업번호"));
	//	arrColName.Add(_T("바코드하단"));
	//	arrColName.Add(_T("바코드상단"));
	//	arrColName.Add(_T("REMARKS"));
	//	arrColName.Add(_T("입력일시"));
	//	arrColName.Add(_T("입력자"));
	//	arrColName.Add(_T("수정일시"));
	//	arrColName.Add(_T("수정자"));	
	//}
	////영어
	//else if(nLANG == 1)
	//{
	//	arrColName.Add(_T("WH_TYP"));
	//	arrColName.Add(_T("WH_TYP_NM"));
	//	arrColName.Add(_T("LOG_DATE"));
	//	arrColName.Add(_T("LOG_TIME"));
	//	arrColName.Add(_T("MES_CMD"));
	//	arrColName.Add(_T("SR_TYPE_NM"));
	//	arrColName.Add(_T("MESSAGE"));
	//	arrColName.Add(_T("LUGG_NO"));
	//	arrColName.Add(_T("BCR_BOTTOM"));
	//	arrColName.Add(_T("BCR_TOP"));
	//	arrColName.Add(_T("REMARKS"));
	//	arrColName.Add(_T("INS_DT"));
	//	arrColName.Add(_T("INS_USER_ID"));
	//	arrColName.Add(_T("UPD_DT"));
	//	arrColName.Add(_T("UPD_USER_ID"));	
	//}
	////헝가리
	//else if(nLANG == 2)
	//{
	//	arrColName.Add(_T("WH_TYP"));
	//	arrColName.Add(_T("WH_TYP_NM"));
	//	arrColName.Add(_T("LOG_DATE"));
	//	arrColName.Add(_T("LOG_TIME"));
	//	arrColName.Add(_T("MES_CMD"));
	//	arrColName.Add(_T("SR_TYPE_NM"));
	//	arrColName.Add(_T("MESSAGE"));
	//	arrColName.Add(_T("LUGG_NO"));
	//	arrColName.Add(_T("BCR_BOTTOM"));
	//	arrColName.Add(_T("BCR_TOP"));
	//	arrColName.Add(_T("REMARKS"));
	//	arrColName.Add(_T("INS_DT"));
	//	arrColName.Add(_T("INS_USER_ID"));
	//	arrColName.Add(_T("UPD_DT"));
	//	arrColName.Add(_T("UPD_USER_ID"));	
	//}
	////중국어
	//else if(nLANG == 3)
	//{
	//	arrColName.Add(_T("WH_TYP"));
	//	arrColName.Add(_T("WH_TYP_NM"));
	//	arrColName.Add(_T("LOG_DATE"));
	//	arrColName.Add(_T("LOG_TIME"));
	//	arrColName.Add(_T("MES_CMD"));
	//	arrColName.Add(_T("SR_TYPE_NM"));
	//	arrColName.Add(_T("MESSAGE"));
	//	arrColName.Add(_T("LUGG_NO"));
	//	arrColName.Add(_T("BCR_BOTTOM"));
	//	arrColName.Add(_T("BCR_TOP"));
	//	arrColName.Add(_T("REMARKS"));
	//	arrColName.Add(_T("INS_DT"));
	//	arrColName.Add(_T("INS_USER_ID"));
	//	arrColName.Add(_T("UPD_DT"));
	//	arrColName.Add(_T("UPD_USER_ID"));	
	//}


	//int nColSize = arrColName.GetCount();
	//m_pSpreadMain.SetMaxCols(nColSize);
	//for(int nIdxCol = 0; nIdxCol < nColSize; nIdxCol++)
	//{
	//	FillSpreadColumn(nIdxCol, arrColName[nIdxCol]);
	//}

	//CLib::GetEng(arrColName[0],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[1],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[2],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[3],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[4],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[5],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[6],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[7],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[8],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[9],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[10],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[11],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[12],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[13],m_pDoc, nLANG);
	//CLib::GetEng(arrColName[14],m_pDoc, nLANG);
	////CLib::GetEng(arrColName[15],m_pDoc, nLANG);


	//int nRowCnt = -1;
	//CString strMessage = _T("");
	//_RecordsetPtr pRspEqpData = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMesMessage);
	//CRecordSetWrap* pRswEqpDataWrap = new CRecordSetWrap(pRspEqpData); 

	//m_pSpreadMain.SetMaxRows(nRowCnt);
	//if(nRowCnt < 1)
	//{
	//	if(pRswEqpDataWrap != NULL)
	//	{
	//		nRowCnt = 0;
	//		m_lblMESMstCnt.SetWindowText(CConvert::ToString(nRowCnt));
	//		delete pRswEqpDataWrap;	
	//	}
	//	return;
	//}
	//pRswEqpDataWrap->MoveFirst();


	//for( int nIdxScData = 0; nIdxScData < nRowCnt; nIdxScData++ )
	//{

	//	FillSpreadRow(0, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[0]));
	//	FillSpreadRow(1, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[1]));
	//	FillSpreadRow(2, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[2]));
	//	FillSpreadRow(3, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[3]));
	//	FillSpreadRow(4, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[4]));
	//	FillSpreadRow(5, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[5]));
	//	FillSpreadRow(6, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[6]));
	//	FillSpreadRow(7, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[7]));
	//	FillSpreadRow(8, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[8]));
	//	FillSpreadRow(9, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[9]));
	//	FillSpreadRow(10, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[10]));
	//	FillSpreadRow(11, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[11]));
	//	FillSpreadRow(12, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[12]));
	//	FillSpreadRow(13, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[13]));
	//	FillSpreadRow(14, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[14]));
	//	//FillSpreadRow(15, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[15]));
	//	//FillSpreadRow(16, nIdxScData, pRswEqpDataWrap->GetItem(arrColName[16]));
	//	pRswEqpDataWrap->MoveNext();
	//}

	//if (pRswEqpDataWrap != NULL)
	//{
	//	delete pRswEqpDataWrap;
	//}
	//m_lblMESMstCnt.SetWindowText(CConvert::ToString(nRowCnt));



}
void CLogMesSkinDlg::FillSpreadColumn(int nColIdx, CString strColumnName)
{
	//CString bbb = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	//CString aaa = _T("");
	//aaa.Format(_T("%s\r"), bbb);
	//m_pSpreadMain.SetText(nColIdx + 1, 0, variant_t(aaa));
	//m_pSpreadMain.put_ColWidth(7, 60);
}



void CLogMesSkinDlg::FillSpreadRow(int nColIdx, int nRowIdx, CString strValue)
{
	//CString aaa = _T("");
	//aaa.Format(_T("%s\r"), strValue);
	//m_pSpreadMain.SetText(nColIdx + 1, nRowIdx + 1, variant_t(aaa));
}


void CLogMesSkinDlg::OnSpdLogIo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int a = 0;
}


void CLogMesSkinDlg::OnUpdateSpdLogIo(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
		int a = 0;
}

void CLogMesSkinDlg::OnBnClickedSpread()
{
	int a = 0;
	int bb = 0;
}


void CLogMesSkinDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
}




////////SPREAD 값채우기
void CLogMesSkinDlg::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	//정렬 가능
	//m_pSpreadMain.SetUserColAction(1);

	//m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);

	m_SpreadSheet.PrepareLoadSpread();
	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_lblSpdMainCnt.SetWindowText(CConvert::ToString(nRowCnt));
	m_SpreadSheet.FinishLoadSpread();

}

void CLogMesSkinDlg::SetMaxRows(int pRowCnt)
{
	//m_pSpreadMain.SetMaxRows(pRowCnt);
}

void CLogMesSkinDlg::SetColWidth(int pColCnt, int pColSize)
{
	//m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

int CLogMesSkinDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
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

void CLogMesSkinDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

int CLogMesSkinDlg::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
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

				// [LGLS 2026-09-03] 표시값에 공백 4칸을 붙이던 것 제거 - 가운데 정렬이 왼쪽으로 밀려 보였다(폭 계산에만 반영)
				int nPadLen = strColValue.GetLength() + 4;
				//if (strColValue.GetLength() < 6)
				//	continue;

				int nPreSize = CConvert::ToInt(arrColSize[nIdxCol]);

				if ((nPadLen*1.5 > nPreSize) && (nPreSize != 0))
				{
					int nSize = (int)(nPadLen*1.5);
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

void CLogMesSkinDlg::SetMaxCols(int pMaxCol)
{
	//m_pSpreadMain.SetMaxCols(pMaxCol);
}

CString CLogMesSkinDlg::GetQrySelect_Main(int nRowCheck, BOOL bSearch)
{
	int nLANG = m_pDoc->m_enLang;
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	CTime tToDate, tToTime, tFromDate, tFromTime;
	CString strdtFrom, strdtTo;
	CString strWH_TYP = CLib::GetComboKey(m_cbxMESLogWhTyp);
	CString strDirection = CLib::GetComboKey(m_cbxMesLogDirection);
	CString strHostCmd = CLib::GetComboKey(m_cbxMesLogHostCmd);
	CString strMesMessage3;
	//m_edtMESMessage.GetWindowText(strMesMessage1);
	//m_edtMESMessage2.GetWindowText(strMesMessage2);
	m_edtMESMessage3.GetWindowText(strMesMessage3);

	// [LGLS 2026-08-23] 추가 조회 조건 : 작업구분 / 작업번호 / 스테이션 번호
	CString strJobDefine, strLuggNo, strStnNo;
	int nJd = m_cbxJobDefine.GetCurSel();
	if (nJd >= 0) strJobDefine = m_cbxJobDefine.GetItemKey(nJd);
	m_edtHostLuggNo.GetWindowText(strLuggNo);
	m_edtStnNo.GetWindowText(strStnNo);
	strLuggNo.Trim();
	strStnNo.Trim();

	m_dtpTo.GetTime(tToDate);
	m_dtpToTime.GetTime(tToTime);
	m_dtpFrom.GetTime(tFromDate);
	m_dtpFromTime.GetTime(tFromTime);

	CString strRowCnt;
	int nRowCnt;	
	int nRowSum;
	strRowCnt = CLib::GetComboKey(m_cbxRowCnt); 
	nRowCnt = CConvert::ToInt(strRowCnt);
	if (nRowCheck == 0) { nRowSum = nRowCnt; m_pPreRowCnt = 0; }
	if (nRowCheck < 0) { nRowSum = m_pPreRowCnt - nRowCnt; }
	if (nRowCheck > 0) { nRowSum = m_pPreRowCnt + nRowCnt; }
	if (nRowSum == 0) { nRowSum = nRowCnt; m_pPreRowCnt = 0; }

	CString strPreRowCnt = CConvert::ToString(nRowSum - nRowCnt + 1);
	CString strNextRowCnt = CConvert::ToString(nRowSum);
	m_pPreRowCnt = nRowSum;

	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tFromDate, tFromTime);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tToDate, tToTime);

	
	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, HIL.WH_TYP) AS WH_TYP			");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_HOST_CMD.CCD_NM_KOR, HIL.HOST_CMD) AS HOST_CMD		");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_DIRECTION.CCD_NM_KOR, HIL.DIRECTION) AS DIRECTION	");
	strSql += CRLF + _T("      ,HIL.MESSAGE AS MESSAGE															");
	strSql += CRLF + _T("      ,HIL.INS_DT AS INS_DT															");
	strSql += CRLF + _T("  FROM HOST_IF_LOG HIL																	");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP											");
	strSql += CRLF + _T("                    ON HIL.WH_TYP = CCD_WH_TYP.CCD_CD									");
	strSql += CRLF + _T("                   AND CCD_WH_TYP.CDX_CD = 'WH_TYP'									");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_DIRECTION										");
	strSql += CRLF + _T("                    ON HIL.DIRECTION = CCD_DIRECTION.CCD_CD							");
	strSql += CRLF + _T("                   AND CCD_DIRECTION.CDX_CD = 'DIRECTION'								");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_HOST_CMD										");
	strSql += CRLF + _T("       	     	 ON HIL.HOST_CMD = CCD_HOST_CMD.CCD_CD								");
	strSql += CRLF + _T("       	        AND CCD_HOST_CMD.CDX_CD = 'HOST_CMD'								");
	strSql += CRLF + _T(" WHERE (CONVERT(VARCHAR(8),HIL.INS_DT,112) + REPLACE(CONVERT(VARCHAR(8),HIL.INS_DT,108),':','')) BETWEEN '") + strdtFrom + _T("' AND '") + strdtTo + _T("'");	// [LGLS]
	if(strWH_TYP != _T("") && strWH_TYP != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HIL.WH_TYP =  ") + CLib::Quot(strWH_TYP);
	}

	if(strHostCmd != _T("") && strHostCmd != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HIL.HOST_CMD =  ") + CLib::Quot(strHostCmd);
	}

	if(strDirection != _T("") && strDirection != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HIL.DIRECTION =  ") + CLib::Quot(strDirection);
	}

	if(strMesMessage3 != _T("") && strMesMessage3 != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HIL.MESSAGE LIKE '%") + strMesMessage3 + _T("%'");
	}

	// [LGLS 2026-08-23] 전문 본문에서 뽑아 거른다(WMS-ECS 인터페이스명세서 2010.03.11 기준).
	//   저장된 MESSAGE 는 헤더 15자 + STX 1자 뒤에 본문이 오므로 SUBSTRING 위치 = 명세서 offset + 1.
	//   작업구분  : offset 17 → 18  (작업지시 O / 재작업 R / 완료보고 F)
	//   작업번호  : offset 18 → 19  (응답 전문은 offset 20 → 21 에 있어 함께 본다)
	//   스테이션  : 출발 offset 33 → 34, 도착 offset 46 → 47, C/V 모드(M) offset 17 → 18
	if(strJobDefine != _T("") && strJobDefine != _T("ALL"))
	{
		strSql += CRLF + _T("    AND SUBSTRING(HIL.MESSAGE,18,1) = ") + CLib::Quot(strJobDefine);
	}

	if(strLuggNo != _T(""))
	{
		strSql += CRLF + _T("    AND ( SUBSTRING(HIL.MESSAGE,19,4) = ") + CLib::Quot(strLuggNo);
		strSql += CRLF + _T("       OR SUBSTRING(HIL.MESSAGE,21,4) = ") + CLib::Quot(strLuggNo);
		strSql += CRLF + _T("       OR ") + m_pDoc->NVL + _T("(HIL.LUGG_NO,'') = ") + CLib::Quot(strLuggNo) + _T(" )");
	}

	if(strStnNo != _T(""))
	{
		strSql += CRLF + _T("    AND ( SUBSTRING(HIL.MESSAGE,34,3) = ") + CLib::Quot(strStnNo);
		strSql += CRLF + _T("       OR SUBSTRING(HIL.MESSAGE,47,3) = ") + CLib::Quot(strStnNo);
		strSql += CRLF + _T("       OR SUBSTRING(HIL.MESSAGE,18,3) = ") + CLib::Quot(strStnNo) + _T(" )");
	}
#if ORACLE
	strSql += CRLF + _T(" AND ROWNUM <=	") + strNextRowCnt;
#endif

	strSql += CRLF + _T(" ORDER BY HIL.INS_DT DESC");

#if POSTGRESQL
	if (strRowCnt != _T("ALL"))
		strSql += CRLF + _T(" LIMIT	") + strNextRowCnt;
#elif MSSQL
	if (strRowCnt != _T("ALL"))	// [LGLS] SQL2008: inject TOP n instead of LIMIT
		CLib::ApplyTopN(strSql, strNextRowCnt);	// [LGLS]
#endif
	

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}


void CLogMesSkinDlg::OnBnClickedPreRow()
{
	InitializeSpread(-1, TRUE);
}

void CLogMesSkinDlg::OnBnClickedNextRow() 
{
	InitializeSpread(1, TRUE);
}




void CLogMesSkinDlg::ClickSpread(long Col, long Row) //셀클릭
{
	/*
	variant_t val;
	m_nActiveRow = Row;
	
	m_pSpreadMain.GetText(3, m_nActiveRow, &val);
	CString strComment = (LPCTSTR)(_bstr_t)val;
	m_edtMESDetailLog.SetWindowText(strComment);

	int nFindTo = strComment.Find(_T("' [MRCD]"));

	if (nFindTo > 0)
	{
		CString strSql = _T("");
		CString strMSG = _T("");
		CString strMessage = _T("");
		int nRowCnt = 0;

		int nFindFrom = strComment.Find(_T("<U2,1 '"));
		CString strMRCD = strComment.Mid(nFindFrom+7, nFindTo-nFindFrom-7);

		strSql += _T("	SELECT ")+ m_pDoc->NVL + _T("(MSG_KOR, '0') AS MSG 					");
		strSql += _T("	  FROM EQP_ECD_MST				");
		strSql += _T("	 WHERE EQP_TYP		= 'MES'		");
		strSql += _T("	   AND EQP_ERR_CD	= '") + strMRCD + _T("'		");

		strSql = CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);

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
			strMSG = pRsw->GetItem(_T("MSG"));
		}

		m_edtMESDetailLog2.SetWindowText(_T(" MRCD : ") + strMRCD + _T(" \r\n COMMENT : ") + strMSG);
	}
	else
	{
		m_edtMESDetailLog2.SetWindowText(_T(""));
	}
	
	*/

}