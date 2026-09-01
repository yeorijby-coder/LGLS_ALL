#pragma once
#include "afx.h"
#include "CELL_DTL.h"

class CCellInfo : public CObject
{
public:
	CCellInfo(void);
	~CCellInfo(void);

public:
	CString	WH_TYP;   
	CString	CELL_SC_NO;      
	CString	CELL_NO;     
	CString	BANK;  
	CString	BAY; 
	CString	LEV;
	CString	GDBOX_NO;
	CString	GDBOX_RACD;
	CString	FIRE_DETECTOR_NO;
	CString	CELL_FIRE_YN;
	CString	CELL_STA;
	CString	CELL_USE_DEF;
	CString	CELL_USE_YN;
	CString	AGING_TYP;
	CString	CELL_TYP;
	CString	BOTTOM_TRAY;
	CString	TOP_TRAY;
	CString	UPD_DT;
	CString	USER_REMARKS;
	CString	REMARKS;
	CString	CELL_STATUS;

	CList<CCELL_DTL*> m_pCELL_DTL;
};

