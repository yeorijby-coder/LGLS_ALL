#pragma once

// ---------------------------------------------------------------------------
// [LGLS 2026-09-09] CLegendPane : 메인 화면 왼쪽 범례 패널
//
//   기존에는 범례(색상 정의)를 리본 [환경설정] > [범례] 버튼으로 여는 대화상자
//   (IDD_CONFIG_STATUS)에서만 볼 수 있었다. 운전 중에는 그 창을 계속 띄워 둘 수
//   없어 색의 의미를 외워야 했다 -> 메인 화면 왼쪽 여백에 항상 보이는 읽기 전용
//   범례를 둔다.
//
//   - 색은 CConfig(m_clrUSER_COLOR_*) 를 그대로 읽는다. 범례 대화상자에서 색을
//     바꿔 [저장] 하면 Reload() 로 즉시 반영된다.
//   - 크레인(S/C)/RTV 는 도면에서 CDciRvCtrl(레일+바퀴+포크)로 그려지므로 범례도
//     같은 모양의 축소 아이콘으로 보여 준다. 레일 색/포크 색을 따로 표시한다.
//   - 문구는 rc_resource\legend\legend.ini 의 [kor]/[eng]/[hun]/[chin] 에서 읽는다
//     (다국어 필수 요건). ini 가 없거나 값이 비면 내장 한글 문구로 대체한다.
//     매번 읽으면 한 번 그리는 데 파일을 30여 번 여는 셈이라 언어별로 캐시한다.
//   - 더블클릭하면 기존 범례(색상 설정) 대화상자가 열린다.
// ---------------------------------------------------------------------------

class CEcsDoc;

class CLegendPane : public CWnd
{
	DECLARE_DYNAMIC(CLegendPane)

public:
	CLegendPane();
	virtual ~CLegendPane();

	// 패널 폭(고정). 높이는 항목 수로 계산한다.
	enum { PANE_WIDTH = 116 };

	BOOL CreatePane(CWnd* pParent, UINT nID);
	void SetDoc(CEcsDoc* pDoc) { m_pDoc = pDoc; }

	// 언어/색상이 바뀌었을 때 다시 읽어 그린다.
	void Reload();

	// 내용을 다 그리는 데 필요한 높이
	int  GetWantHeight() const;

protected:
	CEcsDoc*	m_pDoc;
	CFont		m_fntItem;
	CFont		m_fntTitle;

	// 다국어 문구 캐시 (index 0 = 제목, 1.. = 항목)
	mutable CStringArray	m_arrText;
	mutable int				m_nTextLang;

	static CString GetIniPath();
	static void DrawRvIcon(CDC* pDC, const CRect& rcCell, COLORREF clrRail, COLORREF clrFork);

	void     EnsureText() const;
	COLORREF GetItemColor(int nIdx) const;
	CString  GetItemText(int nIdx) const;

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};
