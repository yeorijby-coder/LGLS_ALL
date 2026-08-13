#if !defined(AFX_MYTABCTRL_H__1154C1DE_A38F_4E3D_B50C_AA7720BF0923__INCLUDED_)
#define AFX_MYTABCTRL_H__1154C1DE_A38F_4E3D_B50C_AA7720BF0923__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTabCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl window
#include "Color.h"
#include "Ecsdef.h"
class CMyTabCtrl : public CTabCtrl
{
// Construction
public:
	CMyTabCtrl();

	CFont font;
	
	COLORREF m_ErrorColor;
	COLORREF m_SelectedColor;
	COLORREF m_UnSelectedColor;
	COLORREF m_BackColor;
	COLORREF m_PageColor;
	COLORREF m_SelectFontColor;
	COLORREF m_UnSelectFontColor;

//	void Refresh();
	void SetColor(COLORREF pSelectColor, COLORREF pUnSelectColor, COLORREF pBackColor, 
		          COLORREF pPageColor, COLORREF pSelectFontColor, COLORREF pUnSelectFontColor);
//	void fnCaptureBack(CDC* pDC);
	void DrawItemBorder(LPDRAWITEMSTRUCT lpdis, int nDraw);
	void DrawMainBorder(LPDRAWITEMSTRUCT lpdis);
	void fnDrawLine(CDC* pDC, COLORREF pColor, CPoint pStartPoint, CPoint pEndPoint, int pLineWidth);
	CFont* fnSetFont(CDC *pDC,int pszHeight, int pszWidth, BOOL pszBold, BOOL pszItalic, BOOL pszUnderLine, char *pszFontName);
	
	int m_tabCurrent;

	int m_nErrorTab[ECS_ALL_TAB_COUNT];
	
// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTabCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTABCTRL_H__1154C1DE_A38F_4E3D_B50C_AA7720BF0923__INCLUDED_)
