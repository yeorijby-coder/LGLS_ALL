// ScInfo.h: interface for the CScInfo class.
//

#pragma once

#include "Info.h"
#include "DciRvCtrl.h"
#include "SC_DATA.h"

class CEquipment;

class CScInfo : public CInfo  
{
	DECLARE_SERIAL(CScInfo)
		
public:
	CScInfo() {}
	CScInfo(CEquipment* pEquipment);
	virtual ~CScInfo();

public:
	enum EN_TYPE { enL2R, enR2L, enT2B, enB2T, enTypeSize };
	enum EN_FORK_TYPE { enSingle, enTwin, enDouble, enForkTypeSize };

	enum EN_RC_MODE { enOff=0, enOnline=1, enRemote=2 };																			// D 95
	enum EN_SC_MODE { enAuto=1, enManual=0 };																						// D100-L
	enum EN_PROD_LOAD { enProdEmpty=0, enProdExistFork1=1, enProdExistFork2=2, enProdExistFork12=3 };								// D100-H
	enum EN_SC_STATUS { enIdle=0, enWaiting=1, enRunning=2, enError=4  };															// D101
	enum EN_FORK_POS { enForkPosCenter= 0 , enForkPosBank1=1, enForkPosBank2=2 };													// D104
	enum EN_FORK_STATUS { enForkReady=0, enForkDualStoreError=1, enForkEmptyRetrieveError=2 };																			// D106
	enum EN_RC_STATUS { enStop=0, enActive=1 };																						// D109
	enum EN_JOB_STATUS { enMoving=0, en1ForkComplete=1, en2ForkComplete=2, enComplete=3  };									// D110
	enum EN_CMD_TYPE { enCmdNone='0', enStore='1', enRetrieve='2', enHs2Hs='3', enRack2Rack='4', enHomeReturn='5', enMove='6', enDualStore='7' };	// D111
	enum EN_ROUTINE { enRoutineStore, enRoutineRetrieve, enRoutineRackToRack };
	enum EN_FLOOR_POS { enFloor0, enFloor1, enFloor2, enFloor3, enFloorSize };														// enFloor0 : 지하  


public:
	CDciRvCtrl* m_pControl;
	CDciRvCtrl* m_pControl2;
	CDciRvCtrl* m_pControl3;
	CDciRvCtrl* m_pControl4;
	CDciRvCtrl* m_pControl5;
	
public:
	CMap<CString, LPCTSTR, CSC_DATA*, CSC_DATA*> m_MapSC_DATA;
	CSC_DATA* m_pSC_DATA;

public:
	void InvokeControl(CSC_DATA*	pSC_DATA);
	virtual void InvokeControl();
	// [LGLS 2026-08-22] 창고 모니터링 보기(작업번호/호기/제품정보)를 크레인에도 적용
	void CalcScText(CSC_DATA* pData, CString& strOut, COLORREF& clrOut);
	void ApplyScTextMode(CSC_DATA* pData, CDciRvCtrl* c1, CDciRvCtrl* c2, CDciRvCtrl* c3, CDciRvCtrl* c4, CDciRvCtrl* c5);
	CString m_strScProdLugg;	// 제품정보 조회 캐시 키(작업번호)
	CString m_strScProdVal;		// 제품정보 조회 캐시 값
	COLORREF GetForkColor2();
	COLORREF GetForkColor1();
	COLORREF GetForkColor2(CSC_DATA* pSC_DATA);
	COLORREF GetForkColor1(CSC_DATA* pSC_DATA);
	COLORREF GetRailColor(CSC_DATA* pSC_DATA);
	
	
	COLORREF GetForkColor();
	COLORREF GetRailColor();

	COLORREF GetPostColor();

protected:
	virtual void Serialize(CArchive& ar);

public:
	CSC_DATA* CreateSC_DATA( CString pstrEQP_NO );
	CString GetStringPLC_NO();
	CString GetStringPLC_NO(int pnPLC_NO);
	CString GetStringEQP_NO(CString pstrEQP_NO);
};
