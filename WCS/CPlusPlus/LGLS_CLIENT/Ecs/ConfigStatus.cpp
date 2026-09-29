// ConfigStatus.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "ConfigStatus.h"
#include "afxdialogex.h"
#include "EcsView.h"


// CConfigStatus 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConfigStatus, CDialogEx)

// [LGLS 2026-09-29] [숨김] 체크박스 - 순서는 Config::g_szLegendHideKey 와 같다 (사용자 지시)
const UINT CConfigStatus::m_nChkHideId[12] =
{
	IDC_CHK_HIDE_STO,
	IDC_CHK_HIDE_RET,
	IDC_CHK_HIDE_MOVE,
	IDC_CHK_HIDE_STN_STO,
	IDC_CHK_HIDE_STN_RET,
	IDC_CHK_HIDE_SUSPEND,
	IDC_CHK_HIDE_ERR,
	IDC_CHK_HIDE_MANUAL,
	IDC_CHK_HIDE_SEARCH,
	IDC_CHK_HIDE_SEMI_STO,
	IDC_CHK_HIDE_SEMI_RET,
	IDC_CHK_HIDE_SEMI_MOVE,
};


CConfigStatus::CConfigStatus(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CConfigStatus::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CConfigStatus::CConfigStatus(CEcsDoc* pDoc, CWnd* pParent)
	: CSkinDialog(CConfigStatus::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
}

CConfigStatus::~CConfigStatus()
{
}

void CConfigStatus::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRP_CV_JOB_STATUS, m_grpCvJobStatus);
	DDX_Control(pDX, IDC_GRP_CV_STATUS, m_grpCvStatus);
	DDX_Control(pDX, IDC_GRP_SC_STATUS, m_grpScStatus);
	DDX_Control(pDX, IDC_GRP_STATUS, m_grpStatus);

	DDX_Control(pDX, IDC_LBL_CV_STO, m_lblCvSto);
	DDX_Control(pDX, IDC_LBL_CV_RET, m_lblCvRet);
	DDX_Control(pDX, IDC_LBL_CV_MOVE, m_lblCvMove);

	DDX_Control(pDX, IDC_LBL_CV_STO_READY, m_lblCvStoReady);
	DDX_Control(pDX, IDC_LBL_CV_RET_READY, m_lblCvRetReady);
	DDX_Control(pDX, IDC_LBL_CV_SUSPEND, m_lblCvSuspend);

	DDX_Control(pDX, IDC_LBL_ERR, m_lblErr);
	DDX_Control(pDX, IDC_LBL_MANUAL, m_lblManual);

	DDX_Control(pDX, IDC_LBL_CV_SEARCH, m_lblSearch);


	DDX_Control(pDX, IDC_BTN_AUTO_STO	, m_btnAutoSto		);
	DDX_Control(pDX, IDC_BTN_AUTO_RET	, m_btnAutoRet		);
	DDX_Control(pDX, IDC_BTN_AUTO_MOV	, m_btnAutoMove		);
	DDX_Control(pDX, IDC_BTN_STN_STO	, m_btnStnSto		);
	DDX_Control(pDX, IDC_BTN_STN_RET	, m_btnStnRet		);
	DDX_Control(pDX, IDC_BTN_SUSPEND	, m_btnSuspend		);
	DDX_Control(pDX, IDC_BTN_ERR		, m_btnErr			);
	DDX_Control(pDX, IDC_BTN_MANUAL		, m_btnManual		);
	DDX_Control(pDX, IDC_BTN_CV_SEARCH	, m_btnCvSearch		);
	// [LGLS 2026-07-19] 반자동 작업 색상
	DDX_Control(pDX, IDC_GRP_SEMI_STATUS, m_grpSemiStatus);
	DDX_Control(pDX, IDC_LBL_SEMI_STO , m_lblSemiSto );
	DDX_Control(pDX, IDC_LBL_SEMI_RET , m_lblSemiRet );
	DDX_Control(pDX, IDC_LBL_SEMI_MOVE, m_lblSemiMove);
	DDX_Control(pDX, IDC_BTN_SEMI_STO , m_btnSemiSto );
	DDX_Control(pDX, IDC_BTN_SEMI_RET , m_btnSemiRet );
	DDX_Control(pDX, IDC_BTN_SEMI_MOVE, m_btnSemiMove);

	// [LGLS 2026-09-29] 숨김 체크박스 12개 (사용자 지시)
	for (int nLh = 0; nLh < 12; nLh++)
		DDX_Control(pDX, m_nChkHideId[nLh], m_chkHide[nLh]);
}


BEGIN_MESSAGE_MAP(CConfigStatus, CSkinDialog)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CConfigStatus::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_INIT_COLOR, &CConfigStatus::OnBnClickedInitColor)
END_MESSAGE_MAP()


// CConfigStatus 메시지 처리기입니다.


void CConfigStatus::OnClose()
{
	m_pDoc->m_pConfigStatus =  NULL;
	CSkinDialog::OnClose();
}


BOOL CConfigStatus::OnInitDialog()
{
	m_bDisableMaximize = TRUE;	// [LGLS 2026-09-27] 최대화 버튼 제거 + 크기조절 금지 (사용자 지시)
	CSkinDialog::OnInitDialog();
	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{	
		LoadColor();
		m_bInitialized = TRUE;		
	}

	
	Invalidate(TRUE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CConfigStatus::LoadColor()
{
	m_btnAutoSto	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STO			: LIGHT_GRAY;
	m_btnAutoRet	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_RET			: LIGHT_GRAY;
	m_btnAutoMove	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_MOVE		: LIGHT_GRAY;
	m_btnStnSto		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STN_STO		: LIGHT_GRAY;
	m_btnStnRet		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STN_RET		: LIGHT_GRAY;
	m_btnSuspend	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SUSPEND		: LIGHT_GRAY;
	m_btnErr		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_ERROR		: LIGHT_GRAY;
	m_btnManual		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_MANUAL		: LIGHT_GRAY;
	// [LGLS 2026-09-13] 이 현장에는 WC 패스 모드가 없어 비활성이던 칸(09-03)을 "작업번호 있음" 색 편집으로 쓴다(사용자 지시).
	//   화물 없이 작업번호(트래킹)만 남은 트랙 색 = USER_COLOR_TRACKING (메인 범례 "작업번호 있음" 과 같은 값)
	m_btnCvSearch	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_CV_SEARCH	: LIGHT_GRAY;
	// [LGLS 2026-07-19] 반자동 작업 색상
	m_btnSemiSto	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_STO		: LIGHT_GRAY;
	m_btnSemiRet	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_RET		: LIGHT_GRAY;
	m_btnSemiMove	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_MOVE	: LIGHT_GRAY;

	// [LGLS 2026-09-29] 숨김 체크 상태도 같이 읽는다 (사용자 지시)
	if (m_pDoc != NULL && m_pDoc->m_pConfig != NULL)
		for (int nLh = 0; nLh < 12; nLh++)
			if (m_chkHide[nLh].GetSafeHwnd() != NULL)
				m_chkHide[nLh].SetCheck(m_pDoc->m_pConfig->m_bLEGEND_HIDE[nLh] ? BST_CHECKED : BST_UNCHECKED);

	return;
}

void CConfigStatus::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) 
	{
		m_pDoc->m_pConfig->m_clrUSER_COLOR_STO			= m_btnAutoSto.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_RET			= m_btnAutoRet.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_MOVE			= m_btnAutoMove.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_STN_STO		= m_btnStnSto.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_STN_RET		= m_btnStnRet.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SUSPEND		= m_btnSuspend.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_ERROR		= m_btnErr.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_MANUAL		= m_btnManual.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_CV_SEARCH	= m_btnCvSearch.m_crColor;
		// [LGLS 2026-07-19] 반자동 작업 색상
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_STO		= m_btnSemiSto.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_RET		= m_btnSemiRet.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_MOVE	= m_btnSemiMove.m_crColor;

		
		// [LGLS 2026-09-29] 숨김 체크 상태를 함께 저장한다 (사용자 지시)
		for (int nLh = 0; nLh < 12; nLh++)
			if (m_chkHide[nLh].GetSafeHwnd() != NULL)
				m_pDoc->m_pConfig->m_bLEGEND_HIDE[nLh] = (m_chkHide[nLh].GetCheck() == BST_CHECKED);

		m_pDoc->m_pConfig->SaveConfigUSER();

		// [LGLS 2026-09-09] 레이아웃에 그려 둔 범례 견본도 바뀐 색으로 다시 칠한다.
		//   (검색 색을 노랑으로 바꾸면 메인 화면 범례의 [검색] 칸도 곧바로 노랑이 된다)
		m_pDoc->ApplyLegendColors();
		{
			CWnd* pView = m_pDoc->GetViewObject();
			// [LGLS 2026-09-13] 새 메인 화면의 왼쪽 범례 칸(뷰의 자식)까지 다시 그린다
			if (pView != NULL) pView->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
		}

		Invalidate(TRUE);

		// 레이아웃 수정
		m_pDoc->m_pEquipments.InvokeControl(TRUE);
	}
}


void CConfigStatus::OnBnClickedInitColor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) 
		m_pDoc->m_pConfig->InitializeConfigUSER();	

	LoadColor();
	Invalidate(TRUE);

	// 레이아웃 수정
	m_pDoc->m_pEquipments.InvokeControl(TRUE);
}
