// DciStatusCtrl.h: interface for the CDciStatusCtrl class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include "DciControl.h"


class AFX_EXT_CLASS CDciStatusCtrl : public CDciControl  
{
	DECLARE_SERIAL(CDciStatusCtrl)

public:
	CDciStatusCtrl(void);
	virtual ~CDciStatusCtrl(void);

public:
	enum { enStatusCtrlPropSize = CDciControl::enControlPropSize + 2 };
	enum EN_SHAPE { enShpaeBasic, enShapeHorizontal, enShapeVertical };

public:
	int m_nShape;
	int m_nRatio;
//	CRect m_rcGap;


public:
	int m_nStatus;

public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);
	void SetDciMaster(CDciMaster* pDCI) { m_pDCI = pDCI; }

public:
	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enStatusCtrlPropSize; }
};

//#undef AFX_DATA
//#define AFX_DATA

