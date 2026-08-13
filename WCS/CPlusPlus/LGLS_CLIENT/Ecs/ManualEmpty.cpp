// ManualEmpty.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"
#include "ManualEmpty.h"

// CManualEmpty 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualEmpty, CSkinDialog)

CManualEmpty::CManualEmpty(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CManualEmpty::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CManualEmpty::CManualEmpty(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent)
	: CSkinDialog(CManualEmpty::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pDB = pDb;
}

CManualEmpty::~CManualEmpty()
{
	m_pDoc->m_pManualJob = NULL;
	this->DestroyWindow();
}

void CManualEmpty::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LBL_MANUAL_EMPTY_WH_TYP, m_lblManualEmptyWhTyp);
	DDX_Control(pDX, IDC_LBL_MANUAL_EMPTY_KIND, m_lblManualEmptyKind);
	DDX_Control(pDX, IDC_LBL_MANUAL_EMPTY_START_POS, m_lblManualEmptyStartPos);
	DDX_Control(pDX, IDC_LBL_MANUAL_EMPTY_DEST_POS, m_lblManualEmptyDestPos);
	DDX_Control(pDX, IDC_COMBO_MANUAL_EMPTY_WH_TYP, m_cbxManualEmptyWhTyp);
	DDX_Control(pDX, IDC_GRP_MANUAL_EMPTY_SEARCH, m_grpManualEmptySearch);
	DDX_Control(pDX, IDC_COMBO_MANUAL_EMPTY_KIND, m_cbxManualEmptyKind);
	DDX_Control(pDX, IDC_COMBO_MANUAL_EMPTY_START_POS, m_cbxManualEmptyStartPos);
	DDX_Control(pDX, IDC_COMBO_MANUAL_EMPTY_DEST_POS, m_cbxManualEmptyDestPos);
	DDX_Control(pDX, IDC_BTN_MANUAL_EMPTY_CLEAR, m_btnManualEmptyClear);
	DDX_Control(pDX, IDC_BTN_MANUAL_EMPTY_INSERT, m_btnManualEmptyInsert);
}


BEGIN_MESSAGE_MAP(CManualEmpty, CSkinDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_MANUAL_EMPTY_CLEAR, &CManualEmpty::OnBnClickedBtnManualEmptyClear)
	ON_BN_CLICKED(IDC_BTN_MANUAL_EMPTY_INSERT, &CManualEmpty::OnBnClickedBtnManualEmptyInsert)
	ON_CBN_SELCHANGE(IDC_COMBO_MANUAL_EMPTY_KIND, &CManualEmpty::OnCbnSelchangeComboManualEmptyKind)
END_MESSAGE_MAP()


// CManualEmpty 메시지 처리기입니다.


BOOL CManualEmpty::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn, 0);

	//CLib::SetBindCombo_DEST_POS_DEF(m_cbxManualJobStartPos, m_pDoc);
	//CLib::SetBindCombo_DEST_POS_DEF(m_cbxManualJobDestPos, m_pDoc);

	CLib::BindCombo(m_cbxManualEmptyWhTyp, _T("WH_TYP"), m_pDoc,(int)pEn, FALSE);
	CLib::BindCombo(m_cbxManualEmptyKind, _T("KIND"), m_pDoc,(int)pEn, TRUE);
	CLib::BindCombo_STN_KIND(m_cbxManualEmptyStartPos, _T("'108','149'"), m_pDoc,(int)pEn, TRUE);
	CLib::BindCombo_STN_KIND(m_cbxManualEmptyDestPos, _T("'108','149'"), m_pDoc,(int)pEn, TRUE);

	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		//RelocationControls();
		RedrawImage();
		RenameResource(pEn);

		m_bInitialized = TRUE;		
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CManualEmpty::OnClose()
{
	m_pDoc->m_pManualEmpty = NULL;
	delete this;
}


void CManualEmpty::OnBnClickedBtnManualEmptyClear()
{
	DataClear();
}

void CManualEmpty::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_emptymanual\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_emptymanual\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;


	m_btnManualEmptyInsert.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnManualEmptyInsert.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);
	m_btnManualEmptyInsert.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnManualEmptyInsert.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnManualEmptyClear.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnManualEmptyClear.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cut"), strExtension)), NULL, 5, 5);
	m_btnManualEmptyClear.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnManualEmptyClear.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);
}

void CManualEmpty::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_emptymanual\\"), _T("dlg_emptymanual"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_emptymanual\\"), _T("dlg_emptymanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MANUAL_EMPTY_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_emptymanual\\"), _T("dlg_emptymanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("kind"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MANUAL_EMPTY_KIND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_emptymanual\\"), _T("dlg_emptymanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MANUAL_EMPTY_START_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_emptymanual\\"), _T("dlg_emptymanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_MANUAL_EMPTY_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_emptymanual\\"), _T("dlg_emptymanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("insert"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_MANUAL_EMPTY_INSERT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_emptymanual\\"), _T("dlg_emptymanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("clear"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_MANUAL_EMPTY_CLEAR, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_emptymanual\\"), _T("dlg_emptymanual"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_MANUAL_EMPTY_SEARCH, strValue);

}

void CManualEmpty::DataClear()
{
	m_cbxManualEmptyWhTyp.SetCurSel(0);
	m_cbxManualEmptyKind.SetCurSel(0);
	m_cbxManualEmptyStartPos.SetCurSel(0);
	m_cbxManualEmptyDestPos.SetCurSel(0);
	m_cbxManualEmptyStartPos.EnableWindow(TRUE);
	m_cbxManualEmptyDestPos.EnableWindow(TRUE);
}

void CManualEmpty::OnBnClickedBtnManualEmptyInsert()
{
	CString strWhTyp = _T("");
	CString strKind = _T("");
	CString strStartPos = _T("");
	CString strDestPos = _T("");
	CString strStn = _T("");

	UpdateData(TRUE);
	strWhTyp = m_pDoc->m_WH_TYP;
	strKind = m_cbxManualEmptyKind.GetItemKey(m_cbxManualEmptyKind.GetCurSel());
	strStartPos = m_cbxManualEmptyStartPos.GetItemKey(m_cbxManualEmptyStartPos.GetCurSel());
	strDestPos = m_cbxManualEmptyDestPos.GetItemKey(m_cbxManualEmptyDestPos.GetCurSel());

	//if (!m_pDoc->Permission(_T("CManualEmpty"), EXE_YN))
	//{
	//	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
	//	return;
	//}

	if(AfxMessageBox(m_pDoc->GetMsgLangDef(_T("공 PLT 작업을 생성하시겠습니까?")), MB_YESNO) == IDYES)
    {
		//작업구분
		if (strKind == _T("") || strKind == _T("ALL"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업구분을 선택해주세요.")));
			return ;
		}

		if (strKind == "1")
		{
			//출발지
			if (strStartPos == _T("") || strStartPos == _T("ALL"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출발지를 선택해주세요.")));
				return ;
			}
			strStn = strStartPos;
		}
		else
		{
			//도착지
			if (strDestPos == _T("") || strDestPos == _T("ALL"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지를 선택해주세요.")));
				return ;
			}
			strStn = strDestPos;
		}

		strWhTyp.Trim();
		strKind.Trim();
		strStn.Trim();

		CString CRLF = _T("\r\n");
		CString strSql = _T("");
		int nRowCnt = 0;
		CString strMessage = _T("");

		strSql.Format(_T("SELECT *					\n")
					  _T("  FROM HOST_EMPTY_PLT		\n")
					  _T(" WHERE WH_TYP = '%s'		\n")
					  _T("   AND KIND = '%s'		\n")
					  _T("   AND STN = '%s'			\n")
					  _T("   AND LUGG_NO = '0'		\n"), strWhTyp, strKind, strStn);
	
		_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
		CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

		pRsw->MoveFirst(); 

		delete pRsw;

		if (nRowCnt > 0)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업대는 이미 요청한 작업이 있습니다.")));
			return;
		}

		strSql = _T("");
		
		strSql.Format(_T(" INSERT INTO HOST_EMPTY_PLT     (  WH_TYP							\n")
				      _T("					                , KIND							\n")
				      _T("					                , STN							\n")
				      _T("					                , STATUS						\n")
				      _T("                                  , LUGG_NO						\n")
				      _T("                                  , INS_ID						\n")
				      _T("                                  , INS_DT						\n")
				      _T("                                  , UPD_ID						\n")
				      _T("                                  , UPD_DT)						\n")
				      _T("                            VALUES ('%s'							\n")
				      _T("                                   , '%s'							\n") 
				      _T("                                   , '%s'							\n")
				      _T("                                   , 'N'							\n") // N으로 생성
				      _T("                                   , '0'							\n") // 작업번호 0
				      _T("                                   , '%s'							\n")
				      _T("                                   , ") + m_pDoc->SYSDATE + _T("  \n")
				      _T("								     , NULL							\n")					
				      _T("                                   , NULL)						\n")  , strWhTyp, strKind, strStn, m_pDoc->m_strId);
		

		//m_pDoc->BeginTrans_DLG();

		BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if(isSuccess == FALSE || isSuccess < 0)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			//m_pDoc->RollbackTrans_DLG();
			return;
		}

		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("공 PLT 수동 지시 -> 작업구분 : %s , 스테이션 : %s"), strKind, strStn);
		
		if (!m_pDoc->GetQueryInsertClientLog(_T("CManualEmpty"), _T("0"), _T(""), _T(""), strLOG_MSG))
		{
			//m_pDoc->RollbackTrans_DLG();
			return;
		}

		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SUCCESS")));
		DataClear();
		//m_pDoc->CommitTrans_DLG();
	}
}


void CManualEmpty::OnCbnSelchangeComboManualEmptyKind()
{
	//EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	//SetFontNation((int)pEn);

	CString strKind =_T("");
	
	//UpdateData(TRUE);

	strKind = m_cbxManualEmptyKind.GetItemKey(m_cbxManualEmptyKind.GetCurSel());
	if (strKind == _T("1"))
	{
		m_cbxManualEmptyStartPos.EnableWindow(TRUE);
		m_cbxManualEmptyDestPos.EnableWindow(FALSE);
	}
	else if (strKind == _T("2"))
	{
		m_cbxManualEmptyStartPos.EnableWindow(FALSE);
		m_cbxManualEmptyDestPos.EnableWindow(TRUE);
	}
	else
	{
		m_cbxManualEmptyStartPos.EnableWindow(TRUE);
		m_cbxManualEmptyDestPos.EnableWindow(TRUE);
	}
}
