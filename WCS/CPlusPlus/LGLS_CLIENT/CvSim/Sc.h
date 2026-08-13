// Sc.h: interface for the CSc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SC_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_)
#define AFX_SC_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Equipment.h"
#include "ScSocket.h"
#include "MelsecEthernetSk.h"
#include "ScInfo.h"

class CSc : public CEquipment  
{
	DECLARE_DYNAMIC(CSc)

public:
	CSc(CEcsDoc* pDoc, int nIndex, int nNumber, LPCTSTR lpszDevice);
	virtual ~CSc();

public:
	CScInfo* m_pInfo;
	BOOL m_bReset;
	virtual CInfo* GetInfo() { return m_pInfo; }

public:
	virtual void MulticastInfo(CMonitorServer* pMonitorSv);
	virtual BOOL IsIdleJob(int nLuggNum) { 
		return ((m_pInfo->m_ScInfo[0].m_nInternalLuggNum == 0) || (m_pInfo->m_ScInfo[0].m_nInternalLuggNum != nLuggNum)); 
	}

protected:
	virtual void AutoRunProc(int nConnNum);
	virtual void CommandProc();
	virtual CInterfaceSk* CreateSocket(int nIndex) { return new CMelsecEthernetSk(this, nIndex); }
	//virtual CInterfaceSk1* CreateSocket1() { return new CMelsecEthernetSk1(this); }
	//virtual CInterfaceSk2* CreateSocket2() { return new CMelsecEthernetSk2(this); }
	//virtual CInterfaceSk3* CreateSocket3() { return new CMelsecEthernetSk3(this); }
	//virtual CInterfaceSk4* CreateSocket4() { return new CMelsecEthernetSk4(this); }
	//virtual CInterfaceSk5* CreateSocket5() { return new CMelsecEthernetSk5(this); }
	//virtual CInterfaceSk6* CreateSocket6() { return new CMelsecEthernetSk6(this); }
	//virtual CInterfaceSk7* CreateSocket7() { return new CMelsecEthernetSk7(this); }
	//virtual CInterfaceSk8* CreateSocket8() { return new CMelsecEthernetSk8(this); }
	//virtual CInterfaceSk9* CreateSocket9() { return new CMelsecEthernetSk9(this); }
	//virtual CInterfaceSk10* CreateSocket10() { return new CMelsecEthernetSk10(this); }
	//virtual CInterfaceSk11* CreateSocket11() { return new CMelsecEthernetSk11(this); }
	//virtual CInterfaceSk12* CreateSocket12() { return new CMelsecEthernetSk12(this); }
	//virtual CInterfaceSk13* CreateSocket13() { return new CMelsecEthernetSk13(this); }
	//virtual CInterfaceSk14* CreateSocket14() { return new CMelsecEthernetSk14(this); }
	//virtual CInterfaceSk15* CreateSocket15() { return new CMelsecEthernetSk15(this); }
	//virtual CInterfaceSk16* CreateSocket16() { return new CMelsecEthernetSk16(this); }
	//virtual CInterfaceSk17* CreateSocket17() { return new CMelsecEthernetSk17(this); }
	//virtual CInterfaceSk18* CreateSocket18() { return new CMelsecEthernetSk18(this); }
protected:
	void InvokeCheck();
	void CompleteCheck();
	void ErrorCheck();
	void SafeCheck();

protected:
	BOOL ErrorRoutine();
	BOOL StoreRoutine();
	BOOL RetrieveRoutine();
	BOOL RackToRackRoutine();
	BOOL HsToHsRoutine(CJobItem* pJobItem, CTrackHS* pStoHS);

protected:
	BOOL ReadStatus();
//	BOOL Store(int nLuggNum, CString strLocation, int nHsNum, int nSize = 0, BOOL bStoHS = FALSE, BOOL bRetHS = FALSE);
//	BOOL Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nSize = 0, BOOL bStoHS = FALSE, BOOL bRetHS = FALSE);
//	BOOL HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nSize = 0, BOOL bStoHS = FALSE, BOOL bRetHS = FALSE);
	BOOL Store(int nLuggNum, CString strLocation, int nHsNum, int nSize = 0);
	BOOL Retrieve(int nLuggNum, CString strLocation, int nHsNum, int nSize = 0);
	BOOL HsToHs(int nLuggNum, int nHsNumFrom, int nHsNumTo, int nSize = 0);
	BOOL RackToRack(int nLuggNum, CString strLocFrom, CString strLocTo, int nSize = 0);
	BOOL CallToHome();

protected:
	BOOL Emergency();
	BOOL Active();
	BOOL Stop();
	BOOL Reset();
	BOOL Delete();

public:
	// 화재발생시 강제이동 명령
	BOOL FireAlarmOn();
	BOOL FireAlarmOff();

public:
	BOOL IsValidLocation(CString& strLocation);
};

#endif // !defined(AFX_SC_H__8CB40FD8_485F_4CBB_9B5D_9635082147DF__INCLUDED_)
