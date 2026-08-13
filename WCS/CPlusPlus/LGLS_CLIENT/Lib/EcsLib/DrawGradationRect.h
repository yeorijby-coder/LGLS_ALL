#pragma once

class CDrawGradationRect
{
public:
	enum BackFillMode { Normal, Gradient };
	enum GradationType {LEFTTORIGHT, RIGHTTOLEFT, UPTODOWN, DOWNTOUP, SpreadToLeftAndRightFromCenter, SpreadToUpAndDownFromCenter};

public:
	CDrawGradationRect();
	CDrawGradationRect(CDC *pDC, CRect rect, COLORREF crBkgnd, COLORREF crBkgndHigh, UINT mode, UINT tpercent);
	~CDrawGradationRect();
	
private:
	CDC*	m_pDC;

	CPoint	m_StartPoint;
	CRect	m_rect;
	UINT	m_Gradation_Type;
	UINT	m_fillmode;
	UINT	m_tpercent;

	COLORREF m_crBkgnd1;
	COLORREF m_crBkgnd2;

public:
	void GetDC(CDC* pDC);
	UINT GetFillMode()	{ return m_fillmode; };
	UINT GetPercent()	{ return m_tpercent; };

	void SetDrawArea(CPoint nStartPt, CRect rect);
	void SetBkColor(COLORREF crBkgnd, COLORREF crBkgndHigh=RGB(255,255,255), UINT mode=1, UINT tpercent=100);
	void SetGradationType(UINT GType=0);

public:
	void Draw();
	void DrawGradientFill(CDC* pDC);
};