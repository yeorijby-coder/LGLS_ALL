
// EcsView.cpp : CEcsView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Ecs.h"
#endif

#include "MainFrm.h"
#include "EcsDoc.h"
#include "EcsView.h"
#include "WarningDlg.h"
#include "SystemLoginDlg.h"
#include "CvSkinDlg.h"
#include "ScSkinDlg.h"
#include "RtvSkinDlg.h"
#include "ViewUsageRackDlg.h"
#include "FireMessageDlg.h"
#include "ViewJobListDlg.h"
#include "RecordSetWrap.h"
#include "PanelJobDlg.h"		// [LGLS 2026-09-13] 새 메인 화면 - 작업정보 판넬을 왼쪽에 고정
#include "PanelVehDlg.h"		// [LGLS 2026-09-13] 새 메인 화면 - 설비반송 판넬도 왼쪽에

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEcsView
//
IMPLEMENT_DYNCREATE(CEcsView, CFormView)

BEGIN_MESSAGE_MAP(CEcsView, CFormView)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SEARCH, &CEcsView::OnSearch)
	ON_MESSAGE(WM_USER_REFRESH_LAYOUT, &CEcsView::OnViewLayoutInfo)
	ON_MESSAGE(WM_USER_CONTROL_CLICK, &CEcsView::OnControlClick)
	ON_MESSAGE(WM_USER_REFRESH_DIALOG, &CEcsView::OnRefreshDialog)
END_MESSAGE_MAP()




// ═══════════════════════════════════════════════════════════════════════════
// [LGLS 2026-09-13] 메인 화면 새 배치 (Ecs.ini [MENU] MAIN_UI=2) - 사용자 지시 1안
//   종전 범례 표(69칸)가 화면 왼쪽 절반을 먹어 지도로 눈이 가지 않았다.
//   그 자리를 위에서부터 [통신 상태] [범례] [작업정보] 로 나눈다. 지도는 손대지 않는다.
//   세 칸의 경계에 손잡이를 두어 마우스로 크기를 조절하고, 놓은 크기는 Ecs.ini 에 남긴다.
//   통신 상태는 DB 의 마지막 갱신 시각으로 판단한다 - 리본의 램프 3개는 색이 고정이라 실제 상태가 아니다.
//     설비   = CV_DATA.READ_UPD_DT   (WCS_TASK_CV 가 0.3초마다 갱신)
//     상위   = HOST_IF_LOG.INS_DT    (HOST 전문이 오갈 때마다)
//     스케줄 = JOB_MST.UPD_DT        (IO_TASK 가 작업을 진행시킬 때마다)
// ═══════════════════════════════════════════════════════════════════════════
class CLglsInfoBar : public CWnd
{
public:
	CEcsDoc* m_pDoc;
	int      m_nKind;			// 0 = 통신 상태, 1 = 범례
	CLglsInfoBar() { m_pDoc = NULL; m_nKind = 0; for (int i = 0; i < 3; i++) m_nAge[i] = -1; }
	enum { TIMER_HB = 7501, CHIP_CNT = 28 };
	// 이름이 잘리지 않을 만큼만 열을 둔다(폭이 좁으면 줄 수가 는다).
	static int ChipCols(int nWidth) { if (nWidth < 420) return 3; if (nWidth < 560) return 4; if (nWidth < 760) return 6; return 8; }
	// 그룹 4개(10/10/3/5개)를 다 담는 데 필요한 높이 - 범례 칸의 기본값으로 쓴다
	static int LegendBestH(int nWidth)
	{
		int c = ChipCols(nWidth - 4), n[4] = { 10, 10, 3, 5 }, h = 14;		// -4 = 테두리
		for (int g = 0; g < 4; g++) h += 17 + ((n[g] + c - 1) / c) * 18 + 3;
		return h;
	}
protected:
	int m_nAge[3];			// 설비 / 상위 / 스케줄 마지막 갱신 경과(초). -1 = 아직 모름
	void ReadHeartbeat();
	void PaintComm(CDC& dc, CRect rc, CFont& fnt, CFont& fntB);
	void PaintLegend(CDC& dc, CRect rc, CFont& fnt, CFont& fntB);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CLglsInfoBar, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CLglsInfoBar::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;			// 바탕은 OnPaint 에서 한 번에 그린다(깜빡임 방지)
}

void CLglsInfoBar::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_HB)
	{
		ReadHeartbeat();
		Invalidate(FALSE);
		return;
	}
	CWnd::OnTimer(nIDEvent);
}

void CLglsInfoBar::ReadHeartbeat()
{
	if (m_pDoc == NULL || m_nKind != 0) return;
	CString strSql = _T("");
	strSql += _T(" SELECT ISNULL((SELECT DATEDIFF(second, MAX(READ_UPD_DT), GETDATE()) FROM CV_DATA),     99999) AS EQP \n");
	strSql += _T("      , ISNULL((SELECT DATEDIFF(second, MAX(INS_DT),      GETDATE()) FROM HOST_IF_LOG), 99999) AS HST \n");
	strSql += _T("      , ISNULL((SELECT DATEDIFF(second, MAX(UPD_DT),      GETDATE()) FROM JOB_MST),     99999) AS SCH ");
	int nRowCnt = 0;
	CString strMsg = _T("");
	_RecordsetPtr pRs = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMsg);
	if (nRowCnt <= 0)
	{
		for (int i = 0; i < 3; i++) m_nAge[i] = -1;		// 조회 실패 = 회색(모름)
		return;
	}
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRs);
	pRsw->MoveFirst();
	m_nAge[0] = _ttoi(pRsw->GetItem(_T("EQP")));
	m_nAge[1] = _ttoi(pRsw->GetItem(_T("HST")));
	m_nAge[2] = _ttoi(pRsw->GetItem(_T("SCH")));
	delete pRsw;
}

// 경과 초 → 램프 색 (초록 정상 / 주황 늦음 / 빨강 끊김 / 회색 모름)
static COLORREF PfLamp(int nAge, int nOk, int nWarn)
{
	if (nAge < 0 || nAge >= 99999) return RGB(150, 150, 150);
	if (nAge <= nOk)   return RGB( 30, 142,  62);
	if (nAge <= nWarn) return RGB(214, 139,   0);
	return RGB(192, 38, 31);
}

static CString PfAgeText(int nAge)
{
	CString s;
	if (nAge < 0 || nAge >= 99999) return _T("확인 불가");
	if (nAge < 60)   { s.Format(_T("%d초 전"), nAge); return s; }
	if (nAge < 3600) { s.Format(_T("%d분 전"), nAge / 60); return s; }
	s.Format(_T("%d시간 전"), nAge / 3600);
	return s;
}

static CString PfStateText(int nAge, int nOk, int nWarn)
{
	if (nAge < 0 || nAge >= 99999) return _T("확인 불가");
	if (nAge <= nOk)   return _T("정상");
	if (nAge <= nWarn) return _T("느림");
	return _T("끊김");
}

// 통신 상태 : 칸이 낮으면 한 줄에 3개, 높으면 한 줄에 하나씩 크게
void CLglsInfoBar::PaintComm(CDC& dc, CRect rc, CFont& fnt, CFont& fntB)
{
	struct { LPCTSTR name; int ok; int warn; } CM[3] = {
		{ _T("설비 통신"), 10, 60 }, { _T("상위 통신"), 60, 300 }, { _T("스케줄러"), 120, 900 } };
	BOOL bTall = (rc.Height() >= 70);
	for (int i = 0; i < 3; i++)
	{
		CRect rcC;
		if (bTall) rcC.SetRect(6, 4 + i * (rc.Height() - 8) / 3, rc.right - 6, 4 + (i + 1) * (rc.Height() - 8) / 3);
		else       rcC.SetRect(6 + i * rc.Width() / 3, 3, (i + 1) * rc.Width() / 3 - 2, rc.bottom - 3);
		int nCy = (rcC.top + rcC.bottom) / 2;
		CRect rcDot(rcC.left, nCy - 6, rcC.left + 12, nCy + 6);
		CBrush br(PfLamp(m_nAge[i], CM[i].ok, CM[i].warn));
		CBrush* pOldBr = dc.SelectObject(&br);
		dc.Ellipse(rcDot);
		dc.SelectObject(pOldBr);
		CRect rcT(rcDot.right + 6, rcC.top, rcC.right, rcC.bottom);
		dc.SelectObject(&fntB);
		dc.SetTextColor(RGB(20, 32, 41));
		dc.DrawText(bTall ? CM[i].name : CString(CM[i].name).Left(2), rcT, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		dc.SelectObject(&fnt);
		dc.SetTextColor(RGB(90, 108, 118));
		CString strR = PfAgeText(m_nAge[i]);
		if (bTall) strR = PfStateText(m_nAge[i], CM[i].ok, CM[i].warn) + _T("  ·  ") + strR;
		dc.DrawText(strR, rcT, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		if (bTall && i < 2)
			dc.FillSolidRect(CRect(6, rcC.bottom, rc.right - 6, rcC.bottom + 1), RGB(228, 234, 236));
	}
}

// 범례 : 예전 범례표처럼 ★그룹으로 묶어★ 그린다(작업 색상 / C/V 상태 / S/C·RGV 상태 / 레일).
//   칸을 줄이면 열 수를 늘려 접고, 아주 낮으면 제목을 빼고 색칩만 늘어놓는다.
void CLglsInfoBar::PaintLegend(CDC& dc, CRect rc, CFont& fnt, CFont& fntB)
{
	CConfig* pCfg = (m_pDoc != NULL) ? m_pDoc->m_pConfig : NULL;
	if (pCfg == NULL) return;
	struct LGIT { int grp; int rail; COLORREF clr; LPCTSTR name; };
	LGIT IT[CHIP_CNT] = {
		{ 0, 0, pCfg->m_clrUSER_COLOR_STO,        _T("입고") },
		{ 0, 0, pCfg->m_clrUSER_COLOR_RET,        _T("출고") },
		{ 0, 0, pCfg->m_clrUSER_COLOR_MOVE,       _T("이동") },
		{ 0, 0, pCfg->m_clrUSER_COLOR_RTR,        _T("랙투랙") },
		{ 0, 0, pCfg->m_clrUSER_COLOR_ATA,        _T("호기간이동") },
		{ 0, 0, pCfg->m_clrUSER_COLOR_SEMI_STO,   _T("반자동 입고") },
		{ 0, 0, pCfg->m_clrUSER_COLOR_SEMI_RET,   _T("반자동 출고") },
		{ 0, 0, pCfg->m_clrUSER_COLOR_SEMI_MOVE,  _T("반자동 이동") },
		{ 0, 0, pCfg->m_clrUSER_COLOR_SEMI_RTR,   _T("반자동 랙투랙") },
		{ 0, 0, pCfg->m_clrUSER_COLOR_SEMI_ATA,   _T("반자동 호기간") },
		{ 1, 0, pCfg->m_clrUSER_COLOR_STN_STO,    _T("입고대") },
		{ 1, 0, pCfg->m_clrUSER_COLOR_STN_RET,    _T("출고대") },
		{ 1, 0, pCfg->m_clrUSER_COLOR_HS_STO,     _T("입고 HS") },
		{ 1, 0, pCfg->m_clrUSER_COLOR_HS_RET,     _T("출고 HS") },
		{ 1, 0, pCfg->m_clrUSER_COLOR_SUSPEND,    _T("일시정지") },
		{ 1, 0, pCfg->m_clrUSER_COLOR_ERROR,      _T("에러") },
		{ 1, 0, pCfg->m_clrUSER_COLOR_MANUAL,     _T("수동") },
		{ 1, 0, pCfg->m_clrUSER_COLOR_DISCONNECT, _T("통신두절") },
		{ 1, 0, pCfg->m_clrUSER_COLOR_CV_SEARCH,  _T("검색") },
		{ 1, 0, pCfg->m_clrUSER_COLOR_TRACKING,   _T("작업번호 있음") },
		{ 2, 0, LEGEND_IDLE_GRAY,                 _T("작업없음(정상)") },
		{ 2, 0, pCfg->m_clrUSER_COLOR_MANUAL,     _T("미가동(수동)") },
		{ 2, 0, pCfg->m_clrUSER_COLOR_ERROR,      _T("에러") },
		{ 3, 1, pCfg->m_clrUSER_COLOR_STO_SUSPEND,  _T("입고 금지") },
		{ 3, 1, pCfg->m_clrUSER_COLOR_RET_SUSPEND,  _T("출고 금지") },
		{ 3, 1, pCfg->m_clrUSER_COLOR_ALL_SUSPEND,  _T("입출고 정지") },
		{ 3, 1, pCfg->m_clrUSER_COLOR_RAIL_ERROR,   _T("레일 에러") },
		{ 3, 1, pCfg->m_clrUSER_COLOR_SC_INVK,      _T("작업중") } };
	LPCTSTR GRP[4] = { _T("작업 색상"), _T("C/V 상태"), _T("S/C · RGV 상태"), _T("S/C · RGV 레일") };

	int nCol = ChipCols(rc.Width());
	int nW   = (rc.Width() - 8) / nCol;
	int nRowH = 18, nHdrH = 17;
	BOOL bGroup = (rc.Height() >= 90);			// 칸이 아주 낮으면 제목 없이 색칩만

	dc.SetTextColor(RGB(35, 48, 56));
	int y = 3;
	int nPrevGrp = -1;
	int nSlot = 0;
	for (int i = 0; i < CHIP_CNT; i++)
	{
		if (bGroup && IT[i].grp != nPrevGrp)
		{
			if (nPrevGrp >= 0) y += ((nSlot + nCol - 1) / nCol) * nRowH + 3;	// 앞 그룹이 쓴 줄
			if (y + nHdrH > rc.bottom) break;
			dc.SelectObject(&fntB);
			dc.SetTextColor(RGB(15, 110, 103));
			CRect rcH(4, y, rc.right - 4, y + nHdrH);
			dc.DrawText(GRP[IT[i].grp], rcH, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			CSize sz = dc.GetTextExtent(GRP[IT[i].grp]);
			dc.FillSolidRect(CRect(8 + sz.cx, y + nHdrH / 2, rc.right - 6, y + nHdrH / 2 + 1), RGB(222, 232, 232));
			dc.SelectObject(&fnt);
			dc.SetTextColor(RGB(35, 48, 56));
			y += nHdrH;
			nPrevGrp = IT[i].grp;
			nSlot = 0;
		}
		int x  = 4 + (nSlot % nCol) * nW;
		int yy = y + (nSlot / nCol) * nRowH;
		if (yy + 14 > rc.bottom) break;			// 칸에 들어가는 만큼만
		CBrush brFrm(RGB(90, 90, 90));
		if (IT[i].rail != 0)
		{
			// 레일은 선으로 - 트랙 사이 레일 색이라 네모보다 알아보기 쉽다
			dc.FillSolidRect(CRect(x, yy + 7, x + 16, yy + 10), IT[i].clr);
			dc.FillSolidRect(CRect(x, yy + 5, x + 3, yy + 12), IT[i].clr);
			dc.FillSolidRect(CRect(x + 13, yy + 5, x + 16, yy + 12), IT[i].clr);
		}
		else
		{
			CRect rcBox(x, yy + 3, x + 13, yy + 15);
			dc.FillSolidRect(rcBox, IT[i].clr);
			dc.FrameRect(rcBox, &brFrm);
		}
		CRect rcTx(x + 19, yy, x + nW - 2, yy + nRowH);
		dc.DrawText(IT[i].name, rcTx, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		nSlot++;
		if (!bGroup) { nPrevGrp = IT[i].grp; }
	}
}

void CLglsInfoBar::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	if (rc.Width() <= 0 || rc.Height() <= 0) return;

	CDC mem;
	mem.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap* pOldBmp = mem.SelectObject(&bmp);
	mem.FillSolidRect(rc, RGB(255, 255, 255));
	mem.SetBkMode(TRANSPARENT);

	CFont fnt, fntB;
	fnt.CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, _T("돋움"));
	fntB.CreateFont(-12, 0, 0, 0, FW_BOLD,   0, 0, 0, HANGEUL_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, _T("돋움"));
	CFont* pOldFont = mem.SelectObject(&fnt);

	if (m_nKind == 0) PaintComm(mem, rc, fnt, fntB);
	else              PaintLegend(mem, rc, fnt, fntB);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &mem, 0, 0, SRCCOPY);
	mem.SelectObject(pOldFont);
	mem.SelectObject(pOldBmp);
}

// ── 칸 크기 조절 손잡이 ─────────────────────────────────────────────────────
//   끄는 동안 바로 다시 배치하고, 손을 떼면 그 크기를 Ecs.ini 에 남긴다.
class CLglsSplitBar : public CWnd
{
public:
	CEcsView* m_pView;
	int       m_nWhich;			// 0=왼쪽 칸 폭, 1=통신 칸 높이, 2=범례 칸 높이
	BOOL      m_bVert;			// TRUE=세로 손잡이(좌우로 끈다)
	CLglsSplitBar() { m_pView = NULL; m_nWhich = 0; m_bVert = FALSE; m_bDrag = FALSE; }
protected:
	BOOL   m_bDrag;
	CPoint m_ptLast;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CLglsSplitBar, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CLglsSplitBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	UNREFERENCED_PARAMETER(pWnd); UNREFERENCED_PARAMETER(nHitTest); UNREFERENCED_PARAMETER(message);
	::SetCursor(::LoadCursor(NULL, m_bVert ? IDC_SIZEWE : IDC_SIZENS));
	return TRUE;
}

void CLglsSplitBar::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(226, 232, 234));
	// 가운데 손잡이 점 세 개 - 끌 수 있는 자리임을 알린다
	int cx = rc.Width() / 2, cy = rc.Height() / 2;
	for (int i = -1; i <= 1; i++)
	{
		int x = m_bVert ? cx - 1 : cx - 1 + i * 7;
		int y = m_bVert ? cy - 1 + i * 7 : cy - 1;
		dc.FillSolidRect(CRect(x, y, x + 2, y + 2), RGB(140, 152, 156));
	}
}

void CLglsSplitBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	m_bDrag = TRUE;
	m_ptLast = point;
	ClientToScreen(&m_ptLast);
	SetCapture();
}

void CLglsSplitBar::OnMouseMove(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	if (!m_bDrag || m_pView == NULL) return;
	CPoint pt(point);
	ClientToScreen(&pt);
	int nDelta = m_bVert ? (pt.x - m_ptLast.x) : (pt.y - m_ptLast.y);
	if (nDelta == 0) return;
	m_ptLast = pt;
	m_pView->OnUiDrag(m_nWhich, nDelta);
}

void CLglsSplitBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags); UNREFERENCED_PARAMETER(point);
	if (!m_bDrag) return;
	m_bDrag = FALSE;
	ReleaseCapture();
	if (m_pView != NULL) m_pView->SaveUiSizes();
}

// 왼쪽 고정 칸(통신 / 범례 / 설비반송 / 작업정보)과 손잡이 4개를 만든다. Ecs.ini [MENU] MAIN_UI=2 일 때만.
void CEcsView::CreateMainUi2()
{
	m_nMainUi = ::GetPrivateProfileInt(_T("MENU"), _T("MAIN_UI"), 1, ECS_INI_FILE);
	if (m_nMainUi != 2) return;
	CEcsDoc* pDoc = GetDocument();
	if (pDoc == NULL) return;

	// 지난번에 끌어 놓은 크기 (0 = 기본값)
	m_nUiLeftW = ::GetPrivateProfileInt(_T("MENU"), _T("MAIN_UI_LEFT_W"), 0, ECS_INI_FILE);
	m_nUiCommH = ::GetPrivateProfileInt(_T("MENU"), _T("MAIN_UI_COMM_H"), 0, ECS_INI_FILE);
	m_nUiLegH  = ::GetPrivateProfileInt(_T("MENU"), _T("MAIN_UI_LEG_H"),  0, ECS_INI_FILE);
	m_nUiVehH  = ::GetPrivateProfileInt(_T("MENU"), _T("MAIN_UI_VEH_H"),  0, ECS_INI_FILE);

	LPCTSTR pszCls = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)::GetStockObject(WHITE_BRUSH), NULL);

	for (int k = 0; k < 2; k++)			// 0 = 통신, 1 = 범례
	{
		CWnd** ppDst = (k == 0) ? &m_pCommBar : &m_pLegBar;
		if (*ppDst != NULL) continue;
		CLglsInfoBar* pBar = new CLglsInfoBar();
		pBar->m_pDoc = pDoc;
		pBar->m_nKind = k;
		if (pBar->CreateEx(0, pszCls, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 10, 10), this, 0))
		{
			if (k == 0) pBar->SetTimer(CLglsInfoBar::TIMER_HB, 5000, NULL);
			*ppDst = pBar;
		}
		else
			delete pBar;
	}

	// 이미 있는 판넬 2개를 그대로 쓴다 - 표 항목·자동 갱신이 판넬과 같다.
	if (m_pVehFixed == NULL)
	{
		CPanelVehDlg* pVeh = new CPanelVehDlg(this);
		pVeh->m_pDoc = pDoc;
		if (pVeh->Create(IDD_PANEL_VEH, this)) { pVeh->ShowWindow(SW_SHOW); m_pVehFixed = pVeh; }
		else delete pVeh;
	}
	if (m_pJobFixed == NULL)
	{
		CPanelJobDlg* pJob = new CPanelJobDlg(this);
		pJob->m_pDoc = pDoc;
		if (pJob->Create(IDD_PANEL_JOB, this)) { pJob->ShowWindow(SW_SHOW); m_pJobFixed = pJob; }
		else delete pJob;
	}

	CWnd** ppSp[4] = { &m_pSplitV, &m_pSplitH1, &m_pSplitH2, &m_pSplitH3 };
	for (int i = 0; i < 4; i++)
	{
		if (*ppSp[i] != NULL) continue;
		CLglsSplitBar* pSp = new CLglsSplitBar();
		pSp->m_pView = this;
		pSp->m_nWhich = i;
		pSp->m_bVert = (i == 0);
		if (pSp->CreateEx(0, pszCls, _T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 10, 10), this, 0))
			*ppSp[i] = pSp;
		else
			delete pSp;
	}

	LayoutMainUi2();
	CLib::UiLog(_T("[UI2] left column created (comm=%d legend=%d veh=%d job=%d)"),
		(m_pCommBar != NULL) ? 1 : 0, (m_pLegBar != NULL) ? 1 : 0,
		(m_pVehFixed != NULL) ? 1 : 0, (m_pJobFixed != NULL) ? 1 : 0);
}

// 손잡이를 끌면 그 칸만 늘리고 나머지는 따라 움직인다.
void CEcsView::OnUiDrag(int nWhich, int nDelta)
{
	if (m_nMainUi != 2) return;
	CRect rc;
	GetClientRect(&rc);
	if (nWhich == 0)
	{
		if (m_nUiLeftW <= 0) m_nUiLeftW = rc.Width() * 19 / 42;
		m_nUiLeftW += nDelta;
		if (m_nUiLeftW < 320) m_nUiLeftW = 320;
		if (m_nUiLeftW > rc.Width() - 360) m_nUiLeftW = rc.Width() - 360;
	}
	else
	{
		int* pnH = (nWhich == 1) ? &m_nUiCommH : ((nWhich == 2) ? &m_nUiLegH : &m_nUiVehH);
		int  nDef = (nWhich == 1) ? 96 : ((nWhich == 2) ? 240 : 130);
		if (*pnH <= 0) *pnH = nDef;
		*pnH += nDelta;
		if (*pnH < 24) *pnH = 24;
		int nOther = (m_nUiCommH > 0 ? m_nUiCommH : 96) + (m_nUiLegH > 0 ? m_nUiLegH : 240) + (m_nUiVehH > 0 ? m_nUiVehH : 130) - *pnH;
		int nMax = rc.Height() - nOther - 140;			// 작업정보가 최소 140 은 되게
		if (nMax < 40) nMax = 40;
		if (*pnH > nMax) *pnH = nMax;
	}
	LayoutMainUi2();
}

void CEcsView::SaveUiSizes()
{
	if (m_nMainUi != 2) return;
	CString s;
	s.Format(_T("%d"), m_nUiLeftW); ::WritePrivateProfileString(_T("MENU"), _T("MAIN_UI_LEFT_W"), s, ECS_INI_FILE);
	s.Format(_T("%d"), m_nUiCommH); ::WritePrivateProfileString(_T("MENU"), _T("MAIN_UI_COMM_H"), s, ECS_INI_FILE);
	s.Format(_T("%d"), m_nUiLegH);  ::WritePrivateProfileString(_T("MENU"), _T("MAIN_UI_LEG_H"),  s, ECS_INI_FILE);
	s.Format(_T("%d"), m_nUiVehH);  ::WritePrivateProfileString(_T("MENU"), _T("MAIN_UI_VEH_H"),  s, ECS_INI_FILE);
	CLib::UiLog(_T("[UI2] size saved left=%d comm=%d legend=%d veh=%d"), m_nUiLeftW, m_nUiCommH, m_nUiLegH, m_nUiVehH);
}

// 왼쪽 칸이 차지한 만큼 지도(레이아웃)를 오른쪽으로 밀어 다시 배치한다.
//   CEcsLayout 이 이미 왼쪽 여백(m_nLeftInsetS)을 받아 배치하므로 그 값만 바꿔 주면 된다.
void CEcsView::RelayoutMap()
{
	CEcsDoc* pDoc = GetDocument();
	if (pDoc == NULL || m_tabLayout.GetSafeHwnd() == NULL) return;
	int nSel = m_tabLayout.GetCurSel();
	if (nSel < 0 || nSel >= pDoc->m_pEcsLayOuts.GetSize()) return;
	CEcsLayout* pLayout = pDoc->m_pEcsLayOuts[nSel];
	if (pLayout == NULL) return;
	CRect rc;
	GetClientRect(&rc);
	pLayout->OnSize(this, 0, rc.Width(), rc.Height());
}

// 왼쪽 칸의 기본 폭은 종전 범례가 쓰던 만큼(격자 42칸 중 19칸)이다.
void CEcsView::LayoutMainUi2()
{
	if (m_nMainUi != 2) return;
	CRect rc;
	GetClientRect(&rc);
	if (rc.Width() < 300 || rc.Height() < 300) return;

	int nLeft = (m_nUiLeftW > 0) ? m_nUiLeftW : rc.Width() * 19 / 42;
	if (nLeft < 320) nLeft = 320;
	if (nLeft > rc.Width() - 360) nLeft = rc.Width() - 360;
	int nCommH = (m_nUiCommH > 0) ? m_nUiCommH : 96;
	int nLegH  = m_nUiLegH;
	if (nLegH <= 0)			// 기본값 = 그룹 4개가 다 들어가는 높이(폭에 따라 다르다)
	{
		nLegH = CLglsInfoBar::LegendBestH(nLeft - 12);
		if (nLegH < 120) nLegH = 120;
		if (nLegH > 340) nLegH = 340;
	}
	int nVehH  = (m_nUiVehH  > 0) ? m_nUiVehH  : 130;
	int nRoom  = rc.Height() - 140;					// 작업정보 몫을 남긴다
	while (nCommH + nLegH + nVehH > nRoom)
	{
		if (nLegH >= nVehH && nLegH > 40) nLegH -= 10;
		else if (nVehH > 40) nVehH -= 10;
		else if (nCommH > 30) nCommH -= 10;
		else break;
	}
	const int nSp = 6;			// 손잡이 두께
	int x = 6, w = nLeft - 12, y = 6;
	if (w < 100) w = 100;

	if (m_pCommBar  != NULL && ::IsWindow(m_pCommBar->m_hWnd))  m_pCommBar->SetWindowPos(&wndTop, x, y, w, nCommH, SWP_SHOWWINDOW);
	y += nCommH;
	if (m_pSplitH1  != NULL && ::IsWindow(m_pSplitH1->m_hWnd))  m_pSplitH1->SetWindowPos(&wndTop, x, y, w, nSp, SWP_SHOWWINDOW);
	y += nSp;
	if (m_pLegBar   != NULL && ::IsWindow(m_pLegBar->m_hWnd))   m_pLegBar->SetWindowPos(&wndTop, x, y, w, nLegH, SWP_SHOWWINDOW);
	y += nLegH;
	if (m_pSplitH2  != NULL && ::IsWindow(m_pSplitH2->m_hWnd))  m_pSplitH2->SetWindowPos(&wndTop, x, y, w, nSp, SWP_SHOWWINDOW);
	y += nSp;
	if (m_pVehFixed != NULL && ::IsWindow(m_pVehFixed->m_hWnd)) m_pVehFixed->SetWindowPos(&wndTop, x, y, w, nVehH, SWP_SHOWWINDOW);
	y += nVehH;
	if (m_pSplitH3  != NULL && ::IsWindow(m_pSplitH3->m_hWnd))  m_pSplitH3->SetWindowPos(&wndTop, x, y, w, nSp, SWP_SHOWWINDOW);
	y += nSp;
	int nJobH = rc.Height() - y - 8;
	if (nJobH < 80) nJobH = 80;
	if (m_pJobFixed != NULL && ::IsWindow(m_pJobFixed->m_hWnd)) m_pJobFixed->SetWindowPos(&wndTop, x, y, w, nJobH, SWP_SHOWWINDOW);
	if (m_pSplitV   != NULL && ::IsWindow(m_pSplitV->m_hWnd))   m_pSplitV->SetWindowPos(&wndTop, nLeft - 2, 6, nSp, rc.Height() - 14, SWP_SHOWWINDOW);

	// 지도도 왼쪽 칸만큼 밀어 다시 배치 (판넬을 켰을 때 화면이 줄어들던 것과 같은 동작)
	if (CEcsLayout::m_nLeftInsetS != nLeft + 6)
	{
		CEcsLayout::m_nLeftInsetS = nLeft + 6;
		RelayoutMap();
	}
}

CEcsView::CEcsView()
	: CFormView(CEcsView::IDD)
{
	m_ullIniWriteTime = 0; m_ullIniPendingTime = 0; m_nIniZoomBtn = -1;	// [LGLS 2026-09-12] ini 핫 리로드
	// [LGLS 2026-09-13] 메인 화면 새 배치 (MAIN_UI=2)
	m_nMainUi = 1; m_nUiLeftW = 0; m_nUiCommH = 0; m_nUiLegH = 0;
	m_nUiVehH = 0;
	m_pCommBar = NULL; m_pLegBar = NULL; m_pVehFixed = NULL; m_pJobFixed = NULL;
	m_pSplitV = NULL; m_pSplitH1 = NULL; m_pSplitH2 = NULL; m_pSplitH3 = NULL;
	m_nSearchType = 0;
	m_bSearchFlag = FALSE;
	m_nSearchCount = 0;
	m_bTest = false;
}

CEcsView::~CEcsView()
{
}

void CEcsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAYOUT_TAB, m_tabLayout);
}

BOOL CEcsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}
#define			ID_MAIN_TIMER		100

void CEcsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();

	CEcsDoc* pDoc = GetDocument();
	DEBUGER_ASSERT_VALID(pDoc != NULL);

	//pDoc->m_layoutHUN_GF.OnInitialUpdate(this);
	//pDoc->m_layoutHUN_GFDECK.OnInitialUpdate(this);
	//pDoc->m_layoutHUN_1F.OnInitialUpdate(this);
	//pDoc->m_layoutHUN_1FDECK.OnInitialUpdate(this);
	//pDoc->m_layoutHUN_ASSEMBLE.OnInitialUpdate(this);

	//pDoc->m_pViewJobListDlg = new CViewJobListDlg(pDoc);
	//pDoc->m_pViewJobListDlg->Create(IDD_VIEW_JOBLIST);
	//pDoc->m_pViewJobListDlg->ShowWindow(SW_HIDE);//SW_HIDE

	// [LGLS 2026-08-22] 작업 체류 경고창을 미리 만들어 숨겨 둔다.
	//   TASK 로그는 서버에만 남아 운전자가 보지 못하므로 Client 에서 창으로 알린다.
	//   창 자신이 10초 주기로 JOB_MST 를 살펴 체류 작업이 생기면 스스로 나타난다.
	if (pDoc->m_pWarningDlg == NULL)
	{
		CWarningDlg* pWarn = new CWarningDlg(pDoc);
		if (pWarn->Create(IDD_WARNING_DLG, this))
		{
			pWarn->ShowWindow(SW_HIDE);
			pDoc->m_pWarningDlg = pWarn;
		}
		else
		{
			delete pWarn;
		}
	}

	int nMonitoringId = 35030 + pDoc->m_pConfig->m_nUSER_LAST_TAB_INDEX;
	pDoc->OnCommandRangeMainFrameMONITORING(nMonitoringId);
	CRect rect;
	int nMakeSizeX, nMakeSizeY, nWindowSizeX, nWindowSizeY;
	nMakeSizeX = MAKE_WINDOW_SIZE_X - 4;
	nMakeSizeY = MAKE_WINDOW_SIZE_Y - 200;

	GetDesktopWindow()->GetWindowRect(&rect);
	nWindowSizeX = rect.Width() - 4;
	nWindowSizeY = rect.Height() - 200;
	pDoc->m_pMaxSizeX = (double)nWindowSizeX/nMakeSizeX;
	pDoc->m_pMaxSizeY = (double)nWindowSizeY/nMakeSizeY;
	switch(pDoc->m_pConfig->m_nUSER_LAST_LANG)
	{
	case ID_LANGUAGE_KOR:
		{
			pDoc->m_enLang = EN_KOR;
			break;
		}
	case ID_LANGUAGE_ENGLISH:
		{
			pDoc->m_enLang = EN_ENG;
			break;
		}
	case ID_LANGUAGE_CHIN:
		{
			pDoc->m_enLang = EN_CHIN;
			break;
		}
	case ID_LANGUAGE_HUNGARIAN:
		{
			pDoc->m_enLang = EN_HUN;
			break;
		}    
	}

	// 레이아웃당 1개의 탭을 구성할 수 있다.  - 레이아웃에 여러대의 SC를 넣을 수 있다. 
	int i = 0;
	int nPlcCount = 0;
	int nLayoutCnt = pDoc->m_pEcsLayOuts.GetSize();
	for (i = 0 ; i < nLayoutCnt ; ++i)				
	{
		CEcsLayout * pEcsLayout = pDoc->m_pEcsLayOuts[i];

		if (pEcsLayout == NULL)
			continue;

		m_tabLayout.InsertItem(nPlcCount, pEcsLayout->m_strText, nPlcCount);	
		m_tabLayout.HighlightItem(nPlcCount++, FALSE);
		m_tabLayout.SetItemSize(CSize(200, 20));				// ?

		// 탭이 1개일때는 표시하지 않는다. 
	//	if (i==0)
	//		m_tabLayout.ShowWindow(SW_HIDE);
	}



	pDoc->UpdateRibbonLang();
	::SetTimer(this->m_hWnd, 1000, NULL, NULL);
	CreateMainUi2();		// [LGLS 2026-09-13] MAIN_UI=2 이면 왼쪽 고정 칸을 만든다
	Invalidate(TRUE);
}

void CEcsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDI_NON_COMP, point.x, point.y, this, TRUE);
#endif
}


// CEcsView 진단
//
#ifdef _DEBUG
void CEcsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CEcsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEcsDoc* CEcsView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEcsDoc)));
	return (CEcsDoc*)m_pDocument;
}
#endif //_DEBUG

// CEcsView 메시지 처리기
//
void CEcsView::OnTimer(UINT_PTR nIDEvent) 
{
	static int nCount = 1;	 
	int nSelCnt = 0;
	CEcsDoc* pDoc = GetDocument();
	if(pDoc == NULL)
	{
		CFormView::OnTimer(nIDEvent);
		return;
	}
	// [LGLS 2026-09-12] Ecs.ini 저장 자동 감지 - 1초에 한 번(다른 1초 작업과 50틱 어긋나게)
	if (nCount % 100 == 50)
		CheckIniHotReload();

 	if (nCount % 100 == 0) //HEART BEAT COLLECT DB
 	{
		if(pDoc->IsAlliveCollectDB() == FALSE)
		{
			pDoc->AlliveCollectDB(); //죽으나 마나 타이머에서 계속 생성시킬거임
		}

		//if(pDoc->IsAlliveCollectRequest() == FALSE)
		//{
		//	pDoc->AlliveCollectRequest(); //죽으나 마나 타이머에서 계속 생성시킬거임
		//}
 	}

	//HOST 상태확인을 위해 최초 1회만 탐
	if(pDoc->m_blConnectStatus == FALSE)
	{
		pDoc->EquipStatusCheck();
	}

	//TEXT 컨트롤 값에 따라서 보여주는 값 변화
	if (nCount % 100 == 0)
	{
		CDciControl* pDciControl = NULL; //보류4
		int nLayoutCnt = pDoc->m_pEcsLayOuts.GetSize();
	
		for(int nIdxLayout = 0; nIdxLayout < nLayoutCnt; nIdxLayout++)
		{
			//CTrackInfo* pTrackInfo101 = pDoc->GetTrackInfoNew(_T("101"));
			CTrackInfo* pTrackInfo149 = pDoc->GetTrackInfoNew(_T("148"));
			CTrackInfo* pTrackInfo154 = pDoc->GetTrackInfoNew(_T("154"));
	
			if(pTrackInfo149 == NULL || pTrackInfo154 == NULL){ break; };
			
			
			CString strSTOCK_MODE, strREMOTE_CONTROL, strROLL_MODE;
			GetQrySelectStatusAll(pTrackInfo149->m_pCV_DATA, strSTOCK_MODE, strREMOTE_CONTROL, strROLL_MODE);

			CString strSTOCK_MODE_CID, strREMOTE_CONTROL_CID, strROLL_MODE_CID;
	
	
			strSTOCK_MODE_CID = _T("17111149");
			pDciControl = pDoc->GetLayout_PARM((EN_LAYOUT)nIdxLayout)->GetDciControl(strSTOCK_MODE_CID);
	
			if(pDciControl != NULL)
			{
				if (strSTOCK_MODE == _T("0"))
				{
					pDciControl->m_strText = _T("출고모드");
				}
				else
				{
					pDciControl->m_strText = _T("입고모드");
				}
				pDciControl->InvalidateControl(pDoc->m_hWndView, FALSE);
			}
	
			strROLL_MODE_CID = _T("17111154");
			pDciControl = pDoc->GetLayout_PARM((EN_LAYOUT)nIdxLayout)->GetDciControl(strROLL_MODE_CID);
	
			if(pDciControl != NULL)
			{
				if (strROLL_MODE == _T("0"))
				{
					pDciControl->m_strText = _T("정상모드");
				}
				else
				{
					pDciControl->m_strText = _T("배출모드");
				}				
				pDciControl->InvalidateControl(pDoc->m_hWndView, FALSE);
			}
	
			continue;
		}
	}
	
	nCount++;

	if(nCount >= 10000)
	{
		nCount = 0;
	}

	CFormView::OnTimer(nIDEvent);
}

void CEcsView::OnDestroy() 
{
	CFormView::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CEcsView printing

// [LGLS 2026-09-14] 메인 화면 그리기 시간 계측 (Ecs.ini [MENU] UI_TRACE=1 일 때 5초마다 한 줄)
//   buf    = 메모리 DC 에 레이아웃을 그려 화면으로 옮기는 시간
//   direct = 그 뒤 화면에 직접 한 번 더 그리는 시간
static void LglsPaintStat(LONGLONG llBuf, LONGLONG llDirect)
{
	static LARGE_INTEGER s_liFreq = { 0 };
	static int s_nCnt = 0;
	static double s_dBufSum = 0, s_dBufMax = 0, s_dDirSum = 0, s_dDirMax = 0, s_dTotMax = 0;
	static DWORD s_dwStart = 0;
	if (s_liFreq.QuadPart == 0) ::QueryPerformanceFrequency(&s_liFreq);
	double dBuf = llBuf * 1000.0 / (double)s_liFreq.QuadPart;
	double dDir = llDirect * 1000.0 / (double)s_liFreq.QuadPart;
	if (s_dwStart == 0) s_dwStart = ::GetTickCount();
	s_nCnt++; s_dBufSum += dBuf; s_dDirSum += dDir;
	if (dBuf > s_dBufMax) s_dBufMax = dBuf;
	if (dDir > s_dDirMax) s_dDirMax = dDir;
	if (dBuf + dDir > s_dTotMax) s_dTotMax = dBuf + dDir;
	DWORD dwNow = ::GetTickCount();
	if (dwNow - s_dwStart >= 5000)
	{
		CLib::UiLog(_T("[PAINT] %u ms 동안 %d 회 | buf 평균 %.2f 최대 %.2f ms | direct 평균 %.2f 최대 %.2f ms | 합 평균 %.2f 최대 %.2f ms"),
			dwNow - s_dwStart, s_nCnt, s_dBufSum / s_nCnt, s_dBufMax, s_dDirSum / s_nCnt, s_dDirMax,
			(s_dBufSum + s_dDirSum) / s_nCnt, s_dTotMax);
		s_nCnt = 0; s_dBufSum = s_dBufMax = s_dDirSum = s_dDirMax = s_dTotMax = 0; s_dwStart = dwNow;
	}
}

struct LglsPaintTimer
{
	LARGE_INTEGER t0, t1;
	BOOL bMid;
	LglsPaintTimer() : bMid(FALSE) { ::QueryPerformanceCounter(&t0); t1 = t0; }
	void Mid() { ::QueryPerformanceCounter(&t1); bMid = TRUE; }
	~LglsPaintTimer()
	{
		LARGE_INTEGER t2;
		::QueryPerformanceCounter(&t2);
		if (!bMid) t1 = t2;
		LglsPaintStat(t1.QuadPart - t0.QuadPart, t2.QuadPart - t1.QuadPart);
	}
};

void CEcsView::OnDraw(CDC* pDC) 
{
	LglsPaintTimer _lglsPt;	// [LGLS 2026-09-14] 그리기 시간 계측
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	CBitmap* pOldBitmap, bitmap;

	memDC.CreateCompatibleDC(pDC);

	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);

	DrawSelectedLayout(&memDC, pDoc);

	pDC->BitBlt(0,0, rect.Width(), rect.Height(), &memDC, 0,0,SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
	_lglsPt.Mid();

	// [LGLS 2026-09-14] 아래 호출을 뺐다 (사용자 지시 : 화면 지연 개선).
	//   바로 위에서 같은 레이아웃(선택된 탭)을 메모리 DC 에 다 그려 화면으로 옮겼는데,
	//   여기서 컨트롤 전체를 버퍼 없이 화면에 한 번 더 그리고 있었다. CEcsLayout::OnDraw 는
	//   탭 번호를 쓰지 않으므로 결과는 같고, 비용만 두 배에 깜빡임이 생겼다.
	//CEcsLayout layout;
	//int nTemp = -1;
	//if (m_tabLayout.GetSafeHwnd())
	//{
	//	nTemp = m_tabLayout.GetCurSel();
	//}
	//if (nTemp < 0)
	//	return;
	//CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
	//if (pEcsLayout == NULL)
	//	return;
	//pEcsLayout->OnDraw(this, pDC, nTemp + 1);
}

BOOL CEcsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	WheelSelectedLayout(pDoc, nFlags, zDelta, pt);	
	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CEcsView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	RButtonUpSelectedLayout(pDoc);
	CFormView::OnRButtonUp(nFlags, point);
}

void CEcsView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	LayoutMainUi2();		// [LGLS 2026-09-13] 왼쪽 고정 칸 다시 배치

	
	
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	
//	MoveSearchWnd(cx, cy, pDoc->m_bOnSizeReady); 
//	OnSizeSelectedLayout(pDoc, nType, cx, cy);		// 원본

	int width = cx;
	int height = cy;

	if (m_tabLayout.GetSafeHwnd())
	{
		int nTemp = m_tabLayout.GetCurSel();
		if (nTemp < 0)
			return;
		
		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return;

		int nLayoutCnt = pDoc->m_pEcsLayOuts.GetSize();
		for (int i = 0 ; i < nLayoutCnt ; i++)
		{
			if (nTemp == i)
			{
				// Tab 선택에 맞게 PLC 번호가 조정이 되어야 한다. 
				m_tabLayout.HighlightItem(nTemp, TRUE);	
				//SET_INT(IDC_COMBO_PLC, pEcsLayout->m_nRepresentPlc + 1);		// @@@
				//OnButton5();
			}
			else
			{
				m_tabLayout.HighlightItem(i, FALSE);
			}
		}

		pEcsLayout->OnSize(this, nType, cx, cy);
	}

}

void CEcsView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

// 	pDoc->m_bMoveFlag = TRUE;
// 	pDoc->m_PrevPoint = point;
// 	pDoc->m_tChecktime = CTime::GetCurrentTime();
	//OnLButtonDownSelectedLayout(pDoc, nFlags, point);		// 원본
	pDoc->m_bMoveFlag = TRUE; //1
	pDoc->m_PrevPoint = point;
	pDoc->m_tChecktime = CTime::GetCurrentTime();

	int a = m_tabLayout.GetCurSel();
	if (m_tabLayout.GetSafeHwnd())
	{
		int nTemp = m_tabLayout.GetCurSel();
		if (nTemp < 0)
			return;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return;

		pEcsLayout->OnButtonDown(this, nFlags, point);
	}

	CFormView::OnLButtonDown(nFlags, point);
}

// [LGLS 2026-09-15] 메인 화면의 "입고 모드"/"출고 모드" 칸을 더블클릭하면 방향을 반대로 바꾼다 (사용자 지시).
//   대상 칸 : C/V#11(트랙 122) = 레이아웃 id 90000122, C/V#2(트랙 103·104) = 90000103 (Cv.cpp 가 글자를 채우는 칸과 같다).
//   Client 는 PLC 에 직접 쓰지 않는다. CV_DATA 명령 컬럼(CMD_RQ_ID=DIR, CMD_RQ_PARM=0|1)에 남기면
//   설비 통신(CvThread.CvChg_CMD_RQ_YN)이 다음 주기에 D 방향 워드에 쓴다 - IO_TASK·C/V 창 [H/S 배출]과 같은 규약.
//   반대 방향 작업 화물이 그 트랙에 있으면 설비 통신이 전환을 보류한다(IsDualCvDirChangeHeld).
BOOL CEcsView::DirectionToggleByDblClk(CPoint point)
{
	CEcsDoc* pDoc = GetDocument();
	if (pDoc == NULL) return FALSE;
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if (pLayout == NULL || pLayout->GetDciMaster() == NULL) return FALSE;

	static const LPCTSTR s_szCid[2]   = { _T("90000122"), _T("90000103") };
	static const LPCTSTR s_szTrack[2] = { _T("122"), _T("103") };
	static const LPCTSTR s_szName[2]  = { _T("C/V#11"), _T("C/V#2") };
	int nHit = -1;
	CRect rcHit[2];
	for (int i = 0; i < 2; i++)
	{
		rcHit[i].SetRectEmpty();
		CString strCid(s_szCid[i]);
		CDciControl* pCtrl = pDoc->GetDciControl_FindAllLayout(strCid);
		if (pCtrl == NULL) continue;
		rcHit[i] = pLayout->GetDciMaster()->ConvertRectS(pCtrl->m_rcControlL);
		rcHit[i].NormalizeRect();
		if (nHit < 0 && rcHit[i].PtInRect(point)) nHit = i;
	}
	if (nHit < 0)
	{
		// 빗나간 더블클릭도 남긴다 - 현장에서 칸 위치가 다를 때 대조용 (UI_TRACE=1 일 때만)
		CLib::UiLog(_T("[DIR] dblclk miss pt=(%d,%d) 122=(%d,%d)-(%d,%d) 103=(%d,%d)-(%d,%d)"), point.x, point.y,
			rcHit[0].left, rcHit[0].top, rcHit[0].right, rcHit[0].bottom, rcHit[1].left, rcHit[1].top, rcHit[1].right, rcHit[1].bottom);
		return FALSE;
	}

	CLib::UiLog(_T("[DIR] dblclk track=%s"), s_szTrack[nHit]);
	if (!pDoc->Permission(_T("CCvSkinDlg"), UPD_YN))
	{
		AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
		return TRUE;
	}

	// 현재 방향은 DB 의 STOCK_MODE(설비 통신이 정규화한 논리값 1=출고, 옛 원시값 49 도 출고) 로 본다
	CString strSql;
	strSql.Format(_T(" SELECT ISNULL(STOCK_MODE,'') AS STOCK_MODE FROM CV_DATA WHERE WH_TYP = '%s' AND MC_NO = '%s' "),
		(LPCTSTR)pDoc->m_WH_TYP, s_szTrack[nHit]);
	int nRowCnt = 0;
	CString strMsg;
	_RecordsetPtr pRs = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMsg);
	if (nRowCnt <= 0)
	{
		AfxMessageBox(pDoc->GetMsgLangDef(_T("설비 데이터를 읽지 못했습니다")));
		return TRUE;
	}
	CString strMode;
	{
		CRecordSetWrap* pRsw = new CRecordSetWrap(pRs);
		pRsw->MoveFirst();
		strMode = pRsw->GetItem(_T("STOCK_MODE"));
		delete pRsw;
	}
	strMode.Trim();
	BOOL bOutNow = (strMode == _T("1") || strMode == _T("49"));
	CString strNow  = pDoc->GetMsgLangDef(bOutNow ? _T("출고 모드") : _T("입고 모드"));
	CString strNext = pDoc->GetMsgLangDef(bOutNow ? _T("입고 모드") : _T("출고 모드"));
	// [LGLS 2026-09-15] 확인창에 PLC 방향 워드 주소도 보인다(사용자 지시). 주소맵(PlcAddressMap.xml)은 설비 통신이 갖고 있어
	//   Client 는 Ecs.ini [MENU] DIR_ADDR_CV11 / DIR_ADDR_CV2 (기본 310 / 301 = D0300 + 설비번호 - 1) 로 표기한다.
	//   설비 통신 로그 "방향지시 … → D워드 310" 과 같은 번호다. 주소맵이 바뀌면 ini 만 맞추면 된다.
	int nDirAddr = ::GetPrivateProfileInt(_T("MENU"), (nHit == 0) ? _T("DIR_ADDR_CV11") : _T("DIR_ADDR_CV2"),
		(nHit == 0) ? 310 : 301, ECS_INI_FILE);
	CString strAddr;
	strAddr.Format(_T("D%04d (%%DB%d)"), nDirAddr, nDirAddr * 2);
	// [LGLS 2026-09-15] 값도 보인다(사용자 지시). PLC 에 실제로 쓰는 문자는 설비 통신의 부호 설정(WCS_DB.INI [PLC] DIR_CODE,
	//   IN0 = 입고 '0'/출고 '1', IN1 = 반대)을 따르는데 Client 는 그 ini 를 모른다.
	//   설비 통신이 마지막으로 남긴 방향지시 로그("… 방향지시 출고('0') → D워드 310 [IN1]")의 [IN1] 표기로 부호를 읽는다.
	BOOL bIn1 = FALSE;
	{
		CString strSqlLog;
		// 최근 하루·설비 통신 줄만 본다 (전체 LIKE 는 89만 행에서 7초 - 확인창이 늦게 뜬다). 없으면 IN0 으로 표시.
		strSqlLog.Format(_T(" SELECT TOP 1 log_kor FROM wcs_log_pgr WHERE ins_dt >= DATEADD(day, -1, GETDATE()) AND pgr_nm = 'WCS_TASK_CV_COMM0' AND log_kor LIKE '%%방향지시%%D워드%%' ORDER BY ins_dt DESC "));
		int nCntLog = 0;
		CString strMsgLog;
		_RecordsetPtr pRsLog = pDoc->GetSelectQryRecordsetPtr_DLG(strSqlLog, nCntLog, strMsgLog);
		if (nCntLog > 0)
		{
			CRecordSetWrap* pRswLog = new CRecordSetWrap(pRsLog);
			pRswLog->MoveFirst();
			CString strLast = pRswLog->GetItem(_T("log_kor"));
			delete pRswLog;
			bIn1 = (strLast.Find(_T("[IN1]")) >= 0);
		}
	}
	BOOL bOutNext = !bOutNow;
	TCHAR chNow  = ((bOutNow  ? 1 : 0) ^ (bIn1 ? 1 : 0)) ? _T('1') : _T('0');
	TCHAR chNext = ((bOutNext ? 1 : 0) ^ (bIn1 ? 1 : 0)) ? _T('1') : _T('0');
	CString strVal;
	strVal.Format(_T("'%c' → '%c'%s"), chNow, chNext, bIn1 ? _T("  [IN1]") : _T(""));
	CString strAsk;
	strAsk.Format(_T("%s (%s %s)\n\n[%s] → [%s]\n%s : %s\n%s : %s\n\n%s"),
		(LPCTSTR)pDoc->GetMsgLangDef(_T("방향을 바꾸겠습니까?")), s_szName[nHit],
		(LPCTSTR)(pDoc->GetMsgLangDef(_T("트랙")) + _T(" ") + s_szTrack[nHit]),
		(LPCTSTR)strNow, (LPCTSTR)strNext,
		(LPCTSTR)pDoc->GetMsgLangDef(_T("PLC 주소")), (LPCTSTR)strAddr,
		(LPCTSTR)pDoc->GetMsgLangDef(_T("값")), (LPCTSTR)strVal,
		(LPCTSTR)pDoc->GetMsgLangDef(_T("수동 전환은 즉시 반영되고, 잠시 동안 자동 방향 정합이 멈춥니다.")));   // [LGLS 2026-09-17] DIRM 즉시 전환
	if (AfxMessageBox(strAsk, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)
		return TRUE;

	// IO_TASK RequestCvDirection / C/V 창 [H/S 배출] 과 같은 명령 규약. 대기 중인 다른 명령은 덮어쓰지 않는다.
	// [LGLS 2026-09-17] 'DIRM' = 수동(즉시) 전환. 설비 통신이 화물 보류 없이 바로 쓰고, IO_TASK 는 한동안 자동 정합을 멈춘다(사용자 지시).
	CString strParm = bOutNow ? _T("0") : _T("1");
	strSql.Format(_T(" UPDATE CV_DATA                                          \n")
		  _T("    SET CMD_RQ_ID    = 'DIRM'                            \n")
		  _T("      , CMD_RQ_PARM  = '%s'                              \n")
		  _T("      , CMD_RQ_YN    = 'Y'                               \n")
		  _T("      , WRITE_UPD_DT = ") + pDoc->SYSDATE + _T("           \n")
		  _T("  WHERE WH_TYP = '%s'                                    \n")
		  _T("    AND MC_NO  = '%s'                                    \n")
		  _T("    AND (CMD_RQ_YN <> 'Y' OR CMD_RQ_ID IN ('DIR','DIRM','DIRW'))  "),
		(LPCTSTR)strParm, (LPCTSTR)pDoc->m_WH_TYP, s_szTrack[nHit]);
	pDoc->BeginTrans_DLG();
	if (pDoc->ExcuteQueryString_DLG(strSql) != TRUE)
	{
		pDoc->RollbackTrans_DLG();
		AfxMessageBox(pDoc->GetMsgLangDef(_T("실패")));
		return TRUE;
	}
	CString strLog;
	// [LGLS 2026-09-17] 로그 컬럼(varchar 200)을 넘지 않게 짧게 - 길면 INSERT 실패 팝업이 겹쳐 떴다(사용자 보고).
	strLog.Format(_T("수동 방향 전환(즉시) 트랙 %s : %s -> %s, %s"), s_szTrack[nHit], (LPCTSTR)strNow, (LPCTSTR)strNext, (LPCTSTR)strAddr);
	pDoc->GetQueryInsertClientLog(_T("CEcsView"), _T(""), _T(""), _T(""), strLog);
	pDoc->CommitTrans_DLG();
	CLib::UiLog(_T("[DIR] request track=%s parm=%s"), s_szTrack[nHit], (LPCTSTR)strParm);
	AfxMessageBox(pDoc->GetMsgLangDef(_T("방향 전환을 요청했습니다. 잠시 뒤 화면에 반영됩니다.")));
	return TRUE;
}

void CEcsView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// 방향 칸이면 여기서 처리하고, 그 밖의 칸은 종전대로 클릭 두 번과 같게 둔다
	if (DirectionToggleByDblClk(point)) return;
	OnLButtonDown(nFlags, point);
}

void CEcsView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

//	pDoc->m_bMoveFlag = FALSE;
	//OnButtonUpSelectedLayout(pDoc, nFlags, point);
	pDoc->m_bMoveFlag = FALSE; //3

	if (m_tabLayout.GetSafeHwnd())
	{
		int nTemp = m_tabLayout.GetCurSel();
		if (nTemp < 0)
			return;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return;

		pEcsLayout->OnButtonUp(this, nFlags, point);
	}

	CFormView::OnLButtonUp(nFlags, point);
}

void CEcsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	//OnKeyDownSelectedLayout(pDoc, nChar, nRepCnt, nFlags);
	if (m_tabLayout.GetSafeHwnd())
	{
		int nTemp = m_tabLayout.GetCurSel();

		if (nTemp < 0)
			return;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return;

		pEcsLayout->OnKeyDown(this, nChar, nRepCnt, nFlags);
	}



	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}


LRESULT CEcsView::OnRefreshDialog(WPARAM wParam, LPARAM lParam)
{
	CEquipment::EN_KIND enEquipKind = CEquipment::EN_KIND(lParam);
	CEcsDoc* pDoc = GetDocument();
	if(pDoc == NULL)
	{
		return 0;
	}

	switch(enEquipKind)
	{
	case CEquipment::enCV:
		{

			if (!pDoc->Permission(_T("CCvSkinDlg"), SEL_YN))
			{
				AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
				return 0;
			}

			if (pDoc->m_pCvSkinDlg == NULL)
			{
				pDoc->m_pCvSkinDlg = new CCvSkinDlg(pDoc);
				pDoc->m_pCvSkinDlg->Create(IDD_SKIN_CV_CTRL1);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pCvSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				// [LGLS 2026-09-11] 화면 밖으로 나가지 않게 못 박는다(배율 걸린 PC 대비).
				CLib::ClampToWorkArea(::AfxGetApp()->GetMainWnd(), PosRect, Rect.Width(), Rect.Height());
				pDoc->m_pCvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);
			}
			::SetWindowPos(pDoc->m_pCvSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pCvSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pCvSkinDlg->m_hWnd, SW_SHOW);
			::SendMessage(pDoc->m_pCvSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, NULL, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
			break;
		}
	case CEquipment::enSC:
		{

			if (!pDoc->Permission(_T("CScSkinDlg"), SEL_YN))
			{
				AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
				return 0;
			}

			if (pDoc->m_pScSkinDlg == NULL)
			{
				pDoc->m_pScSkinDlg = new CScSkinDlg(pDoc);
				pDoc->m_pScSkinDlg->Create(IDD_SKIN_SC_CTRL);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pScSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				// [LGLS 2026-09-11] 화면 밖으로 나가지 않게 못 박는다(배율 걸린 PC 대비).
				CLib::ClampToWorkArea(::AfxGetApp()->GetMainWnd(), PosRect, Rect.Width(), Rect.Height());
				pDoc->m_pScSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

				if(pDoc->m_pScSkinDlg == NULL)
					break;
			}
			
			::SetWindowPos(pDoc->m_pScSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pScSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pScSkinDlg->m_hWnd, SW_SHOW);
			::SendMessage(pDoc->m_pScSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)NULL, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람

			// [LGLS 2026-09-11] ★break 가 없어 RTV 케이스로 흘러 들어갔다★
			//   증상(현장 1600x900) : 크레인을 누르면 대화상자가 안 뜨고 멈추며 ESC 로 풀림.
			//   RTV 케이스 첫 줄의 권한 검사가 실패하면 AfxMessageBox(모달)가 뜨는데,
			//   직전에 SC 대화상자를 TOPMOST 로 올려서 그 박스가 뒤에 가려 보이지 않았다.
			//   권한이 있는 계정이면 SC 를 눌렀는데 RTV 대화상자까지 같이 떴다.
			break;
		}
	case CEquipment::enRTV:
		{

			if (!pDoc->Permission(_T("CRTVSkinDlg"), SEL_YN))
			{
				AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
				return 0;
			}

			if (pDoc->m_pRtvSkinDlg == NULL)
			{
				pDoc->m_pRtvSkinDlg = new CRtvSkinDlg(pDoc);
				// [LGLS 2026-09-09] IDD_SKIN_RTV_CTRL 은 옛 템플릿이라 CRtvSkinDlg 의 DDX 대상 컨트롤이 없어
				//   dlgdata.cpp:40 어설션으로 죽었다. 클릭 경로(OnControlClick)와 같은 CTRL1 을 쓴다.
				pDoc->m_pRtvSkinDlg->Create(IDD_SKIN_RTV_CTRL1);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pRtvSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				// [LGLS 2026-09-11] 화면 밖으로 나가지 않게 못 박는다(배율 걸린 PC 대비).
				CLib::ClampToWorkArea(::AfxGetApp()->GetMainWnd(), PosRect, Rect.Width(), Rect.Height());
				pDoc->m_pRtvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);

				if(pDoc->m_pRtvSkinDlg == NULL)
					break;
			}
			
			::SetWindowPos(pDoc->m_pRtvSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pRtvSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pRtvSkinDlg->m_hWnd, SW_SHOW);
			::SendMessage(pDoc->m_pRtvSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)NULL, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람

			// [LGLS 2026-09-11] 마지막 케이스지만 다음에 케이스를 붙여도 안전하게 둔다.
			break;
		}
	}
	return 0;
}

LRESULT CEcsView::OnControlClick(WPARAM wParam, LPARAM lParam)
{
	CEcsDoc* pDoc = GetDocument();	
	if(pDoc == NULL)
	{	
		return FALSE;	
	}

	CString strCID;
	CString strMC_NO;
	strCID.Format(_T("%u"), wParam);
	if (CLib::IsValidControlID(strCID) == FALSE)
		return 0;

	//if (CLib::IsValidControlMCNO(strMC_NO) == FALSE)
	//	return 0;

	CString strEqpKey, strWH_TYP, strEN_EQP_TYP; 
	int nEN_EQP_TYP;
	int nIntTemp = 0;

	//strEqpKey = strCID.Right(5);
	strEqpKey = strCID.Right(3);
	//strEqpKey = strMC_NO;
	strWH_TYP = pDoc->m_WH_TYP;
	strEN_EQP_TYP = strCID.Mid(3, 2);
	nIntTemp = CConvert::ToInt(strEN_EQP_TYP);
	nEN_EQP_TYP = nIntTemp; //(nIntTemp / 10) * 10;


	switch (nEN_EQP_TYP)
	{
	case CEquipment::enCV:
		{			
			if (pDoc->m_strSeachTrack == strEqpKey)
			{
				CString strTrack;
				strTrack = pDoc->m_strSeachTrack;
				CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(strTrack);
				pTrackInfo->SetSeachColor(pTrackInfo->m_pTrackCtrl, true);
				pDoc->m_strSeachTrack = _T("");
			}

			CTrackInfo* pTrackInfo = pDoc->GetTrackInfoNew(strEqpKey);
			if(pTrackInfo == NULL){ return 0; };

			// [LGLS 2026-09-01] 판넬 모드 : 상세정보 판넬 CV 탭으로 전환(대화상자 대신)
			{
				CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
				if (pFrame != NULL && pFrame->m_bUiModePanel && pFrame->m_bPanelBarsCreated
					&& pFrame->m_InfoPane.IsVisible())
				{
					CString strMcNo = strEqpKey;
					if (pTrackInfo->m_pCV_DATA != NULL) strMcNo = pTrackInfo->m_pCV_DATA->V_MC_NO;
					pFrame->m_PanelInfoDlg.SetEquip(0 /*TAB_CV*/, strMcNo);
					// [LGLS 2026-09-10] 판넬만 바꾸고 끝내지 않는다.
					//   판넬 보기가 기본이 되면서 대화상자를 쓸 길이 막혔다는 지적.
					//   판넬은 그대로 따라가고, 아래로 내려가 대화상자도 띄운다.
				}
			}
			if (pDoc->m_pCvSkinDlg == NULL)
			{

				if (!pDoc->Permission(_T("CCvSkinDlg"), SEL_YN))
				{
					AfxMessageBox(pDoc->GetMsgLangDef(_T("권한이 없습니다")));
					return 0;
				}

				pDoc->m_pCvSkinDlg = new CCvSkinDlg(pDoc);
				pDoc->m_pCvSkinDlg->Create(IDD_SKIN_CV_CTRL1);


				//// [LGLS 2026-09-11] 화면 밖으로 나가지 않게 못 박는다(배율 걸린 PC 대비).
				//CLib::ClampToWorkArea(::AfxGetApp()->GetMainWnd(), PosRect, Rect.Width(), Rect.Height());
				//pDoc->m_pCvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
				//	Rect.Width(), Rect.Height(), 
				//	SWP_SHOWWINDOW);


			}

			CRect MainRect;
			CRect Rect;
			CRect PosRect;
			::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
			pDoc->m_pCvSkinDlg->GetWindowRect(&Rect); 

			PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
			PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 

			// [LGLS 2026-09-11] 화면 밖으로 나가지 않게 못 박는다(배율 걸린 PC 대비).
			CLib::ClampToWorkArea(::AfxGetApp()->GetMainWnd(), PosRect, Rect.Width(), Rect.Height());
			pDoc->m_pCvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
				Rect.Width(), Rect.Height(), 
				SWP_SHOWWINDOW);

			::SetWindowPos(pDoc->m_pCvSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pCvSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pCvSkinDlg->m_hWnd, SW_SHOWNORMAL);
			::SendMessage(pDoc->m_pCvSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)pTrackInfo, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람

			
			break;
		}

	case CEquipment::enSC:
		{
			// [LGLS 2026-09-11] 여기서부터 단계 기록. 현장에서 어디서 멎는지 가린다.
			CLib::UiEnvLog();
			CLib::UiLog(_T("[SC] CLICK cid=%s key=%s"), (LPCTSTR)strCID, (LPCTSTR)strEqpKey);
			CSC_DATA* pSC_DATA = pDoc->GetSC_DATA(strEqpKey);
			if(pSC_DATA == NULL)
			{
				// EcsDefine.xml <Scs number=..> 에 없는 키다. 조용히 끝나므로 화면엔
				// "눌렀는데 아무 일도 없다" 로만 보인다 - 반드시 남긴다.
				CLib::UiLog(_T("[SC] ABORT no SC_DATA (EcsDefine.xml <Scs number> mismatch) key=%s"),
					(LPCTSTR)strEqpKey);
				return 0;
			}

			// [LGLS 2026-09-01] 판넬 모드 : 상세정보 판넬 SC 탭으로 전환
			{
				CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
				if (pFrame != NULL && pFrame->m_bUiModePanel && pFrame->m_bPanelBarsCreated
					&& pFrame->m_InfoPane.IsVisible())
				{
					pFrame->m_PanelInfoDlg.SetEquip(1 /*TAB_SC*/, strEqpKey);
					// [LGLS 2026-09-10] 판넬만 바꾸고 끝내지 않는다.
					//   판넬 보기가 기본이 되면서 대화상자를 쓸 길이 막혔다는 지적.
					//   판넬은 그대로 따라가고, 아래로 내려가 대화상자도 띄운다.
				}
			}
			if (pDoc->m_pScSkinDlg == NULL)
			{
				if (!pDoc->Permission(_T("CScSkinDlg"), SEL_YN))
				{
					AfxMessageBox(pDoc->m_pLang->GetLangValue(_T("권한이 없습니다"), pDoc->m_enLang));
					return 0;
				}

				pDoc->m_pScSkinDlg = new CScSkinDlg(pDoc);
				pDoc->m_pScSkinDlg->Create(IDD_SKIN_SC_CTRL);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pScSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				// [LGLS 2026-09-11] 화면 밖으로 나가지 않게 못 박는다(배율 걸린 PC 대비).
				CLib::ClampToWorkArea(::AfxGetApp()->GetMainWnd(), PosRect, Rect.Width(), Rect.Height());
				pDoc->m_pScSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);
			}
			::SetWindowPos(pDoc->m_pScSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pScSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pScSkinDlg->m_hWnd, SW_SHOWNORMAL); 
			{
				// 창이 실제로 어디에 어떤 크기로 떴는지, 보이는 상태인지 그대로 남긴다.
				CRect rcNow(0,0,0,0);
				::GetWindowRect(pDoc->m_pScSkinDlg->m_hWnd, &rcNow);
				CLib::UiLog(_T("[SC] SHOW rect=(%d,%d)-(%d,%d) visible=%d"),
					rcNow.left, rcNow.top, rcNow.right, rcNow.bottom,
					(int)::IsWindowVisible(pDoc->m_pScSkinDlg->m_hWnd));
			}
			// ★여기가 핵심★ 아래 SendMessage 는 동기다. 그 안에서 DB 를 읽으므로,
			//   DB 가 네트워크 너머에 있으면 UI 스레드가 그만큼 통째로 멈춘다.
			//   개발 PC 는 DB 가 로컬이라 순식간이다 - 현장에서만 나타나는 이유가 될 수 있다.
			//   걸린 시간을 재서 남긴다.
			DWORD dwT0 = ::GetTickCount();
			CLib::UiLog(_T("[SC] REFRESH begin"));
			::SendMessage(pDoc->m_pScSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)pSC_DATA, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
			CLib::UiLog(_T("[SC] REFRESH end %u ms"), (unsigned)(::GetTickCount() - dwT0));
			break;
		}

 
 	case CEquipment::enRTV:
 		{
			CRTV_DATA* pRTV_DATA = pDoc->GetRTV_DATA(strEqpKey);
			if(pRTV_DATA == NULL){ return 0; };

			// [LGLS 2026-09-01] 판넬 모드 : 상세정보 판넬 RTV 탭으로 전환
			{
				CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
				if (pFrame != NULL && pFrame->m_bUiModePanel && pFrame->m_bPanelBarsCreated
					&& pFrame->m_InfoPane.IsVisible())
				{
					pFrame->m_PanelInfoDlg.SetEquip(2 /*TAB_RTV*/, strEqpKey);
					// [LGLS 2026-09-10] 판넬만 바꾸고 끝내지 않는다.
					//   판넬 보기가 기본이 되면서 대화상자를 쓸 길이 막혔다는 지적.
					//   판넬은 그대로 따라가고, 아래로 내려가 대화상자도 띄운다.
				}
			}
			if (pDoc->m_pRtvSkinDlg == NULL)
			{
				if (!pDoc->Permission(_T("CRtvSkinDlg"), SEL_YN))
				{
					AfxMessageBox(pDoc->m_pLang->GetLangValue(_T("권한이 없습니다"), pDoc->m_enLang));
					return 0;
				}

				pDoc->m_pRtvSkinDlg = new CRtvSkinDlg(pDoc);
				pDoc->m_pRtvSkinDlg->Create(IDD_SKIN_RTV_CTRL1);
				CRect MainRect;
				CRect Rect;
				CRect PosRect;
				::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
				pDoc->m_pRtvSkinDlg->GetWindowRect(&Rect); 

				PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
				PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
				// [LGLS 2026-09-11] 화면 밖으로 나가지 않게 못 박는다(배율 걸린 PC 대비).
				CLib::ClampToWorkArea(::AfxGetApp()->GetMainWnd(), PosRect, Rect.Width(), Rect.Height());
				pDoc->m_pRtvSkinDlg->SetWindowPos(&wndTop, PosRect.left, PosRect.top, 
					Rect.Width(), Rect.Height(), 
					SWP_SHOWWINDOW);
			}
			::SetWindowPos(pDoc->m_pRtvSkinDlg->m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(pDoc->m_pRtvSkinDlg->m_hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
			::ShowWindow(pDoc->m_pRtvSkinDlg->m_hWnd, SW_SHOWNORMAL); 
			::SendMessage(pDoc->m_pRtvSkinDlg->m_hWnd, WM_USER_DIALOG_MESSAGE_REFRESH, (WPARAM)pRTV_DATA, (LPARAM)pDoc->m_enLang); //임시저장소 LPARM 추가파람
			break;
		}
	}
	return 0;
		
}

LRESULT CEcsView::OnViewLayoutInfo(WPARAM wParam, LPARAM lParam)
{
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return 0;

	RECT rc;
	GetClientRect(&rc);
	SendMessage(WM_SIZE,(WPARAM)SIZE_RESTORED, MAKELPARAM(rc.right-rc.left, rc.bottom-rc.top));

	//pLayout->OnViewLayoutInfo(this);
	
	if (m_tabLayout.GetSafeHwnd())
	{
		int nTemp = m_tabLayout.GetCurSel();

		if (nTemp < 0)
			return 0;

		CEcsLayout* pEcsLayout = pDoc->m_pEcsLayOuts[nTemp];
		if (pEcsLayout == NULL)
			return 0;

		pEcsLayout->OnViewLayoutInfo(this);
	}

	pDoc->m_bViewFirstLoad = TRUE;
	return 0;
}

//=============================================================================
//	Desc	: '검색기능' 창을 화면 조정에 따라 우측 하단에 배치
//	Author	: YJY
//	Date	:
//	Update	: 
//=============================================================================
void CEcsView::MoveSearchWnd(int cx, int cy, BOOL bStatus)
{
	// 호출 시점 : 프로그램 시작 시, 윈도우 크기 변경 시, LayOut 1-3단 클릭 시
}

//=============================================================================
//	Desc	: '검색기능' 컨트롤 ID 와 위치 저장
//	Author	: YJY
//	Date	:
//	Update	: 
//=============================================================================
void CEcsView::CalcSerachControlPos()
{
	CEcsDoc *pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	int nTop = 0;
	int nLeft = 0;

}

BOOL CEcsView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CEcsDoc *pDoc = (CEcsDoc*)GetDocument();
	ASSERT(pDoc != NULL);

	if (pMsg->message == WM_KEYDOWN) 
	{
		PreTranslateMessageSelectedLayout(pDoc, pMsg->wParam, 0, 0);
	}

	return CFormView::PreTranslateMessage(pMsg);
}

void CEcsView::OnSearch()
{
	// TODO: Add your command handler code here
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

}


void CEcsView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CEcsDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	OnMouseMoveSelectedLayout(pDoc, nFlags, point);
	
	CFormView::OnMouseMove(nFlags, point);
}

BOOL CEcsView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBrush backBrush(RGB(255, 255, 255));               // 파랑색. 원하는 컬러를 너주면 된다...

	if(pDC == NULL)
		return TRUE;

	CBrush* pOldBrush = pDC->SelectObject(&backBrush); 
	CRect rect; pDC->GetClipBox(&rect); 
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush); 

	return TRUE;      

	return CFormView::OnEraseBkgnd(pDC);
}



void CEcsView::DrawSelectedLayout( CDC* pDC, CEcsDoc* pDoc )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnDraw(this, pDC, 0); 
}

void CEcsView::WheelSelectedLayout( CEcsDoc* pDoc, UINT nFlags, short zDelta, CPoint pt )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnMouseWheel(this, nFlags, zDelta, pt); 
}

void CEcsView::RButtonUpSelectedLayout( CEcsDoc* pDoc)
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnOriginalSize(this);
}

void CEcsView::OnSizeSelectedLayout(CEcsDoc* pDoc, UINT nType, int cx, int cy )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnSize(this, nType, cx, cy);
}

void CEcsView::OnLButtonDownSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnButtonDown(this, nFlags, point);

}

void CEcsView::OnButtonUpSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnButtonUp(this, nFlags, point);
}

void CEcsView::OnKeyDownSelectedLayout( CEcsDoc* pDoc, UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnKeyDown(this, nChar, nRepCnt, nFlags);
}

void CEcsView::PreTranslateMessageSelectedLayout( CEcsDoc * pDoc, WPARAM wParam, int param3, int param4 )
{
	CEcsLayout* pLayout = pDoc->GetSelectedLayout();
	if(pLayout == NULL)
		return;

	pLayout->OnKeyDown(this, wParam, 0, 0);
}

void CEcsView::OnMouseMoveSelectedLayout( CEcsDoc* pDoc, UINT nFlags, CPoint point )
{
}

void CEcsView::GetQrySelectStatusAll( CCV_DATA* pCV_DATA, CString& pSTOCK_MODE, CString& pREMOTE_CONTROL, CString& pROLL_MODE)
{
	CEcsDoc* pDoc = GetDocument();
	CString CRLF = _T("\r\n");
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	//strSql += CRLF + _T("SELECT (SELECT REMOTE_CONTROL FROM CV_DATA WHERE MC_NO = '101') AS REMOTE_CONTROL  ");
	strSql += CRLF + _T("SELECT TOP 1 (SELECT STOCK_MODE FROM CV_DATA WHERE MC_NO = '149') AS STOCK_MODE			");	// [LGLS] TOP 1 instead of LIMIT 1
	strSql += CRLF + _T("  FROM CV_DATA  ");
	strSql += CRLF + _T(" WHERE WH_TYP = ") + CLib::Quot(pCV_DATA->K_WH_TYP);
	strSql += CRLF + _T("   AND PLC_NO = ") + CLib::Quot(pCV_DATA->K_PLC_NO);	// [LGLS] LIMIT removed (TOP 1 above)
	
	_RecordsetPtr pRsptr = pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	pRsw->MoveFirst(); 


	pSTOCK_MODE = pRsw->GetItem(_T("STOCK_MODE"));
	pREMOTE_CONTROL = _T("");
	// [LGLS 2026-09-10] ROLL_MODE 컬럼은 DB 에서 지웠다(타 현장 설비용).
	pROLL_MODE = _T("");

	delete pRsw;
}

// [LGLS 2026-09-12] Ecs.ini 저장 자동 감지 (사용자 지시 : "ini 에 적용하면 바로 적용되게").
//   1초마다 파일 수정 시각을 본다. 바뀐 뒤 1초 더 그대로면(편집기 저장이 끝난 뒤) 재기동 없이 되는 키만 다시 읽는다.
//   프로그램 자신이 쓰는 값(LAST_LANG·LAST_TAB_INDEX·[설정 상태] 색 저장)도 시각을 바꾸지만, 대상 키가 그대로면 "no change" 로 끝난다.
void CEcsView::CheckIniHotReload()
{
	WIN32_FILE_ATTRIBUTE_DATA fad;
	ZeroMemory(&fad, sizeof(fad));
	if (!::GetFileAttributesEx(ECS_INI_FILE, GetFileExInfoStandard, &fad)) return;
	ULONGLONG t = ((ULONGLONG)fad.ftLastWriteTime.dwHighDateTime << 32) | fad.ftLastWriteTime.dwLowDateTime;
	if (m_ullIniWriteTime == 0)
	{
		// 기동 직후 : 기준 시각과 현재 ZOOM_BTN 값만 잡아 둔다(이때는 다시 읽지 않는다)
		m_ullIniWriteTime = t;
		m_nIniZoomBtn = (::GetPrivateProfileInt(_T("MENU"), _T("ZOOM_BTN"), 1, ECS_INI_FILE) != 0) ? 1 : 0;
		return;
	}
	if (t == m_ullIniWriteTime) { m_ullIniPendingTime = 0; return; }
	if (t != m_ullIniPendingTime) { m_ullIniPendingTime = t; return; }	// 방금 바뀜 - 1초 뒤 한 번 더 확인
	m_ullIniWriteTime = t;
	m_ullIniPendingTime = 0;
	ReloadIniHot();
}

// [LGLS 2026-09-12] 재기동 없이 되는 키만 다시 읽는다. 바뀐 키는 UI 로그(LOG\ECS_UI_yyyymmdd.log)에 [INI] reload 로 남긴다.
//   · [ETC] ViewRetCnt        : CConfig 값만 바꾸면 트랙 그리기(TrackInfo)가 다음 갱신에 반영
//   · [MENU] UI_TRACE/LOADBIT_GATE : CLib 캐시 비우기
//   · [MENU] ZOOM_BTN         : 이미 만들어진 CV/SC/RTV 상태창의 [확대] 버튼 표시/숨김(숨기면 펼친 패널도 접음)
//   · [USER] JOB_STALL_WARN_SEC : 체류 경고창 기준 초
//   (USER_COLOR_* 27개는 사용자 지시로 제외)
void CEcsView::ReloadIniHot()
{
	CEcsDoc* pDoc = GetDocument();
	if (pDoc == NULL) return;
	CString strChg = _T("");

	if (pDoc->m_pConfig != NULL)
	{
		int nOld = pDoc->m_pConfig->m_nETC_ViewRetCnt;
		int nNew = ::GetPrivateProfileInt(_T("ETC"), _T("ViewRetCnt"), 0, ECS_INI_FILE);
		if (nOld != nNew)
		{
			pDoc->m_pConfig->m_nETC_ViewRetCnt = nNew;
			strChg.AppendFormat(_T(" ViewRetCnt=%d"), nNew);
		}
	}

	{
		int nTr0 = CLib::IniUiTrace(), nGt0 = CLib::IniLoadBitGate(), nVc0 = CLib::IniVehClearMode();
		CLib::IniCacheReset();
		int nTr1 = CLib::IniUiTrace(), nGt1 = CLib::IniLoadBitGate(), nVc1 = CLib::IniVehClearMode();
		if (nTr0 != nTr1) strChg.AppendFormat(_T(" UI_TRACE=%d"), nTr1);
		if (nGt0 != nGt1) { strChg.AppendFormat(_T(" LOADBIT_GATE=%d"), nGt1); Invalidate(FALSE); }
		// [LGLS 2026-09-14] 크레인·RTV 색 지우는 시점 (0 종전 / 1 적재 비트 / 2 H/S 기록). 다음 수집 주기에 다시 칠해진다
		if (nVc0 != nVc1) { strChg.AppendFormat(_T(" VEH_CLEAR_MODE=%d"), nVc1); Invalidate(FALSE); }
	}

	{
		int nZoom = (::GetPrivateProfileInt(_T("MENU"), _T("ZOOM_BTN"), 1, ECS_INI_FILE) != 0) ? 1 : 0;
		if (nZoom != m_nIniZoomBtn)
		{
			m_nIniZoomBtn = nZoom;
			strChg.AppendFormat(_T(" ZOOM_BTN=%d"), nZoom);
			if (pDoc->m_pCvSkinDlg  != NULL && ::IsWindow(pDoc->m_pCvSkinDlg->m_hWnd))  ((CCvSkinDlg*)pDoc->m_pCvSkinDlg)->ApplyZoomBtnIni();
			if (pDoc->m_pScSkinDlg  != NULL && ::IsWindow(pDoc->m_pScSkinDlg->m_hWnd))  ((CScSkinDlg*)pDoc->m_pScSkinDlg)->ApplyZoomBtnIni();
			if (pDoc->m_pRtvSkinDlg != NULL && ::IsWindow(pDoc->m_pRtvSkinDlg->m_hWnd)) ((CRtvSkinDlg*)pDoc->m_pRtvSkinDlg)->ApplyZoomBtnIni();
		}
	}

	if (pDoc->m_pWarningDlg != NULL)
	{
		CWarningDlg* pWarn = (CWarningDlg*)pDoc->m_pWarningDlg;
		int nOld = pWarn->m_nStallSec;
		pWarn->ReloadIni();
		if (nOld != pWarn->m_nStallSec) strChg.AppendFormat(_T(" JOB_STALL_WARN_SEC=%d"), pWarn->m_nStallSec);
	}

	// [Title] BuildDate/DbInfo/Path · [RibbonMenu] ToolTip → 메인 프레임(제목줄·리본 툴팁)
	{
		CMainFrame* pFrm = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		if (pFrm != NULL) strChg += pFrm->ReloadTitleAndTipIni();
	}

	// 형식 문자열은 ASCII 만(MBCS 빌드에서 UTF-8 소스의 한글은 FormatV 에서 깨진다)
	CLib::UiLog(_T("[INI] reload%s"), strChg.IsEmpty() ? _T(" (no change in hot keys)") : (LPCTSTR)strChg);
}
