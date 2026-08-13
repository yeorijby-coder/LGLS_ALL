// BCRSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "BCRSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CBCRSkinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBCRSkinDlg, CSkinDialog)

CBCRSkinDlg::CBCRSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CBCRSkinDlg::IDD, pParent)
{
	m_pBCR_MST = NULL;
	m_pCV_DATA = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CBCRSkinDlg::CBCRSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CBCRSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pBCR_MST = NULL;
	m_pCV_DATA = NULL;

}


CBCRSkinDlg::~CBCRSkinDlg()
{
	this->DestroyWindow();
}

void CBCRSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LBL_BCR_LEVEL,				   m_lblBcrLevel);
	DDX_Control(pDX, IDC_LBL_BCR_BOTTOM,			   m_lblBcrBottom);
	DDX_Control(pDX, IDC_LBL_BCR_TOP,				   m_lblBcrTop);
	DDX_Control(pDX, IDC_LBL_BCR_STA,				   m_lblBcrSta);
	DDX_Control(pDX, IDC_LBL_BCR_TRACK_NO,			   m_lblBcrTrackNo);

	DDX_Control(pDX, IDC_EDT_BCR_NO,				   m_edtBcrNo);
	DDX_Control(pDX, IDC_EDIT_BCR_LEVEL,			   m_edtBcrLevel);
	DDX_Control(pDX, IDC_EDIT_BCR_BOTTOM,			   m_edtBcrBottom);
	DDX_Control(pDX, IDC_EDIT_BCR_TOP,				   m_edtBcrTop);
	DDX_Control(pDX, IDC_EDIT_BCR_STA,				   m_edtBcrSta);
	DDX_Control(pDX, IDC_EDIT_BCR_TRACK_NO,            m_edtBcrTrackNo);
	DDX_Control(pDX, IDC_EDIT_BCR_NOREAD_CNT,		   m_edtBcrNoreadCnt);
	DDX_Control(pDX, IDC_EDIT_BCR_SUSPEND,			   m_edtBcrSuspend);

	DDX_Control(pDX, IDC_BTN_BCR_READ_BARCODE,		   m_btnBcrReadBarcode);
	DDX_Control(pDX, IDC_BTN_BCR_WRITE_MANUAL,		   m_btnBcrWriteManual);
	DDX_Control(pDX, IDC_BTN_BCR_SUSPEND,			   m_btnBcrSuspend);
	DDX_Control(pDX, IDC_BTN_BCR_MES_REPORT,		   m_btnBcrMesReport);
	DDX_Control(pDX, IDC_BTN_ACTIVE,				   m_btnBcrActive);


	DDX_Control(pDX, IDC_GRP_BCR_COMMAND,				m_grpBcrCommand);
	DDX_Control(pDX, IDC_GRP_BCR_REPORT,				m_grpBcrReport);
	DDX_Control(pDX, IDC_GRP_BCR_REPORT_ITEM,			m_grpBcrReportItem);
	DDX_Control(pDX, IDC_GRP_BCR_REPORT_VALUE,			m_grpBcrReportValue);
	DDX_Control(pDX, IDC_GRP_BCR_TRACK_STATUS,			m_grpBcrTrackStatus);
	DDX_Control(pDX, IDC_GRP_BCR_TRACK_STATUS_ITEM,		m_grpBcrTrackStatusItem);
	DDX_Control(pDX, IDC_GRP_BCR_TRACK_STATUS_VALUE,	m_grpBcrTrackStatusValue);
	DDX_Control(pDX, IDC_GRP_BCR_ERROR_INFORMATION,		m_grpBcrErrorInfomation);
	DDX_Control(pDX, IDC_GRP_BCR_SUSPEND,				m_grpBcrSuspend);
}


BOOL CBCRSkinDlg::OnInitDialog()
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
	return TRUE;  // return TRUE  unless you set the focus to a control
}




void CBCRSkinDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RenameResource(nEN_LANG);
	RedrawImage();
}



void CBCRSkinDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manual_write"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_BCR_WRITE_MANUAL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manual_read"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_BCR_READ_BARCODE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("mes_report"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_BCR_MES_REPORT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcractive"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_ACTIVE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrsuspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_BCR_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrbottom"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_BCR_BOTTOM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrtop"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_BCR_TOP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrsta"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_BCR_STA, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrlev"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_BCR_LEVEL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("trackinfo"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_BCR_TRACK_NO, strValue);

	//grp
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_BCR_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrrep "), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_BCR_REPORT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item "), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_BCR_REPORT_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value "), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_BCR_REPORT_VALUE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("tracksta"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_BCR_TRACK_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_BCR_TRACK_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_BCR_TRACK_STATUS_VALUE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("error"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_BCR_ERROR_INFORMATION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\"), _T("dlg_bcr"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrsuspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_BCR_SUSPEND, strValue);

	
}


void CBCRSkinDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_bcr\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sctime\\128x32\\");

	CString strExtension = _T(".png");

	m_btnBcrReadBarcode.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnBcrReadBarcode.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);

	m_btnBcrWriteManual.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0 ,0);
	m_btnBcrWriteManual.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);

	m_btnBcrSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0 ,0);
	m_btnBcrSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);

	m_btnBcrMesReport.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0 ,0);
	m_btnBcrMesReport.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("copy"), strExtension)), NULL, 5, 5);
	
	m_btnBcrActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0 ,0);
	m_btnBcrActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("write"), strExtension)), NULL, 5, 5);
}


BEGIN_MESSAGE_MAP(CBCRSkinDlg, CSkinDialog)
	ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CBCRSkinDlg::OnMessageSwitch)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_BCR_READ_BARCODE, &CBCRSkinDlg::OnBnClickedBtnBcrReadBarcode)
	ON_BN_CLICKED(IDC_BTN_BCR_WRITE_MANUAL, &CBCRSkinDlg::OnBnClickedBtnBcrWriteManual)
	ON_BN_CLICKED(IDC_BTN_BCR_SUSPEND, &CBCRSkinDlg::OnBnClickedBtnBcrSuspend)
	ON_BN_CLICKED(IDC_BTN_BCR_MES_REPORT, &CBCRSkinDlg::OnBnClickedBtnBcrMesReport)
	ON_BN_CLICKED(IDC_BTN_ACTIVE, &CBCRSkinDlg::OnBnClickedBtnActive)
END_MESSAGE_MAP()



LRESULT CBCRSkinDlg::OnMessageSwitch(WPARAM wParam, LPARAM lParam)
{
	CBCR_MST* pBCR_MST = (CBCR_MST*)wParam;
	if(pBCR_MST == NULL){ return 0; };
	m_pBCR_MST = pBCR_MST;
	if(m_pCV_DATA == NULL)
	{
		m_pCV_DATA = (CCV_DATA*)(m_pDoc->GetTrackInfoNew(m_pBCR_MST->m_strBCR_MC_NO)->m_pCV_DATA);
	}
	if(m_pCV_DATA == NULL){ return 0; };

	EN_LANG enLangTemp = (EN_LANG)lParam;
	if(m_nLang != enLangTemp)
	{
		m_nLang = enLangTemp;
		InvalidateLangControl(enLangTemp);
	}

	InvalidateBcrData(enLangTemp);

	return 0;
}

// CRtvSkinDlg 메시지 처리기입니다.
void CBCRSkinDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;


	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnBcrReadBarcode.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnBcrReadBarcode.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnBcrWriteManual.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnBcrWriteManual.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);


	m_btnBcrMesReport.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnBcrMesReport.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);


	

	m_btnBcrSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnBcrSuspend.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnBcrActive.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnBcrActive.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

}


void CBCRSkinDlg::InvalidateBcrData(EN_LANG pLang)
{	
	if(m_pDoc == NULL) return;
	if(m_pBCR_MST == NULL) return;
	if(m_pCV_DATA == NULL) return;

	if (!UpdateData())
		return;
	CString strView;
	strView.Format(_T("BCR %s TRACK %s"), m_pBCR_MST->K_BCR_NO, m_pBCR_MST->m_strBCR_MC_NO);
	m_edtBcrNo.SetWindowText(strView);
	m_edtBcrLevel.SetWindowText(m_pBCR_MST->V_BCR_LEV);//BCR LEV
	m_edtBcrBottom.SetWindowText(m_pCV_DATA->V_BCR_BOTTOM);
	m_edtBcrTop.SetWindowText(m_pCV_DATA->V_BCR_TOP);
	m_edtBcrTrackNo.SetWindowText(m_pBCR_MST->m_strBCR_MC_NO);//BCR_MC_NO

	CString strSql = GetQrySelectBCR_STATUS_CCD(m_pBCR_MST);
	CString strMessage;
	int nRowCnt = -1;
	_RecordsetPtr ptr =  m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if(nRowCnt <= 0){	return;	  }
	CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
	pRsw->MoveFirst(); 
	for(int i = 0; i < nRowCnt; i++)
	{
		m_edtBcrSta.SetWindowText(pRsw->GetItem(_T("BCR_STA")));
		m_edtBcrSuspend.SetWindowText(pRsw->GetItem(_T("SUSPEND")));
	}


	CString strNoreadCnt = m_pBCR_MST->V_NOREAD_CNT;

	if (strNoreadCnt != _T("0"))
	{
		strNoreadCnt = _T("NoRead");
	}
	m_edtBcrNoreadCnt.SetWindowText(strNoreadCnt);
	UpdateData(FALSE);
	InvalidateLangControl(pLang);
}


void CBCRSkinDlg::InvalidateLangControl( EN_LANG enLangTemp )
{ // 데이터가 아닌 레이블 / 버튼 등의 값만 수정시킬것
	if(m_pDoc == NULL)
		return;

	//언어추가할것
	//m_lblScJobNo.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호"), enLangTemp));
}




void CBCRSkinDlg::OnClose()
{
	m_pDoc->m_pBcrSkinDlg = NULL;
	delete this;
}


void CBCRSkinDlg::OnBnClickedBtnBcrReadBarcode()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strBcrNo = m_pBCR_MST->K_BCR_NO;

	strSql.Format(_T(" UPDATE BCR_MST				\n")
		_T("    SET BCR_STA = '1'			\n")
		_T("        , NOREAD_CNT = '0'		\n")
		_T("		, CMD_RQ_YN = 'Y'       \n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	  AND BCR_NO= '%s'			  "), strWhTyp, strBcrNo);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;	
}


void CBCRSkinDlg::OnBnClickedBtnBcrWriteManual()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strBcrNo = m_pBCR_MST->K_BCR_NO;
	CString strBcrMcNo = m_pBCR_MST->V_BCR_MC_NO;
	CString strBcrTop = _T("");
	CString strBcrBottom  = _T("");

	UpdateData(TRUE);
	m_edtBcrTop.GetWindowText(strBcrTop);
	m_edtBcrBottom.GetWindowText(strBcrBottom);


	if (!m_pDoc->Permission(_T("CBCRSkinDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}


	if (strBcrTop == _T("0") || strBcrBottom == _T("0"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드상단, 하단 데이터 확인")));
		return;	
	}

	if (strBcrTop == _T("") || strBcrBottom == _T(""))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드상단, 하단 데이터 확인")));
		return;	
	}

	if (IsValidataEditData(strBcrTop, strBcrBottom)  == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("바코드상단, 하단 데이터 확인")));
		return;	
	}
	if (strBcrTop != _T("0"))
	{
		strSql.Format(_T(" UPDATE CV_DATA			\n")
			_T("    SET BCR_BOTTOM = '%s'	\n")
			_T("  WHERE WH_TYP = '%s'		\n")
			_T("	  AND TRACK_NO= '%s'	  "), strBcrBottom, strWhTyp, strBcrMcNo);
	}else if (strBcrBottom != _T("0"))
	{
		strSql.Format(_T(" UPDATE CV_DATA			\n")
			_T("    SET BCR_TOP = '%s'	\n")
			_T("  WHERE WH_TYP = '%s'		\n")
			_T("	  AND TRACK_NO= '%s'	  "), strBcrTop, strWhTyp, strBcrMcNo);
	}else{
		strSql.Format(_T(" UPDATE CV_DATA			\n")
			_T("    SET BCR_TOP = '%s'	\n")
			_T("      , BCR_BOTTOM = %s'	\n")
			_T("  WHERE WH_TYP = '%s'		\n")
			_T("	  AND TRACK_NO= '%s'	  "), strBcrTop, strBcrBottom, strWhTyp, strBcrMcNo);
	}

	BOOL isSuccess = m_pDoc->GetSelectQryCnt(strSql);

	if(isSuccess == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;	
	}


	strSql.Format(_T(" UPDATE BCR_MST				\n")
		_T("    SET CMD_RQ_YN = 'Y'       \n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	  AND BCR_NO= '%s'			  "), strWhTyp, strBcrNo);

	isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;	

}
BOOL CBCRSkinDlg::IsValidataEditData(CString strBcrTop, CString strBcrBottom)
{
	if (strBcrTop.GetLength() > 7){return FALSE;}
	if (strBcrBottom.GetLength() > 7){return FALSE;}


	if (!m_pDoc->Permission(_T("CBCRSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return FALSE;

	}

	return TRUE;
}


void CBCRSkinDlg::OnBnClickedBtnBcrSuspend()
{
	CString strSql=_T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strBcrNo = m_pBCR_MST->K_BCR_NO;


	if (!m_pDoc->Permission(_T("CBCRSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	if (m_pBCR_MST->V_SUSPEND == _T("1"))
		return;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("BCR 사용을 중지하시겠습니까?")), MB_YESNO) != IDYES)
		return;

	m_pDoc->BeginTrans_DLG();

	CString strLOG_LUGG_NO = _T("0");
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
	CString strLOG_BOTTOM_TRAY = _T("");
	CString strLOG_TOP_TRAY = _T("");
	CString strLOG_MSG = _T("BCR SUSPEND. BCR NO : ") + strBcrNo;

	if (!m_pDoc->GetQueryInsertClientLog(_T("CBcrSkinDlg"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}


	strSql.Format(_T(" UPDATE BCR_MST				\n")
		_T("    SET SUSPEND = '1'       \n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	AND BCR_NO= '%s'			  "), strWhTyp, strBcrNo);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;	

}


void CBCRSkinDlg::OnBnClickedBtnBcrMesReport()
{
	CString strSql = _T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strBcrNo = m_pBCR_MST->K_BCR_NO;
	CString strBcrBottom = m_pCV_DATA->V_BCR_BOTTOM;

	if (strBcrBottom == _T("") || strBcrBottom == _T(""))
	{
		return;
	}

	strSql.Format(_T(" UPDATE BCR_MST				\n")
		          _T("    SET BCR_STA = '2'			\n")
				  _T("	  , CMD_RQ_YN = 'Y'       \n")
		          _T("  WHERE WH_TYP = '%s'			\n")
		          _T("	  AND BCR_NO= '%s'			  "), strWhTyp, strBcrNo);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;	
}


void CBCRSkinDlg::OnBnClickedBtnActive()
{
	CString strSql=_T("");
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strBcrNo = m_pBCR_MST->K_BCR_NO;


	if (!m_pDoc->Permission(_T("CBCRSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	if (m_pBCR_MST->V_SUSPEND == _T("0"))
		return;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("BCR을 사용하시겠습니까?")), MB_YESNO) != IDYES)
		return;

	m_pDoc->BeginTrans_DLG();

	CString strLOG_LUGG_NO = _T("");
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
	CString strLOG_BOTTOM_TRAY = _T("");
	CString strLOG_TOP_TRAY = _T("");
	CString strLOG_MSG = _T("BCR ACTIVE. BCR NO : ") + strBcrNo;

	if (!m_pDoc->GetQueryInsertClientLog(_T("CBcrSkinDlg"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}

	strSql.Format(_T(" UPDATE BCR_MST				\n")
		_T("    SET SUSPEND = '0'			   \n")
		_T("  WHERE WH_TYP = '%s'			  \n")
		_T("	AND BCR_NO= '%s'			  \n")
		_T("	AND SUSPEND = '1'			"), strWhTyp, strBcrNo);

	BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;	
}

CString CBCRSkinDlg::GetQrySelectBCR_STATUS_CCD( CBCR_MST* pBCR_MST )
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");

	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(CCD_BCR_STA.CCD_NM_KOR,'0') AS BCR_STA ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_SUSPEND.CCD_NM_KOR, '0') AS SUSPEND ");
	strSql += CRLF + _T("  FROM BCR_MST BM LEFT OUTER JOIN COMMON_CODE CCD_BCR_STA");
	strSql += CRLF + _T("                          ON CCD_BCR_STA.CDX_CD = 'BCR_STA' ");
	strSql += CRLF + _T("                         AND CCD_BCR_STA.CCD_CD = BM.BCR_STA ");
	strSql += CRLF + _T("                         AND CCD_BCR_STA.WH_TYP LIKE ") + CLib::QuotLikeLR(pBCR_MST->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_SUSPEND ");
	strSql += CRLF + _T("                          ON CCD_SUSPEND.CDX_CD = 'BCR_SUSPEND' ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.CCD_CD = BM.SUSPEND ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.WH_TYP LIKE ") + CLib::QuotLikeLR(pBCR_MST->K_WH_TYP);
	strSql += CRLF + _T(" WHERE BM.WH_TYP = ") + CLib::Quot(pBCR_MST->K_WH_TYP);
	strSql += CRLF + _T("   AND BM.BCR_NO = ") + CLib::Quot(pBCR_MST->K_BCR_NO);
	
	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}
