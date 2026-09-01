// Cv.cpp: implementation of the CCv class.
//

#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Cv.h"
#include "ByteArrayEx.h"
#include "RecordSetWrap.h"
#include "CvSkinDlg.h"

#define DEVICE m_port.m_strDevice
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// CCv
//
IMPLEMENT_DYNAMIC(CCv, CEquipment)

CCv::CCv(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex) : CEquipment(pDoc, pstrThreadNo, nIndex)
{
	m_enKind = CEquipment::enCV;
	m_pInfo = new CCvInfo(this);
	DEBUGER_ASSERT_VALID(m_pInfo != NULL);
	m_pDoc = pDoc;
	m_pRsw = NULL;
}
CCv::~CCv()
{
	m_pInfo->m_MapTrackInfo.RemoveAll();
	delete m_pInfo;
}


void CCv::SetVar(CRecordSetWrap* pRsw) //kdh20190521
{
	if(pRsw == NULL)
	{
		return;
	}
	this->m_pRsw = pRsw;
	::SetEvent(m_hEventArray[enEventSend]);
}

BOOL CCv::GetModified()
{
	if(m_pDoc->m_pCvSkinDlg == NULL)
		return FALSE;

	CTrackInfo* pTrackInfo = ((CCvSkinDlg*)m_pDoc->m_pCvSkinDlg)->m_pTrackInfo;
	if(pTrackInfo == NULL)
		return FALSE;
	
	BOOL bModified = pTrackInfo->m_pCV_DATA->m_bModified;
	//pTrackInfo->m_pCV_DATA->m_bModified = FALSE; 
	return bModified;
}

CString CCv::GetSelectQry() //kdh20190521
{
	CString strSql = _T("");

	strSql.Format(_T("SELECT CD.WH_TYP																										\n")
				  _T("       ,CD.TRACK_NO																									\n")
				  _T("       ,CD.MC_NO																										\n")
				  _T("       ,") + m_pDoc->NVL + _T("(CD.MC_NO_NM, ' ') AS MC_NO_NM															\n")
				  _T("       ,") + m_pDoc->NVL + _T("(CD.LUGG_NO_RD, '0') AS LUGG_NO_RD														\n")
				  _T("       ,") + m_pDoc->NVL + _T("(CD.DEST_POS_RD, '0') AS DEST_POS_RD													\n")
				  _T("       ,") + m_pDoc->NVL + _T("(CD.JOB_TYP_RD, '0') AS	JOB_TYP_RD													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.IS_TURN_RD, '0') AS IS_TURN_RD														\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.PULP_SENSOR_RD, '0') AS PULP_SENSOR_RD												\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.TR_PAUSE_RD, '0') AS TR_PAUSE_RD													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.TR_PAUSE_OD, '0') AS TR_PAUSE_OD													\n")   // [LGLS 2026-07-19] 아이템 일시정지 색 표시용
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.WAIT_SC_RET_JOB_RD, '0') AS WAIT_SC_RET_JOB_RD										\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.WAIT_TIME_RD, '0') AS WAIT_TIME_RD													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ERROR_CODE, '0') AS ERROR_CODE														\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.AUTO_MODE_RD, '0') AS AUTO_MODE_RD													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.STO_READY_RD, '0') AS STO_READY_RD													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.RET_READY_RD, '0') AS RET_READY_RD													\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.STOHS_READY_RD, '0') AS STOHS_READY_RD												\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.RETHS_READY_RD, '0') AS RETHS_READY_RD												\n")
				  // [LGLS 2026-08-22] RGV 핸드셰이크 2종 - 조회에서 빠져 있어 대화상자가 항상 OFF 였다
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.RTV_DEPARTHS_READY_RD, '0') AS RTV_DEPARTHS_READY_RD						\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.RTV_ARRIVEHS_READY_RD, '0') AS RTV_ARRIVEHS_READY_RD						\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.SENSOR0_DATA_RD, '0') AS SENSOR0_DATA_RD											\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.SENSOR1_DATA_RD, '0') AS SENSOR1_DATA_RD											\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.SENSOR2_DATA_RD, '0') AS SENSOR2_DATA_RD											\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.DELETE_TRACK_RD, '0') AS DELETE_TRACK_RD											\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.A_TURN_YN, '0') AS A_TURN_YN														\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.B_TURN_YN, '0') AS B_TURN_YN														\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.REMOTE_CONTROL, '0') AS REMOTE_CONTROL												\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.STOCK_MODE, '0') AS STOCK_MODE														\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.SUSPEND, '0') AS SUSPEND   														\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ITEM_NO, ' ') AS ITEM_NO															\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.DRIV_PAPER_POS, '0') AS DRIV_PAPER_POS												 \n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ELEV_ASC_ERR, '0') AS ELEV_ASC_ERR													 \n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ELEV_DESC_ERR, '0') AS ELEV_DESC_ERR												\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.CLAMP_FORWARD_ERR, '0') AS CLAMP_FORWARD_ERR										\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.CLAMP_BACKWARD_ERR, '0') AS CLAMP_BACKWARD_ERR										 \n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.DRIV_FORWARD_ERR, '0') AS DRIV_FORWARD_ERR											\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.DRIV_BACKWARD_ERR, '0') AS DRIV_BACKWARD_ERR										\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.PAPER_BLOCK_SENSOR1, '0') AS PAPER_BLOCK_SENSOR1									\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.PAPER_BLOCK_SENSOR2, '0') AS PAPER_BLOCK_SENSOR2									\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.PAPER_BLOCK_SENSOR3, '0') AS PAPER_BLOCK_SENSOR3									\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.PAPER_BLOCK_SENSOR4, '0') AS PAPER_BLOCK_SENSOR4	 								\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.PAPER_FULL_SENSOR, '0') AS PAPER_FULL_SENSOR										\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.DRIV_FORWARD_POS, '0') AS DRIV_FORWARD_POS											\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.DRIV_BACKWARD_POS, '0') AS DRIV_BACKWARD_POS										\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.CRUSH_PAPER_SENSOR, '0') AS CRUSH_PAPER_SENSOR										\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.CLAMP_FORWARD_SENSOR, '0') AS CLAMP_FORWARD_SENSOR									\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.REV_ELEV_POS_DATA_ASC, '0') AS REV_ELEV_POS_DATA_ASC								\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.REV_ELEV_POS_DATA_1, '0') AS REV_ELEV_POS_DATA_1									\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.REV_ELEV_POS_DATA_2, '0') AS REV_ELEV_POS_DATA_2									\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.REV_DRIV_POS_DATA_PICKUP, '0') AS REV_DRIV_POS_DATA_PICKUP							\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.REV_DRIV_POS_DATA_REVERSE, '0') AS REV_DRIV_POS_DATA_REVERSE						\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.REV_SENSOR_DATA_HEIGHT, '0') AS REV_SENSOR_DATA_HEIGHT								\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ROLL_ELEV_POS_DATA_ASC, '0') AS ROLL_ELEV_POS_DATA_ASC								 \n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ROLL_ELEV_POS_DATA_PICKUP, '0') AS ROLL_ELEV_POS_DATA_PICKUP						\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ROLL_ELEV_POS_DATA_ROLLING, '0') AS ROLL_ELEV_POS_DATA_ROLLING						\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ROLL_CLAMP_POS_DATA_BACKWARD, '0') AS ROLL_CLAMP_POS_DATA_BACKWARD					 \n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ROLL_CLAMP_POS_DATA_FORWARD, '0') AS ROLL_CLAMP_POS_DATA_FORWARD					\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.ROLL_SENSOR_DATA_HEIGHT, '0') AS ROLL_SENSOR_DATA_HEIGHT							\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.SC_PLT_JOB_TYP, '0') AS SC_PLT_JOB_TYP												\n")
				  _T("	     ,") + m_pDoc->NVL + _T("(CD.CLAMP_BACKWARD_SENSOR, '0') AS CLAMP_BACKWARD_SENSOR								\n")
				  _T("	     ,CASE WHEN DATEDIFF(SECOND, EM.UPD_DT, GETDATE()) > 120 THEN 121 ELSE DATEDIFF(SECOND, EM.UPD_DT, GETDATE()) END AS EQP_TIME			\n")
				  _T("	     ,EM.CONNECTED_YN															  									\n")
				  _T(" FROM CV_DATA CD 																										\n")
				  _T(" INNER JOIN (SELECT PLC_NO, CONNECTED_YN, UPD_DT FROM EQP_MST WHERE WH_TYP = '%02s' AND EQP_TYP = 'CV') EM			\n")
				  _T("         ON CD.PLC_NO = EM.PLC_NO																						\n")
				  _T("WHERE CD.WH_TYP = '%02s'																								\n")
				  _T("  AND CD.PLC_NO IN (%s)																								\n")
				  _T("ORDER BY TRACK_NO																										\n"),m_WH_TYP, m_WH_TYP, m_strInPlc);
	
	return strSql;
}
void CCv::CommandProc()
{

}
void CCv::AutoRunProc()
{
	if(m_pInfo == NULL)
	{
		return;
	}

	if(m_pRsw == NULL)
	{			
		return;		
	}

	if(m_pRsw->m_pRecordSet == NULL){ return;}

	BOOL bManualModify = FALSE;
	if (m_pInfo->m_bManualModified == TRUE)
	{
		m_pInfo->m_bManualModified = FALSE;
		bManualModify = TRUE;
	}

	BOOL bSeach = false;
	int nTrackCnt = (m_pRsw != NULL && m_pRsw->m_pRecordSet != NULL) ? m_pRsw->m_pRecordSet->RecordCount : 0; 
	if (m_pRsw != NULL)
		m_pRsw->MoveFirst(); 


	for(int nIdxFor = 0; nIdxFor < nTrackCnt; nIdxFor++)
	{
		CString strTRACK_NO = m_pRsw->GetItem( _T("MC_NO"));
		//CString strTRACK_NO = m_pRsw->GetItem( _T("MC_NO"));
		CTrackInfo* pTrackInfo = m_pInfo->m_MapTrackInfo[strTRACK_NO];

		if(pTrackInfo == NULL)
		{	m_pRsw->MoveNext();	continue;	}
		CCV_DATA* pCV_DATA = pTrackInfo->m_pCV_DATA;
		if(pCV_DATA == NULL){	m_pRsw->MoveNext(); continue;  }
		

		if (strTRACK_NO == "175")
			int a=0;
		

		pCV_DATA->SetLUGG_NO_RD(m_pRsw->GetItem( _T("LUGG_NO_RD")));
		pCV_DATA->SetDEST_POS_RD(m_pRsw->GetItem( _T("DEST_POS_RD")));
		pCV_DATA->SetJOB_TYP_RD(m_pRsw->GetItem( _T("JOB_TYP_RD")));
		pCV_DATA->SetPULP_SENSOR_RD(m_pRsw->GetItem( _T("PULP_SENSOR_RD")));
		pCV_DATA->SetAUTO_MODE_RD(m_pRsw->GetItem( _T("AUTO_MODE_RD")));
		pCV_DATA->SetSTO_READY_RD(m_pRsw->GetItem( _T("STO_READY_RD")));
		pCV_DATA->SetRET_READY_RD(m_pRsw->GetItem( _T("RET_READY_RD")));
		pCV_DATA->SetSTOHS_READY_RD(m_pRsw->GetItem( _T("STOHS_READY_RD")));
		pCV_DATA->SetRETHS_READY_RD( m_pRsw->GetItem( _T("RETHS_READY_RD")));
		// [LGLS 2026-08-22] RGV 출발/도착 HS - 대입이 빠져 있어 CV 상태창 표시가 안 됐다
		pCV_DATA->SetRTV_DEPARTHS_READY_RD(m_pRsw->GetItem( _T("RTV_DEPARTHS_READY_RD")));
		pCV_DATA->SetRTV_ARRIVEHS_READY_RD(m_pRsw->GetItem( _T("RTV_ARRIVEHS_READY_RD")));
		pCV_DATA->SetWAIT_TIME_RD(m_pRsw->GetItem(_T("WAIT_TIME_RD")));
		pCV_DATA->SetSENSOR0_DATA_RD(m_pRsw->GetItem( _T("SENSOR0_DATA_RD")));
		pCV_DATA->SetSENSOR1_DATA_RD(m_pRsw->GetItem( _T("SENSOR1_DATA_RD")));
		pCV_DATA->SetSENSOR2_DATA_RD(m_pRsw->GetItem( _T("SENSOR2_DATA_RD")));
		pCV_DATA->SetERROR_CODE(m_pRsw->GetItem( _T("ERROR_CODE")));
		pCV_DATA->SetSUSPEND(m_pRsw->GetItem(_T("SUSPEND")));
		pCV_DATA->SetMC_NO(m_pRsw->GetItem(_T("MC_NO")));
		pCV_DATA->SetMC_NO_NM(m_pRsw->GetItem(_T("MC_NO_NM")));
		pCV_DATA->SetWAIT_SC_RET_JOB_RD(m_pRsw->GetItem(_T("WAIT_SC_RET_JOB_RD")));
		pCV_DATA->SetTR_PAUSE_RD(m_pRsw->GetItem( _T("TR_PAUSE_RD")));
		pCV_DATA->SetTR_PAUSE_OD(m_pRsw->GetItem( _T("TR_PAUSE_OD")));   // [LGLS 2026-07-19]
		pCV_DATA->SetEQP_TIME(m_pRsw->GetItem( _T("EQP_TIME")));
		pCV_DATA->SetEQP_CONNECTED_YN(m_pRsw->GetItem( _T("CONNECTED_YN")));
		pCV_DATA->SetITEM_NO(m_pRsw->GetItem( _T("ITEM_NO")));

		// [LGLS 2026-08-01] 입출고 겸용대(22번) 방향 표시 : 레이아웃의 "입출고대" 라벨 바로 아래 칸에
		//   현재 설비 방향을 "입고 모드"/"출고 모드" 로 보여준다(레이아웃 컨트롤 id=90000122).
		//   ※CV_DATA.STOCK_MODE 에는 PLC 방향 워드의 원시 워드값이 들어와 ASCII 코드(48='0', 49='1')로 보인다.
		//   [LGLS 2026-08-22] S/C #1 통로 C/V #2 도 입출고 겸용(방향전환형)이라 같은 표시를 둔다.
		//     C/V #2 는 트랙 103·104 를 갖고 방향 워드는 설비당 1개이므로 대표로 103 을 쓴다.
		//     레이아웃 컨트롤 id=90000103 (CV#2 라벨 바로 위 칸).
		if (m_pRsw->GetItem(_T("MC_NO")) == _T("122") || m_pRsw->GetItem(_T("MC_NO")) == _T("103"))
		{
			CString strMode = m_pRsw->GetItem(_T("STOCK_MODE"));
			strMode.Trim();
			BOOL bOut = (strMode == _T("1") || strMode == _T("49"));
			CString strCid = (m_pRsw->GetItem(_T("MC_NO")) == _T("103")) ? _T("90000103") : _T("90000122");
			CDciControl* pDirCtrl = m_pDoc->GetDciControl_FindAllLayout(strCid);
			if (pDirCtrl != NULL)
			{
				// [LGLS 2026-08-22] C/V #2 자리는 RTV 레일(5열)과 랙(8열) 사이 1.5칸뿐이라 짧게 쓴다
			CString strTxt;
			if (m_pRsw->GetItem(_T("MC_NO")) == _T("103"))
				strTxt = bOut ? _T("출고") : _T("입고");
			else
				strTxt = bOut ? _T("출고 모드") : _T("입고 모드");
				if (pDirCtrl->m_strText != strTxt)
				{
					pDirCtrl->m_strText = strTxt;
					pDirCtrl->m_clrFgColor = bOut ? RGB(200,0,0) : RGB(0,0,200);
					pDirCtrl->InvalidateControl(m_pDoc->m_hWndView, TRUE);
				}
			}
		}

		pCV_DATA->SetDRIV_PAPER_POS(m_pRsw->GetItem( _T("DRIV_PAPER_POS")));
		pCV_DATA->SetELEV_ASC_ERR(m_pRsw->GetItem( _T("ELEV_ASC_ERR")));
		pCV_DATA->SetELEV_DESC_ERR(m_pRsw->GetItem( _T("ELEV_DESC_ERR")));
		pCV_DATA->SetCLAMP_FORWARD_ERR(m_pRsw->GetItem( _T("CLAMP_FORWARD_ERR")));
		pCV_DATA->SetCLAMP_BACKWARD_ERR(m_pRsw->GetItem( _T("CLAMP_BACKWARD_ERR")));
		pCV_DATA->SetDRIV_FORWARD_ERR(m_pRsw->GetItem( _T("DRIV_FORWARD_ERR")));
		pCV_DATA->SetDRIV_BACKWARD_ERR(m_pRsw->GetItem( _T("DRIV_BACKWARD_ERR")));
		pCV_DATA->SetPAPER_BLOCK_SENSOR1(m_pRsw->GetItem( _T("PAPER_BLOCK_SENSOR1")));
		pCV_DATA->SetPAPER_BLOCK_SENSOR2(m_pRsw->GetItem( _T("PAPER_BLOCK_SENSOR2")));
		pCV_DATA->SetPAPER_BLOCK_SENSOR3(m_pRsw->GetItem( _T("PAPER_BLOCK_SENSOR3")));
		pCV_DATA->SetPAPER_BLOCK_SENSOR4(m_pRsw->GetItem( _T("PAPER_BLOCK_SENSOR4")));
		pCV_DATA->SetPAPER_FULL_SENSOR(m_pRsw->GetItem( _T("PAPER_FULL_SENSOR")));
		pCV_DATA->SetDRIV_FORWARD_POS(m_pRsw->GetItem( _T("DRIV_FORWARD_POS")));
		pCV_DATA->SetDRIV_BACKWARD_POS(m_pRsw->GetItem( _T("DRIV_BACKWARD_POS")));
		pCV_DATA->SetCRUSH_PAPER_SENSOR(m_pRsw->GetItem( _T("CRUSH_PAPER_SENSOR")));
		pCV_DATA->SetCLAMP_FORWARD_SENSOR(m_pRsw->GetItem( _T("CLAMP_FORWARD_SENSOR")));
		pCV_DATA->SetREV_ELEV_POS_DATA_ASC(m_pRsw->GetItem( _T("REV_ELEV_POS_DATA_ASC")));
		pCV_DATA->SetREV_ELEV_POS_DATA_1(m_pRsw->GetItem( _T("REV_ELEV_POS_DATA_1")));
		pCV_DATA->SetREV_ELEV_POS_DATA_2(m_pRsw->GetItem( _T("REV_ELEV_POS_DATA_2")));
		pCV_DATA->SetREV_DRIV_POS_DATA_PICKUP(m_pRsw->GetItem( _T("REV_DRIV_POS_DATA_PICKUP")));
		pCV_DATA->SetREV_DRIV_POS_DATA_REVERSE(m_pRsw->GetItem( _T("REV_DRIV_POS_DATA_REVERSE")));
		pCV_DATA->SetREV_SENSOR_DATA_HEIGHT(m_pRsw->GetItem( _T("REV_SENSOR_DATA_HEIGHT")));
		pCV_DATA->SetROLL_ELEV_POS_DATA_ASC(m_pRsw->GetItem( _T("ROLL_ELEV_POS_DATA_ASC")));
		pCV_DATA->SetROLL_ELEV_POS_DATA_PICKUP(m_pRsw->GetItem( _T("ROLL_ELEV_POS_DATA_PICKUP")));
		pCV_DATA->SetROLL_ELEV_POS_DATA_ROLLING(m_pRsw->GetItem( _T("ROLL_ELEV_POS_DATA_ROLLING")));
		pCV_DATA->SetROLL_CLAMP_POS_DATA_BACKWARD(m_pRsw->GetItem( _T("ROLL_CLAMP_POS_DATA_BACKWARD")));
		pCV_DATA->SetROLL_CLAMP_POS_DATA_FORWARD(m_pRsw->GetItem( _T("ROLL_CLAMP_POS_DATA_FORWARD")));
		pCV_DATA->SetROLL_SENSOR_DATA_HEIGHT(m_pRsw->GetItem( _T("ROLL_SENSOR_DATA_HEIGHT")));

		pCV_DATA->SetSC_PLT_JOB_TYP(m_pRsw->GetItem( _T("SC_PLT_JOB_TYP")));
		pCV_DATA->SetCLAMP_BACKWARD_SENSOR(m_pRsw->GetItem( _T("CLAMP_BACKWARD_SENSOR")));

		if (pCV_DATA->V_MC_NO == _T("104"))
		{
			if(m_strBypassChkYn != m_pDoc->m_strBypassChkYn)
			{
				m_strBypassChkYn = m_pDoc->m_strBypassChkYn;
				pCV_DATA->m_bModified = TRUE;
			}
		}

		// 강제 Control Update
		if (bManualModify == TRUE)
		{
			pCV_DATA->m_bModified = TRUE;
			//bManualModify = FALSE;

			if (strTRACK_NO == "175")
				int a=0;

		}

		pTrackInfo->InvokeControl(pTrackInfo->m_pTrackCtrl);
		m_pRsw->MoveNext();
	}

	if (bManualModify == TRUE)
		bManualModify = FALSE;
}


