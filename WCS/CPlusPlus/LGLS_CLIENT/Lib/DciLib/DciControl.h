// DciControl.h: interface for the CDciControl class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once
#include <afxmt.h>	// [LGLS 2026-09-10] CCriticalSection

#include <afxtempl.h>
#include "DciMaster.h"
#include "DciProperty.h"


// CDciControl 명령 대상입니다.
class AFX_EXT_CLASS CDciControl : public CObject
{
	DECLARE_SERIAL(CDciControl)

public:
	CDciControl();
	virtual ~CDciControl();

public:
	enum { enControlPropSize = 13 };	// 12

protected:
	CDciMaster*	m_pDCI;

public:
	CString		m_strCID;
	CString		m_strText;
	CRect		m_rcControlL;
	int			m_nBgMode;
	int			m_nFgMode;
	COLORREF	m_clrBgColor;
	COLORREF	m_clrFgColor;
	BOOL		m_bEnable;
	BOOL		m_bVisible;
	BOOL		m_bClick;
	int			m_nFontSize;

	// [LGLS 2026-09-10] m_strText 는 수집 스레드가 바꾸고 UI 스레드가 그린다.
	//   CString 대입이 옛 버퍼를 해제하므로 그리는 도중에 겹치면 DrawText 안에서 죽는다
	//   (2026-09-10 16:48 크래시 : ScriptStringAnalyse 에서 ACCESS_VIOLATION).
	//   글자를 읽고 쓸 때는 이 두 함수를 쓴다. 직접 m_strText 를 만지지 않는다.
	static CCriticalSection& TextLock();
	void		SetTextSafe(LPCTSTR lpszText);
	CString		GetTextSafe();

public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);
	virtual void InvalidateControl(CWnd* pWnd, BOOL bErase);
	virtual void InvalidateControl(HWND hWnd, BOOL bErase);
	virtual void DrawFontText(CDC* pDC, CString strText, CRect* pRect = NULL, int nOldBkMode=0, int nOldFgColor=0, CBrush* pOldBrush = NULL, CPen* pOldPen = NULL);

public:
	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enControlPropSize; }

public:
	virtual int UpdateItemNames(CDciPropertyArray& properties) { return 0; }
	virtual int UpdateItemValues(int nIndex, CDciPropertyArray& properties, BOOL bSaveObject = TRUE) { return 0; }
	virtual int GetItemSize() { return 0; }

public:
	virtual BOOL SetClick(CWnd* pWnd, const CPoint& ptClickS);
};

class AFX_EXT_CLASS CDciControlArray : public CArray<CDciControl*, CDciControl*>
{
public:
	CDciControlArray();
	virtual ~CDciControlArray();

public:
	void Clear();
};

class AFX_EXT_CLASS CDciControlList : public CList<CDciControl*, CDciControl*>
{
public:
	CDciControlList();
	virtual ~CDciControlList();

public:
	void Clear();
};

//#undef AFX_DATA
//#define AFX_DATA


