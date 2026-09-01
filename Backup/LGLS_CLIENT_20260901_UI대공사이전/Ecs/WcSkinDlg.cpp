// WcSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "WcSkinDlg.h"
#include "afxdialogex.h"

#include "RecordSetWrap.h"


// CWcSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWcSkinDlg, CSkinDialog)

CWcSkinDlg::CWcSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CWcSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CWcSkinDlg::CWcSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CWcSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CWcSkinDlg::~CWcSkinDlg()
{
	m_pDoc->m_pWcSkinDlg = NULL;
	//this->DestroyWindow();
}

void CWcSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_WC_ACTIVE, m_btnWcActive);
	DDX_Control(pDX, IDC_BTN_WC_PASS, m_btnWcPass);
	DDX_Control(pDX, IDC_BTN_WC_READ, m_btnWcRead);
	DDX_Control(pDX, IDC_BTN_WC_SUSPEND, m_btnWcSuspend);
	DDX_Control(pDX, IDC_BTN_WC_WRITE, m_btnWcWrite);
	DDX_Control(pDX, IDC_EDT_WC_ERR_CODE, m_edtWcErrCode);
	DDX_Control(pDX, IDC_EDT_WC_NO, m_edtWcNo);
	DDX_Control(pDX, IDC_EDT_WC_RCV_VAL, m_edtWcRcvVal);
	DDX_Control(pDX, IDC_EDT_WC_READ_STA, m_edtWcReadSta);
	DDX_Control(pDX, IDC_EDT_WC_SUSPEND, m_edtWcSuspend);
	DDX_Control(pDX, IDC_EDT_WC_WRITE_VAL, m_edtWcWriteVal);
	DDX_Control(pDX, IDC_GRP_WC_ERROR_INFORMATION, m_grpWcErrorInformation);
	DDX_Control(pDX, IDC_GRP_WC_SUSPEND, m_grpWcSuspend);
	DDX_Control(pDX, IDC_GRP_WC_WC_STATUS, m_grpWcWcStatus);
	DDX_Control(pDX, IDC_GRP_WC_WC_STATUS_COMMAND, m_grpWcStatusCommand);
	DDX_Control(pDX, IDC_GRP_WC_WC_STATUS_ITEM, m_grpWcStatusItem);
	DDX_Control(pDX, IDC_GRP_WC_WC_STATUS_VALUE, m_grpWcStatusValue);
	DDX_Control(pDX, IDC_LBL_WC_RCV_VAL, m_lblWcRcvVal);
	DDX_Control(pDX, IDC_LBL_WC_READ_STA, m_lblWcReadSta);
	DDX_Control(pDX, IDC_LBL_WC_WRITE_VAL, m_lblWcWriteVal);
	DDX_Control(pDX, IDC_LBL_WC_BYPASS, m_lblWcByPass);
	DDX_Control(pDX, IDC_EDT_WC_BYPASS, m_edtWcByPass);
	DDX_Control(pDX, IDC_CHK_WC_BYPASS, m_chkWcByPass);
	DDX_Control(pDX, IDC_BTN_WC_WRITE_MANUAL, m_btnWcWriteManual);
}


BEGIN_MESSAGE_MAP(CWcSkinDlg, CSkinDialog)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CWcSkinDlg::OnMessageSwitch)
	ON_WM_ENTERSIZEMOVE()
	ON_WM_EXITSIZEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_WC_PASS, &CWcSkinDlg::OnBnClickedBtnWcPass)
	ON_BN_CLICKED(IDC_BTN_WC_READ, &CWcSkinDlg::OnBnClickedBtnWcRead)
	ON_BN_CLICKED(IDC_BTN_WC_WRITE, &CWcSkinDlg::OnBnClickedBtnWcWrite)
	ON_BN_CLICKED(IDC_BTN_WC_ACTIVE, &CWcSkinDlg::OnBnClickedBtnWcActive)
	ON_BN_CLICKED(IDC_BTN_WC_SUSPEND, &CWcSkinDlg::OnBnClickedBtnWcSuspend)
	ON_BN_CLICKED(IDC_CHK_WC_BYPASS, &CWcSkinDlg::OnClickedChkWcBypass)
	ON_BN_CLICKED(IDC_BTN_WC_WRITE_MANUAL, &CWcSkinDlg::OnBnClickedBtnWcWriteManual)
END_MESSAGE_MAP()


BOOL CWcSkinDlg::OnInitDialog()
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

	m_edtWcWriteVal.EnableMask(_T("dddd ddd"),_T("____.___"), _T('0'));
	m_edtWcWriteVal.EnableWindow(TRUE);

	InitializeResource(pEn);
	return TRUE;
}

LRESULT CWcSkinDlg::OnMessageSwitch(WPARAM wParam, LPARAM lParam)
{
	CWC_DATA* pWC_DATA = (CWC_DATA*)wParam;
	if(pWC_DATA == NULL)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("WC를 찾을수 없습니다")));
		return 0;
	}
	
	m_pWC_DATA = pWC_DATA;
	
	EN_LANG enLangTemp = (EN_LANG)lParam;
	if(m_nLang != enLangTemp)
	{
		m_nLang = enLangTemp;
	}
	
	InvalidateWcData(enLangTemp);
	
	return 0;
}


void CWcSkinDlg::OnClose()
{
	m_pDoc->m_pWcSkinDlg = NULL;
	delete this;
}


void CWcSkinDlg::OnEnterSizeMove()
{
	CSkinDialog::OnEnterSizeMove(0,0);
}


void CWcSkinDlg::OnExitSizeMove()
{
	CSkinDialog::OnExitSizeMove(0,0);
	Invalidate(TRUE);
}


void CWcSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}


void CWcSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}


HCURSOR CWcSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CWcSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CWcSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;

	m_btnWcWrite.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnWcWrite.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnWcPass.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnWcPass.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnWcRead.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnWcRead.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnWcActive.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnWcActive.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnWcSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnWcSuspend.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}

void CWcSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{

	RenameResource(nEN_LANG);
	RedrawImage();
}

void CWcSkinDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("status"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_WC_WC_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_WC_WC_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_WC_WC_STATUS_VALUE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_WC_WC_STATUS_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("grperror"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_WC_ERROR_INFORMATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("grpsuspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_WC_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("readsta"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_WC_READ_STA, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rcvval"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_WC_RCV_VAL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("writeval"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_WC_WRITE_VAL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("write"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_WC_WRITE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("pass"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_WC_PASS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("read"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_WC_READ, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("active"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_WC_ACTIVE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("suspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_WC_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_wc\\"), _T("dlg_wc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bypass"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_WC_BYPASS, strValue);


}

void CWcSkinDlg::RedrawImage()
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

	m_btnWcWrite.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnWcWrite.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnWcPass.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnWcPass.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("pass"), strExtension)), NULL, 5, 5);

	m_btnWcRead.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnWcRead.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);

	m_btnWcActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnWcActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnWcSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnWcSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("pass"), strExtension)), NULL, 5, 5);

	m_btnWcWriteManual.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnWcWriteManual.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);
}

void CWcSkinDlg::InvalidateWcData(EN_LANG pLang)
{	

	if(m_pDoc == NULL) return;
	if(m_pWC_DATA == NULL) return;
	
	if (!UpdateData())
		return;
	
	//상태값
	m_edtWcNo.SetWindowText(m_pWC_DATA->K_WC_MC_NO);

	m_edtWcWriteVal.SetWindowText(_T("0000.000"));

	if (m_pWC_DATA->V_CHK_BYPASS_YN == _T("N"))
	{
		m_chkWcByPass.SetCheck(FALSE);
	}
	else
	{
		m_chkWcByPass.SetCheck(TRUE);
	}
	
	CString strRcvVal = m_pWC_DATA->V_WEIGHT_RCV_VAL;
	strRcvVal.Insert(4,_T("."));
	
	m_edtWcRcvVal.SetWindowText(strRcvVal);

	CString strCCD_NM_KOR;
	SelCommonCode(m_pWC_DATA->V_WEIGHT_READ_STA, _T("WEIGHT_READ_STA"), strCCD_NM_KOR);
	m_edtWcReadSta.SetWindowText(strCCD_NM_KOR);

	SelCommonCode(m_pWC_DATA->V_CHK_BYPASS_YN, _T("CHK_BYPASS_YN"), strCCD_NM_KOR);
	m_edtWcByPass.SetWindowText(strCCD_NM_KOR);

	SelCommonCode(m_pWC_DATA->V_SUSPEND, _T("WC_SUSPEND"), strCCD_NM_KOR);
	m_edtWcSuspend.SetWindowText(strCCD_NM_KOR);

	//쓰기값 초기화
	m_edtWcWriteVal.SetWindowText(_T(""));
}	

void CWcSkinDlg::OnBnClickedBtnWcPass()
{
	UpdateWeightRcvVal(_T("000000"), _T("PASS"));
}


void CWcSkinDlg::OnBnClickedBtnWcRead()
{
	CString strLOG_MSG = _T("");
	CString strWhTyp = m_pWC_DATA->K_WH_TYP;
	CString strPlcNo = m_pWC_DATA->K_PLC_NO;
	CString strWcMcNo = m_pWC_DATA->K_WC_MC_NO;

	UpdateData(TRUE);

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("수동 읽기를 진행하시겠습니까?")), MB_YESNO) != IDYES)
		return;

	if(m_pWC_DATA->V_CMD_RQ_ID == _T("RQ") || m_pWC_DATA->V_CMD_RQ_YN == _T("Y"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("이미 지시중인 상태입니다.")));
			return;
	}

	m_pDoc->BeginTrans_DLG();

	CString strSql = _T("");

	strSql.Format(_T(" UPDATE WC_DATA									\n")
				_T("      SET CMD_RQ_ID = 'RQ' 							\n")
				_T("         ,CMD_RQ_YN = 'Y' 							\n")
				_T("         ,UPD_DT = ") + m_pDoc->SYSDATE + _T("  \n")
				_T("    WHERE WH_TYP = '%s'								\n")
				_T("	  AND PLC_NO = '%02s'							\n")
				_T("      AND WC_MC_NO = '%s'							\n"), strWhTyp, strPlcNo, strWcMcNo);

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		strLOG_MSG.Format(_T("WC 수동 읽기 BUTTON -> WC NO : %s"), strWcMcNo);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CWcSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
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


void CWcSkinDlg::OnBnClickedBtnWcWrite()
{
	CString strWriteVal = _T("");
	m_edtWcRcvVal.GetWindowText(strWriteVal);

	UpdateWeightRcvVal(strWriteVal, _T("무게보고 재지시"));
}


void CWcSkinDlg::OnBnClickedBtnWcActive()
{
	UpdateWcSuspend(_T("0")); //업데이트 하려는 값
}


void CWcSkinDlg::OnBnClickedBtnWcSuspend()
{
	UpdateWcSuspend(_T("1")); //업데이트 하려는 값
}

void CWcSkinDlg::UpdateWcSuspend(CString strSusTyp)
{
	CString strWhTyp = m_pWC_DATA->K_WH_TYP;
	CString strPlcNo = m_pWC_DATA->K_PLC_NO;
	CString strWcMcNo = m_pWC_DATA->K_WC_MC_NO;
	CString strLOG_MSG = _T("");

	UpdateData(TRUE);

	if(strSusTyp == _T("0"))
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("WC 상태를 정상처리 하시겠습니까?")), MB_YESNO) != IDYES)
			return;

		if(m_pWC_DATA->V_SUSPEND == strSusTyp)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("이미 정상 상태입니다.")));
			return;
		}
	}
	else
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("WC 상태를 정지처리 하시겠습니까?")), MB_YESNO) != IDYES)
			return;

		if(m_pWC_DATA->V_SUSPEND == strSusTyp)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("이미 정지 상태입니다.")));
			return;
		}
	}

	m_pDoc->BeginTrans_DLG();

	CString strSql = _T("");

	strSql.Format(_T(" UPDATE WC_DATA									\n")
				_T("    SET SUSPEND = '%s'   							\n")
				_T("  WHERE WH_TYP = '%s'								\n")
				_T("	AND PLC_NO = '%02s'								\n")
				_T("    AND WC_MC_NO = '%s'								\n"), strSusTyp, strWhTyp, strPlcNo, strWcMcNo);

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		strLOG_MSG.Format(_T("WC SUSPEND BUTTON -> WC NO : %s, 상태 : %s" ), strWcMcNo, strSusTyp);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CWcSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
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

void CWcSkinDlg::UpdateWeightRcvVal(CString pWriteVal, CString EventTyp)
{
	CString strWhTyp = m_pWC_DATA->K_WH_TYP;
	CString strPlcNo = m_pWC_DATA->K_PLC_NO;
	CString strWcMcNo = m_pWC_DATA->K_WC_MC_NO;
	CString strLOG_MSG = _T("");
	
	CString strWriteVal = _T("");

	UpdateData(TRUE);

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(EventTyp + _T("를 진행하시겠습니까?")), MB_YESNO) != IDYES)
			return;

	//if(pWriteVal.GetLength() == 0)
	//{
	//	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("쓰기값 항목에 무게를 적어주세요.")));
	//		return;
	//}

	//if(pWriteVal.GetLength() > 6)
	//{
	//	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("최대 6자리의 무게를 적어주세요.")));
	//		return;
	//}

	CString strLUGG_NO_RD = _T("");
	CString strDEST_POS_RD = _T("");
	CString strRET_READY_RD = _T("");

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	//WC CV의 상태 보기
	strSql.Format(_T("  SELECT LUGG_NO_RD, DEST_POS_RD, RET_READY_RD	")
				  _T("	  FROM CV_DATA					")
				  _T("   WHERE WH_TYP = '%s' 				")
				  _T("     AND MC_NO = '%s' 				"), strWhTyp, strWcMcNo);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("104번 트랙정보를 읽는중 에러.")));
		return;
	}

	pRsw->MoveFirst();
	//출고 HS 번호, 화물감지 유무
	strLUGG_NO_RD = pRsw->GetItem(_T("LUGG_NO_RD"));
	strDEST_POS_RD = pRsw->GetItem(_T("DEST_POS_RD"));
	strRET_READY_RD = pRsw->GetItem(_T("RET_READY_RD"));

	//pRsw->MoveNext();
	delete pRsw;

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

	strWriteVal.Format(_T("%07s"), pWriteVal);

	m_pDoc->BeginTrans_DLG();

	strSql = _T("");

	strSql.Format(_T(" UPDATE WC_DATA									\n")
				_T("    SET OD_RQ_ID = 'RQ' 							\n")
				_T("  WHERE WH_TYP = '%s'								\n")
				_T("	AND PLC_NO = '%02s'								\n")
				_T("    AND WC_MC_NO = '%s'								\n"), strWhTyp, strPlcNo, strWcMcNo);

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		strLOG_MSG.Format(_T("WC ") + EventTyp + _T(" BUTTON -> WC NO : %s, 무게값 : %s" ), strWcMcNo, strWriteVal);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CWcSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
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

void CWcSkinDlg::OnClickedChkWcBypass()
{
	CString strWhTyp = m_pWC_DATA->K_WH_TYP;
	CString strPlcNo = m_pWC_DATA->K_PLC_NO;
	CString strWcMcNo = m_pWC_DATA->K_WC_MC_NO;
	CString strByPass = _T("");
	CString strLOG_MSG = _T("");

	UpdateData(TRUE);

	if (m_chkWcByPass.GetCheck() == 0)
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("PASS 모드를 해제 하시겠습니까?")), MB_YESNO) != IDYES)
		{
			m_chkWcByPass.SetCheck(TRUE);
			return;
		}

		strByPass = _T("N");
	}
	else
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("PASS 모드를 설정 하시겠습니까?")), MB_YESNO) != IDYES)
		{
			m_chkWcByPass.SetCheck(FALSE);
			return;
		}

		strByPass = _T("Y");
	}


	m_pDoc->BeginTrans_DLG();

	CString strSql = _T("");

	strSql.Format(_T(" UPDATE WC_DATA									\n")
				_T("    SET CHK_BYPASS_YN = '%s' 						\n")
				_T("  WHERE WH_TYP = '%s'								\n")
				_T("	AND PLC_NO = '%02s'								\n")
				_T("    AND WC_MC_NO = '%s'								\n"), strByPass, strWhTyp, strPlcNo, strWcMcNo);

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		strLOG_MSG.Format(_T("BYPASS CHECK -> WC NO : %s, BYPASS 체크 : %s" ), strWcMcNo, strByPass);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CWcSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("LOG 입력중 실패")));
			return;
		}

		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));

		if (strByPass == _T("N"))
		{
			m_edtWcByPass.SetWindowTextW(_T("비활성화"));
		}
		else
		{
			m_edtWcByPass.SetWindowTextW(_T("활성화"));
		}

		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
	return;
}


void CWcSkinDlg::OnBnClickedBtnWcWriteManual()
{
	CString strWhTyp = m_pWC_DATA->K_WH_TYP;
	CString strPlcNo = m_pWC_DATA->K_PLC_NO;
	CString strWcMcNo = m_pWC_DATA->K_WC_MC_NO;
	CString strRcvVal = _T("");
	int nRcvVal = 0;
	CString strLOG_MSG = _T("");

	UpdateData(TRUE);
	m_edtWcWriteVal.GetWindowText(strRcvVal);
	strRcvVal.Trim();
	strRcvVal.Remove(4);


	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("설정한 무게값을 수동으로 쓰시겠습니까?")), MB_YESNO) != IDYES)
	{
		return;
	}

	CString strLUGG_NO_RD = _T("");
	CString strDEST_POS_RD = _T("");
	CString strRET_READY_RD = _T("");

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	//WC CV의 상태 보기
	strSql.Format(_T("  SELECT LUGG_NO_RD, DEST_POS_RD, RET_READY_RD	")
				  _T("	  FROM CV_DATA					")
				  _T("   WHERE WH_TYP = '%s' 				")
				  _T("     AND MC_NO = '%s' 				"), strWhTyp, strWcMcNo);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("104번 트랙정보를 읽는중 에러.")));
		return;
	}

	pRsw->MoveFirst();
	//출고 HS 번호, 화물감지 유무
	strLUGG_NO_RD = pRsw->GetItem(_T("LUGG_NO_RD"));
	strDEST_POS_RD = pRsw->GetItem(_T("DEST_POS_RD"));
	strRET_READY_RD = pRsw->GetItem(_T("RET_READY_RD"));

	//pRsw->MoveNext();
	delete pRsw;

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

	strSql = _T("");

	strSql.Format(_T(" UPDATE WC_DATA									\n")
				_T("    SET WEIGHT_RCV_VAL = '%s' 						\n")
				_T("	   ,OD_RQ_ID = 'RQ' 					    	\n")
				_T("  WHERE WH_TYP = '%s'								\n")
				_T("	AND PLC_NO = '%02s'								\n")
				_T("    AND WC_MC_NO = '%s'								\n"), strRcvVal, strWhTyp, strPlcNo, strWcMcNo);

	BOOL isSuccess =  m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		strLOG_MSG.Format(_T("WC 수동 쓰기 -> WC NO : %s, 수동 쓰기 : %s" ), strWcMcNo, strRcvVal);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CWcSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("LOG 입력중 실패")));
			return;
		}

		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));

		m_edtWcWriteVal.SetWindowText(_T("0000.000"));

		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
	return;
}

void CWcSkinDlg::SelCommonCode(CString pCCD_CD, CString pCMD, CString& pCCD_NM_KOR)
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
