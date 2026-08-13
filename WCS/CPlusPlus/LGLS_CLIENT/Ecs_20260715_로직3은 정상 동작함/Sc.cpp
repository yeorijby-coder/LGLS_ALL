
// Sc.cpp: implementation of the CSc class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Sc.h"
#include "ByteArrayEx.h"
#include "RecordSetWrap.h"
#include "ScSkinDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEVICE m_port.m_strDevice


// CSc
//
IMPLEMENT_DYNAMIC(CSc, CEquipment)

CSc::CSc(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex) : CEquipment(pDoc,pstrThreadNo, nIndex)
{
	m_enKind = CEquipment::enSC;
	//m_pInfo = new CScInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	//m_pSC_DATA = NULL;
}

CSc::~CSc()
{
	//delete m_pInfo;
}


void CSc::AutoRunProc()
{

	
}

void CSc::CommandProc()
{
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CString CSc::GetSelectQry() //kdh20190521
{
	CString strSql = _T("");
	
	strSql.Format(_T(" SELECT ") + m_pDoc->NVL + _T("(SD.SC_VIEW_NM,SD.SC_NO) AS SC_VIEW_NM, SD.SC_NO, SD.PLC_NO, SD.WH_TYP,												\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.JOB_TYP_RD,'0') AS JOB_TYP_RD,																						\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.ACTIVE_MODE_RD,'0') AS ACTIVE_MODE_RD,																				\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.AUTO_MODE_RD,'0') AS AUTO_MODE_RD,																					\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.UNLOAD_COMPLETE_RD,'0') AS COMPLETE_RD,																					\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.ERR_CODE_RD,'0') AS ERR_CODE_RD,																					\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.ONLINE_MODE_RD,'0') AS ONLINE_MODE_RD,																				\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.POS_H_RD,'0') AS POS_H_RD,																							\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.POS_V_RD,'0') AS POS_V_RD,																							\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.SENSOR_FK_RD,'0') AS SENSOR_FK_RD,																					 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.SUBSYSTEM_STATUS_RD,'0') AS UCSTATUS_RD,																					 \n") 
		_T("				  ") + m_pDoc->NVL + _T("(SD.USE_FK_RD,'0') AS USE_FK_RD,																						\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.DEST_BANK_FK1_RD,'0') AS DEST_BANK_FK1_RD,																				\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.DEST_BANK_FK2_RD,'0') AS DEST_BANK_FK2_RD,																				\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.DEST_BAY_FK1_RD,'0') AS DEST_BAY_FK1_RD,																			 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.DEST_BAY_FK2_RD,'0') AS DEST_BAY_FK2_RD,																			 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.DEST_HSPOS_FK1_RD,'0') AS DEST_HSPOS_FK1_RD,																		\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.DEST_HSPOS_FK2_RD,'0') AS DEST_HSPOS_FK2_RD,																		\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.DEST_LEVEL_FK1_RD,'0') AS DEST_LEVEL_FK1_RD,																		 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.DEST_LEVEL_FK2_RD,'0') AS DEST_LEVEL_FK2_RD,																		 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.ERR_STA_FK1_RD,'0') AS ERR_STA_FK1_RD,																				\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.ERR_STA_FK2_RD,'0') AS ERR_STA_FK2_RD,																				\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.FORKPOS_FK1_RD,'0') AS FORKPOS_FK1_RD,																				\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.FORKPOS_FK2_RD,'0') AS FORKPOS_FK2_RD,																				\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.PALLET_ON_VEHICLE_RD,'0') AS LUGG_NO_FK1_RD,																				 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.LUGG_NO_FK2_RD,'0') AS LUGG_NO_FK2_RD,																				 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.START_BANK_FK1_RD,'0') AS START_BANK_FK1_RD,																		\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.START_BANK_FK2_RD,'0') AS START_BANK_FK2_RD,																		 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.START_BAY_FK1_RD,'0') AS START_BAY_FK1_RD,																			\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.START_BAY_FK2_RD,'0') AS START_BAY_FK2_RD,																			\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.START_HSPOS_FK1_RD,'0') AS START_HSPOS_FK1_RD,																		\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.START_HSPOS_FK2_RD,'0') AS START_HSPOS_FK2_RD,																		\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.START_LEVEL_FK1_RD,'0') AS START_LEVEL_FK1_RD,																		\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.START_LEVEL_FK2_RD,'0') AS START_LEVEL_FK2_RD,																		\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.FORKPOS_FK1_RD,'0') AS FORKPOS_FK1_RD,																				\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.FORKPOS_FK2_RD,'0') AS FORKPOS_FK2_RD,																				 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.CV_WORKBENCH_RD,'0') AS CV_WORKBENCH_RD,																			 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.CV_WORKBENCH_SUB_RD,'0') AS CV_WORKBENCH_SUB_RD,																	\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.PLT_INFO_RD,'0') AS PLT_INFO_RD,																					\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.ITN_LUGG_FK1,'0') AS ITN_LUGG_FK1,																					\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.ITN_LUGG_FK2,'0') AS ITN_LUGG_FK2,																					\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.SUSPEND,'0') AS SUSPEND,																							 \n")
		_T("				  EM.PLC_IP, EM.PLC_PORT_FROM, EM.CONNECTED_YN,																									\n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.MC_NO,'0') AS MC_NO,																								 \n")
		_T("				  ") + m_pDoc->NVL + _T("(SD.MC_NO_NM,' ') AS MC_NO_NM,						 																	\n")
		_T("	     CASE WHEN DATEDIFF(SECOND, EM.UPD_DT, GETDATE()) > 5 THEN 6 ELSE 0 END AS EQP_TIME,		  											\n")
		_T("	     EM.CONNECTED_YN															  																			\n")
		_T("			 FROM SC_DATA SD INNER JOIN (SELECT PLC_NO, PLC_IP, PLC_PORT_FROM, CONNECTED_YN, UPD_DT FROM EQP_MST WHERE WH_TYP = '%s' AND EQP_TYP = 'SC') EM		\n")
		_T("                                     ON SD.PLC_NO = EM.PLC_NO \n")
		_T("            WHERE SD.WH_TYP = '%s'                            \n")
		_T("		ORDER BY SD.SC_NO		"),  m_pDoc->m_WH_TYP, m_pDoc->m_WH_TYP);


	return strSql;
}

void CSc::SetVar(CRecordSetWrap* pRsw) //kdh20190521
{
	if(pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	::SetEvent(m_hEventArray[enEventSend]);
}

BOOL CSc::GetModified()
{
	if(m_pDoc->m_pScSkinDlg == NULL)
		return FALSE;

	CSC_DATA* pSC_DATA = ((CScSkinDlg*)m_pDoc->m_pScSkinDlg)->m_pSC_DATA;
	if(pSC_DATA == NULL)
		return FALSE;

	return pSC_DATA->m_bModified;
}