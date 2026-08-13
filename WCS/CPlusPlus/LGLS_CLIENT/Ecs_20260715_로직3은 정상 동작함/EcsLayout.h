// EcsLayout.h: interface for the CEcsLayout class.
//

#pragma once

#include "DciMaster.h"
#include "DciControl.h"
#include "EcsDef.h"


// CEcsLayout
//
class CEcsLayout : public CObject  
{
public:
	CEcsLayout();
	virtual ~CEcsLayout();

public:
	enum EN_CONTROL_KIND {
		enCV = 10, enOPBOX = 11, enDOOR = 12,
		enSC = 20, enRTV = 30, enLIFTER = 40, enLGV = 50,
		enRFID = 60, enGroupBCR = 70, enGroupDISPLAY = 80, enThs = 90
	};

protected:
	CDciMaster* m_pDCI;
	CDciControlList* m_pDciCtrls;

public:
	EN_LAYOUT m_enLayout;

protected:
	int		m_nFontSize;
	CFont	m_oFont;

public:
	CString m_strLog;
	BOOL	m_bFirst;
	int		m_nZoomCount;
	int		m_nZoomLimit;
	CString m_strText;

public:		// ABOUT VIEW
	void OnInitialUpdate(CWnd* pWnd);
	void OnDraw(CWnd* pWnd, CDC* pDC, int nTabType = 0);
	void OnSize(CWnd* pWnd, UINT nType, int cx, int cy);
	BOOL OnButtonDown(CWnd* pWnd, UINT nFlag, const CPoint& point);
	void OnButtonUp(CWnd* pWnd, UINT nFlag, const CPoint& point);
	void OnKeyDown(CWnd* pWnd, UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnViewLayoutInfo(CWnd* pWnd);
	BOOL LoadXml(LPCTSTR lpszFullPath, CString strKioskNo);
	void OnMouseWheel(CWnd* pWnd, UINT nFlags, short zDelta, CPoint pt);
	void OnOriginalSize(CWnd* pWnd);
	void OnZoomIn(CWnd* pWnd);
	void OnZoomOut(CWnd* pWnd);

public:
	CDciControl* GetDciControl(const CString& strCID);
	CDciMaster* GetDciMaster() { return m_pDCI; }
	int GetDciCtrlCount() { return m_pDciCtrls->GetCount(); }
	BOOL Test(CWnd* pWnd, int nLeft, int nRight, int nTop, int nBottom);
};
