// RollSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "RollSkinDlg.h"
#include "afxdialogex.h"


// CRollSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRollSkinDlg, CSkinDialog)

CRollSkinDlg::CRollSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CRollSkinDlg::IDD, pParent)
{	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_blAutoSel = false;
}

CRollSkinDlg::CRollSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CRollSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_blAutoSel = false;
}

CRollSkinDlg::~CRollSkinDlg()
{
	m_pDoc->m_pRollSkinDlg = NULL;
	CSkinDialog::OnClose();
	this->DestroyWindow();
}

void CRollSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CLAMP_BACKWARD_ERR, m_btnClampBackwardErr);
	DDX_Control(pDX, IDC_BTN_CLAMP_FORWARD_ERR, m_btnClampforwardErr);
	DDX_Control(pDX, IDC_BTN_CRUSH_PAPER_SENSOR, m_btnCrushPaperSensor);
	DDX_Control(pDX, IDC_BTN_DRIV_BACKWARD_ERR, m_btnDrivBackwardErr);
	DDX_Control(pDX, IDC_BTN_DRIV_BACKWARD_POS, m_btnDrivBackwardPos);
	DDX_Control(pDX, IDC_BTN_DRIV_FORWARD_ERR, m_btnDrivForwardErr);
	DDX_Control(pDX, IDC_BTN_DRIV_FORWARD_POS, m_btnDrivForwardPos);
	DDX_Control(pDX, IDC_BTN_ELEV_ASC_ERR, m_btnElevAscErr);
	DDX_Control(pDX, IDC_BTN_ELEV_DESC_ERR, m_btnElevDescErr);
	DDX_Control(pDX, IDC_BTN_PAPER_BLOCK_SENSOR1, m_btnPaperBlockSensor1);
	DDX_Control(pDX, IDC_BTN_PAPER_BLOCK_SENSOR2, m_btnPaperBlockSensor2);
	DDX_Control(pDX, IDC_BTN_PAPER_BLOCK_SENSOR3, m_btnPaperBlockSensor3);
	DDX_Control(pDX, IDC_BTN_PAPER_BLOCK_SENSOR4, m_btnPaperBlockSensor4);
	DDX_Control(pDX, IDC_BTN_PAPER_FULL_SENSOR, m_btnPaperFullSensor);
	DDX_Control(pDX, IDC_CHK_AUTO_SEL, m_chkAutoSel);
	DDX_Control(pDX, IDC_EDT_ROLL_CLAMP_POS_DATA_BACKWARD, m_edtRollClampPosDataBackward);
	DDX_Control(pDX, IDC_EDT_ROLL_CLAMP_POS_DATA_FORWARD, m_edtRollClampPosDataForward);
	DDX_Control(pDX, IDC_EDT_ROLL_ELEV_POS_DATA_ASC, m_edtRollElevPosDataAsc);
	DDX_Control(pDX, IDC_EDT_ROLL_ELEV_POS_DATA_PICKUP, m_edtRollElevPosDataPickup);
	DDX_Control(pDX, IDC_EDT_ROLL_ELEV_POS_DATA_ROLLING, m_edtRollElevPosDataAscRolling);
	DDX_Control(pDX, IDC_EDT_ROLL_NO, m_edtRollNo);
	DDX_Control(pDX, IDC_EDT_ROLL_SENSOR_DATA_HEIGHT, m_edtRollSensorDataHeight);
	DDX_Control(pDX, IDC_LBL_ROLL_CLAMP_POS_DATA_BACKWARD, m_lblRollClampPosDataBackward);
	DDX_Control(pDX, IDC_LBL_ROLL_CLAMP_POS_DATA_FORWARD1, m_lblRollClampPosDataForward);
	DDX_Control(pDX, IDC_LBL_ROLL_ELEV_POS_DATA_ASC, m_lblRollElevPosDataAsc);
	DDX_Control(pDX, IDC_LBL_ROLL_ELEV_POS_DATA_PICKUP, m_lblRollElevPosDataPickup);
	DDX_Control(pDX, IDC_LBL_ROLL_ELEV_POS_DATA_ROLLING, m_lblRollElevPosDataRolling);
	DDX_Control(pDX, IDC_LBL_ROLL_SENSOR_DATA_HEIGHT, m_lblRollSensorDataHeight);
	DDX_Control(pDX, IDC_GRP_ROLL_POS_DATA, m_grpRollPosData);
	DDX_Control(pDX, IDC_GRP_ROLL_POS_ITEM, m_grpRollPosItem);
	DDX_Control(pDX, IDC_GRP_ROLL_POS_VALUE, m_grpRollPosValue);
	DDX_Control(pDX, IDC_GRP_ROLL_STATUS, m_grpRollStatus);
	DDX_Control(pDX, IDC_GRP_TRANS_STATUS, m_grpTransStatus);
}


BEGIN_MESSAGE_MAP(CRollSkinDlg, CSkinDialog)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CRollSkinDlg::OnMessageSwitch)
	ON_WM_ENTERSIZEMOVE()
	ON_WM_EXITSIZEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHK_AUTO_SEL, &CRollSkinDlg::OnBnClickedChkAutoSel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRollSkinDlg 메시지 처리기입니다.


void CRollSkinDlg::OnClose()
{
	m_pDoc->m_pRollSkinDlg = NULL;
	delete this;
}

LRESULT CRollSkinDlg::OnMessageSwitch(WPARAM wParam, LPARAM lParam)
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

void CRollSkinDlg::OnEnterSizeMove()
{
	CSkinDialog::OnEnterSizeMove(0,0);
}


void CRollSkinDlg::OnExitSizeMove()
{
	CSkinDialog::OnExitSizeMove(0,0);
	Invalidate(TRUE);
}


void CRollSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}


void CRollSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}


HCURSOR CRollSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CRollSkinDlg::OnInitDialog()
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

void CRollSkinDlg::InvalidateTrackData(EN_LANG pLang)
{	
	if(m_pDoc == NULL) return;
	if(m_pTrackInfo == NULL) return;
	if(m_pTrackInfo->m_pCV_DATA == NULL) return;

	if (!UpdateData())
		return;

	CString strTemp;
	
	m_edtRollNo.SetWindowText(m_pTrackInfo->m_pCV_DATA->V_MC_NO + _T(" (") + m_pTrackInfo->m_pCV_DATA->V_MC_NO_NM + _T(")"));

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_ROLL_ELEV_POS_DATA_ASC));
	m_edtRollElevPosDataAsc.SetWindowText(strTemp);

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_ROLL_ELEV_POS_DATA_PICKUP));
	m_edtRollElevPosDataPickup.SetWindowText(strTemp);

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_ROLL_ELEV_POS_DATA_ROLLING));
	m_edtRollElevPosDataAscRolling.SetWindowText(strTemp);

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_ROLL_CLAMP_POS_DATA_FORWARD));
	m_edtRollClampPosDataForward.SetWindowText(strTemp);

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_ROLL_CLAMP_POS_DATA_BACKWARD));
	m_edtRollClampPosDataBackward.SetWindowText(strTemp);

	strTemp = CConvert::ToString(CConvert::ToInt(m_pTrackInfo->m_pCV_DATA->V_ROLL_SENSOR_DATA_HEIGHT));
	m_edtRollSensorDataHeight.SetWindowText(strTemp);



	//버튼(아이콘) 바꾸기
	m_btnClampBackwardErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_CLAMP_BACKWARD_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnClampforwardErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_CLAMP_FORWARD_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnCrushPaperSensor.SetIcon((m_pTrackInfo->m_pCV_DATA->V_CRUSH_PAPER_SENSOR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivBackwardErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DRIV_BACKWARD_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivBackwardPos.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DRIV_BACKWARD_POS == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivForwardErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DRIV_FORWARD_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivForwardPos.SetIcon((m_pTrackInfo->m_pCV_DATA->V_DRIV_FORWARD_POS == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnElevAscErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_ELEV_ASC_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnElevDescErr.SetIcon((m_pTrackInfo->m_pCV_DATA->V_ELEV_DESC_ERR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnPaperBlockSensor1.SetIcon((m_pTrackInfo->m_pCV_DATA->V_PAPER_BLOCK_SENSOR1 == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnPaperBlockSensor2.SetIcon((m_pTrackInfo->m_pCV_DATA->V_PAPER_BLOCK_SENSOR2 == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnPaperBlockSensor3.SetIcon((m_pTrackInfo->m_pCV_DATA->V_PAPER_BLOCK_SENSOR3 == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnPaperBlockSensor4.SetIcon((m_pTrackInfo->m_pCV_DATA->V_PAPER_BLOCK_SENSOR4 == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
	m_btnPaperFullSensor.SetIcon((m_pTrackInfo->m_pCV_DATA->V_PAPER_FULL_SENSOR == _T("1")) ? Global.GetIcon(Global.ICO_LED_ON) : Global.GetIcon(Global.ICO_LED_OFF));
}

void CRollSkinDlg::InvalidateLangControl( EN_LANG enLangTemp )
{ // 데이터가 아닌 레이블 / 버튼 등의 값만 수정시킬것
	if(m_pDoc == NULL)
		return;
}

void CRollSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{

	RenameResource(nEN_LANG);
	RedrawImage();
}

void CRollSkinDlg::RenameResource( EN_LANG m_enLang)
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

void CRollSkinDlg::RedrawImage()
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
	m_btnClampforwardErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnCrushPaperSensor.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivBackwardErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivBackwardPos.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivForwardErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnDrivForwardPos.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnElevAscErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnElevDescErr.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnPaperBlockSensor1.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnPaperBlockSensor2.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnPaperBlockSensor3.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnPaperBlockSensor4.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
	m_btnPaperFullSensor.SetIcon(Global.GetIcon(Global.ICO_LED_OFF));
}

void CRollSkinDlg::RelocationControls()
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

void CRollSkinDlg::OnBnClickedChkAutoSel()
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

void CRollSkinDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_blAutoSel == true)
	{
		//조회
		InvalidateTrackData(EN_KOR);
	}
}
