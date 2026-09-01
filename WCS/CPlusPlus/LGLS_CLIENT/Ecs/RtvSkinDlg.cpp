// RtvSkinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include <uxtheme.h>	// [LGLS 2026-08-05] 그룹박스 비주얼스타일 해제용
#pragma comment(lib, "uxtheme.lib")
#include "Ecs.h"
#include "RtvSkinDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CRtvSkinDlg 대화 상자입니다.
IMPLEMENT_DYNAMIC(CRtvSkinDlg, CSkinDialog)


CRtvSkinDlg::CRtvSkinDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CRtvSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pRTV_DATA = NULL;
	m_brLedOn = NULL; m_brLedOff = NULL; m_brStatus = NULL;
}
CRtvSkinDlg::CRtvSkinDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CRtvSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pRTV_DATA = NULL;
	m_brLedOn = NULL; m_brLedOff = NULL; m_brStatus = NULL;
}

CRtvSkinDlg::~CRtvSkinDlg()
{
	m_pDoc->m_pRtvSkinDlg = NULL;
	if(m_brLedOn) ::DeleteObject(m_brLedOn);
	if(m_brLedOff) ::DeleteObject(m_brLedOff);
	if(m_brStatus) ::DeleteObject(m_brStatus);
	this->DestroyWindow();
}

// [LGLS 2026-08-31] 작업구분을 "[코드]명칭" 으로 표기한다(작업상태 표기와 통일).
static CString FormatJobTyp(const CString& strTyp)
{
	CString t(strTyp); t.Trim();
	if (t.IsEmpty() || t == _T("0")) return _T("-");

	LPCTSTR pszNm = _T("");
	if      (t == _T("1"))  pszNm = _T("입고");
	else if (t == _T("2"))  pszNm = _T("출고");
	else if (t == _T("3"))  pszNm = _T("피킹출고");
	else if (t == _T("4"))  pszNm = _T("랙이동");
	else if (t == _T("5"))  pszNm = _T("호기간이동");
	else if (t == _T("6"))  pszNm = _T("이동");
	else if (t == _T("11")) pszNm = _T("반자동입고");
	else if (t == _T("12")) pszNm = _T("반자동출고");
	else if (t == _T("13")) pszNm = _T("반자동랙이동");
	else if (t == _T("14")) pszNm = _T("반자동피킹출고");
	else if (t == _T("15")) pszNm = _T("반자동이동");

	CString s;
	s.Format(_T("[%s]%s"), (LPCTSTR)t, pszNm);
	return s;
}

void CRtvSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	// [LGLS 2026-08-05] DDX 가 없어서 명령 버튼이 CSkinButton 에 붙지 않았고,
	//   이미 있던 SetBitmaps/SetIcon 이 전부 무효(m_hWnd=NULL)라 아이콘이 안 나왔다.
	DDX_Control(pDX, IDC_BTN_RTV_COMPLETE,      m_btnRtvComplete);
	DDX_Control(pDX, IDC_BTN_RTV_ESTOP,         m_btnRtvEstop);
	DDX_Control(pDX, IDC_BTN_RTV_ACTIVE,        m_btnRtvActive);
	DDX_Control(pDX, IDC_BTN_RTV_STOP,          m_btnRtvStop);
	DDX_Control(pDX, IDC_BTN_RTV_RESET_ERROR,   m_btnRtvResetError);
	DDX_Control(pDX, IDC_BTN_RTV_DELETE,        m_btnRtvDelete);
	DDX_Control(pDX, IDC_LGLS_RTV_ZOOM,         m_btnVehZoom);	// [LGLS 2026-08-05] 확대/축소
	DDX_Control(pDX, IDC_LGLS_RTV_RESEND,       m_btnRtvResend);	// [LGLS 2026-08-12] 지시 재전송
	DDX_Control(pDX, IDC_BTN_RTV_CALL_TO_HOME,  m_btnRtvCallToHome);
	DDX_Control(pDX, IDC_BTN_RTV_MANUAL,        m_btnRtvManual);
	DDX_Control(pDX, IDC_BTN_RTV_SUSPEND,       m_btnRtvSuspend);
}

BEGIN_MESSAGE_MAP(CRtvSkinDlg, CSkinDialog)
		ON_MESSAGE(WM_USER_DIALOG_MESSAGE_REFRESH, &CRtvSkinDlg::OnMessagSwitch)
		ON_WM_TIMER()
		ON_WM_CLOSE()
		ON_BN_CLICKED(IDC_BTN_RTV_COMPLETE, &CRtvSkinDlg::OnBnClickedBtnRtvComplete)
		ON_BN_CLICKED(IDC_BTN_RTV_ESTOP, &CRtvSkinDlg::OnBnClickedBtnRtvEstop)
		ON_BN_CLICKED(IDC_BTN_RTV_ACTIVE, &CRtvSkinDlg::OnBnClickedBtnRtvActive)
		ON_BN_CLICKED(IDC_BTN_RTV_STOP, &CRtvSkinDlg::OnBnClickedBtnRtvStop)
		ON_BN_CLICKED(IDC_BTN_RTV_RESET_ERROR, &CRtvSkinDlg::OnBnClickedBtnRtvResetError)
		ON_BN_CLICKED(IDC_BTN_RTV_DELETE, &CRtvSkinDlg::OnBnClickedBtnRtvDelete)
		ON_BN_CLICKED(IDC_BTN_RTV_CALL_TO_HOME, &CRtvSkinDlg::OnBnClickedBtnRtvCallToHome)
		ON_BN_CLICKED(IDC_BTN_RTV_SUSPEND, &CRtvSkinDlg::OnBnClickedBtnRtvSuspend)
		ON_BN_CLICKED(IDC_CHK_RTV_FORK1, &CRtvSkinDlg::OnBnClickedChkRtvFork1)
		ON_BN_CLICKED(IDC_CHK_RTV_FORK2, &CRtvSkinDlg::OnBnClickedChkRtvFork2)
		ON_BN_CLICKED(IDC_CHK_RTV_FORK12, &CRtvSkinDlg::OnBnClickedChkRtvFork12)
		ON_WM_CTLCOLOR()
		ON_BN_CLICKED(IDC_RTVV_RESEND, &CRtvSkinDlg::OnBnClickedRtvvResend)
	ON_BN_CLICKED(IDC_LGLS_RTV_RESEND, &CRtvSkinDlg::OnBnClickedRtvvResend)	// [LGLS 2026-08-12] 원 대화상자 버튼(동일 기능)
		ON_BN_CLICKED(IDC_RTVV_OK, &CRtvSkinDlg::OnBnClickedRtvvOk)
	ON_BN_CLICKED(IDC_LGLS_RTV_ZOOM, &CRtvSkinDlg::OnBnClickedVehZoom)
END_MESSAGE_MAP()


BOOL CRtvSkinDlg::OnInitDialog()
{
	m_bDisableMaximize = TRUE;	// [LGLS] 최대화 버튼 숨김+크기고정
	CSkinDialog::OnInitDialog();
	SetTimer(1, 1500, NULL);

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		RelocationControls();
		m_bInitialized = TRUE;		
	}
	m_strRtvFork = _T("0");

	InitializeResource(pEn);

	m_brLedOn = ::CreateSolidBrush(RGB(0,200,0)); m_brLedOff = ::CreateSolidBrush(RGB(90,90,90)); m_brStatus = ::CreateSolidBrush(RGB(255,255,150));	// [LGLS] RTV 싱글 포크: Fork2 계열 숨김 + Fork1 라벨 정리
	BuildVehStatusPanel();	// [LGLS 2026-08-01] 통신상태 패널 생성(rc 미포함 컨트롤 보완)
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// [LGLS] RTV는 싱글 포크이므로 Fork2 관련 컨트롤과 포크 선택 체크박스를 숨긴다.



BOOL CRtvSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int a = 0;
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CRtvSkinDlg::InvalidateRtvData(EN_LANG pLang)
{
	if(m_pDoc == NULL) return;
	if(m_pRTV_DATA == NULL) return;
	CSingleLock _lockRtvRead(&m_pDoc->m_csEqpData, TRUE);   // [LGLS] 수집 thread write serialize (refcount assert guard)

	int nPlc = CConvert::ToInt(m_pRTV_DATA->K_PLC_NO);
	// [LGLS] 설비명 = RGV N호기 (상단 박스 + 창 제목)
	CString strEqpNm;
	strEqpNm.Format(_T("RGV %d호기"), nPlc);
	SetDlgItemText(IDC_EDIT_RTV_NO, strEqpNm);
	{
		TCHAR _cf[MAX_PATH] = {0}; GetModuleFileName(NULL, _cf, MAX_PATH);
		CString _ap = _cf;
		CString _fp = Global.GetConcatPath(_ap.Left(_ap.ReverseFind(_T('\\'))) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), _T(".ini"));
		CString _cap = CLib::GetIniStringFromPath(_fp, _T("dlgname"), (int)pLang);
		if(_cap.IsEmpty()) _cap = _T("RTV 상태");
		_cap += _T(" - ") + strEqpNm;
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
		+ N + _T("(SENSOR_RTV_RD,'0') AS A7, ")
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
		+ N + _T("(JOB_TYP_OD,'') AS JT, ")
		+ N + _T("(SUSPEND,'0') AS SUS, ")
		+ N + _T("(AUTO_MODE_RD,'0') AS AUT, ") + N + _T("(ERR_CODE_RD,'0000') AS ERR, ")
		+ N + _T("(POS_H_RD,'0') AS PH, ") + N + _T("(COMPLETE_RD,'0') AS CMPRD, ")
		+ N + _T("(LUGG_OD,'0') AS LOD, ") + N + _T("(PALLET_ON_VEHICLE_RD,'0') AS ITN ")
		+ _T("FROM RTV_DATA_LGLS WHERE WH_TYP='") + m_pDoc->m_WH_TYP + _T("' AND PLC_NO='") + m_pRTV_DATA->K_PLC_NO + _T("' AND RTV_NO='") + m_pRTV_DATA->K_RTV_NO + _T("'");

	CString strMessage; int nRowCnt = -1;
	_RecordsetPtr ptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if(nRowCnt > 0)
	{
		CRecordSetWrap* pRsw = new CRecordSetWrap(ptr);
		pRsw->MoveFirst();
		CString st = pRsw->GetItem(_T("ST"));
		// [LGLS 2026-09-01] 원시값을 함께 표기 (사용자 요청) - 시나리오 문서의
		//   "D0210 IDLE=1 확인" 과 그대로 대응되게 "1 = IDLE" 형식으로 보인다.
		CString stMean = (st == _T("2")) ? _T("RUN") : ((st == _T("1")) ? _T("IDLE") : _T("DOWN"));
		CString stTxt; stTxt.Format(_T("%s = %s"), (LPCTSTR)st, (LPCTSTR)stMean);
		SetDlgItemText(IDC_RTVV_STATUS, stTxt);
		SetLed(IDC_RTVV_LED_LOAD_CMP,        pRsw->GetItem(_T("A1")));
		SetLed(IDC_RTVV_LED_LOAD_CMP_ACK,    pRsw->GetItem(_T("A2")));
		SetLed(IDC_RTVV_LED_UNLOAD_CMP,      pRsw->GetItem(_T("A3")));
		SetLed(IDC_RTVV_LED_UNLOAD_CMP_ACK,  pRsw->GetItem(_T("A4")));
		SetLed(IDC_RTVV_LED_TR_REQ,          pRsw->GetItem(_T("A5")));
		SetLed(IDC_RTVV_LED_TR_REQ_ACK,      pRsw->GetItem(_T("A6")));
		SetLed(IDC_RTVV_LED_PALLET_EXIST,    pRsw->GetItem(_T("A7")));
		SetLed(IDC_RTVV_LED_ALARM_SET,       pRsw->GetItem(_T("A8")));
		SetLed(IDC_RTVV_LED_ALARM_SET_ACK,   pRsw->GetItem(_T("A9")));
		SetLed(IDC_RTVV_LED_ALARM_RESET,     pRsw->GetItem(_T("A10")));
		SetLed(IDC_RTVV_LED_ALARM_RESET_ACK, pRsw->GetItem(_T("A11")));
		SetDlgItemText(IDC_RTVV_PALLET_ID, pRsw->GetItem(_T("PID")));
		SetDlgItemText(IDC_RTVV_CUR1, pRsw->GetItem(_T("L1")));
		SetDlgItemText(IDC_RTVV_CUR2, pRsw->GetItem(_T("L2")));
		SetDlgItemText(IDC_RTVV_CUR3, pRsw->GetItem(_T("L3")));
		SetDlgItemText(IDC_RTVV_FROM1, pRsw->GetItem(_T("F1")));
		SetDlgItemText(IDC_RTVV_FROM2, pRsw->GetItem(_T("F2")));
		SetDlgItemText(IDC_RTVV_FROM3, pRsw->GetItem(_T("F3")));
		SetDlgItemText(IDC_RTVV_TO1, pRsw->GetItem(_T("T1")));
		SetDlgItemText(IDC_RTVV_TO2, pRsw->GetItem(_T("T2")));
		SetDlgItemText(IDC_RTVV_TO3, pRsw->GetItem(_T("T3")));
		SetDlgItemText(IDC_RTVV_CMP1, pRsw->GetItem(_T("C1")));
		SetDlgItemText(IDC_RTVV_CMP2, pRsw->GetItem(_T("C2")));
		SetDlgItemText(IDC_RTVV_CMP3, pRsw->GetItem(_T("C3")));
		SetDlgItemText(IDC_RTVV_ALARM_CODE, pRsw->GetItem(_T("AC")));
		SetDlgItemText(IDC_RTVV_PALLET, pRsw->GetItem(_T("POD")));
		CString jt = pRsw->GetItem(_T("JT"));
		CString io = (jt == _T("1")) ? _T("입고") : ((jt == _T("2")) ? _T("출고") : _T(""));
		SetDlgItemText(IDC_RTVV_IO_TAG, io);
		CString sus = pRsw->GetItem(_T("SUS"));
		CheckDlgButton(IDC_BTN_RTV_SUSPEND, (sus == _T("1")) ? BST_CHECKED : BST_UNCHECKED);

		// [LGLS 2026-08-05] 제목 두 칸 - SC 와 같은 규칙(호기명 + 설비명). 채우는 코드가 없어 비어 있었다.
		{
			CString strT1; strT1.Format(_T("RGV %d호기"), CConvert::ToInt(m_pRTV_DATA->K_PLC_NO));
			CString strT2; strT2.Format(_T("RTV %s"), (LPCTSTR)m_pRTV_DATA->K_RTV_NO);
			SetDlgItemText(IDC_RTVV_TITLE1, strT1);
			SetDlgItemText(IDC_RTVV_TITLE2, strT2);
		}

		// [LGLS 2026-08-01] 대화상자 상단(RTV 상태/작업) 항목 채우기 : 기존엔 통신상태 패널만 채워 비어 있었다.
		{
			CString aut = pRsw->GetItem(_T("AUT"));
			CString err = pRsw->GetItem(_T("ERR"));
			CString cmp = pRsw->GetItem(_T("CMPRD"));
			CString sen = pRsw->GetItem(_T("A7"));
			CString lod = pRsw->GetItem(_T("LOD"));

			SetDlgItemText(IDC_EDT_RTV_MODE,        (aut == _T("1")) ? _T("자동") : _T("수동"));
			SetDlgItemText(IDC_EDT_RTV_ACTIVE,      (st == _T("2")) ? _T("가동") : _T("대기"));
			SetDlgItemText(IDC_EDT_RTV_STATUS,      stTxt);
			SetDlgItemText(IDC_EDT_RTV_COMPLETE,    (cmp == _T("1")) ? _T("완료") : _T("-"));
			SetDlgItemText(IDC_EDT_RTV_PRODLOAD,    (sen == _T("1")) ? _T("있음") : _T("없음"));
			SetDlgItemText(IDC_EDT_RTV_HORIZONTAL_POS, pRsw->GetItem(_T("PH")));
			SetDlgItemText(IDC_EDT_RTV_JISANG_MODE, (err == _T("0000") || err == _T("0")) ? _T("정상") : (_T("에러 ") + err));
			// [LGLS 2026-08-05] 에러 그룹에 화물번호(lod)가 나오던 버그 - 에러코드(ERR_CODE_RD)로 교정
			SetDlgItemText(IDC_EDT_RTV_DIAGNOSIS,   (err.IsEmpty() || err == _T("0") || err == _T("0000")) ? _T("-") : err);
			SetDlgItemText(IDC_EDT_RTV_SUSPEND,     (sus == _T("1")) ? _T("정지") : _T("-"));

			CString sLoc, dLoc;
			sLoc.Format(_T("%s-%s-%s"), pRsw->GetItem(_T("F1")), pRsw->GetItem(_T("F2")), pRsw->GetItem(_T("F3")));
			dLoc.Format(_T("%s-%s-%s"), pRsw->GetItem(_T("T1")), pRsw->GetItem(_T("T2")), pRsw->GetItem(_T("T3")));
			SetDlgItemText(IDC_EDT_RTV_JOB_START_LOC, sLoc);
			SetDlgItemText(IDC_EDT_RTV_JOB_DEST_LOC,  dLoc);

			// [LGLS 2026-08-05] RTV상태 그룹이 계속 비어 있던 부분.
			//   SC 대화상자와 같은 출처를 쓴다 : 작업구분/출발·도착위치는 PLC 로 내보낸 지시값(_OD),
			//   작업번호는 PLC 가 돌려주는 실적재 화물번호(PALLET_ON_VEHICLE_RD).
			CString itn = pRsw->GetItem(_T("ITN"));
			if (itn == _T("0") || itn == _T("0000") || itn.IsEmpty()) itn = lod;
			// [LGLS 2026-08-31] 작업상태와 같은 "[코드]명칭" 표기로 통일한다(사용자 요구).
			//   종전에는 1/2 만 알아서 반자동(11/12)이 "-" 로 나왔다.
			CString jtTxt = FormatJobTyp(jt);
			SetDlgItemText(IDC_EDIT_RTV_JOB_NO,   (itn == _T("0") || itn == _T("0000") || itn.IsEmpty()) ? _T("-") : itn);
			SetDlgItemText(IDC_CBX_RTV_JOB_TYP,   jtTxt);
			SetDlgItemText(IDC_CBX_RTV_START_POS, sLoc);
			SetDlgItemText(IDC_CBX_RTV_DEST_POS,  dLoc);

			// [LGLS 2026-08-05] PLC 에서 받아오는 현재위치/완료위치도 RTV상태 그룹에 표시
			CString curLoc, cmpLoc;
			curLoc.Format(_T("%s-%s-%s"), pRsw->GetItem(_T("L1")), pRsw->GetItem(_T("L2")), pRsw->GetItem(_T("L3")));
			cmpLoc.Format(_T("%s-%s-%s"), pRsw->GetItem(_T("C1")), pRsw->GetItem(_T("C2")), pRsw->GetItem(_T("C3")));
			SetDlgItemText(IDC_LGLS_RTV_CUR_VAL, curLoc);
			SetDlgItemText(IDC_LGLS_RTV_CMP_VAL, cmpLoc);
		}
		delete pRsw;
	}

	strSql = GetQrySelectJOB_MST(m_pRTV_DATA);
	nRowCnt = -1;
	_RecordsetPtr ptr2 = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if(nRowCnt > 0)
	{
		CRecordSetWrap* pRsw2 = new CRecordSetWrap(ptr2);
		pRsw2->MoveFirst();
		// [LGLS] 작업정보를 실제 rc 컨트롤(하단 JOB_STATUS 그룹)에 바인딩 (IDC_RTVV_*는 rc 미배치라 무효였음)
		SetDlgItemText(IDC_EDIT_RTV_JOB_JOB_NO,    pRsw2->GetItem(_T("LUGG_NO")));		// 작업번호
		SetDlgItemText(IDC_CBX_RTV_JOB_JOB_TYP,    pRsw2->GetItem(_T("JOB_TYP")));		// 작업구분
		SetDlgItemText(IDC_CBX_RTV_JOB_JOB_STATUS, pRsw2->GetItem(_T("JOB_STATUS")));	// 작업상태
		SetDlgItemText(IDC_CBX_RTV_JOB_START_POS,  pRsw2->GetItem(_T("START_POS")));		// 출발위치
		SetDlgItemText(IDC_EDT_RTV_JOB_START_LOC,  pRsw2->GetItem(_T("START_LOCATION")));	// 출발지
		SetDlgItemText(IDC_CBX_RTV_JOB_DEST_POS,   pRsw2->GetItem(_T("DEST_POS")));		// 도착위치
		SetDlgItemText(IDC_EDT_RTV_JOB_DEST_LOC,   pRsw2->GetItem(_T("DEST_LOCATION")));
		SetDlgItemText(IDC_LGLS_RTV_LOT_VAL,       pRsw2->GetItem(_T("LOT_NO")));		// [LGLS] 적재 용기
		SetDlgItemText(IDC_LGLS_RTV_PRD_VAL,       pRsw2->GetItem(_T("PRODUCT_ID")));	// [LGLS] 제품 정보	// 도착지
		delete pRsw2;
	}
	else
	{	// [LGLS 2026-08-01] 조회 실패/무작업 시 이전 값이 남지 않도록 비운다
		SetDlgItemText(IDC_EDIT_RTV_JOB_JOB_NO,    _T("-"));
		SetDlgItemText(IDC_CBX_RTV_JOB_JOB_TYP,    _T(""));
		SetDlgItemText(IDC_CBX_RTV_JOB_JOB_STATUS, _T(""));
		SetDlgItemText(IDC_CBX_RTV_JOB_START_POS,  _T(""));
		SetDlgItemText(IDC_EDT_RTV_JOB_START_LOC,  _T(""));
		SetDlgItemText(IDC_CBX_RTV_JOB_DEST_POS,   _T(""));
		SetDlgItemText(IDC_EDT_RTV_JOB_DEST_LOC,   _T(""));
		SetDlgItemText(IDC_LGLS_RTV_LOT_VAL,       _T(""));
		SetDlgItemText(IDC_LGLS_RTV_PRD_VAL,       _T(""));
	}
}

void CRtvSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}
HCURSOR CRtvSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRtvSkinDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CRtvSkinDlg::RelocationControls()
{
	return;
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;


	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnRtvComplete.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvComplete.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvEstop.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvEstop.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvActive.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvActive.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvStop.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvStop.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvResetError.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvResetError.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvDelete.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvDelete.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvCallToHome.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvCallToHome.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

	m_btnRtvSuspend.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnRtvSuspend.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);
}


// [LGLS 2026-08-13] 그룹박스 안 명령 버튼들을 스킨 비트맵 크기로 세로 정렬.
//   rc 크기가 비트맵(110x27)보다 작으면 가장자리가 잘려 보이고 버튼마다 크기가 달라지므로
//   전부 비트맵 크기로 맞춘다. 그룹 높이가 모자라면 그룹을 아래로 늘린다.
static void StackCommandButtons(CWnd* pDlg, UINT nGrpId, const UINT* pIds, int nCnt, SIZE szL, int nGap, BOOL bBottom)
{
	CWnd* pGrp = pDlg->GetDlgItem(nGrpId);
	if (pGrp == NULL || !::IsWindow(pGrp->m_hWnd)) return;
	CRect rcGrp; pGrp->GetWindowRect(&rcGrp); pDlg->ScreenToClient(&rcGrp);
	int nPitch = szL.cy + nGap;
	int nNeed  = 13 + nCnt * nPitch - nGap + 5;
	if (!bBottom && rcGrp.Height() < nNeed)
	{
		rcGrp.bottom = rcGrp.top + nNeed;
		pGrp->MoveWindow(&rcGrp);
	}
	int x = rcGrp.left + (rcGrp.Width() - szL.cx) / 2;
	int y = bBottom ? (rcGrp.bottom - 5 - (nCnt * nPitch - nGap)) : (rcGrp.top + 13);
	for (int i = 0; i < nCnt; i++)
	{
		CWnd* pBtn = pDlg->GetDlgItem(pIds[i]);
		if (pBtn == NULL || !::IsWindow(pBtn->m_hWnd)) continue;
		pBtn->MoveWindow(x, y, szL.cx, szL.cy);
		y += nPitch;
	}
}

void CRtvSkinDlg::RedrawImage()
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


	m_btnRtvComplete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvComplete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("forcecompletion"), strExtension)), NULL, 5, 5);

	m_btnRtvEstop.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0 ,0);
	m_btnRtvEstop.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("emergencystop"), strExtension)), NULL, 5, 5);

	m_btnRtvActive.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvActive.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("active"), strExtension)), NULL, 5, 5);

	m_btnRtvStop.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvStop.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("stop"), strExtension)), NULL, 5, 5);

	m_btnRtvResetError.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvResetError.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("errorreset"), strExtension)), NULL, 5, 5);

	m_btnRtvDelete.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvDelete.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);

	m_btnRtvCallToHome.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvCallToHome.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("calltohome"), strExtension)), NULL, 5, 5);

	m_btnRtvManual.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvManual.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("manual"), strExtension)), NULL, 5, 5);

	m_btnRtvSuspend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvSuspend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("save"), strExtension)), NULL, 5, 5);

	// [LGLS 2026-08-05] 확대/축소 버튼도 같은 스킨 + 아이콘.
	//   ★비트맵만 주고 아이콘이 없으면 PrepareBitmapRect 가 캡션 영역을 비트맵 폭만큼
	//   오른쪽으로 밀어 글자가 통째로 안 그려진다(빈 버튼 증상의 원인).
	//   아이콘이 있으면 DrawItem 이 캡션 영역을 다시 잡으므로 다른 버튼처럼 나온다.
	// [LGLS 2026-08-12] 원 대화상자 [지시 재전송] 버튼도 같은 스킨 + 아이콘
	m_btnRtvResend.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnRtvResend.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("manual"), strExtension)), NULL, 5, 5);

	m_btnVehZoom.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnVehZoom.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("arrow-down"), strExtension)), NULL, 5, 5);

	// [LGLS 2026-08-13] 명령 버튼 크기 통일(비트맵 110x27) + 세로 재배치
	{
		SIZE szL = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
		UINT nCol1[] = { IDC_BTN_RTV_COMPLETE, IDC_BTN_RTV_ESTOP, IDC_BTN_RTV_ACTIVE, IDC_BTN_RTV_STOP,
		                 IDC_BTN_RTV_RESET_ERROR, IDC_BTN_RTV_CALL_TO_HOME, IDC_BTN_RTV_DELETE,
		                 IDC_LGLS_RTV_RESEND, IDC_LGLS_RTV_ZOOM };
		UINT nCol2[] = { IDC_BTN_RTV_SUSPEND };
		StackCommandButtons(this, IDC_GRP_FK_FK_STATUS_COMMAND,   nCol1, 9, szL, 0, FALSE);
		StackCommandButtons(this, IDC_GRP_RTV_JOB_STATUS_COMMAND, nCol2, 1, szL, 0, TRUE);
	}

}

LRESULT CRtvSkinDlg::OnMessagSwitch(WPARAM wParam, LPARAM lParam)
{
	CRTV_DATA* pRTV_DATA = (CRTV_DATA*)wParam;
	if(pRTV_DATA == NULL)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("RTV를 찾을수 없습니다")));
		return 0;
	}

	m_pRTV_DATA = pRTV_DATA;

	EN_LANG enLangTemp = (EN_LANG)lParam;
	if(m_nLang != enLangTemp)
	{
		m_nLang = enLangTemp;
		InvalidateLangControl(enLangTemp);
	}

	InvalidateRtvData(enLangTemp);

	return 0;
}

void CRtvSkinDlg::SetButtonScImage( CButton& pButton, CString pFieldName, int pValue )
{
	//HICON hIcon = Global.GetIcon(Global.GetCvIconIndex(pFieldName, pValue));
	//pButton.SetIcon(hIcon);
}

void CRtvSkinDlg::InvalidateLangControl( EN_LANG enLangTemp )
{ // 데이터가 아닌 레이블 / 버튼 등의 값만 수정시킬것
	if(m_pDoc == NULL)
		return;

	//언어추가할것
	//m_lblScJobNo.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호"), enLangTemp));
	//m_lblScJobNo2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호2"), enLangTemp));
	//m_lblScJobTyp.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분"), enLangTemp));
	//m_lblScJobTyp2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분2"), enLangTemp));
	//m_lblScStartPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발위치"), enLangTemp));
	//m_lblScStartPos2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발위치2"), enLangTemp));
	//m_lblScDestPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착위치"), enLangTemp));
	//m_lblScDestPos2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착위치2"), enLangTemp));
	//m_lblScRcMode.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("지상반"), enLangTemp));
	//m_lblScScMode.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("기상반"), enLangTemp));
	//m_lblScForkPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("포크위치"), enLangTemp));
	//m_lblScForkPos2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("포크위치2"), enLangTemp));
	//m_lblScForkStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("포크상태"), enLangTemp));
	//m_lblScForkStatus2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("포크상태2"), enLangTemp));
	//m_lblScJobStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("완료상태"), enLangTemp));
	//m_lblScRcStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("SRC상태"), enLangTemp));
	//m_lblScScStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("동작상태"), enLangTemp));
	//m_lblScHorizontalPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("수평주행"), enLangTemp));
	//m_lblScVerticalPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("수직주행"), enLangTemp));
	//m_lblScProdLoad.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("화물유무"), enLangTemp));
	//m_lblScJobJobNo.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호"), enLangTemp));
	//m_lblScJobJobNo2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업번호2"), enLangTemp));
	//m_lblScJobJobTyp.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분"), enLangTemp));
	//m_lblScJobJobTyp2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업구분2"), enLangTemp));
	//m_lblScJobStartPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발위치"), enLangTemp));
	//m_lblScJobStartPos2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("출발위치2"), enLangTemp));
	//m_lblScJobDestPos.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착위치"), enLangTemp));
	//m_lblScJobDestPos2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("도착위치2"), enLangTemp));
	//m_lbScJobJobStatus.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업상태"), enLangTemp));
	//m_lbScJobJobStatus2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("작업상태2"), enLangTemp));
	//m_lblScJobBcrTop.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("BCR상단"), enLangTemp));
	//m_lblScJobBcrTop2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("BCR상단2"), enLangTemp));
	//m_lblScJobBcrBottom.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("BCR하단"), enLangTemp));
	//m_lblScJobBcrBottom2.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("BCR하단2"), enLangTemp));
	//m_lblScSuspend.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("SC상태"), enLangTemp));
}


// [LGLS 2026-08-05] 런타임에 만든 라벨은 RenameResource 가 돌 때 아직 존재하지 않는다.
//   생성 직후 같은 ini 로 다시 이름을 붙여 다국어가 적용되게 한다.

// [LGLS 2026-08-05] 그룹박스 캡션 뒤에 회색 네모가 남는 문제.
//   이 그룹박스들은 비주얼 스타일(테마)이 직접 그리기 때문에 WM_CTLCOLOR 로는 못 막는다.
//   테마를 떼면 클래식 방식으로 그려지고, 그때는 OnCtlColor 의 투명 처리가 그대로 먹는다.
void CRtvSkinDlg::MakeGroupBoxesTransparent()
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

void CRtvSkinDlg::RenameRuntimeLabels()
{
	TCHAR chrFile[500]; GetModuleFileName(NULL, chrFile, MAX_PATH);
	CString strApp; strApp.Format(_T("%s"), chrFile);
	CString strPath = Global.GetConcatPath(strApp.Left(strApp.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), _T(".ini"));
	EN_LANG enLang = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;
	CString s;
	CDC* pDC = GetDC();
	CFont* pOld = (pDC != NULL) ? pDC->SelectObject(GetFont()) : NULL;
	s = CLib::GetIniStringFromPath(strPath, _T("lotno"),     (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_RTV_LOT_LBL, s);
	s = CLib::GetIniStringFromPath(strPath, _T("productid"), (int)enLang); if (!s.IsEmpty()) SetDlgItemText(IDC_LGLS_RTV_PRD_LBL, s);

	// 번역문이 원래 한글보다 길면 잘리므로 글자 폭에 맞춰 오른쪽 끝을 고정한 채 넓힌다.
	if (pDC != NULL)
	{
		const int nIds[] = { IDC_LGLS_RTV_LOT_LBL, IDC_LGLS_RTV_PRD_LBL };
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

void CRtvSkinDlg::RenameResource( EN_LANG m_enLang)
{
	// [LGLS 2026-08-05] 여기 있던 return; 때문에 RTV 대화상자만 다국어 적용이 통째로 죽어 있었다.
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");


	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("forcecompletion"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_RTV_COMPLETE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("emergencystop"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_RTV_ESTOP, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("active"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_RTV_ACTIVE, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("stop"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_RTV_STOP, strValue);


	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("errorreset"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_RTV_RESET_ERROR, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("delete"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_RTV_DELETE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("resend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LGLS_RTV_RESEND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("calltohome"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_RTV_CALL_TO_HOME, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("manualorder"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_RTV_MANUAL, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_JOB_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_START_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ordermode"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_MODE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("completestatus"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_COMPLETE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("freightyn"), (int)m_enLang); //화물유무
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_PRODLOAD, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rtvstauts"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("active"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_ACTIVE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("horizontallocation"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_HORIZONTAL_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ground"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_JISANG_MODE, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobno"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_JOB_JOB_NO, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("startpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_JOB_START_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_JOB_DEST_POS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobtyp"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LBL_RTV_JOB_JOB_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("suspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_BTN_RTV_SUSPEND, strValue);

	//grp
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rtvstauts"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_RTV_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_RTV_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork1"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_RTV_STATUS_FK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork2"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_RTV_STATUS_FK2, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_FK_FK_STATUS_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_FK_FK_STATUS_ITEM3, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("rtvstauts"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_FK_FK_STATUS_RTV, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("jobsta"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_JOB_STATUS, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("command"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_JOB_STATUS_COMMAND, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("item"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_JOB_STATUS_ITEM, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork1"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_JOB_STATUS_FK1, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("fork2"), (int)m_enLang);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("error"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_STATUS_SENSOR, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("suspend"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_GRP_RTV_SUSPEND, strValue);

	

	// [LGLS 2026-08-05] 런타임/신규 라벨도 다국어 대상에 포함한다.
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("lotno"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LGLS_RTV_LOT_LBL, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("productid"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LGLS_RTV_PRD_LBL, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("curloc"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LGLS_RTV_CUR_LBL, strValue);
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("cmploc"), (int)m_enLang);
	if (!strValue.IsEmpty()) SetDlgItemText(IDC_LGLS_RTV_CMP_LBL, strValue);
}

void CRtvSkinDlg::InitializeResource(EN_LANG nEN_LANG)
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

void CRtvSkinDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1 && m_pDoc != NULL)
		InvalidateRtvData(m_pDoc->m_enLang);
	CSkinDialog::OnTimer(nIDEvent);
}

void CRtvSkinDlg::OnClose()
{
	KillTimer(1);
	m_pDoc->m_pRtvSkinDlg = NULL;
	delete this;
}

//강제완료
void CRtvSkinDlg::OnBnClickedBtnRtvComplete()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvConfirm);
}

//비상정지
void CRtvSkinDlg::OnBnClickedBtnRtvEstop()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvEmergency);
}

//ACTIVE
void CRtvSkinDlg::OnBnClickedBtnRtvActive()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvActive);
	
}

//정지
void CRtvSkinDlg::OnBnClickedBtnRtvStop()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvStop);
}

//에러리셋
void CRtvSkinDlg::OnBnClickedBtnRtvResetError()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvErrReset);
}

//삭제
void CRtvSkinDlg::OnBnClickedBtnRtvDelete()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	// [LGLS] RTV는 싱글 포크 → 포크 선택 없이 바로 삭제
	UpdateRtvData(EN_BtnRtvFk1Delete);
}

//홈복귀
void CRtvSkinDlg::OnBnClickedBtnRtvCallToHome()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	UpdateRtvData(EN_BtnRtvCallToHome);
}

void CRtvSkinDlg::UpdateRtvData(int nBtnJob)
{
	CString strSql, strWhTyp, strRtvNo, strPlcNo, strScGrpNo, strCmdId, strFK, strMsg=_T("");
	UpdateData(TRUE);

	strWhTyp = m_pDoc->m_WH_TYP;
	strRtvNo = m_pRTV_DATA->K_RTV_NO;
	strPlcNo = m_pRTV_DATA->K_PLC_NO;

	//CMD_RQ_ID 공통코드 참조
	if (     nBtnJob == EN_BtnRtvConfirm  )  { strCmdId = _T("FCMP"); strFK = _T("0"); strMsg = _T("강제완료를 진행하시겠습니까?");   } //강제완료
	else if (nBtnJob == EN_BtnRtvEmergency)  { strCmdId = _T("EMERGENCY"); strFK = _T("0"); strMsg = _T("비상정지를 진행하시겠습니까?");   } //비상정지
	else if (nBtnJob == EN_BtnRtvActive)     { strCmdId = _T("ACTIVE"); strFK = _T("0"); strMsg = _T("ACTIVE를 진행하시겠습니까?");	 } //ACTIVE
	else if (nBtnJob == EN_BtnRtvStop)	    { strCmdId = _T("PAUSE"); strFK = _T("0"); strMsg = _T("STOP을 진행하시겠습니까?");		 } //정지
	else if (nBtnJob == EN_BtnRtvErrReset)   { strCmdId = _T("RESET"); strFK = _T("0"); strMsg = _T("ERROR RESET을 진행하시겠습니까?");} //ERROR RESET
	else if (nBtnJob == EN_BtnRtvFk1Delete)     { strCmdId = _T("DELFK1"); strFK = _T("1");   strMsg = _T("삭제를 진행하시겠습니까?");		 } //삭제
	else if (nBtnJob == EN_BtnRtvFk2Delete)     { strCmdId = _T("DELFK2"); strFK = _T("2");   strMsg = _T("Fork2 삭제를 진행하시겠습니까?");		 } //삭제
	else if (nBtnJob == EN_BtnRtvFk1Fk2Delete)     { strCmdId = _T("DELFK12"); strFK = _T("0");   strMsg = _T("Fork1,2 삭제를 진행하시겠습니까?");		 } //삭제
	else if (nBtnJob == EN_BtnRtvCallToHome) { strCmdId = _T("CTH"); strFK = _T("0");  strMsg = _T("홈복귀를 진행하시겠습니까?");	 } //홈복귀

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(strMsg), MB_YESNO) != IDYES)
		return;

	strSql.Format(_T(" UPDATE RTV_DATA_LGLS		\n")
		_T("    SET CMD_RQ_ID = '%s'		\n")
		_T("	  , CMD_RQ_YN = 'Y'		    \n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	AND PLC_NO = '%02s'			\n")
		_T("    AND RTV_NO = '%s'			  "), strCmdId, strWhTyp, strPlcNo, strRtvNo);

	BOOL isSuccess = FALSE;

	m_pDoc->BeginTrans_DLG();
	isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	// [LGLS 2026-09-01] ★삭제는 최소한 내부 작업번호(지시값)를 지운다★ (사용자 지시)
	//   DELFK 명령은 통신 Task 에 소비자가 없어 명령만 남고 지시 정보가 그대로였다.
	//   지시(_OD) 일체를 비워 스케줄러/화면이 이 RTV 를 유휴로 보게 한다.
	if (isSuccess && (nBtnJob == EN_BtnRtvFk1Delete || nBtnJob == EN_BtnRtvFk2Delete || nBtnJob == EN_BtnRtvFk1Fk2Delete))
	{
		CString strSqlClr;
		strSqlClr.Format(_T(" UPDATE RTV_DATA_LGLS \n")
			_T("    SET LUGG_OD      = '0000' \n")
			_T("      , PALLET_ID_OD = '0000' \n")
			_T("      , JOB_TYP_OD   = '0' \n")
			_T("      , FROM_01_OD = '00', FROM_02_OD = '00', FROM_03_OD = '00' \n")
			_T("      , TO_01_OD   = '00', TO_02_OD   = '00', TO_03_OD   = '00' \n")
			_T("      , RTV_DEST_OD = '', RTV_PASSCV_OD = '' \n")
			_T("  WHERE WH_TYP = '%s' \n")
			_T("    AND RTV_NO = '%s'   "), strWhTyp, strRtvNo);
		isSuccess = m_pDoc->ExcuteQueryString_DLG(strSqlClr);
	}

	if(isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO1 = m_pRTV_DATA->V_ITN_LUGG_FK1;
		if (strLOG_LUGG_NO1 == _T("")) { strLOG_LUGG_NO1 = _T("0");}
		CString strLOG_LUGG_NO2 = m_pRTV_DATA->V_ITN_LUGG_FK2;
		if (strLOG_LUGG_NO2 == _T("")) { strLOG_LUGG_NO2 = _T("0");}
		CString strLOG_BOTTOM_TRAY = _T("");
		CString strLOG_TOP_TRAY = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("RTV ACTION BUTTON -> SC NO : %s , ACTION : %s"), strRtvNo, strCmdId);

		if (strFK == _T("1"))
		{
			if (strLOG_LUGG_NO1 != _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
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
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO2, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
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
					if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
					{
						m_pDoc->RollbackTrans_DLG();
						return;
					}
				}
			}
			else if (strLOG_LUGG_NO1 != _T("0") && strLOG_LUGG_NO2 == _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
			else if (strLOG_LUGG_NO1 == _T("0") && strLOG_LUGG_NO2 != _T("0"))
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO2, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
			else
			{
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO1, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
				if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO2, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
				{
					m_pDoc->RollbackTrans_DLG();
					return;
				}
			}
		}
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;
}

void CRtvSkinDlg::OnBnClickedBtnRtvSuspend()
{

	if (!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage;
	CString strMsg;
	CString strWhTyp = m_pDoc->m_WH_TYP;
	CString strPlcNo = m_pRTV_DATA->K_PLC_NO;
	CString strRtvNo = m_pRTV_DATA->K_RTV_NO;

	strSql.Format(_T(" SELECT SUSPEND		\n")
		_T("			 FROM RTV_DATA_LGLS		\n")
		_T("			WHERE WH_TYP = '%s'			\n")
		_T("		      AND PLC_NO = '%02s'			\n")
		_T("			  AND RTV_NO = '%s'			")
		_T("			  AND SUSPEND IN ('1')			"), strWhTyp, strPlcNo, strRtvNo);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	/*
	pRsw->MoveFirst(); 
	*/
	delete pRsw;
	if (nRowCnt == 0)
	{
		strMsg = _T("Suspend");
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("Rtv 사용을 중지하시겠습니까?")), MB_YESNO) != IDYES)
		return;
	}
	else
	{
		strMsg = _T("Release");
		if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("Rtv를 사용하시겠습니까?")), MB_YESNO) != IDYES)
			return;
	}

	strSql.Format(_T(" UPDATE RTV_DATA_LGLS		\n")
		_T("    SET SUSPEND = %s		\n")
		_T("  WHERE WH_TYP = '%s'			\n")
		_T("	AND PLC_NO = '%02s'			\n")
		_T("    AND RTV_NO = '%s'			  "), _T("1 - SUSPEND"),strWhTyp, strPlcNo, strRtvNo);

	BOOL isSuccess = FALSE;
	m_pDoc->BeginTrans_DLG();
	isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		CString strLOG_LUGG_NO =  m_pRTV_DATA->V_ITN_LUGG_FK1;
		if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0");}
		CString strLOG_BOTTOM_TRAY = _T("");
		CString strLOG_TOP_TRAY = _T("");
		CString strLOG_MSG = _T("");
		strLOG_MSG.Format(_T("RTV SUSPEND BOTTON -> RTV_NO : %s , %s"), strRtvNo, strMsg);
		if (!m_pDoc->GetQueryInsertClientLog(_T("CRtvSkinDlg"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG))
		{
			m_pDoc->RollbackTrans_DLG();
			return;
		}
		m_pDoc->CommitTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
		return;
	}
	m_pDoc->RollbackTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
	return;
}



void CRtvSkinDlg::OnBnClickedChkRtvFork1()
{
	UpdateData(TRUE);

	if (m_chkRtvFork1.GetCheck() == 1)
	{
		m_strRtvFork = _T("1");
		//Enable 처리
		m_chkRtvFork1.EnableWindow(TRUE);
		m_chkRtvFork2.EnableWindow(FALSE);
		m_chkRtvFork1Fork2.EnableWindow(FALSE);
	}else{
		m_strRtvFork = _T("0");
		m_chkRtvFork1.EnableWindow(TRUE);
		m_chkRtvFork2.EnableWindow(TRUE);
		m_chkRtvFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}


void CRtvSkinDlg::OnBnClickedChkRtvFork2()
{
	UpdateData(TRUE);

	if (m_chkRtvFork2.GetCheck() == 1)
	{
		m_strRtvFork = _T("2");
		//Enable 처리
		m_chkRtvFork1.EnableWindow(FALSE);
		m_chkRtvFork2.EnableWindow(TRUE);
		m_chkRtvFork1Fork2.EnableWindow(FALSE);
	}else{
		m_strRtvFork = _T("0");
		m_chkRtvFork1.EnableWindow(TRUE);
		m_chkRtvFork2.EnableWindow(TRUE);
		m_chkRtvFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}


void CRtvSkinDlg::OnBnClickedChkRtvFork12()
{
	UpdateData(TRUE);

	if (m_chkRtvFork1Fork2.GetCheck() == 1)
	{
		m_strRtvFork = _T("3");
		//Enable 처리
		m_chkRtvFork1.EnableWindow(FALSE);
		m_chkRtvFork2.EnableWindow(FALSE);
		m_chkRtvFork1Fork2.EnableWindow(TRUE);
	}else{
		m_strRtvFork = _T("0");
		m_chkRtvFork1.EnableWindow(TRUE);
		m_chkRtvFork2.EnableWindow(TRUE);
		m_chkRtvFork1Fork2.EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	return;
}
void CRtvSkinDlg::GetErrorCode(CString strEqpTyp, CString strErrorCode, int nEN_LANG, CString& strGetErrorCode)
{
	CStringList strList;
	CString strSql;
	CString strMSG_KOR, strMSG_CHIN, strMSG_ENG, strMSG_HUN;
	int nRowCnt = 0, j=0;
	CString strMessage;
	strSql.Format(_T(" SELECT MSG_KOR, ") + m_pDoc->NVL + _T("(MSG_CHIN,MSG_KOR) AS MSG_CHIN, ") + m_pDoc->NVL + _T("(MSG_ENG,MSG_KOR) AS MSG_ENG, ") + m_pDoc->NVL + _T("(MSG_HUN,MSG_KOR) AS MSG_HUN   ")
		_T("    FROM EQP_ECD_MST                     ")
		_T("   WHERE EQP_TYP      = '%s'               ")
		_T("    AND EQP_ERR_CD   = '%04s'               "), strEqpTyp, strErrorCode);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
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

CString CRtvSkinDlg::GetQrySelectJOB_MST( CRTV_DATA* pRTV_DATA )
{
	if(pRTV_DATA == NULL)
		return _T("");

	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(JM.LUGG_NO, '0') AS LUGG_NO ");
	strSql += CRLF + _T("	  , ") + _T("'[' + JM.JOB_STATUS + '] ' + ") + m_pDoc->NVL + _T("(CCD.CCD_NM_KOR, '0') AS JOB_STATUS ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.START_POS, '00000') AS START_POS ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.DEST_POS, '00000') AS DEST_POS ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.PRODUCT_ID, '') AS PRODUCT_ID ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.LOT_NO, '') AS LOT_NO ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(CCD_JT.CCD_NM_KOR, '[' + JM.JOB_TYP + ']') AS JOB_TYP ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.START_LOCATION, '00-000-00') AS START_LOCATION ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.DEST_LOCATION, '00-000-00') AS DEST_LOCATION ");
	strSql += CRLF + _T("  FROM JOB_MST JM INNER JOIN RTV_DATA_LGLS RD_FK1 ");
	strSql += CRLF + _T("                         ON JM.WH_TYP = ") + CLib::Quot(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                        AND JM.WH_TYP = RD_FK1.WH_TYP ");
	strSql += CRLF + _T("                        AND JM.LUGG_NO = RD_FK1.LUGG_OD ");	// [LGLS] ITN_LUGG_FK1 -> LUGG_OD
	strSql += CRLF + _T("		     RIGHT OUTER JOIN ( SELECT '1' AS D ) TRASH ");
	strSql += CRLF + _T("                          ON 1=1 ");
	strSql += CRLF + _T("             LEFT OUTER JOIN COMMON_CODE CCD ");
	strSql += CRLF + _T("                          ON CCD.CDX_CD = 'JOB_STATUS' ");
	strSql += CRLF + _T("                         AND CCD.CCD_CD = JM.JOB_STATUS ");
	strSql += CRLF + _T("                         AND CCD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("             LEFT OUTER JOIN COMMON_CODE CCD_JT ");
	strSql += CRLF + _T("                          ON CCD_JT.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JT.CCD_CD = JM.JOB_TYP ");
	strSql += CRLF + _T("                         AND CCD_JT.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("UNION ALL ");
	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(JM.LUGG_NO, '0') AS LUGG_NO ");
	strSql += CRLF + _T("	  , ") + _T("'[' + JM.JOB_STATUS + '] ' + ") + m_pDoc->NVL + _T("(CCD.CCD_NM_KOR, '0') AS JOB_STATUS ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.START_POS, '00000') AS START_POS ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.DEST_POS, '00000') AS DEST_POS ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.PRODUCT_ID, '') AS PRODUCT_ID ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.LOT_NO, '') AS LOT_NO ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(CCD_JT.CCD_NM_KOR, '[' + JM.JOB_TYP + ']') AS JOB_TYP ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.START_LOCATION, '00-000-00') AS START_LOCATION ");
	strSql += CRLF + _T("	  , ") + m_pDoc->NVL + _T("(JM.DEST_LOCATION, '00-000-00') AS DEST_LOCATION ");
	strSql += CRLF + _T("  FROM JOB_MST JM INNER JOIN RTV_DATA_LGLS RD_FK2 ");
	strSql += CRLF + _T("                         ON JM.WH_TYP = ") + CLib::Quot(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                        AND JM.WH_TYP = RD_FK2.WH_TYP ");
	strSql += CRLF + _T("                        AND JM.LUGG_NO = NULL ");	// [LGLS] ITN_LUGG_FK2 has no equivalent: never match (fork2 unused)
	strSql += CRLF + _T("		     RIGHT OUTER JOIN ( SELECT '1' AS D ) TRASH ");
	strSql += CRLF + _T("                          ON 1=1 ");
	strSql += CRLF + _T("             LEFT OUTER JOIN COMMON_CODE CCD ");
	strSql += CRLF + _T("                          ON CCD.CDX_CD = 'JOB_STATUS' ");
	strSql += CRLF + _T("                         AND CCD.CCD_CD = JM.JOB_STATUS ");
	strSql += CRLF + _T("                         AND CCD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("             LEFT OUTER JOIN COMMON_CODE CCD_JT ");
	strSql += CRLF + _T("                          ON CCD_JT.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JT.CCD_CD = JM.JOB_TYP ");
	strSql += CRLF + _T("                         AND CCD_JT.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}

CString CRtvSkinDlg::GetQrySelectRTV_STATUS_CCD( CRTV_DATA* pRTV_DATA )
{
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	// [LGLS 2026-08-31] "[코드]명칭" 표기 (사용자 요구)
	strSql += CRLF + _T("SELECT ") + m_pDoc->NVL + _T("(CCD_JOB_TYP.CCD_NM_KOR, '[' + RD.JOB_TYP_OD + ']') AS JOB_TYP_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_COMPLETE_RD.CCD_NM_KOR, '0') AS COMPLETE_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_SENSOR_FK_RD.CCD_NM_KOR, '0') AS SENSOR_FK_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_AUTO_MODE_RD.CCD_NM_KOR, '0') AS AUTO_MODE_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_UCSTATUS_RD.CCD_NM_KOR, '0') AS UCSTATUS_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_ACTIVE_MODE_RD.CCD_NM_KOR, '0') AS ACTIVE_MODE_RD ");
	strSql += CRLF + _T("	   ,") + m_pDoc->NVL + _T("(CCD_SUSPEND.CCD_NM_KOR, '0') AS SUSPEND ");
	strSql += CRLF + _T("  FROM RTV_DATA_LGLS RD LEFT OUTER JOIN COMMON_CODE CCD_JOB_TYP");
	strSql += CRLF + _T("                          ON CCD_JOB_TYP.CDX_CD = 'JOB_TYP' ");
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.CCD_CD = RD.JOB_TYP_OD ");	// [LGLS] JOB_TYP_RD -> JOB_TYP_OD
	strSql += CRLF + _T("                         AND CCD_JOB_TYP.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_COMPLETE_RD ");
	strSql += CRLF + _T("                          ON CCD_COMPLETE_RD.CDX_CD = 'RTV_COMPLETE' ");
	strSql += CRLF + _T("                         AND CCD_COMPLETE_RD.CCD_CD = RD.UNLOAD_COMPLETE_RD ");	// [LGLS] COMPLETE_RD -> UNLOAD_COMPLETE_RD
	strSql += CRLF + _T("                         AND CCD_COMPLETE_RD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_SENSOR_FK_RD ");
	strSql += CRLF + _T("                          ON CCD_SENSOR_FK_RD.CDX_CD = 'RTV_SENSOR_FK' ");
	strSql += CRLF + _T("                         AND CCD_SENSOR_FK_RD.CCD_CD = RD.SENSOR_RTV_RD ");	// [LGLS] SENSOR_FK_RD -> SENSOR_RTV_RD
	strSql += CRLF + _T("                         AND CCD_SENSOR_FK_RD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_AUTO_MODE_RD ");
	strSql += CRLF + _T("                          ON CCD_AUTO_MODE_RD.CDX_CD = 'RTV_AUTO_MODE' ");
	strSql += CRLF + _T("                         AND CCD_AUTO_MODE_RD.CCD_CD = RD.AUTO_MODE_RD ");
	strSql += CRLF + _T("                         AND CCD_AUTO_MODE_RD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_ACTIVE_MODE_RD ");
	strSql += CRLF + _T("                          ON CCD_ACTIVE_MODE_RD.CDX_CD = 'RTV_ACTIVE_MODE' ");
	strSql += CRLF + _T("                         AND CCD_ACTIVE_MODE_RD.CCD_CD = (CASE WHEN RD.SUBSYSTEM_STATUS_RD = '2' THEN '1' ELSE '0' END) ");	// [LGLS] ACTIVE_MODE_RD derived from SUBSYSTEM_STATUS_RD
	strSql += CRLF + _T("                         AND CCD_ACTIVE_MODE_RD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_UCSTATUS_RD ");
	strSql += CRLF + _T("                          ON CCD_UCSTATUS_RD.CDX_CD = 'RTV_UCSTATUS' ");
	strSql += CRLF + _T("                         AND CCD_UCSTATUS_RD.CCD_CD = RD.SUBSYSTEM_STATUS_RD ");	// [LGLS] UCSTATUS -> SUBSYSTEM_STATUS_RD
	strSql += CRLF + _T("                         AND CCD_UCSTATUS_RD.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("                  LEFT OUTER JOIN COMMON_CODE CCD_SUSPEND ");
	strSql += CRLF + _T("                          ON CCD_SUSPEND.CDX_CD = 'RTV_SUSPEND' ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.CCD_CD = RD.SUSPEND ");
	strSql += CRLF + _T("                         AND CCD_SUSPEND.WH_TYP LIKE ") + CLib::QuotLikeLR(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T(" WHERE RD.WH_TYP = ") + CLib::Quot(pRTV_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND RD.PLC_NO = ") + CLib::Quot(pRTV_DATA->K_PLC_NO);
	strSql += CRLF + _T("   AND RD.RTV_NO = ") + CLib::Quot(pRTV_DATA->K_RTV_NO);

	return CLib::GetCommonCodeLang(strSql, (int)m_pDoc->m_enLang);
}


HBRUSH CRtvSkinDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);
	int nId = pWnd->GetDlgCtrlID();
	if(nId >= IDC_RTVV_LED_LOAD_CMP && nId <= IDC_RTVV_LED_ALARM_RESET_ACK)
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
	else if(nId == IDC_RTVV_STATUS)
	{
		pDC->SetBkColor(RGB(255,255,150));
		if(m_brStatus) return m_brStatus;
	}
	else if (nId == IDC_LGLS_ADDR_LBL)
	{
		// [LGLS 2026-08-06] 실주소 라벨은 파란 글씨(값이 안 변하므로 투명 배경 안전)
		pDC->SetTextColor(RGB(0,0,200));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	else if(nId == IDC_RTVV_TITLE1)
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


// [LGLS 2026-08-01] RTVV 통신상태 패널을 런타임에 생성한다.
//   배경: InvalidateXxData 가 IDC_RTVV_* 컨트롤에 값을 쓰는데, Ecs.rc 의 대화상자 템플릿에는
//   그 컨트롤들이 하나도 없었다(리소스 편집 미완). SetDlgItemText/GetDlgItem 은 없는 ID 에 대해
//   조용히 실패하므로 "대화상자에 값이 하나도 안 나오는" 증상이 됐다.
//   → rc 를 손대는 대신 동일한 ID 로 컨트롤을 직접 만들어 기존 표시 코드가 그대로 동작하게 한다.

// [LGLS 2026-08-01] RTV 대화상자 여백 정리(사용자 지시 사진).
//   ① JOB_STATUS 의 COMMAND 열 : "-" 표시란과 [RTV SUSPEND] 버튼이 아래로 떨어져 있어 위로 올린다.
//   ② ERROR INFORMATION 그룹 : JOB_STATUS 그룹과의 빈 공간만큼 위로 당기고 창 높이도 줄인다.
//   rc 를 고치지 않고 실제 컨트롤 위치로 계산해 이동한다.
void CRtvSkinDlg::CompactJobStatusArea()
{
	// ── ① COMMAND 열 정리 ─────────────────────────────────────────
	CWnd* pCmdGrp = GetDlgItem(IDC_GRP_RTV_JOB_STATUS_COMMAND);
	CWnd* pSusEdt = GetDlgItem(IDC_EDT_RTV_SUSPEND);
	CWnd* pSusBtn = GetDlgItem(IDC_BTN_RTV_SUSPEND);
	if (pCmdGrp != NULL && pSusEdt != NULL && pSusBtn != NULL)
	{
		CRect rcGrp, rcEdt, rcBtn;
		pCmdGrp->GetWindowRect(&rcGrp); ScreenToClient(&rcGrp);
		pSusEdt->GetWindowRect(&rcEdt); ScreenToClient(&rcEdt);
		pSusBtn->GetWindowRect(&rcBtn); ScreenToClient(&rcBtn);

		int nTop = rcGrp.top + 24;						// COMMAND 라벨 바로 아래
		rcEdt.OffsetRect(0, nTop - rcEdt.top);
		pSusEdt->MoveWindow(rcEdt);

		rcBtn.OffsetRect(0, (rcEdt.bottom + 8) - rcBtn.top);
		pSusBtn->MoveWindow(rcBtn);
	}

	// ── ② ERROR INFORMATION 그룹을 위로 당기고 창 축소 ────────────
	CWnd* pJobGrp = GetDlgItem(IDC_GRP_RTV_JOB_STATUS);
	CWnd* pErrGrp = GetDlgItem(IDC_GRP_RTV_STATUS_SENSOR);
	if (pJobGrp != NULL && pErrGrp != NULL)
	{
		CRect rcJob, rcErr;
		pJobGrp->GetWindowRect(&rcJob); ScreenToClient(&rcJob);
		pErrGrp->GetWindowRect(&rcErr); ScreenToClient(&rcErr);

		int nShift = rcErr.top - (rcJob.bottom + 10);	// 10px 여백만 남긴다
		if (nShift > 0)
		{
			int nThreshold = rcErr.top - 2;
			for (CWnd* pChild = GetWindow(GW_CHILD); pChild != NULL; pChild = pChild->GetWindow(GW_HWNDNEXT))
			{
				CRect rc; pChild->GetWindowRect(&rc); ScreenToClient(&rc);
				if (rc.top >= nThreshold)
				{
					rc.OffsetRect(0, -nShift);
					pChild->MoveWindow(rc);
				}
			}
			CRect rcWin; GetWindowRect(&rcWin);
			SetWindowPos(NULL, 0, 0, rcWin.Width(), rcWin.Height() - nShift, SWP_NOMOVE | SWP_NOZORDER);
		}
	}
}

void CRtvSkinDlg::BuildVehStatusPanel()
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
		for (i = 0; i < m_arLglsCtrl.GetCount(); i++) { CWnd* p = (CWnd*)m_arLglsCtrl.GetAt(i); if (p != NULL) delete p; }
		m_arVehCtrl.RemoveAll();
		m_arLglsCtrl.RemoveAll();
	}

	// [LGLS 2026-08-01] 도착지 아래에 [적재 용기](JOB_MST.LOT_NO) / [제품 정보](JOB_MST.PRODUCT_ID) 두 행 추가
	{
		const int nGrps[] = { IDC_GRP_RTV_JOB_STATUS, IDC_GRP_RTV_JOB_STATUS_ITEM,
		                      IDC_GRP_RTV_JOB_STATUS_FK1, IDC_GRP_RTV_JOB_STATUS_COMMAND };
		// ★ID 이름과 실제 라벨이 반대다 : IDC_..._DEST_POS='도착위치', IDC_..._DEST_LOC='도착지'(마지막 행)
		CLib::AddTwoRowsBelow(this, IDC_LBL_RTV_JOB_DEST_POS, IDC_LBL_RTV_JOB_DEST_LOC, IDC_EDT_RTV_JOB_DEST_LOC,
		                      _T("적재용기"), IDC_LGLS_RTV_LOT_LBL, IDC_LGLS_RTV_LOT_VAL,
		                      _T("제품정보"), IDC_LGLS_RTV_PRD_LBL, IDC_LGLS_RTV_PRD_VAL,
		                      nGrps, sizeof(nGrps)/sizeof(nGrps[0]), m_arLglsCtrl);
	}		// 1회만 생성

	CompactJobStatusArea();		// [LGLS 2026-08-01] 빈 공간 정리 후 패널을 붙인다

	CRect rcCli; GetClientRect(&rcCli);
	CRect rcWin; GetWindowRect(&rcWin);

	const int PH    = 222;						// [LGLS 2026-08-06] 화면 안에 들어가게 압축
	const int STRIP = 30;						// [확대]/[축소] 버튼 띠 높이(px)
	int nTop = rcCli.Height();					// 기존 컨트롤 아래(빈 영역)에서 시작
	m_nVehBaseH  = rcWin.Height();				// 축소 상태 창 높이(원래 DLG 그대로)
	m_nVehPanelH = PH;

	CFont* pFont = GetFont();
	struct L { static void F(CWnd* p, CFont* f) { if (f) p->SetFont(f); } };

	// 라벨 생성기
	struct MK {
		CRtvSkinDlg* dlg; CFont* font;
		void Label(LPCTSTR s, int x, int y, int w, int h) {
			CStatic* p = new CStatic();
			p->Create(s, WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(x, y, x + w, y + h), dlg);
			if (font) p->SetFont(font);
			dlg->m_arVehCtrl.Add(p);
		}
		void LabelA(LPCTSTR nm, const CString& addr, int x, int y, int wN, int wA) {
			// [LGLS 2026-08-06] 이름(검정) + 실주소(파랑, IDC_LGLS_ADDR_LBL) 분리 라벨
			Label(nm, x, y, wN, 16);
			if (addr.IsEmpty()) return;
			CStatic* p = new CStatic();
			p->Create(addr, WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(x + wN + 2, y, x + wN + 2 + wA, y + 16), dlg, IDC_LGLS_ADDR_LBL);
			if (font) p->SetFont(font);
			dlg->m_arVehCtrl.Add(p);
		}
		void Value(int id, int x, int y, int w, int h, DWORD extra = 0) {
			CStatic* p = new CStatic();
			p->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_LEFT | WS_BORDER | extra,
			          CRect(x, y, x + w, y + h), dlg, id);
			if (font) p->SetFont(font);
			dlg->m_arVehCtrl.Add(p);
		}
		void Led(int id, int x, int y) {
			CStatic* p = new CStatic();
			p->Create(_T("0"), WS_CHILD | WS_VISIBLE | SS_CENTER | WS_BORDER,
			          CRect(x, y, x + 14, y + 14), dlg, id);
			if (font) p->SetFont(font);
			dlg->m_arVehCtrl.Add(p);
		}
		void Button(int id, LPCTSTR s, int x, int y, int w, int h) {
			CButton* p = new CButton();
			p->Create(s, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
			          CRect(x, y, x + w, y + h), dlg, id);
			if (font) p->SetFont(font);
			dlg->m_arVehCtrl.Add(p);
		}
	} mk = { this, pFont };

	int y = nTop + 4;
	// [LGLS 2026-08-06] 창 폭이 대화상자마다 달라(SC 좁음) 고정 좌표는 잘린다 - 동적 계산
	int nColW = (rcCli.Width() - 12) / 2;
	int nCol1 = 6 + nColW;
	int nBtnX = rcCli.Width() - 118;

	// [LGLS 2026-08-05] 관측표(observables.tsv)의 실주소를 라벨에 병기한다.
	CString strOwner = _T("VEHICLE:1");	// RTV(RGV) 는 관측표에서 VEHICLE:1
	// ── 제목 / 상태 / 버튼(우측 2줄) ─────────────────────────────
	mk.Value(IDC_RTVV_TITLE1, 6,   y, 110, 18);
	mk.Value(IDC_RTVV_TITLE2, 120, y, 130, 18);
	mk.LabelA(_T("상태"), CLib::GetObsAddr(strOwner, _T("SUBSYSTEM_STATUS")), 256, y + 2, 30, 52);
	mk.Value(IDC_RTVV_STATUS, 344, y, 90, 18);	// [LGLS 2026-09-01] "1 = IDLE" 폭
	y += 20;

	// ── 핸드셰이크 LED : 2열 x 6행 (라벨에 실주소) ────────────────
	struct LEDDEF { LPCTSTR name; int id; LPCTSTR obs; };
	LEDDEF leds[] = {
		{ _T("적재완료"),    IDC_RTVV_LED_LOAD_CMP,        _T("LOAD_COMPLETE") },
		{ _T("적재ACK"),     IDC_RTVV_LED_LOAD_CMP_ACK,    _T("LOAD_COMPLETE_ACK") },
		{ _T("하역완료"),    IDC_RTVV_LED_UNLOAD_CMP,      _T("UNLOAD_COMPLETE") },
		{ _T("하역ACK"),     IDC_RTVV_LED_UNLOAD_CMP_ACK,  _T("UNLOAD_COMPLETE_ACK") },
		{ _T("반송요청"),    IDC_RTVV_LED_TR_REQ,          _T("TRANSFER_REQUEST") },
		{ _T("반송ACK"),     IDC_RTVV_LED_TR_REQ_ACK,      _T("TRANSFER_ACK") },
		{ _T("화물감지"),    IDC_RTVV_LED_PALLET_EXIST,    _T("PALLET_EXIST_FLAG") },
		{ _T("알람SET"),     IDC_RTVV_LED_ALARM_SET,       _T("ALARM_SET_REPORT") },
		{ _T("알람SET ACK"), IDC_RTVV_LED_ALARM_SET_ACK,   _T("ALARM_SET_REPORT_ACK") },
		{ _T("알람RST"),     IDC_RTVV_LED_ALARM_RESET,     _T("ALARM_RESET_REPORT") },
		{ _T("알람RST ACK"), IDC_RTVV_LED_ALARM_RESET_ACK, _T("ALARM_RESET_REPORT_ACK") },
	};
	for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++)
	{
		int col = i / 6, row = i % 6;
		int x = (col == 0) ? 6 : nCol1;
		int yy = y + row * 18;
		mk.LabelA(leds[i].name, CLib::GetObsAddr(strOwner, leds[i].obs), x, yy + 1, 88, 86);
		mk.Led(leds[i].id, x + 180, yy);
	}
	y += 6 * 18 + 2;

	// ── 위치 / 지시 값 (라벨에 시작 주소) ─────────────────────────
	struct TRIO { LPCTSTR name; int a, b, c; LPCTSTR obs; };
	TRIO trios[] = {
		{ _T("현재위치"), IDC_RTVV_CUR1, IDC_RTVV_CUR2, IDC_RTVV_CUR3, _T("SUBSYSTEM_LOCATION_01") },
		{ _T("출발지시"), IDC_RTVV_FROM1, IDC_RTVV_FROM2, IDC_RTVV_FROM3, _T("FROM_01") },
		{ _T("도착지시"), IDC_RTVV_TO1, IDC_RTVV_TO2, IDC_RTVV_TO3, _T("TO_01") },
		{ _T("완료위치"), IDC_RTVV_CMP1, IDC_RTVV_CMP2, IDC_RTVV_CMP3, _T("TRANSFER_COMPLETE_LOCATION_01") },
	};
	for (int i = 0; i < sizeof(trios)/sizeof(trios[0]); i++)
	{
		int col = i % 2, row = i / 2;
		int x = (col == 0) ? 6 : nCol1;
		int yy = y + row * 18;
		mk.LabelA(trios[i].name, CLib::GetObsAddr(strOwner, trios[i].obs), x, yy + 2, 58, 56);
		mk.Value(trios[i].a, x + 120, yy, 30, 18);
		mk.Value(trios[i].b, x + 156, yy, 30, 18);
		mk.Value(trios[i].c, x + 192, yy, 30, 18);
	}
	y += 2 * 18 + 2;

	// ── 파렛트 / 알람코드 ──────────────────────────────────────────
	mk.LabelA(_T("파렛트ID"), CLib::GetObsAddr(strOwner, _T("PALLET_ID")), 6, y + 2, 58, 56);
	mk.Value(IDC_RTVV_PALLET_ID, 126, y, 110, 18);
	mk.LabelA(_T("알람코드"), CLib::GetObsAddr(strOwner, _T("ALARM_SET_CODE")), nCol1, y + 2, 58, 56);
	mk.Value(IDC_RTVV_ALARM_CODE, nCol1 + 120, y, 110, 18);
	y += 20;

	// ── 버튼(맨 아래) ──────────────────────────────────────────────
	mk.Button(IDC_RTVV_RESEND, _T("지시 재전송"), 6, y, 112, 22);
	mk.Button(IDC_RTVV_OK,  _T("닫기"), 124, y, 80, 22);
	y += 26;

	// [LGLS 2026-08-05] [확대] 버튼은 rc(IDC_LGLS_RTV_ZOOM)로 옮겼다.
	//   런타임 생성 버튼은 CSkinButton 이 붙어도 다른 명령 버튼과 모양/글자가 달라서,
	//   rc 에 두고 DDX 로 묶는 편이 확실하다.

	RenameRuntimeLabels();	// [LGLS 2026-08-05]
	MakeGroupBoxesTransparent();	// [LGLS 2026-08-05]
	SetVehPanelExpanded(FALSE);		// 기본은 축소(원래 DLG 만)
}

// [LGLS 2026-08-01] 상태 패널 표시/숨김 + 창 크기 전환.
//   확대 : 원래 DLG + 통신상태 패널(이전 창 내용) / 축소 : 원래 DLG 만
void CRtvSkinDlg::SetVehPanelExpanded(BOOL bExpand)
{
	m_bVehExpanded = bExpand;
	for (int i = 0; i < m_arVehCtrl.GetCount(); i++)
	{
		CWnd* p = (CWnd*)m_arVehCtrl.GetAt(i);
		if (p != NULL && ::IsWindow(p->m_hWnd)) p->ShowWindow(bExpand ? SW_SHOW : SW_HIDE);
	}
	CRect rc; GetWindowRect(&rc);
	SetWindowPos(NULL, 0, 0, rc.Width(), m_nVehBaseH + (bExpand ? m_nVehPanelH : 0),
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
			CRect rcNow; GetWindowRect(&rcNow);
			int nOver = rcNow.bottom - mi.rcWork.bottom;
			if (nOver > 0)
			{
				int nTopNew = rcNow.top - nOver;
				if (nTopNew < mi.rcWork.top) nTopNew = mi.rcWork.top;
				SetWindowPos(NULL, rcNow.left, nTopNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			}
		}
	}
	if (::IsWindow(m_btnVehZoom.m_hWnd))
	{
		// [LGLS 2026-08-05] 버튼 문구도 다국어 ini 에서 읽는다(없으면 기본 한글).
		TCHAR chrFile[500]; GetModuleFileName(NULL, chrFile, MAX_PATH);
		CString strApp; strApp.Format(_T("%s"), chrFile);
		CString strPath = Global.GetConcatPath(strApp.Left(strApp.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\"), _T("dlg_rtv"), _T(".ini"));
		EN_LANG enLang = (m_pDoc == NULL) ? EN_KOR : m_pDoc->m_enLang;
		CString strTxt = CLib::GetIniStringFromPath(strPath, bExpand ? _T("zoomout") : _T("zoomin"), (int)enLang);
		if (strTxt.IsEmpty()) strTxt = bExpand ? _T("축소") : _T("확대");
		m_btnVehZoom.SetWindowText(strTxt);
		CString strIco = strApp.Left(strApp.ReverseFind('\\')) + _T("\\rc_resource\\dlg_rtv\\20x20\\")
		               + (bExpand ? _T("arrow-up.png") : _T("arrow-down.png"));
		m_btnVehZoom.SetIcon(Global.HICONFromPATH(strIco), NULL, 5, 5);
	}
}

void CRtvSkinDlg::OnBnClickedVehZoom()
{
	SetVehPanelExpanded(!m_bVehExpanded);
}

void CRtvSkinDlg::SetLed(int nId, CString strVal)
{
	CString t = (strVal == _T("1")) ? _T("1") : _T("0");
	SetDlgItemText(nId, t);
	CWnd* p = GetDlgItem(nId);
	if(p) p->Invalidate();
}

void CRtvSkinDlg::OnBnClickedRtvvResend()
{
	if(m_pRTV_DATA == NULL) return;
	if(!m_pDoc->Permission(_T("CRtvSkinDlg"), EXE_YN))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return;
	}

	// [LGLS 2026-08-13] 구 ECS(RGVForm buttonRetry)와 동일: 진행 중 지시(LUGG_OD)가 없으면(IDLE)
	//   재전송할 명령이 없으므로 안내만 하고 중단한다.
	{
		CString strChk = _T("SELECT ISNULL(LUGG_OD,'') AS ITN FROM RTV_DATA_LGLS WHERE WH_TYP = ")
		               + CLib::Quot(m_pDoc->m_WH_TYP)
		               + _T(" AND RTV_NO = ") + CLib::Quot(m_pRTV_DATA->K_RTV_NO);
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
	strSql.Format(_T(" UPDATE RTV_DATA_LGLS SET CMD_RQ_YN='Y' WHERE WH_TYP='%s' AND PLC_NO='%02s' AND RTV_NO='%s' "),
		m_pDoc->m_WH_TYP, m_pRTV_DATA->K_PLC_NO, m_pRTV_DATA->K_RTV_NO);
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

void CRtvSkinDlg::OnBnClickedRtvvOk()
{
	m_pDoc->m_pRtvSkinDlg = NULL;
	delete this;
}
