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
	DDX_Control(pDX, IDC_GRP_RAIL_STATUS, m_grpRailStatus);
	DDX_Control(pDX, IDC_GRP_SC_STATUS, m_grpScStatus);
	DDX_Control(pDX, IDC_GRP_STATUS, m_grpStatus);

	DDX_Control(pDX, IDC_LBL_CV_STO, m_lblCvSto);
	DDX_Control(pDX, IDC_LBL_CV_RET, m_lblCvRet);
	DDX_Control(pDX, IDC_LBL_CV_MOVE, m_lblCvMove);
	DDX_Control(pDX, IDC_LBL_CV_RTR, m_lblCvRtr);
	DDX_Control(pDX, IDC_LBL_CV_ATA, m_lblCvAta);
	DDX_Control(pDX, IDC_LBL_CV_WTW, m_lblCvWtw);

	DDX_Control(pDX, IDC_LBL_CV_STO_READY, m_lblCvStoReady);
	DDX_Control(pDX, IDC_LBL_CV_RET_READY, m_lblCvRetReady);
	DDX_Control(pDX, IDC_LBL_CV_STO_HS, m_lblCvStoHs);
	DDX_Control(pDX, IDC_LBL_CV_RET_HS, m_lblCvRetHs);
	DDX_Control(pDX, IDC_LBL_CV_SUSPEND, m_lblCvSuspend);

	DDX_Control(pDX, IDC_LBL_ERR, m_lblErr);
	DDX_Control(pDX, IDC_LBL_MANUAL, m_lblManual);
	DDX_Control(pDX, IDC_LBL_DISCONNECT, m_lblDisConnect);

	DDX_Control(pDX, IDC_LBL_CV_WC_PASS, m_lblCvWcPass);
	DDX_Control(pDX, IDC_LBL_CV_SEARCH, m_lblSearch);

	DDX_Control(pDX, IDC_LBL_RAIL_STO, m_lblRailSto);
	DDX_Control(pDX, IDC_LBL_RAIL_RET, m_lblRailRet);
	DDX_Control(pDX, IDC_LBL_RAIL_STORET, m_lblRailStoret);
	DDX_Control(pDX, IDC_LBL_RAIL_ERR, m_lblRailErr);
	DDX_Control(pDX, IDC_LBL_RAIL_ITN_LUGG, m_lblRailItnLugg);

	DDX_Control(pDX, IDC_BTN_AUTO_STO	, m_btnAutoSto		);
	DDX_Control(pDX, IDC_BTN_AUTO_RET	, m_btnAutoRet		);
	DDX_Control(pDX, IDC_BTN_AUTO_MOV	, m_btnAutoMove		);
	DDX_Control(pDX, IDC_BTN_AUTO_A2A	, m_btnAutoA2A		);
	DDX_Control(pDX, IDC_BTN_AUTO_W2W	, m_btnAutoW2W		);
	DDX_Control(pDX, IDC_BTN_AUTO_R2R	, m_btnAutoR2R		);
	DDX_Control(pDX, IDC_BTN_STN_STO	, m_btnStnSto		);
	DDX_Control(pDX, IDC_BTN_STN_RET	, m_btnStnRet		);
	DDX_Control(pDX, IDC_BTN_HS_STO		, m_btnHsSto		);
	DDX_Control(pDX, IDC_BTN_HS_RET		, m_btnHsRet		);
	DDX_Control(pDX, IDC_BTN_SUSPEND	, m_btnSuspend		);
	DDX_Control(pDX, IDC_BTN_ERR		, m_btnErr			);
	DDX_Control(pDX, IDC_BTN_MANUAL		, m_btnManual		);
	DDX_Control(pDX, IDC_BTN_DISCONNECT	, m_btnDisConnect	);
	DDX_Control(pDX, IDC_BTN_WC_PASS	, m_btnWcPass		);
	DDX_Control(pDX, IDC_BTN_CV_SEARCH	, m_btnCvSearch		);
	DDX_Control(pDX, IDC_BTN_SC_RAIL_STO, m_btnScRailSto	);
	DDX_Control(pDX, IDC_BTN_SC_RAIL_RET, m_btnScRailRet	);
	DDX_Control(pDX, IDC_BTN_SC_RAIL_ALL, m_btnScRailAll	);
	DDX_Control(pDX, IDC_BTN_SC_RAIL_ERR, m_btnScRailErr	);
	DDX_Control(pDX, IDC_BTN_SC_RAIL_INVK,m_btnScRailInvk	);
	// [LGLS 2026-07-19] 반자동 작업 색상
	DDX_Control(pDX, IDC_GRP_SEMI_STATUS, m_grpSemiStatus);
	DDX_Control(pDX, IDC_LBL_SEMI_STO , m_lblSemiSto );
	DDX_Control(pDX, IDC_LBL_SEMI_RET , m_lblSemiRet );
	DDX_Control(pDX, IDC_LBL_SEMI_MOVE, m_lblSemiMove);
	DDX_Control(pDX, IDC_LBL_SEMI_RTR , m_lblSemiRtr );
	DDX_Control(pDX, IDC_LBL_SEMI_ATA , m_lblSemiAta );
	DDX_Control(pDX, IDC_BTN_SEMI_STO , m_btnSemiSto );
	DDX_Control(pDX, IDC_BTN_SEMI_RET , m_btnSemiRet );
	DDX_Control(pDX, IDC_BTN_SEMI_MOVE, m_btnSemiMove);
	DDX_Control(pDX, IDC_BTN_SEMI_RTR , m_btnSemiRtr );
	DDX_Control(pDX, IDC_BTN_SEMI_ATA , m_btnSemiAta );	
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

	//m_btnAutoSto	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STO			: LIGHT_GRAY;
	//m_btnAutoRet	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_RET			: LIGHT_GRAY;
	//m_btnAutoMove	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_MOVE		: LIGHT_GRAY;
	//m_btnAutoA2A	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_ATA			: LIGHT_GRAY;
	//m_btnAutoW2W	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_WTW			: LIGHT_GRAY;
	//m_btnAutoR2R	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_RTR			: LIGHT_GRAY;
	//m_btnStnSto		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STN_STO		: LIGHT_GRAY;
	//m_btnStnRet		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STN_RET		: LIGHT_GRAY;
	//m_btnHsSto		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_HS_STO		: LIGHT_GRAY;
	//m_btnHsRet		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_HS_RET		: LIGHT_GRAY;
	//m_btnSuspend	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SUSPEND		: LIGHT_GRAY;
	//m_btnErr		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_ERROR		: LIGHT_GRAY;
	//m_btnManual		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_MANUAL		: LIGHT_GRAY;
	//m_btnDisConnect	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_DISCONNECT	: LIGHT_GRAY;
	//m_btnWcPass		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_WC_PASS		: LIGHT_GRAY;
	//m_btnCvSearch	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_CV_SEARCH	: LIGHT_GRAY;
	//m_btnScRailSto	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STO_SUSPEND : LIGHT_GRAY;
	//m_btnScRailRet	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_RET_SUSPEND : LIGHT_GRAY;
	//m_btnScRailAll	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_ALL_SUSPEND : LIGHT_GRAY;
	//m_btnScRailErr	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_RAIL_ERROR  : LIGHT_GRAY;
	//m_btnScRailInvk	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SC_INVK		: LIGHT_GRAY;	
	
	Invalidate(TRUE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CConfigStatus::LoadColor()
{
	m_btnAutoSto	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STO			: LIGHT_GRAY;
	m_btnAutoRet	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_RET			: LIGHT_GRAY;
	m_btnAutoMove	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_MOVE		: LIGHT_GRAY;
	m_btnAutoA2A	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_ATA			: LIGHT_GRAY;
	m_btnAutoW2W	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_WTW			: LIGHT_GRAY;
	m_btnAutoR2R	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_RTR			: LIGHT_GRAY;
	m_btnStnSto		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STN_STO		: LIGHT_GRAY;
	m_btnStnRet		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STN_RET		: LIGHT_GRAY;
	m_btnHsSto		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_HS_STO		: LIGHT_GRAY;
	m_btnHsRet		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_HS_RET		: LIGHT_GRAY;
	m_btnSuspend	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SUSPEND		: LIGHT_GRAY;
	m_btnErr		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_ERROR		: LIGHT_GRAY;
	m_btnManual		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_MANUAL		: LIGHT_GRAY;
	m_btnDisConnect	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_DISCONNECT	: LIGHT_GRAY;
	m_btnWcPass		.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_WC_PASS		: LIGHT_GRAY;
	// [LGLS 2026-09-03] 이 현장에는 WC(작업대 컨트롤러) 패스 모드가 없다 - 범례에서 비활성(사용자 요청)
	m_btnWcPass.EnableWindow(FALSE);
	m_lblCvWcPass.EnableWindow(FALSE);
	m_btnCvSearch	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_CV_SEARCH	: LIGHT_GRAY;
	// [LGLS 2026-07-19] 반자동 작업 색상
	m_btnSemiSto	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_STO		: LIGHT_GRAY;
	m_btnSemiRet	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_RET		: LIGHT_GRAY;
	m_btnSemiMove	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_MOVE	: LIGHT_GRAY;
	m_btnSemiRtr	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_RTR		: LIGHT_GRAY;
	m_btnSemiAta	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_ATA		: LIGHT_GRAY;
	m_btnScRailSto	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_STO_SUSPEND : LIGHT_GRAY;
	m_btnScRailRet	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_RET_SUSPEND : LIGHT_GRAY;
	m_btnScRailAll	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_ALL_SUSPEND : LIGHT_GRAY;
	m_btnScRailErr	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_RAIL_ERROR  : LIGHT_GRAY;
	m_btnScRailInvk	.m_crColor = (m_pDoc != NULL && m_pDoc->m_pConfig != NULL) ? m_pDoc->m_pConfig->m_clrUSER_COLOR_SC_INVK		: LIGHT_GRAY;	

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
		m_pDoc->m_pConfig->m_clrUSER_COLOR_ATA			= m_btnAutoA2A.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_WTW			= m_btnAutoW2W.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_RTR			= m_btnAutoR2R.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_STN_STO		= m_btnStnSto.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_STN_RET		= m_btnStnRet.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_HS_STO		= m_btnHsSto.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_HS_RET		= m_btnHsRet.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SUSPEND		= m_btnSuspend.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_ERROR		= m_btnErr.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_MANUAL		= m_btnManual.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_DISCONNECT	= m_btnDisConnect.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_WC_PASS		= m_btnWcPass.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_CV_SEARCH	= m_btnCvSearch.m_crColor;
		// [LGLS 2026-07-19] 반자동 작업 색상
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_STO		= m_btnSemiSto.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_RET		= m_btnSemiRet.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_MOVE	= m_btnSemiMove.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_RTR		= m_btnSemiRtr.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SEMI_ATA		= m_btnSemiAta.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_STO_SUSPEND	= m_btnScRailSto.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_RET_SUSPEND	= m_btnScRailRet.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_ALL_SUSPEND	= m_btnScRailAll.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_RAIL_ERROR	= m_btnScRailErr.m_crColor;
		m_pDoc->m_pConfig->m_clrUSER_COLOR_SC_INVK		= m_btnScRailInvk.m_crColor;

		
		m_pDoc->m_pConfig->SaveConfigUSER();

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
