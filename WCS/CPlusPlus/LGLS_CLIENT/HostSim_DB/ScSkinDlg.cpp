

// ScSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "ScSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"

// CScSkinDlg 대화 상자입니다.0

IMPLEMENT_DYNAMIC(CScSkinDlg, CSkinDialog)

	CScSkinDlg::CScSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CScSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pSC_DATA = NULL;
	m_pDB = NULL;
	m_pScManualRet = NULL;
}
CScSkinDlg::CScSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CScSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pSC_DATA = NULL;
	m_pDB = NULL;
	m_pScManualRet = NULL;
}

CScSkinDlg::~CScSkinDlg()
{
	m_pDoc->m_pScSkinDlg = NULL;
	CSkinDialog::OnClose();
	this->DestroyWindow();
}

void CScSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//BASE BUTTON
	DDX_Control(pDX, ID_BTN_SC_CANCEL,						m_btnScCancel);
	DDX_Control(pDX, ID_BTN_SC_OK,							m_btnScOk);
	//														

	//CSTATIC												
	DDX_Control(pDX, IDC_LBL_SC_JOB_NO,						m_lblScJobNo);
	DDX_Control(pDX, IDC_LBL_SC_JOB_TYP,					m_lblScJobTyp);
	DDX_Control(pDX, IDC_LBL_SC_START_POS,					m_lblScStartPos);
	DDX_Control(pDX, IDC_LBL_SC_DEST_POS,					m_lblScDestPos);

	DDX_Control(pDX, IDC_LBL_SC_RC_MODE,					m_lblScRcMode);
	DDX_Control(pDX, IDC_LBL_SC_SC_MODE,					m_lblScScMode);
	DDX_Control(pDX, IDC_LBL_SC_FORK_POS,					m_lblScForkPos);
	DDX_Control(pDX, IDC_LBL_SC_FORK_STATUS,				m_lblScForkStatus);
	DDX_Control(pDX, IDC_LBL_SC_JOB_STATUS,					m_lblScJobStatus);
	DDX_Control(pDX, IDC_LBL_SC_RC_STATUS,					m_lblScRcStatus);
	DDX_Control(pDX, IDC_LBL_SC_SC_STATUS,					m_lblScScStatus);
	DDX_Control(pDX, IDC_LBL_SC_HORIZONTAL_POS,				m_lblScHorizontalPos);
	DDX_Control(pDX, IDC_LBL_SC_VERTICAL_POS,				m_lblScVerticalPos);
	DDX_Control(pDX, IDC_LBL_SC_PROD_LOAD,					m_lblScProdLoad);

	DDX_Control(pDX, IDC_LBL_SC_JOB_JOB_NO,					m_lblScJobJobNo);
	DDX_Control(pDX, IDC_LBL_SC_JOB_JOB_TYP,				m_lblScJobJobTyp);
	DDX_Control(pDX, IDC_LBL_SC_JOB_START_POS,				m_lblScJobStartPos);
	DDX_Control(pDX, IDC_LBL_SC_JOB_DEST_POS,				m_lblScJobDestPos);
	DDX_Control(pDX, IDC_LBL_SC_JOB_JOB_STATUS,				m_lbScJobJobStatus);
	DDX_Control(pDX, IDC_LBL_SC_JOB_BCR_TOP,				m_lblScJobBcrTop);
	DDX_Control(pDX, IDC_LBL_SC_JOB_BCR_BOTTOM,				m_lblScJobBcrBottom);

	DDX_Control(pDX, IDC_LBL_SC_JOB_SC_SUSPEND,             m_lblScSuspend);
	DDX_Control(pDX, IDC_BTN_DUPL_STO,						m_btnDuplSto);


	//GRP
	//SCSTATUS
	//--JOBSTATUS
	//--GRP

	////GRP
	////SCSTATUS
	DDX_Control(pDX, IDC_GRP_SC_SC_STATUS,					m_grpScScStauts);
	DDX_Control(pDX, IDC_GRP_SC_SC_STATUS_ITEM1,			m_grpScScStautsItem1);
	DDX_Control(pDX, IDC_GRP_SC_SC_STATUS_ITEM2,			m_grpScScStautsItem3);
	DDX_Control(pDX, IDC_GRP_SC_SC_STATUS_VALUE1,			m_grpScScStatusValue1);
	DDX_Control(pDX, IDC_GRP_SC_SC_STATUS_VALUE2,			m_grpScScStatusValue3);
	DDX_Control(pDX, IDC_GRP_SC_SC_STATUS_COMMAND,			m_grpScScStautsCommand);
	DDX_Control(pDX, IDC_GRP_SC_JOB_STATUS2,				m_grpScJobStatus2);
	DDX_Control(pDX, IDC_GRP_SC_SUSPEND,					m_grpBottom);
	//--SCSTATUS

	//JOBSTATUS
	DDX_Control(pDX, IDC_GRP_SC_JOB_STATUS,					m_grpScJobStatus);
	DDX_Control(pDX, IDC_GRP_SC_SC_STATUS_ITEM9,		    m_grpScJobStatusItem1);
	DDX_Control(pDX, IDC_GRP_SC_SC_STATUS_VALUE8,			m_grpScJobStatusValue1);
	DDX_Control(pDX, IDC_GRP_SC_JOB_STATUS_COMMAND,			m_grpScJobStatusCommand);

	////--JOBSTATUS
	////--GRP


	//--CSTATIC
	DDX_Control(pDX, IDC_EDT_SC_NO,							m_edtScNo);

	//JOBSTATUS
	DDX_Control(pDX, IDC_EDT_SC_JOB_NO,						m_edtScJobNo);
	DDX_Control(pDX, IDC_CBX_SC_JOB_TYP,					m_cbxScJobTyp);
	DDX_Control(pDX, IDC_CBX_SC_START_POS,					m_cbxScStartPos);
	DDX_Control(pDX, IDC_CBX_SC_DEST_POS,					m_cbxScDestPos);


	//--JOBSTATUS

	//SCSTATUS
	DDX_Control(pDX, IDC_EDT_SC_RC_MODE,					m_edtScRcMode);
	DDX_Control(pDX, IDC_EDT_SC_SC_MODE,					m_edtScScMode);
	DDX_Control(pDX, IDC_EDT_SC_FORK_POS,					m_edtScForkPos);
	DDX_Control(pDX, IDC_EDT_SC_FORK_STATUS,				m_edtScForkStatus);
	DDX_Control(pDX, IDC_EDT_SC_JOB_STATUS,					m_edtScJobStatus);
	DDX_Control(pDX, IDC_EDT_SC_RC_STATUS,					m_edtScRcStatus);
	DDX_Control(pDX, IDC_EDT_SC_SC_STATUS,					m_edtScScStatus);
	DDX_Control(pDX, IDC_EDT_SC_HORIZONTAL_POS,				m_edtScHorizontalPos);
	DDX_Control(pDX, IDC_EDT_SC_VERTICAL_POS,				m_edtScVerticalPos);
	DDX_Control(pDX, IDC_EDT_SC_PROD_LOAD,					m_edtScProdLoad);
	DDX_Control(pDX, IDC_EDT_SC_JOB_SC_SUSPEND,				m_edtScSuspend);
	DDX_Control(pDX, IDC_EDT_SC_JOB_SC_ERR_CODE,			m_edtScErrCode);
	//--SCSTATUS

	//JOBSTATUS
	DDX_Control(pDX, IDC_EDT_SC_JOB_JOB_NO,					m_edtScJobJobNo);
	DDX_Control(pDX, IDC_EDT_SC_JOB_BCR_TOP,				m_edtScJobBcrTop);
	DDX_Control(pDX, IDC_EDT_SC_JOB_BCR_BOTTOM,				m_edtScJobBcrBottom);

	DDX_Control(pDX, IDC_CBX_SC_JOB_JOB_TYP,				m_cbxScJobJobTyp);
	DDX_Control(pDX, IDC_CBX_SC_JOB_START_POS,				m_cbxScJobStartPos);
	DDX_Control(pDX, IDC_CBX_SC_JOB_DEST_POS,				m_cbxScJobDestPos);
	DDX_Control(pDX, IDC_CBX_SC_JOB_JOB_STATUS,				m_cbxScJobJobStatus);
	//--JOBSTATUS

	//COMMAND
	DDX_Control(pDX, IDC_BTN_SC_CONFIRM,					m_btnScConfirm);
	DDX_Control(pDX, IDC_BTN_SC_EMERGENCY,					m_btnScEmergency);
	DDX_Control(pDX, IDC_BTN_SC_ACTIVE,						m_btnScActive);
	DDX_Control(pDX, IDC_BTN_SC_STOP,						m_btnScStop);
	DDX_Control(pDX, IDC_BTN_SC_ERROR_RESET,				m_btnScErrorReset);
	DDX_Control(pDX, IDC_BTN_SC_DELTE,						m_btnScDelete);
	DDX_Control(pDX, IDC_BTN_SC_CALL_TO_HOME,				m_btnScCallToHome);
	DDX_Control(pDX, IDC_BTN_SC_MANUAL,						m_btnScManual);
	DDX_Control(pDX, IDC_BTN_SC_STO_SUSPEND,				m_btnScStoSuspend);
	DDX_Control(pDX, IDC_BTN_SC_RET_SUSPEND,				m_btnScRetSuspend);
	DDX_Control(pDX, IDC_BTN_SC_ALL_SUSPEND,				m_btnScAllSuspend);
	//--COMMAND
	//DDX_Control(pDX, IDC_CHK_SC_FORK1,						m_chkScFork1);
	//DDX_Control(pDX, IDC_CHK_SC_FORK2,						m_chkScFork2);
	//DDX_Control(pDX, IDC_CHK_SC_FORK12,						m_chkScFork1Fork2);
	DDX_Control(pDX, IDC_EDT_SC_JOB_DEST_LOC, m_edtScJobDestLoc);
	DDX_Control(pDX, IDC_EDT_SC_JOB_START_LOC, m_edtScJobStartLoc);
	DDX_Control(pDX, IDC_LBL_SC_JOB_DEST_LOC, m_lblScJobDestLoc);
	DDX_Control(pDX, IDC_LBL_SC_JOB_START_LOC, m_lblScJobStartLoc);
	DDX_Control(pDX, IDC_BTN_SC_MANUAL_RET, m_btnScManualRet);
	DDX_Control(pDX, IDC_EDT_SC_ITN_LUGG_NO, m_edtScItnLuggNo);
	DDX_Control(pDX, IDC_LBL_ITN_LUGG_NO, m_lblItnLuggNo);
}

BEGIN_MESSAGE_MAP(CScSkinDlg, CSkinDialog)
	ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CScSkinDlg::OnMessagSwitch)

	ON_BN_CLICKED(IDC_BTN_SC_CONFIRM, &CScSkinDlg::OnBnClickedBtnScConfirm)

	ON_BN_CLICKED(IDC_BTN_SC_EMERGENCY, &CScSkinDlg::OnBnClickedBtnScEmergency)
	ON_BN_CLICKED(IDC_BTN_SC_ACTIVE, &CScSkinDlg::OnBnClickedBtnScActive)
	ON_BN_CLICKED(IDC_BTN_SC_STOP, &CScSkinDlg::OnBnClickedBtnScStop)
	ON_BN_CLICKED(IDC_BTN_SC_ERROR_RESET, &CScSkinDlg::OnBnClickedBtnScErrorReset)

	ON_BN_CLICKED(IDC_BTN_SC_CALL_TO_HOME, &CScSkinDlg::OnBnClickedBtnScCallToHome)
	ON_BN_CLICKED(IDC_BTN_SC_MANUAL, &CScSkinDlg::OnBnClickedBtnScManual)
	ON_BN_CLICKED(ID_BTN_SC_OK, &CScSkinDlg::OnBnClickedBtnScOk)

	ON_BN_CLICKED(ID_BTN_SC_CANCEL, &CScSkinDlg::OnBnClickedBtnScCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SC_DELTE, &CScSkinDlg::OnBnClickedBtnScDelte)
	ON_BN_CLICKED(IDC_BTN_SC_STO_SUSPEND, &CScSkinDlg::OnBnClickedBtnCvStoSuspend)
	ON_BN_CLICKED(IDC_BTN_SC_RET_SUSPEND, &CScSkinDlg::OnBnClickedBtnScRetSuspend)
	ON_BN_CLICKED(IDC_BTN_SC_ALL_SUSPEND, &CScSkinDlg::OnBnClickedBtnScAllSuspend)
	//ON_BN_CLICKED(IDC_CHK_SC_FORK1, &CScSkinDlg::OnBnClickedChkScFork1)
	//ON_BN_CLICKED(IDC_CHK_SC_FORK2, &CScSkinDlg::OnBnClickedChkScFork2)
	//ON_BN_CLICKED(IDC_CHK_SC_FORK12, &CScSkinDlg::OnBnClickedScRtvFork12)
	ON_BN_CLICKED(IDC_BTN_DUPL_STO, &CScSkinDlg::OnBnClickedScDuplicationSto)
	ON_BN_CLICKED(IDC_BTN_SC_MANUAL_RET, &CScSkinDlg::OnBnClickedBtnScManualRet)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CScSkinDlg::OnInitDialog()
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

	m_strScFork = _T("0");

	InitializeResource(pEn);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScSkinDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("forcecompletion"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_CONFIRM, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("emergencystop"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_EMERGENCY, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("active"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_ACTIVE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stop"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_STOP, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("errorreset"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_ERROR_RESET, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("delete"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_DELTE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("calltohome"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_CALL_TO_HOME, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manualorder"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_MANUAL, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_NO, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_TYP, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_START_POS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_DEST_POS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ground"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_RC_MODE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rising"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_SC_MODE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("horizontaldrive"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_HORIZONTAL_POS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("verticaldrive"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_VERTICAL_POS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("forklocation"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_FORK_POS, strValue);


	//strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	//strValue = CLib::GetIniStringFromPath(strFullPath, _T("forklocation2"), (int)m_enLang);
	//SetDlgItemText(IDC_LBL_SC_FORK_POS2, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("forkstatus"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_FORK_STATUS, strValue);


	//strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	//strValue = CLib::GetIniStringFromPath(strFullPath, _T("forkstatus2"), (int)m_enLang);
	//SetDlgItemText(IDC_LBL_SC_FORK_STATUS2, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("srcstatus"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_RC_STATUS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("activestatus"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_SC_STATUS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("completestatus"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_STATUS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("freightyn"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_PROD_LOAD, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrbottom"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_BCR_BOTTOM, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrtop"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_BCR_TOP, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stosuspend"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_STO_SUSPEND, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("retsuspend"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_RET_SUSPEND, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("allsuspend"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_ALL_SUSPEND, strValue);
	
	//

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_JOB_NO, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_JOB_TYP, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno2"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_JOB_NO2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobstartpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_START_POS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobdestpos"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobstatus"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_JOB_STATUS, strValue);


	//qq
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scsta"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS_ITEM1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS_ITEM2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS_ITEM6, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS_VALUE1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS_VALUE5, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS_VALUE2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("error"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_JOB_STATUS2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scsuspend"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobstatus"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_JOB_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS_ITEM9, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_SC_STATUS_VALUE8, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SC_JOB_STATUS_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_START_LOC, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destloc"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SC_JOB_DEST_LOC, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manualret"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_MANUAL_RET, strValue);
}

void CScSkinDlg::RedrawImage()
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

	m_btnScConfirm.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScConfirm.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("forcecompletion"), strExtension)), NULL, 5, 5);

	m_btnScEmergency.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0 ,0);
	m_btnScEmergency.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("emergencystop"), strExtension)), NULL, 5, 5);

	m_btnScActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("active"), strExtension)), NULL, 5, 5);

	m_btnScStop.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScStop.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("stop"), strExtension)), NULL, 5, 5);

	m_btnScErrorReset.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScErrorReset.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("errorreset"), strExtension)), NULL, 5, 5);

	m_btnScDelete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScDelete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);

	m_btnScCallToHome.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScCallToHome.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("calltohome"), strExtension)), NULL, 5, 5);

	m_btnScManual.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScManual.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("manual"), strExtension)), NULL, 5, 5);

	m_btnScStoSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScStoSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("save"), strExtension)), NULL, 5, 5);

	m_btnScRetSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScRetSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("save"), strExtension)), NULL, 5, 5);

	m_btnScAllSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScAllSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("save"), strExtension)), NULL, 5, 5);

	m_btnScManualRet.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScManualRet.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("save"), strExtension)), NULL, 5, 5);
}
void CScSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}
HCURSOR CScSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CScSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CScSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;


	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnScConfirm.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScConfirm.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScEmergency.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScEmergency.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScActive.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScActive.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScStop.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScStop.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScErrorReset.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScErrorReset.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScDelete.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScDelete.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScCallToHome.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScCallToHome.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScStoSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScStoSuspend.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScRetSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScRetSuspend.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScAllSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScAllSuspend.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnScManualRet.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScManualRet.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}


BOOL CScSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CScSkinDlg::InvalidateScData(EN_LANG pLang)
{	

	if(m_pDoc == NULL) return;
	if(m_pSC_DATA == NULL) return;

	if (!UpdateData())
		return;

	CString strScNo = m_pSC_DATA->K_SC_NO;//SC 번호
	CString strStartHsPosFk1 = m_pSC_DATA->V_START_HSPOS_FK1_RD;
	CString strStartHsPosFk2 = m_pSC_DATA->V_START_HSPOS_FK2_RD;
	CString strStartBank1 = m_pSC_DATA->V_START_BANK_FK1_RD;
	CString strStartBank2 = m_pSC_DATA->V_START_BANK_FK2_RD;
	CString strDestHsPosFk1 = m_pSC_DATA->V_DEST_HSPOS_FK1_RD;
	CString strDestHsPosFk2 = m_pSC_DATA->V_DEST_HSPOS_FK2_RD;
	CString strDestBank1 = m_pSC_DATA->V_DEST_BANK_FK1_RD;
	CString strDestBank2 = m_pSC_DATA->V_DEST_BANK_FK2_RD;
	CString strStartLocation1;
	CString strStartLocation2;
	CString strDestLocation1;
	CString strDestLocation2;

	strStartLocation1.Format(_T("%02s-%03s-%02s"), m_pSC_DATA->V_START_BANK_FK1_RD, m_pSC_DATA->V_START_BAY_FK1_RD, m_pSC_DATA->V_START_LEVEL_FK1_RD);
	strStartLocation2.Format(_T("%02s-%03s-%02s"), m_pSC_DATA->V_START_BANK_FK2_RD, m_pSC_DATA->V_START_BAY_FK2_RD, m_pSC_DATA->V_START_LEVEL_FK2_RD);
	strDestLocation1.Format(_T("%02s-%03s-%02s"), m_pSC_DATA->V_DEST_BANK_FK1_RD, m_pSC_DATA->V_DEST_BAY_FK1_RD, m_pSC_DATA->V_DEST_LEVEL_FK1_RD);
	strDestLocation2.Format(_T("%02s-%03s-%02s"), m_pSC_DATA->V_DEST_BANK_FK2_RD, m_pSC_DATA->V_DEST_BAY_FK2_RD, m_pSC_DATA->V_DEST_LEVEL_FK2_RD);

	//StartHsPosFk 값이 0일 시 Bank 값을 넣어준다.
	if (strStartHsPosFk1 == _T("0"))
	{
		m_cbxScStartPos.SetWindowText(strStartLocation1);
	}else{
		m_cbxScStartPos.SetWindowText(strStartHsPosFk1);
	}

	//DestHsPosFk 값이 0일 시 Bank 값을 넣어준다.
	if (strDestHsPosFk1 == _T("0"))
	{
		m_cbxScDestPos.SetWindowText(strDestLocation1);
	}else{
		m_cbxScDestPos.SetWindowText(strDestHsPosFk1);
	}

	m_edtScNo.SetWindowText(m_pSC_DATA->V_MC_NO + " (" + m_pSC_DATA->V_MC_NO_NM + ")");//SC 명
	m_edtScJobNo.SetWindowText(m_pSC_DATA->V_LUGG_NO_FK1_RD);//작업번호1
	m_edtScVerticalPos.SetWindowText(m_pSC_DATA->V_POS_V_RD);//수직주행
	m_edtScHorizontalPos.SetWindowText(m_pSC_DATA->V_POS_H_RD);//수평주행
	m_edtScItnLuggNo.SetWindowText(m_pSC_DATA->V_ITN_LUGG_FK1);//내부작업번호


	CString strSql = GetQrySelectSC_STATUS_CCD(m_pSC_DATA);
	CString strMessage;
	int nRowCnt = -1;
	_RecordsetPtr ptr =  m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if(nRowCnt <= 0){	return;	  }
	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
	pRsw->MoveFirst(); 
	for(int i = 0; i < nRowCnt; i++)
	{
		CString strJOB_TYP_TEMP = pRsw->GetItem(_T("JOB_TYP_RD"));
		m_cbxScJobTyp.SetWindowText(strJOB_TYP_TEMP);//작업구분1
		m_edtScRcMode.SetWindowText(pRsw->GetItem(_T("ONLINE_MODE_RD")));//지상반
		m_edtScScMode.SetWindowText(pRsw->GetItem(_T("AUTO_MODE_RD")));//기상반
		m_edtScRcStatus.SetWindowText(pRsw->GetItem(_T("ACTIVE_MODE_RD")));//SRC상태
		m_edtScScStatus.SetWindowText(pRsw->GetItem(_T("UCSTATUS_RD")));//동작상태
		m_edtScForkPos.SetWindowText(pRsw->GetItem(_T("FORKPOS_FK1_RD")));//포크위치1
		m_edtScForkStatus.SetWindowText(pRsw->GetItem(_T("ERR_STA_FK1_RD")));//포크상태
		m_edtScJobStatus.SetWindowText(pRsw->GetItem(_T("COMPLETE_RD")));//완료상태
		m_edtScProdLoad.SetWindowText(pRsw->GetItem(_T("SENSOR_FK_RD")));//화물유무
		m_edtScSuspend.SetWindowText(pRsw->GetItem(_T("SUSPEND")));//SUSPEND
	}
	delete pRsw;


	//SK
	CString strGetErrorCode = _T("");
	GetErrorCode(_T("SC"), m_pSC_DATA->V_ERR_CODE_RD, (int)pLang, strGetErrorCode); 
	m_edtScErrCode.SetWindowText(strGetErrorCode);//에러코드
	//m_edtScErrCode.SetWindowText(m_pSC_DATA->V_ERR_CODE_RD);//에러코드

	m_edtScJobJobNo.SetWindowText(_T(""));
	m_cbxScJobJobStatus.SetWindowText(_T(""));
	m_cbxScJobStartPos.SetWindowText(_T(""));
	m_cbxScJobDestPos.SetWindowText(_T(""));
	m_edtScJobStartLoc.SetWindowText(_T(""));
	m_edtScJobDestLoc.SetWindowText(_T(""));
	m_edtScJobBcrTop.SetWindowText(_T(""));
	m_edtScJobBcrBottom.SetWindowText(_T(""));
	m_cbxScJobJobTyp.SetWindowText(_T(""));

	if(m_pSC_DATA->V_LUGG_NO_FK1_RD != _T("0"))
	{
		strSql = GetQrySelectJOB_MST_FK1(m_pSC_DATA);
		nRowCnt = -1;
		_RecordsetPtr ptr =  m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
		if(nRowCnt <= 0){	return;	  }
		CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
		pRsw->MoveFirst(); 
		for(int i = 0; i < nRowCnt; i++)
		{
			m_edtScJobJobNo.SetWindowText(pRsw->GetItem(_T("LUGG_NO")));
			m_cbxScJobStartPos.SetWindowText(pRsw->GetItem(_T("START_POS")));
			m_cbxScJobDestPos.SetWindowText(pRsw->GetItem(_T("DEST_POS")));
			m_edtScJobStartLoc.SetWindowText(pRsw->GetItem(_T("START_LOCATION")));
			m_edtScJobDestLoc.SetWindowText(pRsw->GetItem(_T("DEST_LOCATION")));
			m_cbxScJobJobStatus.SetWindowText(pRsw->GetItem(_T("JOB_STATUS")));
			m_edtScJobBcrTop.SetWindowText(pRsw->GetItem(_T("BCR_TOP")));
			m_edtScJobBcrBottom.SetWindowText(pRsw->GetItem(_T("BCR_BOTTOM")));
			m_cbxScJobJobTyp.SetWindowText(pRsw->GetItem(_T("JOB_TYP")));
			pRsw->MoveNext();
		}
		delete pRsw;
	}
	InvalidateLangControl(pLang);
}	

void CScSkinDlg::InvalidateLangControl( EN_LANG enLangTemp )
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


LRESULT CScSkinDlg::OnMessagSwitch(WPARAM wParam, LPARAM lParam)
{
	CSC_DATA* pSC_DATA = (CSC_DATA*)wParam;
	if(pSC_DATA == NULL)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC를 찾을수 없습니다")));
		return 0;
	}

	m_pSC_DATA = pSC_DATA;

	EN_LANG enLangTemp = (EN_LANG)lParam;
	if(m_nLang != enLangTemp)
	{
		m_nLang = enLangTemp;
		InvalidateLangControl(enLangTemp);
	}

	InvalidateScData(enLangTemp);

	return 0;
}

void CScSkinDlg::SetButtonScImage( CButton& pButton, CString pFieldName, int pValue )
{
	//HICON hIcon = Global.GetIcon(Global.GetCvIconIndex(pFieldName, pValue));
	//pButton.SetIcon(hIcon);
}

//강제완료
void CScSkinDlg::OnBnClickedBtnScConfirm()
{

	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateScData(EN_BtnScConfirm);
}

//비상정지
void CScSkinDlg::OnBnClickedBtnScEmergency()
{

	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateScData(EN_BtnScEmergency);
}

//ACTIVE
void CScSkinDlg::OnBnClickedBtnScActive()
{

	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateScData(EN_BtnScActive);
}

//정지
void CScSkinDlg::OnBnClickedBtnScStop()
{
	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateScData(EN_BtnScStop);
}

//에러리셋
void CScSkinDlg::OnBnClickedBtnScErrorReset()
{

	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateScData(EN_BtnScErrReset);
}

//삭제
void CScSkinDlg::OnBnClickedBtnScDelte()
{

	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateScData(EN_BtnScFk1Delete);

	//if (m_strScFork == _T("0")) 
	//{
	//	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("포크를 선택하세요.")));
	//	return;
	//}
	//else if (m_strScFork == _T("1"))
	//{
	//	UpdateScData(EN_BtnScFk1Delete);
	//}
	//else if (m_strScFork == _T("2"))
	//{
	//	UpdateScData(EN_BtnScFk2Delete);
	//}
	//else if (m_strScFork == _T("3"))
	//{
	//	UpdateScData(EN_BtnScFk1Fk2Delete);
	//}
}
//홈복귀
void CScSkinDlg::OnBnClickedBtnScCallToHome()
{

	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateScData(EN_BtnScCallToHome);
}

//사용안함
void CScSkinDlg::OnBnClickedBtnScManual()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CScSkinDlg::OnBnClickedBtnScOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CScSkinDlg::OnBnClickedBtnScCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc->m_pScSkinDlg = NULL;
	//CSkinDialog::OnClose();
	//OnClose();
}

void CScSkinDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pDoc->m_pScSkinDlg = NULL;
	delete this;
}

void CScSkinDlg::UpdateScData(int nBtnJob)
{
	CString strSql=_T("");
	CString strWhTyp = _T("");
	CString strScNo = _T("");
	CString strPlcNo = _T("");
	CString strScGrpNo = _T("");
	CString strCmdId=_T("");
	CString strMsg = _T("");
	CString strFK = _T("");
	UpdateData(TRUE);

	strWhTyp = m_pDoc->m_WH_TYP;
	strScNo = m_pSC_DATA->K_SC_NO;
	strPlcNo = m_pSC_DATA->K_PLC_NO;



	//CMD_RQ_ID 공통코드 참조
	if (     nBtnJob == EN_BtnScConfirm  )  { strCmdId = _T("FCMP"); strFK = _T("0");  strMsg = _T("강제완료를 진행하시겠습니까?");   } //강제완료
	else if (nBtnJob == EN_BtnScEmergency)  { strCmdId = _T("EMERGENCY"); strFK = _T("0"); strMsg = _T("비상정지를 진행하시겠습니까?");   } //비상정지
	else if (nBtnJob == EN_BtnScActive)     { strCmdId = _T("ACTIVE"); strFK = _T("0"); strMsg = _T("ACTIVE를 진행하시겠습니까?");	 } //ACTIVE
	else if (nBtnJob == EN_BtnScStop)	    { strCmdId = _T("PAUSE"); strFK = _T("0"); strMsg = _T("STOP을 진행하시겠습니까?");		 } //정지
	else if (nBtnJob == EN_BtnScErrReset)   { strCmdId = _T("RESET"); strFK = _T("0"); strMsg = _T("ERROR RESET을 진행하시겠습니까?");} //ERROR RESET
	else if (nBtnJob == EN_BtnScFk1Delete)  { strCmdId = _T("DELFK1"); strFK = _T("1");   strMsg = _T("Fork1 삭제를 진행하시겠습니까?");		 } //삭제
	else if (nBtnJob == EN_BtnScFk2Delete)  { strCmdId = _T("DELFK2"); strFK = _T("2");   strMsg = _T("Fork2 삭제를 진행하시겠습니까?");		 } //삭제
	else if (nBtnJob == EN_BtnScFk1Fk2Delete)  { strCmdId = _T("DELFK12"); strFK = _T("0");  strMsg = _T("Fork1,2 삭제를 진행하시겠습니까?");		 } //삭제
	else if (nBtnJob == EN_BtnScCallToHome) { strCmdId = _T("CTH"); strFK = _T("0");  strMsg = _T("홈복귀를 진행하시겠습니까?");	 } //홈복귀
	
 	if (AfxMessageBox(m_pDoc->GetMsgLangDef(strMsg), MB_YESNO) != IDYES)
		return;

	if (strCmdId == _T("FCMP"))
	{
		
		CString strSensorFkRd = m_pSC_DATA->V_SENSOR_FK_RD; //sc 화물
		CString strForkPosRd = m_pSC_DATA->V_FORKPOS_FK1_RD; //포크 위치

		//SC 화물 없어야됨.
		if (strSensorFkRd == _T("1") || strSensorFkRd == _T("3"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC에 화물이 있어서 강제완료를 할 수 없습니다.")));
			return;
		}

		//Sc 포크 센터여야 함.
		if (strForkPosRd != _T("0"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 포크가 센터가 아니어서 강제완료를 할 수 없습니다.")));
			return;
		}
	}

	//공출고, 이중입고일 때 에러처리
	if (strCmdId == _T("RESET"))
	{
		if (m_pSC_DATA->V_ERR_CODE_RD == _T("0058") || m_pSC_DATA->V_ERR_CODE_RD == _T("0059"))
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 공출고 에러상태 입니다.\n에러해제 시 수동조작을 해야합니다.\n그래도 하시겠습니까?")), MB_YESNO) != IDYES)
				return;
		}

		if (m_pSC_DATA->V_ERR_CODE_RD == _T("0054") || m_pSC_DATA->V_ERR_CODE_RD == _T("0055"))
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 이중입고 에러상태 입니다.\n에러해제 시 수동조작을 해야합니다.\n그래도 하시겠습니까?")), MB_YESNO) != IDYES)
				return;
		}
	}

	m_pDoc->BeginTrans_DLG();

	//공출고 일 때 삭제처리
	if(strCmdId == _T("DELFK1"))
	{
		if (m_pSC_DATA->V_ERR_CODE_RD == _T("0058") || m_pSC_DATA->V_ERR_CODE_RD == _T("0059"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 공출고 에러상태 입니다.\nECS 작업도 삭제됩니다.\nWMS 작업도 삭제해주세요.")));

			//가지고있는 작업정보 삭제
			CString strSql = _T("");
			int nRowCnt = 0;
			CString strMessage = _T("");

			CString strScNo = m_pSC_DATA->K_SC_NO;//SC 번호
			CString strLuggNo = m_pSC_DATA->V_ITN_LUGG_FK1;//SC작업번호

			strSql.Format(_T(" SELECT *					 ")
				_T("		     FROM JOB_MST                     ")
				_T("		    WHERE WH_TYP      = '%s'          ")
				_T("		      AND LUGG_NO   = '%s'       "), m_pDoc->m_WH_TYP, strLuggNo);


			_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);

			//작업정보가 있는 경우에만 삭제
 			if (nRowCnt > 0)
			{
				strSql.Format(_T(" DELETE FROM JOB_MST					 ")
					_T("		    WHERE WH_TYP  = '%s'          ")
					_T("		      AND LUGG_NO = '%s'       "), m_pDoc->m_WH_TYP, strLuggNo);

				BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

				if(isSuccess == TRUE)
				{
				}
				else
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
		}
	}

	strSql.Format(_T(" UPDATE SC_DATA						  \n")
		_T("    SET CMD_RQ_ID = '%s'						  \n")
		_T("	  , CMD_RQ_YN = 'Y'							  \n")
		_T("	  , READ_UPD_DT = ") + m_pDoc->SYSDATE + _T(" \n") 
		_T("  WHERE WH_TYP = '%s'							  \n")
		_T("	AND PLC_NO = '%s'							  \n")
		_T("    AND MC_NO = '%s'							   "), strCmdId, strWhTyp, strPlcNo, strScNo);

	BOOL IsSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(IsSuccess == TRUE)
	{
		CString strLOG_LUGG_NO1 = m_pSC_DATA->V_ITN_LUGG_FK1;
		if (strLOG_LUGG_NO1 == _T("")) { strLOG_LUGG_NO1 = _T("0");}
		CString strLOG_LUGG_NO2 = m_pSC_DATA->V_ITN_LUGG_FK2;
		if (strLOG_LUGG_NO2 == _T("")) { strLOG_LUGG_NO2 = _T("0");}
		CString strLOG_BOTTOM_TRAY = _T("");
		CString strLOG_TOP_TRAY = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("SC ACTION BUTTON -> SC NO : %s , ACTION : %s"), strScNo, strCmdId);

		if (strFK == _T("1"))
		{
			if (strLOG_LUGG_NO1 != _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
			else
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
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
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO2, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
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
					if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
					{
						m_pDoc->RollbackTrans_DLG();
						return;
					}
				}
			}
			else if (strLOG_LUGG_NO1 != _T("0") && strLOG_LUGG_NO2 == _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
			else if (strLOG_LUGG_NO1 == _T("0") && strLOG_LUGG_NO2 != _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO2, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
			else
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
				if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO2, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
		}
		

		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		SetScStatus(strCmdId, _T("")); 
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;
}


void CScSkinDlg::InitializeResource(EN_LANG nEN_LANG)
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

void CScSkinDlg::OnBnClickedBtnCvStoSuspend()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strScNo = _T("");
	CString strPlcNo = m_pSC_DATA->K_PLC_NO;
	CString strMsg = _T("");
	CString strSUSPEND = _T("");
	int nSUSPEND = 0;

	UpdateData(TRUE);


	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	strScNo = m_pSC_DATA->K_SC_NO;
	nSUSPEND = CConvert::ToInt(m_pSC_DATA->V_SUSPEND);
	
	//현재 SUSPEND 상황에 따른 버튼처리
	switch(nSUSPEND)
	{
	case 0 : 
		strSUSPEND = _T("1"); //정상 -> 입고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입고금지 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	case 1 :
		strSUSPEND = _T("0"); //입고정지 -> 정상
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 정상처리 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	case 2 :
		strSUSPEND = _T("3"); //출고정지 -> 입출고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입출고금지 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	case 3 :
		strSUSPEND = _T("2"); //입출고정지 -> 출고금지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 출고금지 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	}

	strSql = GetUpdateQry(strWhTyp, strPlcNo, strScNo, strSUSPEND);

	m_pDoc->BeginTrans_DLG();

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO =  m_pSC_DATA->V_ITN_LUGG_FK1;
		if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
		CString strLOG_BOTTOM_TRAY = _T("");
		CString strLOG_TOP_TRAY = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("SC SUSPEND STO BUTTON -> SC NO : %s , 기존 %d , 변경 %s"), strScNo, nSUSPEND, strSUSPEND);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		SetScStatus(_T("SUSPEND"), strSUSPEND); 
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;	
}


void CScSkinDlg::OnBnClickedBtnScRetSuspend()
{

	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strScNo = _T("");
	CString strPlcNo = m_pSC_DATA->K_PLC_NO;
	CString strMsg = _T("");
	CString strSUSPEND = _T("");
	int nSUSPEND = 0;

	UpdateData(TRUE);

	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	strScNo = m_pSC_DATA->K_SC_NO;
	nSUSPEND = CConvert::ToInt(m_pSC_DATA->V_SUSPEND);
	
	//현재 SUSPEND 상황에 따른 버튼처리
	switch(nSUSPEND)
	{
	case 0 : 
		strSUSPEND = _T("2"); //정상 -> 출고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 출고금지 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	case 1 :
		strSUSPEND = _T("3"); //입고정지 -> 입출고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입출고금지 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	case 2 :
		strSUSPEND = _T("0"); //출고정지 -> 정상처리
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 정상처리 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	case 3 :
		strSUSPEND = _T("1"); //입출고정지 -> 입고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입고금지 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	}

	strSql = GetUpdateQry(strWhTyp, strPlcNo, strScNo, strSUSPEND);

	m_pDoc->BeginTrans_DLG();

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO =  m_pSC_DATA->V_ITN_LUGG_FK1;
		if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
		CString strLOG_BOTTOM_TRAY = _T("");
		CString strLOG_TOP_TRAY = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("SC SUSPEND RET BUTTON -> SC NO : %s , 기존 %d , 변경 %s"), strScNo, nSUSPEND, strSUSPEND);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		SetScStatus(_T("SUSPEND"), strSUSPEND); 
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;	
}


void CScSkinDlg::OnBnClickedBtnScAllSuspend()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strScNo = _T("");
	CString strPlcNo = m_pSC_DATA->K_PLC_NO;
	CString strMsg = _T("");
	CString strSUSPEND = _T("");
	int nSUSPEND = 0;

	UpdateData(TRUE);

	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	strScNo = m_pSC_DATA->K_SC_NO;
	nSUSPEND = CConvert::ToInt(m_pSC_DATA->V_SUSPEND);
	
	//현재 SUSPEND 상황에 따른 버튼처리
	switch(nSUSPEND)
	{
	case 0 : 
		strSUSPEND = _T("3"); //정상 -> 입출고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입출고금지 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	case 1 :
		strSUSPEND = _T("3"); //입고정지 -> 입출고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입출고금지 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	case 2 :
		strSUSPEND = _T("3"); //출고정지 -> 입출고정지
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 입출고금지 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	case 3 :
		strSUSPEND = _T("0"); //입출고정지 -> 정상처리
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("S/C 정상처리 하시겠습니까?")), MB_YESNO) != IDYES)	return;	
		break;
	}

	strSql = GetUpdateQry(strWhTyp, strPlcNo, strScNo, strSUSPEND);

	m_pDoc->BeginTrans_DLG();

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO =  m_pSC_DATA->V_ITN_LUGG_FK1;
		if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
		CString strLOG_BOTTOM_TRAY = _T("");
		CString strLOG_TOP_TRAY = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("SC SUSPEND STOP BUTTON -> SC NO : %s , 기존 %d , 변경 %s"), strScNo, nSUSPEND, strSUSPEND);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		SetScStatus(_T("SUSPEND"), strSUSPEND); 
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;	
}



void CScSkinDlg::OnBnClickedChkScFork1()
{

	UpdateData(TRUE);

	if (m_chkScFork1.GetCheck() == 1)
	{
		m_strScFork = _T("1");
		//Enable 처리
		m_chkScFork1.EnableWindow(TRUE);
		m_chkScFork2.EnableWindow(FALSE);
		m_chkScFork1Fork2.EnableWindow(FALSE);
	}else{
		m_strScFork = _T("0");
		m_chkScFork1.EnableWindow(TRUE);
		m_chkScFork2.EnableWindow(TRUE);
		m_chkScFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}


void CScSkinDlg::OnBnClickedChkScFork2()
{
	UpdateData(TRUE);

	if (m_chkScFork2.GetCheck() == 1)
	{
		m_strScFork = _T("2");
		//Enable 처리
		m_chkScFork1.EnableWindow(FALSE);
		m_chkScFork2.EnableWindow(TRUE);
		m_chkScFork1Fork2.EnableWindow(FALSE);
	}else{
		m_strScFork = _T("0");
		m_chkScFork1.EnableWindow(TRUE);
		m_chkScFork2.EnableWindow(TRUE);
		m_chkScFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}

void CScSkinDlg::OnBnClickedScDuplicationSto()
{
	UpdateData(TRUE);

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	if(m_pSC_DATA == NULL) return;

	CString strScNo = m_pSC_DATA->K_SC_NO;//SC 번호
	CString strLuggNo = m_pSC_DATA->V_LUGG_NO_FK1_RD;//SC 번호

	strSql.Format(_T(" SELECT *					 ")
		_T("		     FROM SC_DATA                     ")
		_T("		    WHERE WH_TYP      = '%s'          ")
		_T("		      AND EQP_ERR_CD   = 'E054'       ")
		_T("		      AND SC_NO   = '%s'       "), m_pDoc->m_WH_TYP, strScNo);


	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);

 	if (nRowCnt > 0)
	{
		strSql.Format(_T(" UPDATE JOB_MST					 ")
			_T("		      SET JOB_STA = '44'                     ")
			_T("		    WHERE WH_TYP  = '%s'          ")
			_T("		      AND LUGG_NO = '%s'       "), m_pDoc->m_WH_TYP, strLuggNo);

		BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if(isSuccess == TRUE)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
			return;
		}
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	}
	else
	{

	}
	

}

void CScSkinDlg::OnBnClickedScRtvFork12()
{
	UpdateData(TRUE);

	if (m_chkScFork1Fork2.GetCheck() == 1)
	{
		m_strScFork = _T("3");
		//Enable 처리
		m_chkScFork1.EnableWindow(FALSE);
		m_chkScFork2.EnableWindow(FALSE);
		m_chkScFork1Fork2.EnableWindow(TRUE);
	}else{
		m_strScFork = _T("0");
		m_chkScFork1.EnableWindow(TRUE);
		m_chkScFork2.EnableWindow(TRUE);
		m_chkScFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}

void CScSkinDlg::GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode)
{
	if(m_pDoc == NULL){ return; };
	CStringList strList;
	CString strSql;
	CString strMSG_KOR, strMSG_CHIN, strMSG_ENG, strMSG_HUN;
	int nRowCnt = 0, j=0;
	CString strMessage;

	
	strSql.Format(_T(" SELECT MSG_KOR, \n")
		 _T(" ") + m_pDoc->NVL + _T("(MSG_CHIN,MSG_KOR) AS MSG_CHIN, \n")
		 _T(" ") + m_pDoc->NVL + _T("(MSG_ENG,MSG_KOR) AS MSG_ENG, \n")
		 _T(" ") + m_pDoc->NVL + _T("(MSG_HUN,MSG_KOR) AS MSG_HUN  \n ")
	     _T("    FROM EQP_ECD_MST                    \n ")
	     _T("   WHERE EQP_TYP      = '%s'             \n  ")
	     _T("    AND EQP_ERR_CD   = '%04s'            \n   "), strEqpTyp, strErrorCode);
	

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr(strSql, nRowCnt, strMessage);
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


void CScSkinDlg::GetCommonCode(CString pWH_TYP, CString pSC_NO)
{
	CString strSql = GetSelectQry(pWH_TYP, pSC_NO);
	int nRowCnt = 0;
	CString strMessage;

	_RecordsetPtr pRsptr = NULL; //m_pDoc->m_pUrmDBAccess->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	
	pRsw->MoveFirst(); 
	
	for(int i = 0; i < nRowCnt; i++)
	{
		
	}

	delete pRsw;
	
}

CString CScSkinDlg::GetUpdateQry(CString pWH_TYP, CString pPLC_NO, CString pSC_NO, CString pSuspend)
{
	CString strSql;

	strSql.Format(_T(" UPDATE SC_DATA		\n")
			      _T("    SET SUSPEND = '%s' \n")
				  _T("  WHERE WH_TYP = '%s'			\n")
				  _T("	  AND PLC_NO = '%02s'			\n")
				  _T("    AND SC_NO = '%s'			"),  pSuspend, pWH_TYP, pPLC_NO, pSC_NO);

	return strSql;
}
CString CScSkinDlg::GetSelectQry(CString pWH_TYP, CString pSC_NO)
{
	CString strSql;

	
	strSql.Format(_T("                                                                           ")
		      _T("      SELECT SC.*																		 ")
		      _T("           , ") + m_pDoc->NVL + _T("(CM1.CCD_NM_KOR,ONLINE_MODE_RD) AS ONLINE_MODE_RD_KOR      ")
		      _T("           , ") + m_pDoc->NVL + _T("(CM2.CCD_NM_KOR,AUTO_MODE_RD) AS AUTO_MODE_RD_KOR          ")
		      _T("           , ") + m_pDoc->NVL + _T("(CM3.CCD_NM_KOR,ACTIVE_MODE_RD) AS ACTIVE_MODE_RD_KOR      ")
		      _T("           , ") + m_pDoc->NVL + _T("(CM4.CCD_NM_KOR,UCSTATUS_RD) AS UCSTATUS_RD_KOR            ")
		      _T("           , ") + m_pDoc->NVL + _T("(CM5.CCD_NM_KOR,ERR_STA_FK1_RD) AS ERR_STA_RD_KOR          ")
		      _T("           , ") + m_pDoc->NVL + _T("(CM6.CCD_NM_KOR,ERR_STA_FK2_RD) AS ERR_STA_RD_KOR          ")
		      _T("           , ") + m_pDoc->NVL + _T("(CM7.CCD_NM_KOR,COMPLETE_RD) AS COMPLETE_RD_KOR            ")
		      _T("           , ") + m_pDoc->NVL + _T("(CM8.CCD_NM_KOR,SENSOR_FK_RD) AS SENSOR_FK_RD_KOR          ")
		      _T("        FROM SC_DATA SC                                                    ")
		      _T("    LEFT JOIN (                                                            ")
		      _T("                SELECT *                                                   ")
		      _T("                FROM COMMON_CODE                                           ")
		      _T("                WHERE CDX_CD = 'ONLINE_MODE_RD'                            ")
		      _T("            ) CM1                                                          ")
		      _T("            ON SC.ONLINE_MODE_RD = CM1.CCD_CD                              ")
		      _T("    LEFT JOIN (                                                            ")
		      _T("                SELECT *                                                   ")
		      _T("                FROM COMMON_CODE                                           ")
		      _T("                WHERE CDX_CD = 'AUTO_MODE_RD'                              ")
		      _T("            ) CM2                                                          ")
		      _T("            ON SC.AUTO_MODE_RD = CM2.CCD_CD                                ")
		      _T("    LEFT JOIN (                                                            ")
		      _T("                SELECT *                                                   ")
		      _T("                FROM COMMON_CODE                                           ")
		      _T("                WHERE CDX_CD = 'ACTIVE_MODE_RD'                            ")
		      _T("            ) CM3                                                          ")
		      _T("            ON SC.ACTIVE_MODE_RD = CM3.CCD_CD                              ")
		      _T("    LEFT JOIN (                                                            ")
		      _T("                SELECT *                                                   ")
		      _T("                FROM COMMON_CODE                                           ")
		      _T("                WHERE CDX_CD = 'UCSTATUS_RD'                               ")
		      _T("            ) CM4                                                          ")
		      _T("            ON SC.UCSTATUS_RD = CM4.CCD_CD                                 ")
		      _T("    LEFT JOIN (                                                            ")
		      _T("                SELECT *                                                   ")
		      _T("                FROM COMMON_CODE                                           ")
		      _T("                WHERE CDX_CD = 'ERR_STA_RD'                                ")
		      _T("            ) CM5                                                          ")
		      _T("            ON SC.ERR_STA_FK1_RD = CM5.CCD_CD                              ")
		      _T("    LEFT JOIN (                                                            ")
		      _T("                SELECT *                                                   ")
		      _T("                FROM COMMON_CODE                                           ")
		      _T("                WHERE CDX_CD = 'ERR_STA_RD'                                ")
		      _T("            ) CM6                                                          ")
		      _T("            ON SC.ERR_STA_FK2_RD = CM6.CCD_CD                              ")
		      _T("    LEFT JOIN (                                                            ")
		      _T("                SELECT *                                                   ")
		      _T("                FROM COMMON_CODE                                           ")
		      _T("                WHERE CDX_CD = 'COMPLETE_RD'                               ")
		      _T("            ) CM7                                                          ")
		      _T("            ON SC.COMPLETE_RD = CM7.CCD_CD                                 ")
		      _T("    LEFT JOIN (                                                            ")
		      _T("                SELECT *                                                   ")
		      _T("                FROM COMMON_CODE                                           ")
		      _T("                WHERE CDX_CD = 'SENSOR_FK_RD'                              ")
		      _T("            ) CM8                                                          ")
		      _T("            ON SC.SENSOR_FK_RD = CM8.CCD_CD                                ")
		      _T("        WHERE SC.WH_TYP = '%s'                                             ")
		      _T("        AND SC.SC_NO = '%s'                                               "), pWH_TYP, pSC_NO);

	return strSql;
}

CString CScSkinDlg::GetQrySelectJOB_MST_FK1( CSC_DATA * pSC_DATA )
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
	strSql += CRLF + _T("  FROM JOB_MST JM LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP");
	strSql += CRLF + _T("                          ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.CCD_CD = JM.JOB_TYP ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.WH_TYP LIKE ") + CLib::QuotLikeLR(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_JOB_STATUS ");
	strSql += CRLF + _T("                          ON CCD_JOB_STATUS.CDX_CD = 'JOB_STATUS' ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.CCD_CD = JM.JOB_STATUS ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.WH_TYP LIKE ") + CLib::QuotLikeLR(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T(" WHERE JM.WH_TYP = ") + CLib::Quot(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND JM.LUGG_NO = ") + CLib::Quot(pSC_DATA->V_ITN_LUGG_FK1);
	//strSql += CRLF + _T("   AND JM.LUGG_NO = ") + CLib::Quot(pSC_DATA->V_LUGG_NO_FK1_RD);

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CScSkinDlg::GetQrySelectJOB_MST_FK2( CSC_DATA * pSC_DATA )
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
	strSql += CRLF + _T("  FROM JOB_MST JM LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP");
	strSql += CRLF + _T("                          ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.CCD_CD = JM.JOB_TYP ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.WH_TYP LIKE ") + CLib::QuotLikeLR(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_JOB_STATUS ");
	strSql += CRLF + _T("                          ON CCD_JOB_STATUS.CDX_CD = 'JOB_STATUS' ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.CCD_CD = JM.JOB_STATUS ");
	strSql += CRLF + _T("                         AND CCD_JOB_STATUS.WH_TYP LIKE ") + CLib::QuotLikeLR(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T(" WHERE JM.WH_TYP = ") + CLib::Quot(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND JM.LUGG_NO = ") + CLib::Quot(pSC_DATA->V_LUGG_NO_FK2_RD);
	
	
	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CScSkinDlg::GetQrySelectSC_STATUS_CCD( CSC_DATA* pSC_DATA )
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	
	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR,'10') AS JOB_TYP_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ONLINE_MODE_RD.CCD_NM_KOR, '0') AS ONLINE_MODE_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_AUTO_MODE_RD.CCD_NM_KOR, '0') AS AUTO_MODE_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ACTIVE_MODE_RD.CCD_NM_KOR, '0') AS ACTIVE_MODE_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_UCSTATUS_RD.CCD_NM_KOR, '0') AS UCSTATUS_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_FORKPOS_FK1_RD.CCD_NM_KOR, '0') AS FORKPOS_FK1_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_FORKPOS_FK2_RD.CCD_NM_KOR, '0') AS FORKPOS_FK2_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ERR_STA_FK1_RD.CCD_NM_KOR, '0') AS ERR_STA_FK1_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ERR_STA_FK2_RD.CCD_NM_KOR, '0') AS ERR_STA_FK2_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_COMPLETE_RD.CCD_NM_KOR, '0') AS COMPLETE_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_SENSOR_FK_RD.CCD_NM_KOR, '0') AS SENSOR_FK_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_SUSPEND.CCD_NM_KOR, '0') AS SUSPEND ");
	strSql += CRLF + _T("  FROM SC_DATA SD LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP");
	strSql += CRLF + _T("                          ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.CCD_CD = SD.JOB_TYP_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ONLINE_MODE_RD ");
	strSql += CRLF + _T("                          ON CCD_ONLINE_MODE_RD.CDX_CD = 'SC_ONLINE_MODE' ");
	strSql += CRLF + _T("                         AND CCD_ONLINE_MODE_RD.CCD_CD = SD.ONLINE_MODE_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_AUTO_MODE_RD ");
	strSql += CRLF + _T("                          ON CCD_AUTO_MODE_RD.CDX_CD = 'SC_AUTO_MODE' ");
	strSql += CRLF + _T("                         AND CCD_AUTO_MODE_RD.CCD_CD = SD.AUTO_MODE_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ACTIVE_MODE_RD ");
	strSql += CRLF + _T("                          ON CCD_ACTIVE_MODE_RD.CDX_CD = 'SC_ACTIVE_MODE' ");
	strSql += CRLF + _T("                         AND CCD_ACTIVE_MODE_RD.CCD_CD = SD.ACTIVE_MODE_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_UCSTATUS_RD ");
	strSql += CRLF + _T("                          ON CCD_UCSTATUS_RD.CDX_CD = 'SC_UCSTATUS' ");
	strSql += CRLF + _T("                         AND CCD_UCSTATUS_RD.CCD_CD = SD.UCSTATUS_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_FORKPOS_FK1_RD ");
	strSql += CRLF + _T("                          ON CCD_FORKPOS_FK1_RD.CDX_CD = 'SC_FORKPOS' ");
	strSql += CRLF + _T("                         AND CCD_FORKPOS_FK1_RD.CCD_CD = SD.FORKPOS_FK1_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_FORKPOS_FK2_RD ");
	strSql += CRLF + _T("                          ON CCD_FORKPOS_FK2_RD.CDX_CD = 'SC_FORKPOS' ");
	strSql += CRLF + _T("                         AND CCD_FORKPOS_FK2_RD.CCD_CD = SD.FORKPOS_FK2_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ERR_STA_FK1_RD ");
	strSql += CRLF + _T("                          ON CCD_ERR_STA_FK1_RD.CDX_CD = 'SC_ERR_STA_FK' ");
	strSql += CRLF + _T("                         AND CCD_ERR_STA_FK1_RD.CCD_CD = SD.ERR_STA_FK1_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ERR_STA_FK2_RD ");
	strSql += CRLF + _T("                          ON CCD_ERR_STA_FK2_RD.CDX_CD = 'SC_ERR_STA_FK' ");
	strSql += CRLF + _T("                         AND CCD_ERR_STA_FK2_RD.CCD_CD = SD.ERR_STA_FK2_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_COMPLETE_RD ");
	strSql += CRLF + _T("                          ON CCD_COMPLETE_RD.CDX_CD = 'SC_COMPLETE' ");
	strSql += CRLF + _T("                         AND CCD_COMPLETE_RD.CCD_CD = SD.COMPLETE_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_SENSOR_FK_RD ");
	strSql += CRLF + _T("                          ON CCD_SENSOR_FK_RD.CDX_CD = 'SC_SENSOR_FK' ");
	strSql += CRLF + _T("                         AND CCD_SENSOR_FK_RD.CCD_CD = SD.SENSOR_FK_RD ");
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_SUSPEND ");
	strSql += CRLF + _T("                          ON CCD_SUSPEND.CDX_CD = 'SC_SUSPEND' ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.CCD_CD = SD.SUSPEND ");
	strSql += CRLF + _T(" WHERE SD.WH_TYP = ") + CLib::Quot(pSC_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND SD.PLC_NO = ") + CLib::Quot(pSC_DATA->K_PLC_NO);
	strSql += CRLF + _T("   AND SD.MC_NO = ") + CLib::Quot(pSC_DATA->K_SC_NO);
	
	

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}


void CScSkinDlg::OnBnClickedBtnScManualRet()
{
	CString strSC_NO = m_pSC_DATA->K_SC_NO;
	m_pDoc->OnCreateScManualRet(strSC_NO);

	return;

	CString strLuggNo = _T("");
	CString strHsMcNo = _T("");
	CString strSensor0DataRd = _T("");
	CString strItnLuggNo = m_pSC_DATA->V_ITN_LUGG_FK1; //진행중인 작업번호 미리 담기.
	CString strSensorFkRd = m_pSC_DATA->V_SENSOR_FK_RD;
	CString strForkPosRd = m_pSC_DATA->V_FORKPOS_FK1_RD;
	CString strProductSize = _T("");
	CString strLOG_MSG = _T("");

	if (!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}
	
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("강제배출은 비상 시 사용합니다. 하시겠습니까? ")), MB_YESNO) != IDYES)	
		return;	
	
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고 HS에 화물이 있습니까? ")), MB_YESNO) != IDYES)	
		return;	
	
	
	//화물감지유무 확인.
	if (RetHsDataSelect(strHsMcNo, strSensor0DataRd) == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고HS 화물유무감지 조회중 실패")));
		return;
	}
	
	if (strSensor0DataRd == _T("0"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고HS에 화물이 없습니다. 센서를 확인해주세요.")));
		return;
	}
	
	//SC 화물 없어야됨.
	if (strSensorFkRd == _T("1") || strSensorFkRd == _T("3"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC에 화물이 있어서 강제배출을 할 수 없습니다.")));
		return;
	}
	
	//Sc 포크 센터여야 함.
	if (strForkPosRd != _T("0"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 포크가 센터가 아니어서 강제배출을 할 수 없습니다.")));
		return;
	}
	
	m_pDoc->BeginTrans_DLG();
	
	//SC DATA 삭제
	if (ScDataDelete() == FALSE)
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 데이터 삭제중 실패")));
		return;
	}
	
	//해당 작업정보 조회 및 삭제(없으면 안함)
	if (JobMstDelete(strItnLuggNo, strProductSize) == FALSE)
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업정보 삭제중 실패")));
		return;
	}
	
	//반자동작업 생성
	if (JobMstInsert(strHsMcNo, strLuggNo, strProductSize) == FALSE)
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("반자동 작업 생성중 실패")));
		return;
	}
	
	//출고HS DATA 쓰기
	if (CvDataUpdate(strHsMcNo, strLuggNo, strProductSize) == FALSE)
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고 HS DATA 쓰기중 실패")));
		return;
	}
	
	strLOG_MSG.Format(_T("SC 강제 배출 BUTTON -> SC NO : %s , 삭제 작업번호 : %s, 생성 작업번호 : %s"), m_pSC_DATA->K_SC_NO, strItnLuggNo, strLuggNo);
	if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("LOG 입력중 실패")));
		return;
	}
	
	m_pDoc->CommitTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("강제배출 성공")));
	return;
}

BOOL CScSkinDlg::RetHsDataSelect(CString& strHS_MC_NO, CString& strSENSOR0_DATA_RD)
{
	CString strHS_NO = _T("");
	strHS_MC_NO = _T("");
	strSENSOR0_DATA_RD = _T("");

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	//해당 SC의 출고 HS 구하기
	strSql.Format(_T("  SELECT SHD.HS_MC_NO, CD.SENSOR0_DATA_RD	")
				  _T("	  FROM SC_HS_DEF SHD					")
				  _T("   INNER JOIN CV_DATA CD				    ")
				  _T("           ON SHD.HS_MC_NO = CD.MC_NO 	")
				  _T("   WHERE SHD.WH_TYP = '%s' 				")
				  _T("     AND SHD.HS_NO = '02' 				")
				  _T("     AND SHD.SC_NO = '%s'				    "), m_pSC_DATA->K_WH_TYP, m_pSC_DATA->K_SC_NO);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
		return FALSE;

	pRsw->MoveFirst();
	//출고 HS 번호, 화물감지 유무
	strHS_MC_NO = pRsw->GetItem(_T("HS_MC_NO"));
	strSENSOR0_DATA_RD = pRsw->GetItem(_T("SENSOR0_DATA_RD"));

	//pRsw->MoveNext();
	delete pRsw;

	if (nRowCnt <= 0)
		return FALSE;

	return TRUE;

}

BOOL CScSkinDlg::ScDataDelete()
{
	CString strSql = _T("");
	
	//해당 SC DATA 삭제
	strSql.Format(_T("UPDATE SC_DATA 				\n")
                  _T("	 SET CMD_RQ_ID = 'DELFK1'   \n")
                  _T("	   , CMD_RQ_YN = 'Y'		 \n")
				  _T("    WHERE WH_TYP = '%s'		\n")
		          _T("      AND PLC_NO = '%s'		\n")
		          _T("      AND SC_NO = '%s'		  "), m_pSC_DATA->K_WH_TYP, m_pSC_DATA->K_PLC_NO, m_pSC_DATA->K_SC_NO);
	
	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);
	
	if(isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;

	//strSql.Format(_T("UPDATE SC_DATA 					\n")
    //              _T("   SET JOB_TYP_OD = '0'			 \n")
    //              _T("      ,LUGG_NO_FK1_OD = '0000'	 \n")
    //              _T("      ,ITN_LUGG_FK1 = '0'			 \n")
    //              _T("      ,START_BANK_FK1_OD = '00'	 \n")
    //              _T("      ,START_BAY_FK1_OD = '000'	 \n")
    //              _T("      ,START_LEVEL_FK1_OD = '00'	 \n")
    //              _T("      ,START_HSPOS_FK1_OD = '0'	 \n")
	//              _T("      ,DEST_BANK_FK1_OD = '00'	 \n")
    //              _T("      ,DEST_BAY_FK1_OD = '000'	 \n")
    //              _T("      ,DEST_LEVEL_FK1_OD = '00'	 \n")
    //              _T("      ,DEST_HSPOS_FK1_OD = '0'	 \n")
    //              _T("      ,OD_RQ_YN = 'Y'				 \n")
	//			  _T("    WHERE WH_TYP = '%s'			 \n")
	//	          _T("      AND PLC_NO = '%s'			 \n")
	//	          _T("      AND SC_NO = '%s'		       "), m_pSC_DATA->K_WH_TYP, m_pSC_DATA->K_PLC_NO, m_pSC_DATA->K_SC_NO);
	//
	//int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);
	//
	//if(isSuccess == TRUE)
	//{
	//	return TRUE;
	//}
	//return FALSE;
}

BOOL CScSkinDlg::JobMstDelete(CString pLuggNo, CString& strProductSize)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strProductSize = _T("");

	//SC의 작업 작업번호가 작업정보에 있는지 확인
	strSql.Format(_T("  SELECT *						")
				  _T("	  FROM JOB_MST					")
				  _T("   WHERE LUGG_NO = '%s'		    "), pLuggNo);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt == 1)
	{
		pRsw->MoveFirst(); 
		strProductSize =  pRsw->GetItem(_T("PRODUCT_SIZE"));

		delete pRsw;

		//해당 작업정보 삭제
		strSql.Format(_T("DELETE FROM JOB_MST 	 \n")
					  _T(" WHERE LUGG_NO = '%s'	 \n"), pLuggNo);

		int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if(isSuccess == TRUE)
		{
			//자동 작업인 경우 메시지 보여주기
			if (pLuggNo.Left(1) != _T("9"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업을 WMS에서도 삭제해주세요.")));
			}
			return TRUE;
		}
		return FALSE;
	}
	strProductSize = _T("0");
	return TRUE;
}

BOOL CScSkinDlg::JobMstInsert(CString pHsMcNo, CString& strNextVal, CString& strProductSize)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");
	strNextVal = _T("");

	//UpdateData(TRUE);
	//CString strDestPos = _T("");
	//m_cbxDestPos.GetWindowText(strDestPos);

	//해당 SC의 출고 HS 구하기
	strSql.Format(_T("  SELECT NEXTVAL('MANUAL_SEQ') "));

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
		return FALSE;

	pRsw->MoveFirst();
	//seq에서 작업에 사용할 번호 가져오기
	strNextVal = pRsw->GetItem(_T("nextval"));

	//pRsw->MoveNext();
	delete pRsw;

	//작업대이동 반자동작업 생성
	strSql.Format(_T("INSERT INTO JOB_MST (WH_TYP					\n")
				  _T("					  ,LUGG_NO					\n")
       			  _T("					  ,START_POS 				\n")
       			  _T("					  ,START_LOCATION 			\n")
       			  _T("					  ,DEST_POS 				\n")
       			  _T("					  ,DEST_LOCATION 			\n")
       			  _T("					  ,PRODUCT_SIZE 			\n")
       			  _T("					  ,JOB_TYP 					\n")
       			  _T("					  ,JOB_STATUS 				\n")
       			  _T("					  ,JOB_PRIORITY 			\n")
				  _T("					  ,HS_TRACK_NO 				\n")
				  _T("					  ,SC_NO		 			\n")
       			  _T("					  ,INS_DT 					\n")
       			  _T("					  ,INS_USER_ID)				\n")
				  _T("			   VALUES ('%s'						\n")
				  _T("			          ,'%s'						\n") // 작업번호
				  _T("			          ,'%s'						\n") // 출발지 해당 SC
				  _T("			          ,'00-000-00'				\n") // 출발 로케이션 '00-000-00'
				  _T("			          ,'148'					\n") // 도착지 '148'
				  _T("			          ,'00-000-00'				\n") // 도착 로케이션 '00-000-00'
				  _T("			          ,'0'						\n") // 펄프 단수 '0'
				  _T("			          ,'6'						\n") // 작업구분 '6'(작업대이동)
				  _T("			          ,'11'				    	\n") // 작업상태 '11' (CV구동중)
				  _T("			          ,'100'					\n") // 우선순위 '100'
				  _T("			          ,'%s'						\n") // 출고 HS
				  _T("			          ,'%s'						\n") // SC 번호
				  _T("			          ,") + m_pDoc->SYSDATE + _T("\n")
				  _T("			          ,'%s')					 \n"), m_pSC_DATA->K_WH_TYP, strNextVal, m_pSC_DATA->K_SC_NO, pHsMcNo, m_pSC_DATA->K_SC_NO, m_pDoc->m_strId);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CScSkinDlg::CvDataUpdate(CString strMcNo, CString strLuggNo, CString strPulpSensor)
{
	CString strSql = _T("");

	//입고 HS 트랙 데이터 삭제
	strSql.Format(_T("  UPDATE CV_DATA					 \n")
		            _T("   SET LUGG_NO_OD = '%s'		 \n")
		            _T("     , DEST_POS_OD = '148'		 \n")
		            _T("     , JOB_TYP_OD = '6'			 \n")
					_T("     , PULP_SENSOR_OD = '%s'	 \n")
					_T("     , WAIT_SC_RET_JOB_OD = '0'	 \n")
					_T("     , OD_RQ_YN = 'Y'			 \n")
		            _T(" WHERE WH_TYP = '%s'			 \n")
		            _T("   AND MC_NO = '%s'		       "), strLuggNo, strPulpSensor, m_pSC_DATA->K_WH_TYP, strMcNo);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

void CScSkinDlg::SetScStatus(CString pCMD, CString pSC_SUSPEND)
{
	CString strCcdNmKor = _T("");
	CString strGetErrorCode = _T("");

	if (pCMD == _T("ACTIVE"))
	{
		SelCommonCode(_T("1"), _T("SC_ACTIVE_MODE"), strCcdNmKor);
		m_edtScRcStatus.SetWindowText(strCcdNmKor);
	}
	else if (pCMD == _T("PAUSE"))
	{
		SelCommonCode(_T("0"), _T("SC_ACTIVE_MODE"), strCcdNmKor);
		m_edtScRcStatus.SetWindowText(strCcdNmKor);
	}
	else if (pCMD == _T("EMERGENCY"))
	{
		SelCommonCode(_T("4"), _T("SC_UCSTATUS"), strCcdNmKor);
		m_edtScScStatus.SetWindowText(strCcdNmKor);
		GetErrorCode(_T("SC"), _T("0003"), 0, strGetErrorCode); 
		m_edtScErrCode.SetWindowText(strGetErrorCode);
	}
	else if (pCMD == _T("RESET"))
	{
		SelCommonCode(_T("0"), _T("SC_UCSTATUS"), strCcdNmKor);
		m_edtScScStatus.SetWindowText(strCcdNmKor);
		GetErrorCode(_T("SC"), _T("0000"), 0, strGetErrorCode); 
		m_edtScErrCode.SetWindowText(strGetErrorCode);
	}
	else if (pCMD == _T("DELFK1"))
	{
		SelCommonCode(_T("0"), _T("SC_ACTIVE_MODE"), strCcdNmKor);
		m_edtScRcStatus.SetWindowText(strCcdNmKor);
		SelCommonCode(_T("1"), _T("SC_UCSTATUS"), strCcdNmKor);
		m_edtScScStatus.SetWindowText(strCcdNmKor);
	}
	else if (pCMD == _T("SUSPEND"))
	{
		SelCommonCode(pSC_SUSPEND, _T("SC_SUSPEND"), strCcdNmKor);
		m_edtScSuspend.SetWindowText(strCcdNmKor);
	}
}

void CScSkinDlg::SelCommonCode(CString pCCD_CD, CString pCMD, CString& pCCD_NM_KOR)
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

HBRUSH CScSkinDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID() == IDC_EDT_SC_JOB_SC_SUSPEND)
	{
		if(m_pSC_DATA->V_SUSPEND == _T("0"))
		{
			 hbr = (HBRUSH)::GetSysColorBrush(COLOR_WINDOW);
			 pDC->SetTextColor(BLACK);  // 텍스트 색
			 
			pDC->AssertValid();

		}
		else
		{
			hbr = (HBRUSH)::GetSysColorBrush(COLOR_WINDOW);
			pDC->SetTextColor(RED);  // 텍스트 색
			pDC->AssertValid();
		}
    }

	return hbr;
}
