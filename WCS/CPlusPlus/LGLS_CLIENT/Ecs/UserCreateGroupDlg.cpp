// UserCreateGroupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "UserCreateGroupDlg.h"
#include "afxdialogex.h"


// CUserCreateGroupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserCreateGroupDlg, CSkinDialog)

CUserCreateGroupDlg::CUserCreateGroupDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CUserCreateGroupDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CUserCreateGroupDlg::CUserCreateGroupDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CUserCreateGroupDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
}

CUserCreateGroupDlg::~CUserCreateGroupDlg()
{
	this->DestroyWindow();
}


void CUserCreateGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_USER_CREATEGROUP_OK, m_btnOk);
	DDX_Control(pDX, ID_USER_CREATEGROUP_CANCEL, m_btnCancel);

	DDX_Control(pDX, IDC_LBL_USERGRP_GRP_CD,   m_lblGrpCd);
	DDX_Control(pDX, IDC_LBL_USERGRP_GRP_DESC, m_lblGrpDesc);
	DDX_Control(pDX, IDC_LBL_USERGRP_REMARKS,   m_lblRemarks);
	DDX_Control(pDX, IDC_EDIT_USERGRP_GRP_CD,   m_edtGrpCd);
	DDX_Control(pDX, IDC_EDIT_USERGRP_GRP_DESC, m_edtGrpDesc);
	DDX_Control(pDX, IDC_EDIT_USERGRP_REMARKS,  m_edtRemarks);

}

BEGIN_MESSAGE_MAP(CUserCreateGroupDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_USER_CREATEGROUP_OK, &CUserCreateGroupDlg::OnBnClickedUserCreategroupOk)
	ON_BN_CLICKED(ID_USER_CREATEGROUP_CANCEL, &CUserCreateGroupDlg::OnBnClickedUserCreategroupCancel)
END_MESSAGE_MAP()

BOOL CUserCreateGroupDlg::OnInitDialog()
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

	SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("그룹관리"), m_pDoc->m_enLang));

	InitializeControlLanguage();

	Invalidate(TRUE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CUserCreateGroupDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}


HCURSOR CUserCreateGroupDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUserCreateGroupDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CUserCreateGroupDlg::RelocationControls()
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


BOOL CUserCreateGroupDlg::PreTranslateMessage(MSG* pMsg)
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

void CUserCreateGroupDlg::InitializeControlLanguage()
{
	m_lblGrpCd.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("그룹코드"), m_pDoc->m_enLang));
	m_lblGrpDesc.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("그룹설명"), m_pDoc->m_enLang));
	m_lblRemarks.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("비고"), m_pDoc->m_enLang));

	m_btnOk.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("확인"), m_pDoc->m_enLang));
	m_btnCancel.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("취소"), m_pDoc->m_enLang));
}
void CUserCreateGroupDlg::OnBnClickedUserCreategroupOk()
{

	LPTSTR lptstrGrpCd = new TCHAR[100];
	LPTSTR lptstrGrpDesc = new TCHAR[100];
	LPTSTR lptstrRemarks = new TCHAR[100];


	m_edtGrpCd.GetWindowText(lptstrGrpDesc, 20);
	m_edtGrpDesc.GetWindowText(lptstrGrpDesc, 20);
	m_edtRemarks.GetWindowText(lptstrRemarks, 20);


	CString strGrpCd	= lptstrGrpCd;
	CString strGrpDesc  = lptstrGrpDesc;
	CString strRemarks	= lptstrRemarks;	


	delete	lptstrGrpCd;
	delete  lptstrGrpDesc;
	delete	lptstrRemarks;

	if( strGrpCd.GetLength() <= 1 ||
		strGrpDesc.GetLength() <= 1 ||
		strRemarks.GetLength() <= 1 )
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}
	CString strSql = _T("");
	CStringList strList_GRP_CD;

	strSql.Format(_T(" SELECT *")
		_T("   FROM USER_GRP")
		_T("  WHERE GRP_CD = '%s'"), strGrpCd);

	int nCntSel = 0;// m_pDB->SelectQry(strSql, strList_GRP_CD);
	if(nCntSel >= 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("DEBUG :: GRP_CD 중복!")));
		return;
	}
	if(nCntSel < 0)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("DEBUG :: DB ERROR")));
		return;
	}

	
	strSql.Format(_T(" INSERT INTO USER_MST ( GRP_CD \n")
			  _T("                      , GRP_DESC \n ")
			  _T("                      , INS_DT \n")
			  _T("                      , INS_USER_ID \n")
			  _T("                      , UPD_DT \n ")
			  _T("                      , UPD_USER_ID \n")
			  _T("                      , REMARKS ) ")
	          _T("                 VALUES ('%s' \n")
			  _T("                       , '%s' \n")
			  _T("                       , ") + m_pDoc->SYSDATE + _T(" \n")
			  _T("                       , '%s' \n")
			  _T("                       , ") + m_pDoc->SYSDATE + _T(" \n")
			  _T("                       , '%s' \n")
			  _T("                       , '%s')"), strGrpCd, strGrpDesc, m_pDoc->m_strId, m_pDoc->m_strId,strRemarks);
	


	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}

	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
}


void CUserCreateGroupDlg::OnBnClickedUserCreategroupCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSkinDialog::OnCancel();
}
