// UserCreateUserDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "UserCreateUserDlg.h"
#include "afxdialogex.h"


// CUserCreateUserDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserCreateUserDlg, CSkinDialog)

CUserCreateUserDlg::CUserCreateUserDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CUserCreateUserDlg::IDD, pParent)
{

}

CUserCreateUserDlg::CUserCreateUserDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CUserCreateUserDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;	
}

CUserCreateUserDlg::~CUserCreateUserDlg()
{
	this->DestroyWindow();
}


void CUserCreateUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_USER_CREATERUSER_OK, m_btnOk);
	DDX_Control(pDX, ID_USER_CREATERUSER_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_USER_CREATEUSER_STATIC_USER_ID, m_lblUSER_ID);
	DDX_Control(pDX, IDC_USER_CREATEUSER_STATIC_USER_PW, m_lblUSER_PW);
	DDX_Control(pDX, IDC_USER_CREATEUSER_STATIC_USER_NM, m_lblUSER_NM);
	DDX_Control(pDX, IDC_USER_CREATEUSER_STATIC_GRP_CD, m_lblGRP_CD);
	DDX_Control(pDX, IDC_USER_CREATEUSER_STATIC_REMARKS, m_lblREMARKS);

	DDX_Control(pDX, IDC_USER_CREATEUSER_EDIT_USER_ID,	m_edtUSER_ID);
	DDX_Control(pDX, IDC_USER_CREATEUSER_EDIT_USER_PW,	m_edtUSER_PW);
	DDX_Control(pDX, IDC_USER_CREATEUSER_EDIT_USER_NM,	m_edtUSER_NM);
	DDX_Control(pDX, IDC_USER_CREATEUSER_COMBO_GRP_CD,	m_comboGRP_CD);
	DDX_Control(pDX, IDC_USER_CREATEUSER_EDIT_REMARK,	m_edtREMARKS);

}

BEGIN_MESSAGE_MAP(CUserCreateUserDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_USER_CREATERUSER_OK, &CUserCreateUserDlg::OnBnClickedUserCreateruserOk)
	ON_BN_CLICKED(ID_USER_CREATERUSER_CANCEL, &CUserCreateUserDlg::OnBnClickedUserCreateruserCancel)
END_MESSAGE_MAP()

BOOL CUserCreateUserDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	if( !m_bInitialized )
	{
		m_bInitialized = TRUE;

		m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
		m_btnOk.SetIcon(Global.GetIcon(IDX_ICON_OK, ICON16), NULL, 5);

		m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
		m_btnCancel.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);


		//IDC_LOGIN_PICTUREBOX
		HBITMAP hbitBg;
		hbitBg = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SPLASH_ECS));
		//m_bgPic.SetBitmap(hbitBg);
		RelocationControls();
	}

	SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("회원가입"), m_pDoc->m_enLang));

	InitializeControlLanguage();
	FillComboBox();

	Invalidate(TRUE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CUserCreateUserDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}


HCURSOR CUserCreateUserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUserCreateUserDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CUserCreateUserDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);

	x = rc.Width() - 20 - size.cx;
	y = rc.Height() - 10 - size.cy;
	m_btnCancel.MoveWindow(x, y, size.cx, size.cy, TRUE);

	x -= size.cx + 10;
	m_btnOk.MoveWindow(x, y, size.cx, size.cy, TRUE);
	m_btnOk.Invalidate();
}


BOOL CUserCreateUserDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)  
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			{
				OnBnClickedUserCreateruserOk();
				break;
			}
		case VK_ESCAPE:
			{
				OnBnClickedUserCreateruserCancel();
				break;
			}
		}
	}
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CUserCreateUserDlg::InitializeControlLanguage()
{
	m_lblUSER_ID.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("아이디"), m_pDoc->m_enLang));
	m_lblUSER_PW.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("패스워드"), m_pDoc->m_enLang));
	m_lblUSER_NM.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("유저명"), m_pDoc->m_enLang));
	m_lblGRP_CD.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("그룹명"), m_pDoc->m_enLang));
	m_lblREMARKS.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("메모"), m_pDoc->m_enLang));

 	m_btnOk.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("확인"), m_pDoc->m_enLang));
 	m_btnCancel.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("취소"), m_pDoc->m_enLang));
}

void CUserCreateUserDlg::FillComboBox()
{
	CString strSql = _T("");
	CStringList strList_GRP_CD;

	strSql.Format(_T(" SELECT GRP_CD ")
				  _T("	 FROM USER_GRP "));

	int nCntSel = 0;//m_pDB->SelectQry(strSql, strList_GRP_CD);
	if(nCntSel < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("DEBUG :: GRP_CD가 수집되지 않았습니다")));
		return;
	}

	int nLoopCnt = -1;
	for (POSITION pos = strList_GRP_CD.GetHeadPosition(); pos != NULL;)
	{
		nLoopCnt++;
		CString strTemp = strList_GRP_CD.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);

		if ( strTemp.IsEmpty() ) continue;

		if (StrFieldName == "GRP_CD")		
		{
			m_comboGRP_CD.InsertString(nLoopCnt, strValue);
		}		
	}
}



void CUserCreateUserDlg::OnBnClickedUserCreateruserOk()
{
	LPTSTR lptstrUSER_ID = new TCHAR[100];		
	LPTSTR lptstrUSER_PW = new TCHAR[100];  
	LPTSTR lptstrUSER_NM = new TCHAR[100];
	LPTSTR lptstrGRP_CD = new TCHAR[100];
	LPTSTR lptstrREMARK = new TCHAR[100];

	m_edtUSER_ID.GetWindowText(lptstrUSER_ID, 100);
	m_edtUSER_PW.GetWindowText(lptstrUSER_PW, 20);
	m_edtUSER_NM.GetWindowText(lptstrUSER_NM, 20);
	m_comboGRP_CD.GetLBText(m_comboGRP_CD.GetCurSel(), lptstrGRP_CD);
	m_edtREMARKS.GetWindowText(lptstrREMARK, 20);

	CString strUSER_ID	= lptstrUSER_ID;		
	CString strUSER_PW  = lptstrUSER_PW;  
	CString strUSER_NM  = lptstrUSER_NM;  
	CString strGRP_CD	= lptstrGRP_CD;
	CString strREMARK	= lptstrREMARK;	

	delete	lptstrUSER_ID;
	delete	lptstrUSER_PW;
	delete	lptstrUSER_NM;
	delete	lptstrGRP_CD;
	delete	lptstrREMARK;

	if( strUSER_ID.GetLength() <= 1 ||
		strUSER_PW.GetLength() <= 1 ||
		strGRP_CD.GetLength() <= 1 )
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}
	CString strSql = _T("");
	CStringList strList_GRP_CD;

	strSql.Format(_T(" SELECT *")
		          _T("   FROM USER_MST")
				  _T("  WHERE USER_ID = '%s'"), strUSER_ID);

	int nCntSel = 0;//m_pDB->SelectQry(strSql, strList_GRP_CD);
	if(nCntSel >= 1)
	{
		AfxMessageBox(_T("DEBUG :: GRP_CD가 수집되지 않았습니다"));
		return;
	}
	if(nCntSel < 0)
	{
		AfxMessageBox(_T("DEBUG :: DB ERROR"));
		return;
	}

	
	strSql.Format(_T(" INSERT INTO USER_MST ( USER_ID, USER_PW, USER_NM, GRP_CD, REMARKS, INS_USER_ID, INS_DT ) ")
		_T(" VALUES ('%s', '%s', '%s', '%s', '%s', '%s', ") + m_pDoc->SYSDATE + _T(")"), strUSER_ID, strUSER_PW, strUSER_NM, strGRP_CD, strREMARK, m_pDoc->m_strId);


	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}

	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
}


void CUserCreateUserDlg::OnBnClickedUserCreateruserCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSkinDialog::OnCancel();
}
