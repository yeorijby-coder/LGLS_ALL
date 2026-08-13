// LgvVehicle.h: interface for the CLgvVehicle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LGVVEHICLE_H__23D2DFD7_D5FF_4394_BF49_18D4206A3290__INCLUDED_)
#define AFX_LGVVEHICLE_H__23D2DFD7_D5FF_4394_BF49_18D4206A3290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DciRvCtrl.h"
//#include "EcsDoc.h"


//class CEcsDoc;
class CCv;
class CCvInfo;
class CEquipment;
class CLgvVehicle : public CObject
{
public:
	CLgvVehicle(int nVehicleID);
	virtual ~CLgvVehicle();

public:
	enum EN_VEHICLE_MODE { enModeManual, enModeLocal, enModeAuto};
	enum EN_VEHICLE_STATUS { enWait, enMove, enLoading, enUnloading, enCarError };
	enum EN_LOAD_STATUS { enUnload = '0', enLoad = '1' };

public:
	CDciRvCtrl*	m_pControl;

public:
	int  m_nVehicleID;			
	int  m_nLuggNum;			//		
	int  m_nErrorCode;			
	int  m_nCurPosition;		
	BYTE m_ucVehicleMode;		//
	BYTE m_ucVehicleStatus;		
	BYTE m_ucLoadStatus;		
	BOOL m_bModified;
	BOOL m_bInvoke;

public:
	BOOL	m_bFirstDoorCheck;

public:
	CString GetAgvString();
	CString GetModeString();
	CString GetVehicleStatusString();
//	CString GetBatteryChargeString();
	CString GetLoadStatusString();
//	CString GetActivityString();
	CString GetErrorString();
	CString GetXmlString();

public:
	void SetMode(BYTE ucMode);													// RGV 동작모드		- {0:Manual, 1:Local, 2:Auto}					
	void SetStatus(BYTE ucRun);													// RGV 동작상태		- {0:대기, 1:이동, 2:로딩, 3:언로딩, 4:에러}	
	void SetProdLoad(BYTE ucContain);											// RGV 화물감지상태 - {0:화물무, 1:화물유}						 	
	void SetHorizontalPos(int nPos);											// RGV 위치
	void SetErrorCode(int nECode, CCvInfo* pCvInfo = NULL, int nLgvNum = 0);	// RGV 에러코드
	void SetLuggNum(int nLuggNo);												// RGV 작업번호
	void SetLampInfo(BOOL bOn, CCvInfo* pCvInfo, int nRgvcNum);

public:
	BOOL IsProductExist() { return (m_ucLoadStatus == enLoad); }
	BOOL IsErrorStatus() { return ((m_ucVehicleStatus == enCarError) || (m_nErrorCode != 0)); }
	BOOL IsAutoMode() { return (m_ucVehicleMode == enModeAuto); }
	BOOL IsInvoked() { return (m_bInvoke == TRUE); }
	
public:
	void Serialize(CArchive& ar);
};

class CLgvVehicles : public CArray<CLgvVehicle*, CLgvVehicle*>
{
public:
	CLgvVehicles();
	virtual ~CLgvVehicles();

public:
	CLgvVehicle* Find(int nVehicleID);
	BOOL IsAllRgvStop();
	BOOL IsAllRgvRestart();
	CString GetXmlString();
	BOOL IsIdleJob(int nLuggNum);
	BOOL IsErrorStatus();
};

#endif // !defined(AFX_LGVVEHICLE_H__23D2DFD7_D5FF_4394_BF49_18D4206A3290__INCLUDED_)
