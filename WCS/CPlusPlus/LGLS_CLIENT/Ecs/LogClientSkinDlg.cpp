// LogClientSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "LogClientSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CLogClientSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogClientSkinDlg, CSkinDialog)

CLogClientSkinDlg::CLogClientSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogClientSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CLogClientSkinDlg::CLogClientSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CLogClientSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}


CLogClientSkinDlg::~CLogClientSkinDlg()
{
	m_pDoc->m_pLogClientSkinDlg = NULL;
	this->DestroyWindow();
}

BOOL CLogClientSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);

}

void CLogClientSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GRP_LOG_CLIENT_SEARCH,						 m_grpClientSearch);
	DDX_Control(pDX, IDC_GRP_LOG_CLIENT_SEARCH2,					 m_grpClientSearch2);
	//DDX_Control(pDX, IDC_SPD_LOG_CLIENT,							 m_pSpreadMain);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_RESULT,						 m_lblClientLogResult);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_CNT,						 m_lblSpdMainCnt);
	DDX_Control(pDX, IDC_LBL_CLIENT_LOG_WH_TYP,			             m_lblClientLogWhTyp);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_TRAY_BOTTOM,	             m_lblClientLogBcrBottom);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_BCR_TOP,		             m_lblClientLogBcrTop);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_LUGG_NO,		             m_lblClientLogLuggNo);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_USER_ID,		             m_lblClientLogUserId);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_USER_IP,		             m_lblClientLogUserIp);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_REMARK,			             m_lblClientLogRemark);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_REMARK2,			         m_lblClientLogRemark3);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_REMARK3,			         m_lblClientLogRemark2);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_FROM_DATE,		             m_lblClientLogInsFrDt);
	DDX_Control(pDX, IDC_LBL_LOG_CLIENT_TO_DATE,		             m_lblClientLogInsToDt);
	DDX_Control(pDX, IDC_EDT_LOG_CLIENT_BCR_BOTTOM,		             m_edtClientLogBcrBottom);
	DDX_Control(pDX, IDC_EDT_LOG_CLIENT_TRAY_TOP,		             m_edtClientLogBcrTop);
	DDX_Control(pDX, IDC_EDT_LOG_CLIENT_LUGG_NO,					 m_edtClientLogLuggNo);
	DDX_Control(pDX, IDC_EDT_LOG_CLINET_REMARK,						 m_edtClientLogMessage);
	DDX_Control(pDX, IDC_EDT_LOG_CLINET_REMARK2,					 m_edtClientLogMessage2);
	DDX_Control(pDX, IDC_EDT_LOG_CLINET_REMARK3,					 m_edtClientLogMessage3);
	DDX_Control(pDX, IDC_EDT_LOG_CLIENT_USER_ID,					 m_edtClientLogUserId);
	DDX_Control(pDX, IDC_EDT_LOG_CLIENT_USER_IP,					 m_edtClientLogUserIp);
	DDX_Control(pDX, IDC_BTN_LOG_CLINET_SEARCH,						 m_btnClientLogSearch);
	DDX_Control(pDX, IDC_CBX_CLINET_LOG_WH_TYP,						 m_cbxClientLogWhTyp);
	DDX_Control(pDX, IDC_DTP_LOG_CLIENT_FROM_DATE,					 m_dtpFrom);
	DDX_Control(pDX, IDC_DTP_LOG_CLIENT_FROM_TIME,					 m_dtpFromTime);
	DDX_Control(pDX, IDC_DTP_LOG_CLIENT_TO_DATE,					 m_dtpTo);
	DDX_Control(pDX, IDC_DTP_LOG_CLIENT_TO_TIME,					 m_dtpToTime);
	DDX_Control(pDX, IDC_BTN_PRE_CLIENT_LOG,					 m_btnPre);
	DDX_Control(pDX, IDC_BTN_NEXT_CLIENT_LOG,					 m_btnNext);
	DDX_Control(pDX, IDC_CBX_ROW_CNT_CLIENT_LOG,					 m_cbxRowCnt);


	DDX_Control(pDX, IDC_CBX_CLINET_LOG_PGR_NM, m_cbxClientLogPgrNm);
	DDX_Control(pDX, IDC_LBL_CLIENT_LOG_PGR_NM, m_lblClientLogPgrNm);
}


BEGIN_MESSAGE_MAP(CLogClientSkinDlg, CSkinDialog)
	ON_MESSAGE(SSM_CLICK, &CLogClientSkinDlg::OnLglsHdrSort)
	ON_BN_CLICKED(IDC_BTN_LOG_CLINET_SEARCH, &CLogClientSkinDlg::OnBnClickedBtnLogClinetSearch)
	ON_BN_CLICKED(IDC_BTN_PRE_CLIENT_LOG, &CLogClientSkinDlg::OnBnClickedPreRow)
	ON_BN_CLICKED(IDC_BTN_NEXT_CLIENT_LOG, &CLogClientSkinDlg::OnBnClickedNextRow)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// [LGLS] 그리드 헤더 클릭 정렬
LRESULT CLogClientSkinDlg::OnLglsHdrSort(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if(pCell != NULL && pCell->Row == 0 && pCell->Col >= 1)
		m_SpreadSheet.SortByHeaderColumn(pCell->Col);
	return 0;
}

BOOL CLogClientSkinDlg::OnInitDialog()
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
	InitializeResource(pEn);

	CLib::BindCombo(m_cbxClientLogWhTyp, _T("WH_TYP"),m_pDoc, (int)pEn, FALSE);
	CLib::BindCombo(m_cbxClientLogPgrNm, _T("CLIENT_PGR_NM"),m_pDoc, (int)pEn, TRUE);
	CLib::BindCombo(m_cbxRowCnt, _T("ROW_CNT"),m_pDoc, (int)pEn, FALSE);

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
	m_SpreadSheet.AddColHead(_T("추가 시간"), 19);
	m_SpreadSheet.AddColHead(_T("접속 IP"), 10);
	m_SpreadSheet.AddColHead(_T("화면"), 15);
	m_SpreadSheet.AddColHead(_T("작업 번호"), 9);
	m_SpreadSheet.AddColHead(_T("메시지"), 40);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

#pragma endregion

	InitializeSpread(0, TRUE);
	{ CString _tt; GetWindowText(_tt); if(_tt.Find(_T("[")) < 0) SetWindowText(_tt + _T(" [WCS_CLIENT_LOG]")); }	// [LGLS] 제목에 조회 테이블명
	return TRUE;  
}

HCURSOR CLogClientSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLogClientSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);
	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

void CLogClientSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CLogClientSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);
	
	int x=0, y=0;
	
	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE sizeWideLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_WIDELARGE);
	
	CRect rc2;
	m_btnClientLogSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnClientLogSearch.MoveWindow(rc2.left, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);

}
void CLogClientSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);
	
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\128x16\\");
	strExtension = _T(".png");
	
	m_btnClientLogSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnClientLogSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
}

void CLogClientSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{	//
	RenameResource(nEN_LANG);
	RedrawImage();
}

void CLogClientSkinDlg::RenameResource( EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_CLIENT_LOG_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("userid"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_USER_ID, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("userip"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_USER_IP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("luggno"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_LUGG_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("traybottom"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_TRAY_BOTTOM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("traytop"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_BCR_TOP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("insdatefr"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_FROM_DATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("insdateto"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_TO_DATE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("message1"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_REMARK, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("message2"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_REMARK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("message3"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_REMARK3, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_LOG_CLINET_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_LOG_CLIENT_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_LOG_CLIENT_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_clientlog\\"), _T("dlg_clientlog"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("result"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_LOG_CLIENT_RESULT, strValue);

}



void CLogClientSkinDlg::OnBnClickedBtnLogClinetSearch()
{
	InitializeSpread(0, TRUE);
}


void CLogClientSkinDlg::OnClose()
{
	m_pDoc->m_pLogClientSkinDlg = NULL;
	CSkinDialog::OnClose();
}



////////SPREAD 값채우기
void CLogClientSkinDlg::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	//정렬 가능
	//m_pSpreadMain.SetUserColAction(1);
	//m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);

	m_SpreadSheet.PrepareLoadSpread();
	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_lblSpdMainCnt.SetWindowText(CConvert::ToString(nRowCnt));
	m_SpreadSheet.FinishLoadSpread();

}

void CLogClientSkinDlg::SetMaxRows(int pRowCnt)
{
	//m_pSpreadMain.SetMaxRows(pRowCnt);
}

void CLogClientSkinDlg::SetColWidth(int pColCnt, int pColSize)
{
	//m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

int CLogClientSkinDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
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

void CLogClientSkinDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");

	//m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

int CLogClientSkinDlg::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
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

void CLogClientSkinDlg::SetMaxCols(int pMaxCol)
{
	//m_pSpreadMain.SetMaxCols(pMaxCol);
}

CString CLogClientSkinDlg::GetQrySelect_Main(int nRowCheck,BOOL bSearch)
{
	int nLANG = m_pDoc->m_enLang;
	CString strSql = _T("");
	CString CRLF = _T("\r\n");
	CString strSqlWhTypCommonCode = CLib::GetCommonCode(strSql, _T("WH_TYP"), m_pDoc, nLANG);
	CString strMessage1, strMessage2, strMessage3;
	CString strTRAY_BOTTOM, strTRAY_TOP, strLUGG_NO, strREQUEST_ID, strEQP_NO = _T("");
	CString strUserId, strUserIp;
	CString strdtFrom, strdtTo;
	CTime tDATE_FROM, tDATE_TO, tTIME_FROM, tTIME_TO;
	CString strWH_TYP = CLib::GetComboKey(m_cbxClientLogWhTyp);
	CString strPGR_NM = CLib::GetComboKey(m_cbxClientLogPgrNm);

	m_edtClientLogBcrBottom.GetWindowText(strTRAY_BOTTOM);
	m_edtClientLogBcrTop.GetWindowText(strTRAY_TOP);
	m_edtClientLogLuggNo.GetWindowText(strLUGG_NO);
	m_edtClientLogMessage.GetWindowText(strMessage1);
	m_edtClientLogMessage2.GetWindowText(strMessage2);
	m_edtClientLogMessage3.GetWindowText(strMessage3);
	m_edtClientLogUserId.GetWindowText(strUserId);
	m_edtClientLogUserIp.GetWindowText(strUserIp);

	m_dtpFrom.GetTime(tDATE_FROM);
	m_dtpFromTime.GetTime(tTIME_FROM);
	m_dtpTo.GetTime(tDATE_TO);
	m_dtpToTime.GetTime(tTIME_TO);

	strTRAY_BOTTOM.Trim();
	strTRAY_TOP.Trim();
	strLUGG_NO.Trim();
	strMessage1.Trim();
	strMessage2.Trim();
	strMessage3.Trim();
	strUserId.Trim();
	strUserIp.Trim();


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

	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tDATE_FROM, tTIME_FROM);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);

	strSql += CRLF + _T("SELECT ")+ m_pDoc->NVL + _T("(CD_WH_TYP.CCD_NM_KOR, WCL.WH_TYP) AS WH_TYP , ");
	strSql += CRLF + _T("		INS_DT AS INS_DT, ");
	strSql += CRLF + _T("		IP AS IP, ");
	strSql += CRLF + _T("		")+ m_pDoc->NVL + _T("(CD_PGR_NM.CCD_NM_KOR, WCL.WIN_ID) AS WIN_ID, ");
	strSql += CRLF + _T("		")+ m_pDoc->NVL + _T("(LUGG_NO, '0000') AS LUGG_NO, ");
	strSql += CRLF + _T("		")+ m_pDoc->NVL + _T("(MESSAGE, '') AS MESSAGE ");
	strSql += CRLF + _T("  FROM WCS_CLIENT_LOG WCL LEFT OUTER JOIN COMMON_CODE CD_WH_TYP ");
	strSql += CRLF + _T("                                      ON CD_WH_TYP.CDX_CD = 'WH_TYP' ");
	strSql += CRLF + _T("                                     AND CD_WH_TYP.CCD_CD = WCL.WH_TYP ");
	strSql += CRLF + _T("                          LEFT OUTER JOIN COMMON_CODE CD_PGR_NM ");
	strSql += CRLF + _T("                                      ON CD_PGR_NM.CDX_CD = 'CLIENT_PGR_NM' ");
	strSql += CRLF + _T("                                     AND CD_PGR_NM.CCD_CD = WCL.WIN_ID ");
	strSql += CRLF + _T(" WHERE WCL.WH_TYP =  ") + CLib::Quot(strWH_TYP);
	strSql += CRLF + _T("   AND (CONVERT(VARCHAR(8),WCL.INS_DT,112) + REPLACE(CONVERT(VARCHAR(8),WCL.INS_DT,108),':','')) BETWEEN '") + strdtFrom + _T("' AND '") + strdtTo + _T("'");	// [LGLS]

	if(strLUGG_NO != _T(""))
	{
		strSql += CRLF + _T("    AND LUGG_NO LIKE '%") + strLUGG_NO + _T("%'");
	}
	if(strPGR_NM != _T("") && strPGR_NM != _T("ALL"))
	{
		strSql += CRLF + _T("    AND WIN_ID =  ") + CLib::Quot(strPGR_NM);
	}
	if(strUserIp != _T(""))
	{
		strSql += CRLF + _T("    AND IP LIKE  '%") + strUserIp + _T("%'");
	}
	if(strMessage1 != _T(""))
	{
		strSql += CRLF + _T("    AND MESSAGE LIKE '%") + strMessage1 + _T("%'");
	}
#if ORACLE
	strSql += CRLF + _T(" AND ROWNUM <=	") + strNextRowCnt;
#endif
	strSql += CRLF + _T(" ORDER BY WCL.INS_DT DESC");
#if POSTGRESQL
	if (strRowCnt != _T("ALL"))
		strSql += CRLF + _T(" LIMIT	") + strNextRowCnt;
#elif MSSQL
	if (strRowCnt != _T("ALL"))	// [LGLS] SQL2008: inject TOP n instead of LIMIT
		CLib::ApplyTopN(strSql, strNextRowCnt);	// [LGLS]
#endif

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CLogClientSkinDlg::OnBnClickedPreRow()
{
	InitializeSpread(-1, TRUE);
}

void CLogClientSkinDlg::OnBnClickedNextRow() 
{
	InitializeSpread(1, TRUE);
}

void CLogClientSkinDlg::OnSetfocusCmbRowCnt()
{
	m_pPreRowCnt = 0;
}