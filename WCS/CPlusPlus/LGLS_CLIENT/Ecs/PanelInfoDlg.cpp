// PanelInfoDlg.cpp : [LGLS 2026-09-01] 상세정보 도킹 판넬 (CV/SC/RTV/작업 탭)
//   각 설비 DLG 확대 패널(CvSkinDlg/ScSkinDlg/RtvSkinDlg BuildVehStatusPanel)의 항목을 전부 표로 반영.
//   실제주소 = observables.tsv(CLib::GetObsAddr, DLG 확대 라벨과 동일), 구ECS주소 = ezMCS W/M/R 표기 환산.
#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "MainFrm.h"
#include "PanelInfoDlg.h"
#include "RecordSetWrap.h"
#include "Lib.h"
#include "JobCollection.h"
#include "JOB_MST.h"

#define TIMER_PANEL_INFO     7302
#define TIMER_PANEL_INFO_MS  3000

enum { TAB_CV = 0, TAB_SC, TAB_RTV, TAB_JOB };

// 구ECS 주소 종류
enum { EZ_NONE = 0, EZ_ST, EZ_CMD, EZ_EVT, EZ_ACK, EZ_TRK };

// SC/RTV 워드/비트 주소 (PlcAddressMap.xml 확정값)
//   SC  : 상태 D160+10k(W0100+0x10k) 지시 D320+10k(W0300+0x10k) Event M768+32k(M0300) Ack M1536+16k(M0600) 트래킹 R300+2k
//   RTV : 상태 D210(W0150) 지시 D370(W0350) Event M928(M03A0) Ack M1616(M0650) 트래킹 R310
static CString EzAddr(BOOL bRtv, int nUnit, int nKind, int nOff)
{
	int k = bRtv ? 0 : (nUnit - 1);
	CString s;
	switch (nKind)
	{
	case EZ_ST:  s.Format(_T("W%04X"), (bRtv ? 0x150 : 0x100 + 0x10*k) + nOff); break;
	case EZ_CMD: s.Format(_T("W%04X"), (bRtv ? 0x350 : 0x300 + 0x10*k) + nOff); break;
	case EZ_EVT: s.Format(_T("M%04X"), (bRtv ? 928 : 768 + 32*k) + nOff); break;
	case EZ_ACK: s.Format(_T("M%04X"), (bRtv ? 1616 : 1536 + 16*k) + nOff); break;
	case EZ_TRK: s.Format(_T("R%04d"), (bRtv ? 310 : 300 + 2*k) + nOff); break;
	}
	return s;
}

// [LGLS 2026-09-03] 탭별 명령 버튼 바 정의
//   CvSkinDlg / ScSkinDlg / RtvSkinDlg 의 명령 버튼을 판넬에서도 쓸 수 있게 이식한다.
//   버튼 ID = IDC_PI_CMD_BASE + (아래 배열의 첨자)
enum {
	CMD_CV_COPY = 0, CMD_CV_CUT, CMD_CV_PASTE, CMD_CV_TRACK_PAUSE, CMD_CV_MZ_MOVE,
	CMD_SC_ACTIVE, CMD_SC_STOP, CMD_SC_ERR_RESET, CMD_SC_CTH, CMD_SC_EMERGENCY,
	CMD_SC_STO_SUSPEND, CMD_SC_RET_SUSPEND, CMD_SC_ALL_SUSPEND, CMD_SC_DUPL_STO, CMD_SC_DELETE,
	CMD_RTV_ACTIVE, CMD_RTV_STOP, CMD_RTV_ERR_RESET, CMD_RTV_CTH, CMD_RTV_EMERGENCY,
	CMD_RTV_SUSPEND, CMD_RTV_COMPLETE, CMD_RTV_DELETE,
	CMD_COUNT
};

static const struct { int nTab; LPCTSTR strCap; } CMDS[] = {
	{ TAB_CV,  _T("복사") },        { TAB_CV,  _T("잘라내기") },  { TAB_CV,  _T("붙여넣기") },
	{ TAB_CV,  _T("트랙 일시정지") }, { TAB_CV, _T("MZ 이동") },
	{ TAB_SC,  _T("ACTIVE") },      { TAB_SC,  _T("정지") },      { TAB_SC,  _T("에러리셋") },
	{ TAB_SC,  _T("홈복귀") },      { TAB_SC,  _T("비상정지") },
	{ TAB_SC,  _T("입고금지") },    { TAB_SC,  _T("출고금지") },  { TAB_SC,  _T("입출고금지") },
	{ TAB_SC,  _T("이중입고") },    { TAB_SC,  _T("지시 삭제") },
	{ TAB_RTV, _T("ACTIVE") },      { TAB_RTV, _T("정지") },      { TAB_RTV, _T("에러리셋") },
	{ TAB_RTV, _T("홈복귀") },      { TAB_RTV, _T("비상정지") },
	{ TAB_RTV, _T("사용중지/해제") }, { TAB_RTV, _T("강제완료(설비)") }, { TAB_RTV, _T("지시 삭제") },
};

#define CMD_BTN_W   92
#define CMD_BTN_H   23
#define CMD_BTN_GAP 3

CPanelInfoDlg::CPanelInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelInfoDlg::IDD, pParent)
{
	m_pDoc = NULL;
	m_nRefreshRows = 0;
	m_nBarRows = 1;
}

void CPanelInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PANEL_INFO_TAB, m_tab);
	DDX_Control(pDX, IDC_PANEL_INFO_UNIT, m_cmbUnit);
	DDX_Control(pDX, IDC_PANEL_INFO_LIST, m_list);
	DDX_Control(pDX, IDC_PI_BTN_CVDELETE, m_btnCvDelete);
}

BEGIN_MESSAGE_MAP(CPanelInfoDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_PANEL_INFO_TAB, OnTabChanged)
	ON_NOTIFY(LVN_ENDSCROLL, IDC_PANEL_INFO_LIST, OnListScrolled)
	ON_CBN_SELCHANGE(IDC_PANEL_INFO_UNIT, OnUnitChanged)
	ON_BN_CLICKED(IDC_PI_BTN_STATUS, OnBtnStatus)
	ON_BN_CLICKED(IDC_PI_BTN_PRI, OnBtnPri)
	ON_BN_CLICKED(IDC_PI_BTN_CVWRITE, OnBtnCvWrite)
	ON_BN_CLICKED(IDC_PI_BTN_CVDELETE, OnBtnCvDelete)
	ON_BN_CLICKED(IDC_PI_BTN_FORCE, OnBtnForce)
	ON_BN_CLICKED(IDC_PI_BTN_ABORT, OnBtnAbort)
	ON_COMMAND_RANGE(IDC_PI_CMD_BASE, IDC_PI_CMD_LAST, OnCmdBtn)
END_MESSAGE_MAP()

BOOL CPanelInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_tab.InsertItem(TAB_CV,  _T("CV"));
	m_tab.InsertItem(TAB_SC,  _T("SC"));
	m_tab.InsertItem(TAB_RTV, _T("RTV"));
	m_tab.InsertItem(TAB_JOB, _T("작업정보"));
	m_tab.SetCurSel(-1);                       // 초기 = 아무것도 선택 안 됨

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// [LGLS 2026-09-02] 리스트가 셀 위 컨트롤을 덮어 그려 마우스 오버 때만 보이던 문제 -
	//   형제 창 영역을 클리핑해 컨트롤이 항상 보이게 한다.
	m_list.ModifyStyle(0, WS_CLIPSIBLINGS);
	ModifyStyle(0, WS_CLIPCHILDREN);
	struct { LPCTSTR strHead; int nWidth; } COLS[] = {
		{ _T("항목"),       95 }, { _T("값"),        110 }, { _T("설정"),      95 },
		{ _T("확인"),       72 }, { _T("구ECS주소"),  72 }, { _T("실제주소"),  85 },
		{ _T("기록시명칭"), 190 },
	};
	for (int i = 0; i < (int)(sizeof(COLS)/sizeof(COLS[0])); i++)
		m_list.InsertColumn(i, COLS[i].strHead, LVCFMT_LEFT, COLS[i].nWidth);

	CRect rc0(0, 0, 10, 10);
	m_cmbStatus.Create(WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, rc0, this, IDC_PI_CMB_STATUS);
	m_btnStatus.Create(_T("적용"), WS_CHILD | BS_PUSHBUTTON, rc0, this, IDC_PI_BTN_STATUS);
	m_cmbPri.Create(WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, rc0, this, IDC_PI_CMB_PRI);
	m_btnPri.Create(_T("적용"), WS_CHILD | BS_PUSHBUTTON, rc0, this, IDC_PI_BTN_PRI);
	m_edtCvJob.Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, rc0, this, IDC_PI_EDT_CVJOB);
	m_btnCvWrite.Create(_T("쓰기"), WS_CHILD | BS_PUSHBUTTON, rc0, this, IDC_PI_BTN_CVWRITE);
	m_btnForce.Create(_T("강제완료"), WS_CHILD | BS_PUSHBUTTON, rc0, this, IDC_PI_BTN_FORCE);
	m_btnAbort.Create(_T("이상종료"), WS_CHILD | BS_PUSHBUTTON, rc0, this, IDC_PI_BTN_ABORT);
	m_btnCvDelete.SetWindowText(_T("지시 삭제"));
	m_lblSet1.Create(_T(""), WS_CHILD | SS_CENTERIMAGE | SS_RIGHT, rc0, this);
	m_lblSet2.Create(_T(""), WS_CHILD | SS_CENTERIMAGE | SS_RIGHT, rc0, this);

	CFont* pFont = GetFont();
	m_cmbStatus.SetFont(pFont); m_btnStatus.SetFont(pFont);
	m_cmbPri.SetFont(pFont);    m_btnPri.SetFont(pFont);
	m_edtCvJob.SetFont(pFont);  m_btnCvWrite.SetFont(pFont);
	m_btnForce.SetFont(pFont);  m_btnCvDelete.SetFont(pFont);
	m_btnAbort.SetFont(pFont);
	m_lblSet1.SetFont(pFont);   m_lblSet2.SetFont(pFont);

	// 작업상태 콤보 : COMMON_CODE JOB_STATUS
	if (m_pDoc != NULL)
	{
		CString strSql;
		strSql.Format(_T(" SELECT CCD_CD, ") + m_pDoc->NVL + _T("(CCD_NM_KOR, CCD_CD) AS CCD_NM ")
			_T(" FROM COMMON_CODE WHERE WH_TYP LIKE '%%%s%%' AND CDX_CD = 'JOB_STATUS' ORDER BY CCD_CD "),
			(LPCTSTR)m_pDoc->m_WH_TYP);
		strSql = CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
		int nCnt = -1; CString strMsg;
		_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
		if (nCnt > 0)
		{
			CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
			pRsw->MoveFirst();
			for (int i = 0; i < nCnt; i++)
			{
				CString strCd = pRsw->GetItem(_T("CCD_CD"));
				CString strNm = pRsw->GetItem(_T("CCD_NM"));
				m_cmbStatus.AddString(_T("[") + strCd + _T("] ") + strNm);
				m_arStatusCd.Add(strCd);
				pRsw->MoveNext();
			}
			delete pRsw;
		}
	}
	for (int i = 1; i <= 9; i++)
	{
		CString strP; strP.Format(_T("%d"), i);
		m_cmbPri.AddString(strP);
	}

	BuildCmdBar();

	SetTimer(TIMER_PANEL_INFO, TIMER_PANEL_INFO_MS, NULL);
	UpdateTitle();
	Refresh();
	return TRUE;
}

void CPanelInfoDlg::UpdateTitle()
{
	CString strTitle = _T("상세정보");
	switch (m_tab.GetCurSel())
	{
	case TAB_CV:  strTitle = _T("CV 상세정보");  break;
	case TAB_SC:  strTitle = _T("SC 상세정보");  break;
	case TAB_RTV: strTitle = _T("RTV 상세정보"); break;
	case TAB_JOB: strTitle = _T("JOB 상세정보"); break;
	}
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (pFrame != NULL)
		pFrame->SetInfoPaneTitle(strTitle);
}

void CPanelInfoDlg::FillUnits()
{
	m_cmbUnit.ResetContent();
	int nTab = m_tab.GetCurSel();
	if (nTab == TAB_SC)
	{
		for (int i = 901; i <= 905; i++)
		{
			CString strNo; strNo.Format(_T("%d"), i);
			m_cmbUnit.AddString(strNo);
		}
		m_cmbUnit.SetCurSel(0);
	}
	else if (nTab == TAB_RTV)
	{
		m_cmbUnit.AddString(_T("801"));
		m_cmbUnit.SetCurSel(0);
	}
	else if (nTab == TAB_CV && m_pDoc != NULL)
	{
		CString strSql;
		strSql.Format(_T(" SELECT MC_NO FROM CV_DATA WHERE WH_TYP = '%s' ORDER BY MC_NO "),
			(LPCTSTR)m_pDoc->m_WH_TYP);
		int nCnt = -1; CString strMsg;
		_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
		if (nCnt > 0)
		{
			CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
			pRsw->MoveFirst();
			for (int i = 0; i < nCnt; i++)
			{
				m_cmbUnit.AddString(pRsw->GetItem(_T("MC_NO")));
				pRsw->MoveNext();
			}
			delete pRsw;
		}
		m_cmbUnit.SetCurSel(0);
	}
	m_cmbUnit.ShowWindow((nTab == TAB_CV || nTab == TAB_SC || nTab == TAB_RTV) ? SW_SHOW : SW_HIDE);
}

void CPanelInfoDlg::HideOverlays()
{
	m_cmbStatus.ShowWindow(SW_HIDE); m_btnStatus.ShowWindow(SW_HIDE);
	m_cmbPri.ShowWindow(SW_HIDE);    m_btnPri.ShowWindow(SW_HIDE);
	m_edtCvJob.ShowWindow(SW_HIDE);  m_btnCvWrite.ShowWindow(SW_HIDE);
	m_btnForce.ShowWindow(SW_HIDE);  m_btnCvDelete.ShowWindow(SW_HIDE);
	m_btnAbort.ShowWindow(SW_HIDE);
	m_lblSet1.ShowWindow(SW_HIDE);   m_lblSet2.ShowWindow(SW_HIDE);
}

void CPanelInfoDlg::PlaceOverCell(CWnd* pCtrl, int nRow, int nCol, BOOL bShow)
{
	if (pCtrl == NULL || !::IsWindow(pCtrl->m_hWnd) || nRow < 0 || nRow >= m_list.GetItemCount())
		return;
	CRect rcCell;
	m_list.GetSubItemRect(nRow, nCol, LVIR_BOUNDS, rcCell);
	CRect rcList;
	m_list.GetWindowRect(rcList);
	ScreenToClient(rcList);
	rcCell.OffsetRect(rcList.left, rcList.top);
	BOOL bCombo = pCtrl->IsKindOf(RUNTIME_CLASS(CComboBox));
	int nH = bCombo ? 160 : rcCell.Height();
	pCtrl->MoveWindow(rcCell.left + 1, rcCell.top, rcCell.Width() - 2, nH);
	pCtrl->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	if (bShow)
	{
		pCtrl->BringWindowToTop();
		pCtrl->Invalidate(FALSE);   // 리스트 재도장 직후에도 즉시 다시 그린다
	}
}

// 설비 행 정의 : 항목 / DB 필드(별칭) / 관측 태그(실제주소) / 구ECS 주소종류+오프셋
struct VROW { LPCTSTR strCap; LPCTSTR strField; LPCTSTR strObs; int nEzKind; int nEzOff; };

// SC/RTV 공통 (ScSkinDlg/RtvSkinDlg 확대 패널 항목 전체)
static const VROW VEH_ROWS[] = {
	{ _T("상태"),         _T("SUBSYSTEM_STATUS_RD"),        _T("SUBSYSTEM_STATUS"),         EZ_ST,  0 },
	{ _T("화물감지"),     _T("SEN"),                        _T("PALLET_EXIST_FLAG"),        EZ_EVT, 0 },
	{ _T("상차완료"),     _T("LOAD_COMPLETE_RD"),           _T("LOAD_COMPLETE"),            EZ_EVT, 16 },
	{ _T("상차ACK"),      _T("LOAD_COMPLETE_ACK_OD"),       _T("LOAD_COMPLETE_ACK"),        EZ_ACK, 1 },
	{ _T("하역완료"),     _T("UNLOAD_COMPLETE_RD"),         _T("UNLOAD_COMPLETE"),          EZ_EVT, 17 },
	{ _T("하역ACK"),      _T("UNLOAD_COMPLETE_ACK_OD"),     _T("UNLOAD_COMPLETE_ACK"),      EZ_ACK, 2 },
	{ _T("반송요청"),     _T("TRANSFER_REQUEST_OD"),        _T("TRANSFER_REQUEST"),         EZ_ACK, 0 },
	{ _T("반송ACK"),      _T("TRANSFER_ACK_RD"),            _T("TRANSFER_ACK"),             EZ_EVT, 20 },
	{ _T("알람SET"),      _T("ALARM_SET_REPORT_RD"),        _T("ALARM_SET_REPORT"),         EZ_EVT, 18 },
	{ _T("알람SET ACK"),  _T("ALARM_SET_REPORT_ACK_OD"),    _T("ALARM_SET_REPORT_ACK"),     EZ_ACK, 3 },
	{ _T("알람RST"),      _T("ALARM_RESET_REPORT_RD"),      _T("ALARM_RESET_REPORT"),       EZ_EVT, 19 },
	{ _T("알람RST ACK"),  _T("ALARM_RESET_REPORT_ACK_OD"),  _T("ALARM_RESET_REPORT_ACK"),   EZ_ACK, 4 },
	{ _T("현재위치"),     _T("CUR_LOC"),                    _T("SUBSYSTEM_LOCATION_01"),    EZ_ST,  6 },
	{ _T("지시출발"),     _T("CMD_FROM"),                   _T("FROM_01"),                  EZ_CMD, 0 },
	{ _T("지시도착"),     _T("CMD_TO"),                     _T("TO_01"),                    EZ_CMD, 3 },
	{ _T("완료위치"),     _T("TC_LOC"),                     _T("TRANSFER_COMPLETE_LOCATION_01"), EZ_ST, 3 },
	{ _T("파레트ID"),     _T("PALLET_ID_OD"),               _T("PALLET_ID"),                EZ_CMD, 6 },
	{ _T("알람코드"),     _T("ALARM_SET_CODE_RD"),          _T("ALARM_SET_CODE"),           EZ_ST,  1 },
	{ _T("알람해제코드"), _T("ALARM_RESET_CODE_RD"),        _T("ALARM_RESET_CODE"),         EZ_ST,  2 },
	{ _T("에러코드"),     _T("ERR_CODE_RD"),                _T("ERR_CODE_RD"),              EZ_ST, 10 },
	{ _T("차상화물"),     _T("PALLET_ON_VEHICLE_RD"),       _T("PALLET_ON_VEHICLE"),        EZ_TRK, 0 },
};
#define VEH_ROW_FORCE 20   // 차상화물 행(강제완료 버튼)

// CV (CvSkinDlg 본체 + 확대 패널 항목 전체. 관측 태그는 슬롯(_01/_02)에 따라 달라 코드에서 조립)
static const VROW CV_ROWS[] = {
	{ _T("작업번호"),      _T("LUGG_NO_RD"),             NULL,                        EZ_NONE, 0 },
	{ _T("지시 작업번호"), _T("LUGG_NO_OD"),             NULL,                        EZ_NONE, 0 },
	{ _T("목적지"),        _T("DEST_POS_RD"),            NULL,                        EZ_NONE, 0 },
	{ _T("지시 목적지"),   _T("DEST_POS_OD"),            NULL,                        EZ_NONE, 0 },
	{ _T("구분"),          _T("JOB_TYP_RD"),             NULL,                        EZ_NONE, 0 },
	{ _T("지시 구분"),     _T("JOB_TYP_OD"),             NULL,                        EZ_NONE, 0 },
	{ _T("에러코드"),      _T("ERROR_CODE"),             NULL,                        EZ_NONE, 0 },
	{ _T("상차완료"),      _T("LOAD_COMPLETE_RD"),       _T("LOAD_COMPLETE#"),        EZ_NONE, 0 },
	{ _T("상차ACK"),       _T("LOAD_COMPLETE_ACK_OD"),   _T("LOAD_COMPLETE_ACK#"),    EZ_NONE, 0 },
	{ _T("하역완료"),      _T("UNLOAD_COMPLETE_RD"),     _T("UNLOAD_COMPLETE#"),      EZ_NONE, 0 },
	{ _T("하역ACK"),       _T("UNLOAD_COMPLETE_ACK_OD"), _T("UNLOAD_COMPLETE_ACK#"),  EZ_NONE, 0 },
	{ _T("출고요청"),      _T("UNLOAD_REQUEST_OD"),      _T("UNLOAD_REQUEST_02"),     EZ_NONE, 0 },
	{ _T("출고ACK"),       _T("UNLOAD_REQUEST_ACK_RD"),  _T("UNLOAD_REQUEST_ACK_02"), EZ_NONE, 0 },
	{ _T("입고준비"),      _T("IN_READY_RD"),            _T("IN_READY_02"),           EZ_NONE, 0 },
	{ _T("대기 IN"),       _T("WAIT_IN_RD"),             _T("WAIT_IN"),               EZ_NONE, 0 },
	{ _T("대기 OUT"),      _T("WAIT_OUT_RD"),            _T("WAIT_OUT"),              EZ_NONE, 0 },
	{ _T("자동운전"),      _T("AUTO_MODE_RD"),           _T("OPERATION_MODE"),        EZ_NONE, 0 },
	{ _T("화물감지"),      _T("SENSOR0_DATA_RD"),        _T("PALLET_EXIST_FLAG#"),    EZ_NONE, 0 },
	{ _T("입출방향"),      _T("DIRECTION_MODE_RD"),      _T("DIRECTION_MODE"),        EZ_NONE, 0 },
	{ _T("입고 준비"),     _T("STO_READY_RD"),           NULL,                        EZ_NONE, 0 },
	{ _T("출고 준비"),     _T("RET_READY_RD"),           NULL,                        EZ_NONE, 0 },
	{ _T("입고 HS"),       _T("STOHS_READY_RD"),         NULL,                        EZ_NONE, 0 },
	{ _T("출고 HS"),       _T("RETHS_READY_RD"),         NULL,                        EZ_NONE, 0 },
};
#define CV_ROW_WRITE 1     // 지시 작업번호 행(에디트 + 쓰기)

static const struct { LPCTSTR strName; LPCTSTR strCap; } JOBF[] = {
	{ _T("LUGG_NO"),        _T("작업번호") },   { _T("JOB_TYP"),      _T("작업구분") },
	{ _T("JOB_STATUS"),     _T("작업상태") },   { _T("START_POS"),    _T("출발") },
	{ _T("START_LOCATION"), _T("출발위치") },   { _T("DEST_POS"),     _T("도착") },
	{ _T("DEST_LOCATION"),  _T("도착위치") },   { _T("LOT_NO"),       _T("LOT") },
	{ _T("PRODUCT_ID"),     _T("제품") },       { _T("JOB_PRIORITY"), _T("우선순위") },
	{ _T("INS_DT"),         _T("등록시각") },   { _T("UPD_DT"),       _T("수정시각") },
};
#define JOB_ROW_STATUS 2
#define JOB_ROW_PRI    9

// [LGLS 2026-09-02] 행 재사용 - 있으면 캡션만 갱신, 없으면 추가 (전체 재작성 깜빡임 제거)
void CPanelInfoDlg::EnsureRow(int i, LPCTSTR strCap)
{
	if (i >= m_list.GetItemCount())
		m_list.InsertItem(i, strCap);
	else
	{
		m_list.SetItemText(i, 0, strCap);
		// 탭 전환으로 행을 재사용할 때 이전 탭의 값/주소가 남지 않게 비운다
		for (int c = 1; c <= 6; c++)
			m_list.SetItemText(i, c, _T(""));
	}
	if (i + 1 > m_nRefreshRows) m_nRefreshRows = i + 1;
}

void CPanelInfoDlg::Refresh()
{
	if (m_pDoc == NULL || !::IsWindow(m_list.m_hWnd))
		return;

	int nTab = m_tab.GetCurSel();
	m_list.SetRedraw(FALSE);
	// [LGLS 2026-09-02] 매 갱신 전체 재작성 -> 행 수가 같으면 텍스트만 갱신(오버레이 깜빡임 제거)
	m_nRefreshRows = 0;

	CString strUnit;
	if (m_cmbUnit.GetCurSel() >= 0)
		m_cmbUnit.GetLBText(m_cmbUnit.GetCurSel(), strUnit);

	if (nTab == TAB_JOB)
	{
		// [LGLS] 행/오버레이가 항상 보이도록, 작업이 선택되지 않아도 항목 골격은 그린다
		CRecordSetWrap* pRsw = NULL;
		int nCnt = -1;
		if (!m_strJobNo.IsEmpty())
		{
			CString strSql;
			strSql.Format(
				_T(" SELECT JM.LUGG_NO ")
				_T("       ,'[' + JM.JOB_TYP + '] ' + ") + m_pDoc->NVL + _T("(CT.CCD_NM_KOR, JM.JOB_TYP) AS JOB_TYP ")
				_T("       ,'[' + JM.JOB_STATUS + '] ' + ") + m_pDoc->NVL + _T("(CS.CCD_NM_KOR, JM.JOB_STATUS) AS JOB_STATUS ")
				_T("       ,JM.START_POS, ") + m_pDoc->NVL + _T("(JM.START_LOCATION, ' ') AS START_LOCATION ")
				_T("       ,JM.DEST_POS,  ") + m_pDoc->NVL + _T("(JM.DEST_LOCATION, ' ') AS DEST_LOCATION ")
				_T("       ,") + m_pDoc->NVL + _T("(JM.LOT_NO, ' ') AS LOT_NO ")
				_T("       ,") + m_pDoc->NVL + _T("(JM.PRODUCT_ID, ' ') AS PRODUCT_ID ")
				_T("       ,") + m_pDoc->NVL + _T("(JM.JOB_PRIORITY, ' ') AS JOB_PRIORITY ")
				_T("       ,CONVERT(VARCHAR(19), JM.INS_DT, 120) AS INS_DT ")
				_T("       ,CONVERT(VARCHAR(19), JM.UPD_DT, 120) AS UPD_DT ")
				_T("   FROM JOB_MST JM ")
				_T("   LEFT OUTER JOIN COMMON_CODE CS ON CS.WH_TYP LIKE '%%%s%%' AND CS.CDX_CD = 'JOB_STATUS' AND JM.JOB_STATUS = CS.CCD_CD ")
				_T("   LEFT OUTER JOIN COMMON_CODE CT ON CT.WH_TYP LIKE '%%%s%%' AND CT.CDX_CD = 'JOB_TYP' AND JM.JOB_TYP = CT.CCD_CD ")
				_T("  WHERE JM.WH_TYP = '%s' AND JM.LUGG_NO = '%s' "),
				(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_pDoc->m_WH_TYP,
				(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_strJobNo);
			strSql = CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
			CString strMsg;
			_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
			if (nCnt > 0) { pRsw = new CRecordSetWrap(pRsp); pRsw->MoveFirst(); }
		}
		for (int i = 0; i < (int)(sizeof(JOBF)/sizeof(JOBF[0])); i++)
		{
			EnsureRow(i, JOBF[i].strCap);
			m_list.SetItemText(i, 1, (pRsw != NULL) ? pRsw->GetItem(JOBF[i].strName) : _T(""));
			m_list.SetItemText(i, 6, JOBF[i].strName);
		}
		if (pRsw != NULL) delete pRsw;
	}
	else if (nTab == TAB_CV)
	{
		if (!strUnit.IsEmpty())
		{
			CString N = m_pDoc->NVL;
			CString strSql;
			strSql.Format(
				_T(" SELECT PLC_NO, TRACK_NO ")
				_T("       ,") + N + _T("(LUGG_NO_RD,'') AS LUGG_NO_RD, ") + N + _T("(LUGG_NO_OD,'') AS LUGG_NO_OD ")
				_T("       ,") + N + _T("(DEST_POS_RD,'') AS DEST_POS_RD, ") + N + _T("(DEST_POS_OD,'') AS DEST_POS_OD ")
				_T("       ,") + N + _T("(JOB_TYP_RD,'') AS JOB_TYP_RD, ") + N + _T("(JOB_TYP_OD,'') AS JOB_TYP_OD ")
				_T("       ,") + N + _T("(ERROR_CODE,'') AS ERROR_CODE ")
				_T("       ,") + N + _T("(LOAD_COMPLETE_RD,'0') AS LOAD_COMPLETE_RD, ") + N + _T("(LOAD_COMPLETE_ACK_OD,'0') AS LOAD_COMPLETE_ACK_OD ")
				_T("       ,") + N + _T("(UNLOAD_COMPLETE_RD,'0') AS UNLOAD_COMPLETE_RD, ") + N + _T("(UNLOAD_COMPLETE_ACK_OD,'0') AS UNLOAD_COMPLETE_ACK_OD ")
				_T("       ,") + N + _T("(UNLOAD_REQUEST_OD,'0') AS UNLOAD_REQUEST_OD, ") + N + _T("(UNLOAD_REQUEST_ACK_RD,'0') AS UNLOAD_REQUEST_ACK_RD ")
				_T("       ,") + N + _T("(IN_READY_RD,'0') AS IN_READY_RD, ") + N + _T("(WAIT_IN_RD,'0') AS WAIT_IN_RD, ") + N + _T("(WAIT_OUT_RD,'0') AS WAIT_OUT_RD ")
				_T("       ,") + N + _T("(AUTO_MODE_RD,'0') AS AUTO_MODE_RD, ") + N + _T("(SENSOR0_DATA_RD,'0') AS SENSOR0_DATA_RD ")
				_T("       ,") + N + _T("(DIRECTION_MODE_RD,'') AS DIRECTION_MODE_RD ")
				_T("       ,") + N + _T("(STO_READY_RD,'0') AS STO_READY_RD, ") + N + _T("(RET_READY_RD,'0') AS RET_READY_RD ")
				_T("       ,") + N + _T("(STOHS_READY_RD,'0') AS STOHS_READY_RD, ") + N + _T("(RETHS_READY_RD,'0') AS RETHS_READY_RD ")
				_T("   FROM CV_DATA WHERE WH_TYP = '%s' AND MC_NO = '%s' "),
				(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strUnit);
			int nCnt = -1; CString strMsg;
			_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
			if (nCnt > 0)
			{
				CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
				pRsw->MoveFirst();
				CString strPlc = pRsw->GetItem(_T("PLC_NO")); strPlc.Trim();
				CString strTrk = pRsw->GetItem(_T("TRACK_NO")); strTrk.Trim();
				CString strOwner; strOwner.Format(_T("CONVEYOR:%d"), _ttoi(strPlc));
				CString strSfx = (_ttoi(strTrk) % 2 == 1) ? _T("_01") : _T("_02");

				for (int i = 0; i < (int)(sizeof(CV_ROWS)/sizeof(CV_ROWS[0])); i++)
				{
					EnsureRow(i, CV_ROWS[i].strCap);
					m_list.SetItemText(i, 1, pRsw->GetItem(CV_ROWS[i].strField));
					if (CV_ROWS[i].strObs != NULL)
					{
						CString strTag = CV_ROWS[i].strObs;
						strTag.Replace(_T("#"), strSfx);
						m_list.SetItemText(i, 5, CLib::GetObsAddr(strOwner, strTag));
					}
					m_list.SetItemText(i, 6, CV_ROWS[i].strField);
				}
				delete pRsw;
			}
		}
	}
	else if (nTab == TAB_SC || nTab == TAB_RTV)
	{
		if (!strUnit.IsEmpty())
		{
			BOOL bRtv = (nTab == TAB_RTV);
			CString strTable = bRtv ? _T("RTV_DATA_LGLS") : _T("SC_DATA_LGLS");
			CString strKey   = bRtv ? _T("RTV_NO")        : _T("MC_NO");
			CString strSen   = bRtv ? _T("SENSOR_RTV_RD") : _T("SENSOR_FK_RD");
			CString N = m_pDoc->NVL;
			CString strSql;
			strSql.Format(
				_T(" SELECT ") + N + _T("(SUBSYSTEM_STATUS_RD,'') AS SUBSYSTEM_STATUS_RD ")
				_T("       ,") + N + _T("(%s,'0') AS SEN ")
				_T("       ,") + N + _T("(LOAD_COMPLETE_RD,'0') AS LOAD_COMPLETE_RD, ") + N + _T("(LOAD_COMPLETE_ACK_OD,'0') AS LOAD_COMPLETE_ACK_OD ")
				_T("       ,") + N + _T("(UNLOAD_COMPLETE_RD,'0') AS UNLOAD_COMPLETE_RD, ") + N + _T("(UNLOAD_COMPLETE_ACK_OD,'0') AS UNLOAD_COMPLETE_ACK_OD ")
				_T("       ,") + N + _T("(TRANSFER_REQUEST_OD,'0') AS TRANSFER_REQUEST_OD, ") + N + _T("(TRANSFER_ACK_RD,'0') AS TRANSFER_ACK_RD ")
				_T("       ,") + N + _T("(ALARM_SET_REPORT_RD,'0') AS ALARM_SET_REPORT_RD, ") + N + _T("(ALARM_SET_REPORT_ACK_OD,'0') AS ALARM_SET_REPORT_ACK_OD ")
				_T("       ,") + N + _T("(ALARM_RESET_REPORT_RD,'0') AS ALARM_RESET_REPORT_RD, ") + N + _T("(ALARM_RESET_REPORT_ACK_OD,'0') AS ALARM_RESET_REPORT_ACK_OD ")
				_T("       ,") + N + _T("(LOCATION_01_RD,'') + '/' + ") + N + _T("(LOCATION_02_RD,'') + '/' + ") + N + _T("(LOCATION_03_RD,'') AS CUR_LOC ")
				_T("       ,") + N + _T("(FROM_01_OD,'') + '/' + ") + N + _T("(FROM_02_OD,'') + '/' + ") + N + _T("(FROM_03_OD,'') AS CMD_FROM ")
				_T("       ,") + N + _T("(TO_01_OD,'') + '/' + ") + N + _T("(TO_02_OD,'') + '/' + ") + N + _T("(TO_03_OD,'') AS CMD_TO ")
				_T("       ,") + N + _T("(TRANSFER_COMPLETE_LOCATION_01_RD,'') + '/' + ") + N + _T("(TRANSFER_COMPLETE_LOCATION_02_RD,'') + '/' + ") + N + _T("(TRANSFER_COMPLETE_LOCATION_03_RD,'') AS TC_LOC ")
				_T("       ,") + N + _T("(PALLET_ID_OD,'') AS PALLET_ID_OD ")
				_T("       ,") + N + _T("(ALARM_SET_CODE_RD,'') AS ALARM_SET_CODE_RD, '' AS ALARM_RESET_CODE_RD ")   // 해제코드는 DB 미기록(주소만 표기)
				_T("       ,") + N + _T("(ERR_CODE_RD,'') AS ERR_CODE_RD, ") + N + _T("(PALLET_ON_VEHICLE_RD,'') AS PALLET_ON_VEHICLE_RD ")
				_T("   FROM %s WHERE WH_TYP = '%s' AND %s = '%s' "),
				(LPCTSTR)strSen, (LPCTSTR)strTable, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strKey, (LPCTSTR)strUnit);
			int nCnt = -1; CString strMsg;
			_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
			if (nCnt > 0)
			{
				int nUnit = _ttoi(strUnit) - 900;   // SC 901~905 -> 1~5
				CString strOwner = bRtv ? _T("VEHICLE:1") : _T("");
				if (!bRtv) strOwner.Format(_T("VEHICLE:1%d"), nUnit);

				CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
				pRsw->MoveFirst();
				for (int i = 0; i < (int)(sizeof(VEH_ROWS)/sizeof(VEH_ROWS[0])); i++)
				{
					EnsureRow(i, VEH_ROWS[i].strCap);
					CString strVal = pRsw->GetItem(VEH_ROWS[i].strField);
					if (i == 0)
					{
						strVal.Trim();
						if (strVal == _T("0")) strVal = _T("0 (DOWN)");
						else if (strVal == _T("1")) strVal = _T("1 (IDLE)");
						else if (strVal == _T("2")) strVal = _T("2 (RUN)");
					}
					m_list.SetItemText(i, 1, strVal);
					CString strEz;
					if (bRtv && VEH_ROWS[i].nEzKind == EZ_ST && VEH_ROWS[i].nEzOff == 10)
						strEz = _T("-");   // RTV 는 에러코드 워드가 없다(알람코드 유래)
					else
						strEz = EzAddr(bRtv, nUnit, VEH_ROWS[i].nEzKind, VEH_ROWS[i].nEzOff);
					m_list.SetItemText(i, 4, strEz);
					m_list.SetItemText(i, 5, CLib::GetObsAddr(strOwner, VEH_ROWS[i].strObs));
					m_list.SetItemText(i, 6, VEH_ROWS[i].strField);
				}
				delete pRsw;
			}
		}
	}

	// 남는 행 정리(탭 전환으로 행 수가 줄어든 경우)
	while (m_list.GetItemCount() > m_nRefreshRows)
		m_list.DeleteItem(m_list.GetItemCount() - 1);
	m_list.SetRedraw(TRUE);
	m_list.Invalidate(FALSE);

	PlaceOverlays();
}

// [LGLS 2026-09-02] 설정/확인 컨트롤을 해당 행의 [설정]/[확인] 칸 안에 배치(사용자 확정).
//   갱신·스크롤·리사이즈 때마다 위치를 다시 잡아 항상 칸을 따라간다.
void CPanelInfoDlg::PlaceOverlays()
{
	HideOverlays();

	int nTab = m_tab.GetCurSel();
	if (nTab == TAB_JOB && m_list.GetItemCount() > JOB_ROW_PRI)
	{
		PlaceOverCell(&m_cmbStatus, JOB_ROW_STATUS, 2, TRUE);
		PlaceOverCell(&m_btnStatus, JOB_ROW_STATUS, 3, TRUE);
		PlaceOverCell(&m_cmbPri,    JOB_ROW_PRI, 2, TRUE);
		PlaceOverCell(&m_btnPri,    JOB_ROW_PRI, 3, TRUE);
	}
	else if (nTab == TAB_CV && m_list.GetItemCount() > CV_ROW_WRITE)
	{
		PlaceOverCell(&m_edtCvJob,   CV_ROW_WRITE, 2, TRUE);
		PlaceOverCell(&m_btnCvWrite, CV_ROW_WRITE, 3, TRUE);
		// 지시 삭제는 행 아래(지시 목적지 행)의 [확인] 칸
		PlaceOverCell(&m_btnCvDelete, CV_ROW_WRITE + 2, 3, TRUE);
	}
	else if ((nTab == TAB_SC || nTab == TAB_RTV) && m_list.GetItemCount() > VEH_ROW_FORCE)
	{
		PlaceOverCell(&m_btnForce, VEH_ROW_FORCE, 3, TRUE);
		// [LGLS 2026-09-02] 이상종료(구 ECS 대응) : 지시 작업번호(파레트ID) 행의 [확인] 칸
		PlaceOverCell(&m_btnAbort, 16, 3, TRUE);
	}
}

void CPanelInfoDlg::SetJob(CString strLuggNo)
{
	m_strJobNo = strLuggNo;
	if (!::IsWindow(m_tab.m_hWnd))
		return;
	if (m_tab.GetCurSel() != TAB_JOB)
	{
		m_tab.SetCurSel(TAB_JOB);
		FillUnits();
		UpdateTitle();
	}
	Refresh();
}

// [LGLS 2026-09-01] 화면 설비 클릭 -> 해당 탭/호기 선택
void CPanelInfoDlg::SetEquip(int nTab, CString strUnit)
{
	if (!::IsWindow(m_tab.m_hWnd))
		return;
	if (m_tab.GetCurSel() != nTab)
	{
		m_tab.SetCurSel(nTab);
		FillUnits();
		UpdateTitle();
	}
	strUnit.Trim();
	if (!strUnit.IsEmpty())
		m_cmbUnit.SelectString(-1, strUnit);
	Refresh();
}

BOOL CPanelInfoDlg::ExecUpdate(CString strSql, CString strLogMsg, CString strLuggNo, CString strWidId)
{
	if (!m_pDoc->Permission(strWidId, UPD_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return FALSE;
	}
	if (m_pDoc->BeginTrans_DLG() < 1)
		return FALSE;
	if (strLuggNo.IsEmpty()) strLuggNo = _T("0");
	if (!m_pDoc->GetQueryInsertClientLog(strWidId, strLuggNo, _T(""), _T(""), strLogMsg))
	{
		m_pDoc->RollbackTrans_DLG();
		return FALSE;
	}
	if (!m_pDoc->ExcuteQueryString_DLG(strSql))
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return FALSE;
	}
	m_pDoc->CommitTrans_DLG();
	return TRUE;
}

void CPanelInfoDlg::OnBtnStatus()
{
	if (m_strJobNo.IsEmpty() || m_cmbStatus.GetCurSel() < 0) return;
	CString strCd = m_arStatusCd[m_cmbStatus.GetCurSel()];
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업 상태를 변경 하시겠습니까?")) + _T(" [") + m_strJobNo + _T(" -> ") + strCd + _T("]"), MB_YESNO) != IDYES)
		return;
	CString strSql;
	strSql.Format(_T("UPDATE JOB_MST SET JOB_STATUS = '%s', UPD_DT = GETDATE() WHERE WH_TYP = '%s' AND LUGG_NO = '%s'"),
		(LPCTSTR)strCd, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_strJobNo);
	if (ExecUpdate(strSql, _T("JOB_MST UPDATE : JOB_STATUS -> ") + strCd, m_strJobNo, _T("CViewJobListDlg")))
		Refresh();
}

void CPanelInfoDlg::OnBtnPri()
{
	if (m_strJobNo.IsEmpty() || m_cmbPri.GetCurSel() < 0) return;
	CString strPri; m_cmbPri.GetLBText(m_cmbPri.GetCurSel(), strPri);
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("우선순위를 변경 하시겠습니까?")) + _T(" [") + m_strJobNo + _T(" -> ") + strPri + _T("]"), MB_YESNO) != IDYES)
		return;
	CString strSql;
	strSql.Format(_T("UPDATE JOB_MST SET JOB_PRIORITY = '%s', UPD_DT = GETDATE() WHERE WH_TYP = '%s' AND LUGG_NO = '%s'"),
		(LPCTSTR)strPri, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)m_strJobNo);
	if (ExecUpdate(strSql, _T("JOB_MST UPDATE : JOB_PRIORITY -> ") + strPri, m_strJobNo, _T("CViewJobListDlg")))
		Refresh();
}

void CPanelInfoDlg::OnBtnCvWrite()
{
	CString strUnit;
	if (m_cmbUnit.GetCurSel() >= 0) m_cmbUnit.GetLBText(m_cmbUnit.GetCurSel(), strUnit);
	if (strUnit.IsEmpty()) return;
	CString strJob; m_edtCvJob.GetWindowText(strJob); strJob.Trim();
	if (strJob.IsEmpty()) return;
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("지시 작업번호를 쓰시겠습니까?")) + _T(" [") + strUnit + _T(" <- ") + strJob + _T("]"), MB_YESNO) != IDYES)
		return;
	CString strSql;
	strSql.Format(_T("UPDATE CV_DATA SET LUGG_NO_OD = '%s', WRITE_UPD_DT = GETDATE() WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
		(LPCTSTR)strJob, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strUnit);
	if (ExecUpdate(strSql, _T("CV_DATA UPDATE : LUGG_NO_OD -> ") + strJob + _T(" (") + strUnit + _T(")"), strJob, _T("CCvSkinDlg")))
		Refresh();
}

void CPanelInfoDlg::OnBtnCvDelete()
{
	CString strUnit;
	if (m_cmbUnit.GetCurSel() >= 0) m_cmbUnit.GetLBText(m_cmbUnit.GetCurSel(), strUnit);
	if (strUnit.IsEmpty()) return;
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("지시값을 삭제 하시겠습니까?")) + _T(" [") + strUnit + _T("]"), MB_YESNO) != IDYES)
		return;
	CString strSql;
	strSql.Format(_T("UPDATE CV_DATA SET LUGG_NO_OD = '0', DEST_POS_OD = '0', JOB_TYP_OD = '0', PULP_SENSOR_OD = '0', WRITE_UPD_DT = GETDATE() WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strUnit);
	if (ExecUpdate(strSql, _T("CV_DATA UPDATE : OD CLEAR (") + strUnit + _T(")"), _T("0"), _T("CCvSkinDlg")))
		Refresh();
}

void CPanelInfoDlg::OnBtnForce()
{
	BOOL bRtv = (m_tab.GetCurSel() == TAB_RTV);
	CString strJob = m_list.GetItemText(VEH_ROW_FORCE, 1); strJob.Trim();   // 차상화물
	if (strJob.IsEmpty() || strJob == _T("0") || strJob == _T("0000"))
	{
		strJob = m_list.GetItemText(16, 1); strJob.Trim();                  // 파레트ID(지시)
	}
	if (strJob.IsEmpty() || strJob == _T("0") || strJob == _T("0000"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("대상 작업이 없습니다")));
		return;
	}
	CString strStatus = bRtv ? _T("39") : _T("29");
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("강제완료 하시겠습니까?")) + _T(" [") + strJob + _T(" -> ") + strStatus + _T("]"), MB_YESNO) != IDYES)
		return;
	CString strSql;
	strSql.Format(_T("UPDATE JOB_MST SET JOB_STATUS = '%s', UPD_DT = GETDATE() WHERE WH_TYP = '%s' AND LUGG_NO = '%s'"),
		(LPCTSTR)strStatus, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strJob);
	if (ExecUpdate(strSql, _T("JOB_MST UPDATE : 강제완료 JOB_STATUS -> ") + strStatus, strJob, bRtv ? _T("CRtvSkinDlg") : _T("CScSkinDlg")))
		Refresh();
}

// [LGLS 2026-09-02] 이상종료 - 구 ECS RGV/Stacker 팝업의 [이상종료]와 같은 의미.
//   설비가 물고 있는 지시(OD)를 해제해 "설비-명령 연결이 꼬였을 때 리셋" 한다. JOB_MST 는 건드리지 않는다.
//   구 ECS 와 동일하게 RUN(작업중)/DOWN(정지) 상태에서는 거부한다.
void CPanelInfoDlg::OnBtnAbort()
{
	BOOL bRtv = (m_tab.GetCurSel() == TAB_RTV);
	CString strUnit;
	if (m_cmbUnit.GetCurSel() >= 0) m_cmbUnit.GetLBText(m_cmbUnit.GetCurSel(), strUnit);
	if (strUnit.IsEmpty()) return;

	CString strState = m_list.GetItemText(0, 1);
	if (strState.Find(_T("(RUN)")) >= 0)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("RUN(작업중) 상태입니다. 작업 완료 후 이상종료 하십시오")));
		return;
	}
	if (strState.Find(_T("(DOWN)")) >= 0)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("DOWN(정지) 상태에서는 이상종료 할 수 없습니다")));
		return;
	}

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("이상종료 하시겠습니까? (지시 연결 해제, 작업정보는 유지)")) + _T(" [") + strUnit + _T("]"), MB_YESNO) != IDYES)
		return;

	CString strSql;
	if (bRtv)
	{
		// RtvSkinDlg [삭제] 의 지시 클리어와 동일
		strSql.Format(_T("UPDATE RTV_DATA_LGLS SET LUGG_OD = '0000', PALLET_ID_OD = '0000', JOB_TYP_OD = '0'")
			_T(", FROM_01_OD = '00', FROM_02_OD = '00', FROM_03_OD = '00'")
			_T(", TO_01_OD = '00', TO_02_OD = '00', TO_03_OD = '00'")
			_T(", RTV_DEST_OD = '', RTV_PASSCV_OD = '', OD_RQ_YN = 'N'")
			_T(" WHERE WH_TYP = '%s' AND RTV_NO = '%s'"),
			(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strUnit);
	}
	else
	{
		strSql.Format(_T("UPDATE SC_DATA_LGLS SET PALLET_ID_OD = '0000'")
			_T(", FROM_01_OD = '00', FROM_02_OD = '00', FROM_03_OD = '00'")
			_T(", TO_01_OD = '00', TO_02_OD = '00', TO_03_OD = '00'")
			_T(", OD_RQ_YN = 'N'")
			_T(" WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
			(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strUnit);
	}
	if (ExecUpdate(strSql, CString(_T("이상종료 : 지시(OD) 해제 (")) + strUnit + _T(")"), _T("0"),
			bRtv ? _T("CRtvSkinDlg") : _T("CScSkinDlg")))
		Refresh();
}

void CPanelInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (::IsWindow(m_tab.m_hWnd))
		m_tab.MoveWindow(0, 0, cx, 24);
	if (::IsWindow(m_cmbUnit.m_hWnd))
		m_cmbUnit.MoveWindow(cx - 92, 1, 90, 160);
	LayoutCmdBar(cx, cy);
	if (::IsWindow(m_list.m_hWnd))
		Refresh();
}

void CPanelInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_PANEL_INFO && IsWindowVisible())
	{
		CWnd* pFocus = GetFocus();
		BOOL bEditing = (pFocus == &m_edtCvJob) ||
			(m_cmbStatus.GetDroppedState()) || (m_cmbPri.GetDroppedState());
		if (!bEditing)
			Refresh();
	}
	CDialog::OnTimer(nIDEvent);
}

void CPanelInfoDlg::OnTabChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	HideOverlays();
	CRect rcC; GetClientRect(rcC);
	LayoutCmdBar(rcC.Width(), rcC.Height());
	FillUnits();
	UpdateTitle();
	Refresh();
}

void CPanelInfoDlg::OnUnitChanged()
{
	Refresh();
}

void CPanelInfoDlg::OnListScrolled(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	PlaceOverlays();
}

// ==========================================================================
// [LGLS 2026-09-03] 탭별 명령 버튼 바
//   설비 대화상자(CvSkinDlg/ScSkinDlg/RtvSkinDlg)의 명령을 판넬에서 그대로 수행한다.
//   원 대화상자 코드는 수정하지 않았고, 같은 SQL/같은 확인 메시지/같은 권한 검사를 따른다.
// ==========================================================================

void CPanelInfoDlg::BuildCmdBar()
{
	CRect rc0(0, 0, 10, 10);
	CFont* pFont = GetFont();
	for (int i = 0; i < CMD_COUNT; i++)
	{
		m_btnCmd[i].Create(CMDS[i].strCap, WS_CHILD | BS_PUSHBUTTON, rc0, this, IDC_PI_CMD_BASE + i);
		m_btnCmd[i].SetFont(pFont);
	}
}

// 판넬 폭에 맞춰 자동 줄바꿈 배치. 리스트는 버튼 바 높이만큼 줄인다.
void CPanelInfoDlg::LayoutCmdBar(int cx, int cy)
{
	if (!::IsWindow(m_btnCmd[0].m_hWnd))
	{
		if (::IsWindow(m_list.m_hWnd))
			m_list.MoveWindow(0, 26, cx, cy - 26);
		return;
	}

	int nTab = m_tab.GetCurSel();
	int nPerRow = (cx - CMD_BTN_GAP) / (CMD_BTN_W + CMD_BTN_GAP);
	if (nPerRow < 1) nPerRow = 1;

	int nCnt = 0;
	for (int i = 0; i < CMD_COUNT; i++)
		if (CMDS[i].nTab == nTab) nCnt++;

	int nRows = (nCnt > 0) ? ((nCnt + nPerRow - 1) / nPerRow) : 0;
	m_nBarRows = nRows;
	int nBarH = (nRows > 0) ? (nRows * (CMD_BTN_H + CMD_BTN_GAP) + CMD_BTN_GAP) : 0;

	int nListH = cy - 26 - nBarH;
	if (nListH < 40) nListH = 40;
	if (::IsWindow(m_list.m_hWnd))
		m_list.MoveWindow(0, 26, cx, nListH);

	int nTop = cy - nBarH + CMD_BTN_GAP;
	int nSeq = 0;
	for (int i = 0; i < CMD_COUNT; i++)
	{
		if (CMDS[i].nTab != nTab)
		{
			m_btnCmd[i].ShowWindow(SW_HIDE);
			continue;
		}
		int r = nSeq / nPerRow;
		int c = nSeq % nPerRow;
		m_btnCmd[i].MoveWindow(CMD_BTN_GAP + c * (CMD_BTN_W + CMD_BTN_GAP),
			nTop + r * (CMD_BTN_H + CMD_BTN_GAP), CMD_BTN_W, CMD_BTN_H);
		m_btnCmd[i].ShowWindow(SW_SHOW);
		nSeq++;
	}
}

CString CPanelInfoDlg::GetCurUnit()
{
	CString strUnit;
	if (m_cmbUnit.GetCurSel() >= 0)
		m_cmbUnit.GetLBText(m_cmbUnit.GetCurSel(), strUnit);
	strUnit.Trim();
	return strUnit;
}

CString CPanelInfoDlg::QueryOne(CString strSql, LPCTSTR strCol)
{
	CString strRet;
	if (m_pDoc == NULL) return strRet;
	int nCnt = -1; CString strMsg;
	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
	if (nCnt > 0)
	{
		CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
		pRsw->MoveFirst();
		strRet = pRsw->GetItem(strCol);
		delete pRsw;
	}
	strRet.Trim();
	return strRet;
}

BOOL CPanelInfoDlg::GetCvCtx(CString& strMc, CString& strPlc)
{
	strMc = GetCurUnit();
	if (strMc.IsEmpty()) return FALSE;
	CString strSql;
	strSql.Format(_T("SELECT PLC_NO FROM CV_DATA WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strMc);
	strPlc = QueryOne(strSql, _T("PLC_NO"));
	if (strPlc.IsEmpty())
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("설비 정보를 찾을 수 없습니다")));
		return FALSE;
	}
	return TRUE;
}

BOOL CPanelInfoDlg::GetVehCtx(BOOL bRtv, CString& strNo, CString& strPlc)
{
	strNo = GetCurUnit();
	if (strNo.IsEmpty()) return FALSE;
	CString strSql;
	if (bRtv)
		strSql.Format(_T("SELECT PLC_NO FROM RTV_DATA_LGLS WHERE WH_TYP = '%s' AND RTV_NO = '%s'"),
			(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strNo);
	else
		strSql.Format(_T("SELECT PLC_NO FROM SC_DATA_LGLS WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
			(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strNo);
	strPlc = QueryOne(strSql, _T("PLC_NO"));
	if (strPlc.IsEmpty())
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("설비 정보를 찾을 수 없습니다")));
		return FALSE;
	}
	return TRUE;
}

// ExecUpdate 와 같은 절차. 권한 종류(UPD_YN / EXE_YN)만 골라 쓴다.
BOOL CPanelInfoDlg::ExecUpdateEx(CString strSql, CString strLogMsg, CString strLuggNo, CString strWidId, int nPerm)
{
	if (!m_pDoc->Permission(strWidId, nPerm))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return FALSE;
	}
	if (m_pDoc->BeginTrans_DLG() < 1)
		return FALSE;
	if (strLuggNo.IsEmpty()) strLuggNo = _T("0");
	if (!m_pDoc->GetQueryInsertClientLog(strWidId, strLuggNo, _T(""), _T(""), strLogMsg))
	{
		m_pDoc->RollbackTrans_DLG();
		return FALSE;
	}
	if (!m_pDoc->ExcuteQueryString_DLG(strSql))
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
		return FALSE;
	}
	m_pDoc->CommitTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
	return TRUE;
}

// ---------------- CV : 복사 / 잘라내기 ----------------
void CPanelInfoDlg::DoCvCopy(BOOL bCut)
{
	CString strMc, strPlc;
	if (!GetCvCtx(strMc, strPlc)) return;

	CString strSql;
	strSql.Format(_T("SELECT ") + m_pDoc->NVL + _T("(LUGG_NO_RD,'') AS LUGG_NO_RD, ")
		+ m_pDoc->NVL + _T("(DEST_POS_RD,'') AS DEST_POS_RD, ")
		+ m_pDoc->NVL + _T("(JOB_TYP_RD,'') AS JOB_TYP_RD, ")
		+ m_pDoc->NVL + _T("(PULP_SENSOR_RD,'0') AS PULP_SENSOR_RD ")
		_T(" FROM CV_DATA WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strMc);
	int nCnt = -1; CString strMsg;
	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
	if (nCnt <= 0) return;
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
	pRsw->MoveFirst();
	CString strJob  = pRsw->GetItem(_T("LUGG_NO_RD"));    strJob.Trim();
	CString strDest = pRsw->GetItem(_T("DEST_POS_RD"));   strDest.Trim();
	CString strTyp  = pRsw->GetItem(_T("JOB_TYP_RD"));    strTyp.Trim();
	CString strPulp = pRsw->GetItem(_T("PULP_SENSOR_RD"));strPulp.Trim();
	delete pRsw;

	CJOB_MST* pCopyJob = m_pDoc->m_pJob->GetCopyJob();
	if (pCopyJob == NULL) return;
	pCopyJob->LUGG_NO      = strJob;
	pCopyJob->DEST_POS     = strDest;
	pCopyJob->JOB_TYP      = strTyp;
	pCopyJob->PRODUCT_SIZE = strPulp;
	pCopyJob->COPY_YN      = true;

	if (!bCut)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("데이터 복사 성공")));
		return;
	}

	// 잘라내기 : 복사 후 지시(OD) 를 0 으로 지운다 (CvSkinDlg EN_BtnCut 과 동일)
	strSql.Format(_T(" UPDATE CV_DATA SET LUGG_NO_OD = '0', DEST_POS_OD = '0', JOB_TYP_OD = '0' ")
		_T(" , PULP_SENSOR_OD = '0', WRITE_UPD_DT = ") + m_pDoc->SYSDATE +
		_T(" , OD_RQ_YN = 'Y', TRACKING_WRITE_YN = 'Y' ")
		_T(" WHERE WH_TYP = '%s' AND PLC_NO = '%02s' AND MC_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strPlc, (LPCTSTR)strMc);
	CString strLog;
	strLog.Format(_T("CV 잘라내기 -> CV 번호 : %s , 작업번호 : %s->0"), (LPCTSTR)strMc, (LPCTSTR)strJob);
	if (ExecUpdateEx(strSql, strLog, strJob, _T("CCvSkinDlg"), UPD_YN))
		Refresh();
}

// ---------------- CV : 붙여넣기 ----------------
void CPanelInfoDlg::DoCvPaste()
{
	CString strMc, strPlc;
	if (!GetCvCtx(strMc, strPlc)) return;

	CJOB_MST* pCopyJob = m_pDoc->m_pJob->GetCopyJob();
	if (pCopyJob == NULL || pCopyJob->LUGG_NO.IsEmpty())
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업번호 없습니다")));
		return;
	}

	// 중간 목적지(CV_DEF_INF) 조회 - CvSkinDlg EN_BtnPaste 와 동일
	CString strSql, strTO_CV;
	strSql.Format(_T(" SELECT TO_CV FROM CV_DEF_INF WHERE WH_TYP = '%s' AND FR_CV = '%s' ")
		_T(" AND BUFFER_CV = '%s' AND USE_YON = 'Y' "),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strMc, (LPCTSTR)pCopyJob->DEST_POS);
	strTO_CV = QueryOne(strSql, _T("TO_CV"));
	if (strTO_CV.IsEmpty())
	{
		if (pCopyJob->DEST_POS == _T("904"))
		{
			CString strSqlPlt;
			strSqlPlt.Format(_T("SELECT ") + m_pDoc->NVL + _T("(SC_PLT_JOB_TYP,'') AS SC_PLT_JOB_TYP FROM CV_DATA WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
				(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strMc);
			CString strPlt = QueryOne(strSqlPlt, _T("SC_PLT_JOB_TYP"));
			strTO_CV = (strPlt == _T("2")) ? _T("904") : _T("125");
		}
		else
		{
			strTO_CV = pCopyJob->DEST_POS;
		}
	}

	CString strPulpOd = pCopyJob->PRODUCT_SIZE;
	if (strPulpOd.GetLength() > 2) strPulpOd = strPulpOd.Left(2);
	if (strPulpOd.IsEmpty()) strPulpOd = _T("0");

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업을 붙여넣으시겠습니까?")) + _T(" [") + strMc + _T(" <- ") + pCopyJob->LUGG_NO + _T("]"), MB_YESNO) != IDYES)
		return;

	strSql.Format(_T(" UPDATE CV_DATA SET LUGG_NO_OD = '%s', DEST_POS_OD = '%s', JOB_TYP_OD = '%s' ")
		_T(" , PULP_SENSOR_OD = '%s', WRITE_UPD_DT = ") + m_pDoc->SYSDATE +
		_T(" , OD_RQ_YN = 'Y', TRACKING_WRITE_YN = 'Y', TR_PAUSE_OD = '0', ERR_RQ_OD = '0' ")
		_T(" WHERE WH_TYP = '%s' AND PLC_NO = '%02s' AND MC_NO = '%s'"),
		(LPCTSTR)pCopyJob->LUGG_NO, (LPCTSTR)strTO_CV, (LPCTSTR)pCopyJob->JOB_TYP, (LPCTSTR)strPulpOd,
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strPlc, (LPCTSTR)strMc);

	CString strLog;
	strLog.Format(_T("CV 붙여넣기 -> CV 번호 : %s , 작업번호 : %s , 작업구분 : %s , 도착지 : %s"),
		(LPCTSTR)strMc, (LPCTSTR)pCopyJob->LUGG_NO, (LPCTSTR)pCopyJob->JOB_TYP, (LPCTSTR)strTO_CV);
	if (!ExecUpdateEx(strSql, strLog, pCopyJob->LUGG_NO, _T("CCvSkinDlg"), UPD_YN))
		return;

	// 작업정보가 있으면 CV 구동중(11) 으로 - CvSkinDlg 와 동일
	CString strSqlChk;
	strSqlChk.Format(_T("SELECT COUNT(1) AS CNT FROM JOB_MST WHERE WH_TYP = '%s' AND LUGG_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)pCopyJob->LUGG_NO);
	if (QueryOne(strSqlChk, _T("CNT")) != _T("0"))
	{
		CString strSqlJob;
		strSqlJob.Format(_T(" UPDATE JOB_MST SET JOB_STATUS = '11', UPD_USER_ID = '%s', UPD_DT = ") + m_pDoc->SYSDATE +
			_T(" WHERE WH_TYP = '%s' AND LUGG_NO = '%s'"),
			(LPCTSTR)m_pDoc->m_strId, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)pCopyJob->LUGG_NO);
		m_pDoc->ExcuteQueryString_DLG(strSqlJob);
	}
	pCopyJob->COPY_YN = false;
	Refresh();
}

// ---------------- CV : 트랙 일시정지 ----------------
void CPanelInfoDlg::DoCvTrackPause()
{
	CString strMc, strPlc;
	if (!GetCvCtx(strMc, strPlc)) return;

	CString strSql;
	strSql.Format(_T("SELECT ") + m_pDoc->NVL + _T("(TR_PAUSE_RD,'0') AS TR_PAUSE_RD FROM CV_DATA WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strMc);
	CString strPause = QueryOne(strSql, _T("TR_PAUSE_RD"));
	CString strNew = (strPause == _T("0")) ? _T("1") : _T("0");

	if (strPause == _T("0"))
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 트랙을 일시정지 설정 하시겠습니까?")), MB_YESNO) != IDYES) return;
	}
	else
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 트랙을 일시정지 해제 하시겠습니까?")), MB_YESNO) != IDYES) return;
	}

	// 일시정지 가능 트랙인지(STN_KIND) 확인 - CvSkinDlg 와 동일
	strSql.Format(_T(" SELECT COUNT(1) AS CNT FROM CV_DATA WHERE WH_TYP = '%s' AND PLC_NO = '%s' AND MC_NO = '%s' AND STN_KIND IS NOT NULL "),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strPlc, (LPCTSTR)strMc);
	if (QueryOne(strSql, _T("CNT")) != _T("1"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 트랙은 일시정지를 할 수 없습니다.")));
		return;
	}

	strSql.Format(_T(" UPDATE CV_DATA SET TR_PAUSE_OD = '%s', TR_PAUSE_RD = '%s', CMD_RQ_ID = '3', CMD_RQ_YN = 'Y' ")
		_T(" WHERE WH_TYP = '%s' AND PLC_NO = '%s' AND MC_NO = '%s'"),
		(LPCTSTR)strNew, (LPCTSTR)strNew, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strPlc, (LPCTSTR)strMc);
	CString strLog;
	strLog.Format(_T("트랙 일시정지 설정 -> TRACK NO : %s , 일시정지 : %s"), (LPCTSTR)strMc, (LPCTSTR)strNew);
	if (ExecUpdateEx(strSql, strLog, _T("0"), _T("CViewJobListDlg"), UPD_YN))
		Refresh();
}

// ---------------- CV : MZ(매거진 108) 이동 ----------------
void CPanelInfoDlg::DoCvMzMove()
{
	CString strMc, strPlc;
	if (!GetCvCtx(strMc, strPlc)) return;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("매거진(108번)으로 이동시키겠습니까?")), MB_YESNO) != IDYES)
		return;

	CString strSql;
	strSql.Format(_T(" UPDATE CV_DATA SET LUGG_NO_OD = '9999', DEST_POS_OD = '108', JOB_TYP_OD = '6' ")
		_T(" , WRITE_UPD_DT = ") + m_pDoc->SYSDATE + _T(" , OD_RQ_YN = 'Y' ")
		_T(" WHERE WH_TYP = '%s' AND PLC_NO = '%02s' AND MC_NO = '%s' AND OD_RQ_YN = 'N'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strPlc, (LPCTSTR)strMc);
	CString strLog;
	strLog.Format(_T("MZ 이동 지시 -> 트랙번호 : %s , 작업구분 : 6 , 도착지 : 108"), (LPCTSTR)strMc);
	if (ExecUpdateEx(strSql, strLog, _T("9999"), _T("CCvSkinDlg"), UPD_YN))
		Refresh();
}

// ---------------- SC / RTV : CMD_RQ 계열 명령 ----------------
void CPanelInfoDlg::DoVehCmd(BOOL bRtv, LPCTSTR strCmdIdIn, LPCTSTR strAskMsg)
{
	CString strNo, strPlc;
	if (!GetVehCtx(bRtv, strNo, strPlc)) return;
	CString strCmdId = strCmdIdIn;

	CString strErr, strLugg;
	if (!bRtv)
	{
		CString strSqlR;
		strSqlR.Format(_T("SELECT ") + m_pDoc->NVL + _T("(ERR_CODE_RD,'') AS ERR_CODE_RD, ") + m_pDoc->NVL + _T("(ITN_LUGG_FK1,'') AS ITN_LUGG_FK1 ")
			_T(" FROM SC_DATA_LGLS WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
			(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strNo);
		int nCnt = -1; CString strMsgQ;
		_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSqlR, nCnt, strMsgQ);
		if (nCnt > 0)
		{
			CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
			pRsw->MoveFirst();
			strErr  = pRsw->GetItem(_T("ERR_CODE_RD"));  strErr.Trim();
			strLugg = pRsw->GetItem(_T("ITN_LUGG_FK1")); strLugg.Trim();
			delete pRsw;
		}
	}

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(strAskMsg), MB_YESNO) != IDYES)
		return;

	// ERROR RESET : 공출고/이중입고 상태 재확인 (ScSkinDlg 와 동일)
	if (!bRtv && strCmdId == _T("RESET"))
	{
		if (strErr == _T("0058") || strErr == _T("0059"))
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 공출고 에러상태 입니다. 에러해제 시 수동조작을 해야합니다. 그래도 하시겠습니까?")), MB_YESNO) != IDYES)
				return;
		}
		if (strErr == _T("0054") || strErr == _T("0055"))
		{
			if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 이중입고 에러상태 입니다. 에러해제 시 수동조작을 해야합니다. 그래도 하시겠습니까?")), MB_YESNO) != IDYES)
				return;
		}
	}

	CString strSql;
	if (bRtv)
	{
		strSql.Format(_T(" UPDATE RTV_DATA_LGLS SET CMD_RQ_ID = '%s', CMD_RQ_YN = 'Y' ")
			_T(" WHERE WH_TYP = '%s' AND PLC_NO = '%02s' AND RTV_NO = '%s'"),
			(LPCTSTR)strCmdId, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strPlc, (LPCTSTR)strNo);
		// 삭제는 지시(OD) 도 함께 비운다 (RtvSkinDlg 와 동일)
		if (strCmdId == _T("DELFK1"))
		{
			CString strClr;
			strClr.Format(_T(" UPDATE RTV_DATA_LGLS SET LUGG_OD = '0000', PALLET_ID_OD = '0000', JOB_TYP_OD = '0' ")
				_T(" , FROM_01_OD = '00', FROM_02_OD = '00', FROM_03_OD = '00' ")
				_T(" , TO_01_OD = '00', TO_02_OD = '00', TO_03_OD = '00' ")
				_T(" , RTV_DEST_OD = '', RTV_PASSCV_OD = '' ")
				_T(" WHERE WH_TYP = '%s' AND RTV_NO = '%s'"),
				(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strNo);
			strSql = strSql + _T(" ") + strClr;
		}
	}
	else
	{
		// 공출고 에러 상태의 삭제는 ECS 작업정보도 지운다 (ScSkinDlg 와 동일)
		if (strCmdId == _T("DELFK1") && (strErr == _T("0058") || strErr == _T("0059")) && !strLugg.IsEmpty() && strLugg != _T("0"))
		{
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("현재 공출고 에러상태 입니다. ECS 작업도 삭제됩니다. WMS 작업도 삭제해주세요.")));
			CString strDel;
			strDel.Format(_T(" DELETE FROM JOB_MST WHERE WH_TYP = '%s' AND LUGG_NO = '%s' "),
				(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strLugg);
			strSql += strDel;
		}
		CString strUpd;
		strUpd.Format(_T(" UPDATE SC_DATA_LGLS SET CMD_RQ_ID = '%s', CMD_RQ_YN = 'Y', READ_UPD_DT = ") + m_pDoc->SYSDATE +
			_T(" WHERE WH_TYP = '%s' AND PLC_NO = '%s' AND MC_NO = '%s'"),
			(LPCTSTR)strCmdId, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strPlc, (LPCTSTR)strNo);
		strSql += strUpd;
	}

	CString strLog;
	strLog.Format(_T("%s ACTION BUTTON -> NO : %s , ACTION : %s"),
		bRtv ? _T("RTV") : _T("SC"), (LPCTSTR)strNo, (LPCTSTR)strCmdId);
	if (ExecUpdateEx(strSql, strLog, strLugg, bRtv ? _T("CRtvSkinDlg") : _T("CScSkinDlg"), EXE_YN))
		Refresh();
}

// ---------------- SC : 입고/출고/입출고 금지 ----------------
//   nKind 1 = 입고금지(STO), 2 = 출고금지(RET), 3 = 입출고금지(ALL)
void CPanelInfoDlg::DoScSuspend(int nKind)
{
	CString strNo, strPlc;
	if (!GetVehCtx(FALSE, strNo, strPlc)) return;

	CString strSql;
	strSql.Format(_T("SELECT ") + m_pDoc->NVL + _T("(SUSPEND,'0') AS SUSPEND FROM SC_DATA_LGLS WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strNo);
	int nCur = _ttoi(QueryOne(strSql, _T("SUSPEND")));

	CString strNew, strAsk;
	if (nKind == 1)
	{
		switch (nCur) {
		case 0: strNew = _T("1"); strAsk = _T("S/C 입고금지 하시겠습니까?");   break;
		case 1: strNew = _T("0"); strAsk = _T("S/C 정상처리 하시겠습니까?");   break;
		case 2: strNew = _T("3"); strAsk = _T("S/C 입출고금지 하시겠습니까?"); break;
		default:strNew = _T("2"); strAsk = _T("S/C 출고금지 하시겠습니까?");   break;
		}
	}
	else if (nKind == 2)
	{
		switch (nCur) {
		case 0: strNew = _T("2"); strAsk = _T("S/C 출고금지 하시겠습니까?");   break;
		case 1: strNew = _T("3"); strAsk = _T("S/C 입출고금지 하시겠습니까?"); break;
		case 2: strNew = _T("0"); strAsk = _T("S/C 정상처리 하시겠습니까?");   break;
		default:strNew = _T("1"); strAsk = _T("S/C 입고금지 하시겠습니까?");   break;
		}
	}
	else
	{
		if (nCur == 3) { strNew = _T("0"); strAsk = _T("S/C 정상처리 하시겠습니까?"); }
		else           { strNew = _T("3"); strAsk = _T("S/C 입출고금지 하시겠습니까?"); }
	}

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(strAsk), MB_YESNO) != IDYES)
		return;

	strSql.Format(_T(" UPDATE SC_DATA_LGLS SET SUSPEND = '%s' WHERE WH_TYP = '%s' AND PLC_NO = '%02s' AND SC_NO = '%s'"),
		(LPCTSTR)strNew, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strPlc, (LPCTSTR)strNo);
	CString strLog;
	strLog.Format(_T("SC SUSPEND BUTTON -> SC NO : %s , 기존 %d , 변경 %s"), (LPCTSTR)strNo, nCur, (LPCTSTR)strNew);
	if (ExecUpdateEx(strSql, strLog, _T("0"), _T("CScSkinDlg"), EXE_YN))
		Refresh();
}

// ---------------- SC : 이중입고 ----------------
void CPanelInfoDlg::DoScDuplSto()
{
	CString strNo, strPlc;
	if (!GetVehCtx(FALSE, strNo, strPlc)) return;

	CString strSql;
	strSql.Format(_T("SELECT COUNT(1) AS CNT FROM SC_DATA_LGLS WHERE WH_TYP = '%s' AND EQP_ERR_CD = 'E054' AND SC_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strNo);
	if (QueryOne(strSql, _T("CNT")) == _T("0"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("이중입고(E054) 상태가 아닙니다")));
		return;
	}

	strSql.Format(_T("SELECT ") + m_pDoc->NVL + _T("(LUGG_NO_FK1_RD,'') AS LUGG_NO_FK1_RD FROM SC_DATA_LGLS WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strNo);
	CString strLugg = QueryOne(strSql, _T("LUGG_NO_FK1_RD"));
	if (strLugg.IsEmpty()) return;

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("이중입고 처리 하시겠습니까?")) + _T(" [") + strLugg + _T("]"), MB_YESNO) != IDYES)
		return;

	strSql.Format(_T("UPDATE JOB_MST SET JOB_STA = '44' WHERE WH_TYP = '%s' AND LUGG_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strLugg);
	if (ExecUpdateEx(strSql, _T("SC 이중입고 처리 -> JOB_STA 44 : ") + strLugg, strLugg, _T("CScSkinDlg"), EXE_YN))
		Refresh();
}

// ---------------- RTV : 사용중지 / 해제 ----------------
void CPanelInfoDlg::DoRtvSuspend()
{
	CString strNo, strPlc;
	if (!GetVehCtx(TRUE, strNo, strPlc)) return;

	CString strSql;
	strSql.Format(_T("SELECT COUNT(1) AS CNT FROM RTV_DATA_LGLS WHERE WH_TYP = '%s' AND PLC_NO = '%02s' AND RTV_NO = '%s' AND SUSPEND IN ('1')"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strPlc, (LPCTSTR)strNo);
	BOOL bSuspended = (QueryOne(strSql, _T("CNT")) != _T("0"));

	if (!bSuspended)
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("Rtv 사용을 중지하시겠습니까?")), MB_YESNO) != IDYES) return;
	}
	else
	{
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("Rtv를 사용하시겠습니까?")), MB_YESNO) != IDYES) return;
	}

	strSql.Format(_T("UPDATE RTV_DATA_LGLS SET SUSPEND = 1 - SUSPEND WHERE WH_TYP = '%s' AND PLC_NO = '%02s' AND RTV_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strPlc, (LPCTSTR)strNo);
	CString strLog;
	strLog.Format(_T("RTV SUSPEND BUTTON -> RTV_NO : %s , %s"), (LPCTSTR)strNo, bSuspended ? _T("Release") : _T("Suspend"));
	if (ExecUpdateEx(strSql, strLog, _T("0"), _T("CRtvSkinDlg"), EXE_YN))
		Refresh();
}

void CPanelInfoDlg::OnCmdBtn(UINT nID)
{
	if (m_pDoc == NULL) return;
	int nCmd = (int)nID - IDC_PI_CMD_BASE;
	if (nCmd < 0 || nCmd >= CMD_COUNT) return;

	switch (nCmd)
	{
	case CMD_CV_COPY:        DoCvCopy(FALSE); break;
	case CMD_CV_CUT:         DoCvCopy(TRUE);  break;
	case CMD_CV_PASTE:       DoCvPaste();     break;
	case CMD_CV_TRACK_PAUSE: DoCvTrackPause();break;
	case CMD_CV_MZ_MOVE:     DoCvMzMove();    break;

	case CMD_SC_ACTIVE:      DoVehCmd(FALSE, _T("ACTIVE"),    _T("ACTIVE를 진행하시겠습니까?"));      break;
	case CMD_SC_STOP:        DoVehCmd(FALSE, _T("PAUSE"),     _T("STOP을 진행하시겠습니까?"));        break;
	case CMD_SC_ERR_RESET:   DoVehCmd(FALSE, _T("RESET"),     _T("ERROR RESET을 진행하시겠습니까?")); break;
	case CMD_SC_CTH:         DoVehCmd(FALSE, _T("CTH"),       _T("홈복귀를 진행하시겠습니까?"));      break;
	case CMD_SC_EMERGENCY:   DoVehCmd(FALSE, _T("EMERGENCY"), _T("비상정지를 진행하시겠습니까?"));    break;
	case CMD_SC_STO_SUSPEND: DoScSuspend(1); break;
	case CMD_SC_RET_SUSPEND: DoScSuspend(2); break;
	case CMD_SC_ALL_SUSPEND: DoScSuspend(3); break;
	case CMD_SC_DUPL_STO:    DoScDuplSto();  break;
	case CMD_SC_DELETE:      DoVehCmd(FALSE, _T("DELFK1"),    _T("삭제를 진행하시겠습니까?"));        break;

	case CMD_RTV_ACTIVE:     DoVehCmd(TRUE,  _T("ACTIVE"),    _T("ACTIVE를 진행하시겠습니까?"));      break;
	case CMD_RTV_STOP:       DoVehCmd(TRUE,  _T("PAUSE"),     _T("STOP을 진행하시겠습니까?"));        break;
	case CMD_RTV_ERR_RESET:  DoVehCmd(TRUE,  _T("RESET"),     _T("ERROR RESET을 진행하시겠습니까?")); break;
	case CMD_RTV_CTH:        DoVehCmd(TRUE,  _T("CTH"),       _T("홈복귀를 진행하시겠습니까?"));      break;
	case CMD_RTV_EMERGENCY:  DoVehCmd(TRUE,  _T("EMERGENCY"), _T("비상정지를 진행하시겠습니까?"));    break;
	case CMD_RTV_SUSPEND:    DoRtvSuspend(); break;
	case CMD_RTV_COMPLETE:   DoVehCmd(TRUE,  _T("FCMP"),      _T("강제완료를 진행하시겠습니까?"));    break;
	case CMD_RTV_DELETE:     DoVehCmd(TRUE,  _T("DELFK1"),    _T("삭제를 진행하시겠습니까?"));        break;
	}
}
