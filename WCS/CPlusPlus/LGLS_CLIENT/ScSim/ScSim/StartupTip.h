

#pragma once


typedef struct TextUnit
{
	TCHAR	Text[100];
} TextUnit;

class CStartupTip : public CStatic
{

public:
	CStartupTip();
	virtual ~CStartupTip();

public:
	int		m_nTipLen;
	int		m_nLines;
	struct	TextUnit Tips[10];

	HICON	m_hIcon;

	int		m_nVsize;
	int		m_nVspace;
	int		m_nLeftPane;

	COLORREF m_colorLeftPane;
	COLORREF m_colorBack;

	CFont *m_pfontOld;

public:
	CString m_strTitle;
	CString m_strHeader;
	CString m_strContent;
	CStringList m_listHead;

	CFont m_fontTitle;
	CFont m_fontHead;
	CFont m_fontContent;

	COLORREF m_colorTitle;
	COLORREF m_colorHead;
	COLORREF m_colorContent;

	int		m_nOffY;

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartupTip)
	//}}AFX_VIRTUAL

public:
	void SetMaxLen(int n);
	void SetTipText(LPCTSTR lpText);
	void SetTipText(CString strHead, CString strContent);

	void ResetTipText();

	void SetIcon(UINT n);
	void SetLeftPaneColor(COLORREF color);
	void SetLeftPaneSize(int c);
	void SetBgColor(COLORREF color);

protected:
	void DrawLeftPane(CPaintDC *pDC);
	void DrawTopTitle(CPaintDC *pDC);
	void DrawSubTitle(CPaintDC *pDC);
	void DrawContent(CPaintDC *pDC);
	void DrawMultiLine(CPaintDC *pDC);

	int	 GetWord(LPCTSTR in_str, LPTSTR lpWord);

public:
	//{{AFX_MSG(CStartupTip)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

