

#ifndef __ENCARRIER__H_
#define __ENCARRIER__H_

class CEnCarrierItem : public CObject
{
public:
	CEnCarrierItem();
	~CEnCarrierItem() {};

public:
	CString		m_strCOMMANDID;
	CString		m_strCSTID;
	CString		m_strSHIPID;
	CString		m_strFOSBID;
	CString		m_strEMPTYFLAG;
	CString		m_strLOTID;
	CString		m_strCSTSTATE;
	CString		m_strCARRIERLOC;
	CString		m_strZONENAME;

// public:
// 	CString GetInstallTime();
};

class CEnEquipmentItem : public CObject
{
public:
	CEnEquipmentItem(CString strUNITID, int nUNITSTATE);
	~CEnEquipmentItem() {};

public:
	CString		m_strUNITID;
	int			m_nUNITSTATE;
};

class CEnTransferItem : public CObject
{
public:
	CEnTransferItem();
	~CEnTransferItem() {};

public:
	CString		m_strCOMMANDID;

	CString		m_strCSTID;
	CString		m_strSHIPID;
	CString		m_strFOSBID;
	CString		m_strEMPTYFLAG;
	CString		m_strLOTID;
	CString		m_strPRIORITY;
	CString		m_strSOURCEZONE;
	CString		m_strDESTZONE;

// public:
// 	CString GetCommandTime();
};

class CReportItem
{
public:
	CReportItem();
	~CReportItem();

public:
	int			m_nLuggNum;
	BOOL		m_bManual;

public:
	CString     m_strCOMMANDID;
	CString		m_strSOURCE;
	CString		m_strDEST;
	CString		m_strCSTID;
	CString		m_strSHIPID;
	CString		m_strFOSBID;
	CString		m_strEMPTYFLAG;
	CString		m_strCARRIERLOC;

	CString		m_strEVENTTIME;

public:
	void	SetEventTime(CTime tTime);
};

#endif
