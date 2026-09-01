// PanelInfoDlg.cpp : [LGLS 2026-09-01] 상세정보 도킹 판넬 (CV/SC/RTV/작업 탭)
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

// SC/RTV 상태·지시 워드 주소 계산 (PlcAddressMap.xml 확정값과 동일)
//   SC  : 상태 D0160+10(n-1) / 구ECS W0100+0x10(n-1),  지시 D0320+10(n-1) / W0300+0x10(n-1),  트래킹 R0300+2(n-1)
//   RTV : 상태 D0210 / W0150,  지시 D0370 / W0350 (To3 특례 D0345/W0345),  트래킹 R0310
static void VehBases(BOOL bRtv, int nUnit, int& nStDoc, int& nStEz, int& nCmdDoc, int& nCmdEz, int& nTrk)
{
	if (bRtv) { nStDoc = 210; nStEz = 0x150; nCmdDoc = 370; nCmdEz = 0x350; nTrk = 310; }
	else      { int k = nUnit - 1;
	            nStDoc = 160 + 10*k; nStEz = 0x100 + 0x10*k;
	            nCmdDoc = 320 + 10*k; nCmdEz = 0x300 + 0x10*k; nTrk = 300 + 2*k; }
}

CPanelInfoDlg::CPanelInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelInfoDlg::IDD, pParent)
{
	m_pDoc = NULL;
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
		{ _T("항목"),       95 }, { _T("값"),        120 }, { _T("설정"),      95 },
		{ _T("확인"),       70 }, { _T("구ECS주소"),  75 }, { _T("실제주소"),  75 },
		{ _T("기록시명칭"), 190 },
	};
	for (int i = 0; i < (int)(sizeof(COLS)/sizeof(COLS[0])); i++)
		m_list.InsertColumn(i, COLS[i].strHead, LVCFMT_LEFT, COLS[i].nWidth);

	// 오버레이 컨트롤(설정/확인 열 위) - 다이얼로그 자식으로 만들고 셀 위치로 이동시킨다
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

// 리스트의 (행, 열) 셀 위로 컨트롤 이동. 콤보는 드롭다운 높이를 더 준다.
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

void CPanelInfoDlg::Refresh()
{
	if (m_pDoc == NULL || !::IsWindow(m_list.m_hWnd))
		return;

	int nTab = m_tab.GetCurSel();
	HideOverlays();
	m_list.SetRedraw(FALSE);
	m_list.DeleteAllItems();

	CString strUnit;
	if (m_cmbUnit.GetCurSel() >= 0)
		m_cmbUnit.GetLBText(m_cmbUnit.GetCurSel(), strUnit);

	if (nTab == TAB_JOB)
	{
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
			int nCnt = -1; CString strMsg;
			_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
			if (nCnt > 0)
			{
				CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
				pRsw->MoveFirst();
				static struct { LPCTSTR strName; LPCTSTR strCap; } JOBF[] = {
					{ _T("LUGG_NO"),        _T("작업번호") },   { _T("JOB_TYP"),      _T("작업구분") },
					{ _T("JOB_STATUS"),     _T("작업상태") },   { _T("START_POS"),    _T("출발") },
					{ _T("START_LOCATION"), _T("출발위치") },   { _T("DEST_POS"),     _T("도착") },
					{ _T("DEST_LOCATION"),  _T("도착위치") },   { _T("LOT_NO"),       _T("LOT") },
					{ _T("PRODUCT_ID"),     _T("제품") },       { _T("JOB_PRIORITY"), _T("우선순위") },
					{ _T("INS_DT"),         _T("등록시각") },   { _T("UPD_DT"),       _T("수정시각") },
				};
				for (int i = 0; i < (int)(sizeof(JOBF)/sizeof(JOBF[0])); i++)
				{
					m_list.InsertItem(i, JOBF[i].strCap);
					m_list.SetItemText(i, 1, pRsw->GetItem(JOBF[i].strName));
					m_list.SetItemText(i, 6, JOBF[i].strName);
				}
				delete pRsw;
			}
		}
	}
	else if (nTab == TAB_CV)
	{
		if (!strUnit.IsEmpty())
		{
			CString strSql;
			strSql.Format(
				_T(" SELECT LUGG_NO_RD, LUGG_NO_OD, DEST_POS_RD, DEST_POS_OD, JOB_TYP_RD, JOB_TYP_OD ")
				_T("       ,ERROR_CODE, AUTO_MODE_RD, STO_READY_RD, RET_READY_RD, STOHS_READY_RD, RETHS_READY_RD ")
				_T("   FROM CV_DATA WHERE WH_TYP = '%s' AND MC_NO = '%s' "),
				(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strUnit);
			int nCnt = -1; CString strMsg;
			_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
			if (nCnt > 0)
			{
				CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
				pRsw->MoveFirst();
				static struct { LPCTSTR strName; LPCTSTR strCap; } CVF[] = {
					{ _T("LUGG_NO_RD"),     _T("작업번호") },     { _T("LUGG_NO_OD"),   _T("지시 작업번호") },
					{ _T("DEST_POS_RD"),    _T("목적지") },       { _T("DEST_POS_OD"),  _T("지시 목적지") },
					{ _T("JOB_TYP_RD"),     _T("구분") },         { _T("JOB_TYP_OD"),   _T("지시 구분") },
					{ _T("ERROR_CODE"),     _T("에러코드") },     { _T("AUTO_MODE_RD"), _T("자동모드") },
					{ _T("STO_READY_RD"),   _T("입고 준비") },    { _T("RET_READY_RD"), _T("출고 준비") },
					{ _T("STOHS_READY_RD"), _T("입고 HS") },      { _T("RETHS_READY_RD"), _T("출고 HS") },
				};
				for (int i = 0; i < (int)(sizeof(CVF)/sizeof(CVF[0])); i++)
				{
					m_list.InsertItem(i, CVF[i].strCap);
					m_list.SetItemText(i, 1, pRsw->GetItem(CVF[i].strName));
					m_list.SetItemText(i, 6, CVF[i].strName);
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
			CString strSql;
			strSql.Format(
				_T(" SELECT SUBSYSTEM_STATUS_RD, ALARM_SET_CODE_RD, ALARM_RESET_CODE_RD ")
				_T("       ,TRANSFER_COMPLETE_LOCATION_01_RD + '/' + TRANSFER_COMPLETE_LOCATION_02_RD + '/' + TRANSFER_COMPLETE_LOCATION_03_RD AS TC_LOC ")
				_T("       ,LOCATION_01_RD + '/' + LOCATION_02_RD + '/' + LOCATION_03_RD AS CUR_LOC ")
				_T("       ,ERR_CODE_RD, PALLET_ON_VEHICLE_RD ")
				_T("       ,FROM_01_OD + '/' + FROM_02_OD + '/' + FROM_03_OD AS CMD_FROM ")
				_T("       ,TO_01_OD + '/' + TO_02_OD + '/' + TO_03_OD AS CMD_TO ")
				_T("       ,PALLET_ID_OD ")
				_T("   FROM %s WHERE WH_TYP = '%s' AND %s = '%s' "),
				(LPCTSTR)strTable, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strKey, (LPCTSTR)strUnit);
			int nCnt = -1; CString strMsg;
			_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nCnt, strMsg);
			if (nCnt > 0)
			{
				int nUnit = _ttoi(strUnit) - 900;   // SC 901~905 -> 1~5
				int nStDoc, nStEz, nCmdDoc, nCmdEz, nTrk;
				VehBases(bRtv, nUnit, nStDoc, nStEz, nCmdDoc, nCmdEz, nTrk);

				CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp);
				pRsw->MoveFirst();
				//                      필드              항목            워드오프셋(-1=주소없음, -2=R영역)
				static struct { LPCTSTR strName; LPCTSTR strCap; int nOff; } VF[] = {
					{ _T("SUBSYSTEM_STATUS_RD"),  _T("상태"),          0 },
					{ _T("ALARM_SET_CODE_RD"),    _T("알람발생코드"),  1 },
					{ _T("ALARM_RESET_CODE_RD"),  _T("알람해제코드"),  2 },
					{ _T("TC_LOC"),               _T("반송완료위치"),  3 },
					{ _T("CUR_LOC"),              _T("현재위치"),      6 },
					{ _T("ERR_CODE_RD"),          _T("에러코드"),     10 },
					{ _T("PALLET_ON_VEHICLE_RD"), _T("차상화물"),     -2 },
					{ _T("CMD_FROM"),             _T("지시 From"),   100 },
					{ _T("CMD_TO"),               _T("지시 To"),     103 },
					{ _T("PALLET_ID_OD"),         _T("지시 작업번호"),106 },
				};
				for (int i = 0; i < (int)(sizeof(VF)/sizeof(VF[0])); i++)
				{
					m_list.InsertItem(i, VF[i].strCap);
					CString strVal = pRsw->GetItem(VF[i].strName);
					if (VF[i].strName == CString(_T("SUBSYSTEM_STATUS_RD")))
					{
						strVal.Trim();
						if (strVal == _T("0")) strVal = _T("0 (DOWN)");
						else if (strVal == _T("1")) strVal = _T("1 (IDLE)");
						else if (strVal == _T("2")) strVal = _T("2 (RUN)");
					}
					m_list.SetItemText(i, 1, strVal);

					CString strEz, strReal;
					int nOff = VF[i].nOff;
					if (nOff == -2)                       // R 트래킹 (10진 확정 - 구 표기도 10진)
					{
						strEz.Format(_T("R%04d"), nTrk);
						strReal.Format(_T("R%04d"), nTrk);
					}
					else if (nOff >= 100)                 // 지시(Command) 블록
					{
						int k = nOff - 100;
						if (bRtv && k == 3)               // RTV To 특례 : To3=D0345
						{
							strEz = _T("W0350+,W0345");
							strReal = _T("D0373~4,D0345");
						}
						else
						{
							strEz.Format(_T("W%04X"), nCmdEz + k);
							strReal.Format(_T("D%04d"), nCmdDoc + k);
						}
					}
					else if (nOff >= 0)                   // 상태(Status) 블록
					{
						if (bRtv && nOff == 10)           // RTV 는 에러코드 워드가 없다(알람코드 유래)
						{ strEz = _T("-"); strReal = _T("-"); }
						else
						{
							strEz.Format(_T("W%04X"), nStEz + nOff);
							strReal.Format(_T("D%04d"), nStDoc + nOff);
						}
					}
					m_list.SetItemText(i, 4, strEz);
					m_list.SetItemText(i, 5, strReal);
					m_list.SetItemText(i, 6, VF[i].strName);
				}
				delete pRsw;
			}
		}
	}

	m_list.SetRedraw(TRUE);
	m_list.Invalidate(FALSE);

	// 오버레이 배치
	if (nTab == TAB_JOB && m_list.GetItemCount() >= 12)
	{
		// 현재 상태/우선순위를 콤보 초기 선택으로
		CString strStatus = m_list.GetItemText(2, 1);   // "[11] ..." 형태
		for (int i = 0; i < m_arStatusCd.GetCount(); i++)
			if (strStatus.Find(_T("[") + m_arStatusCd[i] + _T("]")) == 0)
			{ m_cmbStatus.SetCurSel(i); break; }
		CString strPri = m_list.GetItemText(9, 1); strPri.Trim();
		int nPri = _ttoi(strPri);
		if (nPri >= 1 && nPri <= 9) m_cmbPri.SetCurSel(nPri - 1);

		PlaceOverCell(&m_cmbStatus, 2, 2, TRUE);
		PlaceOverCell(&m_btnStatus, 2, 3, TRUE);
		PlaceOverCell(&m_cmbPri,    9, 2, TRUE);
		PlaceOverCell(&m_btnPri,    9, 3, TRUE);
	}
	else if (nTab == TAB_CV && m_list.GetItemCount() >= 2)
	{
		PlaceOverCell(&m_edtCvJob,   1, 2, TRUE);
		PlaceOverCell(&m_btnCvWrite, 1, 3, TRUE);
		m_btnCvDelete.ShowWindow(SW_SHOW);
	}
	else if ((nTab == TAB_SC || nTab == TAB_RTV) && m_list.GetItemCount() >= 7)
	{
		PlaceOverCell(&m_btnForce, 6, 3, TRUE);   // 차상화물 행의 [확인] 칸
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

// ── 액션들 ─────────────────────────────────────────────────────

// 공통 실행 : 권한 + 트랜잭션 + 클라이언트 로그 + UPDATE (ViewJobListDlg 의 수정 절차와 동일)
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
	// CvSkinDlg 의 [삭제] 와 같은 SQL - 지시(OD) 필드 클리어
	CString strSql;
	strSql.Format(_T("UPDATE CV_DATA SET LUGG_NO_OD = '0', DEST_POS_OD = '0', JOB_TYP_OD = '0', PULP_SENSOR_OD = '0', WRITE_UPD_DT = GETDATE() WHERE WH_TYP = '%s' AND MC_NO = '%s'"),
		(LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strUnit);
	if (ExecUpdate(strSql, _T("CV_DATA UPDATE : OD CLEAR (") + strUnit + _T(")"), _T("0"), _T("CCvSkinDlg")))
		Refresh();
}

void CPanelInfoDlg::OnBtnForce()
{
	BOOL bRtv = (m_tab.GetCurSel() == TAB_RTV);
	// 차상화물(없으면 지시 작업번호)을 대상으로 반송완료 상태를 강제 기록
	CString strJob = m_list.GetItemText(6, 1); strJob.Trim();
	if (strJob.IsEmpty() || strJob == _T("0") || strJob == _T("0000"))
	{
		strJob = m_list.GetItemText(9, 1); strJob.Trim();
	}
	if (strJob.IsEmpty() || strJob == _T("0") || strJob == _T("0000"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("대상 작업이 없습니다")));
		return;
	}
	CString strStatus = bRtv ? _T("39") : _T("29");   // RGV 반송완료 / SC 반송완료
	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("강제완료 하시겠습니까?")) + _T(" [") + strJob + _T(" -> ") + strStatus + _T("]"), MB_YESNO) != IDYES)
		return;
	CString strSql;
	strSql.Format(_T("UPDATE JOB_MST SET JOB_STATUS = '%s', UPD_DT = GETDATE() WHERE WH_TYP = '%s' AND LUGG_NO = '%s'"),
		(LPCTSTR)strStatus, (LPCTSTR)m_pDoc->m_WH_TYP, (LPCTSTR)strJob);
	if (ExecUpdate(strSql, _T("JOB_MST UPDATE : 강제완료 JOB_STATUS -> ") + strStatus, strJob, bRtv ? _T("CRtvSkinDlg") : _T("CScSkinDlg")))
		Refresh();
}

// ── 크기/타이머/탭 ────────────────────────────────────────────

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
		// 콤보/에디트 조작 중에는 갱신으로 방해하지 않는다
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
	FillUnits();
	UpdateTitle();
	Refresh();
}

void CPanelInfoDlg::OnUnitChanged()
{
	Refresh();
}
