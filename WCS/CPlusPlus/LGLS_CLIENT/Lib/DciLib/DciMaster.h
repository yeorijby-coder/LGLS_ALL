// DciMaster.h: interface for the CDciMaster class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include <afxtempl.h>
#include <afxmt.h>	// [LGLS 2026-09-14] 글자 맞춤 캐시 락


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
	void	DrawText(CDC* pDC, const CRect& rcRectL, const CString& strText, COLORREF clrText, int nFontSize = 10);

	// [LGLS 2026-09-10] 레이아웃 XML 의 fontsize 규칙에 따라 글자를 그린다.
	//   nBaseFontSize == 0 : 칸(화면 좌표) 크기에 맞춘다. 칸이 커지면 글자도 커진다.
	//   nBaseFontSize >  0 : 그 값을 그대로 쓴다(종전 동작).
	//   랙 / 트랙 / RV / 리프터 / 스태틱이 모두 이 함수를 쓴다.
	void	DrawTextFit(CDC* pDC, const CRect& rcRectS, const CString& strText, int nBaseFontSize);
	// [LGLS 2026-09-17] 두 색 글자 : 글자 안의 '|' 앞은 clrText1, 뒤는 clrText2 로 그린다('|' 는 그리지 않는다).
	//   글꼴 크기와 가운데 맞춤은 '|' 를 뺀 글자로 DrawTextFit 과 같게 정한다. (작업대 명칭 - 입출고대 = 입고색+출고색)
	void	DrawTextFit2(CDC* pDC, const CRect& rcRectS, const CString& strText, COLORREF clrText1, COLORREF clrText2, int nBaseFontSize);

protected:
	// [LGLS 2026-09-14] 글자 맞춤 캐시 (화면 지연 개선).
	//   종전에는 그릴 때마다 칸마다 글꼴을 최대 8번 만들고 글자 폭을 쟀다.
	//   "칸 폭|칸 높이|글자" 로 맞춘 글꼴 높이를 기억하고, 높이별 글꼴은 한 번만 만든다.
	CMap<CString, LPCTSTR, int, int>	m_mapFitHeight;
	CMap<int, int, CFont*, CFont*>		m_mapFitFont;
	CCriticalSection					m_csFit;
	CFont*	GetFitFont(int nHeight);

public:

	void	Draw3dSolidRect(CDC* pDC, const CRect& rcRectL, COLORREF clrBrush, COLORREF clrLeftTop, COLORREF clrRightBottom);
	void	DrawButton(CDC* pDC, const CRect& rcRectL, COLORREF clrBrush, BOOL bClick);
};

//#undef AFX_DATA
//#define AFX_DATA

