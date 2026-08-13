// DciStatisticCtrl.h: interface for the CDciStatusCtrl class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include "DciControl.h"
#include "DciStatusCtrl.h"


class AFX_EXT_CLASS CDciStatisticCtrl : public CDciControl  
{
	DECLARE_SERIAL(CDciStatisticCtrl)

public:
	CDciStatisticCtrl(void);
	virtual ~CDciStatisticCtrl(void);

public:
	enum { enStatisticCtrlPropSize = CDciControl::enControlPropSize + 2 };
	enum EN_SHAPE { enShpaeBasic, enShapeHorizontal, enShapeVertical };

public:
	CArray<CDciStatusCtrl*, CDciStatusCtrl*> m_pStatuCtrls;
	CArray<int, int> m_nRatios;
	int m_nShape;
	int m_nCount;

public:
	int m_nStatus;

public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);

public:
	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enStatisticCtrlPropSize; }
};

//#undef AFX_DATA
//#define AFX_DATA

