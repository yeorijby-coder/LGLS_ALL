// ScSuspendDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "EqpSuspendDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"

// CScSuspendDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEqpSuspendDlg, CSkinDialog)

CEqpSuspendDlg::CEqpSuspendDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CEqpSuspendDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_nActiveRow = -1;
}

CEqpSuspendDlg::CEqpSuspendDlg(CEcsDoc* pDoc, CWnd* pParent )
	: CSkinDialog(CEqpSuspendDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
}


CEqpSuspendDlg::~CEqpSuspendDlg()
{
	m_pDoc->m_pEqpSuspendDlg = NULL;
	this->DestroyWindow();
}

void CEqpSuspendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EQP_SUSPEND,					m_pSpEQPSUSPEND);

	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_WH_TYP,		m_lblEqpSuspendWhTyp);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_EQP_TYP,		m_lblEqpSuspendEqpTyp);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_USE_YN,		m_lblEqpSuspendUseYn);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_CONNECTED_YN,  m_lblEqpSuspnedConnectedYn);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_1,				m_lblEqpSuspend);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_RESULT,		m_lblEqpSuspendResult);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND_CNT,			m_lblEqpSuspendCnt);

	DDX_Control(pDX, IDC_LBL_PLC_IP_2,					m_lblEqpPLC_IP);
	DDX_Control(pDX, IDC_LBL_PLC_PORT_FROM_2,			m_lblEqpPLC_PORT_FROM);
	DDX_Control(pDX, IDC_LBL_PLC_TO_2,					m_lblEqpPLC_PORT_TO);
	DDX_Control(pDX, IDC_LBL_USE_YN_2,					m_lblEqpUSE_YN);
	DDX_Control(pDX, IDC_LBL_EQP_SUSPEND2_3,			m_lblEqpSUSPEND);


	DDX_Control(pDX, IDC_EDIT_PLC_IP_2,					m_edtPLC_IP);
	
	DDX_Control(pDX, IDC_EDIT_PLC_PORT_FROM_2,			m_edtPLC_PORT_FROM);
	DDX_Control(pDX, IDC_EDIT_PLC_PORT_TO_2,			m_edtPLC_PORT_TO);

	DDX_Control(pDX, IDC_CBX_USE_YN_2,					m_cbxUSE_YN);
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND2_2,			m_cbxEQP_SUSPEND);
	
	//
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND_WH_TYP,		m_cbxEqpSuspendWhTyp);
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND_USE_YN,		m_cbxEqpSuspendUseYn);
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND_EQP_TYP,		m_cbxEqpSuspendEqpTyp);
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND_CONNECTED,		m_cbxEqpSusepndConnectedYn);
	// [LGLS 2026-08-05] 구 ID IDC_CBX_EQP_SUSPEND_EQP_SUSPEND 는 템플릿에 없다.
	//   같은 용도의 콤보는 IDC_CBX_EQP_SUSPEND(숨김 update 그룹) 이므로 이쪽으로 연결한다.
	DDX_Control(pDX, IDC_CBX_EQP_SUSPEND,   m_cbxEqpSuspendEqpSuspend);
	//
	DDX_Control(pDX, IDC_GRP_EQP_EQP_STATE,				m_grpEqpEqpState);
	DDX_Control(pDX, IDC_GRP_EQP_SUSPEND,				m_grpEqpSuspend);
	DDX_Control(pDX, IDC_GRP_EQP_SUSPEND2,				m_grpEqpSuspend2); 
	//
	DDX_Control(pDX, ID_BTN_EQP_SUSPEND_SUSPEND,		m_btnEqpSuspend);
	DDX_Control(pDX, ID_BTN_EQP_SUSPEND_SEARCH,			m_btnEqpSuspendSearch);

}


BEGIN_MESSAGE_MAP(CEqpSuspendDlg, CSkinDialog)
	ON_MESSAGE(SSM_CLICK, &CEqpSuspendDlg::OnLglsHdrSort)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BTN_EQP_SUSPEND_SEARCH, &CEqpSuspendDlg::OnBnClickedBtnEqpSuspendSearch)
	ON_BN_CLICKED(ID_BTN_EQP_SUSPEND_SUSPEND, &CEqpSuspendDlg::OnBnClickedBtnEqpSuspendSuspend)
	ON_BN_CLICKED(IDC_EQP_SUSPEND_CHECK_ALL, &CEqpSuspendDlg::OnBnClickedEqpSuspendCheckAll)
	ON_BN_CLICKED(IDC_CHK_SC_ACTIVE, &CEqpSuspendDlg::OnBnClickedChkScActive)
	ON_BN_CLICKED(IDC_CHK_SC_STO_SUSPEND, &CEqpSuspendDlg::OnBnClickedChkScStoSuspend)
	ON_BN_CLICKED(IDC_CHK_SC_RET_SUSPEND, &CEqpSuspendDlg::OnBnClickedChkRetSuspend)
	ON_BN_CLICKED(IDC_CHK_SC_ALL_SUSPEND, &CEqpSuspendDlg::OnBnClickedChkScAllSuspend)
	ON_BN_CLICKED(IDC_CHK_OTHER_EQP_ACTIVE, &CEqpSuspendDlg::OnBnClickedChkOtherEqpActive)
	ON_BN_CLICKED(IDC_CHK_OTHER_SUSPEND, &CEqpSuspendDlg::OnBnClickedChkOtherSuspend)
	ON_CBN_SELCHANGE(IDC_CBX_EQP_SUSPEND_EQP_TYP, &CEqpSuspendDlg::OnCbnSelchangeCbxEqpSuspendEqpTyp)
END_MESSAGE_MAP()

// [LGLS] 그리드 헤더 클릭 정렬
LRESULT CEqpSuspendDlg::OnLglsHdrSort(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if(pCell != NULL && pCell->Row == 0 && pCell->Col >= 1)
		m_SpreadSheet.SortByHeaderColumn(pCell->Col);
	return 0;
}


// CScSuspendDlg 메시지 처리기입니다.

BOOL CEqpSuspendDlg::OnInitDialog()
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

	CLib::BindCombo(m_cbxEqpSuspendWhTyp, _T("WH_TYP"), m_pDoc, (int)pEn);
	CLib::BindCombo(m_cbxEqpSuspendEqpTyp, _T("EQP_TYP"), m_pDoc, (int)pEn, TRUE);
	// [LGLS 2026-09-03] 설비 구분은 ALL / EQP / HOST / SCH 만 표시(사용자 요청).
	//   공통코드(EQP_TYP)에 SCH 가 없어 콤보에 빠지던 것을 보강하고, HOST 선택 시 HOST2 도 함께 조회한다.
	{
		CString strAllTxt = _T("ALL");
		int nAll = m_cbxEqpSuspendEqpTyp.GetCount();
		for (int i = 0; i < nAll; i++)
		{
			CString strKey = m_cbxEqpSuspendEqpTyp.GetItemKey(i); strKey.Trim();
			if (strKey == _T("ALL")) { m_cbxEqpSuspendEqpTyp.GetLBText(i, strAllTxt); break; }
		}
		m_cbxEqpSuspendEqpTyp.ResetContent();
		m_cbxEqpSuspendEqpTyp.m_Key.RemoveAll();   // 인덱스 기반 키 맵도 비운다
		const TCHAR* szKeys[] = { _T("ALL"), _T("EQP"), _T("HOST"), _T("SCH") };
		const TCHAR* szTxts[] = { NULL,      _T("EQP"), _T("HOST"), _T("SCH") };
		for (int j = 0; j < 4; j++)
		{
			// 정렬(CBS_SORT) 콤보는 AddString 이 끼워 넣는 위치가 달라지므로 실제 인덱스에 키를 붙인다
			int nIdx = m_cbxEqpSuspendEqpTyp.AddString(szTxts[j] != NULL ? szTxts[j] : (LPCTSTR)strAllTxt);
			if (nIdx >= 0) m_cbxEqpSuspendEqpTyp.SetItemDataEx(nIdx, szKeys[j]);
		}
		m_cbxEqpSuspendEqpTyp.SetCurSel(0);
	}
	CLib::BindCombo(m_cbxEqpSusepndConnectedYn, _T("CONNECTED_YN"), m_pDoc, (int)pEn, TRUE);
	
	#pragma region 스프레드 초기화 
	//IDC_CUSTOM1 라는 컨트롤에서 Rect 값을 가져와서 스프레드에 적용 
	// 상대 좌표 구하기
	CRect rect;							// 윈도우 기준 절대 좌표 
	GetDlgItem(IDC_STATIC_SPREAD)->GetWindowRect(&rect);

	CRect rectTemp = rect;				// 다이얼 로그 영역안의 컨트롤(rect)의 상대좌표 
	ScreenToClient(&rectTemp);
	 
	CRect rtTemp;
	::GetWindowRect(this->m_hWnd, &rtTemp);
	
	CRect rectMargin = CRect(rectTemp.left, rectTemp.top, rtTemp.right-rect.right, rtTemp.bottom - rect.bottom);

//	m_SpreadSheet.m_rectWnd = rectTemp;//CRect(0, 0, 0, 0);
	m_SpreadSheet.m_nSorting = SS_USERCOLACTION_DEFAULT;
	m_SpreadSheet.m_bAutoResizeCol = true;
	m_SpreadSheet.m_colorBaseBack = WHITE;
	m_SpreadSheet.m_wGridType = SS_GRID_HORIZONTAL | SS_GRID_VERTICAL | SS_GRID_SOLID;

	m_SpreadSheet.m_rectMargin = rectMargin;

	HFONT hFontH = CreateFont(15,0,0,0,700,0,0,0,0,0,0,0,0,_T("System"));
	HFONT hFontB = CreateFont(15,0,0,0,FW_BOLD,0,0,0,0,0,0,0,0,_T("System"));

	m_SpreadSheet.AddSheet(_T(""), this, hFontH, hFontB);
	//m_SpreadSheet.AddColHead(_T(" "), 10);
	m_SpreadSheet.AddColHead(_T("창고 타입"), 10);
	m_SpreadSheet.AddColHead(_T("설비 종류"), 10);
	m_SpreadSheet.AddColHead(_T("설비명"), 10);
	m_SpreadSheet.AddColHead(_T("설비 IP"), 15);
	m_SpreadSheet.AddColHead(_T("사용 포트"), 10);

	BOOL bResult = m_SpreadSheet.Create();

	if (bResult == FALSE)
		AfxMessageBox(_T("생성못함!"));

#pragma endregion

	OnBnClickedBtnEqpSuspendSearch();
	Invalidate(TRUE);
	return TRUE;
}

void CEqpSuspendDlg::InitializeControlLanguage()
{
	//m_lblScWhTyp.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("창고구분"), m_pDoc->m_enLang));
	//m_lblScSuspend.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("S/C상태"), m_pDoc->m_enLang));
	//
	//m_btnSearch.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("조회"), m_pDoc->m_enLang));
	//
	//m_btnOk.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("확인"), m_pDoc->m_enLang));
	//m_btnCancel.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("취소"), m_pDoc->m_enLang));
}


void CEqpSuspendDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}

HCURSOR CEqpSuspendDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEqpSuspendDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CEqpSuspendDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE sizeWideLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_WIDELARGE);

	CRect rc2;
	m_btnEqpSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnEqpSuspend.MoveWindow(rc2.left, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);

	m_btnEqpSuspendSearch.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnEqpSuspendSearch.MoveWindow(rc2.left, rc2.top, sizeWideLarge.cx, sizeWideLarge.cy);
}

BOOL CEqpSuspendDlg::PreTranslateMessage(MSG* pMsg)
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

void CEqpSuspendDlg::FillSpreadColumn(int nColIdx, CString strColumnName)
{
	CString bbb = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	CString aaa = _T("");
	aaa.Format(_T("%s\r"), bbb);
	//m_pSpEQPSUSPEND.SetText( nColIdx + 1, 0, variant_t(aaa));
}

void CEqpSuspendDlg::FillSpreadRow(int nColIdx, int nRowIdx, CString strValue)
{
	CString aaa = _T("");
	aaa.Format(_T("%s\r"), strValue);
	//m_pSpEQPSUSPEND.SetText(nColIdx + 1, nRowIdx + 1, variant_t(aaa));
}
void CEqpSuspendDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue + _T(" [EQP_MST]"));   // [LGLS 2026-09-03] 다른 그리드 창처럼 테이블명 표기


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_EQP_SUSPEND2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("whtyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_WH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqptyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_EQP_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpno"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_EQP_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpsuspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpgrp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_PLC_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcip"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_PLC_IP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcportfr"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_PLC_PORT_FROM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcportto"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_PLC_PORT_TO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("connected"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_CONNECTED_YN, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("retryyn"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_RETRY_YN, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("useyn"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_USE_YN, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("useyn"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_USE_YN_2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("sceqp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_SC_EQP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("othereqp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_OTHER_EQP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("result"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND_RESULT, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpchangesta"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(ID_BTN_EQP_SUSPEND_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcip"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_PLC_IP_2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcportfr"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_PLC_PORT_FROM_2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("plcportto"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_PLC_TO_2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("eqpsuspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_EQP_SUSPEND2_3, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_EQP_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("selectresult"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_EQP_SUSPEND2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\"), _T("dlg_eqpsuspend"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("update"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_EQP_EQP_STATE, strValue);


}

void CEqpSuspendDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	CString strAppPath2 = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_eqpsuspend\\128x32\\");
	CString strExtension = _T(".png");
	
	m_btnEqpSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnEqpSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
	
	m_btnEqpSuspendSearch.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_WIDELARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnEqpSuspendSearch.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
}

void CEqpSuspendDlg::InitializeResource(EN_LANG nEN_LANG)
{
	RenameResource(nEN_LANG);
	RedrawImage();
}

void CEqpSuspendDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pDoc->m_pEqpSuspendDlg =  NULL;
	CSkinDialog::OnClose();
}

void CEqpSuspendDlg::OnBnClickedBtnEqpSuspendSearch()
{
	m_SpreadSheet.PrepareLoadSpread();

	CStringList strList;
	CStringArray arrColName;
	CStringArray arrColSize;
	int nLANG = m_pDoc->m_enLang;
	int nColSize = -1;
	int nRowCnt = -1;
//	int nColCnt;

	CString strMessage = _T("");

	CString strSql = GetQrySelect_Main();

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
		// arrColName과 arrColSize의 갯수가 다름!! Hide되어있는 Column을 가져오느라 어쩔수 없음~
		int nIdxStart = SetHeadColumn(arrColName, arrColSize, nColSize, strSql);

		//SetMaxRows(nRowCnt);

		pRsw->MoveFirst();

		for( int nIdxRow = 1; nIdxRow <= nRowCnt; nIdxRow++ )
		{
			m_SpreadSheet.SetCurrentRow();
			for (int nIdxCol = nIdxStart; nIdxCol < nColSize; nIdxCol++)		
			{
				CString strColValue = pRsw->GetItem(arrColName[nIdxCol]);

				// [LGLS 2026-09-03] 표시값에 공백 4칸을 붙이던 것 제거 - 가운데 정렬이 왼쪽으로 밀려 보였다(폭 계산에만 반영)
				int nPadLen = CLib::DispLen(strColValue) + 4;
				//if (strColValue.GetLength() < 6)
				//	continue;

				int nPreSize = CConvert::ToInt(arrColSize[nIdxCol]);

				if ((nPadLen*1.5 > nPreSize) && (nPreSize != 0))
				{
					int nSize = (int)(nPadLen*1.5);
					arrColSize[nIdxCol]= CConvert::ToString(nSize);
				}
				SetColumnText(nIdxCol, nIdxRow, strColValue);				// 번역됨
				//m_SpreadSheet.SetData(nIdxCol, nIdxRow, strColValue);		// 번역안됨

				// 짝수마다 색깔을 다르게 칠할것!
				if (nIdxRow % 2 == 0)
				{
					m_SpreadSheet.SetColor(nIdxCol+1, nIdxRow, m_pDoc->m_pConfig->m_clrSPREAD_COLOR, BLACK);
				}
				else
				{
					m_SpreadSheet.SetColor(nIdxCol+1, nIdxRow, SPREAD_COLOR_NONE, BLACK);					
				}

			}
			pRsw->MoveNext();
		}

		if (pRsw != NULL)
		{
			delete pRsw;
		}

		for (int nIdxCol = 0; nIdxCol < nColSize; nIdxCol++)
		{
			int nSize =CConvert::ToInt(arrColSize[nIdxCol]);

			//SetColWidth(nIdxCol, nSize);
			m_SpreadSheet.SetColWidth(nIdxCol + 1, nSize);
		}
	}

	m_lblEqpSuspendCnt.SetWindowText(CConvert::ToString(nRowCnt));
	m_SpreadSheet.FinishLoadSpread();
	return;

}

void CEqpSuspendDlg::SetMaxRows(int pRowCnt)
{
	//m_pSpEQPSUSPEND.SetMaxRows(pRowCnt);
}

void CEqpSuspendDlg::SetMaxCols(int pMaxCol)
{
	//m_pSpEQPSUSPEND.SetMaxCols(pMaxCol);
}

void CEqpSuspendDlg::SetColWidth(int pColCnt, int pColSize)
{
	//m_pSpEQPSUSPEND.put_ColWidth(pColCnt+1,pColSize);
}

void CEqpSuspendDlg::SetColumnText(int nColIdx, int nRowIdx, CString strColumnName)
{
	CString strColValue = m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang);
	if (strColumnName == _T(""))
		strColumnName = _T(" ");

	//m_pSpEQPSUSPEND.SetText( nColIdx + 1, nRowIdx, variant_t(strColValue));
	m_SpreadSheet.SetData(nColIdx + 1, nRowIdx, strColValue);
}

int CEqpSuspendDlg::SetHeadColumn(CStringArray& pStrArrColName, CStringArray& pStrArrColSize, int& nColIdx, CString pStrSql)
{
	CLib::GetColumnName(pStrArrColName, pStrSql);
	nColIdx = pStrArrColName.GetCount();
	int nStartCol = 0;
	if (pStrArrColName[0].Right(5) == _T("_HIDE"))
	{
		nStartCol = - 1;
	}

	//SetMaxCols(nColIdx + nStartCol);

	CString strTemp = _T("");
	CStringArray arrColRename;
	CLib::RenameColumn(pStrArrColName, m_pDoc->m_enLang, arrColRename);
	//int nNewColIdx = 0;
	for(int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
	{
		int nSize = arrColRename[nIdxCol].GetLength();

		strTemp = arrColRename[nIdxCol];

		if (strTemp.GetLength() > 5 && strTemp.Right(5) == _T("_HIDE"))
		{
			nSize = 0;
			if (nIdxCol == 0)
				nSize = 3;

			continue;
		}
		//++nNewColIdx;
		pStrArrColSize.Add(CConvert::ToString(nSize));
	}

	//nStartCol = 1;
	for(int nIdxCol = 0; nIdxCol < nColIdx; nIdxCol++)
	{
		CString asd = arrColRename[nIdxCol];
		
		strTemp = arrColRename[nIdxCol];

		if (strTemp.GetLength() > 5 && strTemp.Right(5) == _T("_HIDE"))
			continue;

		SetColumnText(nIdxCol + nStartCol, 0, arrColRename[nIdxCol]);
		
	}

	return 0;
}

CString CEqpSuspendDlg::GetQrySelect_Main()
{
 	CString strSql = _T("");
	CString strTemp = _T("");															
	CString strEQP_TYP = _T("");
	CString strCONNECTED_YN = _T("");

	strEQP_TYP = m_cbxEqpSuspendEqpTyp.GetItemKey(m_cbxEqpSuspendEqpTyp.GetCurSel());
	strCONNECTED_YN = m_cbxEqpSusepndConnectedYn.GetItemKey(m_cbxEqpSusepndConnectedYn.GetCurSel());

	strEQP_TYP.Trim();
	strCONNECTED_YN.Trim();
	
	strSql.Format(_T(" SELECT ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, EM.WH_TYP) AS WH_TYP 																\n")	
			  _T("		     ,EM.EQP_TYP AS EQP_TYP																												\n")		
			  _T("		     ,EM.REMARKS AS EQP_NM																												\n")
			  _T("		     ,EM.PLC_IP AS PLC_IP																												\n")
			  _T("		     ,EM.PLC_PORT AS PLC_PORT																											\n") 
			  //_T("           , ") + m_pDoc->NVL + _T("(CCD_CONNECTED_YN.CCD_NM_KOR, EM.CONNECTED_YN) AS CONNECTED_YN	\n")	
			  //_T("		     ,(CASE WHEN EQP_TYP = 'HOST' THEN (SELECT INS_DT FROM HOST_IF_LOG ORDER BY INS_DT DESC LIMIT 1) ELSE UPD_DT END) AS UPD_DT			\n")
			  //_T("		     ,EM.UPD_DT AS EQP_UPD_DT																											\n")
			  _T("       FROM EQP_MST EM																														\n")	
			  _T("                   LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP																						\n")
			  _T("								  ON CCD_WH_TYP.WH_TYP LIKE '%%%s%%'																			\n")	
			  _T("								 AND CCD_WH_TYP.CDX_CD = 'WH_TYP'																				\n")	
			  _T("								 AND EM.WH_TYP = CCD_WH_TYP.CCD_CD																				\n")	
			  _T("  WHERE EM.WH_TYP = '%s'											\n")
			  _T("    AND EM.EQP_TYP IN ('EQP','HOST','HOST2','SCH') /* [LGLS 2026-09-03] EQP/HOST(HOST2 포함)/SCH 만 */					\n"), m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP);   // [LGLS 2026-07-22] 설비별 행 제외(아래 대표 1행으로 대체)				    
	
	
	if ((strEQP_TYP != "") && (strEQP_TYP != _T("ALL")))
	{	
		if (strEQP_TYP == _T("HOST"))
			strSql += _T("AND EM.EQP_TYP IN ('HOST','HOST2') \n");   // [LGLS 2026-09-03] HOST 는 HOST2 까지
		else
			strSql += _T("AND EM.EQP_TYP ='") + strEQP_TYP + _T("' \n");	
	}
	
	if ((strCONNECTED_YN != "") && (strCONNECTED_YN != _T("ALL")))
	{		
		strSql += _T("AND EM.CONNECTED_YN ='") + strCONNECTED_YN + _T("' \n");	
	}
	
	// [LGLS 2026-07-22] 설비와 통신하는 프로그램은 EQP_TASK 하나뿐 ? 설비별(CV/SC/RTV) 나열 대신
	//   대표 1행(마스터 PLC 소켓 정의 = CV#11 행의 IP/PORT)으로 표현한다.
	CString strUnion;
	strUnion.Format(_T(" UNION ALL SELECT ") + m_pDoc->NVL + _T("(CCD_WH_TYP.CCD_NM_KOR, EM.WH_TYP), 'EQP', 'EQP_TASK 설비통신 (마스터 PLC)', EM.PLC_IP, EM.PLC_PORT	\n")
		_T("   FROM EQP_MST EM LEFT OUTER JOIN COMMON_CODE CCD_WH_TYP										\n")
		_T("     ON CCD_WH_TYP.WH_TYP LIKE '%%%s%%' AND CCD_WH_TYP.CDX_CD = 'WH_TYP' AND EM.WH_TYP = CCD_WH_TYP.CCD_CD	\n")
		_T("  WHERE EM.WH_TYP = '%s' AND EM.EQP_TYP = 'CV' AND EM.PLC_NO = '11'						\n"), m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP);
	strSql += strUnion;
	strSql += _T("   ORDER BY 2 \n");


   return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CEqpSuspendDlg::GetQryUpdate( CString strWH_TYP, CString strEQP_TYP, CString strPLC_NO, CString strEQP_NO)
{
	CString strTABLE_NM;
	CString strEQP_KEY = strEQP_TYP;
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	//if	   (strEQP_KEY == _T("CV")){  strEQP_KEY = _T("TRACK_NO"); strTABLE_NM = _T("CV_DATA");}
	//else if(strEQP_KEY == _T("BCR")){ strEQP_KEY = _T("BCR_NO"); strTABLE_NM = _T("BCR_MST");}
	//else   {						  strEQP_KEY += _T("_NO");  strTABLE_NM = CLib::Concat(strEQP_TYP, _T("_DATA")); }
	//
	//strTABLE_NM.Trim();
	//CString CRLF = _T("\r\n");
	//CString strSql = _T("");
	//strSql += CRLF + _T(" UPDATE ") + CLib::Quot(strTABLE_NM);
	//if (strEQP_TYP == _T("SC"))
	//{
	//	strSql += CRLF + _T("    SET SUSPEND = ") + CLib::Quot(m_strScEqpSuspend);
	//}else{
	//	strSql += CRLF + _T("    SET SUSPEND = ") + CLib::Quot(m_strOtherEqpSuspend);
	//}
	//strSql += CRLF + _T("  WHERE WH_TYP = ") + CLib::Quot(strWH_TYP);
	//strSql += CRLF + _T("    AND PLC_NO = ") + CLib::Quot(strPLC_NO);
	//strSql += CRLF + _T("    AND EQP_NO = ") + CLib::Quot(strEQP_NO);

	if (strEQP_TYP == _T("CV"))
	{
		strSql += CRLF + _T(" UPDATE CV_DATA");
		strSql += CRLF + _T("    SET SUSPEND = '") + m_strOtherEqpSuspend + _T("'");
		strSql += CRLF + _T("  WHERE WH_TYP = '") + strWH_TYP + _T("'");
		strSql += CRLF + _T("    AND PLC_NO = '") + strPLC_NO + _T("'");
		strSql += CRLF + _T("    AND TRACK_NO = '") + strEQP_NO+ _T("'");
	}
	else if (strEQP_TYP == _T("RTV"))
	{
		strSql += CRLF + _T(" UPDATE RTV_DATA_LGLS");
		strSql += CRLF + _T("    SET SUSPEND = '") + m_strOtherEqpSuspend + _T("'");
		strSql += CRLF + _T("  WHERE WH_TYP = '") + strWH_TYP + _T("'");
		strSql += CRLF + _T("    AND PLC_NO = '") + strPLC_NO + _T("'");
		strSql += CRLF + _T("    AND RTV_NO = '") + strEQP_NO + _T("'");
	}
	else if (strEQP_TYP == _T("BCR"))
	{
		strSql += CRLF + _T(" UPDATE BCR_MST");
		strSql += CRLF + _T("    SET SUSPEND = '") + m_strOtherEqpSuspend + _T("'");
		strSql += CRLF + _T("  WHERE WH_TYP = '") + strWH_TYP + _T("'");
		strSql += CRLF + _T("    AND BCR_NO = '") + strPLC_NO + _T("'");
		strSql += CRLF + _T("    AND BCR_MC_NO = '") +strEQP_NO + _T("'");
	
	}
	else if (strEQP_TYP == _T("SC"))
	{
		strSql += CRLF + _T(" UPDATE SC_DATA_LGLS");
		strSql += CRLF + _T("    SET SUSPEND = '") + m_strScEqpSuspend + _T("'");
		strSql += CRLF + _T("  WHERE WH_TYP = '") + strWH_TYP + _T("'");
		strSql += CRLF + _T("    AND PLC_NO = '") + strPLC_NO + _T("'");
		strSql += CRLF + _T("    AND SC_NO = '") + strEQP_NO + _T("'");
	}

	return strSql;
}

CString CEqpSuspendDlg::GetQrySelect(int nLANG,CString strWH_TYP, CString strEQP_TYP, CString strEQP_SUSPEND, 
									 CString strEQP_NO, CString strEQP_GRP_NO, CString strPLC_IP,
									 CString strPLC_PORT_FR, CString strPLC_PORT_TO, 
									 CString strCONNECTED_YN, CString strRETRY_YN, CString strUSE_YN)
{


	//CString CRLF = _T("\r\n");
	CString strSql = _T("");
	//CString strSqlWhTypCommonCode = CLib::GetCommonCode(strSql, _T("WH_TYP"), m_pDoc, nLANG);
	//CString strSqlSuspendCommonCode = CLib::GetCommonCode(strSql, _T("SUSPEND"), m_pDoc, nLANG);
	//CString strSqlConnectedYnCommonCode = CLib::GetCommonCode(strSql, _T("CONNECTED_YN"), m_pDoc, nLANG);
	//CString strSqlRetryYnCommonCode = CLib::GetCommonCode(strSql, _T("RETRY_YN"), m_pDoc, nLANG);
	//CString strSqlUseYnCommonCode = CLib::GetCommonCode(strSql, _T("USE_YN"), m_pDoc, nLANG);


	//if (strEQP_TYP == "CV")
	//{
	//	strSql+= CRLF + _T("         SELECT CV.WH_TYP					");
	//	//***********************언어에 따라 공통코드 값을 다르게 가져온다.**************************//
	//	if (nLANG == 0)//한국어
	//	{strSql+=CRLF + _T("              , COM1.CCD_NM_KOR AS WH_TYP_NM");}
	//	else if (nLANG == 1)//영어
	//	{strSql+=CRLF + _T("			  , COM1.CCD_NM_ENG AS WH_TYP_NM");}
	//	else if (nLANG == 2)//헝가리어
	//	{strSql+=CRLF + _T("			  , COM1.CCD_NM_HUN AS WH_TYP_NM");}
	//	else				//중국어
	//	{strSql+=CRLF + _T("		  , COM1.CCD_NM_CHIN AS WH_TYP_NM");}
	//	//********************************************************************************************
	//	strSql+= CRLF + _T("              , 'CV' AS EQP_TYP				");
	//	strSql+= CRLF + _T("              , CV.PLC_NO					");
	//	strSql+= CRLF + _T("              , CV.TRACK_NO EQP_NO			");
	//	strSql+= CRLF + _T("              , CV.SUSPEND					");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("              , COM2.CCD_NM_KOR AS SUSPEND_NM ");}
	//	else if (nLANG == 1)
	//	{strSql+=CRLF + _T("			  , COM2.CCD_NM_ENG AS SUSPEND_NM ");}
	//	else if (nLANG == 2)
	//	{strSql+=CRLF + _T("			  , COM2.CCD_NM_HUN AS SUSPEND_NM ");}
	//	else{strSql+=CRLF + _T("		  , COM2.CCD_NM_CHIN AS SUSPEND_NM ");}
	//	strSql+= CRLF + _T("              , EQP.PLC_IP					");
	//	strSql+= CRLF + _T("			  , EQP.PLC_PORT_FROM			");
	//	strSql+= CRLF + _T("			  , EQP.PLC_PORT_TO				");
	//	strSql+= CRLF + _T("			  , EQP.CONNECTED_YN			");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("              , COM3.CCD_NM_KOR AS CONNECTED_YN_NM ");}
	//	else if (nLANG == 1)
	//	{strSql+=CRLF + _T("			  , COM3.CCD_NM_ENG AS CONNECTED_YN_NM ");}
	//	else if (nLANG == 2)
	//	{strSql+=CRLF + _T("			  , COM3.CCD_NM_HUN AS CONNECTED_YN_NM ");}
	//	else{strSql+=CRLF + _T("		  , COM3.CCD_NM_CHIN AS CONNECTED_YN_NM ");}
	//	strSql+= CRLF + _T("			  , EQP.RETRY_YN				");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("              , COM4.CCD_NM_KOR AS RETRY_YN_NM ");}
	//	else if (nLANG == 1)
	//	{strSql+=CRLF + _T("			  , COM4.CCD_NM_ENG AS RETRY_YN_NM ");}
	//	else if (nLANG == 2)
	//	{strSql+=CRLF + _T("			  , COM4.CCD_NM_HUN AS RETRY_YN_NM ");}
	//	else{strSql+=CRLF + _T("	      , COM4.CCD_NM_CHIN AS RETRY_YN_NM ");}
	//	strSql+= CRLF + _T("			  , EQP.USE_YN					");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("              , COM5.CCD_NM_KOR AS USE_YN_NM ");}
	//	else if (nLANG == 1)
	//	{strSql+=CRLF + _T("			  , COM5.CCD_NM_ENG AS USE_YN_NM ");}
	//	else if (nLANG == 2)
	//	{strSql+=CRLF + _T("			  , COM5.CCD_NM_HUN AS USE_YN_NM ");}
	//	else{strSql+=CRLF + _T("		  , COM5.CCD_NM_CHIN AS USE_YN_NM ");}
	//	strSql+= CRLF + _T("           FROM CV_DATA CV					");
	//	strSql+= CRLF + _T("				INNER JOIN (SELECT * FROM EQP_MST WHERE WH_TYP = '") + strWH_TYP + _T("' AND EQP_TYP = 'CV') EQP ");
	//	strSql+= CRLF + _T("					    ON CV.PLC_NO = EQP.PLC_NO	");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlWhTypCommonCode + _T(") COM1");
	//	strSql+= CRLF + _T("		        			 ON CV.WH_TYP = COM1.CCD_CD 		");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlSuspendCommonCode + _T(") COM2");
	//	strSql+= CRLF + _T("		        			 ON CV.SUSPEND = COM2.CCD_CD 		");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlConnectedYnCommonCode + _T(") COM3");
	//	strSql+= CRLF + _T("		        			 ON EQP.CONNECTED_YN = COM3.CCD_CD 		");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlRetryYnCommonCode + _T(") COM4");
	//	strSql+= CRLF + _T("		        			 ON EQP.RETRY_YN = COM4.CCD_CD 		");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlUseYnCommonCode + _T(") COM5");
	//	strSql+= CRLF + _T("		        			 ON EQP.USE_YN = COM5.CCD_CD 		");
	//	strSql+= CRLF + _T("          WHERE CV.WH_TYP = '") + strWH_TYP + _T("'");

	//	if (strEQP_GRP_NO != _T(""))
	//	{
	//		if (CConvert::ToInt(strEQP_GRP_NO) < 10) { strEQP_GRP_NO = _T("0") + strEQP_GRP_NO;}

	//		strSql+= CRLF + _T("	 AND CV.PLC_NO = ") + CLib::Quot(strEQP_GRP_NO);
	//	}
	//	if (strEQP_NO != _T(""))
	//	{
	//		strSql+= CRLF + _T("	 AND CV.TRACK_NO LIKE '%") + strEQP_NO + _T("%'");
	//	}
	//	if (strEQP_SUSPEND != _T("-1") )
	//	{
	//		strSql+= CRLF + _T("	 AND CV.SUSPEND = ") + CLib::Quot(strEQP_SUSPEND);
	//	}
	//	if (strPLC_IP != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_IP = ") + CLib::Quot(strPLC_IP);
	//	}
	//	if (strPLC_PORT_FR != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_PORT_FROM = ") + CLib::Quot(strPLC_PORT_FR);
	//	}
	//	if (strPLC_PORT_TO != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_PORT_TO = ") + CLib::Quot(strPLC_PORT_TO);
	//	}
	//	if (strCONNECTED_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.CONNECTED_YN = ") + CLib::Quot(strCONNECTED_YN);
	//	}
	//	if (strRETRY_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.RETRY_YN = ") + CLib::Quot(strRETRY_YN);
	//	}
	//	if (strUSE_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.USE_YN = ") + CLib::Quot(strUSE_YN);
	//	}
	//	strSql += CRLF + _T("    ORDER BY CV.TRACK_NO ");	

	//	return strSql;
	//}
	//else if (strEQP_TYP == _T("SC"))
	//{
	//	strSql+= CRLF + _T("         SELECT SC.WH_TYP          ");
	//	if (nLANG == 0)//한국어
	//	{strSql+=CRLF + _T("              , COM1.CCD_NM_KOR AS WH_TYP_NM");}
	//	else if (nLANG == 1)//영어
	//	{strSql+=CRLF + _T("			  , COM1.CCD_NM_ENG AS WH_TYP_NM");}
	//	else if (nLANG == 2)//헝가리어
	//	{strSql+=CRLF + _T("			  , COM1.CCD_NM_HUN AS WH_TYP_NM");}
	//	else				//중국어
	//	{strSql+=CRLF + _T("			  , COM1.CCD_NM_CHIN AS WH_TYP_NM");}
	//	strSql+= CRLF + _T("              , 'SC' AS EQP_TYP	   ");
	//	strSql+= CRLF + _T("              , SC.PLC_NO		   ");
	//	strSql+= CRLF + _T("              , SC.SC_NO EQP_NO	   ");
	//	strSql+= CRLF + _T("              , SC.SUSPEND		   ");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("              , COM2.CCD_NM_KOR AS SUSPEND_NM ");}
	//	else if (nLANG == 1)
	//	{strSql+=CRLF + _T("			  , COM2.CCD_NM_ENG AS SUSPEND_NM ");}
	//	else if (nLANG == 2)
	//	{strSql+=CRLF + _T("			  , COM2.CCD_NM_HUN AS SUSPEND_NM ");}
	//	else{strSql+=CRLF + _T("		  , COM2.CCD_NM_CHIN AS SUSPEND_NM ");}
	//	strSql+= CRLF + _T("              , EQP.PLC_IP		   ");
	//	strSql+= CRLF + _T("			  , EQP.PLC_PORT_FROM	");
	//	strSql+= CRLF + _T("			  , EQP.PLC_PORT_TO		");
	//	strSql+= CRLF + _T("			  , EQP.CONNECTED_YN	");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("              , COM3.CCD_NM_KOR AS CONNECTED_YN_NM ");}
	//	else if (nLANG == 1)
	//	{strSql+=CRLF + _T("			  , COM3.CCD_NM_ENG AS CONNECTED_YN_NM ");}
	//	else if (nLANG == 2)
	//	{strSql+=CRLF + _T("			  , COM3.CCD_NM_HUN AS CONNECTED_YN_NM ");}
	//	else{strSql+=CRLF + _T("		  , COM3.CCD_NM_CHIN AS CONNECTED_YN_NM ");}
	//	strSql+= CRLF + _T("			  , EQP.RETRY_YN		");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("              , COM4.CCD_NM_KOR AS RETRY_YN_NM ");}
	//	else if (nLANG == 1)
	//	{strSql+=CRLF + _T("			  , COM4.CCD_NM_ENG AS RETRY_YN_NM ");}
	//	else if (nLANG == 2)
	//	{strSql+=CRLF + _T("			  , COM4.CCD_NM_HUN AS RETRY_YN_NM ");}
	//	else{strSql+=CRLF + _T("	      , COM4.CCD_NM_CHIN AS RETRY_YN_NM ");}
	//	strSql+= CRLF + _T("			  , EQP.USE_YN			");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("              , COM5.CCD_NM_KOR AS USE_YN_NM ");}
	//	else if (nLANG == 1)
	//	{strSql+=CRLF + _T("			  , COM5.CCD_NM_ENG AS USE_YN_NM ");}
	//	else if (nLANG == 2)
	//	{strSql+=CRLF + _T("			  , COM5.CCD_NM_HUN AS USE_YN_NM ");}
	//	else{strSql+=CRLF + _T("		  , COM5.CCD_NM_CHIN AS USE_YN_NM ");}
	//	strSql+= CRLF + _T("           FROM SC_DATA_LGLS	SC	        ");
	//	strSql+= CRLF + _T("				INNER JOIN (SELECT * FROM EQP_MST WHERE WH_TYP = '") + strWH_TYP + _T("' AND EQP_TYP = 'SC') EQP ");
	//	strSql+= CRLF + _T("					    ON SC.PLC_NO = EQP.PLC_NO	");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlWhTypCommonCode + _T(") COM1");
	//	strSql+= CRLF + _T("		        			 ON SC.WH_TYP = COM1.CCD_CD 		");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlSuspendCommonCode + _T(") COM2");
	//	strSql+= CRLF + _T("		        			 ON SC.SUSPEND = COM2.CCD_CD 		\n");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlConnectedYnCommonCode + _T(") COM3");
	//	strSql+= CRLF + _T("		        			 ON EQP.CONNECTED_YN = COM3.CCD_CD 		");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlRetryYnCommonCode + _T(") COM4");
	//	strSql+= CRLF + _T("		        			 ON EQP.RETRY_YN = COM4.CCD_CD 		");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlUseYnCommonCode + _T(") COM5");
	//	strSql+= CRLF + _T("		        			 ON EQP.USE_YN = COM5.CCD_CD 		");
	//	strSql+= CRLF + _T("          WHERE SC.WH_TYP = '") + strWH_TYP+ _T("'");

	//	if (strEQP_GRP_NO != _T(""))
	//	{
	//		if (strEQP_GRP_NO.GetLength() < 2){ strEQP_GRP_NO = _T("0") + strEQP_GRP_NO; }

	//		strSql+= CRLF + _T("	 AND SC.PLC_NO = ") + CLib::Quot(strEQP_GRP_NO);
	//	}
	//	if (strEQP_NO != _T(""))
	//	{
	//		strSql+= CRLF + _T("	 AND SC.SC_NO LIKE '%") + strEQP_NO + _T("%'");
	//	}
	//	if (strEQP_SUSPEND != _T("-1") )
	//	{
	//		strSql+= CRLF + _T("	 AND SC.SUSPEND = ") + CLib::Quot(strEQP_SUSPEND);
	//	}
	//	if (strPLC_IP != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_IP = ") + CLib::Quot(strPLC_IP);
	//	}
	//	if (strPLC_PORT_FR != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_PORT_FROM = ") + CLib::Quot(strPLC_PORT_FR);
	//	}
	//	if (strPLC_PORT_TO != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_PORT_TO = ") + CLib::Quot(strPLC_PORT_TO);
	//	}
	//	if (strCONNECTED_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.CONNECTED_YN = ") + CLib::Quot(strCONNECTED_YN);
	//	}
	//	if (strRETRY_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.RETRY_YN = ") + CLib::Quot(strRETRY_YN);
	//	}
	//	if (strUSE_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.USE_YN = ") + CLib::Quot(strUSE_YN);
	//	}
	//	strSql += CRLF + _T("    ORDER BY SC.PLC_NO \n");	

	//	return strSql;
	//}
	//else if (strEQP_TYP == _T("BCR"))
	//{
	//	strSql+= CRLF + _T("          SELECT BCR.WH_TYP                      ");
	//	if (nLANG == 0)//한국어
	//	{strSql+=CRLF + _T("              , COM1.CCD_NM_KOR AS WH_TYP_NM");}
	//	else if (nLANG == 1)//영어
	//	{strSql+=CRLF + _T("			  , COM1.CCD_NM_ENG AS WH_TYP_NM");}
	//	else if (nLANG == 2)//헝가리어
	//	{strSql+=CRLF + _T("			  , COM1.CCD_NM_HUN AS WH_TYP_NM");}
	//	else				//중국어
	//	{strSql+=CRLF + _T("			  , COM1.CCD_NM_CHIN AS WH_TYP_NM");}
	//	strSql+= CRLF + _T("               , 'BCR' AS EQP_TYP				 ");
	//	strSql+= CRLF + _T("               , BCR.BCR_NO AS PLC_NO   	     ");
	//	strSql+= CRLF + _T("               , BCR.BCR_MC_NO EQP_NO            ");
	//	strSql+= CRLF + _T("               , BCR.SUSPEND	                 ");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("                , COM2.CCD_NM_KOR AS SUSPEND_NM ");}
	//	else if (nLANG == 1)
	//	{strSql+=CRLF + _T("			    , COM2.CCD_NM_ENG AS SUSPEND_NM ");}
	//	else if (nLANG == 2)
	//	{strSql+=CRLF + _T("			    , COM2.CCD_NM_HUN AS SUSPEND_NM ");}
	//	else{strSql+=CRLF + _T("		    , COM2.CCD_NM_CHIN AS SUSPEND_NM ");}
	//	strSql+= CRLF + _T("                , EQP.PLC_IP					  ");
	//	strSql+= CRLF + _T(" 				, EQP.PLC_PORT_FROM				 ");
	//	strSql+= CRLF + _T(" 				, EQP.PLC_PORT_TO				 ");
	//	strSql+= CRLF + _T(" 				, EQP.CONNECTED_YN				 ");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("                , COM3.CCD_NM_KOR AS CONNECTED_YN_NM ");}
	//	else if (nLANG == 1)			    
	//	{strSql+=CRLF + _T("			    , COM3.CCD_NM_ENG AS CONNECTED_YN_NM ");}
	//	else if (nLANG == 2)			    
	//	{strSql+=CRLF + _T("			    , COM3.CCD_NM_HUN AS CONNECTED_YN_NM ");}
	//	else{strSql+=CRLF + _T("		    , COM3.CCD_NM_CHIN AS CONNECTED_YN_NM ");}
	//	strSql+= CRLF + _T(" 				, EQP.RETRY_YN					 ");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("                , COM4.CCD_NM_KOR AS RETRY_YN_NM ");}
	//	else if (nLANG == 1)			    
	//	{strSql+=CRLF + _T("			    , COM4.CCD_NM_ENG AS RETRY_YN_NM ");}
	//	else if (nLANG == 2)			    
	//	{strSql+=CRLF + _T("			    , COM4.CCD_NM_HUN AS RETRY_YN_NM ");}
	//	else{strSql+=CRLF + _T("	        , COM4.CCD_NM_CHIN AS RETRY_YN_NM ");}
	//	strSql+= CRLF + _T(" 				, EQP.USE_YN					 ");
	//	if (nLANG == 0)
	//	{strSql+=CRLF + _T("                , COM5.CCD_NM_KOR AS USE_YN_NM ");}
	//	else if (nLANG == 1)			    
	//	{strSql+=CRLF + _T("			    , COM5.CCD_NM_ENG AS USE_YN_NM ");}
	//	else if (nLANG == 2)			    
	//	{strSql+=CRLF + _T("			    , COM5.CCD_NM_HUN AS USE_YN_NM ");}
	//	else{strSql+=CRLF + _T("		    , COM5.CCD_NM_CHIN AS USE_YN_NM ");}
	//	strSql+= CRLF + _T("            FROM BCR_MST	BCR					 ");
	//	strSql+= CRLF + _T(" 				INNER JOIN (SELECT * FROM EQP_MST WHERE WH_TYP = '") + strWH_TYP + _T("' AND EQP_TYP = 'BCR') EQP ");
	//	strSql+= CRLF + _T(" 					    ON BCR.BCR_NO = EQP.PLC_NO			");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlWhTypCommonCode + _T(") COM1");
	//	strSql+= CRLF + _T("		        			 ON BCR.WH_TYP = COM1.CCD_CD 		");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlSuspendCommonCode + _T(") COM2");
	//	strSql+= CRLF + _T("		        			 ON BCR.SUSPEND = COM2.CCD_CD 		\n");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlConnectedYnCommonCode + _T(") COM3");
	//	strSql+= CRLF + _T("		        			 ON EQP.CONNECTED_YN = COM3.CCD_CD 		");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlRetryYnCommonCode + _T(") COM4");
	//	strSql+= CRLF + _T("		        			 ON EQP.RETRY_YN = COM4.CCD_CD 		");
	//	strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlUseYnCommonCode + _T(") COM5");
	//	strSql+= CRLF + _T("		        			 ON EQP.USE_YN = COM5.CCD_CD 		");
	//	strSql+= CRLF + _T("           WHERE BCR.WH_TYP = '") + strWH_TYP + _T("'");

	//	if (strEQP_GRP_NO != _T(""))
	//	{
	//		if (CConvert::ToInt(strEQP_GRP_NO) < 10) { strEQP_GRP_NO = _T("0") + strEQP_GRP_NO;}

	//		strSql+= CRLF + _T("	 AND BCR.BCR_NO = ") + CLib::Quot(strEQP_GRP_NO);
	//	}
	//	if (strEQP_NO != _T(""))
	//	{
	//		strSql+= CRLF + _T("	 AND BCR.BCR_MC_NO LIKE '%") + strEQP_NO + (_T("%'"));
	//	}
	//	if (strEQP_SUSPEND != _T("-1") )
	//	{
	//		strSql+= CRLF + _T("	 AND BCR.SUSPEND = ") + CLib::Quot(strEQP_SUSPEND);
	//	}
	//	if (strPLC_IP != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_IP = ") + CLib::Quot(strPLC_IP);
	//	}
	//	if (strPLC_PORT_FR != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_PORT_FROM = ") + CLib::Quot(strPLC_PORT_FR);
	//	}
	//	if (strPLC_PORT_TO != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_PORT_TO = ") + CLib::Quot(strPLC_PORT_TO);
	//	}
	//	if (strCONNECTED_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.CONNECTED_YN = ") + CLib::Quot(strCONNECTED_YN);
	//	}
	//	if (strRETRY_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.RETRY_YN = ") + CLib::Quot(strRETRY_YN);
	//	}
	//	if (strUSE_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.USE_YN = ") + CLib::Quot(strUSE_YN);
	//	}
	//	strSql += CRLF + _T("    ORDER BY BCR.BCR_NO \n");	

	//	return strSql;
	//}
	//else
	//{
	//	strSql+=CRLF + _T("         SELECT RTV.WH_TYP                      ");
	//		if (nLANG == 0)//한국어
	//		{strSql+=CRLF + _T("              , COM1.CCD_NM_KOR AS WH_TYP_NM");}
	//		else if (nLANG == 1)//영어
	//		{strSql+=CRLF + _T("			  , COM1.CCD_NM_ENG AS WH_TYP_NM");}
	//		else if (nLANG == 2)//헝가리어
	//		{strSql+=CRLF + _T("			  , COM1.CCD_NM_HUN AS WH_TYP_NM");}
	//		else				//중국어
	//		{strSql+=CRLF + _T("			  , COM1.CCD_NM_CHIN AS WH_TYP_NM");}
	//	 strSql+=CRLF + _T("                 , 'RTV' AS EQP_TYP			  ");
	//	 strSql+=CRLF + _T("                 , RTV.PLC_NO   				  ");
	//	 strSql+=CRLF + _T("                 , RTV.RTV_NO EQP_NO             ");
	//	 strSql+=CRLF + _T("                 , RTV.SUSPEND	                  ");
	//	 if (nLANG == 0)//한국어
	//	 {strSql+=CRLF + _T("               , COM2.CCD_NM_KOR AS SUSPEND_NM");}
	//	 else if (nLANG == 1)//영어		  	
	//	 {strSql+=CRLF + _T("			    , COM2.CCD_NM_ENG AS SUSPEND_NM");}
	//	 else if (nLANG == 2)//헝가리어	    
	//	 {strSql+=CRLF + _T("			    , COM2.CCD_NM_HUN AS SUSPEND_NM");}
	//	 else				//중국어		 
	//	 {strSql+=CRLF + _T("			    , COM2.CCD_NM_CHIN AS SUSPEND_NM");}
	//	 strSql+=CRLF + _T("                , EQP.PLC_IP					  ");
	//	 strSql+=CRLF + _T("				, EQP.PLC_PORT_FROM           ");
	//	 strSql+=CRLF + _T("				, EQP.PLC_PORT_TO	          ");
	//	 strSql+=CRLF + _T("				, EQP.CONNECTED_YN	          ");
	//	 if (nLANG == 0)
	//	 {strSql+=CRLF + _T("               , COM3.CCD_NM_KOR AS CONNECTED_YN_NM ");}
	//	 else if (nLANG == 1)			    
	//	 {strSql+=CRLF + _T("			    , COM3.CCD_NM_ENG AS CONNECTED_YN_NM ");}
	//	 else if (nLANG == 2)			    
	//	 {strSql+=CRLF + _T("			    , COM3.CCD_NM_HUN AS CONNECTED_YN_NM ");}
	//	 else{strSql+=CRLF + _T("		    , COM3.CCD_NM_CHIN AS CONNECTED_YN_NM ");}
	//	 strSql+=CRLF + _T("				, EQP.RETRY_YN		          ");
	//	 if (nLANG == 0)
	//	 {strSql+=CRLF + _T("               , COM4.CCD_NM_KOR AS RETRY_YN_NM ");}
	//	 else if (nLANG == 1)			    
	//	 {strSql+=CRLF + _T("			    , COM4.CCD_NM_ENG AS RETRY_YN_NM ");}
	//	 else if (nLANG == 2)			    
	//	 {strSql+=CRLF + _T("			    , COM4.CCD_NM_HUN AS RETRY_YN_NM ");}
	//	 else{strSql+=CRLF + _T("	        , COM4.CCD_NM_CHIN AS RETRY_YN_NM ");}
	//	 strSql+=CRLF + _T("				, EQP.USE_YN		          "); 
	//	 if (nLANG == 0)
	//	 {strSql+=CRLF + _T("               , COM5.CCD_NM_KOR AS USE_YN_NM ");}
	//	 else if (nLANG == 1)			    
	//	 {strSql+=CRLF + _T("			    , COM5.CCD_NM_ENG AS USE_YN_NM ");}
	//	 else if (nLANG == 2)			    
	//	 {strSql+=CRLF + _T("			    , COM5.CCD_NM_HUN AS USE_YN_NM ");}
	//	 else{strSql+=CRLF + _T("		    , COM5.CCD_NM_CHIN AS USE_YN_NM ");}
	//	 strSql+=CRLF + _T("           FROM RTV_DATA_LGLS RTV		          ");
	//	 strSql+=CRLF + _T("				INNER JOIN (SELECT * FROM EQP_MST WHERE WH_TYP = '") + strWH_TYP + _T("' AND EQP_TYP = 'RTV') EQP ");
	//	 strSql+=CRLF + _T("					    ON RTV.PLC_NO = EQP.PLC_NO	");
	//	 strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlWhTypCommonCode + _T(") COM1");
	//	 strSql+= CRLF + _T("		        			 ON RTV.WH_TYP = COM1.CCD_CD 		");
	//	 strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlSuspendCommonCode + _T(") COM2");
	//	 strSql+= CRLF + _T("		        			 ON RTV.SUSPEND = COM2.CCD_CD 		\n");
	//	 strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlConnectedYnCommonCode + _T(") COM3");
	//	 strSql+= CRLF + _T("		        			 ON EQP.CONNECTED_YN = COM3.CCD_CD 		");
	//	 strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlRetryYnCommonCode + _T(") COM4");
	//	 strSql+= CRLF + _T("		        			 ON EQP.RETRY_YN = COM4.CCD_CD 		");
	//	 strSql+= CRLF + _T("		        LEFT OUTER JOIN (") + strSqlUseYnCommonCode + _T(") COM5");
	//	 strSql+= CRLF + _T("		        			 ON EQP.USE_YN = COM5.CCD_CD 		");
	//	 strSql+=CRLF + _T("          WHERE RTV.WH_TYP = '") + strWH_TYP + _T("'");

	//	if (strEQP_GRP_NO != _T(""))
	//	{
	//		if (CConvert::ToInt(strEQP_GRP_NO) < 10) { strEQP_GRP_NO = _T("0") + strEQP_GRP_NO;}

	//		strSql+= CRLF + _T("	 AND RTV.PLC_NO = ") + CLib::Quot(strEQP_GRP_NO);
	//	}
	//	if (strEQP_NO != _T(""))
	//	{
	//		strSql+= CRLF + _T("	 AND RTV.RTV_NO LIKE '%") + strEQP_NO + _T("%'");
	//	}
	//	if (strEQP_SUSPEND != _T("-1") )
	//	{
	//		strSql+= CRLF + _T("	 AND RTV.SUSPEND = ") + CLib::Quot(strEQP_SUSPEND);
	//	}
	//	if (strPLC_IP != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_IP = ") + CLib::Quot(strPLC_IP);
	//	}
	//	if (strPLC_PORT_FR != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_PORT_FROM = ") + CLib::Quot(strPLC_PORT_FR);
	//	}
	//	if (strPLC_PORT_TO != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.PLC_PORT_TO = ") + CLib::Quot(strPLC_PORT_TO);
	//	}
	//	if (strCONNECTED_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.CONNECTED_YN = ") + CLib::Quot(strCONNECTED_YN);
	//	}
	//	if (strRETRY_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.RETRY_YN = ") + CLib::Quot(strRETRY_YN);
	//	}
	//	if (strUSE_YN != _T("") )
	//	{
	//		strSql+= CRLF + _T("	 AND EQP.USE_YN = ") + CLib::Quot(strUSE_YN);
	//	}
	//	strSql += CRLF + _T("    ORDER BY RTV.PLC_NO \n");	

		return strSql;
	//}
}



void CEqpSuspendDlg::OnBnClickedBtnEqpSuspendSuspend()
{
	/*
	UpdateData(TRUE);

	if (!m_pDoc->Permission(_T("CEqpSuspendDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	if (m_nActiveRow < 1)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("스프레드를 클릭하시오"))); 
		return;
	}

	CString strSpreadWH_TYP, strSpreadEQP_TYP, strSpreadPLC_NO;
	CString strItemPLC_IP, strItemPLC_PORT_FROM, strItemPLC_PORT_TO, strItemSUSPEND, strItemUSE_YN;
	variant_t val;
	m_pSpEQPSUSPEND.GetText(1, m_nActiveRow, &val);
	strSpreadWH_TYP = (LPCTSTR)(_bstr_t)val;
	strSpreadWH_TYP.Trim();
	strSpreadWH_TYP = m_cbxEqpSuspendWhTyp.GetItemCCD(strSpreadWH_TYP);

	m_pSpEQPSUSPEND.GetText(2, m_nActiveRow, &val);
	strSpreadEQP_TYP = (LPCTSTR)(_bstr_t)val;
	strSpreadEQP_TYP.Trim();

	m_pSpEQPSUSPEND.GetText(4, m_nActiveRow, &val);	
	strSpreadPLC_NO = (LPCTSTR)(_bstr_t)val;
	strSpreadPLC_NO.Trim();

	m_edtPLC_IP.GetWindowText(strItemPLC_IP);
	m_edtPLC_PORT_FROM.GetWindowText(strItemPLC_PORT_FROM);
	m_edtPLC_PORT_TO.GetWindowText(strItemPLC_PORT_TO);
	strItemSUSPEND = m_cbxEQP_SUSPEND.GetItemKey(m_cbxEQP_SUSPEND.GetCurSel());
	strItemUSE_YN = m_cbxUSE_YN.GetItemKey(m_cbxUSE_YN.GetCurSel());

	CString strSql = _T("");
	//CString CRLF = _T("CRLF");
	CString CRLF = _T("\r\n");

	if(AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 설비 상태 변경 작업을 진행하시겠습니까?")), MB_YESNO) == IDYES)
	{
		strSql += CRLF + _T(" UPDATE EQP_MST ");
		strSql += CRLF + _T("    SET PLC_IP = ") + CLib::Quot(strItemPLC_IP);
		strSql += CRLF + _T("      , PLC_PORT_FROM = ") + CLib::Quot(strItemPLC_PORT_FROM);
		strSql += CRLF + _T("      , PLC_PORT_TO = ") + CLib::Quot(strItemPLC_PORT_TO);
		strSql += CRLF + _T("      , USE_YN = ") + CLib::Quot(strItemUSE_YN);
		strSql += CRLF + _T("  WHERE WH_TYP = ") + CLib::Quot(strSpreadWH_TYP);
		strSql += CRLF + _T("    AND EQP_TYP = ") + CLib::Quot(strSpreadEQP_TYP);
		strSql += CRLF + _T("    AND PLC_NO = ") + CLib::Quot(strSpreadPLC_NO);

		long lTrans = m_pDoc->BeginTrans_DLG();
		if(lTrans < 1){ return; };

		CString strUPDATE_TABLE_NAME = strSpreadEQP_TYP;
		BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if (isSuccess == FALSE)
		{
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			return;
		}

		if(strSpreadEQP_TYP == _T("BCR"))
		{
			strUPDATE_TABLE_NAME =_T("BCR_MST");
		}
		else
		{ 
			strUPDATE_TABLE_NAME = strSpreadEQP_TYP + _T("_DATA");
		}

		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("EQP SUSPEND -> 설비 : %s , 설비번호 : %s , SUSPEND : %s , 사용유무 : %s"), strSpreadEQP_TYP, strSpreadPLC_NO, strItemSUSPEND, strItemUSE_YN);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CEqpSuspendDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}

		strSql = _T("");
		strSql += CRLF + _T(" UPDATE ") + strUPDATE_TABLE_NAME;
		strSql += CRLF + _T("    SET SUSPEND = ") + strItemSUSPEND;
		strSql += CRLF + _T("  WHERE WH_TYP = ") + CLib::Quot(strSpreadWH_TYP);
		//strSql += CRLF + _T("    AND EQP_TYP = ") + CLib::Quot(strSpreadEQP_TYP);
		if (strSpreadEQP_TYP == _T("BCR"))
		{
			strSql += CRLF + _T("    AND BCR_NO = ") + CLib::Quot(strSpreadPLC_NO);
		}
		else
		{
			strSql += CRLF + _T("    AND PLC_NO = ") + CLib::Quot(strSpreadPLC_NO);
		}
		

		isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if (isSuccess == FALSE)
		{
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("FAIL")));
			return;
		}

		lTrans = m_pDoc->CommitTrans_DLG();
		if(lTrans < 1){ return; };

		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SUCCESS")));
			return;
	}
	*/

}

void CEqpSuspendDlg::OnBnClickedEqpSuspendCheckAll()
{
	
}

void CEqpSuspendDlg::OnBnClickedChkScActive()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnBnClickedChkScStoSuspend()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnBnClickedChkRetSuspend()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnBnClickedChkScAllSuspend()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnBnClickedChkOtherEqpActive()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnBnClickedChkOtherSuspend()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
	return;
}

void CEqpSuspendDlg::OnCbnSelchangeCbxEqpSuspendEqpTyp()
{
	CString strEqpTyp = _T("");

	UpdateData(TRUE);
	strEqpTyp = m_cbxEqpSuspendEqpTyp.GetItemKey(m_cbxEqpSuspendEqpTyp.GetCurSel());
	m_cbxEqpSuspendEqpSuspend.ResetContent();
	strEqpTyp += _T("_SUSPEND");
	CLib::BindCombo(m_cbxEqpSuspendEqpSuspend, strEqpTyp,  m_pDoc ,m_pDoc->m_enLang);
	UpdateData(FALSE);

}
BEGIN_EVENTSINK_MAP(CEqpSuspendDlg, CSkinDialog)
	ON_EVENT(CEqpSuspendDlg, IDC_EQP_SUSPEND, 5, CEqpSuspendDlg::ClickEqpSuspend, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CEqpSuspendDlg::ClickEqpSuspend(long Col, long Row)
{
	/*
	m_nActiveRow = Row;

	variant_t val;
	m_pSpEQPSUSPEND.GetText(2, Row, &val);
	CString strEQP_TYP = (LPCTSTR)(_bstr_t)val;
	strEQP_TYP.Trim();

	m_pSpEQPSUSPEND.GetText(3, Row, &val);
	CString strSUSPEND = (LPCTSTR)(_bstr_t)val;
	strSUSPEND.Trim();
	CString strBindName = strEQP_TYP + _T("_SUSPEND");
	CLib::BindCombo(m_cbxEQP_SUSPEND, strBindName, m_pDoc ,(int)m_pDoc->m_enLang);
	m_cbxEQP_SUSPEND.SetCurSelTextEx(strSUSPEND);
	//qq55

	m_pSpEQPSUSPEND.GetText(5, Row, &val);
	CString strPLC_IP = (LPCTSTR)(_bstr_t)val;
	strPLC_IP.Trim();
	m_edtPLC_IP.SetWindowText(strPLC_IP);

	m_pSpEQPSUSPEND.GetText(6, Row, &val);
	CString strPLC_PORT_FROM = (LPCTSTR)(_bstr_t)val;
	strPLC_PORT_FROM.Trim();
	m_edtPLC_PORT_FROM.SetWindowText(strPLC_PORT_FROM);

	m_pSpEQPSUSPEND.GetText(7, Row, &val);
	CString strPLC_PORT_TO = (LPCTSTR)(_bstr_t)val;
	strPLC_PORT_TO.Trim();
	m_edtPLC_PORT_TO.SetWindowText(strPLC_PORT_TO);

	m_pSpEQPSUSPEND.GetText(8, Row, &val);
	CString strUSE_YN = (LPCTSTR)(_bstr_t)val;
	strUSE_YN.Trim();
	CLib::BindCombo(m_cbxUSE_YN, _T("USE_YN"), m_pDoc ,(int)m_pDoc->m_enLang);
	m_cbxUSE_YN.SetCurSelTextEx(strUSE_YN);
	*/
}
