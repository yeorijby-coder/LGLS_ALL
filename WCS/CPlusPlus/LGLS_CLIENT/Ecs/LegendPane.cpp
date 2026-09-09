#include "stdafx.h"
#include "Ecs.h"
#include "LegendPane.h"
#include "EcsDoc.h"
#include "Config.h"
#include "Lib.h"
#include "Lang.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------
// 범례 항목표
//   nKind 0 = 그룹 제목(색 없음), 1 = 색상 항목
//   szKey = rc_resource\legend\legend.ini 의 키, szKor = ini 가 없을 때 쓰는 값
//   nColor = GetItemColor() 의 switch 번호 (CConfig 멤버와 1:1)
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

	{ 0,  0, _T("grp_rail"),	_T("S/C 레일")		},
	{ 1, 20, _T("rail_sto"),	_T("입고정지")		},
	{ 1, 21, _T("rail_ret"),	_T("출고정지")		},
	{ 1, 22, _T("rail_all"),	_T("입출고정지")	},
	{ 1, 23, _T("rail_err"),	_T("에러")			},
	{ 1, 24, _T("rail_invk"),	_T("작업 중")		},
};

static const int LEGEND_ROW_CNT = sizeof(g_arrLegend) / sizeof(g_arrLegend[0]);

#define LEGEND_PAD			4
#define LEGEND_TITLE_H		20
#define LEGEND_GROUP_H		17
#define LEGEND_ITEM_H		15
#define LEGEND_SWATCH_W		18
#define LEGEND_SWATCH_H		10

IMPLEMENT_DYNAMIC(CLegendPane, CWnd)

BEGIN_MESSAGE_MAP(CLegendPane, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

CLegendPane::CLegendPane()
{
	m_pDoc = NULL;
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

int CLegendPane::GetWantHeight() const
{
	int nH = LEGEND_PAD * 2 + LEGEND_TITLE_H;
	for (int i = 0; i < LEGEND_ROW_CNT; i++)
		nH += (g_arrLegend[i].nKind == 0) ? LEGEND_GROUP_H : LEGEND_ITEM_H;
	return nH;
}

void CLegendPane::Reload()
{
	if (GetSafeHwnd() != NULL)
		Invalidate(TRUE);
}

COLORREF CLegendPane::GetItemColor(int nIdx) const
{
	if (m_pDoc == NULL || m_pDoc->m_pConfig == NULL)
		return RGB(200, 200, 200);

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
	return RGB(200, 200, 200);
}

CString CLegendPane::GetItemText(int nIdx) const
{
	// 다국어 : rc_resource\legend\legend.ini 우선, 없으면 내장 한글
	int nLang = (m_pDoc == NULL) ? (int)EN_KOR : (int)m_pDoc->m_enLang;
	CString strValue = CLib::GetIniStringFromPath(GetIniPath(), g_arrLegend[nIdx].szKey, nLang);
	if (strValue.IsEmpty())
		strValue = g_arrLegend[nIdx].szKor;
	return strValue;
}

BOOL CLegendPane::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;		// OnPaint 에서 전부 그린다 (깜빡임 방지)
}

void CLegendPane::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	// 더블버퍼
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

	int nLang = (m_pDoc == NULL) ? (int)EN_KOR : (int)m_pDoc->m_enLang;
	CString strTitle = CLib::GetIniStringFromPath(GetIniPath(), _T("title"), nLang);
	if (strTitle.IsEmpty()) strTitle = _T("범례");

	CRect rcTitle(rcClient.left + LEGEND_PAD, nY, rcClient.right - LEGEND_PAD, nY + LEGEND_TITLE_H);
	memDC.DrawText(strTitle, rcTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDC.SelectObject(&penBorder);
	memDC.MoveTo(rcClient.left + LEGEND_PAD, rcTitle.bottom - 2);
	memDC.LineTo(rcClient.right - LEGEND_PAD, rcTitle.bottom - 2);
	nY = rcTitle.bottom;

	for (int i = 0; i < LEGEND_ROW_CNT; i++)
	{
		if (nY > rcClient.bottom) break;		// 창이 작으면 잘라 그린다

		CString strText = GetItemText(i);

		if (g_arrLegend[i].nKind == 0)
		{
			// 그룹 제목
			memDC.SelectObject(&m_fntTitle);
			memDC.SetTextColor(RGB(70, 70, 70));
			CRect rcGrp(rcClient.left + LEGEND_PAD, nY, rcClient.right - LEGEND_PAD, nY + LEGEND_GROUP_H);
			memDC.DrawText(strText, rcGrp, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			memDC.SelectObject(&penBorder);
			memDC.MoveTo(rcClient.left + LEGEND_PAD, rcGrp.bottom - 1);
			memDC.LineTo(rcClient.right - LEGEND_PAD, rcGrp.bottom - 1);
			nY = rcGrp.bottom;
		}
		else
		{
			// 색 견본 + 문구
			int nSwTop = nY + (LEGEND_ITEM_H - LEGEND_SWATCH_H) / 2;
			CRect rcSw(rcClient.left + LEGEND_PAD + 2, nSwTop,
					   rcClient.left + LEGEND_PAD + 2 + LEGEND_SWATCH_W, nSwTop + LEGEND_SWATCH_H);

			memDC.FillSolidRect(rcSw, GetItemColor(g_arrLegend[i].nColor));
			memDC.SelectObject(&penSwatch);
			memDC.SelectStockObject(NULL_BRUSH);
			memDC.Rectangle(rcSw);

			memDC.SelectObject(&m_fntItem);
			memDC.SetTextColor(RGB(30, 30, 30));
			CRect rcTx(rcSw.right + 4, nY, rcClient.right - 3, nY + LEGEND_ITEM_H);
			memDC.DrawText(strText, rcTx, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

			nY += LEGEND_ITEM_H;
		}
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
