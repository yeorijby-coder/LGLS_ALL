
#pragma once

class AFX_EXT_CLASS CColor
{
public:
	CColor();
	CColor(COLORREF colorB, COLORREF colorT);

public:
	COLORREF m_colorBack;
	COLORREF m_colorText;
};

class AFX_EXT_CLASS CExtLabel : public CStatic
{
public:
	CExtLabel();

public:
	COLORREF	m_colorBack;
	COLORREF	m_colorText;

	void	SetBackColor(COLORREF color);
	void	SetTextColor(COLORREF color);

public:
	BOOL	m_bTransparent;
	void	SetTransparent(BOOL bSW);

	BOOL	m_bOutLine;
	void	UseOutLine(BOOL bSW);

public:
	CFont	m_Font;
	LOGFONT	m_logfont;
	BOOL	m_bFont3DIntaglio;
	BOOL	m_bFont3DShadow;

	void	InitFont();
	void	SetFont(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
	void	SetFontSize(int textsize, BOOL bold);
	void	SetFontAngle(long angle=0);
	void	SetFontShadow(BOOL bShadow);//BOOL bHilight, BOOL bShadow);
	void	SetFontIntaglio(BOOL bIntaglio);
	void	ReconstructFont();
	void	UpdateSurface();

public:
	BOOL	m_bUseBitmap;
	CBitmap* m_pBMPArray;
	int		m_nNumOfBitmap;
	int		m_nBmpNoToDraw;		// 그려질 비트맵 Number;
	CSize   m_ImageSize;	// width and height of Image

	void	InitBitmapArray(int numofbitmap);
	void	SetBmpNoToDraw(int BmpNo);
	void	LoadBitmapFromRes(UINT ResName, int bitmapNo);
	void	DrawBitmapOnBackGround(CDC* pDC, int bitmapNo);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtLabel)
	//}}AFX_VIRTUAL

public:
	virtual ~CExtLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtLabel)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

