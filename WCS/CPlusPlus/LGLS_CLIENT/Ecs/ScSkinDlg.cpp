

// ScSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include <uxtheme.h>	// [LGLS 2026-08-05] 그룹박스 비주얼스타일 해제용
#pragma comment(lib, "uxtheme.lib")
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
	m_brLedOn = NULL; m_brLedOff = NULL; m_brStatus = NULL;
	// [LGLS 2026-09-15] ★확대 패널 멤버 초기화 누락 - 현장 크레인 창이 65535x65535 로 뜬 원인★
	//   RTV/CV 생성자에는 09-12 에 넣었는데 SC 만 빠져 있었다. 쓰레기 값이 양수면
	//   BuildVehStatusPanel 의 "m_nVehBaseH <= 0 이면 측정" 을 건너뛰고 그 값으로 SetWindowPos 한다.
	//   개발 PC 는 우연히 0 이라 재현되지 않았고, 현장(원격 세션)은 [SC] SHOW rect=(-366,76)-(65169,65611).
	m_bVehExpanded = FALSE; m_nVehBaseH = 0; m_nVehPanelH = 0; m_nVehBaseW = 0; m_nVehPanelW = 0;
	m_bScvJobInDb = FALSE;	// [LGLS 2026-10-01]
}
CScSkinDlg::CScSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CScSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pSC_DATA = NULL;
	m_pDB = NULL;
	m_pScManualRet = NULL;
	m_brLedOn = NULL; m_brLedOff = NULL; m_brStatus = NULL;
	// [LGLS 2026-09-15] ★확대 패널 멤버 초기화 누락 - 현장 크레인 창이 65535x65535 로 뜬 원인★
	//   RTV/CV 생성자에는 09-12 에 넣었는데 SC 만 빠져 있었다. 쓰레기 값이 양수면
	//   BuildVehStatusPanel 의 "m_nVehBaseH <= 0 이면 측정" 을 건너뛰고 그 값으로 SetWindowPos 한다.
	//   개발 PC 는 우연히 0 이라 재현되지 않았고, 현장(원격 세션)은 [SC] SHOW rect=(-366,76)-(65169,65611).
	m_bVehExpanded = FALSE; m_nVehBaseH = 0; m_nVehPanelH = 0; m_nVehBaseW = 0; m_nVehPanelW = 0;
	m_bScvJobInDb = FALSE;	// [LGLS 2026-10-01]
}

CScSkinDlg::~CScSkinDlg()
{
	m_pDoc->m_pScSkinDlg = NULL;
	if(m_brLedOn) ::DeleteObject(m_brLedOn);
	if(m_brLedOff) ::DeleteObject(m_brLedOff);
	if(m_brStatus) ::DeleteObject(m_brStatus);
	CSkinDialog::OnClose();
	this->DestroyWindow();
}

void CScSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// [LGLS 2026-08-05] 이 대화상자는 DDX 가 하나도 없어서 명령 버튼들이 CSkinButton 에
	//   붙지 않았다. 그래서 이미 있던 SetBitmaps/SetIcon 호출이 전부 무효였고(m_hWnd=NULL)
	//   CV 와 달리 아이콘이 안 나왔다. 버튼만 연결해 주면 기존 코드가 그대로 살아난다.
	DDX_Control(pDX, IDC_BTN_SC_CONFIRM,        m_btnScConfirm);
	DDX_Control(pDX, IDC_BTN_SC_MANUAL,         m_btnScManual);
	DDX_Control(pDX, IDC_BTN_SC_STO_SUSPEND,    m_btnScStoSuspend);
	DDX_Control(pDX, IDC_BTN_SC_RET_SUSPEND,    m_btnScRetSuspend);
	DDX_Control(pDX, IDC_BTN_SC_ALL_SUSPEND,    m_btnScAllSuspend);
	DDX_Control(pDX, IDC_LGLS_SC_ZOOM,          m_btnVehZoom);	// [LGLS 2026-08-05] 확대/축소
	DDX_Control(pDX, IDC_LGLS_SC_RESEND,        m_btnScResend);	// [LGLS 2026-08-12] 지시 재전송
}

BEGIN_MESSAGE_MAP(CScSkinDlg, CSkinDialog)
	ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CScSkinDlg::OnMessagSwitch)
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BTN_SC_CONFIRM, &CScSkinDlg::OnBnClickedBtnScConfirm)


	ON_BN_CLICKED(IDC_BTN_SC_MANUAL, &CScSkinDlg::OnBnClickedBtnScManual)

	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SC_STO_SUSPEND, &CScSkinDlg::OnBnClickedBtnCvStoSuspend)
	ON_BN_CLICKED(IDC_BTN_SC_RET_SUSPEND, &CScSkinDlg::OnBnClickedBtnScRetSuspend)
	ON_BN_CLICKED(IDC_BTN_SC_ALL_SUSPEND, &CScSkinDlg::OnBnClickedBtnScAllSuspend)
	//ON_BN_CLICKED(IDC_CHK_SC_FORK1, &CScSkinDlg::OnBnClickedChkScFork1)
	//ON_BN_CLICKED(IDC_CHK_SC_FORK2, &CScSkinDlg::OnBnClickedChkScFork2)
	//ON_BN_CLICKED(IDC_CHK_SC_FORK12, &CScSkinDlg::OnBnClickedScRtvFork12)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SCV_RESEND, &CScSkinDlg::OnBnClickedScvResend)
	ON_BN_CLICKED(IDC_SCV_BTN_FORCE,   &CScSkinDlg::OnScvForce)		// [LGLS 2026-10-01] 구 ECS [완료처리]
	ON_BN_CLICKED(IDC_SCV_BTN_ABORT,   &CScSkinDlg::OnScvAbort)		// [LGLS 2026-10-01] 구 ECS [이상종료]
	ON_BN_CLICKED(IDC_SCV_CHK_DISABLE, &CScSkinDlg::OnScvDisable)	// [LGLS 2026-10-01] 구 ECS [사용금지]
	ON_BN_CLICKED(IDC_SCV_OK, &CScSkinDlg::OnBnClickedScvOk)
	ON_BN_CLICKED(IDC_LGLS_SC_ZOOM, &CScSkinDlg::OnBnClickedVehZoom)
	ON_COMMAND_RANGE(IDC_SCV_BTN_LCA_W, IDC_SCV_BTN_UCA_W, &CScSkinDlg::OnAckWrite)	// [LGLS 2026-09-12] Ack 수동 쓰기
	ON_BN_CLICKED(IDC_LGLS_SC_RESEND, &CScSkinDlg::OnBnClickedScvResend)	// [LGLS 2026-08-12] 확대 패널과 동일 기능
END_MESSAGE_MAP()

BOOL CScSkinDlg::OnInitDialog()
{
	m_bDisableMaximize = TRUE;	// [LGLS] 최대화 버튼 숨김+크기고정
	CSkinDialog::OnInitDialog();
	SetTimer(1, 1500, NULL);
	SetTimer(7, 300, NULL);	// [LGLS 2026-09-21] EDIT 흐름 표시 (CLib::MarqueeTick)
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	// [LGLS 2026-09-17] 컨트롤 위치·크기는 Ecs.rc 가 최종값이다(런타임 재배치 없음 - 리소스 뷰 = 실행 화면).
	m_bInitialized = TRUE;

	m_strScFork = _T("0");
	m_brLedOn = ::CreateSolidBrush(RGB(0,200,0)); m_brLedOff = ::CreateSolidBrush(RGB(90,90,90)); m_brStatus = ::CreateSolidBrush(RGB(255,255,150));

	InitializeResource(pEn);
	// [LGLS] 모든 값 TextBox ReadOnly
	int _scvRo[] = { IDC_EDT_SC_JOB_NO, IDC_EDT_SC_FORK_POS,
		IDC_EDT_SC_JOB_STATUS, IDC_EDT_SC_SC_STATUS,
		IDC_EDT_SC_HORIZONTAL_POS, IDC_EDT_SC_PROD_LOAD,
		IDC_EDT_SC_JOB_START_LOC, IDC_EDT_SC_JOB_DEST_LOC, IDC_EDT_SC_JOB_JOB_NO };
	for (int _i = 0; _i < sizeof(_scvRo)/sizeof(int); _i++)
		SendDlgItemMessage(_scvRo[_i], EM_SETREADONLY, TRUE, 0);
	BuildVehStatusPanel();	// [LGLS 2026-08-01] 통신상태 패널 생성(rc 미포함 컨트롤 보완)
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// [LGLS 2026-08-05] 런타임에 만든 라벨은 RenameResource 가 돌 때 아직 존재하지 않는다.
//   생성 직후 같은 ini 로 다시 이름을 붙여 다국어가 적용되게 한다.

// [LGLS 2026-08-05] 그룹박스 캡션 뒤에 회색 네모가 남는 문제.
//   이 그룹박스들은 비주얼 스타일(테마)이 직접 그리기 때문에 WM_CTLCOLOR 로는 못 막는다.
//   테마를 떼면 클래식 방식으로 그려지고, 그때는 OnCtlColor 의 투명 처리가 그대로 먹는다.
void CScSkinDlg::MakeGroupBoxesTransparent()
{
	for (CWnd* pChild = GetWindow(GW_CHILD); pChild != NULL; pChild = pChild->GetWindow(GW_HWNDNEXT))
	{
		TCHAR szCls[32] = { 0 };
		::GetClassName(pChild->GetSafeHwnd(), szCls, 31);
		if (_tcsicmp(szCls, _T("Button")) != 0) continue;
		if ((pChild->GetStyle() & BS_TYPEMASK) != BS_GROUPBOX) continue;
		::SetWindowTheme(pChild->GetSafeHwnd(), L" ", L" ");
		pChild->Invalidate();
	}
}

void CScSkinDlg::RenameRuntimeLabels()
{
	TCHAR chrFile[500]; GetModuleFileName(NULL, chrFile, MAX_PATH);
	CString strApp; strApp.Format(_T("%s"), chrFile);
	CString strPath = Global.GetConcatPath(strApp.Left(strApp.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), _T(".ini"));
	EN_LANG enLang = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;
	CString s;
	CDC* pDC = GetDC();
	CFont* pOld = (pDC != NULL) ? pDC->SelectObject(GetFont()) : NULL;
	s = CLib::GetIniStringFromPath(strPath, _T("lotno"),     (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_SC_LOT_LBL, s);
	s = CLib::GetIniStringFromPath(strPath, _T("productid"), (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_SC_PRD_LBL, s);

	// 번역문이 원래 한글보다 길면 잘리므로 글자 폭에 맞춰 오른쪽 끝을 고정한 채 넓힌다.
	if (pDC != NULL)
	{
		const int nIds[] = { IDC_LGLS_SC_LOT_LBL, IDC_LGLS_SC_PRD_LBL };
		for (int i = 0; i < 2; i++)
		{
			CWnd* pL = GetDlgItem(nIds[i]);
			if (pL == NULL) continue;
			CString t; pL->GetWindowText(t);
			// 라벨이 대화상자와 다른 글꼴을 쓰므로 그 라벨의 글꼴로 재야 정확하다.
			CFont* pPrev = (pL->GetFont() != NULL) ? pDC->SelectObject(pL->GetFont()) : NULL;
			CSize sz = pDC->GetTextExtent(t);
			if (pPrev != NULL) pDC->SelectObject(pPrev);
			CRect rc; pL->GetWindowRect(&rc); ScreenToClient(&rc);
			if (sz.cx + 6 > rc.Width())
			{
				rc.left = rc.right - (sz.cx + 6);
				pL->MoveWindow(rc);
			}
		}
		if (pOld != NULL) pDC->SelectObject(pOld);
		ReleaseDC(pDC);
	}
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
	if (m_pSC_DATA != NULL && !m_pSC_DATA->V_MC_NO_NM.IsEmpty()) strValue += _T(" - ") + m_pSC_DATA->V_MC_NO_NM;
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("forcecompletion"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_SC_CONFIRM, strValue);






	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("resend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LGLS_SC_RESEND, strValue);



	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manualorder"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_SC_MANUAL, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_NO, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_TYP, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_START_POS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_DEST_POS, strValue);






	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("horizontaldrive"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_HORIZONTAL_POS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("forklocation"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_FORK_POS, strValue);


	//strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	//strValue = CLib::GetIniStringFromPath(strFullPath, _T("forklocation2"), (int)m_enLang);
	//SetDlgItemText(IDC_LBL_SC_FORK_POS2, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("forkstatus"), (int)m_enLang);


	//strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	//strValue = CLib::GetIniStringFromPath(strFullPath, _T("forkstatus2"), (int)m_enLang);




	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("activestatus"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_SC_STATUS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("completestatus"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_STATUS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("freightyn"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_PROD_LOAD, strValue);




	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stosuspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_SC_STO_SUSPEND, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("retsuspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_SC_RET_SUSPEND, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("allsuspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_SC_ALL_SUSPEND, strValue);
	
	//

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_JOB_NO, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_JOB_TYP, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno2"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_JOB_NO2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobstartpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_START_POS, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobdestpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobstatus"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_JOB_STATUS, strValue);


	//qq
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scsta"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SC_SC_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SC_SC_STATUS_ITEM1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SC_SC_STATUS_ITEM2, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SC_SC_STATUS_VALUE1, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("value"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SC_SC_STATUS_VALUE2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SC_SC_STATUS_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("error"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SC_JOB_STATUS2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scsuspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SC_SUSPEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobstatus"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_SC_JOB_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startloc"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_START_LOC, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destloc"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_SC_JOB_DEST_LOC, strValue);

	// [LGLS 2026-08-05] 런타임/신규 라벨도 다국어 대상에 포함한다.
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("lotno"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LGLS_SC_LOT_LBL, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("productid"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LGLS_SC_PRD_LBL, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("curloc"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LGLS_SC_CUR_LBL, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cmploc"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LGLS_SC_CMP_LBL, strValue);
}


void CScSkinDlg::RedrawImage()
{
	// [LGLS 2026-08-05] 여기 있던 return; 때문에 버튼 스킨/아이콘 지정이 통째로 죽어 있었다.
	//   (CV 대화상자는 이 return 이 없어서 아이콘이 나온다.)
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







	m_btnScManual.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScManual.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("manual"), strExtension)), NULL, 5, 5);

	m_btnScStoSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScStoSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("save"), strExtension)), NULL, 5, 5);

	// [LGLS 2026-08-12] 원 대화상자 [지시 재전송] 버튼도 같은 스킨 + 아이콘
	m_btnScResend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScResend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("manual"), strExtension)), NULL, 5, 5);

	// [LGLS 2026-08-05] 확대/축소 버튼도 같은 스킨 + 아이콘.
	//   ★비트맵만 주고 아이콘이 없으면 PrepareBitmapRect 가 캡션 영역을 비트맵 폭만큼
	//   오른쪽으로 밀어 글자가 통째로 안 그려진다(빈 버튼 증상의 원인).
	//   아이콘이 있으면 DrawItem 이 캡션 영역을 다시 잡으므로 다른 버튼처럼 나온다.
	m_btnVehZoom.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnVehZoom.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("arrow-down"), strExtension)), NULL, 5, 5);

	m_btnScRetSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScRetSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("save"), strExtension)), NULL, 5, 5);

	m_btnScAllSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScAllSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("save"), strExtension)), NULL, 5, 5);


	// [LGLS 2026-09-17] 명령 버튼(지시 재전송/강제완료/수동지시/확대)과 정지 버튼 3개의 위치·크기(비트맵 110x27)는
	//   Ecs.rc 에 최종값으로 넣었다. 종전의 런타임 세로 쌓기(StackCommandButtons)와 수동지시 표시는 없앴다.
	// [LGLS 2026-09-06] ★[지시 삭제] 제외★ - 설비 인터페이스에 반송지시 취소 신호가 없다.
	//   구 ECS 운영 DB(TB_OBSERVABLE) 기준 S/C·RGV 의 ECS→PLC 쓰기 태그는
	//   FROM/TO/PALLET_ID/TRANSFER_REQUEST + 4개 Ack 뿐이고 취소·삭제 태그가 없다.
	// [LGLS 2026-09-04] [확대] 는 Ecs.ini [MENU] ZOOM_BTN=1/0 으로 표시 여부 선택(기본 1)
	ApplyZoomBtnIni();
}
void CScSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

}
HCURSOR CScSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CScSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
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
	CSingleLock _lockScRead(&m_pDoc->m_csEqpData, TRUE);   // [LGLS] 수집 thread write serialize (refcount assert guard)

	int nSc = CConvert::ToInt(m_pSC_DATA->K_SC_NO);
	CString strTitle;
	strTitle.Format(_T("스태커 크레인 %02d"), nSc);
	SetDlgItemText(IDC_SCV_TITLE1, strTitle);
	SetDlgItemText(IDC_SCV_TITLE2, m_pSC_DATA->V_MC_NO_NM);
	SetDlgItemText(IDC_EDT_SC_NO, m_pSC_DATA->V_MC_NO_NM);	// [LGLS] 상단 박스에 설비명
	// [LGLS] 창 제목 = dlgname(번역) + " - " + 설비명(SC호기). RenameResource 시점엔 m_pSC_DATA=NULL이라 여기서 갱신
	{
		TCHAR _cf[MAX_PATH] = {0}; GetModuleFileName(NULL, _cf, MAX_PATH);
		CString _ap = _cf;
		CString _fp = Global.GetConcatPath(_ap.Left(_ap.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), _T(".ini"));
		CString _cap = CLib::GetIniStringFromPath(_fp, _T("dlgname"), (int)pLang);
		if(!m_pSC_DATA->V_MC_NO_NM.IsEmpty()) _cap += _T(" - ") + m_pSC_DATA->V_MC_NO_NM;
		CString _cur; GetWindowText(_cur);
		if(_cur != _cap) { SetWindowText(_cap); RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW); }
	}

	CString N = m_pDoc->NVL;
	CString strSql =
		_T("SELECT ") + N + _T("(SUBSYSTEM_STATUS_RD,'0') AS ST, ")
		+ N + _T("(LOAD_COMPLETE_RD,'0') AS A1, ")
		+ N + _T("(LOAD_COMPLETE_ACK_OD,'0') AS A2, ")
		+ N + _T("(UNLOAD_COMPLETE_RD,'0') AS A3, ")
		+ N + _T("(UNLOAD_COMPLETE_ACK_OD,'0') AS A4, ")
		+ N + _T("(TRANSFER_REQUEST_OD,'0') AS A5, ")
		+ N + _T("(TRANSFER_ACK_RD,'0') AS A6, ")
		+ N + _T("(SENSOR_FK_RD,'0') AS A7, ")
		+ N + _T("(PALLET_ON_VEHICLE_RD,'') AS PID, ")
		+ N + _T("(ALARM_SET_REPORT_RD,'0') AS A8, ")
		+ N + _T("(ALARM_SET_REPORT_ACK_OD,'0') AS A9, ")
		+ N + _T("(ALARM_RESET_REPORT_RD,'0') AS A10, ")
		+ N + _T("(ALARM_RESET_REPORT_ACK_OD,'0') AS A11, ")
		+ N + _T("(LOCATION_01_RD,'') AS L1, ") + N + _T("(LOCATION_02_RD,'') AS L2, ") + N + _T("(LOCATION_03_RD,'') AS L3, ")
		+ N + _T("(FROM_01_OD,'') AS F1, ") + N + _T("(FROM_02_OD,'') AS F2, ") + N + _T("(FROM_03_OD,'') AS F3, ")
		+ N + _T("(TO_01_OD,'') AS T1, ") + N + _T("(TO_02_OD,'') AS T2, ") + N + _T("(TO_03_OD,'') AS T3, ")
		+ N + _T("(TRANSFER_COMPLETE_LOCATION_01_RD,'') AS C1, ") + N + _T("(TRANSFER_COMPLETE_LOCATION_02_RD,'') AS C2, ") + N + _T("(TRANSFER_COMPLETE_LOCATION_03_RD,'') AS C3, ")
		+ N + _T("(ALARM_SET_CODE_RD,'') AS AC, ")
		+ N + _T("(PALLET_ID_OD,'') AS POD, ")
		+ N + _T("(JOB_TYP_OD,'') AS JT, ") + N + _T("(JOB_TYP_RD,'') AS JTRD, ")
		+ N + _T("(SUSPEND,'0') AS SUS, ")
		+ N + _T("(ONLINE_MODE_RD,'0') AS ONL, ") + N + _T("(AUTO_MODE_RD,'0') AS AUT, ")
		+ N + _T("(ACTIVE_MODE_RD,'0') AS ACT, ") + N + _T("(ERR_CODE_RD,'0000') AS ERR, ")
		+ N + _T("(FORKPOS_FK1_RD,'0') AS FKP, ")
		+ N + _T("(POS_H_RD,'0') AS PH, ")	// [LGLS 2026-09-17] 수직주행(POS_V_RD) 제거 - 현장 값 없음(사용자 지시)
		+ N + _T("(ITN_LUGG_FK1,'0') AS ITN, ") + N + _T("(LUGG_NO_FK1_OD,'0') AS LOD, ")
		+ N + _T("(COMPLETE_RD,'0') AS CMPRD ")
		+ _T("FROM SC_DATA_LGLS WHERE WH_TYP='") + m_pDoc->m_WH_TYP + _T("' AND PLC_NO='") + m_pSC_DATA->K_PLC_NO + _T("' AND MC_NO='") + m_pSC_DATA->K_SC_NO + _T("'");

	CString strMessage; int nRowCnt = -1;
	DWORD dwQ1 = ::GetTickCount();
	_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CLib::UiLog(_T("[SC] SQL1 SC_DATA_LGLS %u ms rows=%d %s"), (unsigned)(::GetTickCount() - dwQ1), nRowCnt,
		strMessage.IsEmpty() ? _T("") : _T("ERR")); 
	if(nRowCnt > 0)
	{
		CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
		pRsw->MoveFirst();
		CString st = pRsw->GetItem(_T("ST"));
		// [LGLS 2026-09-01] 원시값 병기 + 용어를 구 ECS/문서와 통일 (사용자 요청)
		//   구 ECS Vehicle.OnVehicleState : 0=DOWN / 1=IDLE / 2=RUN (Vehicle.cs:1220)
		//   시나리오 문서 D0160~D0210 표기도 Run/Idle/Down.
		CString stMean = (st == _T("2")) ? _T("RUN") : ((st == _T("1")) ? _T("IDLE") : _T("DOWN"));
		CString stTxt = stMean;	// [LGLS 2026-10-01] 구 ECS 사진처럼 낱말만 (DOWN/IDLE/RUN)
		SetDlgItemText(IDC_SCV_STATUS, stTxt);
		SetLed(IDC_SCV_LED_LOAD_CMP,        pRsw->GetItem(_T("A1")));
		SetLed(IDC_SCV_LED_LOAD_CMP_ACK,    pRsw->GetItem(_T("A2")));
		SetLed(IDC_SCV_LED_UNLOAD_CMP,      pRsw->GetItem(_T("A3")));
		SetLed(IDC_SCV_LED_UNLOAD_CMP_ACK,  pRsw->GetItem(_T("A4")));
		SetLed(IDC_SCV_LED_TR_REQ,          pRsw->GetItem(_T("A5")));
		SetLed(IDC_SCV_LED_TR_REQ_ACK,      pRsw->GetItem(_T("A6")));
		SetLed(IDC_SCV_LED_PALLET_EXIST,    pRsw->GetItem(_T("A7")));
		SetLed(IDC_SCV_LED_ALARM_SET,       pRsw->GetItem(_T("A8")));
		SetLed(IDC_SCV_LED_ALARM_SET_ACK,   pRsw->GetItem(_T("A9")));
		SetLed(IDC_SCV_LED_ALARM_RESET,     pRsw->GetItem(_T("A10")));
		SetLed(IDC_SCV_LED_ALARM_RESET_ACK, pRsw->GetItem(_T("A11")));
		SetDlgItemText(IDC_SCV_PALLET_ID, pRsw->GetItem(_T("PID")));
		SetDlgItemText(IDC_SCV_CUR1, pRsw->GetItem(_T("L1")));
		SetDlgItemText(IDC_SCV_CUR2, pRsw->GetItem(_T("L2")));
		SetDlgItemText(IDC_SCV_CUR3, pRsw->GetItem(_T("L3")));
		SetDlgItemText(IDC_SCV_FROM1, pRsw->GetItem(_T("F1")));
		SetDlgItemText(IDC_SCV_FROM2, pRsw->GetItem(_T("F2")));
		SetDlgItemText(IDC_SCV_FROM3, pRsw->GetItem(_T("F3")));
		SetDlgItemText(IDC_SCV_TO1, pRsw->GetItem(_T("T1")));
		SetDlgItemText(IDC_SCV_TO2, pRsw->GetItem(_T("T2")));
		SetDlgItemText(IDC_SCV_TO3, pRsw->GetItem(_T("T3")));
		SetDlgItemText(IDC_SCV_CMP1, pRsw->GetItem(_T("C1")));
		SetDlgItemText(IDC_SCV_CMP2, pRsw->GetItem(_T("C2")));
		SetDlgItemText(IDC_SCV_CMP3, pRsw->GetItem(_T("C3")));
		SetDlgItemText(IDC_SCV_ALARM_CODE, pRsw->GetItem(_T("AC")));
		SetDlgItemText(IDC_SCV_PALLET, pRsw->GetItem(_T("POD")));
		CString jt = pRsw->GetItem(_T("JT"));
		CString io = (jt == _T("1")) ? _T("입고") : ((jt == _T("2")) ? _T("출고") : _T(""));
		SetDlgItemText(IDC_SCV_IO_TAG, io);
		// [LGLS 2026-10-01] 구 ECS 안내문/완료처리/이상종료 판단용으로 상태와 화물을 기억한다
		m_strScvState = st;
		m_strScvJob = pRsw->GetItem(_T("PID"));
		if (m_strScvJob.IsEmpty() || m_strScvJob == _T("0") || m_strScvJob == _T("0000")) m_strScvJob = pRsw->GetItem(_T("POD"));
		CString sus = pRsw->GetItem(_T("SUS"));
		CheckDlgButton(IDC_SCV_CHK_DISABLE, (sus == _T("3")) ? BST_CHECKED : BST_UNCHECKED);
		CheckDlgButton(IDC_BTN_SC_STO_SUSPEND, (sus == _T("1")) ? BST_CHECKED : BST_UNCHECKED);

		// [LGLS 2026-08-01] 대화상자 상단(SC상태/작업상태) 항목 채우기.
		//   기존 코드는 새 통신상태 패널(IDC_SCV_*)만 채우고 rc 의 원래 항목들은 비워 둬서
		//   "값이 하나도 안 나온다"는 상태였다. 관측/지시 컬럼을 사람이 읽는 문구로 표시한다.
		{
			CString itn = pRsw->GetItem(_T("ITN"));
			CString lod = pRsw->GetItem(_T("LOD"));
			CString onl = pRsw->GetItem(_T("ONL"));
			CString aut = pRsw->GetItem(_T("AUT"));
			CString act = pRsw->GetItem(_T("ACT"));
			CString err = pRsw->GetItem(_T("ERR"));
			CString cmp = pRsw->GetItem(_T("CMPRD"));
			CString sen = pRsw->GetItem(_T("A7"));
			// [LGLS 2026-08-05] 작업구분은 PLC 가 돌려주는 값(JOB_TYP_RD) 우선, 없으면 지시값(JOB_TYP_OD).
			CString jt  = pRsw->GetItem(_T("JTRD"));
			if (jt.IsEmpty() || jt == _T("0")) jt = pRsw->GetItem(_T("JT"));

			SetDlgItemText(IDC_EDT_SC_JOB_NO,        (itn == _T("0") || itn == _T("0000")) ? _T("-") : itn);
			// [LGLS 2026-09-21] 지상반·기상반·SRC상태 칸 제거 (사용자 지시) - 셋 다 SUBSYSTEM_STATUS 파생값이라 동작상태와 늘 같았다
			SetDlgItemText(IDC_EDT_SC_SC_STATUS,     stTxt);
			SetDlgItemText(IDC_EDT_SC_JOB_STATUS,    (cmp == _T("1")) ? _T("완료") : _T("-"));
			SetDlgItemText(IDC_EDT_SC_PROD_LOAD,     (sen == _T("1")) ? _T("있음") : _T("없음"));
			CString fkp = pRsw->GetItem(_T("FKP"));		// [LGLS] 포크위치 : COMMON_CODE SC_FORKPOS (0=센터/1=좌출/2=우출)
			CString fkpTxt = (fkp == _T("0")) ? _T("CENTER")
			               : (fkp == _T("1")) ? _T("좌출")
			               : (fkp == _T("2")) ? _T("우출") : fkp;
			SetDlgItemText(IDC_EDT_SC_FORK_POS,      fkpTxt);
			SetDlgItemText(IDC_EDT_SC_HORIZONTAL_POS, pRsw->GetItem(_T("PH")));
			SetDlgItemText(IDC_EDT_SC_JOB_SC_ERR_CODE, CLib::ErrCodeText(m_pDoc, CLib::ScErrTyp(), err));	// [LGLS 2026-09-17] 코드 + 알람 문구
			SetDlgItemText(IDC_EDT_SC_JOB_SC_SUSPEND,
				(sus == _T("1")) ? _T("입고정지") : (sus == _T("2")) ? _T("출고정지") : (sus == _T("3")) ? _T("전체정지") : _T("-"));

			CString sLoc, dLoc, jtTxt;
			sLoc.Format(_T("%s-%s-%s"), pRsw->GetItem(_T("F1")), pRsw->GetItem(_T("F2")), pRsw->GetItem(_T("F3")));
			dLoc.Format(_T("%s-%s-%s"), pRsw->GetItem(_T("T1")), pRsw->GetItem(_T("T2")), pRsw->GetItem(_T("T3")));
			jtTxt = (jt == _T("1")) ? _T("입고") : (jt == _T("2")) ? _T("출고") : _T("-");
			SetDlgItemText(IDC_EDT_SC_JOB_START_LOC, sLoc);
			SetDlgItemText(IDC_EDT_SC_JOB_DEST_LOC,  dLoc);
			SetDlgItemText(IDC_CBX_SC_JOB_TYP,   jtTxt);
			SetDlgItemText(IDC_CBX_SC_START_POS, sLoc);
			SetDlgItemText(IDC_CBX_SC_DEST_POS,  dLoc);

			// [LGLS 2026-08-05] PLC 에서 받아오는 현재위치/완료위치도 SC상태 그룹에 표시
			CString curLoc, cmpLoc;
			curLoc.Format(_T("%s-%s-%s"), pRsw->GetItem(_T("L1")), pRsw->GetItem(_T("L2")), pRsw->GetItem(_T("L3")));
			cmpLoc.Format(_T("%s-%s-%s"), pRsw->GetItem(_T("C1")), pRsw->GetItem(_T("C2")), pRsw->GetItem(_T("C3")));
			SetDlgItemText(IDC_LGLS_SC_CUR_VAL, curLoc);
			SetDlgItemText(IDC_LGLS_SC_CMP_VAL, cmpLoc);
		}
		delete pRsw;
	}

	if(m_pSC_DATA->V_ITN_LUGG_FK1 == _T("") || m_pSC_DATA->V_ITN_LUGG_FK1 == _T("0") || m_pSC_DATA->V_ITN_LUGG_FK1 == _T("0000"))
	{	// [LGLS 2026-08-01] 진행 작업 없음 : 작업상태 그룹 비우기(직전 작업 값 잔류 방지)
		m_bScvJobInDb = FALSE;
		SetDlgItemText(IDC_EDT_SC_JOB_JOB_NO, _T("-"));
		SetDlgItemText(IDC_CBX_SC_JOB_JOB_TYP, _T(""));
		SetDlgItemText(IDC_CBX_SC_JOB_JOB_STATUS, _T(""));
		SetDlgItemText(IDC_CBX_SC_JOB_START_POS, _T(""));
		SetDlgItemText(IDC_CBX_SC_JOB_DEST_POS, _T(""));
		SetDlgItemText(IDC_EDT_SC_JOB_START_LOC, _T(""));
		SetDlgItemText(IDC_EDT_SC_JOB_DEST_LOC, _T(""));
		SetDlgItemText(IDC_LGLS_SC_LOT_VAL, _T(""));
		SetDlgItemText(IDC_LGLS_SC_PRD_VAL, _T(""));
	}
	else
	{
		strSql = GetQrySelectJOB_MST_FK1(m_pSC_DATA);
		nRowCnt = -1;
		DWORD dwQ2 = ::GetTickCount();
		_RecordsetPtr ptr2 = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
		CLib::UiLog(_T("[SC] SQL2 JOB_MST %u ms rows=%d %s"), (unsigned)(::GetTickCount() - dwQ2), nRowCnt,
			strMessage.IsEmpty() ? _T("") : _T("ERR"));
		if(nRowCnt > 0)
		{
			CRecordSetWrap* pRsw2 = new CRecordSetWrap(ptr2);
			pRsw2->MoveFirst();
			SetDlgItemText(IDC_SCV_REQ_NO,    pRsw2->GetItem(_T("LUGG_NO")));
			SetDlgItemText(IDC_SCV_START_POS, pRsw2->GetItem(_T("START_POS")));
			SetDlgItemText(IDC_SCV_DEST_POS,  pRsw2->GetItem(_T("DEST_POS")));
			SetDlgItemText(IDC_SCV_PROD_ID,   pRsw2->GetItem(_T("PRODUCT_ID")));
			SetDlgItemText(IDC_SCV_BATCH_NO,  pRsw2->GetItem(_T("LOT_NO")));		// [LGLS 2026-10-01] 구 ECS 배치번호 = 적재 용기
			SetDlgItemText(IDC_SCV_REQ_NO2,   pRsw2->GetItem(_T("JOB_STATUS")));	// [LGLS 2026-10-01] 구 ECS 명령순번 = 작업상태
			m_bScvJobInDb = TRUE;
			// [LGLS 2026-08-01] 작업상태 그룹(rc 원래 항목)도 JOB_MST 값으로 채운다.
			SetDlgItemText(IDC_EDT_SC_JOB_JOB_NO,       pRsw2->GetItem(_T("LUGG_NO")));
			SetDlgItemText(IDC_CBX_SC_JOB_JOB_TYP,      pRsw2->GetItem(_T("JOB_TYP")));
			SetDlgItemText(IDC_CBX_SC_JOB_JOB_STATUS,   pRsw2->GetItem(_T("JOB_STATUS")));
			SetDlgItemText(IDC_CBX_SC_JOB_START_POS,    pRsw2->GetItem(_T("START_POS")));
			SetDlgItemText(IDC_CBX_SC_JOB_DEST_POS,     pRsw2->GetItem(_T("DEST_POS")));
			SetDlgItemText(IDC_EDT_SC_JOB_START_LOC,    pRsw2->GetItem(_T("START_LOCATION")));
			SetDlgItemText(IDC_EDT_SC_JOB_DEST_LOC,     pRsw2->GetItem(_T("DEST_LOCATION")));
			SetDlgItemText(IDC_LGLS_SC_LOT_VAL,         pRsw2->GetItem(_T("LOT_NO")));
			SetDlgItemText(IDC_LGLS_SC_PRD_VAL,         pRsw2->GetItem(_T("PRODUCT_ID")));
			delete pRsw2;
		}
		else
		{	// [LGLS 2026-08-01] JOB_MST 에 해당 작업이 없으면 이전 값이 남지 않도록 비운다
			m_bScvJobInDb = FALSE;
			SetDlgItemText(IDC_EDT_SC_JOB_JOB_NO,     _T("-"));
			SetDlgItemText(IDC_CBX_SC_JOB_JOB_TYP,    _T(""));
			SetDlgItemText(IDC_CBX_SC_JOB_JOB_STATUS, _T(""));
			SetDlgItemText(IDC_CBX_SC_JOB_START_POS,  _T(""));
			SetDlgItemText(IDC_CBX_SC_JOB_DEST_POS,   _T(""));
			SetDlgItemText(IDC_EDT_SC_JOB_START_LOC,  _T(""));
			SetDlgItemText(IDC_EDT_SC_JOB_DEST_LOC,   _T(""));
			SetDlgItemText(IDC_LGLS_SC_LOT_VAL,       _T(""));
			SetDlgItemText(IDC_LGLS_SC_PRD_VAL,       _T(""));
		}
	}
	// [LGLS 2026-10-01] 구 ECS 팝업의 안내문(labelAdvice) - 같은 문구, 같은 조건
	{
		BOOL bJob = !(m_strScvJob.IsEmpty() || m_strScvJob == _T("0") || m_strScvJob == _T("0000"));
		CString adv;
		if (!bJob)
			SetDlgItemText(IDC_SCV_IO_TAG, (m_strScvState == _T("0")) ? _T("에러") : (m_strScvState == _T("2")) ? _T("작업중") : _T("대기"));
		else if (!m_bScvJobInDb)      adv = _T("입/출고 요청번호가 DB에 없습니다.  [이상종료]처리하세요!!");
		else if (m_strScvState == _T("1")) adv = _T("입/출고 명령이 완료되지 않았습니다. [완료처리]하세요!!");
		else if (m_strScvState == _T("0")) adv = _T("크레인의 상태를 확인하고, [명령재전송] 또는 매뉴얼처리하세요!!");
		SetDlgItemText(IDC_SCV_ADVICE, adv);
	}
}

void CScSkinDlg::InvalidateLangControl( EN_LANG enLangTemp )
{ // 데이터가 아닌 레이블 / 버튼 등의 값만 수정시킬것
	if(m_pDoc == NULL)
		return;

}


LRESULT CScSkinDlg::OnMessagSwitch(WPARAM wParam, LPARAM lParam)
{
	CSC_DATA* pSC_DATA = (CSC_DATA*)wParam;
	if(pSC_DATA == NULL)
	{
		// [LGLS 2026-09-16] wParam NULL (OnRefreshDialog path) -> keep current selection, no modal warning
		if(m_pSC_DATA == NULL) return 0;
		pSC_DATA = m_pSC_DATA;
	}

	// [LGLS 2026-09-01] ★호기 전환 시 확대 패널의 주소 라벨을 다시 만든다★ (사용자 지적)
	//   이 창은 단일 창을 호기 전환으로 재사용하는데, 확대 패널의 주소 라벨
	//   (상태 D0160, M0310, D0320 ...)은 창 생성 때 1회만 만들어져 #1 것이
	//   모든 호기에서 그대로 보였다. 값은 호기별로 갱신되는데 주소만 낡았다.
	// [LGLS 2026-09-01 재수정] 패널은 OnInitDialog(데이터 도착 전, m_pSC_DATA=NULL)에서
	//   만들어져 항상 #1 기본 라벨이었다. NULL → 첫 설정도 "바뀜" 으로 봐야
	//   첫 데이터가 오는 순간 현재 호기 주소로 다시 그려진다.
	BOOL bScChanged = (m_pSC_DATA != pSC_DATA);
	m_pSC_DATA = pSC_DATA;
	if (bScChanged)
	{
		// [LGLS 2026-09-11] 현장에서 SC 창만 안 그려지는 건 - 이 재구성이 SC 에만 있다. 시간을 잰다.
		DWORD dwRb = ::GetTickCount();
		RebuildVehStatusPanel();
		CLib::UiLog(_T("[SC] REBUILD %u ms (ctrls=%d)"), (unsigned)(::GetTickCount() - dwRb), (int)m_arVehCtrl.GetCount());
	}

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

//사용안함
void CScSkinDlg::OnBnClickedBtnScManual()
{
	// [LGLS 2026-09-03] 리본 MANUAL > 크레인 과 같은 창(크레인 수동지시)을 연다
	if (m_pDoc != NULL) m_pDoc->OnCommandRangeMainFrameMANUAL(ID_MANUAL_SC);
}




void CScSkinDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 7) { CLib::MarqueeTick(this); return; }	// [LGLS 2026-09-21] EDIT 흐름 표시
	if (nIDEvent == 1 && m_pDoc != NULL)
		InvalidateScData(m_pDoc->m_enLang);
	CSkinDialog::OnTimer(nIDEvent);
}

void CScSkinDlg::OnClose()
{
	KillTimer(1);
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
		if (CLib::IsScEmptyErr(m_pSC_DATA->V_ERR_CODE_RD))
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 공출고 에러상태 입니다.\n에러해제 시 수동조작을 해야합니다.\n그래도 하시겠습니까?")), MB_YESNO) != IDYES)
				return;
		}

		if (CLib::IsScDualErr(m_pSC_DATA->V_ERR_CODE_RD))
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 이중입고 에러상태 입니다.\n에러해제 시 수동조작을 해야합니다.\n그래도 하시겠습니까?")), MB_YESNO) != IDYES)
				return;
		}
	}

	m_pDoc->BeginTrans_DLG();

	//공출고 일 때 삭제처리
	if(strCmdId == _T("DELFK1"))
	{
		if (CLib::IsScEmptyErr(m_pSC_DATA->V_ERR_CODE_RD))
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

	strSql.Format(_T(" UPDATE SC_DATA_LGLS						  \n")
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

	strSql.Format(_T(" UPDATE SC_DATA_LGLS		\n")
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
		      _T("        FROM SC_DATA_LGLS SC                                                    ")
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
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.PRODUCT_ID,'') AS PRODUCT_ID ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.LOT_NO,'') AS LOT_NO ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR,'N') AS JOB_TYP ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.BCR_TOP,'N') AS BCR_TOP ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.BCR_BOTTOM,'N') AS BCR_BOTTOM ");
	strSql += CRLF + _T("	   ,") + _T("'[' + JM.JOB_STATUS + '] ' + ") + m_pDoc->NVL + _T("(CCD_JOB_STATUS.CCD_NM_KOR,'N') AS JOB_STATUS ");
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
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.PRODUCT_ID,'') AS PRODUCT_ID ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.LOT_NO,'') AS LOT_NO ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR,'N') AS JOB_TYP ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.BCR_TOP,'N') AS BCR_TOP ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(JM.BCR_BOTTOM,'N') AS BCR_BOTTOM ");
	strSql += CRLF + _T("	   ,") + _T("'[' + JM.JOB_STATUS + '] ' + ") + m_pDoc->NVL + _T("(CCD_JOB_STATUS.CCD_NM_KOR,'N') AS JOB_STATUS ");
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
	strSql += CRLF + _T("  FROM SC_DATA_LGLS SD LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP");
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
	{
		// [LGLS 2026-09-10] 0행일 때도 래퍼를 지워 레코드셋을 닫는다(누수)
		delete pRsw;
		return FALSE;
	}

	pRsw->MoveFirst();
	//출고 HS 번호, 화물감지 유무
	strHS_MC_NO = pRsw->GetItem(_T("HS_MC_NO"));
	strSENSOR0_DATA_RD = pRsw->GetItem(_T("SENSOR0_DATA_RD"));

	//pRsw->MoveNext();
	delete pRsw;

	if (nRowCnt <= 0)
	{
		// [LGLS 2026-09-10] 0행일 때도 래퍼를 지워 레코드셋을 닫는다(누수)
		delete pRsw;
		return FALSE;
	}

	return TRUE;

}

BOOL CScSkinDlg::ScDataDelete()
{
	CString strSql = _T("");
	
	//해당 SC DATA 삭제
	strSql.Format(_T("UPDATE SC_DATA_LGLS 				\n")
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
	strSql.Format(_T("  SELECT RIGHT('0000' + CAST(ISNULL(MAX(CAST(LUGG_NO AS INT)), 8999) + 1 AS VARCHAR), 4) AS nextval FROM JOB_MST WHERE LUGG_NO LIKE '9[0-9][0-9][0-9]' ")); // [LGLS] SQL2008 has no sequences: MANUAL_SEQ -> MAX+1 in 9000 range

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
	{
		// [LGLS 2026-09-10] 0행일 때도 래퍼를 지워 레코드셋을 닫는다(누수)
		delete pRsw;
		return FALSE;
	}

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
	return;
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
	int nId = pWnd->GetDlgCtrlID();
	if(nId >= IDC_SCV_LED_LOAD_CMP && nId <= IDC_SCV_LED_ALARM_RESET_ACK)
	{
		CString t; pWnd->GetWindowText(t);
		if(t == _T("1"))
		{
			pDC->SetBkColor(RGB(0,200,0)); pDC->SetTextColor(RGB(0,200,0));
			if(m_brLedOn) return m_brLedOn;
		}
		else
		{
			pDC->SetBkColor(RGB(90,90,90)); pDC->SetTextColor(RGB(90,90,90));
			if(m_brLedOff) return m_brLedOff;
		}
	}
	else if(nId == IDC_SCV_STATUS)
	{
		// [LGLS 2026-10-01] 구 ECS 처럼 상태색 : DOWN 빨강 / IDLE 노랑 / RUN 초록 (글자 "2 = RUN" 의 첫 글자로 판정)
		static CBrush s_brRun(RGB(0,255,0)), s_brIdle(RGB(255,255,0)), s_brDown(RGB(255,0,0));
		CString t; pWnd->GetWindowText(t);
		BOOL bRun = (t.Find(_T("RUN")) >= 0), bIdle = (t.Find(_T("IDLE")) >= 0);
		COLORREF c = bRun ? RGB(0,255,0) : bIdle ? RGB(255,255,0) : RGB(255,0,0);
		pDC->SetBkColor(c); pDC->SetTextColor((bRun || bIdle) ? RGB(0,0,0) : RGB(255,255,255));	// 사진 : DOWN 은 빨강 바탕 흰 글자
		return bRun ? (HBRUSH)s_brRun : bIdle ? (HBRUSH)s_brIdle : (HBRUSH)s_brDown;
	}
	else if(nId == IDC_SCV_ADVICE)
	{
		pDC->SetTextColor(RGB(200,0,0)); pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	else if (nId == IDC_LGLS_ADDR_LBL)
	{
		// [LGLS 2026-08-06] 실주소 라벨은 파란 글씨(값이 안 변하므로 투명 배경 안전)
		pDC->SetTextColor(RGB(0,0,200));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	else if(nId == IDC_SCV_TITLE1 || nId == IDC_SCV_TITLE2)
	{
		pDC->SetTextColor(RGB(0,0,200));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));	// [LGLS 2026-08-05] 잔상 방지
		return ::GetSysColorBrush(COLOR_WINDOW);
	}
	else
	{
		// [LGLS 2026-08-05] CV 대화상자처럼 라벨 배경이 비치게 한다.
		//   여기 라벨들은 CStaticTransparent 로 묶여 있지 않아 기본 회색 브러시로 칠해졌다.
		//   위에서 걸러진 LED/상태/제목 컨트롤은 이 분기에 오지 않는다.
		if (nCtlColor == CTLCOLOR_STATIC && pWnd != NULL)
		{
			TCHAR szCls[32] = { 0 };
			::GetClassName(pWnd->GetSafeHwnd(), szCls, 31);
			if (_tcsicmp(szCls, _T("Static")) == 0)
			{
				// 텍스트용 STATIC 만 대상 - 프레임/아이콘/비트맵 스타일은 건드리지 않는다.
				DWORD dwType = pWnd->GetStyle() & SS_TYPEMASK;
				if (dwType == SS_LEFT || dwType == SS_CENTER || dwType == SS_RIGHT
				 || dwType == SS_SIMPLE || dwType == SS_LEFTNOWORDWRAP)
				{
					// [LGLS 2026-08-05] 값 상자(WS_BORDER)는 투명이면 글자가 바뀔 때 이전 글자가
					//   지워지지 않고 겹쳐 보인다(잔상). 불투명 흰 배경으로 지우면서 그린다.
					if (pWnd->GetStyle() & WS_BORDER)
					{
						pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
						pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
						return ::GetSysColorBrush(COLOR_WINDOW);
					}
					pDC->SetBkMode(TRANSPARENT);
					return (HBRUSH)::GetStockObject(NULL_BRUSH);
				}
			}
			// [LGLS 2026-08-05] 읽기전용 EDIT 는 기본값이 회색이라 일반 입력칸과 달라 보인다.
			//   CV 대화상자와 같이 값 칸 배경을 일반 입력칸과 같게 그린다.
			else if (_tcsicmp(szCls, _T("Edit")) == 0)
			{
				pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
				pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
				return ::GetSysColorBrush(COLOR_WINDOW);
			}
		}
		// [LGLS 2026-08-05] 그룹박스 캡션 배경도 라벨과 같이 비치게 한다.
		//   CSkinDialog 는 모든 CButton 에 단색 브러시를 돌려주어 캡션 자리에 네모가 생긴다.
		if (pWnd != NULL && (pWnd->GetStyle() & BS_TYPEMASK) == BS_GROUPBOX)
		{
			// [LGLS 2026-08-12] 완전 투명(NULL_BRUSH)이면 그룹 경계선이 캡션 글자를
			//   관통해 보인다. 바탕색과 같은 불투명 배경으로 글자 뒤의 선만 가린다
			//   (배경이 단색이라 시각적으로는 투명과 동일).
			pDC->SetBkMode(OPAQUE);
			pDC->SetBkColor(Global.GetRGB(IDX_RGB_BACKGROUND));
			return (HBRUSH)m_CtlColorBrush;
		}
	}
	return hbr;
}


// [LGLS 2026-08-01] SCV 통신상태 패널을 런타임에 생성한다.
//   배경: InvalidateXxData 가 IDC_SCV_* 컨트롤에 값을 쓰는데, Ecs.rc 의 대화상자 템플릿에는
//   그 컨트롤들이 하나도 없었다(리소스 편집 미완). SetDlgItemText/GetDlgItem 은 없는 ID 에 대해
//   조용히 실패하므로 "대화상자에 값이 하나도 안 나오는" 증상이 됐다.
//   → rc 를 손대는 대신 동일한 ID 로 컨트롤을 직접 만들어 기존 표시 코드가 그대로 동작하게 한다.
// [LGLS 2026-09-01] 확대 패널 재빌드 - 호기 전환 시 주소 라벨을 현재 호기 것으로.
void CScSkinDlg::RebuildVehStatusPanel()
{
	// [LGLS 2026-09-02] 호기 전환 반복 시 창이 길어지고 라벨이 겹치던 문제 - 먼저 접고 재구성한다.
	BOOL bWasExpanded = m_bVehExpanded;
	if (m_bVehExpanded) SetVehPanelExpanded(FALSE);
	for (int i = 0; i < m_arVehCtrl.GetCount(); i++)
	{
		CWnd* p = (CWnd*)m_arVehCtrl.GetAt(i);
		if (p != NULL) { if (::IsWindow(p->m_hWnd)) p->DestroyWindow(); delete p; }
	}
	m_arVehCtrl.RemoveAll();
	BuildVehStatusPanel();
	if (bWasExpanded) SetVehPanelExpanded(TRUE);   // 확대 중이었으면 새 컨트롤 표시
}

void CScSkinDlg::BuildVehStatusPanel()
{
	// [LGLS 2026-08-01] 다른 호기를 선택하면 대화상자 창이 다시 만들어져 런타임 컨트롤이 사라진다.
	//   배열에 남은 포인터의 창이 이미 없으면 정리하고 재생성한다([확대] 버튼/적재용기 행 소실 방지).
	if (m_arVehCtrl.GetCount() > 0)
	{
		CWnd* pFirst = (CWnd*)m_arVehCtrl.GetAt(0);
		if (pFirst != NULL && ::IsWindow(pFirst->GetSafeHwnd()))
			return;
		int i;
		for (i = 0; i < m_arVehCtrl.GetCount(); i++)  { CWnd* p = (CWnd*)m_arVehCtrl.GetAt(i);  if (p != NULL) delete p; }
		m_arVehCtrl.RemoveAll();
	}


	// [LGLS 2026-08-01] 도착위치 아래에 [적재 용기](JOB_MST.LOT_NO) / [제품 정보](JOB_MST.PRODUCT_ID) 두 행 추가
	// [LGLS 2026-09-08] 적재 용기 / 제품 정보는 이제 Ecs.rc 에 실물 컨트롤로 들어 있다.
	//   런타임 추가(CLib::AddTwoRowsBelow)는 rc 좌표를 기준으로 계산해서, rc 를 고칠 때마다
	//   두 행이 창 밖으로 밀려 사라졌다. rc 를 기준으로 삼기로 했으므로 호출하지 않는다.


	CRect rcCli; GetClientRect(&rcCli);
	CRect rcWin; GetWindowRect(&rcWin);

	// [LGLS 2026-10-01] ★구 ECS 설비 팝업(StackerForm / RGVForm)과 같은 배치★ (사용자 지시 - 내용 전부, 위치도 맞춘다)
	//   구 ECS 팝업 panel1(505x349) :
	//     panel2 (y 0~46)   : 상태(DOWN/IDLE/RUN 색) · 설비명 · 설명
	//     panel3 (y 46~126) : 신호 LED 4열 x 3행 + Pallet ID,  현재위치/출발지/도착지/완료위치(각 3칸) + 알람코드
	//     panel4 (y 124~285): 요청번호 ― 명령순번, 배치번호, 자재코드, 팔렛(+입고/출고), 출발위치, 도착위치, 안내문
	//                         오른쪽 [명령 재전송] [완료처리] [이상종료]
	//     panel5 (y 288~349): [사용금지] 체크, [확인]
	//   좌표는 구 ECS Designer 값을 그대로 쓰고, 패널 왼쪽 끝(nLeft)만 더한다.
	//   PLC 실주소(파랑)는 종전처럼 이름 옆에 작게 병기한다(사용자 요청 2026-08-05).
	const int PW    = 528;	// [LGLS 2026-10-01] 사진 배치 (영문 LED 이름이 길어 16 더 넓힘)
	const int STRIP = 30;
	int nLeft = rcCli.Width();
	if (m_nVehBaseH <= 0) m_nVehBaseH = rcWin.Height();
	if (m_nVehBaseW <= 0) m_nVehBaseW = rcWin.Width();
	m_nVehPanelW = PW;
	m_nVehPanelH = 0;
	UNREFERENCED_PARAMETER(STRIP);

	CFont* pFont = GetFont();
	struct MK {
		CWnd* dlg; CFont* font; CPtrArray* arr;
		void Add(CWnd* p) { if (font) p->SetFont(font); arr->Add(p); }
		void Label(LPCTSTR s, int x, int y, int w, int h, DWORD extra = 0) {
			CStatic* p = new CStatic();
			p->Create(s, WS_CHILD | WS_VISIBLE | SS_LEFT | extra, CRect(x, y, x + w, y + h), dlg);
			Add(p);
		}
		void LabelA(LPCTSTR nm, const CString& addr, int x, int y, int wN, int wA) {
			Label(nm, x, y, wN, 16);
			if (addr.IsEmpty() || wA <= 0) return;
			CStatic* p = new CStatic();
			p->Create(addr, WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(x + wN + 2, y, x + wN + 2 + wA, y + 16), dlg, IDC_LGLS_ADDR_LBL);
			Add(p);
		}
		void Value(int id, int x, int y, int w, int h, DWORD extra = 0) {
			CStatic* p = new CStatic();
			p->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | WS_BORDER | extra, CRect(x, y, x + w, y + h), dlg, id);
			Add(p);
		}
		void Led(int id, int x, int y) {
			CStatic* p = new CStatic();
			p->Create(_T("0"), WS_CHILD | WS_VISIBLE | SS_CENTER | WS_BORDER, CRect(x, y, x + 14, y + 13), dlg, id);
			Add(p);
		}
		void Button(int id, LPCTSTR s, int x, int y, int w, int h) {
			CButton* p = new CButton();
			p->Create(s, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_MULTILINE, CRect(x, y, x + w, y + h), dlg, id);
			Add(p);
		}
		void Check(int id, LPCTSTR s, int x, int y, int w, int h) {
			CButton* p = new CButton();
			p->Create(s, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, CRect(x, y, x + w, y + h), dlg, id);
			Add(p);
		}
	} mk = { this, pFont, &m_arVehCtrl };

	int X = nLeft + 4;				// 구 ECS panel1 의 원점
	CString strOwner = _T("VEHICLE:11");
	if (m_pSC_DATA != NULL)
		strOwner.Format(_T("VEHICLE:1%d"), CConvert::ToInt(m_pSC_DATA->K_SC_NO) % 10);   // 901→11

	// [LGLS 2026-09-12] 완료 Ack [쓰기] 버튼 글자 (rc_resource\dlg_xx\dlg_xx.ini  ackwrite=)
	CString strAckW = _T("쓰기");
	{
		TCHAR _cfA[MAX_PATH] = {0}; GetModuleFileName(NULL, _cfA, MAX_PATH);
		CString _apA = _cfA;
		CString _fpA = Global.GetConcatPath(_apA.Left(_apA.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), _T(".ini"));
		CString _sA = CLib::GetIniStringFromPath(_fpA, _T("ackwrite"), (int)((m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang));
		if (!_sA.IsEmpty()) strAckW = _sA;
	}
	// ── [LGLS 2026-10-01] ★구 ECS 실행 화면(사진) 배치★ : [Stacker Crane 정보] 폼 그대로 (사용자 지시 - 컨트롤 위치와 순서)
	//   위   : [DOWN] 상태칸(빨강/노랑/초록) + 설비명(파랑, 가운데) + 설명(파랑, 가운데)
	//   LED  : 4열 x 3행 (영문 이름) - Load Complete / Load Complete ACK / Unload Complete / Unload Complete ACK
	//                                  Transfer Request / Transfer Request ACK / Pallet Exist / Pallet ID [값]
	//                                  Alarm Set / Alarm Set ACK / Alarm Reset / Alarm Reset ACK
	//   위치 : 현재위치 □□□ 출발지 □□□ 도착지 □□□ 완료위치 □□□ 알람코드 □
	//   틀   : 요청번호 □ ― □ / 배치번호 / 자재코드 / 팔렛 □ [에러] / 출발위치 / 도착위치    오른쪽 [명령 재전송] [이상종료]
	//   아래 : [사용금지]  [확인]  (+ 완료 Ack [쓰기] 두 개는 오른쪽 빈 자리)
	{
		// 글자 없는 CStatic (테두리 없음)
		struct TX { CWnd* dlg; CFont* f; CPtrArray* a;
			void T(int id, LPCTSTR s, int x, int y, int w, int h, DWORD st) {
				CStatic* p = new CStatic(); p->Create(s, WS_CHILD | WS_VISIBLE | st, CRect(x, y, x + w, y + h), dlg, id);
				if (f) p->SetFont(f); a->Add(p); } } tx = { this, pFont, &m_arVehCtrl };
		// 틀(그룹 상자) - 먼저 만들어 뒤에 깔린다
		{ CStatic* g = new CStatic(); g->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME, CRect(X + 8, 138, X + 512, 300), this, 0); mk.Add(g); }

		// 상태 / 설비명 / 설명
		mk.Value(IDC_SCV_STATUS, X + 10, 4, 52, 36, SS_CENTER);
		tx.T(IDC_SCV_TITLE1, _T(""), X + 80, 4,  430, 22, SS_CENTER);
		{
			static CFont s_fntTitle;
			if (s_fntTitle.GetSafeHandle() == NULL) { LOGFONT lf = {0}; if (pFont) pFont->GetLogFont(&lf); lf.lfHeight = -16; lf.lfWeight = FW_BOLD; s_fntTitle.CreateFontIndirect(&lf); }
			CWnd* pT = GetDlgItem(IDC_SCV_TITLE1); if (pT && s_fntTitle.GetSafeHandle()) pT->SetFont(&s_fntTitle);
		}
		tx.T(IDC_SCV_TITLE2, _T(""), X + 80, 26, 430, 17, SS_CENTER);

		// LED 4열 x 3행
		static const int LX[4] = { 8, 140, 266, 392 };
		static const int LY[3] = { 52, 68, 84 };
		struct LEDDEF2 { LPCTSTR name; int id; int col; int row; };
		LEDDEF2 leds[] = {
			{ _T("Load Complete"),        IDC_SCV_LED_LOAD_CMP,        0, 0 },
			{ _T("Load Complete ACK"),    IDC_SCV_LED_LOAD_CMP_ACK,    1, 0 },
			{ _T("Unload Complete"),      IDC_SCV_LED_UNLOAD_CMP,      2, 0 },
			{ _T("Unload Complete ACK"),  IDC_SCV_LED_UNLOAD_CMP_ACK,  3, 0 },
			{ _T("Transfer Request"),     IDC_SCV_LED_TR_REQ,          0, 1 },
			{ _T("Transfer Request ACK"), IDC_SCV_LED_TR_REQ_ACK,      1, 1 },
			{ _T("Pallet Exist"),         IDC_SCV_LED_PALLET_EXIST,    2, 1 },
			{ _T("Alarm Set"),            IDC_SCV_LED_ALARM_SET,       0, 2 },
			{ _T("Alarm Set ACK"),        IDC_SCV_LED_ALARM_SET_ACK,   1, 2 },
			{ _T("Alarm Reset"),          IDC_SCV_LED_ALARM_RESET,     2, 2 },
			{ _T("Alarm Reset ACK"),      IDC_SCV_LED_ALARM_RESET_ACK, 3, 2 },
		};
		for (int i = 0; i < (int)(sizeof(leds)/sizeof(leds[0])); i++)
		{
			int x = X + LX[leds[i].col], yy = LY[leds[i].row];
			mk.Led(leds[i].id, x, yy);
			mk.Label(leds[i].name, x + 17, yy - 1, 114, 15);
		}
		mk.Label(_T("Pallet ID"), X + 392, LY[1] - 1, 44, 15);
		mk.Value(IDC_SCV_PALLET_ID, X + 438, LY[1] - 2, 80, 16);

		// 위치 한 줄
		{
			const int PY = 104;
			struct TRIO { LPCTSTR name; int a, b, c; int lx, vx; };
			TRIO trios[] = {
				{ _T("현재위치"), IDC_SCV_CUR1,  IDC_SCV_CUR2,  IDC_SCV_CUR3,  8,   52 },
				{ _T("출발지"),   IDC_SCV_FROM1, IDC_SCV_FROM2, IDC_SCV_FROM3, 114, 148 },
				{ _T("도착지"),   IDC_SCV_TO1,   IDC_SCV_TO2,   IDC_SCV_TO3,   214, 250 },
				{ _T("완료위치"), IDC_SCV_CMP1,  IDC_SCV_CMP2,  IDC_SCV_CMP3,  316, 358 },
			};
			for (int i = 0; i < (int)(sizeof(trios)/sizeof(trios[0])); i++)
			{
				mk.Label(trios[i].name, X + trios[i].lx, PY - 1, 42, 15);
				mk.Value(trios[i].a, X + trios[i].vx,      PY, 16, 14, SS_CENTER);
				mk.Value(trios[i].b, X + trios[i].vx + 17, PY, 16, 14, SS_CENTER);
				mk.Value(trios[i].c, X + trios[i].vx + 34, PY, 16, 14, SS_CENTER);
			}
			mk.Label(_T("알람코드"), X + 414, PY - 1, 46, 15);
			mk.Value(IDC_SCV_ALARM_CODE, X + 462, PY, 38, 14, SS_CENTER);
		}

		// 틀 안 : 요청번호 ~ 도착위치
		const int R0 = 152, RH = 21;
		LPCTSTR nms[6] = { _T("요청번호"), _T("배치번호"), _T("자재코드"), _T("팔렛"), _T("출발위치"), _T("도착위치") };
		for (int i = 0; i < 6; i++) mk.Label(nms[i], X + 30, R0 + i * RH + 2, 52, 16, SS_RIGHT);
		mk.Value(IDC_SCV_REQ_NO,    X + 88, R0,          110, 18);
		mk.Label(_T("―"),       X + 204, R0 + 1,     18, 16, SS_CENTER);
		mk.Value(IDC_SCV_REQ_NO2,   X + 226, R0,         142, 18);
		mk.Value(IDC_SCV_BATCH_NO,  X + 88, R0 + RH,     280, 18);
		mk.Value(IDC_SCV_PROD_ID,   X + 88, R0 + 2 * RH, 280, 18);
		mk.Value(IDC_SCV_PALLET,    X + 88, R0 + 3 * RH, 204, 18);
		mk.Value(IDC_SCV_IO_TAG,    X + 296, R0 + 3 * RH, 72, 18, SS_CENTER);
		mk.Value(IDC_SCV_START_POS, X + 88, R0 + 4 * RH, 280, 18);
		mk.Value(IDC_SCV_DEST_POS,  X + 88, R0 + 5 * RH, 280, 18);
		tx.T(IDC_SCV_ADVICE, _T(""), X + 14, R0 + 6 * RH + 3, 356, 16, SS_LEFT);
		mk.Button(IDC_SCV_RESEND,    _T("명령 재전송"), X + 386, 150, 120, 72);
		mk.Button(IDC_SCV_BTN_ABORT, _T("이상종료"),    X + 386, 236, 120, 50);

		// 아래 : 사용금지 / 확인 / 완료 Ack [쓰기]
		mk.Check(IDC_SCV_CHK_DISABLE, _T("사용금지"), X + 30, 332, 76, 16);
		mk.Button(IDC_SCV_OK, _T("확인"), X + 214, 314, 86, 48);
		mk.Button(IDC_SCV_BTN_LCA_W, _T("Load ACK ") + strAckW,   X + 330, 318, 86, 22);
		mk.Button(IDC_SCV_BTN_UCA_W, _T("Unload ACK ") + strAckW, X + 420, 318, 96, 22);
		// PLC 실주소는 자리가 없어 맨 아래 한 줄 (파랑)
		CString strAddrLine;
		strAddrLine.Format(_T("LoadCmp %s / UnloadCmp %s / TrReq %s / Pallet %s / Alarm %s / 위치 %s / 출발 %s / 도착 %s / 완료 %s / 알람코드 %s"),
			(LPCTSTR)CLib::GetObsAddr(strOwner, _T("LOAD_COMPLETE")), (LPCTSTR)CLib::GetObsAddr(strOwner, _T("UNLOAD_COMPLETE")),
			(LPCTSTR)CLib::GetObsAddr(strOwner, _T("TRANSFER_REQUEST")), (LPCTSTR)CLib::GetObsAddr(strOwner, _T("PALLET_EXIST_FLAG")),
			(LPCTSTR)CLib::GetObsAddr(strOwner, _T("ALARM_SET_REPORT")),
			(LPCTSTR)CLib::GetObsAddr(strOwner, _T("SUBSYSTEM_LOCATION_01")), (LPCTSTR)CLib::GetObsAddr(strOwner, _T("FROM_01")),
			(LPCTSTR)CLib::GetObsAddr(strOwner, _T("TO_01")), (LPCTSTR)CLib::GetObsAddr(strOwner, _T("TRANSFER_COMPLETE_LOCATION_01")),
			(LPCTSTR)CLib::GetObsAddr(strOwner, _T("ALARM_SET_CODE")));
		CStatic* pA = new CStatic();
		pA->Create(strAddrLine, WS_CHILD | WS_VISIBLE | SS_LEFT | SS_ENDELLIPSIS, CRect(X + 8, 346, X + 518, 360), this, IDC_LGLS_ADDR_LBL);
		mk.Add(pA);
	}

	RenameRuntimeLabels();	// [LGLS 2026-08-05]
	MakeGroupBoxesTransparent();	// [LGLS 2026-08-05]
	SetVehPanelExpanded(FALSE);		// 기본은 축소(원래 DLG 만)
}

// [LGLS 2026-08-01] 상태 패널 표시/숨김 + 창 크기 전환.
//   확대 : 원래 DLG + 통신상태 패널(이전 창 내용) / 축소 : 원래 DLG 만
void CScSkinDlg::SetVehPanelExpanded(BOOL bExpand)
{
	m_bVehExpanded = bExpand;
	for (int i = 0; i < m_arVehCtrl.GetCount(); i++)
	{
		CWnd* p = (CWnd*)m_arVehCtrl.GetAt(i);
		if (p != NULL && ::IsWindow(p->m_hWnd)) p->ShowWindow(bExpand ? SW_SHOW : SW_HIDE);
	}
	CRect rc; GetWindowRect(&rc);
	// [LGLS 2026-09-08] 세로가 아니라 가로를 늘린다.
	if (m_nVehBaseW <= 0) m_nVehBaseW = rc.Width();
	SetWindowPos(NULL, 0, 0, m_nVehBaseW + (bExpand ? m_nVehPanelW : 0), m_nVehBaseH,
	             SWP_NOMOVE | SWP_NOZORDER);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	if (bExpand)
		for (int i = 0; i < m_arVehCtrl.GetCount(); i++)
		{ CWnd* p = (CWnd*)m_arVehCtrl.GetAt(i); if (p && ::IsWindow(p->m_hWnd)) p->RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW); }
	if (bExpand)
	{
		// [LGLS 2026-08-06] 확대한 창이 모니터 작업영역보다 크면 하단이 화면 밖으로 잘린다
		//   (SC 에서 '테두리가 안 보인다'던 증상의 실체). 넘치는 만큼 위로 밀어 올린다.
		HMONITOR hMon = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi; ::ZeroMemory(&mi, sizeof(mi)); mi.cbSize = sizeof(mi);
		if (::GetMonitorInfo(hMon, &mi))
		{
			// [LGLS 2026-09-08] 오른쪽으로 펴므로 우측이 넘치면 왼쪽으로 밀어 넣는다.
			//   (세로 보정도 남겨 둔다 - 원래 창이 이미 화면보다 클 수 있다)
			CRect rcNow; GetWindowRect(&rcNow);
			int nLeftNew = rcNow.left, nTopNew = rcNow.top;
			int nOverR = rcNow.right - mi.rcWork.right;
			if (nOverR > 0) nLeftNew = rcNow.left - nOverR;
			if (nLeftNew < mi.rcWork.left) nLeftNew = mi.rcWork.left;
			int nOver = rcNow.bottom - mi.rcWork.bottom;
			if (nOver > 0) nTopNew = rcNow.top - nOver;
			if (nTopNew < mi.rcWork.top) nTopNew = mi.rcWork.top;
			if (nLeftNew != rcNow.left || nTopNew != rcNow.top)
			{
				SetWindowPos(NULL, nLeftNew, nTopNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			}
		}
	}
	if (::IsWindow(m_btnVehZoom.m_hWnd))
	{
		// [LGLS 2026-08-05] 버튼 문구도 다국어 ini 에서 읽는다(없으면 기본 한글).
		TCHAR chrFile[500]; GetModuleFileName(NULL, chrFile, MAX_PATH);
		CString strApp; strApp.Format(_T("%s"), chrFile);
		CString strPath = Global.GetConcatPath(strApp.Left(strApp.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\"), _T("dlg_sc"), _T(".ini"));
		EN_LANG enLang = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;
		CString strTxt = CLib::GetIniStringFromPath(strPath, bExpand ? _T("zoomout") : _T("zoomin"), (int)enLang);
		if (strTxt.IsEmpty()) strTxt = bExpand ? _T("축소") : _T("확대");
		m_btnVehZoom.SetWindowText(strTxt);
		CString strIco = strApp.Left(strApp.ReverseFind('\\')) + _T("\\rc_resource\\dlg_sc\\20x20\\")
		               + (bExpand ? _T("arrow-up.png") : _T("arrow-down.png"));
		m_btnVehZoom.SetIcon(Global.HICONFromPATH(strIco), NULL, 5, 5);
	}
}

void CScSkinDlg::OnBnClickedVehZoom()
{
	SetVehPanelExpanded(!m_bVehExpanded);
}

void CScSkinDlg::SetLed(int nId, CString strVal)
{
	CString t = (strVal == _T("1")) ? _T("1") : _T("0");
	SetDlgItemText(nId, t);
	CWnd* p = GetDlgItem(nId);
	if(p) p->Invalidate();
}

void CScSkinDlg::OnBnClickedScvResend()
{
	if(m_pSC_DATA == NULL) return;
	if(!m_pDoc->Permission(_T("CScSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	// [LGLS 2026-08-13] 구 ECS(StackerForm buttonRetry)와 동일: 진행 중 지시(ITN_LUGG_FK1)가
	//   없으면(IDLE) 재전송할 명령이 없으므로 안내만 하고 중단한다.
	{
		CString strChk = _T("SELECT ISNULL(ITN_LUGG_FK1,'') AS ITN FROM SC_DATA_LGLS WHERE WH_TYP = ")
		               + CLib::Quot(m_pDoc->m_WH_TYP)
		               + _T(" AND PLC_NO = ") + CLib::Quot(m_pSC_DATA->K_PLC_NO)
		               + _T(" AND MC_NO = ") + CLib::Quot(m_pSC_DATA->K_SC_NO);
		int nCnt = 0; CString strMsg;
		_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strChk, nCnt, strMsg);
		CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
		CString strItn;
		if (nCnt > 0) { pRsw->MoveFirst(); strItn = pRsw->GetItem(_T("ITN")); }
		delete pRsw;
		strItn.Trim();
		if (strItn.IsEmpty() || strItn == _T("0") || strItn == _T("0000"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("재전송 가능한 명령이 없습니다")));
			return;
		}
	}
	if(AfxMessageBox(m_pDoc->GetMsgLangDef(_T("명령을 재전송 하시겠습니까?")), MB_YESNO) != IDYES)
		return;
	CString strSql;
	strSql.Format(_T(" UPDATE SC_DATA_LGLS SET CMD_RQ_YN='Y' WHERE WH_TYP='%s' AND PLC_NO='%s' AND MC_NO='%s' "),
		m_pDoc->m_WH_TYP, m_pSC_DATA->K_PLC_NO, m_pSC_DATA->K_SC_NO);
	m_pDoc->BeginTrans_DLG();
	if(m_pDoc->ExcuteQueryString_DLG(strSql))
	{
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
	}
	else
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	}
}

void CScSkinDlg::OnBnClickedScvOk()
{
	m_pDoc->m_pScSkinDlg = NULL;
	delete this;
}

// [LGLS 2026-09-12] 확대 패널 [쓰기] : 완료 Ack 를 운전원이 직접 켜고/끈다 (상황 A 구제).
//   Client 는 PLC 에 직접 쓰지 않는다. SC_DATA_LGLS 의 명령 컬럼에 "ACKW:..=1|0" 을 남기면
//   WCS_TASK_CV 의 해당 스레드가 소비해 그 스레드가 쓴다 - 같은 Ack 워드를 만지는 다른 손이 없게 하고,
//   (S/C·RGV 는) 캐시를 함께 맞춰 설비가 보고를 내릴 때 규약대로 Ack 도 내리게 한다.
//   대상은 완료 Ack 두 종류뿐 - 이송지시·PLC→WCS 보고 비트는 절대 쓰지 않는다.
void CScSkinDlg::OnAckWrite(UINT nID)
{
	if (m_pDoc == NULL || m_pSC_DATA == NULL) return;
	if (!m_pDoc->Permission(_T("CScSkinDlg"), UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}
	BOOL bLoad = (nID == IDC_SCV_BTN_LCA_W);
	CString strName = m_pSC_DATA->V_MC_NO_NM;
	CString strOwner = CString(_T("")) ; strOwner.Format(_T("VEHICLE:1%d"), CConvert::ToInt(m_pSC_DATA->K_SC_NO) % 10);
	CString strObs = CString(bLoad ? _T("LOAD_COMPLETE_ACK") : _T("UNLOAD_COMPLETE_ACK")) + _T("");
	CString strRep = CString(bLoad ? _T("LOAD_COMPLETE") : _T("UNLOAD_COMPLETE")) + _T("");
	CString strSig; strSig.Format(_T("%s"), bLoad ? _T("LC") : _T("UC"));
	CString strCur;    GetDlgItemText(bLoad ? IDC_SCV_LED_LOAD_CMP_ACK : IDC_SCV_LED_UNLOAD_CMP_ACK, strCur);
	CString strRepNow; GetDlgItemText(bLoad ? IDC_SCV_LED_LOAD_CMP : IDC_SCV_LED_UNLOAD_CMP, strRepNow);

	// 다른 명령이 처리 대기 중이면 덮어쓰지 않는다
	{
		CString strChk;
		strChk.Format(_T(" SELECT CMD_RQ_ID, CMD_RQ_YN FROM SC_DATA_LGLS WHERE WH_TYP = '%s' AND PLC_NO = '%s' AND MC_NO = '%s' "), (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_pSC_DATA->K_PLC_NO, (LPCTSTR)m_pSC_DATA->K_SC_NO);
		int nRowCnt = -1; CString strMessage;
		_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strChk, nRowCnt, strMessage);
		if (nRowCnt > 0)
		{
			CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
			pRsw->MoveFirst();
			CString strPendId = pRsw->GetItem(_T("CMD_RQ_ID"));
			CString strPendYn = pRsw->GetItem(_T("CMD_RQ_YN"));
			delete pRsw;
			strPendId.Trim(); strPendYn.Trim();
			if (strPendYn == _T("Y") && strPendId.Left(4) != _T("ACKW"))
			{
				CString strBusy; strBusy.Format(_T("다른 명령(%s)이 처리 대기 중입니다. 잠시 후 다시 하세요."), (LPCTSTR)strPendId);
				AfxMessageBox(strBusy);
				return;
			}
		}
	}

	CString strMsg;
	strMsg.Format(_T("%s\n%s  (%s)\n\n현재 Ack = %s   /   설비 보고 %s = %s\n\n[예] = ON 으로 쓰기      [아니오] = OFF 로 쓰기      [취소]"),
		(LPCTSTR)strName, (LPCTSTR)strObs, (LPCTSTR)(CLib::GetObsAddr(strOwner, strObs).IsEmpty() ? CString(_T("주소 미상")) : CLib::GetObsAddr(strOwner, strObs)),
		(LPCTSTR)strCur, (LPCTSTR)strRep, (LPCTSTR)strRepNow);
	int nRet = AfxMessageBox(strMsg, MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON3);
	if (nRet != IDYES && nRet != IDNO) return;
	CString strCmd; strCmd.Format(_T("ACKW-%s=%d"), (LPCTSTR)strSig, (nRet == IDYES) ? 1 : 0);	// 구분자 '-' (':' 는 TASK DB 계층이 '@' 로 바꾼다)

	CString strSql;
	strSql.Format(_T(" UPDATE SC_DATA_LGLS SET CMD_RQ_ID = '%s', CMD_RQ_YN = 'Y' WHERE WH_TYP = '%s' AND PLC_NO = '%s' AND MC_NO = '%s' AND (CMD_RQ_YN <> 'Y' OR CMD_RQ_ID LIKE 'ACKW%%') "),
		(LPCTSTR)strCmd, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_pSC_DATA->K_PLC_NO, (LPCTSTR)m_pSC_DATA->K_SC_NO);
	if (m_pDoc->ExcuteQueryString_DLG(strSql) != TRUE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return;
	}
	CString strLog; strLog.Format(_T("Ack 수동 쓰기 %s -> %s (%s %s)"), (LPCTSTR)strName, (LPCTSTR)strCmd, (LPCTSTR)strObs, (LPCTSTR)CLib::GetObsAddr(strOwner, strObs));
	m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), m_pSC_DATA->V_ITN_LUGG_FK1, _T(""), _T(""), strLog);
}

// [LGLS 2026-09-12] Ecs.ini [MENU] ZOOM_BTN=1/0 → [확대] 를 표시/숨김.
//   창을 만들 때(RedrawImage)와 CEcsView 가 ini 저장을 감지했을 때 부른다. 숨길 때 패널이 펼쳐져 있으면 먼저 접는다.
//   [LGLS 2026-09-17] [확대] 는 rc 에서 명령 버튼 맨 아래에 있으므로 숨겨도 빈틈이 생기지 않는다(버튼 재배치 없음).
void CScSkinDlg::ApplyZoomBtnIni()
{
	if (GetSafeHwnd() == NULL) return;
	BOOL bZoom = (::GetPrivateProfileInt(_T("MENU"), _T("ZOOM_BTN"), 1, ECS_INI_FILE) != 0);
	if (!bZoom && m_bVehExpanded) SetVehPanelExpanded(FALSE);
	CWnd* pZ = GetDlgItem(IDC_LGLS_SC_ZOOM);
	if (pZ) pZ->ShowWindow(bZoom ? SW_SHOW : SW_HIDE);

	// [LGLS 2026-09-18] [수동지시] 표시 여부 - Ecs.ini [MENU] SC_MANUAL_BTN=1/0 (기본 1)
	//   숨겨도 다른 버튼은 옮기지 않는다(자리는 rc 가 정한다). 그 자리는 빈칸으로 남는다.
	BOOL bManual = FALSE;	// [LGLS 2026-09-28] 수동지시 폐기 - 늘 숨긴다 (사용자 지시)
	CWnd* pM = GetDlgItem(IDC_BTN_SC_MANUAL);
	if (pM) pM->ShowWindow(bManual ? SW_SHOW : SW_HIDE);
}

// ===================================================================
// [LGLS 2026-10-01] 구 ECS StackerForm 의 [완료처리] / [이상종료] / [사용금지] (사용자 지시)
//   완료처리 : 설비가 물고 있는 작업을 JOB_MST 에서 SC 반송완료(29)로 올린다 - 판넬의 강제완료와 같다.
//   이상종료 : 설비에 내려간 지시(OD)를 비워 설비-명령 연결을 리셋한다. 작업정보는 두지 않는다. 구 ECS 와 같이 RUN/DOWN 에서는 거부.
//   사용금지 : 이 호기의 [작업금지](입출고 금지) 와 같다 - 기존 버튼 처리로 보낸다.
// ===================================================================
static BOOL LglsScExecUpdate(CEcsDoc* pDoc, const CString& strSql, const CString& strLog, CString strLugg, LPCTSTR strWid)
{
	if (pDoc == NULL) return FALSE;
	if (!pDoc->Permission(strWid, UPD_YN)) { AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다"))); return FALSE; }
	if (pDoc->BeginTrans_DLG() < 1) return FALSE;
	if (strLugg.IsEmpty()) strLugg = _T("0");
	if (!pDoc->GetQueryInsertClientLog(strWid, strLugg, _T(""), _T(""), strLog)) { pDoc->RollbackTrans_DLG(); return FALSE; }
	if (!pDoc->ExcuteQueryString_DLG(strSql)) { pDoc->RollbackTrans_DLG(); AfxMessageBox(pDoc->GetMsgLangDef(_T("실패"))); return FALSE; }
	pDoc->CommitTrans_DLG();
	return TRUE;
}

void CScSkinDlg::OnScvForce()
{
	if (m_pDoc == NULL || m_pSC_DATA == NULL) return;
	if (m_strScvState == _T("2")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("RUN(작업중) 상태입니다. 작업이 완료될때 까지 기다리십시오."))); return; }
	if (m_strScvJob.IsEmpty() || m_strScvJob == _T("0") || m_strScvJob == _T("0000")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("[완료처리]할 명령이 없습니다."))); return; }
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("강제완료 하시겠습니까?")) + _T(" [") + m_strScvJob + _T(" -> 29]"), MB_YESNO) != IDYES) return;
	CString strSql;
	strSql.Format(_T("UPDATE JOB_MST SET JOB_STATUS = '29', UPD_DT = GETDATE() WHERE WH_TYP = '%s' AND LUGG_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_strScvJob);
	if (LglsScExecUpdate(m_pDoc, strSql, _T("JOB_MST UPDATE : 완료처리(구ECS) JOB_STATUS -> 29"), m_strScvJob, _T("CScSkinDlg")))
		AfxMessageBox(_T("[ ") + m_strScvJob + _T(" ] 명령을 완료하였습니다."));
}

void CScSkinDlg::OnScvAbort()
{
	if (m_pDoc == NULL || m_pSC_DATA == NULL) return;
	if (m_strScvState == _T("2")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("RUN(작업중) 상태입니다. 작업완료후 [이상종료]처리를 하십시오."))); return; }
	if (m_strScvState == _T("0")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("DOWN(작업중단) 상태에서는 [이상종료]처리를 할 수 없습니다."))); return; }
	if (m_strScvJob.IsEmpty() || m_strScvJob == _T("0") || m_strScvJob == _T("0000")) { AfxMessageBox(m_pDoc->GetMsgLangDef(_T("[이상종료]처리할 명령이 없습니다."))); return; }
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("이상종료 하시겠습니까? (지시 연결 해제, 작업정보는 유지)")) + _T(" [") + m_pSC_DATA->K_SC_NO + _T("]"), MB_YESNO) != IDYES) return;
	CString strSql;
	strSql.Format(_T("UPDATE SC_DATA_LGLS SET PALLET_ID_OD = '0000'")
		_T(", FROM_01_OD = '00', FROM_02_OD = '00', FROM_03_OD = '00'")
		_T(", TO_01_OD = '00', TO_02_OD = '00', TO_03_OD = '00'")
		_T(", OD_RQ_YN = 'N'")
		_T(" WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_pSC_DATA->K_SC_NO);
	if (LglsScExecUpdate(m_pDoc, strSql, CString(_T("이상종료(구ECS) : 지시(OD) 해제 (")) + m_pSC_DATA->K_SC_NO + _T(")"), _T("0"), _T("CScSkinDlg")))
		AfxMessageBox(_T("[이상종료]처리를 완료하였습니다."));
}

void CScSkinDlg::OnScvDisable()
{
	// 체크 상태는 다음 갱신 때 SUSPEND 값으로 다시 맞춰진다 - 여기서는 기존 [작업금지] 처리만 부른다
	CWnd* pBtn = GetDlgItem(IDC_BTN_SC_ALL_SUSPEND);
	if (pBtn == NULL) return;
	SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_SC_ALL_SUSPEND, BN_CLICKED), (LPARAM)pBtn->GetSafeHwnd());
}
