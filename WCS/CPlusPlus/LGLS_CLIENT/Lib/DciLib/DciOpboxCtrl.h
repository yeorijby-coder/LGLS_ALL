// DciOpboxCtrl.h: interface for the CDciOpboxCtrl class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include "DciControl.h"


class AFX_EXT_CLASS CDciOpboxCtrl : public CDciControl  
{
	DECLARE_SERIAL(CDciOpboxCtrl)

public:
	CDciOpboxCtrl(void);
	virtual ~CDciOpboxCtrl(void);

public:
	enum { enOpboxCtrlPropSize = CDciControl::enControlPropSize + 3 };
	enum EN_MODE { enModeNone, enModeAuto, enModeManual, enModeEmergency, enModeAll, enModeSize };

public:
	int	m_nMode;
	int m_nGapW;
	int m_nGapH;

public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);

public:
	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enOpboxCtrlPropSize; }
};

//#undef AFX_DATA
//#define AFX_DATA

