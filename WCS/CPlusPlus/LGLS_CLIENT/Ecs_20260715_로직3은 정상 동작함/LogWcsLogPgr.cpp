// LogWcsLogPgr.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "LogWcsLogPgr.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CLogWcsLogPgr 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogWcsLogPgr, CSkinDialog)

CLogWcsLogPgr::CLogWcsLogPgr(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogWcsLogPgr::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CLogWcsLogPgr::CLogWcsLogPgr(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogWcsLogPgr::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CLogWcsLogPgr::~CLogWcsLogPgr()
{
	m_pDoc->m_pLogWcsSkinDlg = NULL;
}

void CLogWcsLogPgr::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_SPD_LOG_WCS,	m_pSpreadMain);
	DDX_Control(pDX, IDC_BTN_WCS_LOG_SEARCH, m_btnWcsLogSearch);
	DDX_Control(pDX, IDC_CBX_ROW_CNT, m_cbxRowCnt);
	DDX_Control(pDX, IDC_CBX_WCS_LOG_PGR_NM, m_cbxWcsLogPgrNm);
	DDX_Control(pDX, IDC_CBX_WCS_LOG_WH_TYP, m_cbxWcsLogWhTyp);
	DDX_Control(pDX, IDC_DTP_FROM_DATE, m_dtpFrom);
	DDX_Control(pDX, IDC_DTP_FROM_TIME, m_dtpFromTime);
	DDX_Control(pDX, IDC_DTP_TO_DATE, m_dtpTo);
	DDX_Control(pDX, IDC_DTP_TO_TIME, m_dtpToTime);
	DDX_Control(pDX, IDC_EDT_WCS_LOG_DEST_POS, m_edtWcsLogDestpos);
	DDX_Control(pDX, IDC_EDT_WCS_LOG_MESSAGE, m_edtWcsLogMessage);
	DDX_Control(pDX, IDC_EDT_WCS_LOG_START_POS, m_edtWcsLogStartPos);
	DDX_Control(pDX, IDC_GRP_WCS_SEARCH, m_grpWcsSearch);
	DDX_Control(pDX, IDC_GRP_WCS_SEARCH2, m_grpWcsSearch2);
	DDX_Control(pDX, IDC_LBL_WCS_LOG_CNT, m_lblWcsLogCnt);
	DDX_Control(pDX, IDC_LBL_WCS_LOG_DEST_POS, m_lblWcsLogDestPos);
	DDX_Control(pDX, IDC_LBL_WCS_LOG_FROM_DATE, m_lblWcsLogFromDate);
	DDX_Control(pDX, IDC_LBL_WCS_LOG_LUGG_NO, m_lblWcsLogLuggNo);
	DDX_Control(pDX, IDC_LBL_WCS_LOG_MESSAGE, m_lblWcsLogMessage);
	DDX_Control(pDX, IDC_LBL_WCS_LOG_PGR_NM, m_lblWcsLogPgrNm);
	DDX_Control(pDX, IDC_LBL_WCS_LOG_RESULT, m_lblWcsLogResult);
	DDX_Control(pDX, IDC_LBL_WCS_LOG_START_POS, m_lblWcsLogStartPos);
	DDX_Control(pDX, IDC_LBL_WCS_LOG_TO_DATE, m_lblWcsLogToDate);
	DDX_Control(pDX, IDC_LBL_WCS_LOG_WH_TYP, m_lblWcsLogWhTyp);
	DDX_Control(pDX, IDC_EDT_WCS_LOG_LUGG_NO, m_edtWcsLogLuggNo);
}


BEGIN_MESSAGE_MAP(CLogWcsLogPgr, CSkinDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_WCS_LOG_SEARCH, &CLogWcsLogPgr::OnBnClickedBtnWcsLogSearch)
END_MESSAGE_MAP()


// CLogWcsLogPgr 메시지 처리기입니다.


BOOL CLogWcsLogPgr::OnInitDialog()
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

	CLib::BindCombo(m_cbxWcsLogWhTyp, _T("WH_TYP"),m_pDoc, (int)pEn, FALSE);
	CLib::BindCombo(m_cbxWcsLogPgrNm, _T("PGR_NM"),m_pDoc, (int)pEn, TRUE);
	CLib::BindCombo(m_cbxRowCnt, _T("ROW_CNT"), m_pDoc ,(int)pEn, FALSE);
	
	InitializeResource(pEn);

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
	m_SpreadSheet.AddColHead(_T("창고 타입"), 8);
	m_SpreadSheet.AddColHead(_T("프로그램"), 20);
	m_SpreadSheet.AddColHead(_T("작업 번호"), 10);
	m_SpreadSheet.AddColHead(_T("메시지"), 29);
	m_SpreadSheet.AddColHead(_T("추가 시간"), 19);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

#pragma endregion

	InitializeSpread(0, TRUE);

	return TRUE;
}


void CLogWcsLogPgr::OnClose()
{
	m_pDoc->m_pLogWcsSkinDlg = NULL;
	delete this;
}


void CLogWcsLogPgr::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

void CLogWcsLogPgr::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE sizeWideLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_WIDELARGE);

	CRect rc2;

	m_btnWcsLogSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnWcsLogSearch.MoveWindow(rc2.left - 10, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);
}

void CLogWcsLogPgr::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	//정렬 가능
	//m_pSpreadMain.SetUserColAction(1);

	//m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);

	m_SpreadSheet.PrepareLoadSpread();
	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_lblWcsLogCnt.SetWindowText(CConvert::ToString(nRowCnt));
	m_SpreadSheet.FinishLoadSpread();
}

void CLogWcsLogPgr::SetMaxRows(int pRowCnt)
{
	//m_pSpreadMain.SetMaxRows(pRowCnt);
}

void CLogWcsLogPgr::SetColWidth(int pColCnt, int pColSize)
{
	//m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

int CLogWcsLogPgr::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
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

void CLogWcsLogPgr::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

int CLogWcsLogPgr::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
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

void CLogWcsLogPgr::SetMaxCols(int pMaxCol)
{
	//m_pSpreadMain.SetMaxCols(pMaxCol);
}

void CLogWcsLogPgr::RedrawImage()
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
	m_btnWcsLogSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnWcsLogSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);

}

void CLogWcsLogPgr::InitializeResource(EN_LANG nEN_LANG)
{
	RedrawImage();
}

CString CLogWcsLogPgr::GetQrySelect_Main(int nRowCheck, BOOL bSearch)
{
	int nLANG = m_pDoc->m_enLang;
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	CTime tToDate, tToTime, tFromDate, tFromTime;
	CString strdtFrom, strdtTo;
	CString strWH_TYP = m_cbxWcsLogWhTyp.GetItemKey(m_cbxWcsLogWhTyp.GetCurSel());
	CString strPgrNm = m_cbxWcsLogPgrNm.GetItemKey(m_cbxWcsLogPgrNm.GetCurSel());
	CString strWcsLogMessage, strWcsLogLuggNo, strWcsLogStartPos, strWcsLogDestPos;
	m_edtWcsLogMessage.GetWindowText(strWcsLogMessage);
	m_edtWcsLogStartPos.GetWindowText(strWcsLogStartPos);
	m_edtWcsLogDestpos.GetWindowText(strWcsLogDestPos);
	m_edtWcsLogLuggNo.GetWindowText(strWcsLogLuggNo);

	m_dtpTo.GetTime(tToDate);
	m_dtpToTime.GetTime(tToTime);
	m_dtpFrom.GetTime(tFromDate);
	m_dtpFromTime.GetTime(tFromTime);

	CString strRowCnt;
	int nRowCnt;	
//	int nRowSum;
	strRowCnt = m_cbxRowCnt.GetItemKey(m_cbxRowCnt.GetCurSel()); 
	nRowCnt = CConvert::ToInt(strRowCnt);

	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tFromDate, tFromTime);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tToDate, tToTime);

	
	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, WLP.WH_TYP) AS WH_TYP			");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_PGR_NM.CCD_NM_KOR, WLP.PGR_NM) AS PGR_NM			");
	strSql += CRLF + _T("      ,WLP.LUGG_NO AS LUGG_NO															");
	//strSql += CRLF + _T("      ,WLP.TRACK_FROM AS TRACK_FROM													");
	//strSql += CRLF + _T("      ,WLP.TRACK_TO AS TRACK_TO														");
	strSql += CRLF + _T("      ,WLP.LOG_KOR AS LOG_KOR															");
	strSql += CRLF + _T("      ,WLP.INS_DT AS INS_DT															");
	strSql += CRLF + _T("  FROM WCS_LOG_PGR WLP																	");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP											");
	strSql += CRLF + _T("                    ON WLP.WH_TYP = CCD_WH_TYP.CCD_CD									");
	strSql += CRLF + _T("                   AND CCD_WH_TYP.CDX_CD = 'WH_TYP'									");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_PGR_NM											");
	strSql += CRLF + _T("                    ON WLP.PGR_NM = CCD_PGR_NM.CCD_CD									");
	strSql += CRLF + _T("                   AND CCD_PGR_NM.CDX_CD = 'PGR_NM'									");
	strSql += CRLF + _T(" WHERE (CONVERT(VARCHAR(8),WLP.INS_DT,112) + REPLACE(CONVERT(VARCHAR(8),WLP.INS_DT,108),':','')) BETWEEN '") + strdtFrom + _T("' AND '") + strdtTo + _T("'");	// [LGLS]
	if(strWH_TYP != _T("") && strWH_TYP != _T("ALL"))
	{
		strSql += CRLF + _T("    AND WLP.WH_TYP =  ") + CLib::Quot(strWH_TYP);
	}

	//if(strWcsLogStartPos != _T("") && strWcsLogStartPos != _T("ALL"))
	//{
	//	strSql += CRLF + _T("    AND WLP.TRACK_FROM =  ") + CLib::Quot(strWcsLogStartPos);
	//}

	if(strPgrNm != _T("") && strPgrNm != _T("ALL"))
	{
		strSql += CRLF + _T("    AND WLP.PGR_NM =  ") + CLib::Quot(strPgrNm);
	}

	//if(strWcsLogDestPos != _T("") && strWcsLogDestPos != _T("ALL"))
	//{
	//	strSql += CRLF + _T("    AND WLP.TRACK_TO =  ") + CLib::Quot(strWcsLogDestPos);
	//}

	if(strWcsLogMessage != _T("") && strWcsLogMessage != _T("ALL"))
	{
		strSql += CRLF + _T("    AND WLP.LOG_KOR LIKE '%") + strWcsLogMessage + _T("%'");
	}

	if(strWcsLogLuggNo != _T("") && strWcsLogLuggNo != _T("ALL"))
	{
		strSql += CRLF + _T("    AND WLP.LUGG_NO LIKE '%") + strWcsLogLuggNo + _T("%'");
	}
#if ORACLE
	strSql += CRLF + _T(" AND ROWNUM <=	") + strNextRowCnt;
#endif

	strSql += CRLF + _T(" ORDER BY WLP.INS_DT DESC");

#if POSTGRESQL
	if (strRowCnt != _T("ALL"))
		strSql += CRLF + _T(" LIMIT	") + strRowCnt;
#elif MSSQL
	if (strRowCnt != _T("ALL"))	// [LGLS] SQL2008: inject TOP n instead of LIMIT
		CLib::ApplyTopN(strSql, strRowCnt);	// [LGLS]
#endif
	

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CLogWcsLogPgr::OnBnClickedBtnWcsLogSearch()
{
	UpdateData(TRUE);

	if(m_pDoc == NULL) { return; };
	InitializeSpread(0, TRUE);
}
