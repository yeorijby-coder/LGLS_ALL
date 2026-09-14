
#include "StdAfx.h"
#include "stdafx.h"
#include "CollectDB.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Equipment.h"
#include "RecordSetWrap.h"

#define CRLF _T("\n")

extern "C" __declspec(dllimport) int  Usb_Qu_Getstate(); 

CCollectDB::CCollectDB(CEcsDoc* pDoc)
{
	m_bThreadDoWork = FALSE;
	m_pDoc = pDoc;
	m_pThread = NULL;
	m_dwThreadId = 0;
	m_pDB_ACCESS = NULL;
}


CCollectDB::~CCollectDB(void)
{
	// [LGLS 2026-09-10] 수집 스레드를 먼저 세운다.
	//   종전에는 스레드가 아직 m_pDB_ACCESS 를 쓰고 있는데 여기서 지웠다(use-after-free).
	StopDoWork();

	if(m_pDB_ACCESS != NULL){ delete m_pDB_ACCESS; }
	m_pDB_ACCESS = NULL;
}

BOOL CCollectDB::IsDB_POSSIBLE()
{
	if(m_pDB_ACCESS == NULL || m_pDB_ACCESS->m_pAdoDB == NULL)
	{
		if(m_pDB_ACCESS != NULL)
		{
			if(m_pDB_ACCESS->m_pAdoDB == NULL)
			{
				delete m_pDB_ACCESS->m_pAdoDB;
				m_pDB_ACCESS->m_pAdoDB = NULL;
			}
			delete m_pDB_ACCESS;
			m_pDB_ACCESS = NULL;
		}

		m_pDB_ACCESS = new CURMDBAccess(m_pDoc, new CAdoDB(m_pDoc));
		return m_pDB_ACCESS->m_pAdoDB->ConnectDB();
	}

	if(m_pDB_ACCESS->m_pAdoDB->m_bConnected == FALSE)
	{
		// [LGLS 2026-09-09] 해제한 뒤에 그 포인터로 멤버를 쓰고 있었다(use-after-free).
		//   DB 가 끊긴 순간 수집 스레드가 여기로 들어와 죽었다
		//   (크래시 리포트 Ecs20260904_065507.RPT : CCollectDB::IsDB_POSSIBLE+27F).
		CURMDBAccess* pAccess = m_pDB_ACCESS;
		CAdoDB*       pAdo    = m_pDB_ACCESS->m_pAdoDB;
		m_pDB_ACCESS = NULL;
		pAccess->m_pAdoDB = NULL;
		delete pAdo;		// CURMDBAccess 소멸자는 비어 있어 여기서 직접 지운다
		delete pAccess;
		return FALSE;
	}

	return m_pDB_ACCESS->m_pAdoDB->m_bConnected;
}

BOOL CCollectDB::StartDoWork()
{
	if(m_bThreadDoWork == TRUE)
	{
		return FALSE;
	}

	
	m_bThreadDoWork = TRUE;
	// [LGLS 2026-09-10] CWinThread 는 기본이 자동 삭제라, 스레드가 끝나면 m_pThread 가
	//   가리키는 객체가 사라진다(멈출 때 그 포인터를 쓰면 위험하다). 우리가 지운다.
	m_pThread = ::AfxBeginThread(DoWork, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if(m_pThread != NULL)
	{
		m_pThread->m_bAutoDelete = FALSE;
		m_dwThreadId = m_pThread->m_nThreadID;
		m_pThread->ResumeThread();
	}
	if(m_pThread == NULL)
	{
		m_bThreadDoWork = FALSE;
		return FALSE;
	}

	return IsAllive();
}

BOOL CCollectDB::IsAllive()
{
	return m_bThreadDoWork;
}

BOOL CCollectDB::StopDoWork()
{
	m_bThreadDoWork = FALSE;

	if(m_pThread == NULL)
		return TRUE;

	// [LGLS 2026-09-10] 작업 스레드가 자기 자신에 대해 부르기도 한다(m_bExit 일 때).
	//   그때는 깃발만 내리고 돌아간다. 기다리면 자기를 기다리는 꼴이 된다.
	if(::GetCurrentThreadId() == m_dwThreadId)
		return TRUE;

	// 종전에는 CWinThread* 를 HANDLE 자리에 넘겨(잘못된 핸들) 사실상 기다리지 않았다.
	HANDLE hThread = m_pThread->m_hThread;
	if(hThread != NULL)
		::WaitForSingleObject(hThread, 5000);

	delete m_pThread;
	m_pThread = NULL;
	m_dwThreadId = 0;
	return TRUE;
}

// [LGLS 2026-09-14] 수집 주기 계측 (Ecs.ini [MENU] UI_TRACE=1 일 때 10초마다 한 줄)
//   한 바퀴 = 전 설비 조회 + 통신상태 조회 + 쉬는 시간(COLLECT_INTERVAL_MS) 까지. 수집 스레드 하나에서만 부른다.
static LARGE_INTEGER s_liLglsFreq = { 0 };
static double s_dLglsSqlSum = 0, s_dLglsSqlMax = 0;
static int s_nLglsSqlCnt = 0;
static double LglsQpcMs(const LARGE_INTEGER& a, const LARGE_INTEGER& b)
{
	if (s_liLglsFreq.QuadPart == 0) ::QueryPerformanceFrequency(&s_liLglsFreq);
	return (b.QuadPart - a.QuadPart) * 1000.0 / (double)s_liLglsFreq.QuadPart;
}
static void LglsCollectCycle()
{
	static LARGE_INTEGER s_liPrev = { 0 };
	static int s_nCycles = 0;
	static double s_dCycSum = 0, s_dCycMax = 0;
	static DWORD s_dwStart = 0;
	LARGE_INTEGER liNow;
	::QueryPerformanceCounter(&liNow);
	if (s_liPrev.QuadPart != 0)
	{
		double d = LglsQpcMs(s_liPrev, liNow);
		s_nCycles++; s_dCycSum += d; if (d > s_dCycMax) s_dCycMax = d;
	}
	s_liPrev = liNow;
	if (s_dwStart == 0) s_dwStart = ::GetTickCount();
	if (::GetTickCount() - s_dwStart >= 10000 && s_nCycles > 0)
	{
		CLib::UiLog(_T("[COLLECT] 한 바퀴 %d 회 | 평균 %.0f ms 최대 %.0f ms | 설비 조회 %d 건 평균 %.1f ms 최대 %.1f ms"),
			s_nCycles, s_dCycSum / s_nCycles, s_dCycMax, s_nLglsSqlCnt,
			(s_nLglsSqlCnt > 0) ? s_dLglsSqlSum / s_nLglsSqlCnt : 0.0, s_dLglsSqlMax);
		s_nCycles = 0; s_dCycSum = s_dCycMax = 0; s_nLglsSqlCnt = 0; s_dLglsSqlSum = s_dLglsSqlMax = 0;
		s_dwStart = ::GetTickCount();
	}
}

UINT CCollectDB::DoWork(LPVOID pParm)
{
	CEquipment *pEquipment = NULL;
	CCollectDB* pThis = (CCollectDB*)pParm;
	while(pThis->m_bThreadDoWork)
	{			
		LglsCollectCycle();	// [LGLS 2026-09-14] 수집 주기 계측
		CEcsDoc* pDoc = pThis->m_pDoc;
		if(pDoc == NULL)
		{
			::Sleep(500);
			break;
		}
		int nEqpCount = pDoc->m_pEquipments.GetCount();
		for(int nIdxEqp = 0; nIdxEqp < nEqpCount; nIdxEqp++)
		{
			if(pDoc->m_bExit == true)
			{
				pThis->StopDoWork();
				break;
			}

			if(pThis->IsDB_POSSIBLE() == FALSE)
			{
				::Sleep(500);
				break;
			}
			pEquipment = pDoc->m_pEquipments[nIdxEqp];

			//설비들 값
			// [LGLS 2026-09-10] 종전에는 pEquipment 가 NULL 일 때도 그것을 넘겨 호출했다.
			//   Collect_EQUIPMENT 첫 줄이 pEquipment->GetSelectQry() 라 바로 죽는다.
			if(pEquipment != NULL && pEquipment->m_pRsw == NULL)
			{
				{
					LARGE_INTEGER liQ0, liQ1;	// [LGLS 2026-09-14] 설비 조회 시간 계측
					::QueryPerformanceCounter(&liQ0);
					pThis->Collect_EQUIPMENT(pEquipment);
					::QueryPerformanceCounter(&liQ1);
					double dQ = LglsQpcMs(liQ0, liQ1);
					s_nLglsSqlCnt++; s_dLglsSqlSum += dQ; if (dQ > s_dLglsSqlMax) s_dLglsSqlMax = dQ;
				}
				::Sleep(50); //추가
			}
		}	

		// [LGLS 2026-09-10] DB 가 끊기면 IsDB_POSSIBLE 이 접속 객체를 지우고 NULL 로 만든다.
		//   종전에는 그 상태로 아래 ConnectStatus 에 들어가 NULL 을 역참조했다
		//   (설비가 0대면 위 for 문이 아예 안 돌아 기동 직후에도 같은 일이 났다).
		if(pThis->IsDB_POSSIBLE() == FALSE)
		{
			::Sleep(500);
			continue;
		}

		if(pDoc->m_blConnectStatus == TRUE)
		{
			int HostCnt = 1; //2 (나중에 수정할 부분)
			for(int nIdxHost = 0; nIdxHost < HostCnt; nIdxHost++)
			{
				CString strHostNum = _T("HOST2");  //_T("HOST") + CConvert::ToString(nIdxHost+1); 
				pThis->ConnectStatus(pDoc->m_pConnectStatus, strHostNum);
				::Sleep(50);
				// [LGLS] SCH = IO_TASK heartbeat (EQP_MST EQP_TYP='SCH') -> V_EQP_TIME3 / V_EQP_CONN3
				pThis->ConnectStatus(pDoc->m_pConnectStatus, _T("SCH"));
				::Sleep(50); //추가
			}
		}
		// [LGLS 2026-09-08] 설비 수집 주기. 종전 고정 1000ms 라 화면이 실 데이터보다 1초쯤 늦었다.
		//   설비 통신(VehThread)은 0.3초 주기로 DB 를 갱신하는데 화면이 그것을 1초마다 읽어
		//   전체가 한 박자 느리게 보였다. Ecs.ini [ETC] COLLECT_INTERVAL_MS 로 조정한다.
		{
			int nItv = ::GetPrivateProfileInt(_T("ETC"), _T("COLLECT_INTERVAL_MS"), 300, ECS_INI_FILE);
			if (nItv < 100)  nItv = 100;      // 너무 낮추면 DB 부하만 는다
			if (nItv > 5000) nItv = 5000;
			::Sleep(nItv);
		}
	}
	pThis->m_bThreadDoWork = FALSE;
	return 0;
}


void CCollectDB::Collect_EQUIPMENT(CEquipment* pEquipment)
{
	// [LGLS 2026-09-10] 부르는 쪽을 고쳤지만 여기서도 막는다.
	if(pEquipment == NULL || m_pDB_ACCESS == NULL || m_pDB_ACCESS->m_pAdoDB == NULL)
		return;

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strErrMsg = _T("");

	strSql = pEquipment->GetSelectQry();
	if(strSql == _T(""))
	{
		::Sleep(500);
		return;
	}

	_RecordsetPtr pRsptr = m_pDB_ACCESS->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strErrMsg);
	if(nRowCnt <= 0)
	{
		return;
	}
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	pEquipment->SetVar(pRsw);

	//::Sleep(50); //추가


	//@@@@
	//delete pRsw;
}
void CCollectDB::ConnectStatus(CConnectStatus* pConnectStatus, CString strHostNum)
{
	// [LGLS 2026-09-10] 문서가 아직 안 만들었거나 이미 닫혔으면 NULL 이다.
	if(pConnectStatus == NULL)
		return;

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strErrMsg = _T("");

	pConnectStatus->m_HOST_NUM = strHostNum;
	strSql = pConnectStatus->GetSelectQry();

	if(strSql == _T(""))
	{
		::Sleep(500);
		return;
	}
	// [LGLS 2026-09-10] 이 검사가 주석 처리돼 있었다. DB 가 끊긴 직후 여기로 들어오면
	//   m_pDB_ACCESS 가 NULL 이라 그대로 죽는다(크래시 Ecs20260904_065507.RPT 계열).
	if(m_pDB_ACCESS == NULL || m_pDB_ACCESS->m_pAdoDB == NULL)
		return;

	_RecordsetPtr pRsptr = m_pDB_ACCESS->m_pAdoDB->SelectSqlForThread_RecordSet(strSql, nRowCnt, strErrMsg);

	if(nRowCnt <= 0)
	{
		return;
	}

	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	pConnectStatus->SetVar(pRsw);
	delete pConnectStatus->m_pRsw; 
	pConnectStatus->m_pRsw = NULL;
}

