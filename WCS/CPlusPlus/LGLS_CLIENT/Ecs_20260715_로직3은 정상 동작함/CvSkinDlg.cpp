
// CvSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "CvSkinDlg.h"
#include "afxdialogex.h"
#include "JOB_MST.h"
#include "JobCollection.h"
#include "RecordSetWrap.h"


// CCvSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCvSkinDlg, CSkinDialog)

CCvSkinDlg::CCvSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CCvSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pTrackInfo = NULL;
	m_blAutoSel = false;
}

CCvSkinDlg::CCvSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CCvSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pTrackInfo = NULL;
	m_blAutoSel = false;

}

CCvSkinDlg::~CCvSkinDlg()
{
	m_pDoc->m_pCvSkinDlg = NULL;
	CSkinDialog::OnClose();
	this->DestroyWindow();
}

void CCvSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);	
	//BASE BUTTON
	DDX_Control(pDX, ID_BTN_CV_CANCEL, m_btnCancel);
	DDX_Control(pDX, ID_BTN_CV_OK, m_btnOk);
	//------------

	//DLG DATA ( LBL )

	DDX_Control(pDX, IDC_LBL_CV_JOB_NO,                m_lblCvJobNo);
	DDX_Control(pDX, IDC_LBL_CV_DEST_POS,		       m_lblCvDestPos);
	DDX_Control(pDX, IDC_LBL_CV_JOB_TYP,		       m_lblCvJobTyp);
	DDX_Control(pDX, IDC_LBL_CV_ERR_CD,			       m_lblCvErrCd);
	DDX_Control(pDX, IDC_LBL_JOB_JOB_NO,		       m_lblCvJobJobNo);
	DDX_Control(pDX, IDC_LBL_JOB_START_POS,		       m_lblCvJobStartPos);
	DDX_Control(pDX, IDC_LBL_JOB_DEST_POS,		       m_lblCvJobDestPos);
	DDX_Control(pDX, IDC_LBL_JOB_JOB_TYP,		       m_lblCvJobJobTyp);
	DDX_Control(pDX, IDC_LBL_CV_USE_YN,				   m_lblCvUseYn);
	DDX_Control(pDX, IDC_LBL_CV_WAIT_TIME,			   m_lblCvWaiteTIme);
	DDX_Control(pDX, IDC_LBL_JOB_JOB_STATUS,		   m_lblCvJobStatus);

	//GRP
	DDX_Control(pDX, IDC_GRP_CV_CV_STATUS,			   m_grpCvCvStatus);
	DDX_Control(pDX, IDC_GRP_CV_CV_STATUS_ITEM,        m_grpCvCvStatusItem);
	DDX_Control(pDX, IDC_GRP_CV_CV_STATUS_VALUE,       m_grpCvCvStatusValue);
	DDX_Control(pDX, IDC_GRP_CV_CV_STATUS_COMMAND,     m_grpCvCvStatusCommand);
	//CVSTATUS

	//JOBSTATUS
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS,			   m_grpCvJobStatus);
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS_ITEM,	   m_grpCvJobStatusItem);
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS_VALUE,	   m_grpCvJobStatusValue);
	//--JOBSTATUS

	//STATUS
	//	DDX_Control(pDX, IDC_GRP_CV_STATUS_SENSOR,     	   m_grpCvStatusSensor);
	DDX_Control(pDX, IDC_GRP_CV_STATUS_STATUS,	       m_grpCvStatusStatus);
	//--STATUS
	//GRP


	//TRACKNO
	DDX_Control(pDX, IDC_EDT_TRACK_NO,                 m_edtCvTrackNo);
	//--TRACKNO

	//CVSTATUS					
	DDX_Control(pDX, IDC_EDT_CV_JOB_NO,			       m_edtCvJobNo);
	DDX_Control(pDX, IDC_EDT_CV_ERR_CD,                m_edtCvErrorCode);
	DDX_Control(pDX, IDC_EDT_CV_WAIT_TIME_RD,		   m_edtCvWaitTimeRd);
	DDX_Control(pDX, IDC_EDT_CV_WAIT_TIME_OD,		   m_edtCvWaitTimeOd);

	DDX_Control(pDX, IDC_CBX_CV_DEST_POS,              m_cbxCvDestPos); 
	DDX_Control(pDX, IDC_CBX_CV_JOB_TYP,               m_cbxCvJobTyp);

	//--CVSTATUS									   

	//JOBSTATUS										   
	DDX_Control(pDX, IDC_EDT_JOB_JOB_NO,               m_edtCvJobJobNo);

	DDX_Control(pDX, IDC_CBX_JOB_START_POS,            m_cbxCvJobStartPos);
	DDX_Control(pDX, IDC_CBX_JOB_DEST_POS,             m_cbxCvJobDestPos);
	DDX_Control(pDX, IDC_CBX_JOB_JOB_TYP,              m_cbxCvJobJobTyp);
	DDX_Control(pDX, IDC_CBX_JOB_JOB_STATUS,           m_cbxCvJobJobStatus);

	DDX_Control(pDX, IDC_BTN_CV_WAIT_TIME_UP,		   m_btnCvWaitTimeUp);
	DDX_Control(pDX, IDC_BTN_CV_WAIT_TIME_DOWN,		   m_btnCvWaitTimeDown);
	DDX_Control(pDX, IDC_BTN_CV_ACTIVE,				   m_btnCvActive);
	//--JOBSTATUS

	//SENSOR
	DDX_Control(pDX, IDC_BTN_SENSOR_PROD0,             m_btnCvSenserProd0);
	DDX_Control(pDX, IDC_BTN_SENSOR_PROD1,             m_btnCvSenserProd1);
	DDX_Control(pDX, IDC_BTN_SENSOR_PROD2,             m_btnCvSenserProd2);
	DDX_Control(pDX, IDC_BTN_RTV_INTERLOCK,            m_btnCvRtvLockSign);
	//--SENSOR

	//STATUS
	DDX_Control(pDX, IDC_BTN_STATUS_AUTO_MODE,         m_btnCvAutoMode);
	DDX_Control(pDX, IDC_BTN_STATUS_STO_READY,         m_btnCvStoReady);
	DDX_Control(pDX, IDC_BTN_STATUS_RET_READY,         m_btnCvRetReady);
	DDX_Control(pDX, IDC_BTN_STATUS_STOHS_READY,       m_btnCvStoHsReady);
	DDX_Control(pDX, IDC_BTN_STATUS_RETHS_READY,       m_btnCvRetHsReady);
	//--STATUS

	//COMMAND
	DDX_Control(pDX, IDC_BTN_CV_WRITE,	               m_btnCvWrite);
	DDX_Control(pDX, IDC_BTN_CV_DELETE,                m_btnCvDelete);
	DDX_Control(pDX, IDC_BTN_CV_COPY,	               m_btnCvCopy);
	DDX_Control(pDX, IDC_BTN_CV_CUT,	               m_btnCvCut);
	DDX_Control(pDX, IDC_BTN_CV_PASTE,	               m_btnCvPaste);
	DDX_Control(pDX, IDC_BTN_CV_SUSPEND,			   m_btnCvSuspend);
	DDX_Control(pDX, IDC_BTN_CV_ACTIVE,				   m_btnCvActive);
	DDX_Control(pDX, IDC_BTN_CV_WAITE_TIME,			   m_btnCvWaitTime);
	DDX_Control(pDX, IDC_BTN_CV_ERR_RESET,			   m_btnCvErr_Reset);
	DDX_Control(pDX, IDC_BTN_DP_INSERT_JOB,			   m_btnDpInsertJob);


	DDX_Control(pDX, IDC_LBL_CV_PULP_SENSOR, m_lblCvPulpSensor);
	DDX_Control(pDX, IDC_LBL_JOB_PRODUCT_SIZE, m_lblCvJobProductSize);
	DDX_Control(pDX, IDC_EDT_JOB_PRODUCT_SIZE, m_edtCvJobProductSize);
	DDX_Control(pDX, IDC_CBX_CV_PULP_SENSOR, m_cbxCvPulpSensor);
	DDX_Control(pDX, IDC_BTN_CV_PULP_SENSOR, m_btnCvPulpSensor);
	DDX_Control(pDX, IDC_BTN_CV_TRACK_PAUSE, m_btnCvTrackPause);
	DDX_Control(pDX, IDC_BTN_CV_WAIT_SC_RET_JOB, m_btnCvWaitScRetJob);
	DDX_Control(pDX, IDC_EDT_CV_WAIT_SC_RET_JOB, m_edtCvWaitScRetJob);
	DDX_Control(pDX, IDC_LBL_CV_WAIT_SC_RET_JOB, m_lblCvWaitScRetJob);
	DDX_Control(pDX, IDC_EDT_CV_TRACK_PAUSE, m_edtCvTrackPause);
	//DDX_Control(pDX, IDC_LBL_CV_TRACK_PAUSE, m_lblCvTrackPause);
	DDX_Control(pDX, IDC_BTN_REMOTE_CONTROL, m_btnRemoteControl);
	DDX_Control(pDX, IDC_BTN_ROLL_MODE, m_btnRollMode);
	DDX_Control(pDX, IDC_BTN_STOCK_MODE, m_btnStockMode);
	DDX_Control(pDX, IDC_GRP_CV_STATUS_STATUS_ALL, m_grpCvStatusStatusAll);
	DDX_Control(pDX, IDC_EDT_CV_PULP_SENSOR, m_edtCvPulpSensor);
	DDX_Control(pDX, IDC_CBX_JOB_DEST_LOC, m_cbxJobDestLoc);
	DDX_Control(pDX, IDC_CBX_JOB_START_LOC, m_cbxJobStartLoc);
	DDX_Control(pDX, IDC_LBL_JOB_DEST_LOC, m_lblJobDestLoc);
	DDX_Control(pDX, IDC_LBL_JOB_START_LOC, m_lblJobStartLoc);
	DDX_Control(pDX, IDC_EDT_ITEM, m_edtItem);
	DDX_Control(pDX, IDC_EDT_ITEM_NO, m_edtItemNo);
	DDX_Control(pDX, IDC_EDT_TRACK, m_edtTrack);
	DDX_Control(pDX, IDC_CHK_AUTO_SEL, m_chkAutoSel);
	DDX_Control(pDX, IDC_CHK_AUTO_SEL2, m_chkDebugMode);


	DDX_Control(pDX, IDC_EDT_MG_STATUS, m_edtMgStatus);
	DDX_Control(pDX, IDC_BTN_HOST_EMPTY_PLT_SEND, m_btnHostEmptyPltSend);
	DDX_Control(pDX, IDC_GRP_MGDP_STATUS, m_grpMgdpStatus);
	DDX_Control(pDX, IDC_LBL_MG_STATUS, m_lblMgStatus);
	DDX_Control(pDX, IDC_BTN_MZ_MOVE, m_btnMzMove);
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS_VALUE5, m_grpCvJobStatusValue5);
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS_ITEM3, m_grpCvJobStatusItem3);
}


BEGIN_MESSAGE_MAP(CCvSkinDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CCvSkinDlg::OnMessageSwitch)
	ON_BN_CLICKED(IDC_BTN_CV_WRITE, &CCvSkinDlg::OnBnClickedBtnCvWrite)
	ON_BN_CLICKED(IDC_BTN_CV_DELETE, &CCvSkinDlg::OnBnClickedBtnCvDelete)
	ON_BN_CLICKED(IDC_BTN_CV_COPY, &CCvSkinDlg::OnBnClickedBtnCvCopy)
	ON_BN_CLICKED(IDC_BTN_CV_CUT, &CCvSkinDlg::OnBnClickedBtnCvCut)
	ON_BN_CLICKED(IDC_BTN_CV_PASTE, &CCvSkinDlg::OnBnClickedBtnCvPaste)
	//ON_BN_CLICKED(ID_BTN_CV_OK, &CCvSkinDlg::OnBnClickedBtnCvOk)
	//ON_BN_CLICKED(ID_BTN_CV_CANCEL, &CCvSkinDlg::OnBnClickedBtnCvCancel)
	ON_BN_CLICKED(IDC_BTN_CV_WAIT_TIME_UP, &CCvSkinDlg::OnBnClickedBtnCvWaitTimeUp)
	ON_BN_CLICKED(IDC_BTN_CV_WAIT_TIME_DOWN, &CCvSkinDlg::OnBnClickedBtnCvWaitTimeDown)
	ON_BN_CLICKED(IDC_BTN_CV_WAITE_TIME, &CCvSkinDlg::OnBnClickedBtnCvWaiteTime)
	ON_BN_CLICKED(IDC_BTN_CV_SUSPEND, &CCvSkinDlg::OnBnClickedBtnCvSuspend)
	ON_BN_CLICKED(IDC_BTN_CV_ACTIVE, &CCvSkinDlg::OnBnClickedBtnCvActive)
	ON_BN_CLICKED(IDC_BTN_CV_ERR_RESET, &CCvSkinDlg::OnBnClickedBtnCvErrReset)
	ON_BN_CLICKED(IDC_BTN_DP_INSERT_JOB, &CCvSkinDlg::OnBnClickedBtnDpInsertJob)
	ON_WM_ENTERSIZEMOVE()
	ON_WM_EXITSIZEMOVE()
	ON_BN_CLICKED(IDC_BTN_CV_PULP_SENSOR, &CCvSkinDlg::OnClickedBtnCvPulpSensor)
	ON_BN_CLICKED(IDC_BTN_CV_TRACK_PAUSE, &CCvSkinDlg::OnBnClickedBtnCvTrackPause)
	ON_BN_CLICKED(IDC_BTN_CV_WAIT_SC_RET_JOB, &CCvSkinDlg::OnBnClickedBtnCvWaitScRetJob)
	ON_BN_CLICKED(IDC_CHK_AUTO_SEL, &CCvSkinDlg::OnBnClickedChkAutoSel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_HOST_EMPTY_PLT_SEND, &CCvSkinDlg::OnBnClickedBtnHostEmptyPltSend)
	ON_BN_CLICKED(IDC_BTN_MZ_MOVE, &CCvSkinDlg::OnBnClickedBtnMzMove)
	ON_BN_CLICKED(IDC_CHK_AUTO_SEL2, &CCvSkinDlg::OnBnClickedChkAutoSel2)
END_MESSAGE_MAP()


// CCvSkinDlg 메시지 처리기입니다.

LRESULT CCvSkinDlg::OnMessageSwitch(WPARAM wParam, LPARAM lParam)
{ 
	CTrackInfo* pTrackInfo = (CTrackInfo*)wParam;
	CTrackInfo* pTrackInfoParm = m_pTrackInfo;
	if(pTrackInfo == NULL){ pTrackInfo = pTrackInfoParm;}
	if(pTrackInfo == NULL)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("NOT FIND TRACK")));
		return 0;
	}
	
	m_pTrackInfo = pTrackInfo;
	EN_LANG enLangTemp = (EN_LANG)lParam;
	if(m_nLang != enLangTemp)
	{
		m_nLang = enLangTemp;
		InvalidateLangControl(enLangTemp);
 	}

	InvalidateTrackData(enLangTemp);

	return 0;
}

BOOL CCvSkinDlg::OnInitDialog()
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
	CLib::BindCombo(m_cbxCvJobTyp, _T("JOB_TYP"), m_pDoc, (int)pEn);
	CLib::BindCombo(m_cbxCvPulpSensor, _T("PRODUCT_SIZE"), m_pDoc, (int)pEn);

	int nTemp = (int)m_pDoc->m_bDebugMode;

	m_chkDebugMode.SetCheck(nTemp);

	UpdateData(FALSE);

	return TRUE;
}

void CCvSkinDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("write"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_WRITE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("reset"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_ERR_RESET, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("delete"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_DELETE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("copy"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_COPY, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cut"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_CUT, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("paste"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_PASTE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("arrow-left"), (int)m_enLang);
	SetDlgItemText(IDC_GBX_USAGE_SELCONDIT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("arrow-up"), (int)m_enLang);
	SetDlgItemText(IDC_GBX_USAGE_SELCONDIT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("arrow-down"), (int)m_enLang);
	SetDlgItemText(IDC_GBX_USAGE_SELCONDIT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("waittime"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_WAITE_TIME, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_JOB_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_JOB_NO, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_START_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("waittime"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_WAIT_TIME, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("error"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_ERR_CD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("pulpsensor"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_PULP_SENSOR, strValue); 

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("productsize"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_PRODUCT_SIZE, strValue);

	//strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	//strValue = CLib::GetIniStringFromPath(strFullPath, _T("suspend"), (int)m_enLang);
	//SetDlgItemText(IDC_LBL_CV_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobsta"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_JOB_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("dpinsjob"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_DP_INSERT_JOB, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cvactive"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_ACTIVE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cvsusepend"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("waitscretjob"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_WAIT_SC_RET_JOB, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("trackpause"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CV_TRACK_PAUSE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("pulpsensor"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_PULP_SENSOR, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("waitscretjob"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_WAIT_SC_RET_JOB, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("trackpause"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_CV_TRACK_PAUSE, strValue);

	//qq

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cvsta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_CV_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_CV_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_CV_STATUS_VALUE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_CV_STATUS_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_STATUS_STATUS_ALL, strValue);

	//JOBSTATUS

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobsta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_JOB_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_JOB_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_JOB_STATUS_VALUE, strValue);

	//--JOBSTATUS

	//STATUS
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_CV_STATUS_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_START_LOC, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\"), _T("dlg_cv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_JOB_DEST_LOC, strValue);

	//--grp

}

void CCvSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cv\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\128x32\\");

	CString strExtension = _T(".png");
	m_btnCvWrite.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvWrite.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnCvErr_Reset.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvErr_Reset.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("reset"), strExtension)), NULL, 5, 5);

	m_btnDpInsertJob.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnDpInsertJob.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("mgdp"), strExtension)), NULL, 5, 5);

	//m_btnDpInsertJob.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	//m_btnDpInsertJob.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("dp"), strExtension)), NULL, 5, 5);

	m_btnCvDelete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvDelete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);

	m_btnCvCopy.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvCopy.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("copy"), strExtension)), NULL, 5, 5);

	m_btnCvCut.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvCut.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cut"), strExtension)), NULL, 5, 5);

	m_btnCvPaste.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvPaste.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("paste"), strExtension)), NULL, 5, 5);

	m_btnCvWaitTime.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvWaitTime.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnCvActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);
	





	m_btnCvWaitTimeUp.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvWaitTimeUp.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("arrow-up"), strExtension)), NULL, 5, 5);

	m_btnCvWaitTimeDown.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvWaitTimeDown.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("arrow-down"), strExtension)), NULL, 5, 5);

	m_btnCvSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("save"), strExtension)), NULL, 5, 5);

	m_btnCvActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnCvPulpSensor.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvPulpSensor.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnCvWaitScRetJob.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvWaitScRetJob.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnCvTrackPause.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCvTrackPause.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnHostEmptyPltSend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnHostEmptyPltSend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnMzMove.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnMzMove.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);


	m_btnCvSenserProd0.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvSenserProd1.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvSenserProd2.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvRtvLockSign.SetIcon(Global.GetIcon(Global.ICO_CV_ON));

	m_btnCvAutoMode.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvStoReady.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvRetReady.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvStoHsReady.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnCvRetHsReady.SetIcon(Global.GetIcon(Global.ICO_CV_ON));

	m_btnStockMode.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnRemoteControl.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
	m_btnRollMode.SetIcon(Global.GetIcon(Global.ICO_CV_ON));
}


void CCvSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}


HCURSOR CCvSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCvSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CCvSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnCvWrite.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvWrite.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);


	m_btnCvErr_Reset.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvErr_Reset.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnDpInsertJob.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnDpInsertJob.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	
	

	m_btnCvDelete.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvDelete.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvCopy.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvCopy.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvCut.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvCut.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvPaste.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvPaste.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvSuspend.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvWaitTime.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvWaitTime.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCvActive.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvActive.MoveWindow(rc2.left, rc2.top, sizeLarge.cx,  sizeLarge.cy);

	m_btnCvPulpSensor.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvPulpSensor.MoveWindow(rc2.left, rc2.top, sizeLarge.cx,  sizeLarge.cy);

	m_btnCvWaitScRetJob.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvWaitScRetJob.MoveWindow(rc2.left, rc2.top, sizeLarge.cx,  sizeLarge.cy);

	m_btnCvTrackPause.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvTrackPause.MoveWindow(rc2.left, rc2.top, sizeLarge.cx,  sizeLarge.cy);
}


BOOL CCvSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CCvSkinDlg::InvalidateTrackData(EN_LANG pLang)
{	
	if(m_pDoc == NULL) return;
	if(m_pTrackInfo == NULL) return;
	if(m_pTrackInfo->m_pCV_DATA == NULL) return;

	int nCvWaitTimeRd = (CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_WAIT_TIME_RD));
	CString strGetErrorCode = _T("");
	CString strSUSPEND = _T("");
	CString strSTOCK_MODE = _T("");
	CString strROLL_MODE = _T("");
	CString strREMOTE_CONTROL = _T("");
	CString strKIND = _T("");
	CString strSTATUS = _T("");
	CString strOD_RQ_ID = _T("");
	int nLUGG_NO = 0;

	if (!UpdateData())
		return;
	

	//붙여넣기 활성화 여부 판단 COPY_YN(복사버튼 눌렀는지 유무)
	CJOB_MST* pCopyJob = m_pDoc->m_pJob->GetCopyJob();
	if (pCopyJob->COPY_YN == false)
	{
		GetDlgItem(IDC_BTN_CV_PASTE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_CV_PASTE)->EnableWindow(TRUE);
	}

	//일시정지 활성화 처리
	int nCNT = 0;
	//해당 트랙이 작업대 인지 확인
	SelStnKind(nCNT);

	//작업대 카운트가 있으면
	if(nCNT > 0)
	{
		GetDlgItem(IDC_BTN_CV_TRACK_PAUSE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_CV_TRACK_PAUSE)->EnableWindow(FALSE);
	}

	//대기필요 활성화 처리
	if (m_pTrackInfo->m_pCV_DATA->V_MC_NO == _T("125"))
	{
		GetDlgItem(IDC_BTN_CV_WAIT_SC_RET_JOB)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_CV_WAIT_SC_RET_JOB)->EnableWindow(FALSE);
	}

	//m_cbxCvDestPos.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_DEST_POS_RD);
	CString strJobTyp, strDestPos;

	m_cbxCvJobTyp.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_JOB_TYP_RD);
	//CV상태의 도착위치 콤보박스
	SetBindCombo_DEST_POS_DEF(m_cbxCvDestPos, m_pTrackInfo->m_pCV_DATA->K_PLC_NO,m_pTrackInfo->m_pCV_DATA->V_DEST_POS_RD);

	//상단 명칭 표시
	m_edtTrack.SetWindowText(_T("Track No"));
	m_edtItem.SetWindowText(_T("Item No"));

	//ITEM NO 표시
	m_edtItemNo.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_ITEM_NO);

	//CV 명칭 표시 : [LGLS] 트랙번호는 마지막 2자리만 표시
	CString strTrNo2 = m_pTrackInfo->m_pCV_DATA->V_MC_NO;
	if (strTrNo2.GetLength() > 2) strTrNo2 = strTrNo2.Right(2);
	if(m_pTrackInfo->m_pCV_DATA->V_MC_NO_NM == " ") // 명칭이 없으면 그냥 2자리 NO 
	{
		m_edtCvTrackNo.SetWindowText(strTrNo2);
	}
	else
	{
		m_edtCvTrackNo.SetWindowText(strTrNo2 + " (" + m_pTrackInfo->m_pCV_DATA->V_MC_NO_NM + ")");
	}

	m_cbxCvJobTyp.SetCurSelEx(m_pTrackInfo->m_pCV_DATA->V_JOB_TYP_RD);
	//m_cbxCvPulpSensor.SetCurSelEx(m_pTrackInfo->m_pCV_DATA->V_PULP_SENSOR_RD);
	m_edtCvJobNo.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_LUGG_NO_RD);
	//m_cbxCvFmsRpt.SetWindowText(_T("미정"));
	//m_cbxCvTrayTyp.SetWindowText(_T("미정"));

	GetErrorCode(_T("CV"), m_pTrackInfo->m_pCV_DATA->V_ERROR_CODE, (int)pLang, strGetErrorCode);
	m_edtCvErrorCode.SetWindowText(strGetErrorCode);
	m_edtCvWaitTimeRd.SetWindowText(CConvert::ToString(nCvWaitTimeRd));
	m_edtCvWaitTimeOd.SetWindowText(CConvert::ToString(nCvWaitTimeRd));

	CString strCCD_NM_KOR;
	SelCommonCode(m_pTrackInfo->m_pCV_DATA->V_WAIT_SC_RET_JOB_RD, _T("WAIT_SC_RET_JOB"), strCCD_NM_KOR);
	m_edtCvWaitScRetJob.SetWindowText(strCCD_NM_KOR);

	SelCommonCode(m_pTrackInfo->m_pCV_DATA->V_TR_PAUSE_RD, _T("TRACK_PAUSE"), strCCD_NM_KOR);
	m_edtCvTrackPause.SetWindowText(strCCD_NM_KOR);

	SelCommonCode(m_pTrackInfo->m_pCV_DATA->V_PULP_SENSOR_RD, _T("PRODUCT_SIZE"), strCCD_NM_KOR);
	m_edtCvPulpSensor.SetWindowText(strCCD_NM_KOR);

	//SUSPEND 조회
	CString strSql = GetQrySelectSUSPEND(m_pTrackInfo->m_pCV_DATA, strSUSPEND);
	//m_edtCvSuspend2.SetWindowText(strSUSPEND); //cv suspend
	
	//공통 상태값 조회 및 입력
	strSql = GetQrySelectStatusAll(m_pTrackInfo->m_pCV_DATA, strSTOCK_MODE, strREMOTE_CONTROL, strROLL_MODE);

	m_btnStockMode.SetIcon((strSTOCK_MODE == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	if(strSTOCK_MODE == _T("0"))
	{
		m_btnStockMode.SetWindowText(_T("출고모드"));
		strKIND = _T("1"); // 공팔레트 입고요청
	}
	else
	{
		m_btnStockMode.SetWindowText(_T("입고모드"));
		strKIND = _T("2"); // 공팔레트 출고요청
	}
	m_btnRemoteControl.SetIcon((strREMOTE_CONTROL == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnRollMode.SetIcon((strROLL_MODE == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	if(strROLL_MODE == _T("0"))
	{
		m_btnRollMode.SetWindowText(_T("정상모드"));
	}
	else
	{
		m_btnRollMode.SetWindowText(_T("배출모드"));
	}


	CString strMgMsg = _T("");
	//MG/DP/WC 상태값 표시
	if  (m_pTrackInfo->m_pCV_DATA->V_MC_NO == _T("149"))
	{
		m_btnHostEmptyPltSend.EnableWindow(TRUE);
		SelHostEmptyPlt(strKIND, m_pTrackInfo->m_pCV_DATA->V_MC_NO, strSTATUS, nLUGG_NO);

		CString strLUGG_NO = CConvert::ToString(nLUGG_NO);
		strLUGG_NO.Format(_T("%04d"), nLUGG_NO);

		//출고모드 -> MG -> 공팔레트 입고에 대한 상태값
		if (strSTOCK_MODE == _T("0"))
		{
			m_lblMgStatus.SetWindowText(_T("MG 상태"));
			m_edtMgStatus.EnableWindow(TRUE);

			if (strSTATUS == _T("N"))
			{
				m_edtMgStatus.SetWindowText(_T("ECS->WMS 요청안함"));
			}
			else if (strSTATUS == _T("Q"))
			{
				if (nLUGG_NO == 0)
				{
					m_edtMgStatus.SetWindowText(_T("공PLT입고 재요청 바람"));
				}
				else
				{
					strMgMsg.Format(_T("공PLT입고 진행중[%s]"),strLUGG_NO) ;
					m_edtMgStatus.SetWindowText(strMgMsg);
				}
			}
			else if (strSTATUS == _T("C"))
			{
				m_edtMgStatus.SetWindowText(_T("공PLT입고 완료"));
			}
			else
			{
				m_edtMgStatus.SetWindowText(_T("설비 상태"));
			}
		}
		else //입고모드 -> DP -> 공팔레트 출고에 대한 상태값
		{
			m_lblMgStatus.SetWindowText(_T("DP 상태"));
			m_edtMgStatus.EnableWindow(TRUE);

			if (strSTATUS == _T("N"))
			{
				m_edtMgStatus.SetWindowText(_T("ECS->WMS 요청안함"));
			}
			else if (strSTATUS == _T("Q"))
			{
				if (nLUGG_NO == 0)
				{
					m_edtMgStatus.SetWindowText(_T("공PLT출고 재요청 바람"));
				}
				else
				{
					strMgMsg.Format(_T("공PLT출고 진행중[%s]"),strLUGG_NO) ;
					m_edtMgStatus.SetWindowText(strMgMsg);
				}
			}
			else if (strSTATUS == _T("C"))
			{
				m_edtMgStatus.SetWindowText(_T("공PLT출고 완료"));
			}
			else
			{
				m_edtMgStatus.SetWindowText(_T("설비 상태"));
			}
		}
	}
	else if (m_pTrackInfo->m_pCV_DATA->V_MC_NO == _T("108"))
	{
		m_btnHostEmptyPltSend.EnableWindow(TRUE);
		SelHostEmptyPlt(strKIND, m_pTrackInfo->m_pCV_DATA->V_MC_NO, strSTATUS, nLUGG_NO);

		CString strLUGG_NO = CConvert::ToString(nLUGG_NO);
		strLUGG_NO.Format(_T("%04d"), nLUGG_NO);

		//출고모드 -> MG -> 공팔레트 입고에 대한 상태값
		if (strSTOCK_MODE == _T("0"))
		{
			m_lblMgStatus.SetWindowText(_T("MG 상태"));
			m_edtMgStatus.EnableWindow(TRUE);

			if (strSTATUS == _T("N"))
			{
				m_edtMgStatus.SetWindowText(_T("ECS->WMS 요청안함"));
			}
			else if (strSTATUS == _T("Q"))
			{
				if (nLUGG_NO == 0)
				{
					m_edtMgStatus.SetWindowText(_T("공PLT입고 재요청 바람"));
				}
				else
				{
					strMgMsg.Format(_T("공PLT입고 진행중[%s]"),strLUGG_NO) ;
					m_edtMgStatus.SetWindowText(strMgMsg);
				}
			}
			else if (strSTATUS == _T("C"))
			{
				m_edtMgStatus.SetWindowText(_T("공PLT입고 완료"));
			}
			else
			{
				m_edtMgStatus.SetWindowText(_T("설비 상태"));
			}
		}
		else //공팔레트 출고는 안보여줌.
		{
			m_lblMgStatus.SetWindowText(_T("설비 상태"));
			m_edtMgStatus.EnableWindow(FALSE);
			m_edtMgStatus.SetWindowText(_T(""));
		}
	}
	else if (m_pTrackInfo->m_pCV_DATA->V_MC_NO == _T("104")) //WC상태
	{
		m_btnHostEmptyPltSend.EnableWindow(TRUE);
		//WC상태 조회
		SelWcData(strOD_RQ_ID);
		m_lblMgStatus.SetWindowText(_T("WC 상태"));
		m_edtMgStatus.EnableWindow(TRUE);

		if(strOD_RQ_ID == _T("OK"))
		{
			m_edtMgStatus.SetWindowText(_T("무게보고 요청완료"));
		}
		else
		{
			m_edtMgStatus.SetWindowText(_T("무게보고 요청중"));
		}
	}
	else
	{
		m_lblMgStatus.SetWindowText(_T("설비 상태"));
		m_btnHostEmptyPltSend.EnableWindow(FALSE);
		m_edtMgStatus.EnableWindow(FALSE);
		m_edtMgStatus.SetWindowText(_T(""));
	}

	CString strMessage;
	int nRowCnt = -1;
	
	//버튼추가되야함
	m_btnCvSenserProd0.SetIcon((m_pTrackInfo->m_pCV_DATA->V_SENSOR0_DATA_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvSenserProd1.SetIcon((m_pTrackInfo->m_pCV_DATA->V_SENSOR1_DATA_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvSenserProd2.SetIcon((m_pTrackInfo->m_pCV_DATA->V_SENSOR2_DATA_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	//m_btnCvSenserProd3.SetIcon((m_pTrackInfo->m_pCV_DATA->V_SENSOR3_DATA_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	//m_btnCvSenserProd4.SetIcon((m_pTrackInfo->m_pCV_DATA->V_SENSOR4_DATA_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvRtvLockSign.SetIcon((m_pTrackInfo->m_pCV_DATA->V_RTV_LOCK_SIGN == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));

	m_btnCvAutoMode.SetIcon((m_pTrackInfo->m_pCV_DATA->V_AUTO_MODE_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	if (m_pTrackInfo->m_pCV_DATA->V_AUTO_MODE_RD == _T("0"))
	{
		m_btnCvAutoMode.SetWindowText(_T("수동모드"));
	}
	else
	{
		m_btnCvAutoMode.SetWindowText(_T("자동모드"));
	}
	m_btnCvStoReady.SetIcon((m_pTrackInfo->m_pCV_DATA->V_STO_READY_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvRetReady.SetIcon((m_pTrackInfo->m_pCV_DATA->V_RET_READY_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvStoHsReady.SetIcon((m_pTrackInfo->m_pCV_DATA->V_STOHS_READY_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));
	m_btnCvRetHsReady.SetIcon((m_pTrackInfo->m_pCV_DATA->V_RETHS_READY_RD == _T("1")) ? Global.GetIcon(Global.ICO_CV_ON) : Global.GetIcon(Global.ICO_CV_OFF));

	strSql = GetQrySelectJOB_MST(m_pTrackInfo->m_pCV_DATA);
	nRowCnt = -1;
	_RecordsetPtr ptr =  m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if(nRowCnt <= 0)
	{
		m_edtCvJobJobNo.SetWindowText(_T("N"));
		m_cbxCvJobStartPos.SetWindowText(_T("N"));
		m_cbxCvJobDestPos.SetWindowText(_T("N"));
		m_cbxCvJobJobTyp.SetWindowText(_T("N"));
		m_cbxCvJobJobStatus.SetWindowText(_T("N"));
		m_edtCvJobProductSize.SetWindowText(_T("N"));
		m_cbxJobStartLoc.SetWindowText(_T("N"));
		m_cbxJobDestLoc.SetWindowText(_T("N"));

		return;	  
	}
	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);

	pRsw->MoveFirst(); 
	for(int i = 0; i < nRowCnt; i++)
	{
		m_edtCvJobJobNo.SetWindowText(pRsw->GetItem(_T("LUGG_NO")));
		CString strStartPos = pRsw->GetItem(_T("START_POS"));
		CString strDestPos = pRsw->GetItem(_T("DEST_POS"));
		if(strStartPos == _T("00000")) {strStartPos = pRsw->GetItem(_T("START_LOCATION")); };
		if(strDestPos == _T("00000")) {strDestPos = pRsw->GetItem(_T("DEST_LOCATION")); };
		m_cbxCvJobStartPos.SetWindowText(strStartPos);
		m_cbxCvJobDestPos.SetWindowText(strDestPos);
		m_cbxJobStartLoc.SetWindowText(pRsw->GetItem(_T("START_LOCATION")));
		m_cbxJobDestLoc.SetWindowText(pRsw->GetItem(_T("DEST_LOCATION")));
		m_cbxCvJobJobTyp.SetWindowText(pRsw->GetItem(_T("JOB_TYP")));
		m_cbxCvJobJobStatus.SetWindowText(pRsw->GetItem(_T("JOB_STATUS")));
		m_edtCvJobProductSize.SetWindowText(pRsw->GetItem(_T("PRODUCT_SIZE")));
	
		pRsw->MoveNext();
	}

	delete pRsw;

	//InvalidateLangControl(pLang);

}

void CCvSkinDlg::InvalidateLangControl( EN_LANG enLangTemp )
{ // 데이터가 아닌 레이블 / 버튼 등의 값만 수정시킬것
	if(m_pDoc == NULL)
		return;

	//언어추가할것
	//m_lblCvJobNo.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호"), enLangTemp));
	//m_lblCvStartPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발위치"), enLangTemp));
	//m_lblCvDestPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착위치"), enLangTemp));
	//m_lblCvJobTyp.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분"), enLangTemp));
	//m_lblCvFmsRpt.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("보고여부"), enLangTemp));
	//m_lblCvTrayLev.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("단수"), enLangTemp));
	//m_lblCvTrayTyp.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("타입"), enLangTemp));
	//m_lblCvMesIFSta.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("MES I/F STA"), enLangTemp));
	//m_lblCvErrCd.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("에러코드"), enLangTemp));
	//m_lblCvJobJobNo.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호"), enLangTemp));
	//m_lblCvJobStartPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발위치"), enLangTemp));
	//m_lblCvJobDestPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착위치"), enLangTemp));
	//m_lblCvJobJobTyp.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분"), enLangTemp));
	//m_lblCvJobMrcd.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("MRCD"), enLangTemp));
}

void CCvSkinDlg::InvalidateComboBoxData(EN_LANG enLangTemp, CComboBox* pComboBox, CString pTemp)
{
	pComboBox->InsertString(0,pTemp);
}


void CCvSkinDlg::OnClose()
{
	m_pDoc->m_pCvSkinDlg = NULL;
	delete this;
}


//CV쓰기
void CCvSkinDlg::OnBnClickedBtnCvWrite()
{
	if (!m_pDoc->Permission(_T("CCvSkinDlg"), UPD_YN))
	{
		//AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
 		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}
	UpdateTrackData(EN_BtnWrite);
}
	
//CV삭제
void CCvSkinDlg::OnBnClickedBtnCvDelete()
{
	if (!m_pDoc->Permission(_T("CCvSkinDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}
	UpdateTrackData(EN_BtnDelete);	
}

//CV복사
void CCvSkinDlg::OnBnClickedBtnCvCopy()
{
	UpdateTrackData(EN_BtnCopy);	
}

//CV잘라내기
void CCvSkinDlg::OnBnClickedBtnCvCut()
{
	UpdateTrackData(EN_BtnCut);	
}

//CV붙여넣기
void CCvSkinDlg::OnBnClickedBtnCvPaste()
{
	UpdateTrackData(EN_BtnPaste);
}

void CCvSkinDlg::SetButtonCvImage( CButton& pButton, CString pFieldName, int pValue )
{
	HICON hIcon = Global.GetIcon(CGlobal::ICO_CV_ON);
	pButton.SetIcon(hIcon);
}

//Wait Time 10초 Up
void CCvSkinDlg::OnBnClickedBtnCvWaitTimeUp()
{
	UpdateData(TRUE);

	CString strCvWaitTimeRd = _T("");
	CString strCvWaitTimeOd = _T("");

	m_edtCvWaitTimeRd.GetWindowText(strCvWaitTimeRd);
	m_edtCvWaitTimeOd.GetWindowText(strCvWaitTimeOd);

	int nCvWaitTimeOd = CConvert::ToInt(strCvWaitTimeOd) + 10;

	m_edtCvWaitTimeOd.SetWindowText(CConvert::ToString(nCvWaitTimeOd));

	UpdateData(FALSE);

	return;
}

//Wait Time 10초 Up
void CCvSkinDlg::OnBnClickedBtnCvWaitTimeDown()
{
	UpdateData(TRUE);

	CString strCvWaitTimeRd = _T("");
	CString strCvWaitTimeOd = _T("");

	m_edtCvWaitTimeRd.GetWindowText(strCvWaitTimeRd);
	m_edtCvWaitTimeOd.GetWindowText(strCvWaitTimeOd);

	int nCvWaitTimeOd = CConvert::ToInt(strCvWaitTimeOd) - 10;
	
	if (nCvWaitTimeOd < 0) 
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("0 보다 작을 수 없습니다.")));
		return;
	}

	m_edtCvWaitTimeOd.SetWindowText(CConvert::ToString(nCvWaitTimeOd));

	UpdateData(FALSE);

	return;
}


void CCvSkinDlg::OnBnClickedBtnCvWaiteTime()
{

	if (!m_pDoc->Permission(_T("CCvSkinDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	if(m_pDoc == NULL){ return; };

	CString strSql = _T("");
	CString strCvWaitTimeRD = m_pTrackInfo->m_pCV_DATA->V_WAIT_TIME_RD;
	CString strCvWaitTimeOD = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	CString strTrackNo = _T("");

	UpdateData(TRUE);
	m_edtCvWaitTimeOd.GetWindowText(strCvWaitTimeOD);
	m_edtCvTrackNo.GetWindowText(strTrackNo);

	if (strCvWaitTimeOD == _T("") ){	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패"))); return;}
	if (strCvWaitTimeOD == _T("0") && strCvWaitTimeRD == _T("0") ){	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패"))); return;}



	int nCvWaitTimeRd = (CConvert::ToInt(strCvWaitTimeRD));
	int nCvWaitTimeOd = (CConvert::ToInt(strCvWaitTimeOD));


	strSql.Format(_T(" UPDATE CV_DATA		\n")
		_T("    SET CMD_RQ_PARM = '%d'		 \n")
		_T("	  , CMD_RQ_ID = 'WAIT'		\n")
		_T("      , CMD_RQ_YN = 'Y'			\n")
		_T("  WHERE WH_TYP = '%s'			 \n")
		_T("	AND PLC_NO = '%02s'			\n")
		_T("    AND TRACK_NO = '%05s'		   "), nCvWaitTimeOd, strWhTyp, strPlcNo, strTrackNo);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;
}



//CV_DATA 바코드 상단버튼 클릭
void CCvSkinDlg::OnBnClickedBtnCvBcrTopTransfer()
{
	CString strWhTyp = _T("");
	CString strPLC_NO = _T("");
	CString strBcrTop = _T("");
	CString strSql= _T("");
	CString strTrackNo;
	CString strPlcNo;
	CStringList strCvDataList;
	CString strUserId;
	CString strMessage;

	if (!m_pDoc->Permission(_T("CCvSkinDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateData(TRUE);

	m_edtCvTrackNo.GetWindowText(strTrackNo);

	strWhTyp = m_pDoc->m_WH_TYP;
	strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;

	if ((strBcrTop == "")) 
	{
		AfxMessageBox(m_pDoc->m_pLang->GetLangValue(_T("바코드 형식을 확인해주세요."), m_pDoc->m_enLang));
		return;
	}
	
	strSql=_T("");

	strSql.Format(_T(" UPDATE CV_DATA				       \n")
	_T("    SET BCR_TOP = '%s'						       \n")
	_T("      , WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T(" \n")
	_T("  WHERE WH_TYP = '%s'							   \n")
	_T("	AND PLC_NO = '%02s'							   \n")
	_T("    AND TRACK_NO = '%05s'						     "), strBcrTop, strWhTyp, strPlcNo, strTrackNo);
	

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
	return;

	
}

//CV_DATA 바코드 하단버튼 클릭
void CCvSkinDlg::OnBnClickedBtnCvBcrBottomTransfer()
{
 	CString strWhTyp = _T("");
	CString strPLC_NO = _T("");
	CString strBcrBottom = _T("");
	CString strSql= _T("");
	CString strTrackNo;
	CString strPlcNo;
	CStringList strCvDataList;
	CString strUserId;
	int nCntSel = 0;
	CString strMessage = _T("");

	if (!m_pDoc->Permission(_T("CCvSkinDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateData(TRUE);

	m_edtCvBcrBottomUser.GetWindowText(strBcrBottom);
	m_edtCvTrackNo.GetWindowText(strTrackNo);

	strWhTyp = m_pDoc->m_WH_TYP;
	strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;

	if ((strBcrBottom == "")) 
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 형식을 확인해주세요.")));
		return;
	}

	strSql=_T("");

	strSql.Format(_T(" UPDATE CV_DATA				   \n")
	_T("    SET BCR_BOTTOM = '%s'					   \n")
	_T("      , WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T(" \n")
	_T("  WHERE WH_TYP = '%s'						   \n")
	_T("	AND PLC_NO = '%02s'						   \n")
	_T("    AND TRACK_NO = '%03s'					     "), strBcrBottom, strWhTyp, strPlcNo, strTrackNo);
	

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
	return;
}

//JOB_MST 바코드 상단 버튼클릭
void CCvSkinDlg::OnBnClickedBtnJobBcrTopTransfer()
{
	CString strBcrTop = _T("");
	CString strSql= _T("");
	CString strWhTyp = _T("");
	CString strTrackNo= _T("");
	CString strJobNo=_T("");
	CString strPlcNo = _T("");
	CString strUserId=_T("");
	int nRowCount = 0;
	CStringList strJobMstList;

	if (!m_pDoc->Permission(_T("CViewJobListDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateData(TRUE);
	m_edtCvJobNo.GetWindowText(strJobNo);
	m_edtCvTrackNo.GetWindowText(strTrackNo);
	strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	strUserId = m_pDoc->m_strId;
	strWhTyp = m_pDoc->m_WH_TYP;

	m_edtCvJobBcrTopUser.GetWindowText(strBcrTop);

	if ((strBcrTop == "")) 
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 형식을 확인해주세요.")));
		return;
	}

	strSql.Format(_T("SELECT * FROM JOB_MST WHERE WH_TYP = '%s' AND LUGG_NO = '%04s'"), strWhTyp, strJobNo);

	nRowCount = m_pDoc->GetSelectQryCnt_DLG(strSql);

 	if (nRowCount < 1)
 	{
 		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업번호가 존재하지 않습니다. 작업번호를 확인해주세요.")));
 		return;
 	}

	strSql = _T("");

	strSql.Format(_T(" UPDATE JOB_MST				\n")
	_T("    SET BCR_TOP = '%s'					    \n")
	_T("      , UPD_USER_ID = '%s'				    \n")
	_T("      , UPD_DT = ") + m_pDoc->SYSDATE + _T("		\n")
	_T("	WHERE WH_TYP = '%s'						\n")
	_T("    AND LUGG_NO = '%04s'					  "), strBcrTop, strUserId, strWhTyp, strJobNo);


	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;


}
//JOB_MST 바코드 하단 버튼클릭
void CCvSkinDlg::OnBnClickedBtnJobBcrBottomTransfer()
{
	CString strBcrBottom = _T("");
	CString strSql= _T("");
	CString strWhTyp = _T("");
	CString strTrackNo= _T("");
	CString strJobNo=_T("");
	CString strPlcNo = _T("");
	CString strUserId=_T("");
	int nRowCount = 0;
	CStringList strJobMstList;

	if (!m_pDoc->Permission(_T("CViewJobListDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateData(TRUE);
	m_edtCvJobNo.GetWindowText(strJobNo);
	m_edtCvTrackNo.GetWindowText(strTrackNo);
	strPlcNo = strTrackNo.Mid(0,2);
	strUserId = m_pDoc->m_strId;
	strWhTyp = m_pDoc->m_WH_TYP;

	m_edtCvJobBcrBottomUser.GetWindowText(strBcrBottom);

	if ((strBcrBottom == "")) 
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드 형식을 확인해주세요.")));
		return;
	}

	strSql.Format(_T("SELECT * FROM JOB_MST WHERE WH_TYP = '%s' AND LUGG_NO = '%04s'"), strWhTyp, strJobNo);

	nRowCount = m_pDoc->GetSelectQryCnt_DLG(strSql);

	if (nRowCount < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업번호가 존재하지 않습니다. 작업번호를 확인해주세요.")));
		return;
	}

	strSql = _T("");

	strSql.Format(_T(" UPDATE JOB_MST							\n")
				  _T("    SET BCR_BOTTOM = '%s'					\n")
				  _T("      , UPD_USER_ID = '%s'				\n")
				  _T("      , UPD_DT = ") + m_pDoc->SYSDATE + _T("	\n")
				  _T("	WHERE WH_TYP = '%s'						\n")
				  _T("    AND LUGG_NO = '%04s'					  "), strBcrBottom, strUserId, strWhTyp, strJobNo);


	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;
}

void CCvSkinDlg::UpdateTrackData(int pBtnJob)

{
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;
	CString strPLC_NO = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	CString strTemp = _T("");
	CString strJobNo = _T("");
	CString strDestPos = _T("");
	CString strJobTyp = _T("");
	CString strFmsRptOd = _T("");
	CString strTrayLev = _T("");
	CString strTrayTyp = _T("");
	CString strBcrTop = _T("");
	CString strBcrBottom = _T("");
	CString strErrorCode = _T("");
	CString strPulpSensor = _T("");
	CString strSql=_T("");
	CString CRLF = _T("\r\n");
	CString strLOG_LUGG_NO_RD = m_pTrackInfo->m_pCV_DATA->V_LUGG_NO_RD;
	CString strLOG_JOB_TYP_RD = m_pTrackInfo->m_pCV_DATA->V_JOB_TYP_RD;
	CString strLOG_DEST_POS_RD = m_pTrackInfo->m_pCV_DATA->V_DEST_POS_RD;


	CStringList strManualSeqList;
	CStringList strJobMstList;
	
	CJOB_MST* pCopyJob = m_pDoc->m_pJob->GetCopyJob();

	int nRowCount = 0;

	UpdateData(TRUE);

	m_edtCvJobNo.GetWindowText(strJobNo);
	CLib::GetComBoBoxData(m_cbxCvDestPos, strDestPos, 5);
	strJobTyp = m_cbxCvJobTyp.GetItemKey(m_cbxCvJobTyp.GetCurSel());

	m_edtCvPulpSensor.GetWindowText(strPulpSensor);
	strPulpSensor = m_pTrackInfo->m_pCV_DATA->V_PULP_SENSOR_RD;

	//CLib::GetComBoBoxData(m_cbxCvJobTyp, strJobTyp, 1);
	//m_cbxCvDestPos.GetWindowText(strDestPos);
	//m_cbxCvJobTyp.GetWindowText(strJobTyp);
	//m_cbxCvFmsRpt.GetWindowText(strFmsRptOd);
	//m_cbxCvTrayTyp.GetWindowText(strTrayTyp);
	//MES I/F STA - 확인필요
	m_edtCvErrorCode.GetWindowText(strErrorCode);

	//Edit Data 자릿수 Check
 	if (IsValidataEditData(strJobNo,strDestPos,strJobTyp) == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("Checking Data")));
		return;
	}

	m_pDoc->BeginTrans_DLG();

	

	switch (pBtnJob)
	{
	case EN_BtnWrite :
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업을 적으시겠습니까?")), MB_YESNO) != IDYES)
				break;

			//출발위치 - 확인필요
			if (strJobNo == _T("")){ strJobNo = _T("0");}
			if (strDestPos == _T("")){ strDestPos = _T("0"); }
			if (strJobTyp == _T("")){ strJobTyp=_T("0"); }
			if (strBcrTop == _T("")){ strBcrTop =_T("0");}
			if (strBcrBottom == _T("")){ strBcrBottom=_T("0"); }
			if (strTrayLev == _T("")){ strTrayLev=_T("0"); }
			//MES I/F STA - 확인필요
			if (strErrorCode == _T("") ){strErrorCode=_T("0");}


			strSql.Format(_T(" UPDATE CV_DATA							\n")
			_T("    SET LUGG_NO_OD = '%s'								\n")
			_T("	  , DEST_POS_OD = '%s'								\n")
			_T("	  , JOB_TYP_OD = '%s'								\n")
			_T("	  , PULP_SENSOR_OD = '%s'							\n")	
			//_T("      , TRAY_LEV_OD = '%s'							\n")
			//_T("      , BCR_TOP = '%s'								\n")
			//_T("      , BCR_BOTTOM = '%s'							\n")
			_T("      , WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("  \n")
			//_T("	  , COMP_MG		= 'N'							\n")
			//_T("	  , COMP_DP		= 'N'							\n")
			//_T("	  , COMP_VR		= 'N'							\n")
			_T("      , OD_RQ_YN = 'Y'								\n")
			_T("  WHERE WH_TYP = '%s'								\n")
			_T("	AND PLC_NO = '%02s'								\n")
			_T("    AND MC_NO = '%s'								\n")
			_T("    AND '1'='1'  /* [LGLS] 수동 조작: 진행중 트랙도 허용 */								\n"), strJobNo, strDestPos, strJobTyp, strPulpSensor, strWhTyp, strPLC_NO, strTrackNo);

 			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{
				CString strLOG_LUGG_NO = strJobNo;
				if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
				CString strLOG_BOTTOM_TRAY = strBcrBottom;
				CString strLOG_TOP_TRAY = strBcrTop;
				CString strLOG_MSG = _T("");
				strLOG_MSG.Format(_T("CV 쓰기 -> CV 번호 : %s , 작업번호 : %s->%s, 작업구분 : %s->%s , 도착지 : %s->%s, 단수 : %s->%s"), strTrackNo, strLOG_LUGG_NO_RD, strJobNo, strLOG_JOB_TYP_RD, strJobTyp, strLOG_DEST_POS_RD, strDestPos, strPulpSensor, strPulpSensor);
				if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), strLOG_LUGG_NO_RD, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					InvalidateTrackData(m_nLang);
					return;
				}
				
				CString strTemp = _T("");
				//AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")) + strTemp);
				SetCvStatus(strJobNo, strJobTyp, strDestPos, _T(""), _T(""), _T(""), _T("WRITE"));
				

				// 이미 기록하는게 성공했으면 Commit 하고 다시 Update 할것!
				//m_pDoc->BeginTrans_DLG();

				int nRowCnt = 0;
				strSql = _T("");
				CString strMessage = _T("");

				strSql.Format(_T("  SELECT *						")
						  _T("	  FROM JOB_MST						")
						  _T("	 WHERE WH_TYP = '%s'				")
						  _T("	   AND LUGG_NO = '%s'				"), strWhTyp, strJobNo);


				_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
				CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

				pRsw->MoveFirst(); 

				delete pRsw;

				//조회건수가 있다 -> 이때 작업정보 변경
				if(nRowCnt > 0)
				{
					//// 작업정보도 CV 구동중으로 변경할것!
					strSql.Format(_T(" UPDATE JOB_MST								\n")
								  _T("    SET JOB_STATUS = '11'						\n")
								  _T("	    , UPD_USER_ID = '%s'	    			\n")
								  _T("      , UPD_DT = ") + m_pDoc->SYSDATE + _T("  \n")
								  _T("  WHERE WH_TYP = '%s'							\n")
								  _T("	  AND LUGG_NO = '%s'						\n"), m_pDoc->m_strId, strWhTyp, strJobNo);
					
					BOOL isSuccess2 =  m_pDoc->ExcuteQueryString_DLG(strSql);

					strTemp = isSuccess2 ? _T("!!") : _T("!");
					
					if (isSuccess2 == FALSE)
					{
						m_pDoc->RollbackTrans_DLG();
						AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업정보 변경 실패")));
						return;
					}
					
					m_pDoc->CommitTrans_DLG();
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")) + strTemp);

					return;
				}
				else
				{
					m_pDoc->CommitTrans_DLG();
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")) + strTemp);


					/* 디버깅모드



	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);

	CString strTemp = _T("");
	CString strTime;
	strTime.Format(_T("쓰기 [%02d:%02d:%02d.%03d]"),cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);

	TRACE(_T("%s \n"), strTime);
	//strTemp += strTime;

	if (m_pDoc->m_bDebugMode)
	{
		AfxMessageBox(strTime);
		m_pDoc->m_bDebugStart = TRUE;
	}


	*/

					return;
				}

			}

			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			InvalidateTrackData(m_nLang);
			return;
		}
		break;

	case EN_BtnDelete :
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("트랙의 작업을 삭제하시겠습니까?")), MB_YESNO) != IDYES)
				break;

			strSql=_T("");

			strSql.Format(_T(" UPDATE CV_DATA					\n")
					_T("    SET LUGG_NO_OD = '0'			\n")
					_T("	  , DEST_POS_OD = '0'			\n")
					_T("	  , JOB_TYP_OD = '0'			\n")
					_T("	  , PULP_SENSOR_OD = '0'		\n")
					_T("      , LUGG_NO_RD = '0', JOB_TYP_RD = '0', DEST_POS_RD = '0' ")
					_T("      , SENSOR0_DATA_RD = '0', PULP_SENSOR_RD = '0' ")
					_T("      , WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("  \n")
					_T("      , OD_RQ_YN = 'Y'			\n")
					_T("  WHERE WH_TYP = '%s'			\n")
					_T("	AND PLC_NO = '%02s'			\n")
					_T("    AND MC_NO = '%s'			\n")
					_T("    AND '1'='1'  /* [LGLS] 수동 조작: 진행중 트랙도 허용 */			\n"), strWhTyp, strPLC_NO, strTrackNo);
			
			

			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{
				//CTrackInfo* pTrackInfo = m_pDoc->pCV-m_pInfo->m_MapTrackInfo[strTrackNo];

				//CCV_DATA* pCV_DATA = pTrackInfo->m_pCV_DATA;

				CString strLOG_LUGG_NO = m_pTrackInfo->m_pCV_DATA->V_LUGG_NO_RD;
				if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
				CString strLOG_BOTTOM_TRAY = m_pTrackInfo->m_pCV_DATA->V_BCR_BOTTOM;
				CString strLOG_TOP_TRAY = m_pTrackInfo->m_pCV_DATA->V_BCR_TOP;
				CString strLOG_MSG = _T("");
				strLOG_MSG.Format(_T("CV 삭제 -> CV 번호 : %s , 작업번호 : %s->%s, 작업구분 : %s->%s , 도착지 : %s->%s, 단수 : %s->%s"), strTrackNo, strLOG_LUGG_NO_RD, _T("0"), strLOG_JOB_TYP_RD, _T("0"), strLOG_DEST_POS_RD, _T("0"), strPulpSensor, _T("0"));
				if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), strLOG_LUGG_NO_RD, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					InvalidateTrackData(m_nLang);
					return;
				}

				m_pDoc->CommitTrans_DLG();
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
				SetCvStatus(_T("0000"), _T("0"), _T("000"), _T("0"), _T(""), _T(""), _T("WRITE"));
				return;
			}
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			InvalidateTrackData(m_nLang);
			return;

		}
		break;
	case EN_BtnCopy :
		{
			m_strCopyJobNo=_T("");
			m_strCopyDestPos=_T("");
			m_strCopyJobTyp=_T("");
			m_strCopyBcrTop=_T("");
			m_strCopyBcrBottom=_T("");
			m_strCopyPulpSensor=_T("");
			
			m_edtCvJobNo.GetWindowText(strJobNo);
			//strDestPos = m_cbxCvDestPos.GetItemKey(m_cbxCvDestPos.GetCurSel());
			strJobTyp = m_cbxCvJobTyp.GetItemKey(m_cbxCvJobTyp.GetCurSel());

			pCopyJob->LUGG_NO=strJobNo;
			pCopyJob->DEST_POS=strDestPos;
			pCopyJob->JOB_TYP=strJobTyp;
			pCopyJob->BCR_TOP=strBcrTop;
			pCopyJob->BCR_BOTTOM=strBcrBottom;
			pCopyJob->PRODUCT_SIZE=strPulpSensor;

			pCopyJob->COPY_YN = true;
			GetDlgItem(IDC_BTN_CV_PASTE)->EnableWindow(TRUE);
			m_pDoc->RollbackTrans_DLG();	// [LGLS] Copy는 DB 미변경 - 빈 트랜잭션 정리

		}
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("데이터 복사 성공")));
		break;
	case EN_BtnCut :
		{
			m_strCopyJobNo=_T("");
			m_strCopyDestPos=_T("");
			m_strCopyJobTyp=_T("");
			m_strCopyBcrTop=_T("");
			m_strCopyBcrBottom=_T("");

			m_edtCvJobNo.GetWindowText(strJobNo);
			//strDestPos = m_cbxCvDestPos.GetItemKey(m_cbxCvDestPos.GetCurSel());
			strJobTyp = m_cbxCvJobTyp.GetItemKey(m_cbxCvJobTyp.GetCurSel());

			//m_strCopyJobNo=strJobNo;
			//m_strCopyDestPos=strDestPos;
			//m_strCopyJobTyp=strJobTyp;
			//m_strCopyBcrTop=strBcrTop;
			//m_strCopyBcrBottom=strBcrBottom;

			pCopyJob->LUGG_NO=strJobNo;
			pCopyJob->DEST_POS=strDestPos;
			pCopyJob->JOB_TYP=strJobTyp;
			pCopyJob->BCR_TOP=strBcrTop;
			pCopyJob->BCR_BOTTOM=strBcrBottom;
			pCopyJob->PRODUCT_SIZE=strPulpSensor;

			strSql=_T("");

			strSql.Format(_T(" UPDATE CV_DATA									\n")
						_T("    SET LUGG_NO_OD = '0'							\n")
						_T("	  , DEST_POS_OD = '0'							\n")
						_T("	  , JOB_TYP_OD = '0'							\n")
						_T("	  , PULP_SENSOR_OD = '0'						\n")
						_T("      , WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("  \n")
						_T("      , OD_RQ_YN = 'Y'								\n")
						_T("  WHERE WH_TYP = '%s'								\n")
						_T("	AND PLC_NO = '%02s'								\n")
						_T("    AND MC_NO = '%s'								\n")
						_T("    AND '1'='1'  /* [LGLS] 수동 조작: 진행중 트랙도 허용 */								\n"), strWhTyp, strPLC_NO, strTrackNo);


			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{
				m_pDoc->CommitTrans_DLG();
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
				SetCvStatus(_T("0000"), _T("0"), _T("000"), _T("0"), _T(""), _T(""), _T("WRITE"));
				pCopyJob->COPY_YN = true;
				GetDlgItem(IDC_BTN_CV_PASTE)->EnableWindow(TRUE);
				return;
			}
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			InvalidateTrackData(m_nLang);
			return;

		}
		break;
	case EN_BtnErrReset:
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("에러를 리셋하시겠습니까?")), MB_YESNO) != IDYES)
				break;

			CString strCMD_RQ_ID = _T("RESET");

			m_pDoc->BeginTrans_DLG();

			strSql=_T("");
			strSql.Format(_T(" UPDATE CV_DATA		    \n")
				          _T("    SET CMD_RQ_ID = '%s'  \n")
				          _T("       ,CMD_RQ_YN = 'Y'	\n")
				          _T("  WHERE WH_TYP = '%s'		\n")
				          _T("	AND PLC_NO = '%02s'		\n")
				          _T("    AND MC_NO = '%s'		\n")
				          _T("    AND OD_RQ_YN = 'N'	\n"),  strCMD_RQ_ID, strWhTyp, strPLC_NO, strTrackNo);

 			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{
				m_pDoc->CommitTrans_DLG();
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
				return;
			}
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			return;
		}
		break;
	case EN_BtnPaste :
		{
			CString strTO_CV;
			int nRowCnt = 0;
			CString strMessage;
			strSql.Format(_T("  SELECT TO_CV						")
						  _T("	  FROM CV_DEF_INF					")
						  _T("	 WHERE WH_TYP = '%s'				")
						  _T("	   AND FR_CV = '%s'					")
						  _T("	   AND BUFFER_CV = '%s'				")
						  _T("	   AND USE_YON = 'Y'					"), strWhTyp, strTrackNo, pCopyJob->DEST_POS);


			_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
			CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

			pRsw->MoveFirst(); 

			//조회건수가 있다 -> 현재위치+최종목적지에 대한 중간목적지가 존재해서 중간목적지로 보냄.
			if(nRowCnt > 0)
			{
				strTO_CV = pRsw->GetItem(_T("TO_CV"));
			}
			else
			{
				//조회건수가 없고, 현재 위치에서 4호기로 가야하는경우.
				if(pCopyJob->DEST_POS == _T("904"))
				{
					if(m_pTrackInfo->m_pCV_DATA->V_SC_PLT_JOB_TYP == _T("2")) //롤 작업 하는 CV면 904로 쓰기
					{
						strTO_CV = _T("904");
					}
					else
					{
						strTO_CV = _T("125");
					}
					
				}
				else //4호기를 제외한 나머지 최종 목적지들은 그냥 써주기.
				{
					strTO_CV = pCopyJob->DEST_POS;
				}
				
			}

			delete pRsw;

			strJobNo = _T("");
			strDestPos = _T("");
			strJobTyp = _T("");
			
			if (pCopyJob->LUGG_NO == _T("") ){ AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업번호 없습니다"))); return; }

			//m_pDoc->BeginTrans_DLG();
			
			strSql.Format(_T(" UPDATE CV_DATA					\n")
				_T("    SET LUGG_NO_OD = '%s'		\n")
				_T("	  , DEST_POS_OD = '%s'		\n")
				_T("	  , JOB_TYP_OD = '%s'		\n")
				_T("	  , PULP_SENSOR_OD = '%s'	\n")
				_T("	  , BCR_TOP = '%s'			\n")
				_T("	  , BCR_BOTTOM = '%s'		\n")
				_T("      , OD_RQ_YN = 'Y'			\n")
				_T("	  , IS_TURN_OD = '0'		\n")
				_T("      , TRAY_LEV_OD = '0'       \n")
				_T("      , TRAY_TYP_OD = '0'		\n")
				_T("	  , FMS_RPT_OD = '0'		\n")
				_T("	  , TR_PAUSE_OD = '0'		\n")
				_T("	  , WAIT_TIME_OD = '0'		\n")
				_T("	  , ERR_RQ_OD = '0'			\n")
				_T("  WHERE WH_TYP = '%s'			\n")
				_T("	AND PLC_NO = '%02s'			\n")
				_T("    AND MC_NO = '%s'			\n")
				_T("    AND '1'='1'  /* [LGLS] 수동 조작: 진행중 트랙도 허용 */			  "), pCopyJob->LUGG_NO, strTO_CV, pCopyJob->JOB_TYP, pCopyJob->PRODUCT_SIZE,
														  pCopyJob->BCR_TOP, pCopyJob->BCR_BOTTOM,strWhTyp, strPLC_NO, strTrackNo);


			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{

				CString strLOG_LUGG_NO = strJobNo;
				if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
				CString strLOG_BOTTOM_TRAY = strBcrBottom;
				CString strLOG_TOP_TRAY = strBcrTop;
				CString strLOG_MSG = _T("");
				strLOG_MSG.Format(_T("CV 붙여넣기 -> CV 번호 : %s , 작업번호 : %s->%s, 작업구분 : %s->%s , 도착지 : %s->%s, 단수 : %s->%s"), strTrackNo, strLOG_LUGG_NO_RD, pCopyJob->LUGG_NO, strLOG_JOB_TYP_RD, pCopyJob->JOB_TYP, strLOG_DEST_POS_RD, strTO_CV, strPulpSensor, pCopyJob->PRODUCT_SIZE);
				if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), strLOG_LUGG_NO_RD, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					InvalidateTrackData(m_nLang);
					return;
				}
				
				CString strTemp = _T("");
				//AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")) + strTemp);
				SetCvStatus(pCopyJob->LUGG_NO, pCopyJob->JOB_TYP, strTO_CV, pCopyJob->PRODUCT_SIZE, _T(""), _T(""), _T("WRITE"));
				pCopyJob->COPY_YN = false;
				GetDlgItem(IDC_BTN_CV_PASTE)->EnableWindow(FALSE);
				//UpdateData()

				int nRowCnt = 0;
				strSql = _T("");
				CString strMessage = _T("");

				strSql.Format(_T("  SELECT *						")
						  _T("	  FROM JOB_MST						")
						  _T("	 WHERE WH_TYP = '%s'				")
						  _T("	   AND LUGG_NO = '%s'				"), strWhTyp, pCopyJob->LUGG_NO);


				_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
				CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

				pRsw->MoveFirst(); 

				delete pRsw;

				//조회건수가 있다 -> 이때 작업정보 변경
				if(nRowCnt > 0)
				{
					//// 작업정보도 CV 구동중으로 변경할것!
					strSql.Format(_T(" UPDATE JOB_MST								\n")
								  _T("    SET JOB_STATUS = '11'						\n")
								  _T("	    , UPD_USER_ID = '%s'	    			\n")
								  _T("      , UPD_DT = ") + m_pDoc->SYSDATE + _T("  \n")
								  _T("  WHERE WH_TYP = '%s'							\n")
								  _T("	  AND LUGG_NO = '%s'						\n"), m_pDoc->m_strId, strWhTyp, pCopyJob->LUGG_NO);
					
					BOOL isSuccess2 =  m_pDoc->ExcuteQueryString_DLG(strSql);

					strTemp = isSuccess2 ? _T("!!") : _T("!");
					
					if (isSuccess2 == FALSE)
					{
						m_pDoc->RollbackTrans_DLG();
						AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업정보 변경 실패")));
						return;
					}

					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")) + strTemp);

					m_pDoc->CommitTrans_DLG();
					return;
				}
				else
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")) + strTemp);
					m_pDoc->CommitTrans_DLG();
					return;
				}

			}

			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
			InvalidateTrackData(m_nLang);
			return;
		}
		break;
	case EN_BtnDpInserJob:
		{

			CString strSpace = _T(" ");
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("D/P작업을 추가하겠습니까?")) + strSpace + _T("[TOP_TRAY : " + strBcrTop + ", BOTTOM_TRAY : " + strBcrBottom + " ]"), MB_YESNO) != IDYES)
				break;


			CStringList strList;
			CString strSql;
			CString strMSG_KOR, strMSG_CHIN, strMSG_ENG, strMSG_HUN;
			int nRowCnt = 0, j=0;
			CString strMessage;
			strSql.Format(_T(" SELECT *   ")
				_T("    FROM CV_DATA                     ")
				_T("   WHERE WH_TYP      = '%s'               ")
				_T("    AND MC_NO = '%s'               ")
				_T("    AND RET_READY_RD = '1'               "), strWhTyp, strTrackNo);

			_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
			
			if (nRowCnt < 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("조회에 실패했습니다")));
				return;
			}
			else if (nRowCnt == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고대 READY 여야 가능")));
				return;
			}


			int nResult = GetDP_YN(strWhTyp,strTrackNo);

			if (nResult < 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("조회에 실패했습니다")));
				return;
			}
			else if (nResult == 0)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("D/P 트랙이 아닙니다")));
				return;
			}

			if ((strBcrBottom.Trim() == "") || (strBcrBottom.Trim() == "0") || (strBcrBottom.Trim().GetLength() != 6))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("BOTTOM_TRAY를 확인하시오")));
				return;
			}
			if (strBcrTop.Trim() == "0")
			{
				strBcrTop = "";
			}

			m_pDoc->BeginTrans_DLG();
			strSql = _T("");
			strSql.Format(_T(" UPDATE CV_DATA						\n")
			_T("    SET LUGG_NO_OD = '9999'							\n")
			_T("	  , JOB_TYP_OD = '3'							\n")
			_T("      , BCR_TOP = '0'								\n")
			_T("      , BCR_BOTTOM = '%s'							\n")
			_T("      , WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("   \n")
			_T("	  , COMP_MG		= 'N'							\n")
			_T("	  , COMP_DP		= 'N'							\n")
			_T("	  , COMP_VR		= 'N'							\n")
			_T("      , OD_RQ_YN = 'Y'								\n")
			_T("  WHERE WH_TYP = '%s'								\n")
			_T("	AND PLC_NO = '%02s'								\n")
			_T("    AND MC_NO = '%s'								\n")
			_T("    AND OD_RQ_YN = 'N'								\n"), strBcrBottom, strWhTyp, strPLC_NO, strTrackNo);

			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == FALSE)
			{
				m_pDoc->RollbackTrans_DLG();
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
				return;
			}

			strSql.Format(_T(" UPDATE CV_DATA								\n")
				_T("		      SET BCR_TOP = '0'							\n")
				_T("		        , BCR_BOTTOM = '%s'						\n")
				_T("		        , OD_RQ_YN = 'Y'						\n")
				_T("		    WHERE WH_TYP = '%s'							\n")
				_T("		  	AND PLC_NO = '%02s'							\n")
				_T("		      AND TRACK_NO = (SELECT UP_TRACK_NO		\n")
				_T("		  					  FROM CV_DATA				\n")
				_T("		  					 WHERE WH_TYP = '%s'		\n")
				_T("		  					   AND PLC_NO = '%02s'		\n")
				_T("		  					   AND MC_NO = '%s')	\n"), strBcrTop, strWhTyp, strPLC_NO, strWhTyp, strPLC_NO, strTrackNo);

			isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if (isSuccess == FALSE)
			{
				m_pDoc->RollbackTrans_DLG();
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
				return;
			}

			strSql = _T("");

			strSql.Format(_T(" INSERT INTO JOB_MST						\n")
					  _T("           ( WH_TYP							\n")
					  _T("           , LUGG_NO							\n")
					  _T("           , START_POS						\n")
					  _T("           , DEST_POS							\n")
					  _T("           , JOB_TYP							\n")
					  _T("           , BCR_TOP							\n")
					  _T("           , BCR_BOTTOM						\n")
					  _T("           , MES_WRITE_YN						\n")
					  _T("           , JOB_KIND							\n")
					  _T("           , JOB_STATUS						\n")
					  _T("           , JOB_PRIORITY						\n")
					  _T("           , MES_ERROR_CD						\n")
					  _T("           , OD_LAST_USER						\n")
					  _T("           , JOB_START_DT						\n")
					  _T("           , UPD_DT							\n")
					  _T("           , UPD_USER_ID						\n")
					  _T("           , INS_DT							\n")
					  _T("           , INS_USER_ID						\n")
					  _T("           )									\n")
					  _T("      VALUES									\n")
					  _T("           ( '%s'								\n")
					  _T("           , '9999'							\n")
					  _T("           , '%s'								\n")
					  _T("           , '%s'								\n")
					  _T("           , '3'								\n")
					  _T("           , '%s'								\n")
					  _T("           , '%s'								\n")
					  _T("           , 'N'								\n")
					  _T("           , '0'								\n")
					  _T("           , '13'								\n")
					  _T("           , '0'								\n")
					  _T("           , '0'								\n")
					  _T("           , '0'								\n")
					  _T("           , ") + m_pDoc->SYSDATE + _T("		\n")
					  _T("           , ") + m_pDoc->SYSDATE + _T("		\n")
					  _T("           , '%s'								\n")
					  _T("           , ") + m_pDoc->SYSDATE + _T("		\n")
					  _T("           , '%s'								\n")
					  _T("           )									\n")
					  ,	strWhTyp, strTrackNo, strTrackNo, strBcrTop, strBcrBottom, m_pDoc->m_strId, m_pDoc->m_strId);
			

			isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == FALSE)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
				m_pDoc->RollbackTrans_DLG();
				return;
			}

			m_pDoc->CommitTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
			return;

		}
	case EN_BtnMGDP:
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("Track MG/DP?")), MB_YESNO) != IDYES)
				break;

			CString strCMD_RQ_ID = _T("MGDP");

			strSql = _T("");

			strSql = "";
			strSql += CRLF + _T("  SELECT A.*          ");
			strSql += CRLF + _T("    FROM CV_DATA A    ");
			strSql += CRLF + _T("   INNER JOIN (       ");
			strSql += CRLF + _T("                  SELECT WH_TYP, TRACK_NO, UP_TRACK_NO  ");
			strSql += CRLF + _T("                    FROM CV_DATA              ");
			strSql += CRLF + _T("                   WHERE WH_TYP   = ")  + CLib::Quot(strWhTyp);
			strSql += CRLF + _T("                     AND TRACK_NO = ") + CLib::Quot(strTrackNo);
			strSql += CRLF + _T("                     AND (MG_HS_YN = 'Y' OR DP_HS_YN = 'Y')");
			strSql += CRLF + _T("              ) B                             ");
			strSql += CRLF + _T("      ON A.WH_TYP     = B.WH_TYP              ");
			strSql += CRLF + _T("     AND (A.TRACK_NO  = B.TRACK_NO OR A.TRACK_NO = B.UP_TRACK_NO)  ");
			strSql += CRLF + _T("   ORDER BY A.TRACK_NO            ");

			CString strMessage = _T("");
			_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCount, strMessage);
			if(nRowCount < 2){	return;	  }
			CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
			pRsw->MoveFirst(); pRsw->MoveNext();			
			CString strUP_TRACK_NO = pRsw->GetItem(_T("TRACK_NO"));
			delete pRsw;

			strSql += CRLF + _T(" UPDATE CV_DATA ");
			strSql += CRLF + _T("    SET BCR_BOTTOM = '' ");
			strSql += CRLF + _T("       ,BCR_TOP = '' ");
			strSql += CRLF + _T("  WHERE WH_TYP = ") + CLib::Quot(strWhTyp);
			strSql += CRLF + _T("    AND TRACK_NO = ") + CLib::Quot(strUP_TRACK_NO);
			strSql += CRLF + _T("    AND PLC_NO = ") + CLib::Quot(strPLC_NO);
			strSql += CRLF + _T("    AND OD_RQ_YN = 'N' ");
			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);
			if(isSuccess == FALSE)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
				return;
			}

			strSql.Format(_T(" UPDATE CV_DATA		\n")
				_T("    SET CMD_RQ_ID = '%s'		\n")
				_T("       ,CMD_RQ_YN = 'Y'			\n")
				_T("       ,BCR_BOTTOM = '' ")
				_T("       ,BCR_TOP = 'N' ")
				_T("       ,COMP_MG = 'N' ")
				_T("       ,COMP_DP = 'N' ")
				_T("       ,COMP_VR = 'N' ")
				_T("  WHERE WH_TYP = '%s'			\n")
				_T("	AND PLC_NO = '%02s'			\n")
				_T("    AND TRACK_NO = '%05s'			\n")
				_T("    AND OD_RQ_YN = 'N'			\n"),  strCMD_RQ_ID, strWhTyp, strPLC_NO, strTrackNo);

			isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
				return;
			}
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			return;
		}
		break;
	}
}

void CCvSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{

	RenameResource(nEN_LANG);
	RedrawImage();
}

BOOL CCvSkinDlg::IsValidataEditData(CString strJobNo, CString strDestPos, CString strJobTyp)
{
	int a = strJobNo.GetLength();

	if (strJobNo.GetLength() > 5){return FALSE;}
	if (strDestPos.GetLength() > 6){return FALSE;}

	return TRUE;
}

void CCvSkinDlg::OnBnClickedBtnCvSuspend()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strTrackNo = _T("");
	CString strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;

	UpdateData(TRUE);

	if (!m_pDoc->Permission(_T("CCvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("트랙을 비활성화시키겠습니까?")), MB_YESNO) != IDYES)
		return;

	if(m_pTrackInfo->m_pCV_DATA->V_SUSPEND == _T("1"))
		return;

	strSql.Format(_T(" UPDATE CV_DATA		\n")
		_T("    SET SUSPEND = '1'		\n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	AND PLC_NO = '%02s'			\n")
		_T("    AND MC_NO = '%s'			"), strWhTyp, strPlcNo, strTrackNo);

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;	
}


void CCvSkinDlg::OnBnClickedBtnCvActive()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strTrackNo = _T("");
	CString strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;

	UpdateData(TRUE);

	if (!m_pDoc->Permission(_T("CCvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("트랙을 활성화시키겠습니까?")), MB_YESNO) != IDYES)
		return;

	if (m_pTrackInfo->m_pCV_DATA->V_SUSPEND == _T("0"))
		return;

	strSql.Format(_T(" UPDATE CV_DATA		\n")
		_T("    SET SUSPEND = '0'			\n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	AND PLC_NO = '%02s'			\n")
		_T("    AND MC_NO = '%s'			"), strWhTyp, strPlcNo, strTrackNo);

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;	
}

void CCvSkinDlg::GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode)
{
	CStringList strList;
	CString strSql;
	CString strMSG_KOR, strMSG_CHIN, strMSG_ENG, strMSG_HUN;
	int nRowCnt = 0, j=0;
	CString strMessage;

	strSql.Format(_T(" SELECT MSG_KOR, ") + m_pDoc->NVL + _T("(MSG_CHIN,MSG_KOR) AS MSG_CHIN, ") + m_pDoc->NVL + _T("(MSG_ENG,MSG_KOR) AS MSG_ENG, ") + m_pDoc->NVL + _T("(MSG_HUN,MSG_KOR) AS MSG_HUN   ")
	_T("    FROM EQP_ECD_MST						 ")
	_T("   WHERE EQP_TYP      = '%s'                 ")
	_T("    AND EQP_ERR_CD   = '%04s'                "), strEqpTyp, strErrorCode);

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



int CCvSkinDlg::GetDP_YN(CString pWH_TYP, CString pTrack)
{
	CStringList strList;
	CString strSql;
	CString strMSG_KOR, strMSG_CHIN, strMSG_ENG, strMSG_HUN;
	int nRowCnt = 0, j=0;
	CString strMessage;
	strSql.Format(_T(" SELECT *   ")
		_T("    FROM CV_DATA                     ")
		_T("   WHERE WH_TYP      = '%s'               ")
		_T("    AND MC_NO = '%s'               ")
		_T("    AND DP_HS_YN   = 'Y'               "), pWH_TYP, pTrack);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	delete pRsw;

	return nRowCnt;
}

void CCvSkinDlg::SetBindCombo_DEST_POS_DEF(CComboBoxWrapper& cbx, CString strGroup_No, CString strDEST_POS_RD)
{
	if(m_pDoc == NULL){return;};

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;

	CStringList strList;
	CString strSql;
	CString strTRACK_NO, strREMARKS;
	int nRowCnt = 0, j=0;
	CString strDEST_POS;
	CString strMessage;
	cbx.ResetContent();
	strSql.Format(_T("  SELECT TRACK_NO						")
				  _T("       , PRIORITY						") 
				  _T("       , REMARKS						") 
				  _T("       , GROUP_NO						")
				  _T("       , MC_NO						")
				  _T("	  FROM DEST_POS_DEF					")
				  _T("ORDER BY GROUP_NO, TRACK_NO, PRIORITY	"));

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	
	cbx.SetItemDataEx(0, _T("000"));
	cbx.AddString(_T("000"));

	pRsw->MoveFirst(); 
	
	for(int i = 1; i <= nRowCnt; i++)
	{
		strTRACK_NO = pRsw->GetItem(_T("MC_NO"));		
		strREMARKS = pRsw->GetItem(_T("REMARKS"));		
		strDEST_POS = strTRACK_NO + _T(" - ") + strREMARKS;
		cbx.AddString(strDEST_POS);
		cbx.SetItemData(i, CConvert::ToInt(strTRACK_NO));

		if (strTRACK_NO == strDEST_POS_RD)
		{
			cbx.SetCurSel(i);
		}

		pRsw->MoveNext();
	}

	if (cbx.GetCurSel() == -1)
	{
		cbx.SetWindowText(strDEST_POS_RD);
	}

	delete pRsw;
}


void CCvSkinDlg::SetBindCombo_COMMON_CODE(CComboBox& cbx, CString strCDX_CD, CString strCCD_NM)
{
	CStringList strList;
	CString strSql;
	CString strCCD_CD, strCCD_NM_KOR;
	int nRowCnt = 0, j=0;
	cbx.ResetContent();
	CString strMessage;
	strSql.Format(_T(" SELECT CCD_CD, CCD_NM_KOR	")
		_T("	 FROM COMMON_CODE					")
		_T("	WHERE CCD_CD_YN = 'Y'				")
		_T("    AND CDX_CD = '%s'					")
		_T("	ORDER BY ") + m_pDoc->TO_NUMBER(_T("CCD_CD")) + _T(	")", strCDX_CD));

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 

	for(int i = 0; i < nRowCnt; i++)
	{
		strCCD_CD = pRsw->GetItem(_T("CCD_CD"));
		strCCD_NM_KOR = pRsw->GetItem(_T("CCD_NM_KOR"));
		cbx.AddString(strCCD_NM_KOR);
		cbx.SetItemData(j, CConvert::ToInt(strCCD_CD));
		if (strCCD_CD == strCCD_NM)
		{
			cbx.SetCurSel(j);
		}
		pRsw->MoveNext();
		j++;
	}
	int cc =cbx.GetCurSel();
	if (cc == -1)
	{
		cbx.SetWindowText(strCCD_NM);
	}

 	delete pRsw;
}


void CCvSkinDlg::GetComBoBoxData(CComboBox& cbx, CString& strVal, int nlength = 0)
{
	int nCurSel = cbx.GetCurSel();
	if (nCurSel == -1)
	{
		cbx.GetWindowText(strVal);
		return;
	}
	
	DWORD bItemDate = cbx.GetItemData(nCurSel);
	TCHAR szBuf[10]= {0};
	switch(nlength)
	{
	case 1:
		wsprintf(szBuf,_T("%ld"), bItemDate);
	case 5:
		wsprintf(szBuf,_T("%5d"), bItemDate);
	}
	strVal = (LPCTSTR)szBuf;
	strVal = strVal.Trim();
	/*
	cbx.GetWindowText(strVal);

	strVal = strVal.Left(nlength);*/
}

void CCvSkinDlg::OnBnClickedBtnCvErrReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateTrackData(EN_BtnErrReset);

}

void CCvSkinDlg::OnBnClickedBtnDpInsertJob()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (!m_pDoc->Permission(_T("CCvSkinDlg"), INS_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateTrackData(EN_BtnDpInserJob);

}

CString CCvSkinDlg::GetQrySelectJOB_MST( CCV_DATA* pCV_DATA )
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");

	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(JM.WH_TYP,'10') AS WH_TYP ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.LUGG_NO, '0') AS LUGG_NO ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.START_POS,'00000') AS START_POS ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.START_LOCATION,'00-000-00') AS START_LOCATION ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.DEST_POS,'00000') AS DEST_POS ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.DEST_LOCATION,'00-000-00') AS DEST_LOCATION ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR,'N') AS JOB_TYP ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.BCR_TOP,'N') AS BCR_TOP ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.BCR_BOTTOM,'N') AS BCR_BOTTOM ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_STATUS.CCD_NM_KOR,'N') AS JOB_STATUS ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.MES_ERROR_CD,'0') AS MES_ERROR_CD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_PRODUCT_SIZE.CCD_NM_KOR,JM.PRODUCT_SIZE) AS PRODUCT_SIZE ");
	strSql += CRLF + _T("  FROM JOB_MST JM LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP");
	strSql += CRLF + _T("                          ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.CCD_CD = JM.JOB_TYP ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.WH_TYP LIKE ") + CLib::QuotLikeLR(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_JOB_STATUS ");
	strSql += CRLF + _T("                          ON CCD_JOB_STATUS.CDX_CD = 'JOB_STATUS' ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.CCD_CD = JM.JOB_STATUS ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.WH_TYP LIKE ") + CLib::QuotLikeLR(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_PRODUCT_SIZE ");
	strSql += CRLF + _T("                          ON CCD_PRODUCT_SIZE.CDX_CD = 'PRODUCT_SIZE' ");
	strSql += CRLF + _T("                         AND CCD_PRODUCT_SIZE.CCD_CD = JM.PRODUCT_SIZE ");
	strSql += CRLF + _T("                         AND CCD_PRODUCT_SIZE.WH_TYP LIKE ") + CLib::QuotLikeLR(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T(" WHERE JM.WH_TYP = ") + CLib::Quot(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND JM.LUGG_NO = ") + CLib::Quot(pCV_DATA->V_LUGG_NO_RD);
	

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}



void CCvSkinDlg::OnBnClickedBtnCvMgdp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateTrackData(EN_BtnMGDP);
}

CString CCvSkinDlg::GetQrySelectSUSPEND( CCV_DATA* pCV_DATA, CString& strSUSPEND )
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");
	CString strSUSPEND_CV = _T("");
	CString strSUSPEND_NM = _T("");

	strSql += CRLF + _T("SELECT CD.SUSPEND, ") + m_pDoc->NVL + _T("(CCD_SUSPEND.CCD_NM_KOR, CD.SUSPEND) AS SUSPEND_NM ");
	strSql += CRLF + _T("  FROM CV_DATA CD LEFT OUTER JOIN COMMON_CODE CCD_SUSPEND");
	strSql += CRLF + _T("                          ON CCD_SUSPEND.CDX_CD = 'CV_SUSPEND' ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.CCD_CD = CD.SUSPEND ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.WH_TYP LIKE ") + CLib::QuotLikeLR(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T(" WHERE CD.WH_TYP = ") + CLib::Quot(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND CD.PLC_NO = ") + CLib::Quot(pCV_DATA->K_PLC_NO);
	strSql += CRLF + _T("   AND CD.MC_NO = ") + CLib::Quot(pCV_DATA->K_TRACK_NO);
	
	
	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 


	strSUSPEND_CV = pRsw->GetItem(_T("SUSPEND"));
	strSUSPEND_NM = pRsw->GetItem(_T("SUSPEND_NM"));

	strSUSPEND.Format(_T("[%s] %s"), strSUSPEND_CV, strSUSPEND_NM);


	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CCvSkinDlg::GetQrySelectStatusAll( CCV_DATA* pCV_DATA, CString& pSTOCK_MODE, CString& pREMOTE_CONTROL, CString& pROLL_MODE)
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql += CRLF + _T("SELECT TOP 1 (SELECT REMOTE_CONTROL FROM CV_DATA WHERE MC_NO = '101') AS REMOTE_CONTROL  ");	// [LGLS] TOP 1 instead of LIMIT 1
	strSql += CRLF + _T("      ,(SELECT STOCK_MODE FROM CV_DATA WHERE MC_NO = '149') AS STOCK_MODE			");
	strSql += CRLF + _T("      ,(SELECT ROLL_MODE FROM CV_DATA WHERE MC_NO = '154') AS ROLL_MODE			");
	strSql += CRLF + _T("  FROM CV_DATA  ");
	strSql += CRLF + _T(" WHERE WH_TYP = ") + CLib::Quot(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND PLC_NO = ") + CLib::Quot(pCV_DATA->K_PLC_NO);	// [LGLS] LIMIT removed (TOP 1 above)
	
	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 


	pSTOCK_MODE = pRsw->GetItem(_T("STOCK_MODE"));
	pREMOTE_CONTROL = pRsw->GetItem(_T("REMOTE_CONTROL"));
	pROLL_MODE = pRsw->GetItem(_T("ROLL_MODE"));

	delete pRsw;
	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);

}


void CCvSkinDlg::OnEnterSizeMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSkinDialog::OnEnterSizeMove(0,0);
}


void CCvSkinDlg::OnExitSizeMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSkinDialog::OnExitSizeMove(0,0);
	Invalidate(TRUE);
}


void CCvSkinDlg::OnClickedBtnCvPulpSensor()
{
	CString strSql= _T("");
	CString strPulpSensor = _T("");
	CString strWhTyp = _T("");
	CString strTrackNo= _T("");
	CString strJobNo=_T("");
	CString strPlcNo = _T("");
	CString strUserId=_T("");
	CString strLOG_MSG = _T("");
	int nRowCount = 0;
	CStringList strJobMstList;

	if (!m_pDoc->Permission(_T("CViewJobListDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateData(TRUE);
	m_edtCvJobNo.GetWindowText(strJobNo);
	strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;
	strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	strUserId = m_pDoc->m_strId;
	strWhTyp = m_pDoc->m_WH_TYP;
	strPulpSensor = m_cbxCvPulpSensor.GetItemKey(m_cbxCvPulpSensor.GetCurSel());

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("선택된 PLUP 단수를 설정하시겠습니까?")), MB_YESNO) != IDYES)
		return;

	if (strPulpSensor.Trim() == "")
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("PULP 단수를 선택해주세요.")));
		return;
	}

	strSql = _T("");

	strSql.Format(_T(" UPDATE CV_DATA				 \n")
			      _T("    SET PULP_SENSOR_OD = '%s'	 \n")
			      _T("      , CMD_RQ_ID = '1'		 \n")
			      _T("      , CMD_RQ_YN = 'Y'		 \n")
			      _T("	WHERE WH_TYP = '%s'			 \n")
				  _T("	  AND PLC_NO = '%s'			 \n")
				  _T("	  AND MC_NO = '%s'			   "), strPulpSensor, strWhTyp, strPlcNo, strTrackNo);


	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		strLOG_MSG.Format(_T("PULP 단수 설정 -> TRACK NO : %s , 단수 : %s"), strTrackNo, strPulpSensor);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
		{
			//m_pDoc->RollbackTrans_DLG();
			return;
		}

		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		SetCvStatus( _T(""),  _T(""),  _T(""), strPulpSensor, _T(""), _T(""), _T("PULP"));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;
}


void CCvSkinDlg::OnBnClickedBtnCvTrackPause()
{
	CString strSql= _T("");
	CString strTrackPause = _T("");
	CString strWhTyp = _T("");
	CString strTrackNo= _T("");
	CString strJobNo=_T("");
	CString strPlcNo = _T("");
	CString strUserId=_T("");
	CString strTrPauseRd = _T("");
	CString strLOG_MSG = _T("");
	int nRowCount = 0;
	CStringList strJobMstList;

	if (!m_pDoc->Permission(_T("CViewJobListDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateData(TRUE);
	m_edtCvJobNo.GetWindowText(strJobNo);
	//m_edtCvTrackNo.GetWindowText(strTrackNo);
	strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;
	strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	strUserId = m_pDoc->m_strId;
	strWhTyp = m_pDoc->m_WH_TYP;
	strTrackPause = m_pTrackInfo->m_pCV_DATA->V_TR_PAUSE_RD;

	if (strTrackPause == "0")
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 트랙을 일시정지 설정 하시겠습니까?")), MB_YESNO) != IDYES)
			return;
	}
	else
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 트랙을 일시정지 해제 하시겠습니까?")), MB_YESNO) != IDYES)
			return;
	}

	if (strTrackPause == "0")
	{
		strTrPauseRd = _T("1");
	}
	else
	{
		strTrPauseRd = _T("0");
	}

	strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql.Format(_T(" SELECT COUNT(1) AS CNT    		 \n")
			      _T("   FROM CV_DATA					 \n")
			      _T("	WHERE WH_TYP = '%s'				 \n")
				  _T("	  AND PLC_NO = '%s'				 \n")
				  _T("	  AND MC_NO = '%s'				 \n")
				  _T("	  AND STN_KIND IS NOT NULL		   "), strWhTyp, strPlcNo, strTrackNo);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	CString strCNT = pRsw->GetItem(_T("CNT"));	
	delete pRsw;	

	if (strCNT == "1")
	{
		strSql = _T("");

		strSql.Format(_T(" UPDATE CV_DATA					 \n")
				      _T("    SET TR_PAUSE_OD = '%s'		 \n")
				      _T("      , TR_PAUSE_RD = '%s'		 \n")	// [LGLS] CV pause gateway absent: echo order into readback
				      _T("      , CMD_RQ_ID = '3'			 \n")
				      _T("      , CMD_RQ_YN = 'Y'			 \n")
				      _T("	WHERE WH_TYP = '%s'				 \n")
					  _T("	  AND PLC_NO = '%s'				 \n")
					  _T("	  AND MC_NO = '%s'				   "), strTrPauseRd, strTrPauseRd, strWhTyp, strPlcNo, strTrackNo);


		BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

		if(isSuccess == TRUE)
		{
			strLOG_MSG.Format(_T("트랙 일시정지 설정 -> TRACK NO : %s , 일시정지 : %s"), strTrackNo, strTrPauseRd);
			if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
			{
				//m_pDoc->RollbackTrans_DLG();
				return;
			}

			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
			SetCvStatus( _T(""),  _T(""),  _T(""), _T(""), strTrPauseRd, _T(""), _T("PAUSE"));
			return;
		}
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}
	else
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 트랙은 일시정지를 할 수 없습니다.")));
		return;
	}
	
}


void CCvSkinDlg::OnBnClickedBtnCvWaitScRetJob()
{
	CString strSql= _T("");
	CString strWaitScRetJob = _T("");
	CString strWaitScRetJobRd = _T("");
	CString strWhTyp = _T("");
	CString strTrackNo= _T("");
	CString strJobNo=_T("");
	CString strPlcNo = _T("");
	CString strUserId=_T("");
	CString strLOG_MSG=_T("");
	int nRowCount = 0;
	CStringList strJobMstList;

	if (!m_pDoc->Permission(_T("CViewJobListDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateData(TRUE);
	m_edtCvJobNo.GetWindowText(strJobNo);
	strTrackNo = m_pTrackInfo->m_pCV_DATA->K_TRACK_NO;
	strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	strUserId = m_pDoc->m_strId;
	strWhTyp = m_pDoc->m_WH_TYP;
	strWaitScRetJob = m_pTrackInfo->m_pCV_DATA->V_WAIT_SC_RET_JOB_RD;

	if (strWaitScRetJob == "0")
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("대기필요를 설정 하시겠습니까?")), MB_YESNO) != IDYES)
			return;
	}
	else
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("대기필요를 해제 하시겠습니까?")), MB_YESNO) != IDYES)
			return;
	}

	if (strWaitScRetJob == "0")
	{
		strWaitScRetJobRd = _T("1");
	}
	else
	{
		strWaitScRetJobRd = _T("0");
	}

	strSql = _T("");

	strSql.Format(_T(" UPDATE CV_DATA					 \n")
			      _T("    SET WAIT_SC_RET_JOB_OD = '%s'	 \n")
			      _T("      , CMD_RQ_ID = '2'			 \n")
			      _T("      , CMD_RQ_YN = 'Y'			 \n")
			      _T("	WHERE WH_TYP = '%s'				 \n")
				  _T("	  AND PLC_NO = '%s'				 \n")
				  _T("	  AND MC_NO = '%s'				   "), strWaitScRetJobRd, strWhTyp, strPlcNo, strTrackNo);


	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		strLOG_MSG.Format(_T("트랙 대기필요 설정 -> TRACK NO : %s , 대기필요 : %s"), strTrackNo, strWaitScRetJobRd);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
		{
			//m_pDoc->RollbackTrans_DLG();
			return;
		}

		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		SetCvStatus( _T(""),  _T(""),  _T(""), _T(""), _T(""), strWaitScRetJobRd, _T("WAIT"));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;
}

void CCvSkinDlg::SetCvStatus(CString pLUGG_NO, CString pJOB_TYP, CString pDEST_POS, CString pPULP_SENSOR, CString pTRACK_PAUSE, CString pWAIT_SC_JOB_RET, CString pCMD)
{
	CString strCcdNmKor = _T("");
	if (pCMD == _T("WRITE"))
	{
		m_cbxCvJobTyp.SetCurSelEx(pJOB_TYP);
		m_edtCvJobNo.SetWindowText(pLUGG_NO);
		SetBindCombo_DEST_POS_DEF(m_cbxCvDestPos, m_pTrackInfo->m_pCV_DATA->K_PLC_NO, pDEST_POS);
		SelCommonCode(pPULP_SENSOR, _T("PRODUCT_SIZE"), strCcdNmKor);
		m_edtCvPulpSensor.SetWindowText(strCcdNmKor);
	}
	else if (pCMD == _T("PULP"))
	{
		SelCommonCode(pPULP_SENSOR, _T("PRODUCT_SIZE"), strCcdNmKor);
		m_edtCvPulpSensor.SetWindowText(strCcdNmKor);
		m_cbxCvPulpSensor.SetCurSelEx(_T("0"));
	}
	else if (pCMD == _T("PAUSE"))
	{
		SelCommonCode(pTRACK_PAUSE, _T("TRACK_PAUSE"), strCcdNmKor);
		m_edtCvTrackPause.SetWindowText(strCcdNmKor);
	}
	else if (pCMD == _T("WAIT"))
	{
		SelCommonCode(pWAIT_SC_JOB_RET, _T("WAIT_SC_RET_JOB"), strCcdNmKor);
		m_edtCvWaitScRetJob.SetWindowText(strCcdNmKor);
	}
}

void CCvSkinDlg::SelCommonCode(CString pCCD_CD, CString pCMD, CString& pCCD_NM_KOR)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql.Format(_T("  SELECT CCD_NM_KOR					")
				  _T("	  FROM COMMON_CODE					")
				  _T("	 WHERE CDX_CD = '%s'				")
				  _T("	   AND CCD_CD = '%s'				"), pCMD, pCCD_CD);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 

	pCCD_NM_KOR = pRsw->GetItem(_T("CCD_NM_KOR"));	

	delete pRsw;
}

void CCvSkinDlg::SelStnKind(int& pCNT)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql.Format(_T(" SELECT COUNT(1) AS CNT    		 \n")
			      _T("   FROM CV_DATA					 \n")
			      _T("	WHERE WH_TYP = '%s'				 \n")
				  _T("	  AND PLC_NO = '%s'				 \n")
				  _T("	  AND MC_NO = '%s'				 \n")
				  _T("	  AND STN_KIND IS NOT NULL		   "), m_pDoc->m_WH_TYP, m_pTrackInfo->m_pCV_DATA->K_PLC_NO, m_pTrackInfo->m_pCV_DATA->V_MC_NO);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	CString strCNT = pRsw->GetItem(_T("CNT"));
	pCNT = CConvert::ToInt(strCNT);

	delete pRsw;
}

void CCvSkinDlg::SelHostEmptyPlt(CString pKIND, CString pSTN, CString& pSTATUS, int& pLUGG_NO)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql.Format(_T(" SELECT TOP 1 *    					 \n") // [LGLS] TOP 1 instead of LIMIT 1
			      _T("   FROM HOST_EMPTY_PLT			 \n")
			      _T("	WHERE WH_TYP = '%s'				 \n")
				  _T("	  AND KIND = '%s'				 \n")
				  _T("	  AND STN = '%s'				 \n")
				  _T("	 ORDER BY INS_DT DESC			 \n")
				  _T("	 						 \n"), m_pDoc->m_WH_TYP, pKIND, pSTN); // [LGLS] LIMIT removed

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pSTATUS = pRsw->GetItem(_T("STATUS"));
	CString strLUGG_NO = pRsw->GetItem(_T("LUGG_NO"));
	pLUGG_NO = CConvert::ToInt(strLUGG_NO);

	delete pRsw;

	return;
}

void CCvSkinDlg::SelWcData(CString& pOD_RQ_ID)
{
	CString strMC_NO = m_pTrackInfo->m_pCV_DATA->V_MC_NO;
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strSql.Format(_T(" SELECT *    						 \n")
			      _T("   FROM WC_DATA					 \n")
			      _T("	WHERE WH_TYP = '%s'				 \n")
				  _T("	  AND WC_MC_NO = '%s'			 \n"), m_pDoc->m_WH_TYP, strMC_NO);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pOD_RQ_ID = pRsw->GetItem(_T("OD_RQ_ID"));

	delete pRsw;

	return;
}

void CCvSkinDlg::OnBnClickedChkAutoSel()
{
	int nChk = m_chkAutoSel.GetCheck();

	if(nChk == true)
	{
		if (m_blAutoSel == false)
		{
			SetTimer(1,1500,NULL);
			m_blAutoSel = true;
		}
		else
		{
			return;
		}
	}
	else
	{
		KillTimer(1);
		m_blAutoSel = false;
	}

	return;
}


void CCvSkinDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_blAutoSel == true)
	{
		//조회
		InvalidateTrackData(EN_KOR);
	}
}


void CCvSkinDlg::OnBnClickedBtnHostEmptyPltSend()
{
	CString strMC_NO = m_pTrackInfo->m_pCV_DATA->V_MC_NO;

	if (strMC_NO == _T("104"))
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("WC 무게를 WMS에 재요청 하겟습니까?")), MB_YESNO) != IDYES)
			return;
		CString strWhTyp = m_pTrackInfo->m_pCV_DATA->K_WH_TYP;
		CString strPlcNo = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
		CString strWcMcNo = m_pTrackInfo->m_pCV_DATA->V_MC_NO;
		CString strLOG_MSG = _T("");

		UpdateData(TRUE);

		CString strLUGG_NO_RD = m_pTrackInfo->m_pCV_DATA->V_LUGG_NO_RD;
		CString strDEST_POS_RD = m_pTrackInfo->m_pCV_DATA->V_DEST_POS_RD;
		CString strRET_READY_RD = m_pTrackInfo->m_pCV_DATA->V_RET_READY_RD;

		if(CConvert::ToInt(strLUGG_NO_RD) <= 0)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("104번 트랙에 작업번호가 없습니다.")));
				return;
		}

		if(strDEST_POS_RD != _T("104"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("104번 트랙의 목적지가 104번(자기자신)이 아닙니다.")));
				return;
		}

		if(strRET_READY_RD != _T("1"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("104번 트랙의 출고대 신호가 OFF입니다.")));
				return;
		}

		m_pDoc->BeginTrans_DLG();

		CString strSql = _T("");

		strSql.Format(_T(" UPDATE WC_DATA									\n")
					_T("    SET OD_RQ_ID = 'RQ' 							\n")
					_T("  WHERE WH_TYP = '%s'								\n")
					_T("	AND PLC_NO = '%02s'								\n")
					_T("    AND WC_MC_NO = '%s'								\n"), strWhTyp, strPlcNo, strWcMcNo);

		BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

		if(isSuccess == TRUE)
		{
			strLOG_MSG.Format(_T("WC 재요청 지시 -> WC NO : %s" ), strWcMcNo);
			if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
			{
				m_pDoc->RollbackTrans_DLG();
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("LOG 입력중 실패")));
				return;
			}

			m_pDoc->CommitTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
			return;
		}
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
		return;
	}
	else
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("공 PLT를 WMS에 재요청 하겟습니까?")), MB_YESNO) != IDYES)
			return;

		CString strSql = _T("");
		CString strSTOCK_MODE, strREMOTE_CONTROL, strROLL_MODE, strKIND, strSTATUS, strLOG_STATUS;
		int nLUGG_NO;

		strSql = GetQrySelectStatusAll(m_pTrackInfo->m_pCV_DATA, strSTOCK_MODE, strREMOTE_CONTROL, strROLL_MODE);

		//출고모드일 때
		if(strSTOCK_MODE == _T("0"))
		{
			strKIND = _T("1"); // 공팔레트 입고요청
			strLOG_STATUS = _T("입고요청");
		}
		else //입고모드 일 때
		{
			strKIND = _T("2"); // 공팔레트 출고요청
			strLOG_STATUS = _T("출고요청");
		}

		SelHostEmptyPlt(strKIND, m_pTrackInfo->m_pCV_DATA->V_MC_NO, strSTATUS, nLUGG_NO);

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
						  _T("					 )					   "), m_pDoc->m_WH_TYP, strKIND, m_pTrackInfo->m_pCV_DATA->V_MC_NO, strSTATUS); // [LGLS] LIMIT removed


			BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

			if(isSuccess == TRUE)
			{
				strLOG_MSG.Format(_T("공PLT 재요청 -> 트랙번호 : %s , 작업구분 : %s"), m_pTrackInfo->m_pCV_DATA->V_MC_NO, strLOG_STATUS);
				if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
					m_pDoc->RollbackTrans_DLG();
					return;
				}

				m_pDoc->CommitTrans_DLG();
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("공 PLT 재요청 성공")));
				InvalidateTrackData(EN_KOR);
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
}


void CCvSkinDlg::OnBnClickedBtnMzMove()
{
	CString strSql = _T("");
	CString strWH_TYP = m_pTrackInfo->m_pCV_DATA->K_WH_TYP;
	CString strPLC_NO = m_pTrackInfo->m_pCV_DATA->K_PLC_NO;
	CString strMC_NO = m_pTrackInfo->m_pCV_DATA->V_MC_NO;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("매거진(108번)으로 이동시키겠습니까?")), MB_YESNO) != IDYES)
			return;

	m_pDoc->BeginTrans_DLG();

	strSql.Format(_T(" UPDATE CV_DATA						\n")
	_T("    SET LUGG_NO_OD = '9999'							\n")
	_T("	  , DEST_POS_OD = '108'							\n")
	_T("	  , JOB_TYP_OD = '6'							\n")
	_T("      , WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T("  \n")
	_T("      , OD_RQ_YN = 'Y'								\n")
	_T("  WHERE WH_TYP = '%s'								\n")
	_T("	AND PLC_NO = '%02s'								\n")
	_T("    AND MC_NO = '%s'								\n")
	_T("    AND OD_RQ_YN = 'N'								\n"),  strWH_TYP, strPLC_NO, strMC_NO);

 	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO = _T("9999");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("MZ 이동 지시 -> 트랙번호 : %s , 작업구분 : %s , 도착지 : %s"), strMC_NO, _T("6"), _T("108"));
		if (!m_pDoc->GetQueryInsertClientLog(_T("CCvSkinDlg"), strLOG_LUGG_NO, _T(""), _T(""), strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			InvalidateTrackData(m_nLang);
			return;
		}
		
		CString strTemp = _T("");
		SetCvStatus(_T("9999"), _T("6"), _T("108"), _T(""), _T(""), _T(""), _T("WRITE"));

		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}

	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	InvalidateTrackData(m_nLang);
	return;
}

void CCvSkinDlg::OnBnClickedChkAutoSel2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nChk = m_chkDebugMode.GetCheck();

	if(nChk == true)
	{
		m_pDoc->m_bDebugMode = TRUE;
	}
	else
	{
		m_pDoc->m_bDebugMode = FALSE;
	}

	return;
}