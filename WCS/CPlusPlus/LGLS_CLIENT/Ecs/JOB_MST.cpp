#include "StdAfx.h"
#include "JOB_MST.h"


CJOB_MST::CJOB_MST(void)
{
	COPY_YN = false;
}

CJOB_MST::CJOB_MST(CJOB_MST* pJobMst)
{
	//나중에확인
	WH_TYP = pJobMst->WH_TYP;
	LUGG_NO = pJobMst->LUGG_NO;
	START_POS = pJobMst->START_POS;
	START_LOCATION = pJobMst->START_LOCATION;
	DEST_POS = pJobMst->DEST_POS;
	DEST_LOCATION = pJobMst->DEST_LOCATION;
	TURN = pJobMst->TURN;
	JOB_TYP = pJobMst->JOB_TYP;
	BCR_TOP = pJobMst->BCR_TOP;
	BCR_BOTTOM = pJobMst->BCR_BOTTOM;
	JOB_KIND = pJobMst->JOB_KIND;
	JOB_STATUS = pJobMst->JOB_STATUS;
	JOB_PRIORITY = pJobMst->JOB_PRIORITY;
	MES_ERROR_CD = pJobMst->MES_ERROR_CD;
	TRAY_TYP = pJobMst->TRAY_TYP;
	TRAY_LEV = pJobMst->TRAY_LEV;
	DURATION_TIME = pJobMst->DURATION_TIME;
	PRODUCT_SIZE = pJobMst->PRODUCT_SIZE;
	COPY_YN = pJobMst->COPY_YN;
}

CJOB_MST::~CJOB_MST(void)
{
}
