// RtvInfo.h: interface for the CRtvInfo class.
//

#pragma once

#include "Info.h"
#include "DciRvCtrl.h"
#include "RTV_DATA.h"

typedef struct RtvPosInfo
{
	int  m_nDurgPos;	// 실제 포지션
 	int  m_nEcsPos;		// 도착지 트랙의 배열 인덱스
} RtvPosInfo;

typedef struct RtvJobInfo
{
	int  m_nLuggNum;			// 작업번호
	BYTE m_ucDepartPos;			// 출발지 작업대
	BYTE m_ucArrivePos;			// 도착지 작업대
	
	BOOL m_bInvoke;
	int  m_nInternalLuggNum;
	int	 m_nInternalJobType;
 	int  m_nDepartTrackNum;		// 출발지 트랙의 배열 인덱스
 	int  m_nArriveTrackNum;		// 도착지 트랙의 배열 인덱스

// 	int  m_nLuggNum;			// 작업번호
// 	int	 m_nInternalLuggNum;
// 	int  m_nInternalJobType;
// 	BOOL m_bInvoke;				// SC 작업지시 검사플래그
// 	CString m_strDepartInfo;	// 출발지(4x4) {열, 행, 단, HS}
// 	CString m_strArriveInfo;	// 도착지(4x4) {열, 행, 단, HS}
// 	int	 m_nDepartTrackNum;
// 	int	 m_nArriveTrackNum;
} RtvJobInfo;

class CEquipment;


// CRtvInfo
//
class CRtvInfo : public CInfo  
{
	DECLARE_DYNAMIC(CRtvInfo)
		
public:
	CRtvInfo() {}
	CRtvInfo(CEquipment* pEquipment);
	virtual ~CRtvInfo();


public:
	//enum EN_PROD_LOAD { enProdEmpty = 0, enProdExist = 1 };
	enum EN_PROD_LOAD { enProdEmpty=0, enProdExistFork1=1, enProdExistFork2=2, enProdExistFork12=3 };																							// D100-H
	enum EN_MODE { enModeNone = 0, enModeAuto = 1, enModeManual = 2 };
	enum EN_STATUS { enStatusWait = 0, enStatusIdle = 1, enStatusRunning = 2, enStatusError = 4 };
	enum EN_ACTIVE { enStop = 0, enActive = 1 };
	enum EN_COMPLETE { enWorking = 0, en1ForkComplete = 1, en2ForkComplete = 2, enComplete = 3 };
	enum EN_TYPE { enTypeNone = 0, enTypeHs2Hs = 3, enTypeMove = 4, enTypeReturnHome = 5, enTypeManual=6 };
	enum EN_ERROR { enErrorNone = 0, enErrorCommunication = 999 };
	enum EN_FORK { enForkOne = 0, enForkTwo = 1, enForkAll = 2};
	enum EN_ROUTINE { enRoutineStore, enRoutineRetrieve };

public:
	CDciRvCtrl* m_pControl;

public:
	CMap<CString, LPCTSTR, CRTV_DATA*, CRTV_DATA*> m_MapRTV_DATA;
	CRTV_DATA* m_pRTV_DATA;

public:
	void InvokeControl(CRTV_DATA* pRTV_DATA);
	virtual void InvokeControl();
	virtual void Serialize(CArchive& ar);

public:
	COLORREF GetForkColor();
	COLORREF GetForkColor1();
	COLORREF GetForkColor2();
	COLORREF GetRailColor();
	COLORREF GetPostColor();

	COLORREF GetForkColor1(CRTV_DATA* pRTV_DATA);
	COLORREF GetForkColor2(CRTV_DATA* pRTV_DATA);
	COLORREF GetRailColor(CRTV_DATA* pRTV_DATA);
	COLORREF GetPostColor(CRTV_DATA* pRTV_DATA);

	int GetRtvPosition();

	CRTV_DATA* CreateRTV_DATA( CString pstrEQP_NO );
	CString GetStringPLC_NO();
	CString GetStringPLC_NO(int pnPLC_NO);
	CString GetStringEQP_NO(CString pstrEQP_NO);
};
