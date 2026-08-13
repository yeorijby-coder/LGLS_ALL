// DciTrackCtrl.h: interface for the CDciTrackCtrl class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include "DciControl.h"

class AFX_EXT_CLASS CDciTrackCtrl : public CDciControl
{
	DECLARE_SERIAL(CDciTrackCtrl)

public:
	CDciTrackCtrl(void);
	virtual ~CDciTrackCtrl(void);

public:
	class CDciTrackItem : public CObject
	{  
	public:
		CDciTrackItem() : m_nShape(0), m_clrItem(RGB(128,128,128)), m_rcGap(CRect(20,20,20,20)) {}
		virtual ~CDciTrackItem() {}
 
	public:
		enum EN_SHAPE { enShapeNone, enShapeRectangle, enShapeToLeft, enShapeToRight, enShapeToUp, enShapeToDown };

	public:
		int m_nShape;
		COLORREF m_clrItem;
		CRect m_rcGap;
	};

	class CDciTrackItems : public CArray<CDciTrackItem, CDciTrackItem>
	{
	public:
		CDciTrackItems() {}
		virtual ~CDciTrackItems() {}
	};

public:
	enum { enTrackCtrlPropSize = CDciControl::enControlPropSize + 4 + 1};

public:
	BOOL m_bExist;
	BOOL m_bDoubleExist;

protected:
	int m_nFontSize;
	int m_nRow;
	int m_nCol;

public:
	CDciTrackItems m_items;

public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);

public:
	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enTrackCtrlPropSize; }

public:
	virtual int UpdateItemNames(CDciPropertyArray& properties);
	virtual int UpdateItemValues(int nIndex, CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetItemSize() { return m_items.GetSize(); }
};

//#undef AFX_DATA
//#define AFX_DATA

