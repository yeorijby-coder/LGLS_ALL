// RevSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "RevSkinDlg.h"
#include "afxdialogex.h"


// CRevSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRevSkinDlg, CSkinDialog)

CRevSkinDlg::CRevSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CRevSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_blAutoSel = false;
}

CRevSkinDlg::CRevSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CRevSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_blAutoSel = false;
}

CRevSkinDlg::~CRevSkinDlg()
{
	m_pDoc->m_pRevSkinDlg = NULL;
	CSkinDialog::OnClose();
	this->DestroyWindow();
}

void CRevSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CLAMP_BACKWARD_ERR, m_btnClampBackwardErr);
	DDX_Control(pDX, IDC_BTN_CLAMP_FORWARD_ERR, m_btnClampForwardErr);
	DDX_Control(pDX, IDC_BTN_CLAMP_FORWARD_SENSOR, m_btnClampForwardSensor);
	DDX_Control(pDX, IDC_BTN_CLAMP_FORWARD_ERR, m_btnClampForwardErr);
	DDX_Control(pDX, IDC_BTN_CLAMP_FORWARD_SENSOR, m_btnClampForwardSensor);
	DDX_Control(pDX, IDC_BTN_DRIV_BACKWARD_ERR, m_btnDrivBackwardErr);
	DDX_Control(pDX, IDC_BTN_DRIV_FORWARD_ERR, m_btnDrivForwardErr);
	DDX_Control(pDX, IDC_BTN_ELEV_ASC_ERR, m_btnElevAscErr);
	DDX_Control(pDX, IDC_BTN_ELEV_DESC_ERR, m_btnElevDescErr);
	DDX_Control(pDX, IDC_EDT_REV_DRIV_POS_DATA_PICKUP, m_edtRevDrivPosDataPickup);
	DDX_Control(pDX, IDC_EDT_REV_DRIV_POS_DATA_REVERSE, m_edtRevDrivPosDataReverse);
	DDX_Control(pDX, IDC_EDT_REV_ELEV_POS_DATA_1, m_EdtRevElevPosData1);
	DDX_Control(pDX, IDC_EDT_REV_ELEV_POS_DATA_2, m_edtElevPosData2);
	DDX_Control(pDX, IDC_EDT_REV_ELEV_POS_DATA_ASC, m_edtRevElevPosDataAsc);
	DDX_Control(pDX, IDC_EDT_REV_NO, m_edtRevNo);
	DDX_Control(pDX, IDC_EDT_REV_SENSOR_DATA_HEIGHT, m_edtRevSensorDataHeight);
	DDX_Control(pDX, IDC_GRP_REV_POS_DATA, m_grpRevPosData);
	DDX_Control(pDX, IDC_GRP_REV_POS_VALUE, m_grpRevPosValue);
	DDX_Control(pDX, IDC_GRP_REV_POS_ITEM, m_grpRevPosItem);
	DDX_Control(pDX, IDC_GRP_REV_STATUS, m_grpRevStatus);
	DDX_Control(pDX, IDC_LBL_REV_DRIV_POS_DATA_PICKUP, m_lblRevDrivPosDataPickup);
	DDX_Control(pDX, IDC_LBL_REV_DRIV_POS_DATA_REVERSE, m_lblRevDrivPosDatareverse);
	DDX_Control(pDX, IDC_LBL_REV_ELEV_POS_DATA_1, m_lblRevElevPosData1);
	DDX_Control(pDX, IDC_LBL_REV_ELEV_POS_DATA_2, m_lblRevElevPosData2);
	DDX_Control(pDX, IDC_LBL_REV_ELEV_POS_DATA_ASC, m_lblRevElevPosDataAsc);
	DDX_Control(pDX, IDC_LBL_REV_SENSOR_DATA_HEIGHT, m_lblRevSensorDataHeight);
	DDX_Control(pDX, IDC_CHK_AUTO_SEL, m_chkAutoSel);
	DDX_Control(pDX, IDC_BTN_CLAMP_BACKWARD_SENSOR, m_btnClampBackwardSensor);
	DDX_Control(pDX, IDC_BTN_DRIV_BACKWARD_POS, m_btnDrivBackwardPos);
	DDX_Control(pDX, IDC_BTN_DRIV_FORWARD_POS, m_btnDrivForwardPos);
}


BEGIN_MESSAGE_MAP(CRevSkinDlg, CSkinDialog)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CRevSkinDlg::OnMessageSwitch)
	ON_WM_ENTERSIZEMOVE()
	ON_WM_EXITSIZEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHK_AUTO_SEL, &CRevSkinDlg::OnBnClickedChkAutoSel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRevSkinDlg 메시지 처리기입니다.


void CRevSkinDlg::OnClose()
{
	m_pDoc->m_pRevSkinDlg = NULL;
	delete this;
}

LRESULT CRevSkinDlg::OnMessageSwitch(WPARAM wParam, LPARAM lParam)
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

void CRevSkinDlg::InvalidateTrackData(EN_LANG pLang)
{	
	if(m_pDoc == NULL) return;
	if(m_pTrackInfo == NULL) return;
	if(m_pTrackInfo->m_pCV_DATA == NULL) return;

	if (!UpdateData())
		return;

	CString strTemp;
	
	m_edtRevNo.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_MC_NO + _T(" (") + m_pTrackInfo->m_pCV_DATA->V_MC_NO_NM + _T(")"));

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_REV_DRIV_POS_DATA_PICKUP));
	m_edtRevDrivPosDataPickup.SetWindowText(strTemp);

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_REV_DRIV_POS_DATA_REVERSE));
	m_edtRevDrivPosDataReverse.SetWindowText(strTemp);

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_REV_ELEV_POS_DATA_1));
	m_EdtRevElevPosData1.SetWindowText(strTemp);

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_REV_ELEV_POS_DATA_2));
	m_edtElevPosData2.SetWindowText(strTemp);

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_REV_ELEV_POS_DATA_ASC));
	m_edtRevElevPosDataAsc.SetWindowText(strTemp);

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_REV_SENSOR_DATA_HEIGHT));
	m_edtRevSensorDataHeight.SetWindowText(strTemp);

	//버튼(아이콘) 바꾸기
	m_btnClampBackwardErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_CLAMP_BACKWARD_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnClampForwardErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_CLAMP_FORWARD_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnClampForwardSensor.SetIcon((m_pTrackInfo->m_pCV_DATA->V_CLAMP_FORWARD_SENSOR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivBackwardErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DRIV_BACKWARD_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivForwardErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DRIV_FORWARD_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnElevAscErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_ELEV_ASC_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnElevDescErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_ELEV_DESC_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));

	m_btnClampBackwardSensor.SetIcon((m_pTrackInfo->m_pCV_DATA->V_CLAMP_BACKWARD_SENSOR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivForwardPos.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DRIV_FORWARD_POS == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivBackwardPos.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DRIV_BACKWARD_POS == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
}

void CRevSkinDlg::InvalidateLangControl( EN_LANG enLangTemp )
{ // 데이터가 아닌 레이블 / 버튼 등의 값만 수정시킬것
	if(m_pDoc == NULL)
		return;
}

void CRevSkinDlg::OnEnterSizeMove()
{
	CSkinDialog::OnEnterSizeMove(0,0);
}


void CRevSkinDlg::OnExitSizeMove()
{
	CSkinDialog::OnExitSizeMove(0,0);
	Invalidate(TRUE);
}


void CRevSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}


void CRevSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}


HCURSOR CRevSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CRevSkinDlg::OnInitDialog()
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
	return TRUE;
}

void CRevSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{

	RenameResource(nEN_LANG);
	RedrawImage();
}

void CRevSkinDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	/*CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("status"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_WC_WC_STATUS, strValue);*/


}

void CRevSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\20x20\\");

	CString strExtension = _T(".png");

	m_btnClampBackwardErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnClampForwardErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnClampForwardSensor.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivBackwardErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivForwardErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnElevAscErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnElevDescErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));

	/*m_btnWcWrite.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnWcWrite.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);*/
}

void CRevSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	/*m_btnCvWrite.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCvWrite.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);*/
}

void CRevSkinDlg::OnBnClickedChkAutoSel()
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

void CRevSkinDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_blAutoSel == true)
	{
		//조회
		InvalidateTrackData(EN_KOR);
	}
}
