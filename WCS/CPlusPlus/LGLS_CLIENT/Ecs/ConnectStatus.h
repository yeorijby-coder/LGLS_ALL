#pragma once

#include "Equipment.h"

#include "AdoDB.h"
#include "URMDBAccess.h"
#include <afxmt.h>

class CConnectStatus
{
	//DECLARE_DYNAMIC(CConnectStatus)

public:
	CConnectStatus(CEcsDoc* pDoc);
	~CConnectStatus(void);

public:
	CEcsDoc* m_pDoc;
	CRecordSetWrap* m_pRsw;

public:
	//CMap<CString, LPCTSTR, CConnectStatus*, CConnectStatus*> m_MapCONNECT;
	void SetVar(CRecordSetWrap* pRsw);
	CString GetSelectQry();
	BOOL m_bModified;

	CString V_EQP_TIME1;
	CString V_EQP_TIME2;
	CString V_EQP_COLOR1;
	CString V_EQP_COLOR2;

	// [LGLS] 3번 슬롯 = SCH(IO_TASK 스케줄러) 상태.
	//   IO_TASK 는 원격지 서버에서 돌 수 있어 Client 가 프로세스를 직접 볼 수 없다.
	//   → IO_TASK 가 EQP_MST(EQP_TYP='SCH') 에 하트비트(UPD_DT + CONNECTED_YN)를 쓰고 여기서 그 신선도를 읽는다.
	//   V_EQP_TIME3 = 하트비트 경과초, V_EQP_CONN3 = 'Y'(정상)/'N'(오류)
	CString V_EQP_TIME3;
	CString V_EQP_CONN3;
	CString V_EQP_COLOR3;

	void SetEQP_TIME1(CString V_EQP_TIME1);
	void SetEQP_TIME2(CString V_EQP_TIME2);
	void SetEQP_COLOR1(CString V_EQP_COLOR1);
	void SetEQP_COLOR2(CString V_EQP_COLOR2);
	void SetEQP_TIME3(CString V_EQP_TIME3);
	void SetEQP_CONN3(CString V_EQP_CONN3);
	void SetEQP_COLOR3(CString V_EQP_COLOR3);

	CString m_WH_TYP;
	CString m_HOST_NUM;

	// [thread-safe] worker(SetVar) vs UI(OnUpdate) CString buffer race guard
	CCriticalSection m_csVar;
	CString GetTime2Safe();
	CString GetColor2Safe();
	CString GetTime3Safe();
	CString GetConn3Safe();
	CString GetColor3Safe();
};

