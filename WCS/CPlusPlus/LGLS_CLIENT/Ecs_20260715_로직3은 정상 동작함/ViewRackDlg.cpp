// ViewRackDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "ViewRackDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CViewRackDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewRackDlg, CSkinDialog)

CViewRackDlg::CViewRackDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewRackDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CViewRackDlg::CViewRackDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewRackDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
}

CViewRackDlg::~CViewRackDlg()
{
	m_pDoc->m_pViewRackDlg = NULL;
	this->DestroyWindow();
}

void CViewRackDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CELL_MST, m_pSpreadMain);

	DDX_Control(pDX, IDC_LBL_CELL_WH_TYP, m_lblCellWhTyp);
	DDX_Control(pDX, IDC_LBL_CELL_SCNO,  m_lblCellCellScNo);
	DDX_Control(pDX, IDC_LBL_CELL_SC_NO, m_lblCellScNo);
	DDX_Control(pDX, IDC_LBL_CELL_NO, m_lblCellNo);
	DDX_Control(pDX, IDC_LBL_CELL_STA, m_lblCellSta);
	DDX_Control(pDX, IDC_LBL_CELL_USE_DEF, m_lblCellUseDef);
	DDX_Control(pDX, IDC_LBL_AGING_DT, m_lblAgingDt);
	DDX_Control(pDX, IDC_LBL_PLT_NO, m_lblBcrTop);
	DDX_Control(pDX, IDC_LBL_RACK_BCR_BOTTOM, m_lblBcrBottom);
	DDX_Control(pDX, IDC_LBL_REMARK, m_lblRemark);
	DDX_Control(pDX, IDC_LBL_VIEW_CELL_MST, m_lblDestPos);
	DDX_Control(pDX, IDC_LBL_VIEW_CELL_USE_DEF, m_lblCellUserDef2);
	DDX_Control(pDX, IDC_LBL_CELL_STA2, m_lblCellSta2);
	DDX_Control(pDX, IDC_LBL_RACK_BCR_TOP2, m_lblRackBcrTop2);
	DDX_Control(pDX, IDC_LBL_RACK_BCR_BOTTOM2, m_lblRackBcrBottom2);
	DDX_Control(pDX, IDC_LBL_REMARK3, m_lblRemarks3);

	DDX_Control(pDX, IDC_EDIT_RACK_BCR_TOP, m_edtTopTray);
	DDX_Control(pDX, IDC_EDIT_RACK_BCR_BOTTOM, m_edtBottmTray);
	DDX_Control(pDX, IDC_EDIT_REMARK, m_edtRemarks);
	DDX_Control(pDX, IDC_EDIT_REMARK2, m_edtRemarks2);
	DDX_Control(pDX, IDC_EDIT_RACK_BCR_TOP2, m_edtTopTray2);
	DDX_Control(pDX, IDC_EDIT_RACK_BCR_BOTTOM2, m_edtBottomTray2);

	DDX_Control(pDX, IDC_BTN_CELL_NO_FR_TO, m_btnCellNoFrTo);
	DDX_Control(pDX, IDC_BTN_AGING_DT_FR_TO, m_btnAgingDtFrTo);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BTN_RETIREVE, m_btnRetireve);
	DDX_Control(pDX, IDC_BTN_CELL_USE_DEF, m_btnCellUseDefEdit);

	DDX_Control(pDX, IDC_CMB_CELL_WH_TYP, m_cmbWhTyp);
	DDX_Control(pDX, IDC_CMB_CELL_SC_NO, m_cmbCellScNo);
//	DDX_Control(pDX, IDC_CMB_SC_NO, m_cmbScNo);
	DDX_Control(pDX, IDC_CMB_CELL_STA, m_cmbCellSta);
	DDX_Control(pDX, IDC_CMB_CELL_STA2, m_cmbCellSta2);
	DDX_Control(pDX, IDC_CMB_CELL_USE_DEF, m_cmbCellUseDef);
	
	DDX_Control(pDX, IDC_CMB_CELL_AGING_TYP, m_cmbAgingTyp);
	DDX_Control(pDX, IDC_CMB_VIEW_DEST_POS,	m_cmbDestPos);
	DDX_Control(pDX, IDC_CMB_CELL_USE_DEF2, m_cmbCellUseDef2);

	DDX_Control(pDX, IDC_MASK_EDIT_CELL_NO_FR, m_MaskEditCellNoFr);
	DDX_Control(pDX, IDC_MASK_EDIT_CELL_NO_TO, m_MaskEditCellNoTo);

	DDX_Control(pDX, IDC_DT_AGING_START_D, m_dtAgingStartD);
	DDX_Control(pDX, IDC_DT_AGING_START_T, m_dtAgingStartT);
	DDX_Control(pDX, IDC_DT_AGING_END_D, m_dtAgingEndD);
	DDX_Control(pDX, IDC_DT_AGING_END_T, m_dtAgingEndT);

	DDX_Control(pDX, ID_VIEW_RACK_OK, m_btnOk);
	DDX_Control(pDX, ID_VIEW_RACK_CANCEL, m_btnCancel);

	DDX_Control(pDX, IDC_LBL_CELL_MST_RESULT, m_lblCellMstResult);
	DDX_Control(pDX, IDC_LBL_CELL_MST_CNT,	  m_lblSpdMainCnt);

	DDX_Control(pDX, IDC_BTN_CELL_RET,		 m_btnCellRet);

	DDX_Control(pDX, IDC_GRP_CELL_SEARCH,    m_grpCellSearch);
	DDX_Control(pDX, IDC_GRP_CELL_SEARCH2,   m_grpCellSearch2);
	DDX_Control(pDX, IDC_GRP_CELL_EDIT,	 m_grpCellMst);
	DDX_Control(pDX, IDC_GRP_CELL_EDIT2,     m_grpCellEdit);


}


BEGIN_MESSAGE_MAP(CViewRackDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CViewRackDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_CELL_USE_DEF, &CViewRackDlg::OnBnClickedBtnCellUseDef)
	ON_BN_CLICKED(IDC_BTN_CELL_NO_FR_TO, &CViewRackDlg::OnBnClickedBtnCellNoFrTo)
	ON_BN_CLICKED(IDC_BTN_AGING_DT_FR_TO, &CViewRackDlg::OnBnClickedBtnAgingDtFrTo)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DT_AGING_END_T, &CViewRackDlg::OnDtnDatetimechangeDtAgingEndT)
	ON_BN_CLICKED(ID_VIEW_RACK_CANCEL, &CViewRackDlg::OnBnClickedViewRackCancel)
	ON_BN_CLICKED(IDC_BTN_CELL_RET, &CViewRackDlg::OnBnClickedBtnCellRet)
	ON_CBN_SELCHANGE(IDC_CMB_CELL_SC_NO, &CViewRackDlg::OnSetfocusCmbCellScNo)
	ON_CBN_SELCHANGE(IDC_CMB_CELL_AGING_TYP, &CViewRackDlg::OnSetfocusCmbAingTyp)
END_MESSAGE_MAP()

BOOL CViewRackDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	m_nActiveRow = 0;
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	m_pSpreadMain.SetFontName(Global.GetFontName(pEn));
	if( !m_bInitialized )
	{		
		RelocationControls();
		m_bInitialized = TRUE;

	}

	InitializeResource(pEn);
	//시간 데이트타임 포맷

	m_MaskEditCellNoFr.EnableMask(_T("dd ddd dd"),_T("__-___-__"), _T(' '));
	m_MaskEditCellNoFr.SetWindowText(_T("01-001-01"));

	m_MaskEditCellNoTo.EnableMask(_T("dd ddd dd"),_T("__-___-__"), _T(' '));
	m_MaskEditCellNoTo.SetWindowText(_T("99-999-99"));


	m_strWH_TYP = m_pDoc->m_WH_TYP;

	CLib::BindCombo(m_cmbWhTyp, _T("WH_TYP"), m_pDoc, (int)pEn, FALSE);
	CLib::BindCombo(m_cmbAgingTyp, _T("AGING_TYP"), m_pDoc, (int)pEn, TRUE);
	CLib::BindCombo(m_cmbCellSta, _T("CELL_STA"), m_pDoc, (int)pEn, TRUE);
	CLib::BindCombo(m_cmbCellSta2, _T("CELL_STA"), m_pDoc, (int)pEn, FALSE);
	CLib::BindCombo(m_cmbCellUseDef, _T("CELL_USE_DEF"), m_pDoc, (int)pEn, TRUE);	
	CLib::BindCombo(m_cmbCellUseDef2, _T("CELL_USE_DEF"), m_pDoc, (int)pEn, FALSE);

	CLib::BindCombo_CELL_SC_NO(m_cmbCellScNo, _T("CELL_SC_NO"), m_pDoc, (int)pEn, m_pAging, m_pScNo);
	//CLib::GetComBoBoxData(m_cmbCellScNo, m_pCellScNo, 10);
	m_pCellScNo = m_cmbCellScNo.GetItemKey(m_cmbCellScNo.GetCurSel());
	CLib::BindCombo_SC_HS_DEF_DEST(m_cmbDestPos, m_pDoc, (int)pEn, m_pCellScNo);

	CTime tStartD, tStartT, tEndD, tEndT;
	

	CString strStartTime = _T("2000-01-01 00:00:00");
	CString strEndTime = _T("2999-12-31 23:59:59");
	COleDateTime tStartTime,tEndTime;

	tStartTime = CLib::ConvertCStringToCOleDateTime(strStartTime);
	tEndTime = CLib::ConvertCStringToCOleDateTime(strEndTime);

	m_dtAgingStartD.SetTime(tStartTime);
	m_dtAgingStartT.SetTime(tStartTime);
	m_dtAgingEndD.SetTime(tEndTime);
	m_dtAgingEndT.SetTime(tEndTime);
	
	m_dtAgingStartT.SetFormat(_T("HH:mm:ss"));
	m_dtAgingEndT.SetFormat(_T("HH:mm:ss"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CViewRackDlg::InitializeControlLanguage()
{
	SetDlgItemText(IDC_LBL_CELL_WH_TYP, m_pDoc->m_pLang->GetLangValue(_T("창고구분"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_CELL_STA, m_pDoc->m_pLang->GetLangValue(_T("CELL상태"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_CELL_AGING_TYP, m_pDoc->m_pLang->GetLangValue(_T("AGING타입"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_CELL_USE_DEF, m_pDoc->m_pLang->GetLangValue(_T("CELL사용정의"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_CELL_SC_NO, m_pDoc->m_pLang->GetLangValue(_T("SC번호"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_CELL_NO, m_pDoc->m_pLang->GetLangValue(_T("CELL번호"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_CELL_SCNO, m_pDoc->m_pLang->GetLangValue(_T("CELL SC번호"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_PLT_NO, m_pDoc->m_pLang->GetLangValue(_T("상단BCR"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_RACK_BCR_BOTTOM, m_pDoc->m_pLang->GetLangValue(_T("하단BCR"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_AGING_DT, m_pDoc->m_pLang->GetLangValue(_T("AGING시간"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_REMARK, m_pDoc->m_pLang->GetLangValue(_T("비고"), m_pDoc->m_enLang));

	SetDlgItemText(IDC_LBL_VIEW_CELL_MST, m_pDoc->m_pLang->GetLangValue(_T("도착지"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_VIEW_CELL_USE_DEF, m_pDoc->m_pLang->GetLangValue(_T("CELL사용정의"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_CELL_STA2, m_pDoc->m_pLang->GetLangValue(_T("CELL상태"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_RACK_BCR_TOP2, m_pDoc->m_pLang->GetLangValue(_T("상단BCR"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_RACK_BCR_BOTTOM2, m_pDoc->m_pLang->GetLangValue(_T("하단BCR"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_LBL_REMARK3, m_pDoc->m_pLang->GetLangValue(_T("비고"), m_pDoc->m_enLang));

	SetDlgItemText(IDC_LBL_CELL_MST_RESULT, m_pDoc->m_pLang->GetLangValue(_T("결과"), m_pDoc->m_enLang));

	SetDlgItemText(IDC_BTN_SEARCH, m_pDoc->m_pLang->GetLangValue(_T("조회"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_BTN_CELL_RET, m_pDoc->m_pLang->GetLangValue(_T("출고"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_BTN_CELL_USE_DEF, m_pDoc->m_pLang->GetLangValue(_T("수정"), m_pDoc->m_enLang));

	SetDlgItemText(IDC_GRP_CELL_SEARCH, m_pDoc->m_pLang->GetLangValue(_T("조회"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_GRP_CELL_SEARCH2, m_pDoc->m_pLang->GetLangValue(_T("조회결과"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_GRP_CELL_EDIT2, m_pDoc->m_pLang->GetLangValue(_T("수정"), m_pDoc->m_enLang));
	SetDlgItemText(IDC_GRP_CELL_EDIT, m_pDoc->m_pLang->GetLangValue(_T("수정"), m_pDoc->m_enLang));
}

void CViewRackDlg::FillSpreadColumn(int nColIdx, CString strColumnName)
{
	//m_pSpCellMst.SetText( nColIdx + 1, 0, variant_t(m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang)));
}

void CViewRackDlg::FillSpreadRow(int nColIdx, int nRowIdx, CString strValue)
{	
	//CString aaa = _T("");
	//aaa.Format(_T("%s\r"), strValue);
	//m_pSpCellMst.SetText(nColIdx + 1, nRowIdx + 1, variant_t(aaa));
}

void CViewRackDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
	{
		m_bInitialized = TRUE;
	}
}


HCURSOR CViewRackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CViewRackDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CViewRackDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RedrawImage();
	RenameResource(nEN_LANG);
}


void CViewRackDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;

	m_btnRetireve.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRetireve.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnSearch.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCellRet.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCellRet.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnCellUseDefEdit.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnCellUseDefEdit.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}


BOOL CViewRackDlg::PreTranslateMessage(MSG* pMsg)
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

void CViewRackDlg::OnBnClickedBtnSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	InitializeSpread(TRUE);
	//m_pSpreadMain.SetFontItalic(TRUE);
	m_pSpreadMain.Invalidate(TRUE);
}

//MASK EDIT CELL_NO 복사
void CViewRackDlg::OnBnClickedBtnCellNoFrTo()
{
	CString strCellNoTo = _T("");
	CString strCellNoFr = _T("");
	UpdateData(TRUE);
	
	m_MaskEditCellNoTo.GetWindowText(strCellNoTo);

	CString strBankTo = strCellNoTo.Mid(0,2);
	CString strBayTo = strCellNoTo.Mid(2,3);
	CString strLevelTo = strCellNoTo.Mid(5,2);
	
	m_MaskEditCellNoFr.EnableMask(_T("dd ddd dd"),_T("__-___-__"), _T(' '));
	strCellNoFr.Format(_T("%s-%s-%s"),strBankTo,strBayTo,strLevelTo);


	m_MaskEditCellNoFr.SetWindowText(strCellNoFr);
}

//날짜 형식 복사
void CViewRackDlg::OnBnClickedBtnAgingDtFrTo()
{
	CTime tDATE_TO, tTIME_TO;

	UpdateData(TRUE);

	m_dtAgingEndD.GetTime(tDATE_TO);
	m_dtAgingEndT.GetTime(tTIME_TO);
	
	m_dtAgingStartD.SetTime(&tDATE_TO);
	m_dtAgingStartT.SetTime(&tTIME_TO);

}

void CViewRackDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pDoc->m_pViewRackDlg = NULL;
	CSkinDialog::OnClose();
}


void CViewRackDlg::OnDtnDatetimechangeDtAgingEndT(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

//CELL상태 변경 - 완제품
void CViewRackDlg::OnBnClickedBtnCellSta()
{
	variant_t val;
	CStringList strManualSeqList;
	bool bSuccess = false;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CELL상태를 수정 하시겠습니까?")), MB_YESNO) != IDYES) 
		return;

	CString strInsWhTyp = _T("");
	CString strInsCellNo = _T("");
	CString strSql = _T("");
	CString strIntSql = _T("");
	CString strUpdBottomTray = _T("");
	CString strUpdTopTray = _T("");
	m_strWH_TYP = m_pDoc -> m_WH_TYP;
	
	m_edtBottmTray.GetWindowText(strUpdBottomTray);
	m_edtBottmTray.GetWindowText(strUpdTopTray);


	for (int i = 1; i <= m_pSpreadMain.GetMaxRows(); i++)
	{
		m_pSpreadMain.GetText(1, i, &val);
		CString strCheck = (LPCTSTR)(_bstr_t)val;
		bSuccess = false;

		if (strCheck == "1")
		{
			int nCnt = 0;

			nCnt += 1;

			m_pSpreadMain.GetText(enWH_TYP, i, &val);
			CString strWhTyp = (LPCTSTR)(_bstr_t)val;

			m_pSpreadMain.GetText(enCELL_SC_NO, i, &val);
			CString strCellScNo = (LPCTSTR)(_bstr_t)val;

			m_pSpreadMain.GetText(enSC_NO, i, &val);
			CString strScNo = (LPCTSTR)(_bstr_t)val;

			m_pSpreadMain.GetText(enCELL_NO, i, &val);
			CString strCellNo = (LPCTSTR)(_bstr_t)val;
			
			m_pSpreadMain.GetText(enCELL_STA, i, &val);
			CString strCellSta = (LPCTSTR)(_bstr_t)val;
			

			CString strSql = _T("");

			strWhTyp.Trim();
			strCellScNo.Trim();
			strScNo.Trim();
			strCellNo.Trim();
			strCellSta.Trim();

			
			CString strBank = strCellNo.Mid(0,2);
			CString strBay  = strCellNo.Mid(3,3);
			CString strLevel = strCellNo.Mid(7,2);
			strInsCellNo = strInsCellNo + strBank +strBay +strLevel + _T(",");
			strInsWhTyp = strWhTyp;	

			///////////////////////////테스트 출고작업 생성 수정 필요!!!///////////////////
			UpdateData(TRUE);
			CString strCellStaUpd = _T("");
			//CString strCellSta = _T("");
			//m_cmbCellSta.GetWindowText(strCellStaUpd);
			CLib::GetComBoBoxData(m_cmbCellSta,strCellSta);

			long bTrans = m_pDoc->BeginTrans_DLG();
			
				if (bTrans < 1)
					return;

			strSql = _T("");
			strSql.Format(_T("UPDATE CELL_MST		\n")
			_T("	  SET CELL_STA = '%s'			\n"), strCellSta);
				
			if (strCellSta == "2")
			{
				strSql += _T("	  , BOTTOM_TRAY = '") + strUpdBottomTray + _T("'			\n");
				strSql += _T("	  , TOP_TRAY = '") + strUpdTopTray + _T("'			\n");
			}
			strSql.Format( strSql + _T("    WHERE WH_TYP = '%s'		\n")
				_T("      AND CELL_SC_NO = '%s'			\n")
				_T("	  AND CELL_NO = '%s'			\n")
				_T("      AND SC_NO = '%s'				\n"), strWhTyp, strCellScNo, strCellNo, strScNo);

			BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

			if (isSuccess == FALSE)
			{
				m_pDoc->RollbackTrans_DLG();
				return;
			}

			if (strCellSta == "2")
			{

				strSql.Format(_T(" INSERT INTO CELL_DTL( WH_TYP         	 \n")
								_T("				   , CELL_NO        	 \n")
								_T("				   , CELL_SC_NO     	 \n")
								_T("				   , CELL_SEQ       	 \n")
								_T("				   , BANK           	 \n")
								_T("				   , BAY            	 \n")
								_T("				   , LEV            	 \n")
								_T("				   , AGING_START_DT 	 \n")
								_T("				   , AGING_END_DT   	 \n")
								_T("				   , USER_REMARKS   	 \n")
								_T("				   , REMARKS        	 \n")
								_T("				   , INS_DT         	 \n")
								_T("				   , INS_USER_ID    	 \n")
								_T("				   , UPD_DT         	 \n")
								_T("				   , UPD_USER_ID    	 \n")
								_T("				   , BOTTOM_TRAY    	 \n")
								_T("				   , TOP_TRAY)       	 \n")
								_T("			VALUES ( '%s'		      	 \n")
								_T("				   , '%s'		      	 \n")
								_T("				   , '%s'		      	 \n")
								_T("				   , 1			      	 \n")
								_T("				   , '%s'		      	 \n")
								_T("				   , '%s'		      	 \n")
								_T("				   , '%s'		      	 \n")
								_T("				   , ") +m_pDoc->SYSDATE + _T("		     \n")
								_T("				   , ") +m_pDoc->SYSDATE + _T("		     \n")
								_T("				   , ''		      		 \n")
								_T("				   , 'MANUAL INSERT'	 \n")
								_T("				   , ") +m_pDoc->SYSDATE+ _T("		     \n")
								_T("				   , '%s'		     \n")
								_T("				   , ") +m_pDoc->SYSDATE+ _T("		     \n")
								_T("				   , '%s'		     \n")
								_T("				   , '%s'		      	 \n")
								_T("				   , '%s')		      	 \n") , strWhTyp, strCellNo, strCellScNo, strBank, strBay, strLevel, m_pDoc->m_strId, m_pDoc->m_strId, strUpdBottomTray, strUpdTopTray);

				BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);
				
				if (isSuccess == FALSE)
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}


			if (m_pDoc->m_WH_TYP == "40")
			{
				
				CString strInsCellNo ;
				strInsCellNo.Format(_T("%s%s%s"), strBank, strBay, strLevel); 

				//strSql.Format(_T(" INSERT INTO CELL_MES_REQ(  WH_TYP \n")
				//_T("					                , MES_CMD  \n")
				//_T("					                , MES_RQ  \n")
				//_T("									, PLT_ID  \n")
				//_T("									, PLT_ID_NEW  \n")
				//_T("					                , STA_POSITIONS \n")
				//_T("                                    , STA_STATE  \n")
				//_T("                                    , USER_ID  \n")
				//_T("                                    , MES_ACK  \n")
				//_T("                                    , INS_DT  \n")
				//_T("                                    , INS_USER_ID ) \n")
				//_T("                            VALUES ('%s' \n")
				//_T("                                   ,'S6F11_CEID13' \n") 
				//_T("                                   ,'RQ' \n")
				//_T("                                   ,'%s'  \n")
				//_T("                                   ,'%s'  \n")
				//_T("                                   ,'%s'  \n")
				//_T("                                   ,'0' \n")
				//_T("                                   ,'CLIENT' \n") 
				//_T("                                   ,'ACK' \n")
				//_T("                                   , now() \n")
				//_T("					               , 'CLIENT' )")  , strInsWhTyp, strUP_BCR_, strUpdBottomTray, strInsCellNo);


				//strSql.Format(_T(" INSERT INTO CELL_MES_REQ(  WH_TYP \n")
				//	_T("					                , MES_CMD  \n")
				//	_T("					                , MES_RQ  \n")
				//	_T("					                , STA_POSITIONS \n")
				//	_T("                                    , STA_STATE  \n")
				//	_T("                                    , USER_ID  \n")
				//	_T("                                    , MES_ACK  \n")
				//	_T("                                    , INS_DT  \n")
				//	_T("                                    , INS_USER_ID ) \n")
				//	_T("                            VALUES ('%s' \n")
				//	_T("                                   ,'S6F11_CEID21' \n") 
				//	_T("                                   ,'RQ' \n")
				//	_T("                                   ,'%s'  \n")
				//	_T("                                   ,'0' \n")
				//	_T("                                   ,'CLIENT' \n") 
				//	_T("                                   ,'ACK' \n")
				//	_T("                                   , now() \n")
				//	_T("					               , 'CLIENT' )")  , strWhTyp, strInsCellNo);

				BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

				if(isSuccess == FALSE)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
					bSuccess = true;
					return;
				}
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SUCCESS")));
			}

			m_pDoc->CommitTrans_DLG();
		}
	}

	if (m_pDoc->m_WH_TYP == "40")
	{

		if(strInsCellNo == _T(""))
		{
			return;
		}

		strInsCellNo = strInsCellNo.Left(strInsCellNo.GetLength()-1);

		strSql.Format(_T(" INSERT INTO CELL_MES_REQ(  WH_TYP \n")
			_T("					                , MES_CMD  \n")
			_T("					                , MES_RQ  \n")
			_T("					                , STA_POSITIONS \n")
			_T("                                    , STA_STATE  \n")
			_T("                                    , USER_ID  \n")
			_T("                                    , MES_ACK  \n")
			_T("                                    , INS_DT  \n")
			_T("                                    , INS_USER_ID ) \n")
			_T("                            VALUES ('%s' \n")
			_T("                                   ,'S6F11_CEID21' \n") 
			_T("                                   ,'RQ' \n")
			_T("                                   ,'%s'  \n")
			_T("                                   ,'0' \n")
			_T("                                   ,'%s' \n") 
			_T("                                   ,'ACK' \n")
			_T("                                   , ") + m_pDoc->SYSDATE + _T(" \n")
			_T("					               , '%s' )")  , strInsWhTyp, strInsCellNo, m_pDoc->m_strId, m_pDoc->m_strId);

		strIntSql = strSql;
		
		BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strIntSql);

		if(isSuccess == FALSE)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			bSuccess = true;
			return;
		}
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SUCCESS")));
	}

	for(int nIdxRow = 1; nIdxRow < m_pSpreadMain.GetDataRowCnt() + 1; nIdxRow++)
	{
		m_pSpreadMain.SetText(1, nIdxRow, variant_t(_T("0")));
	}

	m_cmbCellSta.SetWindowText(_T(""));
	InitializeSpread(TRUE);

	return;	
}

//파렛트 수정 - 완제픔
void CViewRackDlg::OnBnClickedBtnPltUpdate()
{

	variant_t val;
	CStringList strManualSeqList;
	bool bSuccess = false;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("파렛트를 변경 하시겠습니까?")), MB_YESNO) != IDYES) 
		return;

	
	CString strInsWhTyp = _T("");
	CString strInsCellNo = _T("");
	CString strSql = _T("");
	CString strUpdBottomTray = _T("");
	CString strUpdTopTray = _T("");
	CString strPltId = _T("");

	UpdateData(TRUE);
	
	m_edtBottmTray.GetWindowText(strUpdBottomTray);
	m_edtBottmTray.GetWindowText(strUpdTopTray);

	if ((strUpdBottomTray == _T("")) && (strUpdTopTray == _T("")))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("변경할 파렛트 번호가 없습니다.")));
		return;
	}

	for (int i = 1; i <= m_pSpreadMain.GetMaxRows(); i++)
	{
			m_pSpreadMain.GetText(1, i, &val);
			CString strCheck = (LPCTSTR)(_bstr_t)val;
			bSuccess = false;

			if (strCheck == "1")
			{
				int nCnt = 0;

				nCnt += 1;

				m_pSpreadMain.GetText(enWH_TYP, i, &val);
				CString strWhTyp = (LPCTSTR)(_bstr_t)val;

				m_pSpreadMain.GetText(enCELL_SC_NO, i, &val);
				CString strCellScNo = (LPCTSTR)(_bstr_t)val;

				m_pSpreadMain.GetText(enSC_NO, i, &val);
				CString strScNo = (LPCTSTR)(_bstr_t)val;

				m_pSpreadMain.GetText(enCELL_NO, i, &val);
				CString strCellNo = (LPCTSTR)(_bstr_t)val;

				m_pSpreadMain.GetText(enCELL_STA, i, &val);
				CString strCellSta = (LPCTSTR)(_bstr_t)val;

				m_pSpreadMain.GetText(enBOTTOM_TRAY, i, &val);
				CString strBottomTray = (LPCTSTR)(_bstr_t)val;


				CString strSql = _T("");

				strWhTyp.Trim();
				strCellScNo.Trim();
				strScNo.Trim();
				strCellNo.Trim();
				strCellSta.Trim();

			
				UpdateData(TRUE);
				CString strCellStaUpd = _T("");
				m_cmbCellSta.GetWindowText(strCellStaUpd);
				
				long bTrans = m_pDoc->BeginTrans_DLG();
			
				if (bTrans < 1)
					return;
				
				strSql=_T("");
				strSql.Format(_T("UPDATE CELL_MST")
					_T("	  SET BOTTOM_TRAY= '%s'")
					_T("	    , TOP_TRAY= '%s'")
					_T("    WHERE WH_TYP = '%s'")
					_T("      AND CELL_SC_NO = '%s'")
					_T("	  AND CELL_NO = '%s'  ")
					_T("      AND SC_NO = '%s'"), strUpdBottomTray, strUpdTopTray, strWhTyp, strCellScNo, strCellNo, strScNo);

				BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

				if (isSuccess == FALSE)
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}

				strSql=_T("");
				strSql.Format(_T("UPDATE CELL_DTL")
					_T("	  SET BOTTOM_TRAY= '%s'")
					_T("	    , TOP_TRAY= '%s'")
					_T("    WHERE WH_TYP = '%s'")
					_T("      AND CELL_SC_NO = '%s'")
					_T("	  AND CELL_NO = '%s'  ")
					_T("      AND SC_NO = '%s'"), strUpdBottomTray, strUpdTopTray, strWhTyp, strCellScNo, strCellNo, strScNo);

				isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

				if (isSuccess == FALSE)
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}

				m_pDoc->CommitTrans_DLG();
			
				
				CString strBank = strCellNo.Mid(0,2);
				CString strBay  = strCellNo.Mid(3,3);
				CString strLevel = strCellNo.Mid(7,2);
				strInsCellNo = strInsCellNo + strBank +strBay +strLevel + _T(",");

				strInsWhTyp = strWhTyp;
				strPltId = strBottomTray;
			}
	}
	
	if (m_pDoc->m_WH_TYP == "40")
	{
		if(strInsCellNo == _T(""))
		{
			return;
		}

		strInsCellNo = strInsCellNo.Left(strInsCellNo.GetLength()-1);

		strInsWhTyp.Trim();
		strPltId.Trim();
		strUpdBottomTray.Trim();
		strInsCellNo.Trim();

		strSql.Format(_T(" INSERT INTO CELL_MES_REQ(  WH_TYP \n")
			_T("					                , MES_CMD  \n")
			_T("					                , MES_RQ  \n")
			_T("									, PLT_ID  \n")
			_T("									, PLT_ID_NEW  \n")
			_T("					                , STA_POSITIONS \n")
			_T("                                    , STA_STATE  \n")
			_T("                                    , USER_ID  \n")
			_T("                                    , MES_ACK  \n")
			_T("                                    , INS_DT  \n")
			_T("                                    , INS_USER_ID ) \n")
			_T("                            VALUES ('%s' \n")
			_T("                                   ,'S6F11_CEID13' \n") 
			_T("                                   ,'RQ' \n")
			_T("                                   ,'%s'  \n")
			_T("                                   ,'%s'  \n")
			_T("                                   ,'%s'  \n")
			_T("                                   ,'0' \n")
			_T("                                   ,'%s' \n") 
			_T("                                   ,'ACK' \n")
			_T("                                   , ") + m_pDoc->SYSDATE + _T(" \n")
			_T("					               , '%s' )")  , strInsWhTyp, strPltId, strUpdBottomTray, strInsCellNo, m_pDoc->m_strId, m_pDoc->m_strId);

		BOOL isIntSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if(isIntSuccess == FALSE)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			bSuccess = true;
			return;
		}

		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SUCCESS")));

	}

	for(int nIdxRow = 1; nIdxRow < m_pSpreadMain.GetDataRowCnt() + 1; nIdxRow++)
	{
		m_pSpreadMain.SetText(1, nIdxRow, variant_t(_T("0")));
	}

	m_edtBottmTray.SetWindowText(_T(""));
	InitializeSpread(TRUE);
	return;	
	

}

//파렛트 삭제 - 완제품
void CViewRackDlg::OnBnClickedBtnPltDelete()
{
	variant_t val;
	CStringList strManualSeqList;
	bool bSuccess = false;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("파렛트를 삭제 하시겠습니까?")), MB_YESNO) != IDYES) 
		return;


	CString strInsWhTyp = _T("");
	CString strInsCellNo = _T("");
	CString strSql = _T("");
	CString strUpdBottomTray = _T("");
	CString strPltId = _T("");

	UpdateData(TRUE);

	for (int i = 1; i <= m_pSpreadMain.GetMaxRows(); i++)
	{
		m_pSpreadMain.GetText(1, i, &val);
		CString strCheck = (LPCTSTR)(_bstr_t)val;
		bSuccess = false;

		if (strCheck == "1")
		{
			int nCnt = 0;

			nCnt += 1;

			m_pSpreadMain.GetText(enWH_TYP, i, &val);
			CString strWhTyp = (LPCTSTR)(_bstr_t)val;

			m_pSpreadMain.GetText(enCELL_SC_NO, i, &val);
			CString strCellScNo = (LPCTSTR)(_bstr_t)val;

			m_pSpreadMain.GetText(enSC_NO, i, &val);
			CString strScNo = (LPCTSTR)(_bstr_t)val;

			m_pSpreadMain.GetText(enCELL_NO, i, &val);
			CString strCellNo = (LPCTSTR)(_bstr_t)val;

			m_pSpreadMain.GetText(enCELL_STA, i, &val);
			CString strCellSta = (LPCTSTR)(_bstr_t)val;

			m_pSpreadMain.GetText(enBOTTOM_TRAY, i, &val);
			CString strBottomTray = (LPCTSTR)(_bstr_t)val;


			CString strSql = _T("");

			strWhTyp.Trim();
			strCellScNo.Trim();
			strScNo.Trim();
			strCellNo.Trim();
			strCellSta.Trim();


			///////////////////////////테스트 출고작업 생성 수정 필요!!!///////////////////
			UpdateData(TRUE);
			long bTrans = m_pDoc->BeginTrans_DLG();
			
			if (bTrans < 1)
				return;

			strSql=_T("");
			strSql.Format(_T("UPDATE CELL_MST		")
				_T("	  SET BOTTOM_TRAY = ''		")
				_T("	    , TOP_TRAY = ''			")
				_T("	    , CELL_STA = '0'		")
				_T("    WHERE WH_TYP = '%s'			")
				_T("      AND CELL_SC_NO = '%s'		")
				_T("	  AND CELL_NO = '%s'  ")
				_T("      AND SC_NO = '%s'"), strWhTyp, strCellScNo, strCellNo, strScNo);

			BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

			if (isSuccess == FALSE)
			{
				m_pDoc->RollbackTrans_DLG();
				return;
			}

			strSql.Format(_T("DELETE FROM CELL_DTL				")
				_T("				WHERE WH_TYP = '%s'			")
				_T("				  AND CELL_SC_NO = '%s'		")
				_T("				  AND CELL_NO = '%s'		")
				_T("				  AND SC_NO = '%s'			"), strWhTyp, strCellScNo, strCellNo, strScNo);

			isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

			if (isSuccess == FALSE)
			{
				m_pDoc->RollbackTrans_DLG();
				return;
			}
			m_pDoc->CommitTrans_DLG();
			
		}
	}

	if(strInsCellNo == _T(""))
	{
		return;
	}


	if (m_pDoc->m_WH_TYP == "40")
	{
		strInsCellNo = strInsCellNo.Left(strInsCellNo.GetLength()-1);



		strSql.Format(_T(" INSERT INTO CELL_MES_REQ(  WH_TYP \n")
			_T("					                , MES_CMD  \n")
			_T("					                , MES_RQ  \n")
			_T("									, PLT_ID  \n")
			_T("					                , STA_POSITIONS \n")
			_T("                                    , STA_STATE  \n")
			_T("                                    , USER_ID  \n")
			_T("                                    , MES_ACK  \n")
			_T("                                    , INS_DT  \n")
			_T("                                    , INS_USER_ID ) \n")
			_T("                            VALUES ('%s' \n")
			_T("                                   ,'S6F11_CEID14' \n") 
			_T("                                   ,'RQ' \n")
			_T("                                   ,'%s'  \n")
			_T("                                   ,'%s'  \n")
			_T("                                   ,'0' \n")
			_T("                                   ,'%s' \n") 
			_T("                                   ,'ACK' \n")
			_T("                                   , ") + m_pDoc->SYSDATE + _T(" \n")
			_T("					               , '%s' )")  , strInsWhTyp, strPltId, strInsCellNo, m_pDoc->m_strId, m_pDoc->m_strId);

		BOOL isIntSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if(isIntSuccess == FALSE)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			bSuccess = true;
			return;
		}	

		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SUCCESS")));
	}

	for(int nIdxRow = 1; nIdxRow < m_pSpreadMain.GetDataRowCnt() + 1; nIdxRow++)
	{
		m_pSpreadMain.SetText(1, nIdxRow, variant_t(_T("0")));
	}	
	m_edtBottmTray.SetWindowText(_T(""));
	InitializeSpread(TRUE);
	return;	
}


void CViewRackDlg::OnBnClickedViewRackCancel()
{
	OnClose();
}
void CViewRackDlg::RenameResource( EN_LANG enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");


	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)enLang);
	SetWindowText(strValue);
	
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cellretrive"), (int)enLang);
	SetDlgItemText(IDC_BTN_CELL_RET, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cellupdate"), (int)enLang);
	SetDlgItemText(IDC_BTN_CELL_USE_DEF, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)enLang);
	SetDlgItemText(IDC_BTN_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)enLang);
	SetDlgItemText(IDC_LBL_CELL_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cellscno"), (int)enLang);
	SetDlgItemText(IDC_LBL_CELL_SCNO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scno"), (int)enLang);
	SetDlgItemText(IDC_LBL_CELL_SC_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("agingtyp"), (int)enLang);
	SetDlgItemText(IDC_LBL_CELL_AGING_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cellsta"), (int)enLang);
	SetDlgItemText(IDC_LBL_CELL_STA, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cellusedef"), (int)enLang);
	SetDlgItemText(IDC_LBL_CELL_USE_DEF, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrtop"), (int)enLang);
	SetDlgItemText(IDC_LBL_PLT_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrbottom"), (int)enLang);
	SetDlgItemText(IDC_LBL_RACK_BCR_BOTTOM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("remark"), (int)enLang);
	SetDlgItemText(IDC_LBL_REMARK, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cellno"), (int)enLang);
	SetDlgItemText(IDC_LBL_CELL_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("result"), (int)enLang);
	SetDlgItemText(IDC_LBL_CELL_MST_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)enLang);
	SetDlgItemText(IDC_LBL_VIEW_CELL_MST, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cellusedef"), (int)enLang);
	SetDlgItemText(IDC_LBL_VIEW_CELL_USE_DEF, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cellsta"), (int)enLang);
	SetDlgItemText(IDC_LBL_CELL_STA2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrtop"), (int)enLang);
	SetDlgItemText(IDC_LBL_RACK_BCR_TOP2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("bcrbottom"), (int)enLang);
	SetDlgItemText(IDC_LBL_RACK_BCR_BOTTOM2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("remark"), (int)enLang);
	SetDlgItemText(IDC_LBL_REMARK3, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("agingtime"), (int)enLang);
	SetDlgItemText(IDC_LBL_AGING_DT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)enLang);
	SetDlgItemText(IDC_GRP_CELL_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)enLang);
	SetDlgItemText(IDC_GRP_CELL_SEARCH2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("update"), (int)enLang);
	SetDlgItemText(IDC_GRP_CELL_EDIT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\"), _T("dlg_cellinfo"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("update"), (int)enLang);
	SetDlgItemText(IDC_GRP_CELL_EDIT2, strValue);
}

void CViewRackDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_cellinfo\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;

	

	m_btnAgingDtFrTo.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnAgingDtFrTo.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath2, _T("arrow-left"), strExtension)), NULL, 5, 5);

	m_btnSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
	m_btnSearch.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnSearch.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnCellNoFrTo.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCellNoFrTo.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath2, _T("arrow-left"), strExtension)), NULL, 5, 5);

	m_btnCellRet.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCellRet.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath2, _T("write"), strExtension)), NULL, 5, 5);
	m_btnCellRet.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnCellRet.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

	m_btnCellUseDefEdit.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnCellUseDefEdit.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath2, _T("write"), strExtension)), NULL, 5, 5);
	m_btnCellUseDefEdit.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnCellUseDefEdit.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);

}



void CViewRackDlg::OnBnClickedBtnCellRet()
{
	UpdateData(TRUE);
	CString CRLF = _T("\r\n");
	CString strSql, strDEST_POS;
	CString strWH_TYP, strSTART_POS, strSTART_LOCATION, strCELL_STA, strBOTTOM_TRAY, strTOP_TRAY = _T("");

	variant_t val;
	m_pSpreadMain.GetText(2, m_nActiveRow, &val);
	strWH_TYP = (LPCTSTR)(_bstr_t)val;
	strWH_TYP.Trim();
	strWH_TYP = m_cmbWhTyp.GetItemCCD(strWH_TYP);

	m_pSpreadMain.GetText(4, m_nActiveRow, &val);
	strSTART_POS = (LPCTSTR)(_bstr_t)val;
	strSTART_POS.Trim();
	strSTART_POS = m_cmbCellScNo.GetItemCCD(strSTART_POS);

	m_pSpreadMain.GetText(5, m_nActiveRow, &val);
	strSTART_LOCATION = (LPCTSTR)(_bstr_t)val;
	strSTART_LOCATION.Trim();

	m_pSpreadMain.GetText(6, m_nActiveRow, &val);
	strBOTTOM_TRAY = (LPCTSTR)(_bstr_t)val;
	strBOTTOM_TRAY.Trim();

	m_pSpreadMain.GetText(7, m_nActiveRow, &val);
	strTOP_TRAY = (LPCTSTR)(_bstr_t)val;
	strTOP_TRAY.Trim();

	m_pSpreadMain.GetText(8, m_nActiveRow, &val);
	strCELL_STA = (LPCTSTR)(_bstr_t)val;
	strCELL_STA.Trim();
	
	strDEST_POS = m_cmbDestPos.GetItemKey(m_cmbDestPos.GetCurSel());

	if ((strDEST_POS == _T("")))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("목적지를 선택해주세요.")));
		return;
	}

	if(AfxMessageBox(m_pDoc->GetMsgLangDef(_T("CELL-출고 작업을 생성하시겠습니까?")), MB_YESNO) == IDYES)
	{
		strSql = _T("");
		strSql += CRLF + _T(" INSERT INTO JOB_MST(WH_TYP, LUGG_NO, START_POS, START_LOCATION, DEST_POS, DEST_LOCATION, JOB_TYP, BCR_TOP, BCR_BOTTOM, JOB_STATUS, INS_DT, INS_USER_ID ) ");
		strSql += CRLF + _T("              VALUES( ") + CLib::Quot(strWH_TYP);
		strSql += CRLF + _T("                     ,(SELECT RIGHT('0000' + CAST(ISNULL(MAX(CAST(LUGG_NO AS INT)), 8999) + 1 AS VARCHAR), 4) FROM JOB_MST WHERE LUGG_NO LIKE '9[0-9][0-9][0-9]') ");	// [LGLS] MANUAL_SEQ -> MAX+1 (SQL2008)
		strSql += CRLF + _T("                     ,") + CLib::Quot(strSTART_POS); 
		strSql += CRLF + _T("                     ,") + CLib::Quot(strSTART_LOCATION); 
		strSql += CRLF + _T("                     ,") + CLib::Quot(strDEST_POS); 
		strSql += CRLF + _T("                     ,") + CLib::Quot(_T("00-000-00")); 
		strSql += CRLF + _T("                     ,") + CLib::Quot(_T("2")); 
		strSql += CRLF + _T("                     ,") + CLib::Quot(strTOP_TRAY); 
		strSql += CRLF + _T("                     ,") + CLib::Quot(strBOTTOM_TRAY); 
		strSql += CRLF + _T("                     ,'20'");
		strSql += CRLF + _T("                     , ") +m_pDoc->SYSDATE+_T("");
		strSql += CRLF + _T("                     , ") + CLib::Quot(m_pDoc->m_strId); 
		strSql += CRLF + _T("					)");

		int nRt = m_pDoc->ExcuteQueryString_DLG(strSql);

		if (nRt < 1)
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		}
		else
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SUCCESS")));
		}
	}

	

	return;

}

CString CViewRackDlg::GetQryInsert(CString strWH_TYP, CString strSC_NO, CString strCELL_NO, CString strDEST_POS, CString strBOTTOM_TRAY, CString strTOP_TRAY)
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	
	strSql += CRLF + _T(" INSERT INTO JOB_MST(  WH_TYP			");
	strSql += CRLF + _T("              , LUGG_NO		");
	strSql += CRLF + _T("			   , START_POS		");
	strSql += CRLF + _T("			   , START_LOCATION ");
	strSql += CRLF + _T("			   , DEST_POS		");
	strSql += CRLF + _T("		       , JOB_TYP		");
	strSql += CRLF + _T("			   , BCR_TOP		");
	strSql += CRLF + _T("			   , BCR_BOTTOM		");
	strSql += CRLF + _T("			   , JOB_STATUS		");
	strSql += CRLF + _T("			   , INS_USER_ID	");
	strSql += CRLF + _T("			   , INS_DT	)		");
	strSql += CRLF + _T("      VALUES ( ") + CLib::Quot(strWH_TYP);
	strSql += CRLF + _T("	           , (SELECT RIGHT('0000' + CAST(ISNULL(MAX(CAST(LUGG_NO AS INT)), 8999) + 1 AS VARCHAR), 4) FROM JOB_MST WHERE LUGG_NO LIKE '9[0-9][0-9][0-9]')");	// [LGLS] MANUAL_SEQ -> MAX+1 (SQL2008)
	strSql += CRLF + _T("			   ,") + CLib::Quot(strSC_NO);
	strSql += CRLF + _T("			   ,") + CLib::Quot(strCELL_NO);
	strSql += CRLF + _T("			   ,") + CLib::Quot(strDEST_POS);
	strSql += CRLF + _T("			   , '2'");
	strSql += CRLF + _T("			   ,") + CLib::Quot(strTOP_TRAY);
	strSql += CRLF + _T("			   ,") + CLib::Quot(strBOTTOM_TRAY);
	strSql += CRLF + _T("			   ,'20'");
	strSql += CRLF + _T("			   , '") + m_pDoc->m_strId + _T("'");
	strSql += CRLF + _T("			   , ") + m_pDoc->SYSDATE + _T(" )");

	return strSql;
}

CString CViewRackDlg::GetQryUpdate(CString strWH_TYP, CString strCELL_SC_NO, CString strCELL_NO, CString strCELL_USE_DEF, CString strUP_CELL_STA, CString strUP_BOTTOM_TRAY, CString strUP_TOP_TRAY, CString strUP_REMARK)
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");

	strSql += CRLF + _T(" UPDATE CELL_MST ");
	strSql += CRLF + _T("    SET CELL_USE_DEF =") + CLib::Quot(strCELL_USE_DEF);
	strSql += CRLF + _T("      , CELL_STA = ") + CLib::Quot(strUP_CELL_STA);
	strSql += CRLF + _T("      , BOTTOM_TRAY = ") + CLib::Quot(strUP_BOTTOM_TRAY);
	strSql += CRLF + _T("      , TOP_TRAY = ") + CLib::Quot(strUP_TOP_TRAY);
	strSql += CRLF + _T("      , UPD_DT = ") + m_pDoc->SYSDATE + _T("");
	strSql += CRLF + _T("      , USER_REMARKS =") + CLib::Quot(m_pDoc->m_strId + _T(" : ") + strUP_REMARK);
	strSql += CRLF + _T("  WHERE WH_TYP =") + CLib::Quot(strWH_TYP);   
	strSql += CRLF + _T("	 AND CELL_NO =") + CLib::Quot(strCELL_NO);
	strSql += CRLF + _T("	 AND CELL_SC_NO =") + CLib::Quot(strCELL_SC_NO);			

 	return strSql;
}

void CViewRackDlg::OnBnClickedBtnCellUseDef()
{
	UpdateData(TRUE);
	
	if (!m_pDoc->Permission(_T("CViewRackDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	if (m_nActiveRow < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("스프레드를 클릭하시오"))); 
		return;
	}

	CString strSql, strTemp, strCellUseDef, strCellScNo = _T("");
	CString strWH_TYP, strLUGG_NO, strSC_NO, strCELL_NO, strCELL_STA, strBOTTOM_TRAY, strTOP_TRAY = _T("");
	CString strUP_CELL_STA, strUP_BOTTOM_TRAY, strUP_TOP_TRAY, strUP_REMARK, strUP_CELL_USE_DEF;
	
	variant_t val;
	
	m_edtTopTray2.GetWindowText(strUP_TOP_TRAY);
	m_edtBottomTray2.GetWindowText(strUP_BOTTOM_TRAY);

	m_pSpreadMain.GetText(2, m_nActiveRow, &val);
	strWH_TYP = (LPCTSTR)(_bstr_t)val;
	strWH_TYP.Trim();
	strWH_TYP = m_cmbWhTyp.GetItemCCD(strWH_TYP);

	m_pSpreadMain.GetText(5, m_nActiveRow, &val);
	strCELL_NO = (LPCTSTR)(_bstr_t)val;

	m_pSpreadMain.GetText(4, m_nActiveRow, &val);
	strCellScNo = (LPCTSTR)(_bstr_t)val;
	strCellScNo.Trim();
	strCellScNo = m_cmbCellScNo.GetItemCCD(strCellScNo);

	m_pSpreadMain.GetText(8, m_nActiveRow, &val);
	strCELL_STA = (LPCTSTR)(_bstr_t)val;

	m_pSpreadMain.GetText(6, m_nActiveRow, &val);
	strBOTTOM_TRAY = (LPCTSTR)(_bstr_t)val;

	m_pSpreadMain.GetText(7, m_nActiveRow, &val);
	strTOP_TRAY = (LPCTSTR)(_bstr_t)val;

	m_pSpreadMain.GetText(9, m_nActiveRow, &val);
	strCellUseDef = (LPCTSTR)(_bstr_t)val;


	strUP_CELL_USE_DEF = m_cmbCellUseDef2.GetItemKey(m_cmbCellUseDef2.GetCurSel());

	strUP_CELL_STA = m_cmbCellSta2.GetItemKey(m_cmbCellSta2.GetCurSel());

	
	strCellScNo.Trim();
	strCELL_NO.Trim();
	strUP_CELL_USE_DEF.Trim();
	strCELL_STA.Trim();
	strBOTTOM_TRAY.Trim();
    strTOP_TRAY.Trim();
    strCellUseDef.Trim();
    strUP_CELL_USE_DEF.Trim();
    strUP_CELL_STA.Trim();
    strUP_TOP_TRAY.Trim();
    strUP_BOTTOM_TRAY.Trim();
    
    if (strBOTTOM_TRAY == _T("")) { strBOTTOM_TRAY = _T("0");}
    if (strTOP_TRAY == _T("")) { strTOP_TRAY = _T("0");}
    if (strUP_BOTTOM_TRAY == _T("")) { strUP_BOTTOM_TRAY = _T("0");}
    if (strUP_TOP_TRAY == _T("")) { strUP_TOP_TRAY = _T("0");}


	m_edtRemarks2.GetWindowText(strUP_REMARK);
	int nCnt = 0;

	//m_cmbCellUseDef2.GetWindowText(strCellUseDef);

	if (strUP_CELL_USE_DEF == _T("-1") || strUP_CELL_USE_DEF == _T(""))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("Cell 사용정의를 선택하세요."))); 
		return;
	}

	CString strSpace = _T(" ");
	if(AfxMessageBox(m_pDoc->GetMsgLangDef(_T("]의 Cell사용정의 수정을 진행하시겠습니까?")) + strSpace+  _T("[Cell_no : ") + strCELL_NO + _T(" Bottom Tray : ") + strUP_BOTTOM_TRAY + _T(" Top Tray : ") + strUP_TOP_TRAY + _T(" ]"), MB_YESNO) == IDYES)
	{
	

		
		
		long bTrans = m_pDoc->BeginTrans_DLG();
		
		if (bTrans < 1)
			return;

		strSql = GetQryUpdate(strWH_TYP, strCellScNo, strCELL_NO, strUP_CELL_USE_DEF, strUP_CELL_STA, strUP_BOTTOM_TRAY, strUP_TOP_TRAY, strUP_REMARK);

		BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);
		if(isSuccess == FALSE)
		{				
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			return;
		}

		// 셀 상태 변경 -> 1 or 0
		if ((strUP_CELL_STA == _T("0")) || (strUP_CELL_STA == _T("1")))
		{
			strSql.Format(_T("SELECT * FROM CELL_DTL				\n")
				_T("				  WHERE WH_TYP = '%s'			\n")
				_T("				    AND CELL_SC_NO = '%s'		\n")
				_T("				    AND CELL_NO = '%s'		\n"), strWH_TYP, strCellScNo, strCELL_NO);

			int nSelCnt = m_pDoc->GetSelectQryCnt_DLG(strSql);

			if (nSelCnt > 0)
			{
				strSql.Format(_T("DELETE FROM CELL_DTL				\n")
					_T("				WHERE WH_TYP = '%s'			\n")
					_T("				  AND CELL_SC_NO = '%s'		\n")
					_T("				  AND CELL_NO = '%s'		\n"), strWH_TYP, strCellScNo, strCELL_NO);

				isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

				if (isSuccess == FALSE)
				{
					m_pDoc->RollbackTrans_DLG();
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
					return;
				}
			}
		}
		else 
		{
			strSql.Format(_T("SELECT * FROM CELL_DTL				\n")
				_T("				  WHERE WH_TYP = '%s'			\n")
				_T("				    AND CELL_SC_NO = '%s'		\n")
				_T("				    AND CELL_NO = '%s'		\n"), strWH_TYP, strCellScNo, strCELL_NO);

			int nSelCnt = m_pDoc->GetSelectQryCnt_DLG(strSql);

			         if (nSelCnt > 0)
         {
            strSql.Format(_T("DELETE FROM CELL_DTL            \n")
               _T("            WHERE WH_TYP = '%s'         \n")
               _T("              AND CELL_SC_NO = '%s'      \n")
               _T("              AND CELL_NO = '%s'      \n"), strWH_TYP, strCellScNo, strCELL_NO);

            isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

            if (isSuccess == FALSE)
            {
               m_pDoc->RollbackTrans_DLG();
               AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
               return;
            }
         }
         

      if (m_pDoc->m_WH_TYP == "40")
      {

         CString strBank = strCELL_NO.Mid(0,2);
         CString strBay  = strCELL_NO.Mid(4,2);
         CString strLevel = strCELL_NO.Mid(7,2);
         CString strInsCellNo = strBank +strBay +strLevel;

         CString strInsWhTyp = m_pDoc->m_WH_TYP;
         CString strUpdBottomTray = strUP_BOTTOM_TRAY;
         CString strPltId = strBOTTOM_TRAY;
      

         strInsWhTyp.Trim();
         strPltId.Trim();
         strUpdBottomTray.Trim();
         strInsCellNo.Trim();

         CString strUP_CELL_USE_DEF_TEXT = _T("");
         m_cmbCellUseDef2.GetWindowText(strUP_CELL_USE_DEF_TEXT);
         //셀 상태 변경
         if (strUP_CELL_USE_DEF_TEXT != strCellUseDef)
         {

            CString strCELLSTATE = _T("");
            strCELLSTATE = m_cmbCellUseDef2.GetItemKey(m_cmbCellUseDef2.GetCurSel());

            if (strCELLSTATE != _T("0")) {   strCELLSTATE = _T("1"); }

            strSql.Format(_T(" INSERT INTO CELL_MES_REQ(  WH_TYP \n")
               _T("                               , MES_CMD  \n")
               _T("                               , MES_RQ  \n")
               _T("                               , STA_POSITIONS \n")
               _T("                                    , STA_STATE  \n")
               _T("                                    , USER_ID  \n")
               _T("                                    , MES_ACK  \n")
               _T("                                    , INS_DT  \n")
               _T("                                    , INS_USER_ID ) \n")
               _T("                            VALUES ('%s' \n")
               _T("                                   ,'S6F11_CEID21' \n") 
               _T("                                   ,'RQ' \n")
               _T("                                   ,'%s'  \n")
               _T("                                   ,'%s' \n")
               _T("                                   ,'%s' \n") 
               _T("                                   ,'ACK' \n")
               _T("                                   , ") + m_pDoc->SYSDATE + _T(" \n")
               _T("                              , '%s' )")  , strInsWhTyp, strInsCellNo, strCELLSTATE, m_pDoc->m_strId, m_pDoc->m_strId);

            BOOL isIntSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

            if(isIntSuccess == FALSE)
            {
               m_pDoc->RollbackTrans_DLG();
               AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
               return;
            }   
         }

         //팔레트 id 변경
         if ((strPltId != strUP_BOTTOM_TRAY) && (strUP_BOTTOM_TRAY != _T("0")))
         {
            strSql.Format(_T(" INSERT INTO CELL_MES_REQ(  WH_TYP \n")
               _T("                               , MES_CMD  \n")
               _T("                               , MES_RQ  \n")
               _T("                           , PLT_ID  \n")
               _T("                           , PLT_ID_NEW  \n")
               _T("                               , STA_POSITIONS \n")
               _T("                                    , STA_STATE  \n")
               _T("                                    , USER_ID  \n")
               _T("                                    , MES_ACK  \n")
               _T("                                    , INS_DT  \n")
               _T("                                    , INS_USER_ID ) \n")
               _T("                            VALUES ('%s' \n")
               _T("                                   ,'S6F11_CEID13' \n") 
               _T("                                   ,'RQ' \n")
               _T("                                   ,'%s'  \n")
               _T("                                   ,'%s'  \n")
               _T("                                   ,'%s'  \n")
               _T("                                   ,'0' \n")
               _T("                                   ,'%s' \n") 
               _T("                                   ,'ACK' \n")
               _T("                                   , ") + m_pDoc->SYSDATE + _T(" \n")
               _T("                              , '%s' )")  , strInsWhTyp, strPltId, strUpdBottomTray, strInsCellNo, m_pDoc->m_strId, m_pDoc->m_strId);
            BOOL isIntSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

            if(isIntSuccess == FALSE)
            {
               m_pDoc->RollbackTrans_DLG();
               AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
               return;
            }   
         }

         //팔레트id 삭제 
         if ((strPltId != strUP_BOTTOM_TRAY) && (strUP_BOTTOM_TRAY == _T("0")))
         {
            strSql.Format(_T(" INSERT INTO CELL_MES_REQ(  WH_TYP \n")
               _T("                               , MES_CMD  \n")
               _T("                               , MES_RQ  \n")
               _T("                           , PLT_ID  \n")
               _T("                               , STA_POSITIONS \n")
               _T("                                    , STA_STATE  \n")
               _T("                                    , USER_ID  \n")
               _T("                                    , MES_ACK  \n")
               _T("                                    , INS_DT  \n")
               _T("                                    , INS_USER_ID ) \n")
               _T("                            VALUES ('%s' \n")
               _T("                                   ,'S6F11_CEID14' \n") 
               _T("                                   ,'RQ' \n")
               _T("                                   ,'%s'  \n")
               _T("                                   ,'%s'  \n")
               _T("                                   ,'0' \n")
               _T("                                   ,'%s' \n") 
               _T("                                   ,'ACK' \n")
               _T("                                   , ") + m_pDoc->SYSDATE + _T(" \n")
               _T("                              , '%s' )")  , strInsWhTyp, strPltId, strInsCellNo, m_pDoc->m_strId, m_pDoc->m_strId);
            BOOL isIntSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

            if(isIntSuccess == FALSE)
            {
               m_pDoc->RollbackTrans_DLG();
               AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
               return;
            }   
         }
      }
	  }
	  
		CString strLOG_LUGG_NO = _T("");
		if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
		CString strLOG_BOTTOM_TRAY = strUP_BOTTOM_TRAY;
		CString strLOG_TOP_TRAY = strUP_TOP_TRAY;
		CString strLOG_MSG = _T("CELL_MST UPDATE : BOTTOM_TRAY : ") + strBOTTOM_TRAY + _T(" -> ") + strUP_BOTTOM_TRAY
							+ _T(" , TOP_TRAY : ") + strTOP_TRAY +  _T(" -> ") + strUP_TOP_TRAY + _T(" , CELL_STA : ") + strCELL_STA +  _T(" -> ") + strUP_CELL_STA
							+ _T(" , CELL_USE_DEF : ") + strCellUseDef +  _T(" -> ") + strUP_CELL_USE_DEF;
		if (!m_pDoc->GetQueryInsertClientLog(_T("CViewRackDlg"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
			return;
		}


		m_pDoc->CommitTrans_DLG();
		

		
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SUCCESS")));
		
		InitializeSpread(TRUE);

		return;
	}
}

void CViewRackDlg::OnSetfocusCmbCellScNo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	SetFontNation((int)pEn);

	CLib::GetComBoBoxData(m_cmbCellScNo, m_pCellScNo, 10);
	CLib::BindCombo_SC_HS_DEF_DEST(m_cmbDestPos, m_pDoc, (int)pEn, m_pCellScNo);

}

void CViewRackDlg::OnSetfocusCmbScNo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	SetFontNation((int)pEn);
	CLib::GetComBoBoxData(m_cmbAgingTyp, m_pAging, 10);
	//CLib::GetComBoBoxData(m_cmbScNo, m_pScNo, 10);
	CLib::BindCombo_CELL_SC_NO(m_cmbCellScNo, _T("CELL_SC_NO"), m_pDoc, (int)pEn, m_pAging, m_pScNo);
	OnSetfocusCmbCellScNo();
}

void CViewRackDlg::OnSetfocusCmbAingTyp()
{
	
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	SetFontNation((int)pEn);
	CString strAging = m_cmbAgingTyp.GetItemKey(m_cmbAgingTyp.GetCurSel());
	//CLib::GetComBoBoxData(m_cmbAgingTyp, m_pAging, 10);
	//CLib::GetComBoBoxData(m_cmbScNo, m_pScNo, 10);
	//CLib::BindCombo_SC_NO(m_cmbScNo, _T("SC_NO"), m_pDoc, (int)pEn, m_pAging);
// 	OnSetfocusCmbScNo();
	CLib::BindCombo_CELL_SC_NO(m_cmbCellScNo, _T("CELL_SC_NO"), m_pDoc, (int)pEn, strAging, m_pScNo);
 	OnSetfocusCmbCellScNo();
}

HRESULT CViewRackDlg::accHitTest(long xLeft, long yTop, VARIANT *pvarChild)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return __super::accHitTest(xLeft, yTop, pvarChild);
}
BEGIN_EVENTSINK_MAP(CViewRackDlg, CSkinDialog)
	ON_EVENT(CViewRackDlg, IDC_CELL_MST, 2, CViewRackDlg::BlockSelectedCellMst, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CViewRackDlg, IDC_CELL_MST, 5, CViewRackDlg::ClickSpread, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CViewRackDlg::BlockSelectedCellMst(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) // 블록설정
{
	int a = 0;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CViewRackDlg::ClickSpread(long Col, long Row) //셀클릭
{
	variant_t val;
	m_nActiveRow = Row;
	m_pSpreadMain.GetText(12, m_nActiveRow, &val);
	CString strRemarks = (LPCTSTR)(_bstr_t)val;
	strRemarks.Trim();
	m_edtRemarks2.SetWindowText(strRemarks);

	m_pSpreadMain.GetText(9, m_nActiveRow, &val);
	CString strCellUseDef = (LPCTSTR)(_bstr_t)val;
	strCellUseDef.Trim();
	m_cmbCellUseDef2.SetCurSelTextEx(strCellUseDef);

	m_pSpreadMain.GetText(8, m_nActiveRow, &val);
	CString strCellSta = (LPCTSTR)(_bstr_t)val;
	strCellSta.Trim();
	m_cmbCellSta2.SetCurSelTextEx(strCellSta);

	m_pSpreadMain.GetText(7, m_nActiveRow, &val);
	CString strTopTray = (LPCTSTR)(_bstr_t)val;
	strTopTray.Trim();
	m_edtTopTray2.SetWindowText(strTopTray);

	m_pSpreadMain.GetText(6, m_nActiveRow, &val);
	CString strBottomTray = (LPCTSTR)(_bstr_t)val;
	strBottomTray.Trim();
	m_edtBottomTray2.SetWindowText(strBottomTray);

	m_pSpreadMain.GetText(4, m_nActiveRow, &val);
	CString strCELL_SC_NO = (LPCTSTR)(_bstr_t)val;
	strCELL_SC_NO.Trim();
	CLib::BindCombo_SC_HS_DEF_DEST_SPREAD(m_cmbDestPos, m_pDoc, (int)m_pDoc->m_enLang, strCELL_SC_NO);
}










//////////////SPREAD 값 채우기

CString CViewRackDlg::GetQrySelect_Main(BOOL bSearch)
{
	UpdateData(TRUE);
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	CString strWH_TYP, strAGING_TYP, strCELL_SC_NO, strCELL_STA, strCELL_USE_DEF, strCELL_NO_FROM, strCELL_NO_TO, strBOTTOM_TRAY, strTOP_TRAY, strREMARK = _T("ALL");
	CString strFromBank, strFromBay, strFromLev, strToBank, strToBay, strToLev;
	CTime tDATE_FROM, tDATE_TO, tTIME_FROM, tTIME_TO;
	CString strdtFrom, strdtTo = _T("ALL");
	m_MaskEditCellNoFr.GetWindowText(strCELL_NO_FROM);
	m_MaskEditCellNoTo.GetWindowText(strCELL_NO_TO);
	CLib::CellMasking(strCELL_NO_FROM);
	CLib::CellMasking(strCELL_NO_TO);
	m_edtTopTray.GetWindowText(strTOP_TRAY);
	m_edtBottmTray.GetWindowText(strBOTTOM_TRAY);
	m_edtRemarks.GetWindowText(strREMARK);

	strWH_TYP = m_cmbWhTyp.GetItemKey(m_cmbWhTyp.GetCurSel());
	strAGING_TYP = m_cmbAgingTyp.GetItemKey(m_cmbAgingTyp.GetCurSel());
	strCELL_SC_NO = m_cmbCellScNo.GetItemKey(m_cmbCellScNo.GetCurSel());
	strCELL_STA = m_cmbCellSta.GetItemKey(m_cmbCellSta.GetCurSel());
	strCELL_USE_DEF = m_cmbCellUseDef.GetItemKey(m_cmbCellUseDef.GetCurSel());
	m_dtAgingStartD.GetTime(tDATE_FROM);
	m_dtAgingStartT.GetTime(tTIME_FROM);
	m_dtAgingEndD.GetTime(tDATE_TO);
	m_dtAgingEndT.GetTime(tTIME_TO);

	strWH_TYP.Trim();
	strAGING_TYP.Trim();
	strCELL_SC_NO.Trim();
	strCELL_STA.Trim();
	strCELL_USE_DEF.Trim();
	strTOP_TRAY.Trim();
	strBOTTOM_TRAY.Trim();

	strdtFrom = CLib::ConvertCTimeToOracleDateTimeString(tDATE_FROM, tTIME_FROM);
	strdtTo = CLib::ConvertCTimeToOracleDateTimeString(tDATE_TO, tTIME_TO);

	strFromBank	= strCELL_NO_FROM.Left(2);
	strFromBay	= strCELL_NO_FROM.Mid(3,3);
	strFromLev	= strCELL_NO_FROM.Right(2);
	strToBank	= strCELL_NO_TO.Left(2);
	strToBay	= strCELL_NO_TO.Mid(3,3);
	strToLev	= strCELL_NO_TO.Right(2);

	strSql += CRLF + _T("  SELECT ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, ' ') AS WH_TYP	");
	strSql += CRLF + _T("       , ") + m_pDoc->NVL + _T("(CCD_AGING_TYP.CCD_NM_KOR, '290') AS AGING_TYP	");
	strSql += CRLF + _T("       , ") + m_pDoc->NVL + _T("(CCD_CELL_SC_NO.CCD_NM_KOR, '290') AS CELL_SC_NO	");
	strSql += CRLF + _T("       , ") + m_pDoc->NVL + _T("(CM.CELL_NO, '00-000-00') AS CELL_NO	");
	strSql += CRLF + _T("       , ") + m_pDoc->NVL + _T("(CM.BOTTOM_TRAY, ' ') AS BOTTOM_TRAY	");
	strSql += CRLF + _T("       , ") + m_pDoc->NVL + _T("(CM.TOP_TRAY, ' ') AS TOP_TRAY	");
	strSql += CRLF + _T("       , ") + m_pDoc->NVL + _T("(CCD_CELL_STATUS.CCD_NM_KOR, '99') AS CELL_STATUS	");
	strSql += CRLF + _T("       , ") + m_pDoc->NVL + _T("(CCD_CELL_USE_DEF.CCD_NM_KOR, '99') AS CELL_USE_DEF	");
	strSql += CRLF + _T("       , ") + m_pDoc->NVL + _T("(CCD_FIRE_OD_RQ_YN.CCD_NM_KOR, 'N') AS FIRE_OD_RQ_YN	");
	strSql += CRLF + _T("       , ") + m_pDoc->NVL + _T("(CM.REMARKS, ' ') AS REMARKS	");
	strSql += CRLF + _T("       , ") + m_pDoc->NVL + _T("(CM.USER_REMARKS, ' ') AS USER_REMARKS	");
	strSql += CRLF + _T("	FROM CELL_MST CM LEFT OUTER JOIN CELL_DTL CDTL");
	strSql += CRLF + _T("                                 ON CM.WH_TYP = CDTL.WH_TYP ");
	strSql += CRLF + _T("                                AND CM.CELL_SC_NO = CDTL.CELL_SC_NO ");
	strSql += CRLF + _T("                                AND CM.CELL_NO = CDTL.CELL_NO ");
	strSql += CRLF + _T("								 AND CDTL.AGING_START_DT >= CONVERT(DATETIME, STUFF(STUFF(STUFF('") + strdtFrom + _T("',13,0,':'),11,0,':'),9,0,' '))");	// [LGLS]
	strSql += CRLF + _T("								 AND CDTL.AGING_END_DT <=  CONVERT(DATETIME, STUFF(STUFF(STUFF('") + strdtTo + _T("',13,0,':'),11,0,':'),9,0,' '))");	// [LGLS]
	strSql += CRLF + _T("                    LEFT OUTER JOIN COMMON_CODE CCD_AGING_TYP ");
	strSql += CRLF + _T("                                 ON CCD_AGING_TYP.WH_TYP LIKE '%' + CM.WH_TYP + '%' ");	// [LGLS] || -> +
	strSql += CRLF + _T("                                AND CCD_AGING_TYP.CDX_CD = 'AGING_TYP' ");
	strSql += CRLF + _T("                                AND CCD_AGING_TYP.CCD_CD = SUBSTRING(CM.AGING_TYP,1,3) ");	// [LGLS] SUBSTR(x,0,3) -> SUBSTRING(x,1,3)
	strSql += CRLF + _T("                    LEFT OUTER JOIN COMMON_CODE CCD_CELL_STATUS ");
	strSql += CRLF + _T("                                 ON CCD_CELL_STATUS.WH_TYP LIKE '%' + CM.WH_TYP + '%' ");	// [LGLS] || -> +
	strSql += CRLF + _T("                                AND CCD_CELL_STATUS.CDX_CD = 'CELL_STA' ");
	strSql += CRLF + _T("                                AND CCD_CELL_STATUS.CCD_CD = CM.CELL_STA ");
	strSql += CRLF + _T("                    LEFT OUTER JOIN COMMON_CODE CCD_CELL_USE_DEF ");
	strSql += CRLF + _T("                                 ON CCD_CELL_USE_DEF.WH_TYP LIKE '%' + CM.WH_TYP + '%' ");	// [LGLS] || -> +
	strSql += CRLF + _T("                                AND CCD_CELL_USE_DEF.CDX_CD = 'CELL_USE_DEF' ");
	strSql += CRLF + _T("                                AND CCD_CELL_USE_DEF.CCD_CD = CM.CELL_USE_DEF ");
	strSql += CRLF + _T("                    LEFT OUTER JOIN COMMON_CODE CCD_FIRE_OD_RQ_YN ");
	strSql += CRLF + _T("                                 ON CCD_FIRE_OD_RQ_YN.WH_TYP LIKE '%' + CM.WH_TYP + '%' ");	// [LGLS] || -> +
	strSql += CRLF + _T("                                AND CCD_FIRE_OD_RQ_YN.CDX_CD = 'FIRE_OD_RQ_YN' ");
	strSql += CRLF + _T("                                AND CCD_FIRE_OD_RQ_YN.CCD_CD = CM.FIRE_OD_RQ_YN ");
// 	strSql += CRLF + _T("                    LEFT OUTER JOIN COMMON_CODE CCD_CELL_TYP ");
// 	strSql += CRLF + _T("                                 ON CCD_CELL_TYP.WH_TYP LIKE '%' || CM.WH_TYP || '%' ");
// 	strSql += CRLF + _T("                                AND CCD_CELL_TYP.CDX_CD = 'CELL_TYP' ");
// 	strSql += CRLF + _T("                                AND CCD_CELL_TYP.CCD_CD = CM.CELL_TYP ");
	strSql += CRLF + _T("                    LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP ");
	strSql += CRLF + _T("                                 ON CCD_WH_TYP.WH_TYP LIKE '%' + CM.WH_TYP + '%' ");	// [LGLS] || -> +
	strSql += CRLF + _T("                                AND CCD_WH_TYP.CDX_CD = 'WH_TYP' ");
	strSql += CRLF + _T("                                AND CCD_WH_TYP.CCD_CD = CM.WH_TYP ");
	strSql += CRLF + _T("                    LEFT OUTER JOIN COMMON_CODE CCD_CELL_SC_NO ");
	strSql += CRLF + _T("                                 ON CCD_CELL_SC_NO.WH_TYP LIKE '%' + CM.WH_TYP + '%' ");	// [LGLS] || -> +
	strSql += CRLF + _T("                                AND CCD_CELL_SC_NO.CDX_CD = 'CELL_SC_NO' ");
	strSql += CRLF + _T("                                AND CCD_CELL_SC_NO.CCD_CD = CM.CELL_SC_NO ");
	strSql += CRLF + _T("   WHERE 1 = 1 ");
	if(strWH_TYP != _T("") && strWH_TYP != _T("ALL") && strWH_TYP != _T("0"))
	{
		strSql += CRLF + _T("     AND CM.WH_TYP = ") + CLib::Quot(strWH_TYP);
	}

	strSql += CRLF + _T("	  AND CM.BANK BETWEEN '") + strFromBank + _T("' AND '")  + strToBank + _T("'");
	strSql += CRLF + _T("	  AND CM.BAY BETWEEN '") + strFromBay + _T("' AND '")  + strToBay + _T("'");
	strSql += CRLF + _T("	  AND CM.LEV BETWEEN '") + strFromLev + _T("' AND '")  + strToLev + _T("'");
	
	if(strAGING_TYP != _T("") && strAGING_TYP != _T("ALL") && strAGING_TYP != _T("0"))
	{
		strSql+= CRLF + _T("     AND CM.AGING_TYP LIKE ") + CLib::QuotLikeLR(strAGING_TYP);
	}
	if(strCELL_SC_NO != _T("") && strCELL_SC_NO != _T("ALL") && strAGING_TYP != _T("0"))
	{
		strSql+= CRLF + _T("     AND CM.CELL_SC_NO = ") + CLib::Quot(strCELL_SC_NO);
	}
	if(strCELL_STA != _T("") && strCELL_STA != _T("ALL") && strAGING_TYP != _T("0"))
	{
		strSql+= CRLF + _T("     AND CM.CELL_STA = ") + CLib::Quot(strCELL_STA);
	}
	if(strCELL_USE_DEF != _T("") && strCELL_USE_DEF != _T("ALL") && strAGING_TYP != _T("0"))
	{
		strSql+= CRLF + _T("     AND CM.CELL_USE_DEF = ") + CLib::Quot(strCELL_USE_DEF);
	}
	if(strTOP_TRAY != _T("") && strTOP_TRAY != _T("ALL") && strAGING_TYP != _T("0"))
	{
		strSql+= CRLF + _T("     AND CM.TOP_TRAY = ") + CLib::Quot(strTOP_TRAY);
	}
	if(strBOTTOM_TRAY != _T("") && strBOTTOM_TRAY != _T("ALL") && strAGING_TYP != _T("0"))
	{
		strSql+= CRLF + _T("     AND CM.BOTTOM_TRAY = ") + CLib::Quot(strBOTTOM_TRAY);
	}
	if(strREMARK != _T("") && strREMARK != _T("ALL") && strREMARK != _T("0"))
	{
		strSql+= CRLF + _T("     AND REMARKS = ") + CLib::Quot(strREMARK);
	}
	if(strREMARK != _T("") && strREMARK != _T("ALL") && strREMARK != _T("0"))
	{
		strSql+= CRLF + _T("     AND REMARKS = ") + CLib::Quot(strREMARK);
	}
 	strSql += CRLF + _T("   ORDER BY CM.WH_TYP, CM.AGING_TYP, CM.CELL_NO \n");	
	
 	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}



int CViewRackDlg::SetSpeadData(BOOL bSearch = FALSE)
{
	CStringList strList;
	CStringArray arrColName;
	CStringArray arrColSize;
	int nLANG = m_pDoc->m_enLang;
	int nColSize = -1;
	int nRowCnt = -1;
	CString strMessage = _T("");

	CString strSql = GetQrySelect_Main(bSearch);

	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp); 

	if(nRowCnt < 0)
	{
		if(pRsw != NULL)
		{
			delete pRsw;	
		}
	}
	else
	{	
		SetHeadColumn_VerChk(arrColName, arrColSize, nColSize, strSql);
		SetMaxRows(nRowCnt);

		pRsw->MoveFirst();

		for( int nIdxRow = 1; nIdxRow <= nRowCnt; nIdxRow++ )
		{
			for (int nIdxCol = 1; nIdxCol < nColSize; nIdxCol++)
			{
				//첫번째 컬럼 체크 박스면 nInxCol = 1 부터
				CString strColValue = pRsw->GetItem(arrColName[nIdxCol]);
				int nPreSize = CConvert::ToInt(arrColSize[nIdxCol]);
				if ((strColValue.GetLength()*1.5 > nPreSize) && (nPreSize != 0))
				{
					int nSize = strColValue.GetLength()*1.5;
					arrColSize[nIdxCol]= CConvert::ToString(nSize);
				}
				SetColumnText_VerChk(nIdxCol, nIdxRow, pRsw->GetItem(arrColName[nIdxCol]));
			}
			pRsw->MoveNext();
		}

		if (pRsw != NULL)
		{
			delete pRsw;
		}

		for (int nIdxCol = 0; nIdxCol < nColSize; nIdxCol++)
		{
			SetColWidth(nIdxCol,CConvert::ToInt(arrColSize[nIdxCol]));
		}
	}
	return nRowCnt;
}


void CViewRackDlg::SetHeadColumn_VerChk(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
{	
	pStrArrColName.Add(_T(""));
	CLib::GetColumnName(pStrArrColName, pStrSql);
	
	nColIdx = pStrArrColName.GetCount();
	SetMaxCols(nColIdx);
	
	pStrArrColSize.Add(_T("2"));
	
	CStringArray arrColRename;
	//arrColRename.Add(_T(""));
	CLib::RenameColumn(pStrArrColName, m_pDoc->m_enLang, arrColRename);

	SetColumnText_VerChk(0, 0, _T(""));

	for(int nIdxCol = 1; nIdxCol < nColIdx; nIdxCol++)
	{
		int nSize = arrColRename[nIdxCol].GetLength()*2;
		if (arrColRename[nIdxCol].Right(5) == _T("_HIDE"))
		{
			nSize = 0;
		}
		pStrArrColSize.Add(CConvert::ToString(nSize));
	}

	for(int nIdxCol = 1; nIdxCol < nColIdx; nIdxCol++)
	{
		CString asd = arrColRename[nIdxCol];
		SetColumnText_VerChk(nIdxCol, 0, arrColRename[nIdxCol]);
	}

}

void CViewRackDlg::SetColumnText_VerChk(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == "")
		strColumnName = _T(" ");
	strColumnName += _T("     "); 
		
	m_pSpreadMain.SetText( nColIdx + 1, nRowIdx, variant_t(strColumnName));
}


void CViewRackDlg::SetMaxRows(int pRowCnt)
{
	m_pSpreadMain.SetMaxRows(pRowCnt);
}

void CViewRackDlg::SetColWidth(int pColCnt, int pColSize)
{
	m_pSpreadMain.put_ColWidth(pColCnt+1,pColSize);
}

void CViewRackDlg::SetMaxCols(int pMaxCol)
{
	m_pSpreadMain.SetMaxCols(pMaxCol);
}

void CViewRackDlg::InitializeSpread(BOOL bSearch = FALSE)
{
	//정렬 가능
	m_pSpreadMain.SetUserColAction(1);

	m_pSpreadMain.ClearRange(1, 1, -1, -1, TRUE);
	int nRowCnt = SetSpeadData(bSearch);
	m_lblSpdMainCnt.SetWindowText(CConvert::ToString(nRowCnt));
	//m_lblSpdMainCnt
}
