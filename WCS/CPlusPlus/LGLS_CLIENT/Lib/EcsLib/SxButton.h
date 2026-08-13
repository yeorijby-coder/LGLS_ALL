#ifndef __SXBUTTON_H__
#define __SXBUTTON_H__

#define	SXBUTTON_CENTER	-1

class AFX_EXT_CLASS CSxButton : public CButton
{
// Construction
public:
	CSxButton();

// Attributes
private:
	//	Positioning
	BOOL		m_bUseOffset;				
	CPoint		m_pointImage;
	CPoint		m_pointText;
	int			m_nImageOffsetFromBorder;
	int			m_nTextOffsetFromImage;

	//	Image
	HICON		m_hIcon;					
	HBITMAP		m_hBitmap;
	HBITMAP		m_hBitmapDisabled;
	int			m_nImageWidth, m_nImageHeight;

	//	Color Tab
	char		m_bColorTab;				
	COLORREF	m_crColorTab;

	//	State
	BOOL		m_bDefault;
	UINT		m_nOldAction;
	UINT		m_nOldState;
	

// Operations
public:
	//	Positioning
	int		SetImageOffset( int nPixels ); 
	int		SetTextOffset( int nPixels );
	CPoint	SetImagePos( CPoint p );
	CPoint	SetTextPos( CPoint p );

	//	Image
	BOOL	SetIcon( UINT nID, int nWidth, int nHeight );
	BOOL	SetBitmap( UINT nID, int nWidth, int nHeight );
	BOOL	SetMaskedBitmap( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask );
	BOOL	HasImage() { return (BOOL)( (m_hIcon != 0)  | (m_hBitmap != 0) ); }

	//	Color Tab
	void	SetColorTab(COLORREF crTab);

	//	State
	BOOL	SetDefaultButton( BOOL bState = TRUE );

private:
	BOOL	SetBitmapCommon( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask, BOOL bUseMask );
	void	CheckPointForCentering( CPoint &p, int nWidth, int nHeight );
	void	Redraw();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSxButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSxButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSxButton)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
