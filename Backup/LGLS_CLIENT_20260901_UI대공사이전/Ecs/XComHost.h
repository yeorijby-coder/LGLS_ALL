
#pragma once


//class CSystem;
//class CUnit;
//class CAlarmInfo;
//class CAlarmGroup;

// ============= SECS_State ================
#define SECS_STAT_NOT_CONNECTED		101
#define SECS_STAT_NOT_SELECTED		102
#define SECS_STAT_SELECTED			103

class CXComHost : public CDialogEx
{

public:
	CXComHost(CWnd* pParent = NULL);

protected:
	CeXCom		m_XCom;
	
	CEcsDoc*	m_pDoc;
//	CSystem*	m_pSystem;

protected:
	int			m_nSecsState;
	int			m_nErrorCode;
//	CAlarmGroup* m_pAlarmGroupError;
//	CAlarmGroup* m_pAlarmGroupEvent;

public:
	BOOL		InitHost();

	BOOL		RunHost();
	BOOL		InitXCom();
	BOOL		StartXCom();
	BOOL		StopXCom();
	BOOL		CloseXCom();
	void		UpdateCommHost(char Status);

public:
	int			GetSecsState()	{	return m_nSecsState;};
	CString		GetEventString(int nEventCode);
	CString		GetErrorString(int nErrCode);
	CString		GetFormatSecsState();
	COLORREF	GetSecsStateBackColor();
	COLORREF	GetSecsStateTextColor();

	void		SetSecsState(int nState);
	BOOL		IsSelected();
	BOOL		IsOnLineMode();
	BOOL		IsOffLineMode();

protected:
	BOOL		SendMsg(LONG nTxMsgID);
	BOOL		MakeSecsMsg(LONG& nTxMsgID, SHORT nStrmNO, SHORT nFuncNO, SHORT nDevID=0, LONG nSysByte=0);

public:
	int			GetList(LONG nRxMsgID);
	BOOL		GetAscii(LONG nRxMsgID, CString& strItem);

	BOOL		GetI8(LONG nRxMsgID, int& nValue);
	BOOL		GetI4(LONG nRxMsgID, int& nValue);
	BOOL		GetI2(LONG nRxMsgID, short& nValue);
	BOOL		GetI1(LONG nRxMsgID, short& nValue);
	BOOL		GetU8(LONG nRxMsgID, double& nValue);
	BOOL		GetU4(LONG nRxMsgID, double& nValue);
	BOOL		GetU2(LONG nRxMsgID, int& nValue);
	BOOL		GetU1(LONG nRxMsgID, int& nValue);
	BOOL		GetBool(LONG nRxMsgID, short& nValue);
	BOOL		GetBinary(LONG nRxMsgID, short& nValue);

	CString		GetAscii(LONG nRxMsgID);
	int			GetI8(LONG nRxMsgID);
	int			GetI4(LONG nRxMsgID);
	short		GetI2(LONG nRxMsgID);
	short		GetI1(LONG nRxMsgID);
	double		GetU8(LONG nRxMsgID);
	double		GetU4(LONG nRxMsgID);
	int			GetU2(LONG nRxMsgID);
	int			GetU1(LONG nRxMsgID);
	short		GetBool(LONG nRxMsgID);
	short		GetBinary(LONG nRxMsgID);


	BOOL		SetList(LONG nTxMsgID, short nListItem);
	BOOL		SetAscii(LONG nTxMsgID, CString strItem, int nLength);
	
	BOOL		SetAscii(LONG nTxMsgID, CString strItem);
	BOOL		SetAscii(LONG nTxMsgID, int nItem);

	BOOL		SetU8(LONG nTxMsgID, double nValue);
	BOOL		SetU4(LONG nTxMsgID, double nValue);
	BOOL		SetU2(LONG nTxMsgID, long nValue);
	BOOL		SetU1(LONG nTxMsgID, short nValue);
	BOOL		SetI8(LONG nTxMsgID, long nValue);
	BOOL		SetI4(LONG nTxMsgID, long nValue);
	BOOL		SetI2(LONG nTxMsgID, short nValue);
	BOOL		SetI1(LONG nTxMsgID, short nValue);
	BOOL		SetBool(LONG nTxMsgID, short nValue);
	BOOL		SetBinary(LONG nTxMsgID, short nValue);

public:
	CString		GetStringCEID(int nCEID);
	CString		GetStringIDRead(int nIDRead);


	void		LogJob(CString strContent, BOOL bAlarm=0);
	void		LogEvent(CString strContent, BOOL bAlarm=0);
	void		LogDebug(CString strContent, BOOL bAlarm=0);
	void		LogError(CString strContent, int nErrorCode=0, BOOL bAlarm=0);
	void		LogAlarm(int nLogType, CString strContent);

	void		LogJob(CJobItem* pLogJob, CString strContent);
	void		LogEvent(CJobItem* pLogJob, CString strContent);
	void		LogDebug(CJobItem* pLogJob, CString strContent);
	void		LogError(CJobItem* pLogJob, CString strContent, BYTE bAlarm=0);


	void		LogJob(CString strCmdID, CString strCstID, CString strCurPos, CString strSrcPos, CString strDesPos, CString strContent, BOOL bAlarm=0);
	void		LogEvent(CString strCmdID, CString strCstID, CString strCurPos, CString strSrcPos, CString strDesPos, CString strContent, BOOL bAlarm=0);
	void		LogDebug(CString strCmdID, CString strCstID, CString strCurPos, CString strSrcPos, CString strDesPos, CString strContent, BOOL bAlarm=0);
	void		LogError(CString strCmdID, CString strCstID, CString strCurPos, CString strSrcPos, CString strDesPos, CString strContent, BOOL bAlarm=0);


public:
// Dialog Data
	//{{AFX_DATA(CXComHost)
	enum { IDD = IDD_ECS_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXComHost)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXComHost)
	virtual BOOL OnInitDialog();
	virtual afx_msg void OnReceiveSecsMsg();
	virtual afx_msg void OnReceiveSecsEvent(short nEventId, long lParam);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

