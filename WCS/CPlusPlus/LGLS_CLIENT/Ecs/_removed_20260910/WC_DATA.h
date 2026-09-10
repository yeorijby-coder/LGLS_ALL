#pragma once
#include "afx.h"
#include "DciButtonCtrl.h"


class CWC_DATA : public CObject
{
public:
	CWC_DATA(void);
	CWC_DATA(CString pstrWH_TYP, CString strWC_MC_NO, CString pstrWC_NO);

	~CWC_DATA(void);

public:
	BOOL m_bModified;

public:
	CDciButtonCtrl* m_pControl;

public:
	//key
	CString K_WH_TYP;
	CString K_PLC_NO;
	CString K_WC_MC_NO;
	CString K_WC_NO;

public:
	void SetWH_TYP(CString K_WH_TYP);
	void SetPLC_NO(CString K_PLC_NO);
	void SetWC_NO(CString K_WC_NO);
	void SetWC_MC_NO(CString K_WC_MC_NO);

public:
	//data
	CString V_WEIGHT_READ_STA;
	CString V_WEIGHT_READ_STA_NM;
	CString V_WEIGHT_RCV_VAL;
	CString V_CMD_RQ_ID;
	CString V_CMD_RQ_YN;
	CString V_SUSPEND;
	CString V_SUSPEND_NM;
	CString V_EQP_TIME;
	CString V_EQP_CONNECTED_YN;
	CString V_EQP_COLOR;
	CString V_CHK_BYPASS_YN;
	CString V_CHK_BYPASS_YN_NM;

public:
	void SetWEIGHT_READ_STA(CString V_WEIGHT_READ_STA);
	void SetWEIGHT_READ_STA_NM(CString V_WEIGHT_READ_STA_NM);
	void SetWEIGHT_RCV_VAL(CString V_WEIGHT_RCV_VAL);
	void SetCMD_RQ_ID(CString V_CMD_RQ_ID);
	void SetCMD_RQ_YN(CString V_CMD_RQ_YN);
	void SetSUSPEND(CString V_SUSPEND);
	void SetSUSPEND_NM(CString V_SUSPEND_NM);
	void SetEQP_TIME(CString V_EQP_TIME);
	void SetEQP_CONNECTED_YN(CString V_EQP_CONNECTED_YN);
	void SetEQP_COLOR(CString V_EQP_COLOR);
	void SetCHK_BYPASS_YN(CString V_CHK_BYPASS_YN);
	void SetCHK_BYPASS_YN_NM(CString V_CHK_BYPASS_YN_NM);

public:
	virtual CString GetCid();
	CString m_strWC_MC_NO;
};

