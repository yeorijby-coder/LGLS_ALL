// Equipment.h: interface for the CEquipment class.
//
#pragma once

#include <afxcoll.h>
#include <afxtempl.h>
#include "Info.h"
#include "TrackInfo.h"

#include "AdoDB.h"
#include "URMDBAccess.h"

class CEcsDoc;
class CRecordSetWrap;

// CEquipment 명령 대상입니다.
//
class CBcr;
class CEquipment : public CObject
{
	DECLARE_DYNAMIC(CEquipment)


public:
	CEquipment(CEcsDoc* pDoc, CString strThreadNo, int nIndex);
	virtual ~CEquipment();

public:
	enum EN_SIZE_EQP_KIND { enEQP_CV = 1, enEQP_SIZE };
	enum EN_KIND { enNone = 0, enCV = 10, enCV1 = 11, enSC = 20, enRTV= 30, enLIFTER = 40, enLGV = 50, enRFID = 60, enBCR = 70, enREV = 71, enROLL = 72, enWC = 80 , enTHS = 90, enOther = 99 };//enDISPLAY = 80
	enum {	enBit_0 = 0x0001, enBit_1 = 0x0002, enBit_2 = 0x0004, enBit_3 = 0x0008, 
			enBit_4 = 0x0010, enBit_5 = 0x0020, enBit_6 = 0x0040, enBit_7 = 0x0080,
			enBit_8 = 0x0100, enBit_9 = 0x0200, enBit_10 = 0x0400, enBit_11 = 0x0800,
			enBit_12 = 0x1000, enBit_13 = 0x2000, enBit_14 = 0x4000, enBit_15 = 0x8000
		};

public:
	CEcsDoc* m_pDoc;
	CString m_WH_TYP;
	EN_KIND m_enKind;
	int m_nNumber;	//사용되지 않음!
	int m_nEQP_NO;
	int m_nIndex;
	int m_nStTrNum;

public:
	CString m_strThreadNo;


public:
	BOOL IsCollect();

public:
	CString GetEquipmentKey(EN_KIND pEN_KIND);
	CString GetEquipmentKey();

public:
	enum EN_EVENT { enEventKill, enEventSend, enEventSize };
	HANDLE m_hEventArray[enEventSize];

	CWnd* m_pEquipDlg;
	HWND			m_hWndInfo;

public:
	CWinThread*		m_pThread;
	CRecordSetWrap* m_pRsw;

protected:
	CCriticalSection m_csSyncThread;
	static UINT ThreadProc(LPVOID pParam);
	typedef UINT (*FPThreadProc)(LPVOID);
	FPThreadProc m_pfThreadProc;

protected:
	virtual void AutoRunProc() = 0;
	virtual void CommandProc() = 0;

public:
	virtual BOOL GetModified() { return FALSE; }
	void CallAutoRunProc();

public:
	virtual CInfo* GetInfo() { return NULL; }
	virtual CInfo* GetInfo(int n) { return NULL; }

public: //kdh20190521
	virtual void SetVar(CRecordSetWrap* pRecord){};
	virtual CString GetSelectQry(){ return _T("");};

protected:
	void ReconnectDB();
};

class CEquipmentArray : public CArray<CEquipment*, CEquipment*>
{
public:
	CEquipmentArray() {}
	virtual ~CEquipmentArray() {}

public:
	CEquipment* GetEquipment(int nEquipKind, int nEquipNum);
	CEquipment* GetEquipmentSC(int nEquipKind, int nEquipNum);
	CEquipment* GetEquipmentRTV(int nEquipKind, int nEquipNum);
	CEquipment* GetEquipmentBCR(int nEquipKind, int nEquipNum);
	void		InvokeControl(BOOL bManual);

public:
	CEquipment* 		IsExist(CEquipment::EN_KIND pEqpKind);
};


