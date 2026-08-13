// Log.h : Defines the class behaviors for the application.

#ifndef __LOG_H__
#define __LOG_H__

class CEcsDoc;
class CReportCtrl;
class CStartupTip;

class CLogEvent : public CObject
{
	DECLARE_SERIAL(CLogEvent)

public:
	CLogEvent() {}
	CLogEvent(BYTE nLogPos, BYTE nType, int nLuggNum, CString strContent, CString strProdID="", int nErrCode=0);
	virtual ~CLogEvent() {}

public:
	COleDateTime	m_tTime;

	BYTE	m_nType;
	BYTE	m_nPos;
	int		m_nLuggNum;
	CString	m_strContent;
	CString m_strProdID;
	int		m_nErrCode;

	BOOL	m_bLoaded;

public:
	CString GetTime();
	CString GetLogType();
	CString GetPosition();
	CString GetLuggNum();
	CString GetErrorCode();

	CString GetPosition(int nPos);
	DWORD   GetReadSize();

	int		GetImageIndex();

	CString GetTipHead();
	CString GetTipContent();

public:
	CString GetXmlString();

public:
	CString GetString();
	void SetString(CString& strLogEvent);
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

class CSortContext
{
public:
	CSortContext();

public:
	CString m_strFileName;
	int		m_nType;
	int		m_nPos;
	int		m_nSort;
	CString m_strContext;
	int		m_nFrom;
	int		m_nTo;

	CStringList m_strFileNameList;
	CFont* m_pFont;

public:
	BOOL Sorting(CLogEvent* pLogEvent);
	BOOL CheckLuggNum(int nLuggNum);
	BOOL CheckProdID(CString strProdID);
	BOOL CheckErrorCode(int nErrCode);
	BOOL CheckContents(CString strContent);
	BOOL IsValidContext();
};

class CLog : public CObject
{
public:
	CLog(CEcsDoc* pDoc);
	virtual ~CLog();

public:
	CEcsDoc* m_pDoc;
	CObList  m_listAlarm;
	POSITION m_posAlarm;

public:
	CString	m_strLogName;
//	CString m_strLogDir;

public:
	CCriticalSection m_syncLogThread;
	CCriticalSection m_syncAlarmLoading;

public:
	void AssertValid() const;

	void Job(int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID=NULL, BYTE bAlarm=0);
	void Event(int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID=NULL, BYTE bAlarm=0);
	void Error(int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID=NULL, BYTE bAlarm=0);
	void Alarm(int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID=NULL, BYTE bAlarm=0);
	void Debug(int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID=NULL, BYTE bAlarm=0);
	void ShowAlarm(int nLogPos, int nLuggNum, CString strContent, CString strProdID = "");

	void Write(int nType, int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszProdID=NULL, BYTE bAlarm=0, int nErrCode=0);

	BOOL Load(CReportCtrl* pReportCtrl, CSortContext* pContext);
	BOOL LoadAlarmList(CReportCtrl* pReportCtrl);

public:
	void PumpupAlarm(CLogEvent* pLogEvent);
	void DeleteAllAlarm();
	void DeleteMaxOverAlarm();

	void LoadAlarmFirst(CStartupTip* pTip);
	void LoadAlarmLast(CStartupTip* pTip);
	void LoadAlarmPrev(CStartupTip* pTip);
	void LoadAlarmNext(CStartupTip* pTip);

	void MultiCastPerLog(CLogEvent* pLogEvent);
};

#endif
