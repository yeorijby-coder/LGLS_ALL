
#ifndef __EDITEX_H__
#define __EDITEX_H__

class CLogFont : public LOGFONT
{
public:
	CLogFont( LONG fontHeight = -8, 
              LONG fontWeight = FW_NORMAL,
              UCHAR pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE,
              LPCTSTR faceName = _T("MS Sans Serif")
            );

public:
    void FaceNameSet( LPCTSTR faceName );
};

class CEditMask
{
public:
    virtual BOOL AddChar( UINT nChar ) = 0;
};

class AFX_EXT_CLASS CEditEx : public CEdit
{

public:
	CEditEx();
    DECLARE_DYNCREATE( CEditEx )

private:
    CBrush   m_brBkGround;
    COLORREF m_crBkColor;
    COLORREF m_crTextColor;

    UINT m_MenuResourceID;

    CFont* m_pCFont;
    CEditMask* m_pEditMask;

protected:
    virtual BOOL CreateBrushType();

public:
   // Allow basics to be copied...
   CEditEx& operator = ( const CEditEx& );

// Attributes
public:
    COLORREF SetBkColor( COLORREF );    
    COLORREF SetTextColor( COLORREF );

    COLORREF GetBkColor()	const{ return m_crBkColor;  }   
    COLORREF GetTextColor()	const { return m_crTextColor; }

    void SetEditExFont( const LOGFONT* lpLogFont );
    void SetEditExFont( LONG fontHeight = -8,
						LONG fontWeight = FW_NORMAL, 
						UCHAR pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE,
						LPCTSTR faceName = _T("MS Sans Serif"));

    void SetCustomMask( CEditMask* editMask ); 
    void DefinePopupMenu( UINT uResourceID = 0 /* 0 uses default */ )   
	{  
		m_MenuResourceID = uResourceID;
	}

// Operations:
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditEx)
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};



#endif
