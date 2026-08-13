// RtvSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "RtvSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CRtvSkinDlg 대화 상자입니다.
IMPLEMENT_DYNAMIC(CRtvSkinDlg, CSkinDialog)


CRtvSkinDlg::CRtvSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CRtvSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pRTV_DATA = NULL;
}
CRtvSkinDlg::CRtvSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CRtvSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pRTV_DATA = NULL;
}

CRtvSkinDlg::~CRtvSkinDlg()
{
	m_pDoc->m_pRtvSkinDlg = NULL;
	this->DestroyWindow();
}

void CRtvSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	//label
	DDX_Control(pDX, IDC_LBL_RTV_JOB_NO,					m_lblLuggNo);
	DDX_Control(pDX, IDC_LBL_RTV_START_POS,					m_lblStartPos);						
	DDX_Control(pDX, IDC_LBL_RTV_DEST_POS,					m_lblDestPos);
	DDX_Control(pDX, IDC_LBL_RTV_JOB_TYP,					m_lblJobTyp);
	DDX_Control(pDX, IDC_LBL_RTV_MODE,					    m_lblRtvMode);
	DDX_Control(pDX, IDC_LBL_RTV_COMPLETE,					m_lblComplete);
	DDX_Control(pDX, IDC_LBL_RTV_PRODLOAD,					m_lblRtvProdLoad);
	DDX_Control(pDX, IDC_LBL_RTV_STATUS,					m_lblRtvStatus);
	DDX_Control(pDX, IDC_LBL_RTV_ACTIVE,					m_lblActive);
	DDX_Control(pDX, IDC_LBL_RTV_HORIZONTAL_POS,			m_lblHorizontalPos);
	DDX_Control(pDX, IDC_LBL_RTV_JISANG_MODE,				m_lblJisangMode);
	DDX_Control(pDX, IDC_LBL_RTV_JOB_JOB_NO,				m_lblJobLuggNo);
	DDX_Control(pDX, IDC_LBL_RTV_JOB_START_POS,				m_lblJobStartPos);
	DDX_Control(pDX, IDC_LBL_RTV_JOB_DEST_POS,				m_lblJobDestPos);
	DDX_Control(pDX, IDC_LBL_RTV_JOB_JOB_TYP,				m_lblJobJobTyp);
	//

	//Edit
	DDX_Control(pDX, IDC_EDIT_RTV_NO,						m_edtRtvNo);
	DDX_Control(pDX, IDC_EDIT_RTV_JOB_NO,					m_edtJobNo);
	DDX_Control(pDX, IDC_EDIT_RTV_JOB_NO2,					m_edtJobNo2);
	DDX_Control(pDX, IDC_EDT_RTV_MODE,						m_edtRtvMode);				
	DDX_Control(pDX, IDC_EDT_RTV_COMPLETE,					m_edtComplete);
	DDX_Control(pDX, IDC_EDT_RTV_PRODLOAD,					m_edtRtvProdLoad);
	DDX_Control(pDX, IDC_EDT_RTV_STATUS,					m_edtRtvStatus);
	DDX_Control(pDX, IDC_EDT_RTV_ACTIVE,					m_edtActive);
	DDX_Control(pDX, IDC_EDT_RTV_HORIZONTAL_POS,			m_edtHorizontalPos);
	DDX_Control(pDX, IDC_EDT_RTV_JISANG_MODE,				m_edtJisangMode); 
	DDX_Control(pDX, IDC_EDIT_RTV_JOB_JOB_NO,				m_edtJobJobNo);
	DDX_Control(pDX, IDC_EDIT_RTV_JOB_JOB_NO2,				m_edtJobJobNo2);
	DDX_Control(pDX, IDC_EDT_RTV_DIAGNOSIS,					m_edtDiagnosis);
	DDX_Control(pDX, IDC_EDT_RTV_SUSPEND,					m_edtRtvSuspend);
	//

	//Button
	DDX_Control(pDX, IDC_BTN_RTV_COMPLETE,					m_btnRtvComplete);
	DDX_Control(pDX, IDC_BTN_RTV_ESTOP,						m_btnRtvEstop);
	DDX_Control(pDX, IDC_BTN_RTV_ACTIVE,					m_btnRtvActive);		
	DDX_Control(pDX, IDC_BTN_RTV_STOP,						m_btnRtvStop);
	DDX_Control(pDX, IDC_BTN_RTV_RESET_ERROR,				m_btnRtvResetError);
	DDX_Control(pDX, IDC_BTN_RTV_DELETE,					m_btnRtvDelete);
	DDX_Control(pDX, IDC_BTN_RTV_CALL_TO_HOME,				m_btnRtvCallToHome);
	DDX_Control(pDX, IDC_BTN_RTV_MANUAL,					m_btnRtvManual);
	DDX_Control(pDX, IDC_BTN_RTV_SUSPEND,					m_btnRtvSuspend);
	//

	DDX_Control(pDX, IDC_CBX_RTV_START_POS,					m_cbxRtvStartPos);
	DDX_Control(pDX, IDC_CBX_RTV_START_POS2,				m_cbxRtvStartPos2);
	DDX_Control(pDX, IDC_CBX_RTV_DEST_POS,					m_cbxRtvDestPos);
	DDX_Control(pDX, IDC_CBX_RTV_DEST_POS2,					m_cbxRtvDestPos2);
	DDX_Control(pDX, IDC_CBX_RTV_JOB_TYP,					m_cbxRtvJobTyp);
	DDX_Control(pDX, IDC_CBX_RTV_JOB_TYP2,					m_cbxRtvJobTyp2);


	DDX_Control(pDX, IDC_CBX_RTV_JOB_START_POS,					m_cbxRtvJobStartPos);
	DDX_Control(pDX, IDC_CBX_RTV_JOB_START_POS2,				m_cbxRtvJobStartPos2);
	DDX_Control(pDX, IDC_CBX_RTV_JOB_DEST_POS,					m_cbxRtvJobDestPos);
	DDX_Control(pDX, IDC_CBX_RTV_JOB_DEST_POS2,					m_cbxRtvJobDestPos2);
	DDX_Control(pDX, IDC_CBX_RTV_JOB_JOB_TYP,					m_cbxRtvJobJobTyp);
	DDX_Control(pDX, IDC_CBX_RTV_JOB_JOB_TYP2,					m_cbxRtvJobJobTyp2);



	DDX_Control(pDX, IDC_GRP_RTV_RTV_STATUS,				m_grpRtvStatus);
	DDX_Control(pDX, IDC_GRP_RTV_RTV_STATUS_ITEM,			m_grpRtvStatusItem);
	DDX_Control(pDX, IDC_GRP_RTV_RTV_STATUS_FK1,			m_grpRtvStatusFk1);
	DDX_Control(pDX, IDC_GRP_RTV_RTV_STATUS_FK2,			m_grpRtvStatusFk2);
	DDX_Control(pDX, IDC_GRP_FK_FK_STATUS_COMMAND,			m_grpRtvStatusCommand);


	DDX_Control(pDX, IDC_GRP_FK_FK_STATUS_ITEM3,			m_grpRtvStatusItem3);
	DDX_Control(pDX, IDC_GRP_FK_FK_STATUS_RTV,				m_grpRtvStatusRtv);
	DDX_Control(pDX, IDC_GRP_RTV_JOB_STATUS,                m_grpRtvJobStatus);
	DDX_Control(pDX, IDC_GRP_RTV_JOB_STATUS_COMMAND,        m_grpRtvCommandRightBottom);
	DDX_Control(pDX, IDC_GRP_RTV_JOB_STATUS_ITEM,           m_grpRtvJobStatusItem);
	DDX_Control(pDX, IDC_GRP_RTV_JOB_STATUS_FK1,			m_grpRtvJobStatusFk1);
	DDX_Control(pDX, IDC_GRP_RTV_JOB_STATUS_FK2,			m_grpRtvJobStatusFk2);
	DDX_Control(pDX, IDC_GRP_RTV_STATUS_SENSOR,				m_grpRtvErrorInfomation);
	DDX_Control(pDX, IDC_GRP_RTV_SUSPEND,					m_grpRtvSuspend);

	DDX_Control(pDX, IDC_CHK_RTV_FORK1,						m_chkRtvFork1);
	DDX_Control(pDX, IDC_CHK_RTV_FORK2,						m_chkRtvFork2);
	DDX_Control(pDX, IDC_CHK_RTV_FORK12,					m_chkRtvFork1Fork2);

}

BEGIN_MESSAGE_MAP(CRtvSkinDlg, CSkinDialog)
		ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CRtvSkinDlg::OnMessagSwitch)
		ON_WM_CLOSE()
		ON_BN_CLICKED(IDC_BTN_RTV_COMPLETE, &CRtvSkinDlg::OnBnClickedBtnRtvComplete)
		ON_BN_CLICKED(IDC_BTN_RTV_ESTOP, &CRtvSkinDlg::OnBnClickedBtnRtvEstop)
		ON_BN_CLICKED(IDC_BTN_RTV_ACTIVE, &CRtvSkinDlg::OnBnClickedBtnRtvActive)
		ON_BN_CLICKED(IDC_BTN_RTV_STOP, &CRtvSkinDlg::OnBnClickedBtnRtvStop)
		ON_BN_CLICKED(IDC_BTN_RTV_RESET_ERROR, &CRtvSkinDlg::OnBnClickedBtnRtvResetError)
		ON_BN_CLICKED(IDC_BTN_RTV_DELETE, &CRtvSkinDlg::OnBnClickedBtnRtvDelete)
		ON_BN_CLICKED(IDC_BTN_RTV_CALL_TO_HOME, &CRtvSkinDlg::OnBnClickedBtnRtvCallToHome)
		ON_BN_CLICKED(IDC_BTN_RTV_SUSPEND, &CRtvSkinDlg::OnBnClickedBtnRtvSuspend)
		ON_BN_CLICKED(IDC_CHK_RTV_FORK1, &CRtvSkinDlg::OnBnClickedChkRtvFork1)
		ON_BN_CLICKED(IDC_CHK_RTV_FORK2, &CRtvSkinDlg::OnBnClickedChkRtvFork2)
		ON_BN_CLICKED(IDC_CHK_RTV_FORK12, &CRtvSkinDlg::OnBnClickedChkRtvFork12)
END_MESSAGE_MAP()


BOOL CRtvSkinDlg::OnInitDialog()
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
	m_strRtvFork = _T("0");

	InitializeResource(pEn);
	return TRUE;  // return TRUE  unless you set the focus to a control
}



BOOL CRtvSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CRtvSkinDlg::InvalidateRtvData(EN_LANG pLang)
{	

	if(m_pDoc == NULL) return;
	if(m_pRTV_DATA == NULL) return;

	if (!UpdateData())
		return;

	m_edtRtvNo.SetWindowText(m_pRTV_DATA->K_RTV_NO);//RTV 번호
	m_edtJobNo.SetWindowText(m_pRTV_DATA->V_LUGG_NO_FK1_RD);//작업번호
	m_edtJobNo2.SetWindowText(m_pRTV_DATA->V_LUGG_NO_FK2_RD);//작업번호2
	m_cbxRtvStartPos.SetWindowText(m_pRTV_DATA->V_DEPARTHS_NO_FK1_RD);//출발위치
    m_cbxRtvStartPos2.SetWindowText(m_pRTV_DATA->V_DEPARTHS_NO_FK2_RD);//출발위치2
	m_cbxRtvDestPos.SetWindowText(m_pRTV_DATA->V_ARRIVEHS_NO_FK1_RD);//도착위치
	m_cbxRtvDestPos2.SetWindowText(m_pRTV_DATA->V_ARRIVEHS_NO_FK2_RD);//도착위치2
	m_edtHorizontalPos.SetWindowText(m_pRTV_DATA->V_POS_H_RD);//주행위치

	CString strSql = GetQrySelectRTV_STATUS_CCD(m_pRTV_DATA);
	CString strMessage;
	int nRowCnt = -1;
	_RecordsetPtr ptr1 =  m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if(nRowCnt <= 0){	return;	  }
	CRecordSetWrap* pRsw1 = new CRecordSetWrap(ptr1);
	pRsw1->MoveFirst(); 
	for(int i = 0; i < nRowCnt; i++)
	{
		CString strJOB_TYP_RD_TEMP = pRsw1->GetItem(_T("JOB_TYP_RD"));
		m_cbxRtvJobTyp.SetWindowText(strJOB_TYP_RD_TEMP);//작업구분
		m_cbxRtvJobTyp2.SetWindowText(strJOB_TYP_RD_TEMP);//작업구분2
		m_edtRtvMode.SetWindowText(pRsw1->GetItem(_T("AUTO_MODE_RD")));//운영모드
		m_edtComplete.SetWindowText(pRsw1->GetItem(_T("COMPLETE_RD")));//완료상태
		m_edtRtvProdLoad.SetWindowText(pRsw1->GetItem(_T("SENSOR_FK_RD")));//화물유무
		m_edtRtvStatus.SetWindowText(pRsw1->GetItem(_T("UCSTATUS_RD")));//rtv상태
		m_edtActive.SetWindowText(pRsw1->GetItem(_T("ACTIVE_MODE_RD")));//active
		m_edtRtvSuspend.SetWindowText(pRsw1->GetItem(_T("SUSPEND")));//suspend
	}
	delete pRsw1;

	CString strGetErrorCode = _T("");
	GetErrorCode(_T("RTV"), m_pRTV_DATA->V_ERR_CODE_RD, (int)pLang, strGetErrorCode);
	m_edtDiagnosis.SetWindowText(strGetErrorCode);//에러코드

	strSql = GetQrySelectJOB_MST(m_pRTV_DATA);
	nRowCnt = -1;
	_RecordsetPtr ptr =  m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if(nRowCnt <= 0){	return;	  }
	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);

	pRsw->MoveFirst(); 
	for(int i = 0; i < nRowCnt; i++)
	{
		if(i == 0)
		{
			m_edtJobJobNo.SetWindowText(pRsw->GetItem(_T("LUGG_NO")));
			m_cbxRtvJobJobTyp.SetWindowText(pRsw->GetItem(_T("JOB_STATUS")));
			m_cbxRtvJobStartPos.SetWindowText(pRsw->GetItem(_T("START_POS")));
			m_cbxRtvJobDestPos.SetWindowText(pRsw->GetItem(_T("DEST_POS")));
		}
		else
		{
			m_edtJobJobNo2.SetWindowText(pRsw->GetItem(_T("LUGG_NO")));
			m_cbxRtvJobJobTyp2.SetWindowText(pRsw->GetItem(_T("JOB_STATUS")));
			m_cbxRtvJobStartPos2.SetWindowText(pRsw->GetItem(_T("START_POS")));
			m_cbxRtvJobDestPos2.SetWindowText(pRsw->GetItem(_T("DEST_POS")));
		}
		pRsw->MoveNext();
	}
	delete pRsw;

	UpdateData(FALSE);
	InvalidateLangControl(pLang);
}

void CRtvSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}
HCURSOR CRtvSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRtvSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CRtvSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;


	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnRtvComplete.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvComplete.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvEstop.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvEstop.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvActive.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvActive.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvStop.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvStop.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvResetError.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvResetError.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvDelete.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvDelete.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvCallToHome.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvCallToHome.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvSuspend.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}

void CRtvSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\128x32\\");

	CString strExtension = _T(".png");


	m_btnRtvComplete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvComplete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("forcecompletion"), strExtension)), NULL, 5, 5);

	m_btnRtvEstop.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0 ,0);
	m_btnRtvEstop.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("emergencystop"), strExtension)), NULL, 5, 5);

	m_btnRtvActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("active"), strExtension)), NULL, 5, 5);

	m_btnRtvStop.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvStop.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("stop"), strExtension)), NULL, 5, 5);

	m_btnRtvResetError.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvResetError.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("errorreset"), strExtension)), NULL, 5, 5);

	m_btnRtvDelete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvDelete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);

	m_btnRtvCallToHome.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvCallToHome.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("calltohome"), strExtension)), NULL, 5, 5);

	m_btnRtvManual.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvManual.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("manual"), strExtension)), NULL, 5, 5);

	m_btnRtvSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("save"), strExtension)), NULL, 5, 5);

}

LRESULT CRtvSkinDlg::OnMessagSwitch(WPARAM wParam, LPARAM lParam)
{
	CRTV_DATA* pRTV_DATA = (CRTV_DATA*)wParam;
	if(pRTV_DATA == NULL)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("RTV를 찾을수 없습니다")));
		return 0;
	}

	m_pRTV_DATA = pRTV_DATA;

	EN_LANG enLangTemp = (EN_LANG)lParam;
	if(m_nLang != enLangTemp)
	{
		m_nLang = enLangTemp;
		InvalidateLangControl(enLangTemp);
	}

	InvalidateRtvData(enLangTemp);

	return 0;
}

void CRtvSkinDlg::SetButtonScImage( CButton& pButton, CString pFieldName, int pValue )
{
	//HICON hIcon = Global.GetIcon(Global.GetCvIconIndex(pFieldName, pValue));
	//pButton.SetIcon(hIcon);
}

void CRtvSkinDlg::InvalidateLangControl( EN_LANG enLangTemp )
{ // 데이터가 아닌 레이블 / 버튼 등의 값만 수정시킬것
	if(m_pDoc == NULL)
		return;

	//언어추가할것
	//m_lblScJobNo.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호"), enLangTemp));
	//m_lblScJobNo2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호2"), enLangTemp));
	//m_lblScJobTyp.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분"), enLangTemp));
	//m_lblScJobTyp2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분2"), enLangTemp));
	//m_lblScStartPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발위치"), enLangTemp));
	//m_lblScStartPos2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발위치2"), enLangTemp));
	//m_lblScDestPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착위치"), enLangTemp));
	//m_lblScDestPos2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착위치2"), enLangTemp));
	//m_lblScRcMode.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("지상반"), enLangTemp));
	//m_lblScScMode.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("기상반"), enLangTemp));
	//m_lblScForkPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("포크위치"), enLangTemp));
	//m_lblScForkPos2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("포크위치2"), enLangTemp));
	//m_lblScForkStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("포크상태"), enLangTemp));
	//m_lblScForkStatus2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("포크상태2"), enLangTemp));
	//m_lblScJobStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("완료상태"), enLangTemp));
	//m_lblScRcStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("SRC상태"), enLangTemp));
	//m_lblScScStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("동작상태"), enLangTemp));
	//m_lblScHorizontalPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("수평주행"), enLangTemp));
	//m_lblScVerticalPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("수직주행"), enLangTemp));
	//m_lblScProdLoad.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("화물유무"), enLangTemp));
	//m_lblScJobJobNo.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호"), enLangTemp));
	//m_lblScJobJobNo2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호2"), enLangTemp));
	//m_lblScJobJobTyp.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분"), enLangTemp));
	//m_lblScJobJobTyp2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분2"), enLangTemp));
	//m_lblScJobStartPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발위치"), enLangTemp));
	//m_lblScJobStartPos2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발위치2"), enLangTemp));
	//m_lblScJobDestPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착위치"), enLangTemp));
	//m_lblScJobDestPos2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착위치2"), enLangTemp));
	//m_lbScJobJobStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업상태"), enLangTemp));
	//m_lbScJobJobStatus2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업상태2"), enLangTemp));
	//m_lblScJobBcrTop.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("BCR상단"), enLangTemp));
	//m_lblScJobBcrTop2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("BCR상단2"), enLangTemp));
	//m_lblScJobBcrBottom.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("BCR하단"), enLangTemp));
	//m_lblScJobBcrBottom2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("BCR하단2"), enLangTemp));
	//m_lblScSuspend.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("SC상태"), enLangTemp));
}

void CRtvSkinDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");


	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("forcecompletion"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_COMPLETE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("emergencystop"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_ESTOP, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("active"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_ACTIVE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stop"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_STOP, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("errorreset"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_RESET_ERROR, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("delete"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_DELETE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("calltohome"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_CALL_TO_HOME, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manualorder"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_MANUAL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_JOB_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_START_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ordermode"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_MODE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("completestatus"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_COMPLETE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("freightyn"), (int)m_enLang); //화물유무
	SetDlgItemText(IDC_LBL_RTV_PRODLOAD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rtvstauts"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("active"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_ACTIVE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("horizontallocation"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_HORIZONTAL_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ground"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_JISANG_MODE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_JOB_JOB_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_JOB_START_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_JOB_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RTV_JOB_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("suspend"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_RTV_SUSPEND, strValue);

	//grp
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rtvstauts"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_RTV_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_RTV_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork1"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_RTV_STATUS_FK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork2"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_RTV_STATUS_FK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_FK_FK_STATUS_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_FK_FK_STATUS_ITEM3, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rtvstauts"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_FK_FK_STATUS_RTV, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobsta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_JOB_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_JOB_STATUS_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_JOB_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork1"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_JOB_STATUS_FK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork2"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_JOB_STATUS_FK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("error"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_STATUS_SENSOR, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("suspend"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_RTV_SUSPEND, strValue);

	
}

void CRtvSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\16x16\\");
	CString strExtension = _T(".png");

	CImageList imageList;
	imageList.Create(16, 16, TRUE/*bMask*/,	1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cv"), strExtension)));
	imageList.Add(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("sc"), strExtension)));

	RenameResource(nEN_LANG);
	RedrawImage();
}

void CRtvSkinDlg::OnClose()
{
	m_pDoc->m_pRtvSkinDlg = NULL;
	delete this;
}

//강제완료
void CRtvSkinDlg::OnBnClickedBtnRtvComplete()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvConfirm);
}

//비상정지
void CRtvSkinDlg::OnBnClickedBtnRtvEstop()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvEmergency);
}

//ACTIVE
void CRtvSkinDlg::OnBnClickedBtnRtvActive()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvActive);
	
}

//정지
void CRtvSkinDlg::OnBnClickedBtnRtvStop()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvStop);
}

//에러리셋
void CRtvSkinDlg::OnBnClickedBtnRtvResetError()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvErrReset);
}

//삭제
void CRtvSkinDlg::OnBnClickedBtnRtvDelete()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	if (m_strRtvFork == _T("0"))
	{
		AfxMessageBox(m_pDoc->m_pLang->GetLangValue(_T("포크를 선택하세요."), m_pDoc->m_enLang));
		return;
	}
	else if (m_strRtvFork == _T("1"))
	{
		UpdateRtvData(EN_BtnRtvFk1Delete);
	}
	else if (m_strRtvFork == _T("2"))
	{
		UpdateRtvData(EN_BtnRtvFk2Delete);
	}
	else if (m_strRtvFork == _T("3"))
	{
		UpdateRtvData(EN_BtnRtvFk1Fk2Delete);
	}
}

//홈복귀
void CRtvSkinDlg::OnBnClickedBtnRtvCallToHome()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvCallToHome);
}

void CRtvSkinDlg::UpdateRtvData(int nBtnJob)
{
	CString strSql, strWhTyp, strRtvNo, strPlcNo, strScGrpNo, strCmdId, strFK, strMsg=_T("");
	UpdateData(TRUE);

	strWhTyp = m_pDoc->m_WH_TYP;
	strRtvNo = m_pRTV_DATA->K_RTV_NO;
	strPlcNo = m_pRTV_DATA->K_PLC_NO;

	//CMD_RQ_ID 공통코드 참조
	if (     nBtnJob == EN_BtnRtvConfirm  )  { strCmdId = _T("FCMP"); strFK = _T("0"); strMsg = _T("강제완료를 진행하시겠습니까?");   } //강제완료
	else if (nBtnJob == EN_BtnRtvEmergency)  { strCmdId = _T("EMERGENCY"); strFK = _T("0"); strMsg = _T("비상정지를 진행하시겠습니까?");   } //비상정지
	else if (nBtnJob == EN_BtnRtvActive)     { strCmdId = _T("ACTIVE"); strFK = _T("0"); strMsg = _T("ACTIVE를 진행하시겠습니까?");	 } //ACTIVE
	else if (nBtnJob == EN_BtnRtvStop)	    { strCmdId = _T("PAUSE"); strFK = _T("0"); strMsg = _T("STOP을 진행하시겠습니까?");		 } //정지
	else if (nBtnJob == EN_BtnRtvErrReset)   { strCmdId = _T("RESET"); strFK = _T("0"); strMsg = _T("ERROR RESET을 진행하시겠습니까?");} //ERROR RESET
	else if (nBtnJob == EN_BtnRtvFk1Delete)     { strCmdId = _T("DELFK1"); strFK = _T("1");   strMsg = _T("Fork1 삭제를 진행하시겠습니까?");		 } //삭제
	else if (nBtnJob == EN_BtnRtvFk2Delete)     { strCmdId = _T("DELFK2"); strFK = _T("2");   strMsg = _T("Fork2 삭제를 진행하시겠습니까?");		 } //삭제
	else if (nBtnJob == EN_BtnRtvFk1Fk2Delete)     { strCmdId = _T("DELFK12"); strFK = _T("0");   strMsg = _T("Fork1,2 삭제를 진행하시겠습니까?");		 } //삭제
	else if (nBtnJob == EN_BtnRtvCallToHome) { strCmdId = _T("CTH"); strFK = _T("0");  strMsg = _T("홈복귀를 진행하시겠습니까?");	 } //홈복귀

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(strMsg), MB_YESNO) != IDYES)
		return;

	strSql.Format(_T(" UPDATE RTV_DATA		\n")
		_T("    SET CMD_RQ_ID = '%s'		\n")
		_T("	  , CMD_RQ_YN = 'Y'		    \n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	AND PLC_NO = '%02s'			\n")
		_T("    AND RTV_NO = '%s'			  "), strCmdId, strWhTyp, strPlcNo, strRtvNo);

	BOOL isSuccess = FALSE;

	m_pDoc->BeginTrans_DLG();
	isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO1 = m_pRTV_DATA->V_ITN_LUGG_FK1;
		if (strLOG_LUGG_NO1 == _T("")) { strLOG_LUGG_NO1 = _T("0");}
		CString strLOG_LUGG_NO2 = m_pRTV_DATA->V_ITN_LUGG_FK2;
		if (strLOG_LUGG_NO2 == _T("")) { strLOG_LUGG_NO2 = _T("0");}
		CString strLOG_BOTTOM_TRAY = _T("");
		CString strLOG_TOP_TRAY = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("RTV ACTION BUTTON -> SC NO : %s , ACTION : %s"), strRtvNo, strCmdId);

		if (strFK == _T("1"))
		{
			if (strLOG_LUGG_NO1 != _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
		}
		else if (strFK == _T("2"))
		{
			if (strLOG_LUGG_NO2 != _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO2, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
		}
		else
		{
			if (strLOG_LUGG_NO1 == _T("0") && strLOG_LUGG_NO2 == _T("0"))
			{
				if (strCmdId != _T("DELFK12"))
				{
					if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
					{
						m_pDoc->RollbackTrans_DLG();
						return;
					}
				}
			}
			else if (strLOG_LUGG_NO1 != _T("0") && strLOG_LUGG_NO2 == _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
			else if (strLOG_LUGG_NO1 == _T("0") && strLOG_LUGG_NO2 != _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO2, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
			else
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO2, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
		}
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;
}

void CRtvSkinDlg::OnBnClickedBtnRtvSuspend()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage;
	CString strMsg;
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strPlcNo = m_pRTV_DATA->K_PLC_NO;
	CString strRtvNo = m_pRTV_DATA->K_RTV_NO;

	strSql.Format(_T(" SELECT SUSPEND		\n")
		_T("			 FROM RTV_DATA		\n")
		_T("			WHERE WH_TYP = '%s'			\n")
		_T("		      AND PLC_NO = '%02s'			\n")
		_T("			  AND RTV_NO = '%s'			")
		_T("			  AND SUSPEND IN ('1')			"), strWhTyp, strPlcNo, strRtvNo);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	/*
	pRsw->MoveFirst(); 
	*/
	delete pRsw;
	if (nRowCnt == 0)
	{
		strMsg = _T("Suspend");
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("Rtv 사용을 중지하시겠습니까?")), MB_YESNO) != IDYES)
		return;
	}
	else
	{
		strMsg = _T("Release");
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("Rtv를 사용하시겠습니까?")), MB_YESNO) != IDYES)
			return;
	}

	strSql.Format(_T(" UPDATE RTV_DATA		\n")
		_T("    SET SUSPEND = %s		\n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	AND PLC_NO = '%02s'			\n")
		_T("    AND RTV_NO = '%s'			  "), _T("1 - SUSPEND"),strWhTyp, strPlcNo, strRtvNo);

	BOOL isSuccess = FALSE;
	m_pDoc->BeginTrans_DLG();
	isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO =  m_pRTV_DATA->V_ITN_LUGG_FK1;
		if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
		CString strLOG_BOTTOM_TRAY = _T("");
		CString strLOG_TOP_TRAY = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("RTV SUSPEND BOTTON -> RTV_NO : %s , %s"), strRtvNo, strMsg);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;
}



void CRtvSkinDlg::OnBnClickedChkRtvFork1()
{
	UpdateData(TRUE);

	if (m_chkRtvFork1.GetCheck() == 1)
	{
		m_strRtvFork = _T("1");
		//Enable 처리
		m_chkRtvFork1.EnableWindow(TRUE);
		m_chkRtvFork2.EnableWindow(FALSE);
		m_chkRtvFork1Fork2.EnableWindow(FALSE);
	}else{
		m_strRtvFork = _T("0");
		m_chkRtvFork1.EnableWindow(TRUE);
		m_chkRtvFork2.EnableWindow(TRUE);
		m_chkRtvFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}


void CRtvSkinDlg::OnBnClickedChkRtvFork2()
{
	UpdateData(TRUE);

	if (m_chkRtvFork2.GetCheck() == 1)
	{
		m_strRtvFork = _T("2");
		//Enable 처리
		m_chkRtvFork1.EnableWindow(FALSE);
		m_chkRtvFork2.EnableWindow(TRUE);
		m_chkRtvFork1Fork2.EnableWindow(FALSE);
	}else{
		m_strRtvFork = _T("0");
		m_chkRtvFork1.EnableWindow(TRUE);
		m_chkRtvFork2.EnableWindow(TRUE);
		m_chkRtvFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}


void CRtvSkinDlg::OnBnClickedChkRtvFork12()
{
	UpdateData(TRUE);

	if (m_chkRtvFork1Fork2.GetCheck() == 1)
	{
		m_strRtvFork = _T("3");
		//Enable 처리
		m_chkRtvFork1.EnableWindow(FALSE);
		m_chkRtvFork2.EnableWindow(FALSE);
		m_chkRtvFork1Fork2.EnableWindow(TRUE);
	}else{
		m_strRtvFork = _T("0");
		m_chkRtvFork1.EnableWindow(TRUE);
		m_chkRtvFork2.EnableWindow(TRUE);
		m_chkRtvFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}
void CRtvSkinDlg::GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode)
{
	CStringList strList;
	CString strSql;
	CString strMSG_KOR, strMSG_CHIN, strMSG_ENG, strMSG_HUN;
	int nRowCnt = 0, j=0;
	CString strMessage;
	strSql.Format(_T(" SELECT MSG_KOR, ") + m_pDoc->NVL + _T("(MSG_CHIN,MSG_KOR) AS MSG_CHIN, ") + m_pDoc->NVL + _T("(MSG_ENG,MSG_KOR) AS MSG_ENG, ") + m_pDoc->NVL + _T("(MSG_HUN,MSG_KOR) AS MSG_HUN   ")
		_T("    FROM EQP_ECD_MST                     ")
		_T("   WHERE EQP_TYP      = '%s'               ")
		_T("    AND EQP_ERR_CD   = '%04s'               "), strEqpTyp, strErrorCode);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 


	strMSG_KOR = pRsw->GetItem(_T("MSG_KOR"));
	strMSG_CHIN = pRsw->GetItem(_T("MSG_CHIN"));
	strMSG_ENG = pRsw->GetItem(_T("MSG_ENG"));
	strMSG_HUN = pRsw->GetItem(_T("MSG_HUN"));
	if(nEN_LANG == 0)
	{
		strGetErrorCode.Format(_T("[%s] %s"), strErrorCode, strMSG_KOR);
	}
	else if(nEN_LANG == 1)
	{
		strGetErrorCode.Format(_T("[%s] %s"), strErrorCode, strMSG_ENG);
	}
	else if(nEN_LANG == 2)
	{
		strGetErrorCode.Format(_T("[%s] %s"), strErrorCode, strMSG_HUN);
	}
	else if(nEN_LANG == 3)
	{
		strGetErrorCode.Format(_T("[%s] %s"), strErrorCode, strMSG_CHIN);
	}

	delete pRsw;

}

CString CRtvSkinDlg::GetQrySelectJOB_MST( CRTV_DATA* pRTV_DATA )
{
	if(pRTV_DATA == NULL)
		return _T("");

	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(JM.LUGG_NO, '0') AS LUGG_NO ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(CCD.CCD_NM_KOR, '0') AS JOB_STATUS ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.START_POS, '00000') AS START_POS ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.DEST_POS, '00000') AS DEST_POS ");
	strSql += CRLF + _T("  FROM JOB_MST JM INNER JOIN RTV_DATA RD_FK1 ");
	strSql += CRLF + _T("                         ON JM.WH_TYP = ") + CLib::Quot(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                        AND JM.WH_TYP = RD_FK1.WH_TYP ");
	strSql += CRLF + _T("                        AND JM.LUGG_NO = RD_FK1.ITN_LUGG_FK1 ");
	strSql += CRLF + _T("		     RIGHT OUTER JOIN ( SELECT '1' FROM DUAL ) TRASH ");
	strSql += CRLF + _T("                          ON 1=1 ");
	strSql += CRLF + _T("             LEFT OUTER JOIN COMMON_CODE CCD ");
	strSql += CRLF + _T("                          ON CCD.CDX_CD = 'JOB_STATUS' ");
	strSql += CRLF + _T("                         AND CCD.CCD_CD = JM.JOB_STATUS ");
	strSql += CRLF + _T("                         AND CCD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("UNION ALL ");
	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(JM.LUGG_NO, '0') AS LUGG_NO ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(CCD.CCD_NM_KOR, '0') AS JOB_STATUS ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.START_POS, '00000') AS START_POS ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.DEST_POS, '00000') AS DEST_POS ");
	strSql += CRLF + _T("  FROM JOB_MST JM INNER JOIN RTV_DATA RD_FK2 ");
	strSql += CRLF + _T("                         ON JM.WH_TYP = ") + CLib::Quot(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                        AND JM.WH_TYP = RD_FK2.WH_TYP ");
	strSql += CRLF + _T("                        AND JM.LUGG_NO = RD_FK2.ITN_LUGG_FK2 ");
	strSql += CRLF + _T("		     RIGHT OUTER JOIN ( SELECT '1' FROM DUAL ) TRASH ");
	strSql += CRLF + _T("                          ON 1=1 ");
	strSql += CRLF + _T("             LEFT OUTER JOIN COMMON_CODE CCD ");
	strSql += CRLF + _T("                          ON CCD.CDX_CD = 'JOB_STATUS' ");
	strSql += CRLF + _T("                         AND CCD.CCD_CD = JM.JOB_STATUS ");
	strSql += CRLF + _T("                         AND CCD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CRtvSkinDlg::GetQrySelectRTV_STATUS_CCD( CRTV_DATA* pRTV_DATA )
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR,'0') AS JOB_TYP_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_COMPLETE_RD.CCD_NM_KOR, '0') AS COMPLETE_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_SENSOR_FK_RD.CCD_NM_KOR, '0') AS SENSOR_FK_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_AUTO_MODE_RD.CCD_NM_KOR, '0') AS AUTO_MODE_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_UCSTATUS_RD.CCD_NM_KOR, '0') AS UCSTATUS_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ACTIVE_MODE_RD.CCD_NM_KOR, '0') AS ACTIVE_MODE_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_SUSPEND.CCD_NM_KOR, '0') AS SUSPEND ");
	strSql += CRLF + _T("  FROM RTV_DATA RD LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP");
	strSql += CRLF + _T("                          ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.CCD_CD = RD.JOB_TYP_RD ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_COMPLETE_RD ");
	strSql += CRLF + _T("                          ON CCD_COMPLETE_RD.CDX_CD = 'RTV_COMPLETE' ");
	strSql += CRLF + _T("                         AND CCD_COMPLETE_RD.CCD_CD = RD.COMPLETE_RD ");
	strSql += CRLF + _T("                         AND CCD_COMPLETE_RD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_SENSOR_FK_RD ");
	strSql += CRLF + _T("                          ON CCD_SENSOR_FK_RD.CDX_CD = 'RTV_SENSOR_FK' ");
	strSql += CRLF + _T("                         AND CCD_SENSOR_FK_RD.CCD_CD = RD.SENSOR_FK_RD ");
	strSql += CRLF + _T("                         AND CCD_SENSOR_FK_RD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_AUTO_MODE_RD ");
	strSql += CRLF + _T("                          ON CCD_AUTO_MODE_RD.CDX_CD = 'RTV_AUTO_MODE' ");
	strSql += CRLF + _T("                         AND CCD_AUTO_MODE_RD.CCD_CD = RD.AUTO_MODE_RD ");
	strSql += CRLF + _T("                         AND CCD_AUTO_MODE_RD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ACTIVE_MODE_RD ");
	strSql += CRLF + _T("                          ON CCD_ACTIVE_MODE_RD.CDX_CD = 'RTV_ACTIVE_MODE' ");
	strSql += CRLF + _T("                         AND CCD_ACTIVE_MODE_RD.CCD_CD = RD.ACTIVE_MODE_RD ");
	strSql += CRLF + _T("                         AND CCD_ACTIVE_MODE_RD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_UCSTATUS_RD ");
	strSql += CRLF + _T("                          ON CCD_UCSTATUS_RD.CDX_CD = 'RTV_UCSTATUS' ");
	strSql += CRLF + _T("                         AND CCD_UCSTATUS_RD.CCD_CD = RD.ACTIVE_MODE_RD ");
	strSql += CRLF + _T("                         AND CCD_UCSTATUS_RD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_SUSPEND ");
	strSql += CRLF + _T("                          ON CCD_SUSPEND.CDX_CD = 'RTV_SUSPEND' ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.CCD_CD = RD.SUSPEND ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T(" WHERE RD.WH_TYP = ") + CLib::Quot(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND RD.PLC_NO = ") + CLib::Quot(pRTV_DATA->K_PLC_NO);
	strSql += CRLF + _T("   AND RD.RTV_NO = ") + CLib::Quot(pRTV_DATA->K_RTV_NO);

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}
