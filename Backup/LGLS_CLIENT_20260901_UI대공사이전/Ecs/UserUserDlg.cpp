// UserUserDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Ecs.h"
#include "LogMesSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"
#include "UserUserDlg.h"


// CUserUserDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserUserDlg, CSkinDialog)

CUserUserDlg::CUserUserDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CUserUserDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CUserUserDlg::CUserUserDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CUserUserDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CUserUserDlg::~CUserUserDlg()
{
	m_pDoc->m_pUserUserDlg= NULL;
	this->DestroyWindow();
}


void CUserUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LBL_USER_USER_ID_S, m_lblUserId_S);
	DDX_Control(pDX, IDC_EDT_USER_USER_ID_S, m_edtUserId_S);
	DDX_Control(pDX, IDC_LBL_USER_USER_NM_S, m_lblUserNm_S);
	DDX_Control(pDX, IDC_EDT_USER_USER_NM_S, m_edtUserNm_S);
	DDX_Control(pDX, IDC_LBL_USER_USER_GRP_S,  m_lblGrpNm_S);
	DDX_Control(pDX, IDC_CBX_USER_USER_GRP_S,  m_cbxGrpCd_S);
	DDX_Control(pDX, IDC_LBL_USER_USER_REMARKS_S, m_lblRemarks_S);
	DDX_Control(pDX, IDC_EDT_USER_USER_REMARKS_S, m_edtRemarks_S);

	DDX_Control(pDX, IDC_LBL_USER_USER_ID_I, m_lblUserId_I);
	DDX_Control(pDX, IDC_EDT_USER_USER_ID_I, m_edtUserId_I);
	DDX_Control(pDX, IDC_LBL_USER_USER_NM_I, m_lblUserNm_I);
	DDX_Control(pDX, IDC_EDT_USER_USER_NM_I, m_edtUserNm_I);
	DDX_Control(pDX, IDC_LBL_USER_USER_GRP_I,  m_lblGrpNm_I);
	// [LGLS 2026-08-05] 등록행 그룹 콤보는 리소스에서 IDC_CBX_USER_USER_GRP_S2 로 바뀌어 있다
	//   (구 ID IDC_CBX_USER_USER_GRP_I 는 템플릿에 없어 DDX_Control 어설션이 났다).
	DDX_Control(pDX, IDC_CBX_USER_USER_GRP_S2,  m_cbxGrpCd_I);
	DDX_Control(pDX, IDC_LBL_USER_USER_REMARKS_I, m_lblRemarks_I);
	DDX_Control(pDX, IDC_EDT_USER_USER_REMARKS_I, m_edtRemarks_I);
	DDX_Control(pDX, IDC_LBL_USER_USER_PW_I, m_lblPW_I);
	DDX_Control(pDX, IDC_EDT_USER_USER_PW_I, m_edtPW_I);

	DDX_Control(pDX, ID_BTN_USER_USER_SEARCH, m_btnSearch);
	DDX_Control(pDX, ID_BTN_USER_USER_DELETE, m_btnDelete);
	DDX_Control(pDX, ID_BTN_USER_USER_INSERT, m_btnInsert);

	DDX_Control(pDX, IDC_SPD_USER_USER, m_pSpreadMain);
	DDX_Control(pDX, IDC_LBL_USER_MST_RESULT, m_lblSpdMainResult);
	DDX_Control(pDX, IDC_LBL_USER_MST_CNT, m_lblSpdMainCnt);

	DDX_Control(pDX, IDC_USER_USER_SEARCH, m_grp1);
	DDX_Control(pDX, IDC_USER_USER_INSERT, m_grp2);
	DDX_Control(pDX, IDC_USER_USER_SEARCH2, m_grp3);
}

BEGIN_MESSAGE_MAP(CUserUserDlg, CSkinDialog)
	ON_MESSAGE(SSM_CLICK, &CUserUserDlg::OnLglsHdrSort)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_BTN_USER_USER_SEARCH, &CUserUserDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(ID_BTN_USER_USER_DELETE, &CUserUserDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(ID_BTN_USER_USER_INSERT, &CUserUserDlg::OnBnClickedBtnInsert)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// [LGLS] 그리드 헤더 클릭 정렬
LRESULT CUserUserDlg::OnLglsHdrSort(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if(pCell != NULL && pCell->Row == 0 && pCell->Col >= 1)
		m_pSpreadMain.SortByHeaderColumn(pCell->Col);
	return 0;
}

BEGIN_EVENTSINK_MAP(CUserUserDlg, CSkinDialog)
	ON_EVENT(CUserUserDlg, IDC_SPD_USER_USER, 5, CUserUserDlg::ClickSpread, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CUserUserDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	m_nActiveRow = 0;
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;

	if( !m_bInitialized )
	{
		m_bInitialized = TRUE;


		/*m_btnSearch.SetIcon(Global.GetIcon(Global.ICO_SEARCH));
		m_btnDelete.SetIcon(Global.GetIcon(Global.ICO_JOBDELETE));*/

		HBITMAP hbitBg;
		hbitBg = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SPLASH_ECS));

		
		RelocationControls();
	}
	CLib::BindCombo(m_cbxGrpCd_S, _T("USER_GRP"), m_pDoc, (int)pEn, TRUE);
	CLib::BindCombo(m_cbxGrpCd_I, _T("USER_GRP"), m_pDoc, (int)pEn, FALSE);

	InitializeResource(pEn);
	//InitializeControlLanguage();
	InitializeSpread(0, TRUE);
	//InitializeSpread(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUserUserDlg::InitializeControlLanguage()
{
	m_lblSpdMainResult.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("조회건수:"), m_pDoc->m_enLang));

	m_btnDelete.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("삭제"), m_pDoc->m_enLang));
	m_btnSearch.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("조회"), m_pDoc->m_enLang));
	m_btnInsert.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("추가"), m_pDoc->m_enLang));

	m_lblUserId_S.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("유저ID"), m_pDoc->m_enLang));
	m_lblUserNm_S.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("유저명"), m_pDoc->m_enLang));
	m_lblGrpNm_S.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("그룹명"), m_pDoc->m_enLang));
	m_lblRemarks_S.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("비고"), m_pDoc->m_enLang));
	m_lblUserId_I.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("유저ID"), m_pDoc->m_enLang));
	m_lblUserNm_I.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("유저명"), m_pDoc->m_enLang));
	m_lblGrpNm_I.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("그룹명"), m_pDoc->m_enLang));
	m_lblRemarks_I.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("비고"), m_pDoc->m_enLang));
	m_lblPW_I.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("패스워드"), m_pDoc->m_enLang));


}

void CUserUserDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}


HCURSOR CUserUserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUserUserDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CUserUserDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RenameResource(nEN_LANG);
	RedrawImage();
}

void CUserUserDlg::RenameResource( EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");


	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("id"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USER_USER_ID_S, strValue);

	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("id"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USER_USER_ID_I, strValue);
	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("nm"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USER_USER_NM_S, strValue);
	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("nm"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USER_USER_NM_I, strValue);
	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("group"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USER_USER_GRP_S, strValue);
	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("group"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USER_USER_GRP_I, strValue);
	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("password"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USER_USER_PW_I, strValue);
	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("remark"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USER_USER_REMARKS_S, strValue);
	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("remark"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USER_USER_REMARKS_I, strValue);
	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(ID_BTN_USER_USER_SEARCH, strValue);
	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("insert"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(ID_BTN_USER_USER_INSERT, strValue);
	
		strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("delete"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(ID_BTN_USER_USER_DELETE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("result"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USER_MST_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_USER_USER_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("insert"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_USER_USER_INSERT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\"), _T("dlg_useruser"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_USER_USER_SEARCH2, strValue);

}

void CUserUserDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);

	m_btnSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);

	m_btnInsert.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnInsert.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("insert"), strExtension)), NULL, 5, 5);

	m_btnDelete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnDelete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);


}

void CUserUserDlg::RelocationControls()
{

	CRect rc;
	GetClientRect(&rc);
	int x=0, y=0;
	//TCHAR chrFileName[500];
	//GetModuleFileName(NULL, chrFileName, MAX_PATH);
	//CString strAppPath;
	//CString strAppPath2;
	//CString strExtension;
	//strAppPath.Format(_T("%s"),chrFileName);
	//strAppPath2.Format(_T("%s"),chrFileName);

	//strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\20x20\\");
	//strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_useruser\\16x16\\");
	//strExtension = _T(".png");


	//SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	//SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	//SIZE sizeWideLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_WIDELARGE);

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE sizeWideLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_WIDELARGE);


	CRect rc2;

	//m_btnSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	//m_btnSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
	m_btnSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnSearch.MoveWindow(rc2.left, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);

	//m_btnDelete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	//m_btnDelete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);
	m_btnDelete.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnDelete.MoveWindow(rc2.left, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);

	//m_btnInsert.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	//m_btnInsert.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("insert"), strExtension)), NULL, 5, 5);
	m_btnInsert.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnInsert.MoveWindow(rc2.left, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy); //sizeLarge
	
}


BOOL CUserUserDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)  
	{
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
	}
	return CSkinDialog::PreTranslateMessage(pMsg);
}


void CUserUserDlg::OnBnClickedBtnSearch()
{
	InitializeSpread(0, TRUE);
}


void CUserUserDlg::OnBnClickedBtnDelete()
{
	DeleteUserMst();
}

void CUserUserDlg::OnBnClickedBtnInsert()
{
	InsertUserMst();
}

void CUserUserDlg::InsertUserMst()
{

	if (!m_pDoc->Permission(_T("CUserUserDlg"), INS_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}


	CString strSql;
	CString strID, strName, strRemarks, strGroup, strPW;
	CString strMessage;
	CString strCURRENT_ID_LEV, strINSERT_ID_LEV;
	int nINSERT_ID_LEV, nCURRENT_ID_LEV;
	int nRowCnt;

	m_edtUserId_I.GetWindowText(strID);
	m_edtUserNm_I.GetWindowText(strName);
	m_edtPW_I.GetWindowText(strPW);
	m_edtRemarks_I.GetWindowText(strRemarks);
	strGroup = m_cbxGrpCd_I.GetItemKey(m_cbxGrpCd_I.GetCurSel());

	strID.Trim();
	strName.Trim();
	strPW.Trim();
	strRemarks.Trim();
	strGroup.Trim();

	if (strID == _T("")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("아이디를 입력하세요"))); return; }
	if (strName == _T("")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("이름을 입력하세요"))); return; }
	if (strPW == _T("")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("패스워드를 입력하세요"))); return; }

	strSql = _T("");
	strSql.Format(_T(" SELECT UM.USER_ID, UG.GRP_LEVEL \n")
		_T("   FROM USER_MST UM JOIN USER_GRP UG \n")
		_T("     ON UM.GRP_CD = UG.GRP_LEVEL \n")
		_T("	WHERE USER_ID IN ('%s','%s')   ") , strID, m_pDoc->m_strId);

	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp); 

	if(nRowCnt < 0)
	{
		if(pRsw != NULL)
		{
			delete pRsw;	
			return;
		}
	}
	else if(nRowCnt > 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("중복된 아이디입니다")));
		return;
	}
	else 
	{
		if (strID == m_pDoc->m_strId) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("로그인한 아이디와 동일합니다"))); return;}
	
		pRsw->MoveFirst();

		if (pRsw->GetItem(_T("USER_ID")) == m_pDoc->m_strId) { strCURRENT_ID_LEV = pRsw->GetItem(_T("GRP_LEVEL")); }
		else  { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("불가능한 상황입니다"))); return;}


		nCURRENT_ID_LEV = CConvert::ToInt(strCURRENT_ID_LEV);
		nINSERT_ID_LEV = CConvert::ToInt(strGroup);

		if (nINSERT_ID_LEV < nCURRENT_ID_LEV) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 부족합니다"))); return;}
	}

	//m_cbxGrpCd_I.GetWindowText(strGroup);

	CString strLOG_MSG = _T("");
	strLOG_MSG.Format(_T("사용자 관리 -> ID : %s , 생성"), strID);
	if (!m_pDoc->GetQueryInsertClientLog(_T("CUserUserDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		return;
	}

	strSql.Format(_T(" INSERT INTO USER_MST( USER_ID         	 \n")
	_T("				   , USER_PW        	 \n")
	_T("				   , USER_NM     	 \n")
	_T("				   , GRP_CD       	 \n")
	_T("				   , REMARKS           	 \n")
	_T("				   , INS_USER_ID            	 \n")
	_T("				   , INS_DT)            	 \n")
	_T("			VALUES ( '%s'		      	 \n")
	_T("				   , '%s'		      	 \n")
	_T("				   , '%s'		      	 \n")
	_T("				   , '%s'			      	 \n")
	_T("				   , '%s'		      	 \n")
	_T("				   , '%s'		      	 \n")
	_T("				   , ") + m_pDoc->SYSDATE + _T(") \n") , strID, strPW, strName, strGroup, strRemarks, m_pDoc->m_strId);


	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if (isSuccess == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));

	m_edtUserId_I.SetWindowText(_T(""));
	m_edtUserNm_I.SetWindowText(_T(""));
	m_edtPW_I.SetWindowText(_T(""));
	m_edtRemarks_I.SetWindowText(_T(""));
	m_cbxGrpCd_I.SetCurSel(0);

	InitializeSpread(0, TRUE);

}
void CUserUserDlg::DeleteUserMst()
{
	CString strSql = _T("");
	variant_t val;
	CString strID, strCURRENT_ID_LEV, strDELETE_ID_LEV;
	CString strMessage;
	int nDELETE_ID_LEV, nCURRENT_ID_LEV;
	int nRowCnt;
	

	if (!m_pDoc->Permission(_T("CUserUserDlg"), DEL_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	if (m_nActiveRow < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업을 선택하세요")));
		return;
	}


	m_pSpreadMain.GetText(1, m_nActiveRow, &val);
	strID = (LPCTSTR)(_bstr_t)val;


	CString strSpace = _T(" ");
	if(IDYES == AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 유저를 삭제 하시겠습니까?")) + strSpace + _T("[ UESR ID : ") + strID + _T(" ]"), MB_YESNO))
	{
		strSql = _T("");
		strSql.Format(_T(" SELECT UM.USER_ID, UG.GRP_LEVEL \n")
					  _T("   FROM USER_MST UM JOIN USER_GRP UG \n")
					  _T("     ON UM.GRP_CD = UG.GRP_LEVEL \n")
					  _T("	WHERE USER_ID IN ('%s','%s')   ") , strID, m_pDoc->m_strId);

		_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
		CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp); 

		if(nRowCnt < 0)
		{
			if(pRsw != NULL)
			{
				delete pRsw;	
				return;
			}
		}
		else if(nRowCnt > 2)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("불가능한 상황입니다")));
			return;
		}
		else if(nRowCnt == 1)
		{
			if (strID != m_pDoc->m_strId) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("불가능한 상황입니다"))); return;}
			//else { AfxMessageBox(m_pDoc->m_pLang->GetLangValue(_T("로그인한 아이디 삭제 불가"), m_pDoc->m_enLang)); return;}
		}
		else
		{
			pRsw->MoveFirst();
			
			if (pRsw->GetItem(_T("USER_ID")) == strID) { strDELETE_ID_LEV = pRsw->GetItem(_T("GRP_LEVEL")); }
			else  { strCURRENT_ID_LEV = pRsw->GetItem(_T("GRP_LEVEL")); }

			pRsw->MoveNext();
			
			if (pRsw->GetItem(_T("USER_ID")) == strID) { strDELETE_ID_LEV = pRsw->GetItem(_T("GRP_LEVEL")); }
			else  { strCURRENT_ID_LEV = pRsw->GetItem(_T("GRP_LEVEL")); }

			nCURRENT_ID_LEV = CConvert::ToInt(strCURRENT_ID_LEV);
			nDELETE_ID_LEV = CConvert::ToInt(strDELETE_ID_LEV);

			if (nDELETE_ID_LEV <= nCURRENT_ID_LEV) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 부족합니다"))); return;}
		}

		m_pDoc->BeginTrans_DLG();

		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("사용자 관리 -> ID : %s , 삭제"), strID);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CUserUserDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}

		strSql = _T("");
		strSql.Format(_T("DELETE FROM USER_MST WHERE USER_ID = '%s' "), strID);

		BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if(isSuccess == FALSE)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
			m_pDoc->RollbackTrans_DLG();
			return;
			
		}
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		if (strID == m_pDoc->m_strId) 
		{
			m_pDoc->m_strId = _T("");
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("로그인한 아이디 삭제. 프로그램을 종료합니다.")));
			AfxGetMainWnd()->PostMessageW(WM_CLOSE);
			return;
		}

		m_pDoc->CommitTrans_DLG();

		InitializeSpread(0, TRUE);

	}
}

////////SPREAD 값채우기
void CUserUserDlg::InitializeSpread(int nRowCheck, BOOL bSearch = FALSE)
{
	//정렬 가능
	m_pSpreadMain.SetUserColAction(1);

	m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);
	int nRowCnt = SetSpeadData(nRowCheck, bSearch);
	m_lblSpdMainCnt.SetWindowText(CConvert::ToString(nRowCnt));

}

void CUserUserDlg::SetMaxRows(int pRowCnt)
{
	m_pSpreadMain.SetMaxRows(pRowCnt);
}
 
void CUserUserDlg::SetColWidth(int pColCnt, int pColSize)
{
	m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

int CUserUserDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
{
	CLib::GetColumnName(pStrArrColName, pStrSql);
	int nStartCol = 0;
	nColIdx = pStrArrColName.GetCount();
	if (pStrArrColName[0] == _T("ROWNO"))
	{
		SetMaxCols(nColIdx - 1);
		nStartCol = 0;
	}
	else
	{
		nStartCol = 1;
		SetMaxCols(nColIdx);
	}

	CStringArray arrColRename;
	CLib::RenameColumn(pStrArrColName, m_pDoc->m_enLang, arrColRename);

	for(int nIdxCol = 0; nIdxCol < nColIdx ; nIdxCol++)
	{
		int nSize = arrColRename[nIdxCol].GetLength()*2;
		if (arrColRename[nIdxCol].Right(5) == _T("_HIDE"))
		{
			nSize = 0;
		}
		else if (arrColRename[nIdxCol] == _T("ROWNO"))
		{
			nSize = 5;
		}
		pStrArrColSize.Add(CConvert::ToString(nSize));
	}

	//SetColumnText(0, 0, _T(""));


	for(int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
	{
		CString asd = arrColRename[nIdxCol];

		if (arrColRename[nIdxCol] != _T("ROWNO"))
		{
			SetColumnText(nIdxCol-1+nStartCol, 0, arrColRename[nIdxCol]);
		}
	}


	return nStartCol;
}

void CUserUserDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == "")
		strColumnName = _T(" ");

	m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
}

int CUserUserDlg::SetSpeadData(int nRowCheck, BOOL bSearch = FALSE)
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
		//SetHeadColumn_VerChk(arrColName, arrColSize, nColSize, strSql);
		int nIdxStart = SetHeadColumn(arrColName, arrColSize, nColSize, strSql);

		SetMaxRows(nRowCnt);

		pRsw->MoveFirst();

		for( int nIdxRow = 1; nIdxRow <= nRowCnt; nIdxRow++ )
		{
			for (int nIdxCol = 0; nIdxCol < nColSize; nIdxCol++)
			{
				CString strColValue = pRsw->GetItem(arrColName[nIdxCol]);

				int nPreSize = CConvert::ToInt(arrColSize[nIdxCol]);
				if ((strColValue.GetLength()*1.5 > nPreSize) && (nPreSize != 0))
				{
					int nSize = strColValue.GetLength()*1.5;
					arrColSize[nIdxCol]= CConvert::ToString(nSize);
				}



				SetColumnText(nIdxCol-1+nIdxStart, nIdxRow, strColValue);
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
			/*
			if (nIdxCol == 1)
			SetColWidth(nIdxCol-1+nIdxStart, 110);
			else*/
				SetColWidth(nIdxCol-1+nIdxStart, nSize);
		}
	}
	return nRowCnt;
}

void CUserUserDlg::SetMaxCols(int pMaxCol)
{
	m_pSpreadMain.SetMaxCols(pMaxCol);
}

CString CUserUserDlg::GetQrySelect_Main(int nRowCheck, BOOL bSearch)
{
	CString strSql;
	CString strID, strName, strRemarks, strGroup;

	m_edtUserId_S.GetWindowText(strID);
	m_edtUserNm_S.GetWindowText(strName);
	m_edtRemarks_S.GetWindowText(strRemarks);
	strGroup = m_cbxGrpCd_S.GetItemKey(m_cbxGrpCd_S.GetCurSel());

	strID.Trim();
	strName.Trim();
	strRemarks.Trim();
	strGroup.Trim();

	
	strSql = _T("	        SELECT UM.USER_ID AS USER_ID, UM.USER_NM AS USER_NM				  \n");
	strSql += _T("	             , ") + m_pDoc->NVL + _T("(UG.GRP_CD, UM.GRP_CD) AS GRP_CD, UM.REMARKS AS REMARKS										      \n");
	strSql += _T("	             , ") + m_pDoc->NVL + _T("(UM.UPD_USER_ID, UM.INS_USER_ID) AS UPD_USER_ID \n");
	strSql += _T("				 , ") + m_pDoc->NVL + _T("(UM.UPD_DT, UM.INS_DT) AS UPD_DT    \n");
	strSql += _T("	          FROM USER_MST	UM LEFT JOIN USER_GRP UG																  \n");
	strSql += _T("	            ON UM.GRP_CD = UG.GRP_LEVEL																  \n");
	strSql += _T("	         WHERE 1 = 1																		  \n");
	if (strID != "")
	{
		strSql += _T("	       AND UM.USER_ID LIKE '%") + strID + _T("%'														  \n");
	}
	if (strName != "")
	{
		strSql += _T("	       AND UM.USER_NM LIKE '%") + strName + _T("%'														  \n");
	}
	if (strRemarks != "")
	{
		strSql += _T("	       AND UM.REMARKS LIKE '%") + strRemarks + _T("%'														  \n");
	}
	if (strGroup != "" && strGroup != _T("ALL"))
	{
		strSql += _T("	       AND UM.GRP_CD = '") + strGroup + _T("'														  \n");
	}


	return strSql;
}

void CUserUserDlg::ClickSpread(long Col, long Row) //셀클릭
{
	variant_t val;
	m_nActiveRow = Row;
}

void CUserUserDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pDoc->m_pUserUserDlg= NULL;
	delete this;
}
