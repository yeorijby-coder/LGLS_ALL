#ifndef __CLABEL_H__
#define __CLABEL_H__

///////////////////////////////////////////
// CLabel window
enum FlashType {None, Text, Background };

class AFX_EXT_CLASS CLabel : public CStatic
{
// Construction
public:
	CLabel();
	CLabel& SetBkColor(COLORREF crBkgnd);
	CLabel& SetTextColor(COLORREF crText);
	CLabel& SetText(const CString& strText);
	CLabel& SetFontBold(BOOL bBold);
	CLabel& SetFontName(const CString& strFont);
	CLabel& SetFontUnderline(BOOL bSet);
	CLabel& SetFontItalic(BOOL bSet);
	CLabel& SetFontSize(int nSize);
	CLabel& SetSunken(BOOL bSet);
	CLabel& SetBorder(BOOL bSet);
	CLabel& FlashText(BOOL bActivate);
	CLabel& FlashBackground(BOOL bActivate);
	CLabel& SetLink(BOOL bLink);
	CLabel& SetLinkCursor(HCURSOR hCursor);

// Attributes
public:
	void SetHomePage();
	void SetE_Mail();
	void SetHiddenHelp(COLORREF crHiddenHelp);

protected:
	void ReconstructFont();
	COLORREF	m_crText;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	LOGFONT		m_lf;
	CFont		m_font;
	CString		m_strText;
	BOOL		m_bState;
	BOOL		m_bTimer;
	BOOL		m_bLink;
	FlashType	m_Type;
	HCURSOR		m_hCursor;

	BOOL        m_bHiddenHelp;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLabel)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
