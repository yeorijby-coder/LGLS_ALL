// SystemScTime.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "ViewUsageRackDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"

// CSystemScTimeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewUsageRackDlg, CSkinDialog)

CViewUsageRackDlg::CViewUsageRackDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewUsageRackDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_pDoc->m_hWndViewRackDlg = ((CViewUsageRackDlg*)AfxGetMainWnd())->m_hWnd; 
}

CViewUsageRackDlg::CViewUsageRackDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewUsageRackDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}


CViewUsageRackDlg::~CViewUsageRackDlg()
{
	m_pDoc->m_pViewUsageRackDlg = NULL;
	this->DestroyWindow();
}

void CViewUsageRackDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CELL_MST, m_pSpdCellList);
	//DDX_Control(pDX, ID_SYSTEM_SCTIME_OK, m_btnOk);
	//DDX_Control(pDX, ID_SYSTEM_SCTIME_CANCEL, m_btnCancel);
	//DDX_Control(pDX, IDC_BTN_AGING_TYPE, m_btnAgingType);
	//DDX_Control(pDX, IDC_BTN_QRY_SELECT, m_btnQrySelect);
	//DDX_Control(pDX, IDC_DRPBX_AGING_TYPE, m_drbtnAgingType);
	//DDX_Control(pDX, IDC_MBTN_SELECT, m_mbtnSelect);
	DDX_Control(pDX, ID_BTN_SELECT, m_btnSelect);
	DDX_Control(pDX, ID_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CMB_WH_TYP, m_cbxWhTyp);
	//  DDX_Control(pDX, IDC_CMB_DEST_POS, m_cbxAging);
	DDX_Control(pDX, IDC_CMB_DEST_POS, m_cbxDestPos);
	DDX_Control(pDX, IDC_CMB_AGING, m_cbxAging);
	DDX_Control(pDX, IDC_CMB_BANK, m_cbxBank);
	DDX_Control(pDX, IDC_STATIC_SELECT, m_grpUsageCondition);
	DDX_Control(pDX, IDC_STATIC_INFORM, m_grpUsageInform);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_grpUsagePreview);
	DDX_Control(pDX, IDC_USAGE_SELECT,         m_grpUsageSelect);
	DDX_Control(pDX, IDC_TOTAL_SELECT_CNT, m_grpUsageSelect);
	DDX_Control(pDX, IDC_LBL_WH, m_lblWhTyp);
	DDX_Control(pDX, IDC_LBL_USAGE_CELL, m_lblUsageCell);
	DDX_Control(pDX, IDC_LBL_SUM_TRAY, m_lblSumTray);
	DDX_Control(pDX, IDC_LBL_RSV_STO, m_lblRsvSto);
	DDX_Control(pDX, IDC_LBL_RSV_RET, m_lblRsvRet);
	DDX_Control(pDX, IDC_LBL_POSSIBLE_STO, m_lblPossibleSto);
	DDX_Control(pDX, IDC_LBL_POSSIBLE_RET, m_lblPossibleRet);
	DDX_Control(pDX, IDC_LBL_NORMAL, m_lblNormal);
	DDX_Control(pDX, IDC_LBL_EMPTY_TRAY, m_lblEmptyTray);
	DDX_Control(pDX, IDC_LBL_EMPTY_ERR, m_lblEmptyErr);
	DDX_Control(pDX, IDC_LBL_EMPTY_CELL, m_lblEmptyCell);
	DDX_Control(pDX, IDC_LBL_DUAL_ERR, m_lblDualErr);
	DDX_Control(pDX, IDC_LBL_CELL_TYP, m_lblCellTyp);
	DDX_Control(pDX, IDC_LBL_CELL_STATUS, m_lblCellStatus);
	//  DDX_Control(pDX, IDC_LBL_CELL_MST_RESULT, m_lblCellMstResult);
	DDX_Control(pDX, IDC_LBL_CELL_CNT, m_lblCellCnt);
	DDX_Control(pDX, IDC_LBL_BANK, m_lblBank);
	DDX_Control(pDX, IDC_LBL_BAN_STO, m_lblBanSto);
	DDX_Control(pDX, IDC_LBL_BAN_RET, m_lblBanRet);
	DDX_Control(pDX, IDC_LBL_AGING, m_lblAging);
	//  DDX_Control(pDX, IDC_LBL_BNK, m_lblBak);
	DDX_Control(pDX, IDC_LBL_BNK, m_lblBnk);
	DDX_Control(pDX, IDC_LBL_AC_TRAY, m_lblAcTray);
	DDX_Control(pDX, IDC_LBL_BAN_USER, m_lblBanUser);
	//  DDX_Control(pDX, IDC_LBL_CELL_MST_CNT, m_lblCellMstCnt);
	DDX_Control(pDX, IDC_LBL_BAN_PHB, m_lblBanPhb);
	DDX_Control(pDX, IDC_RDO_RACK, m_rdoUsage);
	DDX_Control(pDX, IDC_RDO_FIRE, m_rdoFire);


}


BEGIN_MESSAGE_MAP(CViewUsageRackDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_SYSTEM_SCTIME_OK, &CViewUsageRackDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_SYSTEM_SCTIME_CANCEL, &CViewUsageRackDlg::OnBnClickedSystemSctimeCancel)
	ON_BN_CLICKED(IDC_BTN_QRY_SELECT, &CViewUsageRackDlg::OnBnClickedBtnQrySelect)
	ON_BN_CLICKED(IDC_BTN_AGING_TYPE, &CViewUsageRackDlg::OnBnClickedBtnAgingType)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_BTN_SELECT, &CViewUsageRackDlg::OnBnClickedBtnSelect)
	ON_MESSAGE(WM_USER_REFRESH_COLLECT_CELLINFO, &CViewUsageRackDlg::OnRefreshDialog)
	ON_CBN_SELCHANGE(IDC_CMB_WH_TYP, &CViewUsageRackDlg::OnSelChangeCmbWhTyp)
	ON_CBN_SELCHANGE(IDC_CMB_AGING, &CViewUsageRackDlg::OnSelChangeCmbAging)
END_MESSAGE_MAP()


void CViewUsageRackDlg::OnClose()
{
	m_pDoc->m_pViewUsageRackDlg = NULL;
	delete this;
}

BOOL CViewUsageRackDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	
	InitializeFontManager(this);

	//--------------------------------------------------------------------------------
	// 따로 Initiallize로 뺄 것

	//SetWindowTheme(GetDlgItem(IDC_STATIC)->m_hWnd, _T(""), _T(""));
	m_mbtnSelect.EnableWindowsTheming(FALSE); //윈도우 테마 제거
	//((CMFCButton*)GetDlgItem(IDC_MBTN_INFORM))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_PREVIEW))->EnableWindowsTheming(FALSE);
	//((CMFCButton*)GetDlgItem(IDC_MBTN_POSSIBLE_STO))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_RSV_STO))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_POSSIBLE_RET))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_RSV_RET))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_DUAL_ERR))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_EMPTY_ERR))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_NORMAL))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_BAN_STO))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_BAN_RET))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_BAN_USER))->EnableWindowsTheming(FALSE);
	((CMFCButton*)GetDlgItem(IDC_MBTN_BAN_PHB))->EnableWindowsTheming(FALSE);	
	//((CMFCButton*)GetDlgItem(IDC_MBTN_SELECT))->SetFaceColor(RGB(255,255,0));
	//SetWindowTheme(GetDlgItem(IDC_STATIC13)->m_hWnd, _T(""), _T(""));
	m_mbtnSelect.SetFaceColor(RGB(54,114,191));

	//((CMFCButton*)GetDlgItem(IDC_MBTN_INFORM))->SetFaceColor(RGB(54,114,191));
	((CMFCButton*)GetDlgItem(IDC_MBTN_PREVIEW))->SetFaceColor(RGB(211,211,211));
	//((CMFCButton*)GetDlgItem(IDC_MBTN_POSSIBLE_STO))->SetFaceColor(RGB(255,255,255));
	((CMFCButton*)GetDlgItem(IDC_MBTN_RSV_STO))->SetFaceColor(RGB(0,255,255));
	((CMFCButton*)GetDlgItem(IDC_MBTN_POSSIBLE_RET))->SetFaceColor(RGB(0,255,0));
	((CMFCButton*)GetDlgItem(IDC_MBTN_RSV_RET))->SetFaceColor(RGB(255,255,0));
	((CMFCButton*)GetDlgItem(IDC_MBTN_DUAL_ERR))->SetFaceColor(RGB(205,92,92));
	((CMFCButton*)GetDlgItem(IDC_MBTN_EMPTY_ERR))->SetFaceColor(RGB(255,140,0));
	((CMFCButton*)GetDlgItem(IDC_MBTN_NORMAL))->SetFaceColor(RGB(255,255,255));
	((CMFCButton*)GetDlgItem(IDC_MBTN_BAN_STO))->SetFaceColor(RGB(46,139,87));
	((CMFCButton*)GetDlgItem(IDC_MBTN_BAN_RET))->SetFaceColor(RGB(128,128,0));
	((CMFCButton*)GetDlgItem(IDC_MBTN_BAN_USER))->SetFaceColor(RGB(0,0,255));
	((CMFCButton*)GetDlgItem(IDC_MBTN_BAN_PHB))->SetFaceColor(RGB(255,0,0));
	
	m_pSpdCellList.SetFontSize(14);
	m_rdoUsage.SetCheck(1);
		
	/*

	//----------------------------------------------------------------------------------
	m_tabUsage = (CTabCtrl *)GetDlgItem(IDC_TAB_USAGE);
	if(m_tabUsage == NULL)
	{
		return FALSE;
	}
	*/

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	CString strAGING_TYP;

	InitializeFontManager(this);
	SetFontNation((int)pEn);
	

	CLib::BindCombo(m_cbxWhTyp, _T("WH_TYP"), m_pDoc, (int)pEn, FALSE);
	CLib::BindCombo(m_cbxAging, _T("AGING_TYP"), m_pDoc, (int)pEn, FALSE);
	strAGING_TYP = m_cbxAging.GetItemKey(m_cbxAging.GetCurSel());
	CLib::BindCombo_ViewUsageRackDlg(m_cbxBank, m_pDoc, (int)pEn, strAGING_TYP);
	//CLib::BindCombo(m_cbxBank, _T("BANK"), m_pDoc, (int)pEn);

	CSkinDialog::SetFont(this->GetFont());

	
	InitializeRACK_LIST(true);

	if( !m_bInitialized )
	{		
		RelocationControls();


		m_bInitialized = TRUE;		
	}
	InitializeResource(pEn);

	return TRUE;

	 
	
	//CWnd::SetTimer(1,3000,NULL);
	//Invalidate(TRUE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CViewUsageRackDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_WH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("agingtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_AGING, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bank"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_BNK, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)m_enLang);
	SetDlgItemText(ID_BTN_SELECT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cancel"), (int)m_enLang);
	SetDlgItemText(ID_BTN_CANCEL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("totalcnt"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CELL_CNT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("realtray"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_AC_TRAY, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("emptray"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EMPTY_TRAY, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("usetotal"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SUM_TRAY, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("empcell"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EMPTY_CELL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("usecn"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_USAGE_CELL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cellstatus"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CELL_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cellatturibute"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CELL_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stook"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_POSSIBLE_STO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("nomal"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_NORMAL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("storeservation"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RSV_STO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stoban"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_BAN_STO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("retok"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_POSSIBLE_RET, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("retban"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_BAN_RET, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("retreservation"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_RSV_RET, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("userban"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_BAN_USER, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bubblesto"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_DUAL_ERR, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("useimpossible"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_BAN_PHB, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("empret"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_EMPTY_ERR, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)m_enLang);
	SetDlgItemText(IDC_STATIC_SELECT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("inform"), (int)m_enLang);
	SetDlgItemText(IDC_STATIC_INFORM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("preview"), (int)m_enLang);
	SetDlgItemText(IDC_STATIC_PREVIEW, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)m_enLang);
	SetDlgItemText(IDC_TOTAL_SELECT_CNT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("option"), (int)m_enLang);
	SetDlgItemText(IDC_USAGE_SELECT, strValue);



	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_usagerack\\"), _T("dlg_usagerack"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_CELL_MST_RESULT, strValue);

	


	

}

void CViewUsageRackDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	CString strAppPath_BaseButton = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\128x32\\");

	CString strExtension = _T(".png");
	m_btnSelect.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnSelect.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("select"), strExtension)), NULL, 5, 5);

	strExtension = _T(".png");
 	m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
 	m_btnCancel.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("cancel"), strExtension)), NULL, 5, 5);

	//m_btnQrySelect.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	//m_btnQrySelect.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("select"), strExtension)), NULL, 5, 5);

}



void CViewUsageRackDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
	{
		m_bInitialized = TRUE;	
	}
}

HCURSOR CViewUsageRackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CViewUsageRackDlg::OnPaint()
{
		CSkinDialog::OnPaint();
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CSkinDialog::OnPaint();
	}

}

void CViewUsageRackDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

// 	x = rc.Width() - 20 - size.cx;
// 	y = rc.Height() - 10 - size.cy;
// 	m_btnCancel.MoveWindow(x, y, size.cx, size.cy, TRUE);
// 
// 	x -= size.cx + 10;
// 	m_btnOk.MoveWindow(x, y, size.cx, size.cy, TRUE);
// 	m_btnOk.Invalidate();


	CRect rc2;
	m_btnSelect.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnSelect.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCancel.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCancel.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}


// CSystemScTimeDlg 메시지 처리기입니다.


void CViewUsageRackDlg::OnBnClickedOk()
{
	CSkinDialog::OnOK();
}


void CViewUsageRackDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CSkinDialog::OnTimer(nIDEvent);
}


void CViewUsageRackDlg::OnBnClickedSystemSctimeCancel()
{
	CSkinDialog::OnCancel();
}

void CViewUsageRackDlg::OnBnClickedBtnQrySelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CViewUsageRackDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RedrawImage();
	RenameResource(nEN_LANG);
}


CString CViewUsageRackDlg::GetTabIdxEnumToCString(EN_TAB_IDX enTabIdx)
{
	CString strReturn = _T("CV");
	if(enTabIdx == EN_TAB_IDX_CV){ return _T("CV");}
	if(enTabIdx == EN_TAB_IDX_SC){ return _T("SC");}
	return strReturn;
}


void CViewUsageRackDlg::OnBnClickedBtnAgingType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int a = 0;
}


HBRUSH CViewUsageRackDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	//if(pWnd->GetDlgCtrlID() == IDC_BTN_SELECT)
	//{ 
	//	//pDC->SetBkMode(TRANSPARENT);
	//	//pDC->SetTextColor(RGB(0,0,0));
	//	pDC->SetBkColor(0xCDFAFF);
	//	
	//	//GetDlgItem(IDC_STATIC13)->SetB

	//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	//}
	//if(pWnd->GetDlgCtrlID() == IDC_LBL_POSSIBLE_STO ||
	//	pWnd->GetDlgCtrlID() == IDC_LBL_NORMAL	||
	//	pWnd->GetDlgCtrlID() == IDC_LBL_RSV_STO	||
	//	pWnd->GetDlgCtrlID() == IDC_LBL_BAN_STO	||
	//	pWnd->GetDlgCtrlID() == IDC_LBL_POSSIBLE_RET	||
	//	pWnd->GetDlgCtrlID() == IDC_LBL_BAN_RET	||
	//	pWnd->GetDlgCtrlID() == IDC_LBL_RSV_RET	|| 
	//	pWnd->GetDlgCtrlID() == IDC_LBL_BAN_USER	||
	//	pWnd->GetDlgCtrlID() == IDC_LBL_DUAL_ERR	||
	//	pWnd->GetDlgCtrlID() == IDC_LBL_BAN_PHB	||
	//	pWnd->GetDlgCtrlID() == IDC_LBL_EMPTY_ERR ||
	//	pWnd->GetDlgCtrlID() == IDC_LBL_CELL_STATUS || 
	//	pWnd->GetDlgCtrlID() == IDC_LBL_CELL_TYP )
	//{
	//	//pDC->SetBkMode(TRANSPARENT);
	//	//pDC->SetTextColor(RGB(178,255,255));
	//	pDC->SetBkColor(RGB(255,255,255));


	//	//return CreateSolidBrush(RGB(120, 120, 120));
	//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	//	//return (HBRUSH)::GetStockObject(NULL_BRUSH);

	//}



	//if(pWnd->GetDlgCtrlID() == IDC_USAGE_SELECT|| 
	//   pWnd->GetDlgCtrlID() == IDC_STATIC13 || 
	//   pWnd->GetDlgCtrlID() == IDC_STATIC_SELECT ||
	//   pWnd->GetDlgCtrlID() == IDC_STATIC_INFORM ||
	//   pWnd->GetDlgCtrlID() == IDC_TOTAL_SELECT_CNT ||
	//   pWnd->GetDlgCtrlID() == IDC_STATIC_PREVIEW)
	//{ 
	//	//pDC->SetBkMode(TRANSPARENT);
	//	//pDC->SetTextColor(RGB(178,255,255));
	//	pDC->SetBkColor(RGB(255,255,255));


	//	//return CreateSolidBrush(RGB(120, 120, 120));
	//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	//	//return (HBRUSH)::GetStockObject(NULL_BRUSH);
	//}
	//if(pWnd->GetDlgCtrlID() == IDC_LBL_CELL_CNT			|| 
	//   pWnd->GetDlgCtrlID() == IDC_LBL_AC_TRAY			|| 
	//   pWnd->GetDlgCtrlID() == IDC_LBL_EMPTY_TRAY		|| 
	//   pWnd->GetDlgCtrlID() == IDC_LBL_SUM_TRAY			|| 
	//   pWnd->GetDlgCtrlID() == IDC_LBL_EMPTY_CELL		|| 
	//   pWnd->GetDlgCtrlID() == IDC_LBL_USAGE_CELL		||
	//   pWnd->GetDlgCtrlID() == IDC_LBL_WH				||
	//   pWnd->GetDlgCtrlID() == IDC_LBL_CELL_MST_RESULT	||
	//   pWnd->GetDlgCtrlID() == IDC_LBL_CELL_MST_CNT		||
	//   pWnd->GetDlgCtrlID() == IDC_LBL_BNK				||
	//   pWnd->GetDlgCtrlID() == IDC_LBL_AGING)
	//{ 
	//	//pDC->SetBkMode(TRANSPARENT);
	//	//pDC->SetTextColor(RGB(255,255,255));
	//	pDC->SetBkColor(RGB(255,255,255));
	//	

	//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	//}


	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CViewUsageRackDlg::InitializeRACK_LIST(BOOL bSearch)
{
	CString strSql = GetJobMstQry(bSearch);
	int nSizeCol = 0;

	int nColSize = 8;
	int nRowSize = 12;
}

void CViewUsageRackDlg::FillSpread(int nColIdx, int nRowIdx, CString strValue)
{
	CString aaa = _T("");
	aaa.Format(_T("%s\r"), strValue);
	m_pSpdCellList.SetText(nColIdx, nRowIdx, variant_t(aaa));

}

void CViewUsageRackDlg::FillSpreadRow(int nColIdx, int nRowIdx, CString strValue)
{
	CString aaa = _T("");
	aaa.Format(_T("%s\r"), strValue);
	m_pSpdCellList.SetText(nColIdx + 1, nRowIdx + 1, variant_t(aaa));
	
}

void CViewUsageRackDlg::FillSpreadColumn(int nColIdx, CString strColumnName)
{	
	m_pSpdCellList.SetText( nColIdx + 1, 0, variant_t(strColumnName));	
}

CString CViewUsageRackDlg::GetJobMstQry(BOOL bSearch)
{
	
	CString strSql = _T("");

	return strSql;
}


void CViewUsageRackDlg::OnBnClickedBtnSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//InitializeRACK_LIST(TRUE);


	//콤보박스를 DB에서 가져와서 CellInfo 맵에 채울 것
	
	CString strAGING_TYP, strBANK, strWH_TYP;

	strAGING_TYP = m_cbxAging.GetItemKey(m_cbxAging.GetCurSel());
	strBANK = m_cbxBank.GetItemKey(m_cbxBank.GetCurSel());
	strWH_TYP = m_cbxWhTyp.GetItemKey(m_cbxWhTyp.GetCurSel());

	m_pDoc->m_CollectCellInfo.m_strAGING_TYP = strAGING_TYP;
	m_pDoc->m_CollectCellInfo.m_strBANK  = strBANK;
	m_pDoc->m_CollectCellInfo.m_strWH_TYP = strWH_TYP;

	m_pDoc->m_CollectCellInfo.m_bRequest = TRUE;

	//STCollectDataList_CellInfo_Request
}

LRESULT CViewUsageRackDlg::OnRefreshDialog(WPARAM wParam, LPARAM lParam)
{
	//CEquipment::EN_KIND enEquipKind = CEquipment::EN_KIND(lParam);

	CMap<CString, LPCTSTR, CCellInfo*, CCellInfo*>* pMapCellInfo = (CMap<CString, LPCTSTR, CCellInfo*, CCellInfo*>*)(WPARAM)wParam;
	int a = pMapCellInfo->GetCount();
	a = pMapCellInfo->GetSize();

	int aa = 0 ;
	int nMaxLev = 0;	//ROW
	int nMaxBay = 0;	//COLMN
	int nCrrentLev = 0; //ROW
	int nCrrentBay = 0; //COLMN
	int nCellTray = 0;
	int nEmptyTray = 0;
	int nEmptyRack = 0;

	switch(CConvert::ToInt(m_pDoc->m_CollectCellInfo.m_strAGING_TYP))
	{
	case 291 :
		nMaxBay = 22;
		nMaxLev = 12;
		break;
	case 292 :
		nMaxBay = 23;
		nMaxLev = 12;
		break;
	case 293 :
		nMaxBay = 80;
		nMaxLev = 15;
		break;
	case 294 :
		nMaxBay = 23;
		nMaxLev = 12;
		break;
	}

	m_pSpdCellList.SetMaxCols(nMaxBay);
	m_pSpdCellList.SetMaxRows(nMaxLev);

	int nRadio = GetCheckedRadioButton(IDC_RDO_RACK, IDC_RDO_FIRE);  
	m_pSpdCellList.ClearRange(0, 0, 80, 15, false);

	for(int i = 0; i < pMapCellInfo->GetCount(); i++)
	{		
		CCellInfo* pCellInfo = NULL;
		pMapCellInfo->Lookup(m_pDoc->m_CollectCellInfo.m_strAGING_TYP.Mid(0,3)+ CConvert::ToString(i+1), pCellInfo);

		if(i != 0 && i % nMaxLev == 0)
		{
			nCrrentLev = 0;	//ROW 값을 초기화
			nCrrentBay++; //COLMN 값을 옆으로 늘려줌
		}


		if(nRadio == IDC_RDO_FIRE)
		{
			FillSpread(CConvert::ToInt(pCellInfo->BAY), CConvert::ToInt(pCellInfo->LEV), _T("26.3℃"));
		}
		else
		{
			if(pCellInfo->CELL_STA == "1")
			{
				m_pSpdCellList.SetCol(CConvert::ToInt(pCellInfo->BAY));
				m_pSpdCellList.SetRow(CConvert::ToInt(pCellInfo->LEV));
				m_pSpdCellList.SetBackColor(RSV_STO);

			}
			else if(pCellInfo->CELL_STA == "2")
			{
				nCellTray++;
				m_pSpdCellList.SetCol(CConvert::ToInt(pCellInfo->BAY));
				m_pSpdCellList.SetRow(CConvert::ToInt(pCellInfo->LEV));
				m_pSpdCellList.SetBackColor(POSSIBLE_RET);
				//m_pSpdCellList.SetTextTip(2);
	
				FillSpread(CConvert::ToInt(pCellInfo->BAY), CConvert::ToInt(pCellInfo->LEV), _T("●"));
			}
			else if(pCellInfo->CELL_STA == "3")
			{
				m_pSpdCellList.SetCol(CConvert::ToInt(pCellInfo->BAY));
				m_pSpdCellList.SetRow(CConvert::ToInt(pCellInfo->LEV));
				m_pSpdCellList.SetBackColor(RSV_RET);
			}
			else if(pCellInfo->CELL_STA == "0")
			{
				nEmptyRack++;
			}

			if(pCellInfo->CELL_USE_DEF == "1")
			{
				m_pSpdCellList.SetCol(CConvert::ToInt(pCellInfo->BAY));
				m_pSpdCellList.SetRow(CConvert::ToInt(pCellInfo->LEV));
				m_pSpdCellList.SetBackColor(BAN_STO);
			}
			else if(pCellInfo->CELL_USE_DEF == "2")
			{
				m_pSpdCellList.SetCol(CConvert::ToInt(pCellInfo->BAY));
				m_pSpdCellList.SetRow(CConvert::ToInt(pCellInfo->LEV));
				m_pSpdCellList.SetBackColor(BAN_RET);
			}	
			else if(pCellInfo->CELL_USE_DEF == "3")
			{
				m_pSpdCellList.SetCol(CConvert::ToInt(pCellInfo->BAY));
				m_pSpdCellList.SetRow(CConvert::ToInt(pCellInfo->LEV));
				m_pSpdCellList.SetBackColor(BAN_USER);
			}	
			else if(pCellInfo->CELL_USE_DEF == "9")
			{
				m_pSpdCellList.SetCol(CConvert::ToInt(pCellInfo->BAY));
				m_pSpdCellList.SetRow(CConvert::ToInt(pCellInfo->LEV));
				m_pSpdCellList.SetBackColor(BAN_PHB);
			}	
		}

		nCrrentLev++;
	}

	float fUsageRack = ( float((nCellTray + nEmptyTray)) / float(pMapCellInfo->GetCount()) * 100);
	CString strUsageRack;
	strUsageRack.Format(_T("%.3f%%"), fUsageRack);
	SET(IDC_EDIT_ALL_CELL, CConvert::ToString(pMapCellInfo->GetCount()));
	SET(IDC_EDIT_AC_TRAY, CConvert::ToString(nCellTray));
	SET(IDC_EDIT_EMPTY_TRAY, CConvert::ToString(nEmptyTray));
	SET(IDC_EDIT_SUM_CELL, CConvert::ToString(nCellTray + nEmptyTray));
	SET(IDC_EDIT_EMPTY_CELL, CConvert::ToString(nEmptyRack));
	SET(IDC_EDIT_USAGE_CELL, strUsageRack);


	return 0;

}

void CViewUsageRackDlg::OnSelChangeCmbWhTyp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	SetFontNation((int)pEn);
	CLib::BindCombo(m_cbxAging, _T("AGING_TYP"), m_pDoc, (int)pEn);
}	

void CViewUsageRackDlg::OnSelChangeCmbAging()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	CString strAGING_TYP;

	SetFontNation((int)pEn);

	strAGING_TYP = m_cbxAging.GetItemKey(m_cbxAging.GetCurSel());
	
	CLib::BindCombo_ViewUsageRackDlg(m_cbxBank, m_pDoc, (int)pEn, strAGING_TYP);
}
