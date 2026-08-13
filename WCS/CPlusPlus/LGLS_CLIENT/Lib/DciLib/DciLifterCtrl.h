// DciLifterCtrl.h: interface for the CDciLifterCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCILIFTERCTRL_H__B9E4F02D_169C_4C7F_BC96_2EADE1496788__INCLUDED_)
#define AFX_DCILIFTERCTRL_H__B9E4F02D_169C_4C7F_BC96_2EADE1496788__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DciControl.h"

class CDciLifterCtrl : public CDciControl  
{
	DECLARE_SERIAL(CDciLifterCtrl)

protected:
	CDciLifterCtrl();

public:
	virtual ~CDciLifterCtrl();

public:
	class AFX_EXT_CLASS CDciLifterItem : public CObject
	{
	public:
		CDciLifterItem() : m_nLifterID(0), m_rcLifterL(CRect(0,0,0,0)), m_clrLifter(RGB(128,128,128)) {}
		virtual ~CDciLifterItem() {}

	public:
		enum EN_SHAPE { enShapeNone, enShapeRectangle, enShapeToLeft, enShapeToRight, enShapeToUp, enShapeToDown };

	public:
		int m_nLifterID;
		CRect m_rcLifterL;
		COLORREF m_clrLifter;
	};
	class AFX_EXT_CLASS CDciLifterItems : public CArray<CDciLifterItem, CDciLifterItem>
	{
	public:
		CDciLifterItems() {}
		virtual ~CDciLifterItems() {}
	};

public:
	enum { enLifterCtrlPropSize = CDciControl::enControlPropSize + 1 };

protected:
	int m_nSize;

public:
	CDciLifterItems m_items;

public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);

public:
	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enLifterCtrlPropSize; }

public:
	virtual int UpdateItemNames(CDciPropertyArray& properties);
	virtual int UpdateItemValues(int nIndex, CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetItemSize() { return m_items.GetSize(); }

public:
	virtual BOOL SetClick(CWnd* pWnd, const CPoint& ptClickS);
};

#endif // !defined(AFX_DCILIFTERCTRL_H__B9E4F02D_169C_4C7F_BC96_2EADE1496788__INCLUDED_)
