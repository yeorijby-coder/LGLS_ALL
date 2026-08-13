#pragma once
#include "afx.h"

class CJOB_MST : public CObject
{
public:
	CJOB_MST(void);
	CJOB_MST(CJOB_MST* pJobMst);
	~CJOB_MST(void);


public:
	CString WH_TYP;
	CString LUGG_NO;
	CString START_POS;
	CString START_LOCATION;
	CString DEST_POS;
	CString DEST_LOCATION;
	CString TURN;
	CString JOB_TYP;
	CString BCR_TOP;
	CString BCR_BOTTOM;
	CString JOB_KIND;
	CString JOB_STATUS;
	CString JOB_PRIORITY;
	CString MES_ERROR_CD;
	CString TRAY_TYP;
	CString TRAY_LEV;
	CString DURATION_TIME;
	CString PRODUCT_SIZE;

	bool COPY_YN;
};

