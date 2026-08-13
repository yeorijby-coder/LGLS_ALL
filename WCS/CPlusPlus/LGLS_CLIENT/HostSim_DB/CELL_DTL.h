#pragma once
#include "afx.h"

class CCELL_DTL : public CObject
{
public:
	CCELL_DTL(void);
	~CCELL_DTL(void);

public:
	CString	WH_TYP;  
	CString	CELL_NO;         
	CString	BANK;           
	CString	BAY;            
	CString	LEV;            
	CString	AGING_START_DT; 
	CString	AGING_END_DT;   
	CString	USER_REMARKS;   
	CString	REMARKS;        
	CString	INS_DT;         
	CString	INS_USER_ID;    
	CString	UPD_DT;         
	CString	UPD_USER_ID;   
	CString	BOTTOM_TRAY;   
	CString	TOP_TRAY;        
};

