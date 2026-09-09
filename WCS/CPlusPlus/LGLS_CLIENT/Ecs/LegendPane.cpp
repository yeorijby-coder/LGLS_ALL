#include "stdafx.h"
#include "Ecs.h"
#include "LegendPane.h"
#include "EcsDoc.h"
#include "Config.h"
#include "Color.h"
#include "Lib.h"
#include "Lang.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------
// 범례 항목표
//   nKind 0 = 그룹 제목            (색 없음)
//         1 = 색 견본 사각형       (C/V 트랙 계열 - 트랙 칸이 그 색으로 칠해진다)
//         2 = rv 컨트롤 아이콘 : 레일 색  (S/C·RTV 의 레일이 그 색이 된다)
//         3 = rv 컨트롤 아이콘 : 포크 색  (크레인/RTV 본체가 그 색이 된다)
//         4 = 설명 문구           (견본 없음)
//   szKey = rc_resource\legend\legend.ini 의 키, szKor = ini 가 없을 때 쓰는 값
//   nColor = GetItemColor() 의 switch 번호 (CConfig 멤버와 1:1, 90~ 는 고정색)
//
//   [LGLS 2026-09-09] 크레인/RTV 는 CDciRvCtrl(레일+바퀴+포크)로 그려지므로
//   범례도 단색 칩이 아니라 같은 모양으로 보여 준다(사용자 요청).
//   레일 색 = GetRailColor(), 포크 색 = GetForkColor1() 이 내는 값이다.
// ---------------------------------------------------------------------------
struct ST_LEGEND_ROW
{
	int			nKind;
	int			nColor;
	LPCTSTR		szKey;
	LPCTSTR		szKor;
};

static const ST_LEGEND_ROW g_arrLegend[] =
{
	{ 0,  0, _T("grp_cv_job"),	_T("C/V 작업")		},
	{ 1,  1, _T("sto"),			_T("입고")			},
	{ 1,  2, _T("ret"),			_T("출고")			},
	{ 1,  3, _T("move"),		_T("이동")			},
	{ 1,  4, _T("rtr"),			_T("되돌림")		},
	{ 1,  5, _T("ata"),			_T("호기간 이동")	},

	{ 0,  0, _T("grp_cv_stat"),	_T("C/V 상태")		},
	{ 1,  6, _T("sto_ready"),	_T("입고대기")		},
	{ 1,  7, _T("ret_ready"),	_T("출고대기")		},
	{ 1,  8, _T("sto_hs"),		_T("입고 HS")		},
	{ 1,  9, _T("ret_hs"),		_T("출고 HS")		},
	{ 1, 10, _T("suspend"),		_T("일시정지")		},

	{ 0,  0, _T("grp_etc"),		_T("상태")			},
	{ 1, 11, _T("error"),		_T("에러")			},
	{ 1, 12, _T("manual"),		_T("수동/R/Stop")	},
	{ 1, 13, _T("disconnect"),	_T("링크단절")		},
	{ 1, 14, _T("search"),		_T("검색")			},

	{ 0,  0, _T("grp_semi"),	_T("반자동 작업")	},
	{ 1, 15, _T("semi_sto"),	_T("반자동입고")	},
	{ 1, 16, _T("semi_ret"),	_T("반자동출고")	},
	{ 1, 17, _T("semi_move"),	_T("반자동이동")	},
	{ 1, 18, _T("semi_rtr"),	_T("반자동되돌림")	},
	{ 1, 19, _T("semi_ata"),	_T("반자동호기간")	},

	// 크레인(S/C) / RTV : 포크(본체) 색
	{ 0,  0, _T("grp_veh"),		_T("크레인 / RTV")	},
	{ 3, 90, _T("veh_idle"),	_T("작업 없음")		},
	{ 3, 91, _T("veh_off"),		_T("미가동/수동")	},
	{ 3, 11, _T("veh_err"),		_T("에러")			},
	{ 4,  0, _T("veh_note"),	_T("작업 중엔 포크가 작업색") },

	// 크레인(S/C) / RTV : 레일 색
	{ 0,  0, _T("grp_rail"),	_T("S/C·RTV 레일")	},
	{ 2, 20, _T("rail_sto"),	_T("입고정지")		},
	{ 2, 21, _T("rail_ret"),	_T("출고정지")		},
	{ 2, 22, _T("rail_all"),	_T("입출고정지")	},
	{ 2, 23, _T("rail_err"),	_T("에러")			},
	{ 2, 24, _T("rail_invk"),	_T("작업 중")		},
};

static const int LEGEND_ROW_CNT = sizeof(g_arrLegend) / sizeof(g_arrLegend[0]);

#define LEGEND_PAD			4
#define LEGEND_TITLE_H		20
#define LEGEND_GROUP_H		17
#define LEGEND_ITEM_H		15
#define LEGEND_RV_H			17		// rv 아이콘 줄은 조금 높게
#define LEGEND_NOTE_H		30
#define LEGEND_SWATCH_W		18
#define LEGEND_SWATCH_H		10
#define LEGEND_RV_W			28		// rv 아이콘 폭
#define LEGEND_RV_ICON_H	13

IMPLEMENT_DYNAMIC(CLegendPane, CWnd)

BEGIN_MESSAGE_MAP(CLegendPane, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

CLegendPane::CLegendPane()
{
	m_pDoc = NULL;
	m_nTextLang = -1;
}

CLegendPane::~CLegendPane()
{
}

CString CLegendPane::GetIniPath()
{
	TCHAR chrFileName[MAX_PATH] = {0};
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	strAppPath.Format(_T("%s"), chrFileName);
	return strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\legend\\legend.ini");
}

BOOL CLegendPane::CreatePane(CWnd* pParent, UINT nID)
{
	if (pParent == NULL)
		return FALSE;

	LPCTSTR szClass = AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW),
										  (HBRUSH)::GetStockObject(WHITE_BRUSH), NULL);

	if (!CWnd::CreateEx(0, szClass, _T(""), WS_CHILD | WS_CLIPSIBLINGS,
						CRect(0, 0, PANE_WIDTH, 100), pParent, nID))
		return FALSE;

	m_fntItem.CreateFont(11, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
						 HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						 CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("맑은 고딕"));
	m_fntTitle.CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
						  HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						  CLEARTYPE_QUALITY, DEFAULT_PITCH, _T("맑은 고딕"));
	return TRUE;
}

static int RowHeight(int nKind)
{
	switch (nKind)
	{
	case 0:  return LEGEND_GROUP_H;
	case 2:
	case 3:  return LEGEND_RV_H;
	case 4:  return LEGEND_NOTE_H;
	default: return LEGEND_ITEM_H;
	}
}

int CLegendPane::GetWantHeight() const
{
	int nH = LEGEND_PAD * 2 + LEGEND_TITLE_H;
	for (int i = 0; i < LEGEND_ROW_CNT; i++)
		nH += RowHeight(g_arrLegend[i].nKind);
	return nH;
}

void CLegendPane::Reload()
{
	m_nTextLang = -1;			// 문구 캐시 무효화 (언어 전환 대비)
	if (GetSafeHwnd() != NULL)
		Invalidate(TRUE);
}

COLORREF CLegendPane::GetItemColor(int nIdx) const
{
	// 90~ : 설비 코드에 박혀 있는 고정색 (ScInfo/RtvInfo 의 GetForkColor1 참조)
	if (nIdx == 90) return LIGHT_GRAY;			// 작업 없음
	if (nIdx == 91) return DARK_GRAY;			// 미가동/수동(온라인·자동·액티브 아님)

	if (m_pDoc == NULL || m_pDoc->m_pConfig == NULL)
		return LIGHT_GRAY;

	CConfig* p = m_pDoc->m_pConfig;
	switch (nIdx)
	{
	case  1: return p->m_clrUSER_COLOR_STO;
	case  2: return p->m_clrUSER_COLOR_RET;
	case  3: return p->m_clrUSER_COLOR_MOVE;
	case  4: return p->m_clrUSER_COLOR_RTR;
	case  5: return p->m_clrUSER_COLOR_ATA;
	case  6: return p->m_clrUSER_COLOR_STN_STO;
	case  7: return p->m_clrUSER_COLOR_STN_RET;
	case  8: return p->m_clrUSER_COLOR_HS_STO;
	case  9: return p->m_clrUSER_COLOR_HS_RET;
	case 10: return p->m_clrUSER_COLOR_SUSPEND;
	case 11: return p->m_clrUSER_COLOR_ERROR;
	case 12: return p->m_clrUSER_COLOR_MANUAL;
	case 13: return p->m_clrUSER_COLOR_DISCONNECT;
	case 14: return p->m_clrUSER_COLOR_CV_SEARCH;
	case 15: return p->m_clrUSER_COLOR_SEMI_STO;
	case 16: return p->m_clrUSER_COLOR_SEMI_RET;
	case 17: return p->m_clrUSER_COLOR_SEMI_MOVE;
	case 18: return p->m_clrUSER_COLOR_SEMI_RTR;
	case 19: return p->m_clrUSER_COLOR_SEMI_ATA;
	case 20: return p->m_clrUSER_COLOR_STO_SUSPEND;
	case 21: return p->m_clrUSER_COLOR_RET_SUSPEND;
	case 22: return p->m_clrUSER_COLOR_ALL_SUSPEND;
	case 23: return p->m_clrUSER_COLOR_RAIL_ERROR;
	case 24: return p->m_clrUSER_COLOR_SC_INVK;
	default: break;
	}
	return LIGHT_GRAY;
}

// 다국어 문구는 그릴 때마다 ini 를 읽으면 한 번 그리는 데 파일을 30여 번 여는 셈이라
// 언어가 바뀔 때만 한 번 읽어 캐시한다.
void CLegendPane::EnsureText() const
{
	int nLang = (m_pDoc == NULL) ? (int)EN_KOR : (int)m_pDoc->m_enLang;
	if (m_nTextLang == nLang && m_arrText.GetSize() == LEGEND_ROW_CNT + 1)
		return;

	CString strPath = GetIniPath();
	m_arrText.RemoveAll();
	m_arrText.SetSize(LEGEND_ROW_CNT + 1);

	CString strTitle = CLib::GetIniStringFromPath(strPath, _T("title"), nLang);
	if (strTitle.IsEmpty()) strTitle = _T("범례");
	m_arrText[0] = strTitle;

	for (int i = 0; i < LEGEND_ROW_CNT; i++)
	{
		CString strValue = CLib::GetIniStringFromPath(strPath, g_arrLegend[i].szKey, nLang);
		if (strValue.IsEmpty()) strValue = g_arrLegend[i].szKor;
		m_arrText[i + 1] = strValue;
	}
	m_nTextLang = nLang;
}

CString CLegendPane::GetItemText(int nIdx) const
{
	EnsureText();
	return m_arrText[nIdx + 1];
}

BOOL CLegendPane::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;		// OnPaint 에서 전부 그린다 (깜빡임 방지)
}

// ---------------------------------------------------------------------------
// rv 컨트롤(크레인/RTV) 축소 아이콘
//   CDciRvCtrl::UpdateControl 과 같은 구성 : 레일 선 + 양 끝 바퀴 + 가운데 포크.
//   포크는 CDciMaster::DrawButton 과 같이 채운 뒤 3D 테두리를 두른다.
// ---------------------------------------------------------------------------
void CLegendPane::DrawRvIcon(CDC* pDC, const CRect& rcCell, COLORREF clrRail, COLORREF clrFork)
{
	int nCy = rcCell.top + rcCell.Height() / 2;

	// 레일
	CPen penRail(PS_SOLID, 1, clrRail);
	CPen* pOldPen = pDC->SelectObject(&penRail);
	pDC->MoveTo(rcCell.left + 3, nCy);
	pDC->LineTo(rcCell.right - 3, nCy);

	// 양 끝 바퀴
	CBrush brRail(clrRail);
	CBrush* pOldBrush = pDC->SelectObject(&brRail);
	pDC->Ellipse(rcCell.left,     nCy - 3, rcCell.left + 6, nCy + 3);
	pDC->Ellipse(rcCell.right - 6, nCy - 3, rcCell.right,   nCy + 3);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	// 포크(본체)
	int nCx = rcCell.left + rcCell.Width() / 2;
	CRect rcFork(nCx - 6, rcCell.top + 1, nCx + 6, rcCell.bottom - 1);
	pDC->FillSolidRect(rcFork, clrFork);
	pDC->Draw3dRect(rcFork, RGB(255, 255, 255), RGB(0, 0, 0));
}

void CLegendPane::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	CBitmap bmp, *pOldBmp;
	memDC.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	pOldBmp = memDC.SelectObject(&bmp);

	memDC.FillSolidRect(rcClient, RGB(252, 252, 252));

	CPen penBorder(PS_SOLID, 1, RGB(160, 160, 160));
	CPen penSwatch(PS_SOLID, 1, RGB(90, 90, 90));
	CPen* pOldPen = memDC.SelectObject(&penBorder);
	memDC.SelectStockObject(NULL_BRUSH);
	memDC.Rectangle(rcClient);

	memDC.SetBkMode(TRANSPARENT);

	int nY = rcClient.top + LEGEND_PAD;

	// 제목
	CFont* pOldFont = memDC.SelectObject(&m_fntTitle);
	memDC.SetTextColor(RGB(0, 0, 128));

	EnsureText();
	CRect rcTitle(rcClient.left + LEGEND_PAD, nY, rcClient.right - LEGEND_PAD, nY + LEGEND_TITLE_H);
	memDC.DrawText(m_arrText[0], rcTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDC.SelectObject(&penBorder);
	memDC.MoveTo(rcClient.left + LEGEND_PAD, rcTitle.bottom - 2);
	memDC.LineTo(rcClient.right - LEGEND_PAD, rcTitle.bottom - 2);
	nY = rcTitle.bottom;

	for (int i = 0; i < LEGEND_ROW_CNT; i++)
	{
		if (nY > rcClient.bottom) break;		// 창이 작으면 잘라 그린다

		CString strText = GetItemText(i);
		int nH = RowHeight(g_arrLegend[i].nKind);

		switch (g_arrLegend[i].nKind)
		{
		case 0:		// 그룹 제목
			{
				memDC.SelectObject(&m_fntTitle);
				memDC.SetTextColor(RGB(70, 70, 70));
				CRect rcGrp(rcClient.left + LEGEND_PAD, nY, rcClient.right - LEGEND_PAD, nY + nH);
				memDC.DrawText(strText, rcGrp, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				memDC.SelectObject(&penBorder);
				memDC.MoveTo(rcClient.left + LEGEND_PAD, rcGrp.bottom - 1);
				memDC.LineTo(rcClient.right - LEGEND_PAD, rcGrp.bottom - 1);
			}
			break;

		case 4:		// 설명 문구
			{
				memDC.SelectObject(&m_fntItem);
				memDC.SetTextColor(RGB(120, 120, 120));
				CRect rcNote(rcClient.left + LEGEND_PAD + 2, nY, rcClient.right - 3, nY + nH);
				memDC.DrawText(strText, rcNote, DT_LEFT | DT_TOP | DT_WORDBREAK);
			}
			break;

		case 2:		// rv 아이콘 - 레일 색
		case 3:		// rv 아이콘 - 포크 색
			{
				int nTop = nY + (nH - LEGEND_RV_ICON_H) / 2;
				CRect rcCell(rcClient.left + LEGEND_PAD, nTop,
							 rcClient.left + LEGEND_PAD + LEGEND_RV_W, nTop + LEGEND_RV_ICON_H);

				COLORREF clr = GetItemColor(g_arrLegend[i].nColor);
				if (g_arrLegend[i].nKind == 2)
					DrawRvIcon(&memDC, rcCell, clr, LIGHT_GRAY);		// 레일이 그 색
				else
					DrawRvIcon(&memDC, rcCell, DARK_GRAY, clr);			// 포크가 그 색

				memDC.SelectObject(&m_fntItem);
				memDC.SetTextColor(RGB(30, 30, 30));
				CRect rcTx(rcCell.right + 4, nY, rcClient.right - 3, nY + nH);
				memDC.DrawText(strText, rcTx, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			}
			break;

		default:	// 색 견본 사각형 (C/V 트랙 계열)
			{
				int nSwTop = nY + (nH - LEGEND_SWATCH_H) / 2;
				CRect rcSw(rcClient.left + LEGEND_PAD + 5, nSwTop,
						   rcClient.left + LEGEND_PAD + 5 + LEGEND_SWATCH_W, nSwTop + LEGEND_SWATCH_H);

				memDC.FillSolidRect(rcSw, GetItemColor(g_arrLegend[i].nColor));
				memDC.SelectObject(&penSwatch);
				memDC.SelectStockObject(NULL_BRUSH);
				memDC.Rectangle(rcSw);

				memDC.SelectObject(&m_fntItem);
				memDC.SetTextColor(RGB(30, 30, 30));
				CRect rcTx(rcClient.left + LEGEND_PAD + LEGEND_RV_W + 4, nY, rcClient.right - 3, nY + nH);
				memDC.DrawText(strText, rcTx, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			}
			break;
		}

		nY += nH;
	}

	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldPen);

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

void CLegendPane::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// 더블클릭 = 기존 범례(색상 설정) 대화상자 열기
	CWnd* pFrame = AfxGetMainWnd();
	if (pFrame != NULL)
		pFrame->PostMessage(WM_COMMAND, MAKEWPARAM(IDD_CONFIG_STATUS, 0), 0);

	CWnd::OnLButtonDblClk(nFlags, point);
}
