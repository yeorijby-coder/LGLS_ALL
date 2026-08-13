
#pragma once

#define MINBUTTON_CENTER		-1
#include "DrawGradationRect.h"

class AFX_EXT_CLASS CMinButton : public CButton
{

public:
	static enum BackFillMode { Normal, Gradient };
	static enum GradationType {LEFTTORIGHT, RIGHTTOLEFT, UPTODOWN, DOWNTOUP, SpreadToLeftAndRightFromCenter, SpreadToUpAndDownFromCenter};

// Construction
public:
	CMinButton();
	virtual ~CMinButton();

public:
	COLORREF m_textColor;		// 기본 버튼 Color
	BOOL	m_bTextBold;
	int		m_nTextHeight;
	LOGFONT	m_logFont;

	HBRUSH	m_hBackBrush;
	CFont	m_newFont;
	int	m_Gradation_Type;	// 색상 변화 타입

	CDrawGradationRect m_GradationRect;

// Attributes
private:
	//	Positioning
	BOOL		m_bUseOffset;
	CPoint		m_pointText;
	int			m_nTextOffsetFromImage;

	//	State
	BOOL			m_bDefault;
	UINT			m_nOldAction;
	UINT			m_nOldState;

// Operations
public:
	UINT GetFillMode()	{ return m_GradationRect.GetFillMode(); };
	UINT GetPercent()	{ return m_GradationRect.GetPercent(); };

	virtual CMinButton& SetFontName(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
	virtual CMinButton& SetBkColor(COLORREF crBkgnd, COLORREF crBkgndHigh = 0, UINT mode = 1, int tpercent = 100);

	int		SetTextOffset( int nPixels );
	CPoint	SetTextPos( CPoint p );

	BOOL		SetDefaultButton( BOOL bState = TRUE );
	void		SetTextSize(int textsize, BOOL bold = FALSE);
	void		Redraw();

private:
	void		CheckPointForCentering( CPoint &p, int nWidth, int nHeight );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMinButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	

// Generated message map functions
protected:
	//{{AFX_MSG(CMinButton)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

