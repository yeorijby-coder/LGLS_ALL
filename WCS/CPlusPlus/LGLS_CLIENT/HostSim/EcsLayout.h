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
#include "DciTrackCtrl.h"

class CEcsLayout : public CObject  
{
public:
	CEcsLayout();
	virtual ~CEcsLayout();

public:
	enum EN_CONTROL_KIND {
		enCV = 10, enOPBOX = 11, enDOOR = 12,
		enSC = 20, enRTV = 30, enRTVPOS = 35, enLIFTER = 40, enLGV = 50,
		enRFID = 60, enGroupBCR = 70, enGroupDISPLAY = 80
	};

protected:
	CDciMaster* m_pDCI;
	CDciControlList* m_pDciCtrls;

protected:
	int m_nFontSize;
	CFont m_oFont;

public:
	CString m_strLog;
	BOOL m_bVirtical;
	BOOL m_bShowText;

public:		// ABOUT VIEW
	void OnInitialUpdate(CWnd* pWnd);
	void OnDraw(CWnd* pWnd, CDC* pDC);
	void OnSize(CWnd* pWnd, UINT nType, int cx, int cy);
	void OnButtonDown(CWnd* pWnd, UINT nFlag, const CPoint& point);
	void OnButtonUp(CWnd* pWnd, UINT nFlag, const CPoint& point);
	void OnKeyDown(CWnd* pWnd, UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnViewLayoutInfo(CWnd* pWnd, int nFontSize = 14);
	void OnViewLuggNum(CWnd* pWnd, int nFontSize, int LuggNum);
	BOOL LoadXml(LPCTSTR lpszFullPath);

public:
	CDciControl* GetDciControl(const CString& strCID);
	CDciMaster* GetDciMaster() { return m_pDCI; }
	CDciControl* GetRgvPosition(int nRgvNum, int nPos);
};

#endif // !defined(AFX_ECSLAYOUT_H__FAC3F16B_6EE1_47A7_8661_97D0ECDD2B8A__INCLUDED_)
