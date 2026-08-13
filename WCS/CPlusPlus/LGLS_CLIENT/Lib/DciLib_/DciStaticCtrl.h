// DciStaticCtrl.h: interface for the CDciStaticCtrl class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include "DciControl.h"


class AFX_EXT_CLASS CDciStaticCtrl : public CDciControl  
{
	DECLARE_SERIAL(CDciStaticCtrl)

public:
	CDciStaticCtrl(void);
	virtual ~CDciStaticCtrl(void);

public:
	enum { enStaticCtrlPropSize = CDciControl::enControlPropSize + 9 };
	enum EN_BORDER { enBorderNone, enBorderRectangle, enBorderRoundRect, enBorderEllipse, enBorderLargeFont, enBorderSize };

public:
	int m_nFontSize;
	int m_nBorder;
	int m_nPenStyle;
	int m_nPenWidth;
	COLORREF m_clrPen;
	CRect m_rcGap;
	int m_nFontSizeTemp;

public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);

public:
	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enStaticCtrlPropSize; }
};

//#undef AFX_DATA
//#define AFX_DATA

