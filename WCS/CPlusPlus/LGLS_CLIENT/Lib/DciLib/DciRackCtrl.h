// DciRackCtrl.h: interface for the CDciRackCtrl class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include "DciControl.h"

class AFX_EXT_CLASS CDciRackCtrl : public CDciControl
{
	DECLARE_SERIAL(CDciRackCtrl)

public:
	CDciRackCtrl(void);
	virtual ~CDciRackCtrl(void);

public:
	enum { enRackCtrlPropSize = CDciControl::enControlPropSize + 4 };
	enum EN_TYPE { enL2R, enR2L, enT2B, enB2T, enTypeSize };

protected:
	int m_nType;

public:
	int	m_nUnitLenL;
	int	m_nUnitLenH;	// [LGLS 2026-09-16] 세로 유닛(0/미지정=len과 동일=정사각, 하위호환)
	int	m_nStartPos;
//	int	m_nIncreaseCount;	// 수열 개수


public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);

public:
	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enRackCtrlPropSize; }
};

//#undef AFX_DATA
//#define AFX_DATA

