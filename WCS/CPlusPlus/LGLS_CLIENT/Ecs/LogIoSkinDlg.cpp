// LogIoSkinDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Ecs.h"
#include "LogIoSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CLogIoSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogIoSkinDlg, CSkinDialog)

CLogIoSkinDlg::CLogIoSkinDlg(CWnd* pParent /*=NULL*/)
			: CSkinDialog(CLogIoSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CLogIoSkinDlg::CLogIoSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogIoSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CLogIoSkinDlg::~CLogIoSkinDlg()
{
	m_pDoc->m_pLogIoSkinDlg = NULL;
	this->DestroyWindow();
}

BOOL CLogIoSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);

}
void CLogIoSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_BTN_LOG_IO_SEARCH,              m_btnSearch);
	DDX_Control(pDX, IDOK,								 m_btnOk);
	DDX_Control(pDX, IDCANCEL,							 m_btnCancel);

	DDX_Control(pDX, IDC_LBL_WH_TYP,					 m_lblWhTyp);
	DDX_Control(pDX, IDC_LBL_LUGG_NO,					 m_lblLuggNo);
	DDX_Control(pDX, IDC_LBL_FROM_DATE,					 m_lblFromDate);
	DDX_Control(pDX, IDC_LBL_TO_DATE,					 m_lblToDate);
	DDX_Control(pDX, IDC_LBL_IO_LOG_FROM_TRACK,          m_lblFromTrack);
	DDX_Control(pDX, IDC_LBL_IO_LOG_TO_TRACK,			 m_lblToTrack);
	DDX_Control(pDX, IDC_LBL_IO_LOG_FROM_LOCATION,       m_lblFromLocation);
	DDX_Control(pDX, IDC_LBL_IO_LOG_TO_LOCATION,		 m_lblToLocation);
	DDX_Control(pDX, IDC_LBL_JOBTYP,					 m_lblJobTyp);
	
	DDX_Control(pDX, IDC_CBX_IO_LOG_WH_TYP,				 m_cbxIOLogWhTyp);
	DDX_Control(pDX, IDC_EDT_LUGG_NO,					 m_edtLuggNo);
	DDX_Control(pDX, IDC_EDT_FROM_TRACK,				 m_edtFromTrack);
	DDX_Control(pDX, IDC_EDT_TO_TRACK,					 m_edtToTrack);
	DDX_Control(pDX, IDC_EDT_IO_LOG_FROM_LOCATION,		 m_edtFromLocation);
	DDX_Control(pDX, IDC_EDT_IO_LOG_TO_LOCATION,		 m_edtToLocation);
	DDX_Control(pDX, IDC_CBX_IO_LOG_JOB_TYP,			 m_cbxJobTyp);

	DDX_Control(pDX, IDC_DTP_FROM_DATE,					 m_dtpFrom);
	DDX_Control(pDX, IDC_DTP_FROM_TIME,					 m_dtpFromTime);
	DDX_Control(pDX, IDC_DTP_TO_DATE,					 m_dtpTo);
	DDX_Control(pDX, IDC_DTP_TO_TIME,					 m_dtpToTime);
	


	//DDX_Control(pDX, IDC_SPD_LOG_IO,					 m_pSpreadMain);
	DDX_Control(pDX, IDC_LBL_IO_MST_CNT,				 m_lblSpdMainCnt);
	DDX_Control(pDX, IDC_LBL_IO_RESULT,					 m_lblSpdMain);

	DDX_Control(pDX, IDC_BTN_PRE_IO_LOG,				 m_btnPre);
	DDX_Control(pDX, IDC_BTN_NEXT_IO_LOG,				 m_btnNext);
	DDX_Control(pDX, IDC_CBX_ROW_CNT_IO_LOG,			 m_cbxRowCnt);


	DDX_Control(pDX, IDC_GRP_IO_SEARCH,				 m_grp1);
	DDX_Control(pDX, IDC_GRP_IO_SEARCH2,			 m_grp2);


	
}


BEGIN_MESSAGE_MAP(CLogIoSkinDlg, CSkinDialog)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(SSM_CLICK, &CLogIoSkinDlg::OnLglsHdrSort)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_LOG_IO_SEARCH, &CLogIoSkinDlg::OnBnClickedBtnLogIoSearch)
	ON_BN_CLICKED(IDC_BTN_PRE_IO_LOG, &CLogIoSkinDlg::OnBnClickedPreRow)
	ON_BN_CLICKED(IDC_BTN_NEXT_IO_LOG, &CLogIoSkinDlg::OnBnClickedNextRow)
END_MESSAGE_MAP()

// [LGLS] 그리드 헤더 클릭 정렬
LRESULT CLogIoSkinDlg::OnLglsHdrSort(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if(pCell != NULL && pCell->Row == 0 && pCell->Col >= 1)
		m_SpreadSheet.SortByHeaderColumn(pCell->Col);
	return 0;
}
// CLogIoSkinDlg 메시지 처리기입니다.

void CLogIoSkinDlg::OnClose()
{
	m_pDoc->m_pLogIoSkinDlg = NULL;
	delete this;
}

HCURSOR CLogIoSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLogIoSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

void CLogIoSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}


void CLogIoSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	CRect rc2;

	m_btnOk.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnOk.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCancel.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCancel.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnSearch.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}


BOOL CLogIoSkinDlg::OnInitDialog()
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

	CLib::BindCombo(m_cbxIOLogWhTyp,_T("WH_TYP"), m_pDoc, pEn, FALSE);
	CLib::BindCombo(m_cbxJobTyp,_T("JOB_TYP"), m_pDoc, pEn, TRUE);
	CLib::BindCombo(m_cbxRowCnt,_T("ROW_CNT"), m_pDoc, pEn, FALSE);
	

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
	m_SpreadSheet.AddColHead(_T("작업 번호"), 8);
	m_SpreadSheet.AddColHead(_T("작업 구분"), 11);
	m_SpreadSheet.AddColHead(_T("출발지"), 6);
	m_SpreadSheet.AddColHead(_T("출발 위치"), 9);
	m_SpreadSheet.AddColHead(_T("도착지"), 6);
	m_SpreadSheet.AddColHead(_T("도착 위치"), 9);
	m_SpreadSheet.AddColHead(_T("작업 상태"), 15);
	m_SpreadSheet.AddColHead(_T("적재용기"), 12);
	m_SpreadSheet.AddColHead(_T("제품정보"), 12);
	m_SpreadSheet.AddColHead(_T("우선 순위"), 8);
	m_SpreadSheet.AddColHead(_T("추가 시간"), 16);
	m_SpreadSheet.AddColHead(_T("수정 시간"), 16);

	// [LGLS 2026-08-04] 조회 조건에 [적재 용기]/[제품 정보] 추가 (보이는 행 좌표로 한 행 아래에 배치)
	CLib::AddFilterField(this, IDC_LBL_WH_TYP, IDC_LBL_IO_LOG_FROM_TRACK, IDC_EDT_FROM_TRACK,
	                     _T("적재 용기"), IDC_LGLS_IO_LOT_LBL, IDC_LGLS_IO_LOT_EDT, m_arLglsCtrl);
	CLib::AddFilterField(this, IDC_LBL_LUGG_NO, IDC_LBL_IO_LOG_TO_TRACK, IDC_EDT_TO_TRACK,
	                     _T("제품 정보"), IDC_LGLS_IO_PRD_LBL, IDC_LGLS_IO_PRD_EDT, m_arLglsCtrl);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));
	//GetDlgItem(IDC_STATIC_TOP)->BringWindowToTop();
//	m_SpreadSheet.m_Spread.BringWindowToTop();

#pragma endregion

	InitializeSpread(0, TRUE);

	{ CString _tt; GetWindowText(_tt); if(_tt.Find(_T("[")) < 0) SetWindowText(_tt + _T(" [JOB_MST_HIS]")); }	// [LGLS] 제목에 조회 테이블명
	return TRUE;
}

void CLogIoSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\128x32\\");

	CString strExtension = _T(".png");
	m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnOk.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("ok"), strExtension)), NULL, 5, 5);
	m_btnCancel.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cancel"), strExtension)), NULL, 5, 5);
	m_btnSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);

}

void CLogIoSkinDlg::RenameResource( EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_LOG_IO_SEARCH, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ok"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDOK, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cancel"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDCANCEL, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_WH_TYP, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("luggno"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LUGG_NO, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("requestid"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_REQUEST_ID, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromdate"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_FROM_DATE, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("todate"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_TO_DATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_JOBTYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fromtrack"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_IO_LOG_FROM_TRACK, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("totrack"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_IO_LOG_TO_TRACK, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("result"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_IO_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_IO_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_iolog\\"), _T("dlg_iolog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_IO_SEARCH2, strValue);
}

void CLogIoSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RedrawImage();
	RenameResource(nEN_LANG);
}

CString CLogIoSkinDlg::GetQrySelect()
{
	return _T("");
}

void CLogIoSkinDlg::OnBnClickedBtnLogIoSearch()
{
	InitializeSpread(0, TRUE);
}

void CLogIoSkinDlg::FillSpreadColumn(CStringArray& strCols)
{
	//m_spdLog.SetBackColor(WHITE);
	//int nColCnt = strCols.GetSize();
	//m_spdLog.SetMaxCols(nColCnt);
	//CString strValue = _T("");
	//for(int nIdxCol = 0; nIdxCol < nColCnt; nIdxCol++)
	//{
	//	strValue.Format(_T("%s\r"), strCols.GetAt(nIdxCol));
	//	m_spdLog.SetText(nIdxCol + 1, 0, variant_t(strValue));
	//	m_spdLog.put_ColWidth(nIdxCol + 1, strValue.GetLength() * 1.5);
	//}
	//m_spdLog.put_ColWidth(2, 45);
}


////////SPREAD 값채우기
void CLogIoSkinDlg::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	//정렬 가능
	//m_pSpreadMain.SetUserColAction(1);
	m_SpreadSheet.PrepareLoadSpread();

	//m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);
	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_lblSpdMainCnt.SetWindowText(CConvert::ToString(nRowCnt));
	m_SpreadSheet.FinishLoadSpread();

}

void CLogIoSkinDlg::SetMaxRows(int pRowCnt)
{
	//m_pSpreadMain.SetMaxRows(pRowCnt);
}

void CLogIoSkinDlg::SetColWidth(int pColCnt, int pColSize)
{
	//m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

int CLogIoSkinDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
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
		// [LGLS 2026-08-01] 한글 헤더는 글자수 폭으로는 잘린다(데이터 폭 계산과 동일 배율)
		int nSize = (int)(arrColRename[nIdxCol].GetLength() * 2);

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

void CLogIoSkinDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

int CLogIoSkinDlg::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
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
				int nPadLen = CLib::DispLen(strColValue) + 4;
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

void CLogIoSkinDlg::SetMaxCols(int pMaxCol)
{
	//m_pSpreadMain.SetMaxCols(pMaxCol);
}

CString CLogIoSkinDlg::GetQrySelect_Main(int nRowCheck, BOOL bSearch)
{
	if(m_pDoc == NULL){ return _T(""); };
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	CString strWH_TYP, strJOB_TYP;
	CString strLUGG_NO, strTRACK_START, strTRACK_DEST, strSTART_LOCATION, strDEST_LOCATION;
	CString strdtFrom, strdtTo;

	CTime tDATE_FROM, tDATE_TO, tTIME_FROM, tTIME_TO;

	strWH_TYP = CLib::GetComboKey(m_cbxIOLogWhTyp);
	strJOB_TYP = CLib::GetComboKey(m_cbxJobTyp);

	m_edtLuggNo.GetWindowText(strLUGG_NO);
	m_edtFromTrack.GetWindowText(strTRACK_START);
	m_edtToTrack.GetWindowText(strTRACK_DEST);
	m_edtFromLocation.GetWindowText(strSTART_LOCATION);
	m_edtToLocation.GetWindowText(strDEST_LOCATION);
	

	m_dtpFrom.GetTime(tDATE_FROM);
	m_dtpFromTime.GetTime(tTIME_FROM);
	m_dtpTo.GetTime(tDATE_TO);
	m_dtpToTime.GetTime(tTIME_TO);

	strWH_TYP.Trim();
	strLUGG_NO.Trim();
	strTRACK_START.Trim();
	strTRACK_DEST.Trim();
	strSTART_LOCATION.Trim();
	strDEST_LOCATION.Trim();
	strJOB_TYP.Trim();


	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tDATE_FROM, tTIME_FROM);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);

	
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

	strSql += CRLF + _T(" SELECT CCD_WH_TYP.CCD_NM_KOR AS WH_TYP  ");
	strSql += CRLF + _T(" 	  , HIS.LUGG_NO AS LUGG_NO  ");
	strSql += CRLF + _T(" 	  , CCD_JOB_TYP.CCD_NM_KOR AS JOB_TYP  ");
	strSql += CRLF + _T(" 	  , ") + m_pDoc->NVL + _T("(HIS.START_POS, '0') AS START_POS  ");
	strSql += CRLF + _T(" 	  , ") + m_pDoc->NVL + _T("(HIS.START_LOCATION, '00-000-00') AS START_LOCATION  ");
	strSql += CRLF + _T(" 	  , ") + m_pDoc->NVL + _T("(HIS.DEST_POS, '0') AS DEST_POS  ");
	strSql += CRLF + _T(" 	  , ") + m_pDoc->NVL + _T("(HIS.DEST_LOCATION, '00-000-00') AS DEST_LOCATION  ");
	// [LGLS 2026-08-01] 작업상태를 "[코드]상태명" 으로 표시 (예: [29] SC 구동완료)
	strSql += CRLF + _T(" 	  , '[' + HIS.JOB_STATUS + '] ' + ") + m_pDoc->NVL + _T("(CCD_JOB_STATUS.CCD_NM_KOR, HIS.JOB_STATUS) AS JOB_STATUS  ");
	strSql += CRLF + _T(" 	  , ") + m_pDoc->NVL + _T("(HIS.LOT_NO, '') AS LOT_NO  ");
	strSql += CRLF + _T(" 	  , ") + m_pDoc->NVL + _T("(HIS.PRODUCT_ID, '') AS PRODUCT_ID  ");
	strSql += CRLF + _T(" 	  , HIS.JOB_PRIORITY AS JOB_PRIORITY	 ");
	strSql += CRLF + _T(" 	  , HIS.INS_DT AS INS_DT  ");
	strSql += CRLF + _T(" 	  , HIS.UPD_DT AS UPD_DT  ");	// [LGLS] 수정시간(상태변경 순서 구분용)
	strSql += CRLF + _T("   FROM JOB_MST_HIS HIS LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP  ");
	strSql += CRLF + _T("   									 ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP'  ");
	strSql += CRLF + _T("   									AND CCD_JOB_TYP.CCD_CD = HIS.JOB_TYP   ");
	strSql += CRLF + _T("   					    LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP  ");
	strSql += CRLF + _T("   					                 ON CCD_WH_TYP.CDX_CD = 'WH_TYP'  ");
	strSql += CRLF + _T("   					                AND CCD_WH_TYP.CCD_CD = HIS.WH_TYP  ");
	strSql += CRLF + _T("   					    LEFT OUTER JOIN COMMON_CODE CCD_JOB_STATUS  ");
	strSql += CRLF + _T("   					                 ON CCD_JOB_STATUS.CDX_CD = 'JOB_STATUS'  ");
	strSql += CRLF + _T("   					                AND CCD_JOB_STATUS.CCD_CD = HIS.JOB_STATUS  ");
	strSql += CRLF + _T("  WHERE (CONVERT(VARCHAR(8),HIS.INS_DT,112) + REPLACE(CONVERT(VARCHAR(8),HIS.INS_DT,108),':','')) BETWEEN '") + strdtFrom + _T("' AND '") + strdtTo + _T("'");	// [LGLS]

	if ((strJOB_TYP != _T("")) && (strJOB_TYP != _T("ALL")))
	{
		strSql += CRLF + _T("					     AND HIS.JOB_TYP = '") + strJOB_TYP + _T("'");
	}
	if (strWH_TYP != _T(""))
	{
		strSql += CRLF + _T("					 AND HIS.WH_TYP = '") + strWH_TYP + _T("'");
	}
	if (strLUGG_NO != _T(""))
	{
		strSql += CRLF + _T("					 AND HIS.LUGG_NO LIKE '%") + strLUGG_NO + _T("%'");
	}
	// [LGLS 2026-08-01] 적재 용기(LOT_NO) / 제품 정보(PRODUCT_ID) 조회 조건
	{
		CString strLOT_NO, strPRODUCT_ID;
		GetDlgItemText(IDC_LGLS_IO_LOT_EDT, strLOT_NO);     strLOT_NO.Trim();
		GetDlgItemText(IDC_LGLS_IO_PRD_EDT, strPRODUCT_ID); strPRODUCT_ID.Trim();
		if (strLOT_NO != _T(""))
			strSql += CRLF + _T("					 AND HIS.LOT_NO LIKE '%") + strLOT_NO + _T("%'");
		if (strPRODUCT_ID != _T(""))
			strSql += CRLF + _T("					 AND HIS.PRODUCT_ID LIKE '%") + strPRODUCT_ID + _T("%'");
	}
	if (strTRACK_START != _T(""))
	{
		strSql += CRLF + _T("					 AND HIS.START_POS = '") + strTRACK_START + _T("'");
	}
	if (strTRACK_DEST != _T(""))
	{
		strSql += CRLF + _T("					 AND HIS.DEST_POS = '") + strTRACK_DEST + _T("'");
	}
	if (strSTART_LOCATION != _T(""))
	{
		strSql += CRLF + _T("					 AND HIS.START_LOCATION = '") + strSTART_LOCATION + _T("'");
	}
	if (strDEST_LOCATION != _T(""))
	{
		strSql += CRLF + _T("					 AND HIS.DEST_LOCATION = '") + strDEST_LOCATION + _T("'");
	}
#if ORACLE
	strSql += CRLF + _T(" AND ROWNUM <=	") + strNextRowCnt;
#endif

	strSql += CRLF + _T(" ORDER BY HIS.UPD_DT DESC, HIS.LUGG_NO DESC  ");
#if POSTGRESQL
	if (strRowCnt != _T("ALL"))
		strSql += CRLF + _T(" LIMIT	") + strNextRowCnt;
#elif MSSQL
	if (strRowCnt != _T("ALL"))	// [LGLS] SQL2008: inject TOP n instead of LIMIT
		CLib::ApplyTopN(strSql, strNextRowCnt);	// [LGLS]
#endif

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CLogIoSkinDlg::OnBnClickedPreRow()
{
	InitializeSpread(-1, TRUE);
}
void CLogIoSkinDlg::OnBnClickedNextRow()
{
	InitializeSpread(1, TRUE);
}



// [LGLS 2026-08-01] 런타임 생성 조회 라벨은 대화상자 배경이 비치도록 투명 처리한다.
//   (rc 라벨은 GetDlgItem 이 임시 CWnd 를 주어 기본 처리로 잘 그려지지만,
//    직접 만든 라벨은 기본 브러시로 칠해져 네모 배경이 보인다.)
HBRUSH CLogIoSkinDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	int nId = pWnd->GetDlgCtrlID();
	if (nId == IDC_LGLS_IO_LOT_LBL || nId == IDC_LGLS_IO_PRD_LBL)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return CSkinDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
