// CmdMsg.h: interface for the CCmdMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDMSG_H__305EED4A_D4FE_4367_B5D4_9C2F5792D8FF__INCLUDED_)
#define AFX_CMDMSG_H__305EED4A_D4FE_4367_B5D4_9C2F5792D8FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcoll.h>

class CCmdMsg : public CObject  
{
public:
	CCmdMsg();
	virtual ~CCmdMsg();

public:
	enum EN_COMMAND { enCmdWriteLamp = 94, enCmdWriteWord = 95, enCmdTrackDeadLockCheck = 96, enCmdTrackDeadLock = 97, enCmdTrackSuspend = 98, enCmdTrackWrite = 99 };
	enum { enSizeTrackDeadLockCheck = 2 };
	enum { enSizeTrackSuspend = 2 };
	enum { enSizeTrackDeadLock = 2 };
	enum { enSizeTrackWrite = 6 };
	enum { enSizeWriteWord = 2 };
	enum { enSizeWriteLamp = 2 };

public:
	int			 m_nCommand;
	int			 m_nSubCmd;
	CStringArray m_strValues;
};

class CCvCmdMsg : public CCmdMsg
{
public:
	CCvCmdMsg() {}
	virtual ~CCvCmdMsg() {}

public:
	enum EN_COMMAND { enCmdDoorOpenReq, enCmdDoorOpenAccept, enCmdDoorClose, enCmdDoorStatus, enCmdTurnPermission, enCmdTurnPermissionRequest};
	enum { enSizeDoor = 1 };
};

class CScCmdMsg : public CCmdMsg
{
public:
	CScCmdMsg() {}
	virtual ~CScCmdMsg() {}
	
//#define SC_CMD_INVOKE		1
#define SC_CMD_COMPLETE		2
#define SC_CMD_CANCEL		3
#define SC_CMD_RESET		4
#define SC_CMD_ESTOP		5
#define SC_CMD_STOP			6
#define SC_CMD_ACTIVE		7
#define SC_CMD_CALL_TO_HOME	8
#define SC_CMD_DUAL_STO		9
#define SC_CMD_EMPTY_RET	10
#define SC_CMD_MANUAL		11
#define SC_CMD_STO_SUSPEND	12
#define SC_CMD_RESTART		13
#define SC_CMD_NEW_LOCATION	14
#define	SC_CMD_ERROR_RESET	15 
#define SC_CMD_RET_SUSPEND	16
#define SC_CMD_STEP_UPDATE	17
#define SC_CMD_CANCEL1		31
#define SC_CMD_CANCEL2		32
#define SC_CMD_CANCEL3		33
#define SC_CMD_PAUSE		34

#define CMD_SC_STORE	 0
#define CMD_SC_RETRIEVE  1
#define CMD_SC_RTR		 2
#define CMD_SC_SITE		 3

public:
	enum EN_COMMAND {
		enCmdInvoke			= 1,
		enCmdComplete		= 2, 
		enCmdCancel			= 3,
		enCmdCancel1		= 4,
		enCmdCancel2		= 5,
		enCmdCancel3		= 6,
		enCmdReset			= 7, 
		enCmdEmergency		= 8, 
		enCmdStop			= 9, 
		enCmdActive			= 10, 
		enCmdDualStore		= 11,
		enCmdEmptyRet		= 12,
		enCmdHomeReturn		= 13, 
		enCmdNewLocation	= 14,
		enCmdStartUp		= 15, 
		enCmdStoSuspend		= 16, 
		enCmdRetSuspend		= 17,
		enCmdR2RSuspend		= 18,
		enCmdManualSto		= 19, 
		enCmdManualRet		= 20,  
		enCmdManualHs2Hs	= 21,   
		enCmdManualR2R		= 22
	};
	
	enum {	
		enSizeScCommand		= 4,
		enSizeScPairCommand = 6 
	
	};
};

class CRtvCmdMsg : public CCmdMsg
{
public:
	CRtvCmdMsg() {}
	virtual ~CRtvCmdMsg() {}

public:
	enum EN_COMMAND { enCmdHs2Hs, enCmdMove, enCmdReturnHome, enCmdComplete, enCmdEmergency, enCmdActive, enCmdStop, enCmdReset, enCmdDelete, enCmdSuspend };
	enum { enSizeHs2Hs = 4 };
};

class CLifterCmdMsg : public CCmdMsg
{
public:
	CLifterCmdMsg() {}
	virtual ~CLifterCmdMsg() {}

public:
	enum EN_COMMAND { enCmdTrack, enCmdHs2Hs, enCmdComplete, enCmdEmergency, enCmdReset, enCmdDelete, enCmdDoorInStatus, enCmdDoorOutStatus, enCmdSuspend };
	enum { enSizeTrack = 5 };
	enum { enSizeHs2Hs = 4 };
	enum { enSizeDoorStatus = 2};
};

class CLgvCmdMsg : public CCmdMsg
{
public:
	CLgvCmdMsg() {}
	virtual ~CLgvCmdMsg() {}

public:
	enum EN_COMMAND { 
		enTRR, enMOD, enDEL, enTRQ, enAGQ, enHOS, enTEX,	// SEND
		enTRC, enMOR, enDER, enTRS, enAGS, enTOC,			// RECV
		enRgvStop, enCmdSuspend
	};
	enum { enSizeTRR = 5 };
	enum { enSizeMOD = 5 };
	enum { enSizeDEL = 1 };
	enum { enSizeTRQ = 1 };
	enum { enSizeAGQ = 1 };
	enum { enSizeHOS = 2 };
	enum { enSizeTEX = 3 };
};

class CRfidCmdMsg : public CCmdMsg
{
public:
	CRfidCmdMsg() {}
	virtual ~CRfidCmdMsg() {}

public:
	enum EN_COMMAND { enCmdSuspend, enCmdRead, enCmdWrite, enCmdWriteCrate, enCmdWriteBIDP, enCmdClear, enCmdErrorReset };
	enum { enSizeCmd = 2 };
};

class CBcrCmdMsg : public CCmdMsg
{
public:
	CBcrCmdMsg() {}
	virtual ~CBcrCmdMsg() {}

public:
	enum EN_COMMAND { enCmdSuspend, enCmdRead, enCmdErrorReset, enCmdRequestReset };
};

class CDisplayCmdMsg : public CCmdMsg
{
public:
	CDisplayCmdMsg() {}
	virtual ~CDisplayCmdMsg() {}

public:
	enum EN_COMMAND { enCmdData, enCmdClear, enCmdTest, enCmdSuspend };
	enum { enSizeCmd = 1 };
	enum { enSizeData = 2 };
};


#endif // !defined(AFX_CMDMSG_H__305EED4A_D4FE_4367_B5D4_9C2F5792D8FF__INCLUDED_)
