// ManualLogin.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "ManualLogin.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CManualLogin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualLogin, CDialogEx)

CManualLogin::CManualLogin(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CManualLogin::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CManualLogin::CManualLogin(CEcsDoc* pDoc, CURMDBAccess* pDb, CWnd* pParent)
	: CSkinDialog(CManualLogin::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pDB = pDb;
}

CManualLogin::~CManualLogin()
{
	//m_pDoc->m_pManualLogin = NULL;
	//m_pDoc->m_blManualLogin = FALSE;
	this->DestroyWindow();
}

void CManualLogin::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_MANUAL_LOGIN_OK, m_btnManualLoginOk);
	DDX_Control(pDX, IDC_EDT_MANUAL_LOGIN_PW, m_edtManualLoginPw);
}


BEGIN_MESSAGE_MAP(CManualLogin, CSkinDialog)
	ON_BN_CLICKED(IDC_BTN_MANUAL_LOGIN_OK, &CManualLogin::OnBnClickedBtnManualLoginOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CManualLogin 메시지 처리기입니다.


void CManualLogin::OnBnClickedBtnManualLoginOk()
{
	CString strLoginPw = _T("");
	CString strSql = _T("");
	CString strPw = _T("");
	int nRowCnt;
	CString strMessage;

	//DB에서 미리 정의한 비밀번호 불러오기
	strSql = GetQry_Main();

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
    CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt < 0)
	{
		if(pRsw != NULL)
		{
			delete pRsw;	
		}
	}
	else if (nRowCnt == 0)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("비밀번호를 조회하지 못했습니다.")));
		return;
	}
	else
	{
		strPw = pRsw->GetItem(_T("CCD_CD"));

		m_edtManualLoginPw.GetWindowText(strLoginPw);

		strLoginPw.Trim();
		strPw.Trim();

		if (strLoginPw != strPw)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("비밀번호가 틀렸습니다.")));
			return;
		}

		m_pDoc->m_blManualLogin = TRUE;
		//m_pDoc->m_pManualLogin = NULL;
		CManualLogin::OnCancel();
		//delete this;

		if (m_pDoc->m_pManualJob == NULL)
		{
			m_pDoc->m_pManualJob = new CManualJob(m_pDoc, m_pDB);
			m_pDoc->m_pManualJob->Create(IDD_MANAUL_JOB);
			CRect MainRect;
			CRect Rect;
			CRect PosRect;
			::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
			m_pDoc->m_pManualJob->GetWindowRect(&Rect); 

			PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
			PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
			m_pDoc->m_pManualJob->SetWindowPos(&m_pDoc->m_pManualJob->wndTop, PosRect.left, PosRect.top, 
				Rect.Width(), Rect.Height(), 
				SWP_SHOWWINDOW);

		}
		::SetWindowPos(m_pDoc->m_pManualJob->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
		::SetWindowPos(m_pDoc->m_pManualJob->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
		//ShowWindow(m_pDoc->m_pManualJob->m_hWnd, SW_SHOWNORMAL);
	}
}


BOOL CManualLogin::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn, 0);

	CSkinDialog::SetFont(this->GetFont());

	m_edtManualLoginPw.SetWindowText(_T(""));

	if( !m_bInitialized )
	{
		RedrawImage();
		m_bInitialized = TRUE;	
	}

	return TRUE;
}


void CManualLogin::OnClose()
{
	m_edtManualLoginPw.SetWindowText(_T(""));
	CManualLogin::OnCancel();
	//m_pDoc->m_pManualLogin = NULL;
	//m_pDoc->m_blManualLogin = FALSE;
	//delete this;
}

CString CManualLogin::GetQry_Main()
{
	CString strSql = _T("");
	CString strTemp = _T("");															
	CString strEQP_TYP = _T("");
	CString strCONNECTED_YN = _T("");

	strEQP_TYP.Trim();
	strCONNECTED_YN.Trim();
	
	strSql.Format(_T(" SELECT CCD_CD 						\n")	
			  _T("       FROM COMMON_CODE					\n")	
			  _T("  WHERE WH_TYP = '%s'						 \n")
			  _T("    AND CDX_CD = 'MANUAL_LOGIN' "), m_pDoc->m_WH_TYP);			    
	
   return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

void CManualLogin::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_jobmanual\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;


	m_btnManualLoginOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnManualLoginOk.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("copy"), strExtension)), NULL, 5, 5);
	m_btnManualLoginOk.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnManualLoginOk.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

}
