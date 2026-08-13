// LogBcr.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "LogBcrSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"

// CLogBcr 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogBcrSkinDlg, CSkinDialog)

CLogBcrSkinDlg::CLogBcrSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogBcrSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CLogBcrSkinDlg::CLogBcrSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogBcrSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}


CLogBcrSkinDlg::~CLogBcrSkinDlg()
{
	m_pDoc->m_pLogBcrSkinDlg = NULL;
	this->DestroyWindow();
}
BOOL CLogBcrSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);

}
void CLogBcrSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_LOG_BCR,				  m_SpLogBcr);
	DDX_Control(pDX, IDC_LBL_LOG_BCR_BCR_MC_NO,   m_lblBcrMcNo);
	DDX_Control(pDX, IDC_LBL_LOG_BCR_BCR_NO,	  m_lblBcrNo);
	DDX_Control(pDX, IDC_LBL_LOG_BCR_FROM_DATE,   m_lblFromDate);
	DDX_Control(pDX, IDC_LBL_LOG_BCR_TO_DATE,	  m_lblToDate);
	DDX_Control(pDX, IDC_LBL_LOG_BCR_RESULT,	  m_lblLogBcrResult);
	DDX_Control(pDX, IDC_LBL_LOG_BCR_CNT,		  m_lblLogBcrCnt);

	DDX_Control(pDX, IDC_GRP_LOG_BCR_SEARCH,	  m_grpLogBcrSearch);
	DDX_Control(pDX, IDC_GRP_LOG_BCR_SEARCH2,	  m_grpLogBcrSearch2);
	DDX_Control(pDX, IDC_EDT_LOG_BCR_BCR_MC_NO,   m_edtBcrMcNo);
	DDX_Control(pDX, IDC_EDT_LOG_BCR_BCR_NO,	  m_edtBcrNo);

	DDX_Control(pDX, IDC_DTP_LOG_BCR_FROM_DATE,   m_dtInsStartD);
	DDX_Control(pDX, IDC_DTP_LOG_BCR_FROM_TIME,   m_dtInsStartT);
	DDX_Control(pDX, IDC_DTP_LOG_BCR_TO_DATE,     m_dtInsEndD);
	DDX_Control(pDX, IDC_DTP_LOG_BCR_TO_TIME,     m_dtInsEndT);

	DDX_Control(pDX, IDC_BTN_LOG_BCR_SEARCH,      m_btnLogBcrSearch);

	DDX_Control(pDX, IDC_CBX_ROW_CNT_BCR_HIS,	 m_cbxRowCnt);
	DDX_Control(pDX, IDC_BTN_PRE_BCR_HIS,		 m_btnPre);
	DDX_Control(pDX, IDC_BTN_NEXT_BCR_HIS,		 m_btnNext);
	DDX_Control(pDX, IDC_CBX_LOG_BCR_WH_TYP, m_cbxLogBcrWhTyp);
	DDX_Control(pDX, IDC_LBL_LOG_BCR_WH_TYP, m_lblLogBcrWhTyp);
	DDX_Control(pDX, IDC_CBX_LOG_BCR_WEIGHT_READ_STA, m_cbxLogBcrWeightReadSta);
	DDX_Control(pDX, IDC_LBL_LOG_BCR_WEIGHT_READ_STA, m_m_lblLogBcrWeightReadSta);
}


BEGIN_MESSAGE_MAP(CLogBcrSkinDlg, CSkinDialog)
	ON_MESSAGE(SSM_CLICK, &CLogBcrSkinDlg::OnLglsHdrSort)
	ON_BN_CLICKED(IDC_BTN_LOG_BCR_SEARCH, &CLogBcrSkinDlg::OnBnClickedBtnLogBcrSearch)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_CBX_ROW_CNT_BCR_HIS, &CLogBcrSkinDlg::OnSetfocusCmbRowCnt)
	ON_BN_CLICKED(IDC_BTN_NEXT_BCR_HIS, &CLogBcrSkinDlg::OnBnClickedNextRow)
	ON_BN_CLICKED(IDC_BTN_PRE_BCR_HIS, &CLogBcrSkinDlg::OnBnClickedPreRow)
END_MESSAGE_MAP()

// [LGLS] 그리드 헤더 클릭 정렬
LRESULT CLogBcrSkinDlg::OnLglsHdrSort(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if(pCell != NULL && pCell->Row == 0 && pCell->Col >= 1)
		m_SpreadSheet.SortByHeaderColumn(pCell->Col);
	return 0;
}

BOOL CLogBcrSkinDlg::OnInitDialog()
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

	CLib::BindCombo(m_cbxLogBcrWhTyp, _T("WH_TYP"),m_pDoc, (int)pEn, FALSE);
	CLib::BindCombo(m_cbxLogBcrWeightReadSta, _T("WEIGHT_READ_STA"),m_pDoc, (int)pEn, TRUE);
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

	m_dtInsStartD.SetTime(tStartTime);
	m_dtInsStartT.SetTime(tStartTime);

	m_dtInsEndD.SetTime(tstrToTime);
	m_dtInsEndT.SetTime(tstrToTime);

	m_dtInsStartT.SetFormat(_T("HH:mm:ss"));
	m_dtInsEndT.SetFormat(_T("HH:mm:ss"));

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
	m_SpreadSheet.AddColHead(_T("설비 번호"), 8);
	m_SpreadSheet.AddColHead(_T("TRACK 번호"), 10);
	m_SpreadSheet.AddColHead(_T("수신 상태"), 10);
	m_SpreadSheet.AddColHead(_T("무게값"), 15);
	m_SpreadSheet.AddColHead(_T("읽은값"), 19);
	m_SpreadSheet.AddColHead(_T("추가 시간"), 19);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

#pragma endregion

	InitializeSpread(0, FALSE);

	{ CString _tt; GetWindowText(_tt); if(_tt.Find(_T("[")) < 0) SetWindowText(_tt + _T(" [WC_HIS]")); }	// [LGLS] 제목에 조회 테이블명
	return TRUE;  
}
HCURSOR CLogBcrSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLogBcrSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

void CLogBcrSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CLogBcrSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnLogBcrSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnLogBcrSearch.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

}
void CLogBcrSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;

	m_btnLogBcrSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnLogBcrSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);
	m_btnLogBcrSearch.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnLogBcrSearch.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);	
}

void CLogBcrSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{	//
	RenameResource(nEN_LANG);
	RedrawImage();
}

void CLogBcrSkinDlg::RenameResource( EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_BCR_WH_TYP, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("weightreadsta"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_BCR_WEIGHT_READ_STA, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrmcno"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_BCR_BCR_MC_NO, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrno"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_BCR_BCR_NO, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromdate"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_BCR_FROM_DATE, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("todate"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_BCR_TO_DATE, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_LOG_BCR_SEARCH, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("result"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_BCR_RESULT, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_LOG_BCR_SEARCH, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcrlog\\"), _T("dlg_bcrlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_LOG_BCR_SEARCH2, strValue);
}


void CLogBcrSkinDlg::OnBnClickedBtnLogBcrSearch()
{
	InitializeSpread(0, TRUE);
}

CString CLogBcrSkinDlg::GetQrySelect(int nRowCheck, BOOL bSearch = FALSE )
{
	if(m_pDoc == NULL){ return _T(""); };
	//CString strWH_TYP = m_pDoc->m_WH_TYP;
	CString strWH_TYP = m_cbxLogBcrWhTyp.GetItemKey(m_cbxLogBcrWhTyp.GetCurSel());
	CString strWEIGHT_READ_STA = m_cbxLogBcrWeightReadSta.GetItemKey(m_cbxLogBcrWeightReadSta.GetCurSel());

	CString strWC_MC_NO, strWC_NO = _T("");
	CString strdtFrom, strdtTo;
	CTime tDATE_FROM, tDATE_TO, tTIME_FROM, tTIME_TO;
	m_edtBcrMcNo.GetWindowText(strWC_MC_NO);
	m_edtBcrNo.GetWindowText(strWC_NO);
	m_dtInsStartD.GetTime(tDATE_FROM);
	m_dtInsStartT.GetTime(tTIME_FROM);
	m_dtInsEndD.GetTime(tDATE_TO);
	m_dtInsEndT.GetTime(tTIME_TO);

	strWC_MC_NO.Trim();
	strWC_NO.Trim();

	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tDATE_FROM, tTIME_FROM);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);

	CString strRowCnt;
	int nRowCnt;	
	int nRowSum;
	strRowCnt = m_cbxRowCnt.GetItemKey(m_cbxRowCnt.GetCurSel()); 
	nRowCnt = CConvert::ToInt(strRowCnt);
	if (nRowCheck == 0) { nRowSum = nRowCnt; m_pPreRowCnt = 0; }
	if (nRowCheck < 0) { nRowSum = m_pPreRowCnt - nRowCnt; }
	if (nRowCheck > 0) { nRowSum = m_pPreRowCnt + nRowCnt; }
	if (nRowSum == 0) { nRowSum = nRowCnt; m_pPreRowCnt = 0; }

	CString strNextRowCnt = CConvert::ToString(nRowSum);

	CString strSql = _T("");
	CString CRLF = _T("\r\n");

	strSql = CRLF +  _T(" SELECT ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, HIS.WH_TYP) AS WH_TYP							    ");
	strSql += CRLF + _T("      , HIS.WC_NO AS WC_NO																				");
	strSql += CRLF + _T("      , HIS.WC_MC_NO AS WC_MC_NO																		");
	strSql += CRLF + _T("      , ") + m_pDoc->NVL + _T("(CCD_WEIGHT_READ_STA.CCD_NM_KOR, HIS.WEIGHT_READ_STA) AS WEIGHT_READ_STA   ");
	strSql += CRLF + _T("	   , HIS.WEIGHT_RCV_VAL AS WEIGHT_RCV_VAL															 ");
	strSql += CRLF + _T("      , HIS.REMARKS AS REMARK																				");
	strSql += CRLF + _T("      , HIS.INS_DT AS INS_DT 																				");
	strSql += CRLF + _T("   FROM WC_HIS HIS LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP												 ");
	strSql += CRLF + _T("                                 ON CCD_WH_TYP.CDX_CD = 'WH_TYP'										 ");
	strSql += CRLF + _T("                                AND CCD_WH_TYP.CCD_CD = HIS.WH_TYP										 ");
	strSql += CRLF + _T("                   LEFT OUTER JOIN COMMON_CODE CCD_WEIGHT_READ_STA										 ");
	strSql += CRLF + _T("                                 ON CCD_WEIGHT_READ_STA.CDX_CD = 'WEIGHT_READ_STA'						 ");
	strSql += CRLF + _T("                                AND CCD_WEIGHT_READ_STA.CCD_CD = HIS.WEIGHT_READ_STA					 ");
	strSql += CRLF + _T("  WHERE (CONVERT(VARCHAR(8),HIS.INS_DT,112) + REPLACE(CONVERT(VARCHAR(8),HIS.INS_DT,108),':','')) BETWEEN '") + strdtFrom + _T("' AND '") + strdtTo + _T("'");	// [LGLS]
	if(strWH_TYP != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HIS.WH_TYP =  ") + CLib::Quot(strWH_TYP);
	}
	if(strWEIGHT_READ_STA != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HIS.WEIGHT_READ_STA =  ") + CLib::Quot(strWEIGHT_READ_STA);
	}
	if(strWC_NO != _T(""))
	{
		strSql += CRLF + _T("    AND HIS.WC_NO =  ") + CLib::Quot(strWC_NO);
	}
	if(strWC_MC_NO != _T(""))
	{
		strSql += CRLF + _T("    AND HIS.WC_MC_NO =  ") + CLib::Quot(strWC_MC_NO);
	}
#if ORACLE
	strSql += CRLF + _T(" AND ROWNUM <=	") + strNextRowCnt;
#endif
	strSql += CRLF + _T(" ORDER BY HIS.INS_DT DESC");
#if POSTGRESQL
	if (strRowCnt != _T("ALL"))
		strSql += CRLF + _T(" LIMIT	") + strNextRowCnt;
#elif MSSQL
	if (strRowCnt != _T("ALL"))	// [LGLS] SQL2008: inject TOP n instead of LIMIT
		CLib::ApplyTopN(strSql, strNextRowCnt);	// [LGLS]
#endif
	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CLogBcrSkinDlg::FillSpreadColumn(CStringArray& strCols)
{
	//m_SpLogBcr.SetBackColor(WHITE);
	//int nColCnt = strCols.GetSize();
	//m_SpLogBcr.SetMaxCols(nColCnt);
	//CString strValue = _T("");
	//for(int nIdxCol = 0; nIdxCol < nColCnt; nIdxCol++)
	//{
	//	strValue.Format(_T("%s\r"), strCols.GetAt(nIdxCol));
	//	m_SpLogBcr.SetText(nIdxCol + 1, 0, variant_t(strValue));
	//	//m_SpLogBcr.put_ColWidth(nIdxCol + 1, strValue.GetLength() * 1.5);
	//}
	//m_SpLogBcr.put_ColWidth(2, 10);
}


void CLogBcrSkinDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pDoc->m_pLogBcrSkinDlg =  NULL;
	CSkinDialog::OnClose();
}


void CLogBcrSkinDlg::OnSetfocusCmbRowCnt()
{
	m_pPreRowCnt = 0;
}


void CLogBcrSkinDlg::OnBnClickedNextRow()
{
	InitializeSpread(1, TRUE);
}


void CLogBcrSkinDlg::OnBnClickedPreRow()
{
	InitializeSpread(-1, TRUE);
}

void CLogBcrSkinDlg::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	//정렬 가능
	//m_SpLogBcr.SetUserColAction(1);
	//m_SpLogBcr.ClearRange(1, 1, -1, -1, TRUE);

	m_SpreadSheet.PrepareLoadSpread();
	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_lblLogBcrCnt.SetWindowText(CConvert::ToString(nRowCnt));
	m_SpreadSheet.FinishLoadSpread();
}

void CLogBcrSkinDlg::SetMaxRows(int pRowCnt)
{
	//m_SpLogBcr.SetMaxRows(pRowCnt);
}

void CLogBcrSkinDlg::SetMaxCols(int pMaxCol)
{
	//m_SpLogBcr.SetMaxCols(pMaxCol);
}

void CLogBcrSkinDlg::SetColWidth(int pColCnt, int pColSize)
{
	//m_SpLogBcr.put_ColWidth(pColCnt+1,pColSize);
}

int CLogBcrSkinDlg::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
{
	CStringList strList;
	CStringArray arrColName;
	CStringArray arrColSize;
	int nLANG = m_pDoc->m_enLang;
	int nColSize = -1;
	int nRowCnt, nColCnt = -1;
	CString strMessage = _T("");

	CString strSql = GetQrySelect(nRowCheck, bSearch);

	_RecordsetPtr pRsptr  =m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

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

int CLogBcrSkinDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
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

void CLogBcrSkinDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}
