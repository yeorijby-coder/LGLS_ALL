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

CPanelInfoDlg::CPanelInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelInfoDlg::IDD, pParent)
{
	m_pDoc = NULL;
	m_nRefreshRows = 0;
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
	m_btnCvDelete.SetWindowText(_T("지시 삭제"));

	CFont* pFont = GetFont();
	m_cmbStatus.SetFont(pFont); m_btnStatus.SetFont(pFont);
	m_cmbPri.SetFont(pFont);    m_btnPri.SetFont(pFont);
	m_edtCvJob.SetFont(pFont);  m_btnCvWrite.SetFont(pFont);
	m_btnForce.SetFont(pFont);  m_btnCvDelete.SetFont(pFont);

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
	if (bShow) pCtrl->BringWindowToTop();
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
		m_list.SetItemText(i, 0, strCap);
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

// [LGLS 2026-09-02] 오버레이 배치 (항상 표시 - 스크롤/갱신 때 위치만 갱신)
void CPanelInfoDlg::PlaceOverlays()
{
	int nTab = m_tab.GetCurSel();
	if (nTab == TAB_JOB && m_list.GetItemCount() > JOB_ROW_PRI)
	{
		CString strStatus = m_list.GetItemText(JOB_ROW_STATUS, 1);
		for (int i = 0; i < m_arStatusCd.GetCount(); i++)
			if (!strStatus.IsEmpty() && strStatus.Find(_T("[") + m_arStatusCd[i] + _T("]")) == 0)
			{ m_cmbStatus.SetCurSel(i); break; }
		CString strPri = m_list.GetItemText(JOB_ROW_PRI, 1); strPri.Trim();
		int nPri = _ttoi(strPri);
		if (nPri >= 1 && nPri <= 9) m_cmbPri.SetCurSel(nPri - 1);

		PlaceOverCell(&m_cmbStatus, JOB_ROW_STATUS, 2, TRUE);
		PlaceOverCell(&m_btnStatus, JOB_ROW_STATUS, 3, TRUE);
		PlaceOverCell(&m_cmbPri,    JOB_ROW_PRI, 2, TRUE);
		PlaceOverCell(&m_btnPri,    JOB_ROW_PRI, 3, TRUE);
	}
	else if (nTab == TAB_CV && m_list.GetItemCount() > CV_ROW_WRITE)
	{
		PlaceOverCell(&m_edtCvJob,   CV_ROW_WRITE, 2, TRUE);
		PlaceOverCell(&m_btnCvWrite, CV_ROW_WRITE, 3, TRUE);
		m_btnCvDelete.ShowWindow(SW_SHOW);
	}
	else if ((nTab == TAB_SC || nTab == TAB_RTV) && m_list.GetItemCount() > VEH_ROW_FORCE)
	{
		PlaceOverCell(&m_btnForce, VEH_ROW_FORCE, 3, TRUE);
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

void CPanelInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (::IsWindow(m_tab.m_hWnd))
		m_tab.MoveWindow(0, 0, cx, 24);
	if (::IsWindow(m_cmbUnit.m_hWnd))
		m_cmbUnit.MoveWindow(cx - 92, 1, 90, 160);
	if (::IsWindow(m_list.m_hWnd))
		m_list.MoveWindow(0, 26, cx, cy - 26 - 26);
	if (::IsWindow(m_btnCvDelete.m_hWnd))
		m_btnCvDelete.MoveWindow(2, cy - 24, 90, 22);
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
