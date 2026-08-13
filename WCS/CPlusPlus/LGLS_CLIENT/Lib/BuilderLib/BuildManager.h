// BuildManager.h: interface for the CBuildManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUILDMANAGER_H__1C7399AD_A2F9_4B76_84B2_25FA5DAC83A2__INCLUDED_)
#define AFX_BUILDMANAGER_H__1C7399AD_A2F9_4B76_84B2_25FA5DAC83A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XmlDom.h"
#include "DciMaster.h"
#include "DciGridCtrl.h"
#include "DciButtonCtrl.h"
#include "DciRackCtrl.h"
#include "DciTrackCtrl.h"
#include "DciStaticCtrl.h"
#include "ControlGroup.h"

class AFX_EXT_CLASS CBuildManager  
{
public:
	CBuildManager();
	virtual ~CBuildManager(); 

public:
	CDciMaster* m_pDCI;
	CDciGridCtrl* m_pDciGridCtrl;
	CDciControlList* m_pSelectCtrls;
	CControlGroups m_ctrlGroups;

protected:
	CPoint m_ptSelect1st;
	CPoint m_ptSelect2nd;

public:
	CString m_strMessage;
	BOOL	m_bFirst;
	int		m_nZoomCount;
	int		m_nZoomLimit;

public:
//	CControlManagerDlg	m_oControlManagerDlg;

protected:
	BOOL LoadXml(const CString& strFullPath);
	BOOL SaveXml(const CString& strFullPath);

public:		// ABOUT VIEW
	void OnInitialUpdate(CWnd* pWnd);
	void OnDraw(CWnd* pWnd, CDC* pDC);
	void OnSize(CWnd* pWnd, UINT nType, int cx, int cy);
	void OnButtonDown(CWnd* pWnd, UINT nFlag, const CPoint& point);
	void OnButtonUp(CWnd* pWnd, UINT nFlag, const CPoint& point);
	void OnKeyDown(CWnd* pWnd, UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnMouseWheel(CWnd* pWnd, UINT nFlags, short zDelta, CPoint pt);
	void OnOriginalSize(CWnd* pWnd);
	void OnZoomIn(CWnd* pWnd);
	void OnZoomOut(CWnd* pWnd);

public:		// ABOUT MENU
	void OnFileOpen(CWnd* pWnd);
	void OnFileSave(CWnd* pWnd = NULL);
};

#endif // !defined(AFX_BUILDMANAGER_H__1C7399AD_A2F9_4B76_84B2_25FA5DAC83A2__INCLUDED_)
