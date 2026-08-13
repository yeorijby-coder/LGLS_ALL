// EcsLayout.h: interface for the CEcsLayout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECSLAYOUT_H__FAC3F16B_6EE1_47A7_8661_97D0ECDD2B8A__INCLUDED_)
#define AFX_ECSLAYOUT_H__FAC3F16B_6EE1_47A7_8661_97D0ECDD2B8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DciMaster.h"
#include "DciControl.h"
#include "DciStaticCtrl.h"

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

protected:
	int m_nFontSize;
	CFont m_oFont;

public:
	CString m_strLog;
	CString m_strText;
	BOOL	m_bFirst;
	int		m_nZoomCount;
	int		m_nZoomLimit;

	int		m_nRepresentPlc;		// 한 Layout 당 Plc가 여러대 일때 Tab 선택시 기본적으로 보여질 대표 PLC의 번호(순차적 번호중의 PLC 번호)

public:		// ABOUT VIEW
	void OnInitialUpdate(CWnd* pWnd);
	void OnDraw(CWnd* pWnd, CDC* pDC, int nTabType = 0);
	void OnSize(CWnd* pWnd, UINT nType, int cx, int cy);
	void OnButtonDown(CWnd* pWnd, UINT nFlag, const CPoint& point);
	void OnButtonUp(CWnd* pWnd, UINT nFlag, const CPoint& point);
	void OnKeyDown(CWnd* pWnd, UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnViewLayoutInfo(CWnd* pWnd);
	BOOL LoadXml(LPCTSTR lpszFullPath);
	void OnOriginalSize(CWnd* pWnd);


public:
	CDciControl* GetDciControl(const CString& strCID);
	CDciMaster* GetDciMaster() { return m_pDCI; }

public:
	void OnZoomIn(CWnd* pWnd);
	void OnZoomOut(CWnd* pWnd);

};

#endif // !defined(AFX_ECSLAYOUT_H__FAC3F16B_6EE1_47A7_8661_97D0ECDD2B8A__INCLUDED_)
