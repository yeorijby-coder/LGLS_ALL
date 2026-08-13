// DciMaster.h: interface for the CDciMaster class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include <afxtempl.h>


class CDciControl;
//class CDciStatisticCtrl;

class AFX_EXT_CLASS CDciMaster
{
public:
	CDciMaster(void);
	virtual ~CDciMaster(void);

public:
	class CDciCreator : public CArray<CRuntimeClass*, CRuntimeClass* > {};

protected:
	CDciCreator m_dciCreator;

protected:
	CRect	m_rcLayoutL;			// Logical => 논리적 크기
	CRect	m_rcLayoutS;			// Screen  => 실제크기
	CPoint	m_ptOrginL;				// Logical => 논리적 크기
	CPoint	m_ptOrginS;				// Screen  => 실제크기
	int		m_nScale;

public:	
	CRect	m_rcLayoutCenter;
	CRect	m_rcLayoutLTemp;	// 임시 (확대를 위해서 ) 

public:
	CDciControl* CreateDciObject(int nIndex, const CDciControl* pDciCtrlSrc = NULL);
	CDciControl* CreateDciObject(const CString& strClassName, const CDciControl* pDciCtrlSrc = NULL);
//	CDciControl* CreateDciObject(const LPCSTR& strClassName, const CDciControl* pDciCtrlSrc = NULL);
	void ClonDciObject(const CDciControl* pDciCtrlSrc, CDciControl* pDciCtrlTar);
	CRuntimeClass* GetDciClass(int nIndex);
	CRuntimeClass* GetDciClass(const CString& strClassName);
	int GetDciClassIndex(CRuntimeClass* pClass);
	int	GetDciClassSize();

public:
	void	RecalcLayout();

public:
	CRect	GetLayoutL() { return m_rcLayoutL; }
	void	SetLayoutL(const CRect& rcLayoutL) { m_rcLayoutL = rcLayoutL; }
	void	SetLayoutL(int l, int t, int r, int b) { m_rcLayoutL.SetRect(l, t, r, b); }
	CRect	GetLayoutS() { return m_rcLayoutS; }
	void	SetLayoutS(const CRect& rcLayoutS) { m_rcLayoutS = rcLayoutS; }
	void	SetLayoutS(int l, int t, int r, int b) { m_rcLayoutS.SetRect(l, t, r, b); }
	int		GetScale() { return m_nScale; }

public:
	CPoint	ConvertPointL(int x, int y);
	CPoint	ConvertPointL(const CPoint& pt) { return ConvertPointL(pt.x, pt.y); }
	CPoint	ConvertPointS(int x, int y);
	CPoint	ConvertPointS(const CPoint& pt) { return ConvertPointS(pt.x, pt.y); }
	CRect	ConvertRectL(int l, int t, int r, int b);
	CRect	ConvertRectL(const CRect& rc) { return ConvertRectL(rc.left, rc.top, rc.right, rc.bottom); }
	CRect	ConvertRectS(int l, int t, int r, int b);
	CRect	ConvertRectS(const CRect& rc) { return ConvertRectS(rc.left, rc.top, rc.right, rc.bottom); }

public:
	void	DrawText(CDC* pDC, const CRect& rcRectL, const CString& strText, COLORREF clrText);
	void	Draw3dSolidRect(CDC* pDC, const CRect& rcRectL, COLORREF clrBrush, COLORREF clrLeftTop, COLORREF clrRightBottom);
	void	DrawButton(CDC* pDC, const CRect& rcRectL, COLORREF clrBrush, BOOL bClick);
};

//#undef AFX_DATA
//#define AFX_DATA

