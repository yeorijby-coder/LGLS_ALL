// ViewHostEmptyPltDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "ViewHostEmptyPltDlg.h"
#include "afxdialogex.h"
#include "EcsDoc.h"
#include "RecordSetWrap.h"


// CViewHostEmptyPltDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewHostEmptyPltDlg, CSkinDialog)

CViewHostEmptyPltDlg::CViewHostEmptyPltDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewHostEmptyPltDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CViewHostEmptyPltDlg::CViewHostEmptyPltDlg(CEcsDoc* pDoc, CWnd* pParent)
	: CSkinDialog(CViewHostEmptyPltDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CViewHostEmptyPltDlg::~CViewHostEmptyPltDlg()
{
	m_pDoc->m_pViewHostEmptyPltDlg = NULL;
	this->DestroyWindow();
}

BEGIN_MESSAGE_MAP(CViewHostEmptyPltDlg, CSkinDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_HOST_EMPTY_PLT_SEARCH, &CViewHostEmptyPltDlg::OnBnClickedBtnHostEmptyPltSearch)
	ON_BN_CLICKED(IDC_BTN_HOST_EMPTY_PLT_DELETE, &CViewHostEmptyPltDlg::OnBnClickedBtnHostEmptyPltDelete)
	ON_BN_CLICKED(IDC_BTN_HOST_EMPTY_PLT_REQUEST, &CViewHostEmptyPltDlg::OnBnClickedBtnHostEmptyPltRequest)
	ON_MESSAGE(SSM_CLICK, &CViewHostEmptyPltDlg::OnSpreadLClick)
END_MESSAGE_MAP()

void CViewHostEmptyPltDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_SPD_HOST_EMPTY_PLT, m_pSpreadMain);
	DDX_Control(pDX, IDC_BTN_HOST_EMPTY_PLT_DELETE, m_btnHostEmptyPltDelete);
	DDX_Control(pDX, IDC_BTN_HOST_EMPTY_PLT_REQUEST, m_btnHostEmptyPltRequest);
	DDX_Control(pDX, IDC_BTN_HOST_EMPTY_PLT_SEARCH, m_btnHostEmptyPltSearch);
	DDX_Control(pDX, IDC_BTN_HOST_EMPTY_PLT_SEARCH, m_btnHostEmptyPltSearch);
	DDX_Control(pDX, IDC_CBX_HOST_EMPTY_PLT_KIND, m_cbxHostEmptyPltKind);
	DDX_Control(pDX, IDC_CBX_HOST_EMPTY_PLT_STATUS, m_cbxHostEmptyPltStatus);
	DDX_Control(pDX, IDC_CBX_HOST_EMPTY_PLT_STN, m_cbxHostEmptyPltStn);
	DDX_Control(pDX, IDC_CBX_HOST_EMPTY_PLT_WH_TYP, m_cbxHostEmptyPltWhTyp);
	DDX_Control(pDX, IDC_CBX_ROW_CNT, m_cbxRowCnt);
	DDX_Control(pDX, IDC_DTP_FROM_DATE, m_dtpFrom);
	DDX_Control(pDX, IDC_DTP_FROM_TIME, m_dtpFromTime);
	DDX_Control(pDX, IDC_DTP_TO_DATE, m_dtpTo);
	DDX_Control(pDX, IDC_DTP_TO_TIME, m_dtpToTime);
	DDX_Control(pDX, IDC_EDT_HOST_EMPTY_PLT_LUGG_NO, m_edtHostEmptyPltLuggNo);
	DDX_Control(pDX, IDC_LBL_HOST_EMPTY_PLT_CNT, m_lblHostEmptyPltCnt);
	DDX_Control(pDX, IDC_GRP_HOST_EMPTY, m_grpHostEmpty);
	DDX_Control(pDX, IDC_GRP_HOST_EMPTY2, m_grpHostEmpty2);
	DDX_Control(pDX, IDC_LBL_HOST_EMPTY_PLT_FROM_DATE, m_lblHostEmptyPltFromDate);
	DDX_Control(pDX, IDC_LBL_HOST_EMPTY_PLT_KIND, m_lblHostEmptyPltKind);
	DDX_Control(pDX, IDC_LBL_HOST_EMPTY_PLT_LUGG_NO, m_lblHostEmptyPltLuggNo);
	DDX_Control(pDX, IDC_LBL_HOST_EMPTY_PLT_RESULT, m_lblHostEmptyPltResult);
	DDX_Control(pDX, IDC_LBL_HOST_EMPTY_PLT_STATUS, m_lblHostEmptyPltStatus);
	DDX_Control(pDX, IDC_LBL_HOST_EMPTY_PLT_STN, m_lblHostEmptyPltStn);
	DDX_Control(pDX, IDC_LBL_HOST_EMPTY_PLT_TO_DATE, m_lblHostEmptyPltToDate);
	DDX_Control(pDX, IDC_LBL_HOST_EMPTY_PLT_WH_TYP, m_lblHostEmptyPltWhTyp);
}

LRESULT CViewHostEmptyPltDlg::OnSpreadLClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if(pCell != NULL && pCell->Row == 0 && pCell->Col >= 1){ m_SpreadSheet.SortByHeaderColumn(pCell->Col); return 0; }	// [LGLS] 헤더 클릭 정렬
	if ( pCell->Col == 0 || pCell->Row == 0 )
		return 0;

	if ( m_SpreadSheet.IsCellType(pCell->Col, pCell->Row, SS_TYPE_CHECKBOX) )
	{
		BOOL bCheck = !m_SpreadSheet.GetValueINT(pCell->Col, pCell->Row);
		m_SpreadSheet.SetValueINT(pCell->Col, pCell->Row, bCheck);

		if ( m_SpreadSheet.IsCellType(pCell->Col, 0, SS_TYPE_CHECKBOX) && 
			 m_SpreadSheet.IsCellType(pCell->Col, pCell->Row, SS_TYPE_CHECKBOX) )
		{
			if ( !bCheck )  // Uncheck 시
				m_SpreadSheet.SetHeaderCellCheck(pCell->Col, false);
		}
		return 0;
	}

	//variant_t val;
	if (pCell->Row > 0)
	{
		m_nActiveRow = pCell->Row;

		//m_pSpreadMain.GetText(10, m_nActiveRow, &val);
		CString strStn = m_SpreadSheet.GetValueTXT(3, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
		strStn.Trim();

		//m_pSpreadMain.GetText(11, m_nActiveRow, &val);
		CString strKind = m_SpreadSheet.GetValueTXT(4, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
		strKind.Trim();

		//m_pSpreadMain.GetText(12, m_nActiveRow, &val);
		CString strStatus = m_SpreadSheet.GetValueTXT(5, m_nActiveRow);	// (LPCTSTR)(_bstr_t)val;
		strStatus.Trim();
		
		//작업대 가져오기 (CDX_CD, CCD_NM_KOR, CCD_CD 순서)
		CLib::BindSpreadCommonCode(_T("STN"), strStn, m_strSTN, m_pDoc);

		//작업구분 가져오기 (CDX_CD, CCD_NM_KOR, CCD_CD 순서)
		CLib::BindSpreadCommonCode(_T("KIND"), strKind, m_strKIND, m_pDoc);

		//작업상태 가져오기 (CDX_CD, CCD_NM_KOR, CCD_CD 순서)
		CLib::BindSpreadCommonCode(_T("STATUS"), strStatus, m_strSTATUS, m_pDoc);
	}

	return 0;
}


// CViewHostEmptyPltDlg 메시지 처리기입니다.
BOOL CViewHostEmptyPltDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);	
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{	
		m_bInitialized = TRUE;	
	}

	RedrawImage();
	//RenameResource(pEn);
	
	CLib::BindCombo(m_cbxHostEmptyPltWhTyp, _T("WH_TYP"), m_pDoc, int(pEn), FALSE);
	CLib::BindCombo(m_cbxHostEmptyPltKind, _T("KIND"), m_pDoc, int(pEn), TRUE);
	CLib::BindCombo(m_cbxHostEmptyPltStatus, _T("STATUS"), m_pDoc, int(pEn), TRUE);
	CLib::BindCombo(m_cbxHostEmptyPltStn, _T("STN"), m_pDoc, int(pEn), TRUE);
	CLib::BindCombo(m_cbxRowCnt, _T("ROW_CNT"), m_pDoc ,(int)pEn, FALSE);

	m_cbxHostEmptyPltStatus.SetCurSel(2);

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
	m_SpreadSheet.AddColHead(_T("작업 번호"), 10);
	m_SpreadSheet.AddColHead(_T("작업대"), 20);
	m_SpreadSheet.AddColHead(_T("작업 구분"), 15);
	m_SpreadSheet.AddColHead(_T("작업 상태"), 10);
	m_SpreadSheet.AddColHead(_T("추가 시간"), 20);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

#pragma endregion

	InitializeSpread(0, TRUE);

	{ CString _tt; GetWindowText(_tt); if(_tt.Find(_T("[")) < 0) SetWindowText(_tt + _T(" [HOST_EMPTY_PLT]")); }	// [LGLS] 제목에 조회 테이블명
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CViewHostEmptyPltDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobinfo\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;

	m_btnHostEmptyPltSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnHostEmptyPltSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
	m_btnHostEmptyPltSearch.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnHostEmptyPltSearch.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnHostEmptyPltDelete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnHostEmptyPltDelete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);
	m_btnHostEmptyPltDelete.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnHostEmptyPltDelete.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnHostEmptyPltRequest.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnHostEmptyPltRequest.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
	m_btnHostEmptyPltRequest.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnHostEmptyPltRequest.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);
}

void CViewHostEmptyPltDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE sizeWideLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_WIDELARGE);

	CRect rc2;

	m_btnHostEmptyPltSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnHostEmptyPltSearch.MoveWindow(rc2.left - 10, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);

	m_btnHostEmptyPltDelete.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnHostEmptyPltDelete.MoveWindow(rc2.left - 10, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);

	m_btnHostEmptyPltRequest.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnHostEmptyPltRequest.MoveWindow(rc2.left - 10, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);

}

void CViewHostEmptyPltDlg::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	//정렬 가능
	//m_pSpreadMain.SetUserColAction(1);

	//m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);
	m_SpreadSheet.PrepareLoadSpread();

	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_lblHostEmptyPltCnt.SetWindowText(CConvert::ToString(nRowCnt));

	m_SpreadSheet.FinishLoadSpread();

}

void CViewHostEmptyPltDlg::SetMaxRows(int pRowCnt)
{
	//m_pSpreadMain.SetMaxRows(pRowCnt);
}

void CViewHostEmptyPltDlg::SetColWidth(int pColCnt, int pColSize)
{
	//m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

int CViewHostEmptyPltDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
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

void CViewHostEmptyPltDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

void CViewHostEmptyPltDlg::SetMaxCols(int pMaxCol)
{
	//m_pSpreadMain.SetMaxCols(pMaxCol);
}

int CViewHostEmptyPltDlg::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
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

CString CViewHostEmptyPltDlg::GetQrySelect_Main(int nRowCheck, BOOL bSearch)
{
	int nLANG = m_pDoc->m_enLang;
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	CTime tToDate, tToTime, tFromDate, tFromTime;
	CString strdtFrom, strdtTo, strLuggNo;
	CString strWH_TYP = m_cbxHostEmptyPltWhTyp.GetItemKey(m_cbxHostEmptyPltWhTyp.GetCurSel());
	CString strKIND = m_cbxHostEmptyPltKind.GetItemKey(m_cbxHostEmptyPltKind.GetCurSel());
	CString strStatus = m_cbxHostEmptyPltStatus.GetItemKey(m_cbxHostEmptyPltStatus.GetCurSel());
	CString strStn = m_cbxHostEmptyPltStn.GetItemKey(m_cbxHostEmptyPltStn.GetCurSel());
	m_edtHostEmptyPltLuggNo.GetWindowText(strLuggNo);

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

	
	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, HEP.WH_TYP) AS WH_TYP		");
	strSql += CRLF + _T("      ,HEP.LUGG_NO AS LUGG_NO														");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_STN.CCD_NM_KOR, HEP.STN) AS START_POS			");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_KIND.CCD_NM_KOR, HEP.KIND) AS JOB_TYP			");
	strSql += CRLF + _T("      ,") + m_pDoc->NVL + _T("(CCD_STATUS.CCD_NM_KOR, HEP.STATUS) AS JOB_STATUS	");
	strSql += CRLF + _T("      ,HEP.INS_DT AS INS_DT														");
	//strSql += CRLF + _T("      ,HEP.STN AS STN_HIDE															");
	//strSql += CRLF + _T("      ,HEP.KIND AS KIND_HIDE														");
	//strSql += CRLF + _T("      ,HEP.STATUS AS STATUS_HIDE													");
	strSql += CRLF + _T("  FROM HOST_EMPTY_PLT HEP															");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP										");
	strSql += CRLF + _T("                    ON HEP.WH_TYP = CCD_WH_TYP.CCD_CD								");
	strSql += CRLF + _T("                   AND CCD_WH_TYP.CDX_CD = 'WH_TYP'								");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_STN											");
	strSql += CRLF + _T("                    ON HEP.STN = CCD_STN.CCD_CD									");
	strSql += CRLF + _T("                   AND CCD_STN.CDX_CD = 'STN'										");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_KIND										");
	strSql += CRLF + _T("                    ON HEP.KIND = CCD_KIND.CCD_CD									");
	strSql += CRLF + _T("                   AND CCD_KIND.CDX_CD = 'KIND'									");
	strSql += CRLF + _T("       LEFT OUTER JOIN COMMON_CODE CCD_STATUS										");
	strSql += CRLF + _T("                    ON HEP.STATUS = CCD_STATUS.CCD_CD								");
	strSql += CRLF + _T("                   AND CCD_STATUS.CDX_CD = 'STATUS'								");
	strSql += CRLF + _T(" WHERE (CONVERT(VARCHAR(8),HEP.INS_DT,112) + REPLACE(CONVERT(VARCHAR(8),HEP.INS_DT,108),':','')) BETWEEN '") + strdtFrom + _T("' AND '") + strdtTo + _T("'");	// [LGLS]

	if(strWH_TYP != _T("") && strWH_TYP != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HEP.WH_TYP =  ") + CLib::Quot(strWH_TYP);
	}

	if(strLuggNo != _T("") && strLuggNo != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HEP.LUGG_NO LIKE '%") + strLuggNo + _T("%'");
	}

	if(strStn != _T("") && strStn != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HEP.STN =  ") + CLib::Quot(strStn);
	}

	if(strKIND != _T("") && strKIND != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HEP.KIND =  ") + CLib::Quot(strKIND);
	}

	if(strStatus != _T("") && strStatus != _T("ALL"))
	{
		strSql += CRLF + _T("    AND HEP.STATUS =  ") + CLib::Quot(strStatus);
	}

#if ORACLE
	strSql += CRLF + _T(" AND ROWNUM <=	") + strNextRowCnt;
#endif

	strSql += CRLF + _T(" ORDER BY HEP.INS_DT DESC");

#if POSTGRESQL
	if (strRowCnt != _T("ALL"))
		strSql += CRLF + _T(" LIMIT	") + strRowCnt;
#elif MSSQL
	if (strRowCnt != _T("ALL"))	// [LGLS] SQL2008: inject TOP n instead of LIMIT
		CLib::ApplyTopN(strSql, strRowCnt);	// [LGLS]
#endif
	

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CViewHostEmptyPltDlg::OnClose()
{
	m_pDoc->m_pViewHostEmptyPltDlg = NULL;
	delete this;
}

void CViewHostEmptyPltDlg::OnBnClickedBtnHostEmptyPltSearch()
{
	InitializeSpread(TRUE);
}


void CViewHostEmptyPltDlg::OnBnClickedBtnHostEmptyPltDelete()
{
	variant_t val;
//	int nActiveRow;
	CString strLUGG_NO, strSTN, strKIND, strSTATUS, strMC_NO;
	CString strSql;
	CString strWH_TYP = m_pDoc->m_WH_TYP;

	if (m_nActiveRow < 1)
		return;

	int nLUGG_NO;

	//m_pSpreadMain.GetText(2, nActiveRow, &val);
	strLUGG_NO = m_SpreadSheet.GetValueTXT(2, m_nActiveRow);
	strLUGG_NO.Trim();
	nLUGG_NO = CConvert::ToInt(strLUGG_NO);

	strSTN = m_strSTN;
	strSTN.Trim();

	strKIND = m_strKIND;
	strKIND.Trim();

	strSTATUS = m_strSTATUS;
	strSTATUS.Trim();

	if(AfxMessageBox(m_pDoc->GetMsgLangDef(_T("공PLT요청을 삭제하시겠습니까? 작업번호 : ")) + strLUGG_NO, MB_YESNO) == IDNO) {	return;	}

	if(nLUGG_NO > 0)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업은 생성된 작업입니다.\nECS 작업도 같이 삭제됩니다.\nWMS 작업도 확인 후 삭제 해주세요.")));
	}
	
	m_pDoc->BeginTrans_DLG();

	CString strLOG_LUGG_NO = strLUGG_NO;
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
	CString strLOG_BOTTOM_TRAY = _T("");
	CString strLOG_TOP_TRAY = _T("");
	CString strLOG_MSG = _T("");
	strLOG_MSG.Format(_T("공PLT작업삭제 : 작업번호[%s], 작업대[%s], 작업구분[%s], 작업상태[%s]"),strLUGG_NO, strSTN, strKIND, strSTATUS);

	if (!m_pDoc->GetQueryInsertClientLog(_T("CViewHostEmptyPltDlg"), strLOG_LUGG_NO, _T(""), _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	//HOST 테이블 삭제
	strSql.Format(_T("DELETE FROM HOST_EMPTY_PLT	")
				  _T("		WHERE WH_TYP = '%s'		")
				  _T("		  AND KIND = '%s'		")
				  _T("		  AND STN = '%s'		")
				  _T("		  AND STATUS = '%s'		")
				  _T("		  AND LUGG_NO= '%s'		"), strWH_TYP, strKIND, strSTN, strSTATUS, strLUGG_NO);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);
	
	if(isSuccess == FALSE)
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("공PLT 작업 삭제 중 실패")));
		InitializeSpread(TRUE);
		return;
	}
	
	strSql = _T("");
	strSql.Format(_T("DELETE FROM JOB_MST				")
			_T("				WHERE WH_TYP = '%s'			")
			_T("				  AND LUGG_NO= '%s'		"), strWH_TYP, strLUGG_NO);

	isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);
	
	if(isSuccess == TRUE)
	{
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		InitializeSpread(TRUE);
		return;
	}

	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));

	return;	
}


void CViewHostEmptyPltDlg::OnBnClickedBtnHostEmptyPltRequest()
{
	CString strSql = _T("");
//	int nActiveRow;
	variant_t val;

	if (m_nActiveRow < 1)
		return;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("공 PLT를 WMS에 재요청 하겟습니까?")), MB_YESNO) != IDYES)
			return;

	CString strWH_TYP, strLUGG_NO, strSTN, strKIND, strSTATUS;
	int nLUGG_NO;

	strLUGG_NO = m_SpreadSheet.GetValueTXT(2, m_nActiveRow);
	strLUGG_NO.Trim();
	nLUGG_NO = CConvert::ToInt(strLUGG_NO);

	strSTN = m_strSTN;
	strSTN.Trim();

	strKIND = m_strKIND;
	strKIND.Trim();

	strSTATUS = m_strSTATUS;
	strSTATUS.Trim();

	//해당 상태일 때만 재요청 가능
	if(strSTATUS == _T("Q") && nLUGG_NO == 0)
	{
		m_pDoc->BeginTrans_DLG();
		CString strLOG_MSG;
		strSql = _T("");

		strSql.Format(_T(" UPDATE HOST_EMPTY_PLT						 \n")
				      _T("    SET STATUS = 'N'							 \n")
				      _T("      , UPD_ID = 'ECS_CLIENT'					 \n")
				      _T("       ,UPD_DT = ") + m_pDoc->SYSDATE + _T("   \n")
					  _T("	WHERE INS_DT = (SELECT TOP 1 INS_DT			 \n") // [LGLS] TOP 1 instead of LIMIT 1
					  _T("                    FROM HOST_EMPTY_PLT		 \n")
					  _T("					 WHERE WH_TYP = '%s'		 \n")
					  _T("					   AND KIND = '%s'			 \n")
					  _T("					   AND STN = '%s'			 \n")
					  _T("					   AND STATUS = '%s'		 \n")
					  _T("					 ORDER BY INS_DT DESC		 \n")
					  _T("					 )					   "), m_pDoc->m_WH_TYP, strKIND, strSTN, strSTATUS); // [LGLS] LIMIT removed


		BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

		if(isSuccess == TRUE)
		{
			strLOG_MSG.Format(_T("공PLT 재요청 -> 트랙번호 : %s , 작업구분 : %s"), strSTN, strSTATUS);
			if (!m_pDoc->GetQueryInsertClientLog(_T("CViewHostEmptyPltDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
				m_pDoc->RollbackTrans_DLG();
				return;
			}

			m_pDoc->CommitTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("공 PLT 재요청 성공")));
			InitializeSpread(TRUE);
			return;
		}
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}
	else
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("WMS에 재요청 할 수 있는 상태가 아닙니다.")));
		return;
	}
}

BOOL CViewHostEmptyPltDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)  
	{
		int nID = GetFocus()->GetDlgCtrlID();

		switch(pMsg->wParam)
		{
		case VK_RETURN:
			{
				break;
			}
		case VK_ESCAPE:
			{
				break;
			}
		}
		if (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)
		{
			switch(pMsg->wParam)
			{
			case 1768:
				{
					return true;
					break;
				}
			case 1868:
				{
					return true;
					break;
				}
			case 37://LEFT
				{
					return true;
					break;
				}
			case 38://UP
				{
					return true;
					break;
				}
			case 39://RIGHT
				{
					return true;
					break;
				}
			case 40://DOWN
				{
					return true;
					break;
				}
			}
		}
	}
	return CSkinDialog::PreTranslateMessage(pMsg);
}
