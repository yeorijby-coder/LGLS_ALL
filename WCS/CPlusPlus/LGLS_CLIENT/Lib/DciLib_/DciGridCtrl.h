// DciGridCtrl.h: interface for the CDciGridCtrl class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include "DciControl.h"


class AFX_EXT_CLASS CDciGridCtrl : public CDciControl  
{
	DECLARE_SERIAL(CDciGridCtrl)

public:
	CDciGridCtrl(void);
	virtual ~CDciGridCtrl(void);

public:
	enum { enGridCtrlPropSize = CDciControl::enControlPropSize + 1 };

public:
	COLORREF m_clrSelect;
	BOOL	m_bShowLabel;

public:
	CRect	m_rcSelectL;

public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);

public:
	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enGridCtrlPropSize; }

public:
	CRect	GetRectFromPoint(const CPoint& ptScreen);
	CRect	GetRectFromRect(const CRect& rcScreen);
	void	SetSelectRect(const CPoint& ptScreen1, const CPoint& ptScreen2);
	void	SetSelectRect(const CPoint& ptScreen) { m_rcSelectL = GetRectFromPoint(ptScreen); }
	void	SetSelectRect(const CRect& rcScreen) { m_rcSelectL = GetRectFromRect(rcScreen); }
};

//#undef AFX_DATA
//#define AFX_DATA

