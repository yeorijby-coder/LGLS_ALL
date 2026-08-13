// CmdMsg.h: interface for the CCmdMsg class.
//

#pragma once

#include <afxcoll.h>


// CCmdMsg
//
class CCmdMsg : public CObject  
{
public:
	CCmdMsg();
	virtual ~CCmdMsg();

public:
	enum EN_SV_COMMAND {
		enCmdSendDataCmd = 1,
		enCmdSendDataCmdSleep = 2
	};
	enum{ enSizeSendDataCmd = 2 };

public:
	enum EN_COMMAND {
		enCmdArriveReport = 85, 
		enCmdTPStart = 86, 
		enCmdMGStart = 87, 
		enCmdDPStart = 88, 
		enCmdInterBcr1 = 89, 
		enCmdInterBcr2 = 90, 
		enCmdWaitMinute = 91, 
		enCmdWriteError = 92, 
		enCmdIMSReport = 93, 
		enCmdStoRequest = 94, 
		enCmdWriteWord = 95, 
		enCmdTrackDeadLockCheck = 96, 
		enCmdTrackDeadLock = 97, 
		enCmdTrackSuspend = 98, 
		enCmdTrackWrite = 99,  
		enCmdTrackLevel = 100};
	enum { enSizeTrackDeadLockCheck = 2 };
	enum { enSizeTrackSuspend = 2 };
	enum { enSizeTrackDeadLock = 2 };
	enum { enSizeTrackWrite = 6 };
	enum { enSizeWriteWord = 2 };
	enum { enSizeDPStart = 4 };
//	enum { enSizeDPStart = 4 };

public:
	int			 m_nCommand;
	int			 m_nSubCmd;
	CStringArray m_strValues;
};

// CCvCmdMsg
//
class CCvCmdMsg : public CCmdMsg
{
public:
	CCvCmdMsg() {}
	virtual ~CCvCmdMsg() {}

public:
	enum EN_COMMAND { enCmdDoorOpen, enCmdDoorClose, enCmdDoorStatus, enCmdTurnPermission, enCmdTurnPermissionRequest};
	enum { enSizeDoor = 1 };
};

// CScCmdMsg
//
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
		enCmdInvoke		= 1,
		enCmdComplete	= 2, 
		enCmdCancel		= 3,
		enCmdCancel1	= 4,
		enCmdCancel2	= 5,
		enCmdCancel3	= 6,
		enCmdReset		= 7, 
		enCmdEmergency	= 8, 
		enCmdStop		= 9, 
		enCmdActive		= 10, 
		enCmdDualStore	= 11,
		enCmdEmptyRet	= 12,
		enCmdHomeReturn	= 13, 
		enCmdNewLocation= 14,
		enCmdStoSuspend	= 15, 
		enCmdRetSuspend	= 16,
		enCmdManualSto	= 17, 
		enCmdManualRet	= 18,  
		enCmdManualHs2Hs= 19,   
		enCmdManualR2R	= 20,
		enCmdFireAlarmOn= 21,
		enCmdFireAlarmOff= 22
	};
	
	enum {	enSizeScCommand		= 3,
			enSizeScPairCommand = 6 };
};

// CRtvCmdMsg
//
class CRtvCmdMsg : public CCmdMsg
{
public:
	CRtvCmdMsg() {}
	virtual ~CRtvCmdMsg() {}

public:
	enum EN_COMMAND { enCmdHs2Hs, enCmdMove, enCmdReturnHome, enCmdComplete, enCmdEmergency, enCmdActive, enCmdStop, enCmdReset, enCmdDelete, enCmdSuspend, enCmdReject };
	enum { enSizeHs2Hs = 11 };
};

// CLifterCmdMsg
//
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

// CLgvCmdMsg
//
class CLgvCmdMsg : public CCmdMsg
{
public:
	CLgvCmdMsg() {}
	virtual ~CLgvCmdMsg() {}

public:
	enum EN_COMMAND { 
		enTRR, enMOD, enDEL, enTRQ, enAGQ, enHOS, enTEX,	// SEND
		enTRC, enMOR, enDER, enTRS, enAGS, enTOC,			// RECV
		enReset, enCmdSuspend
	};
	enum { enSizeTRR = 5 };
	enum { enSizeMOD = 5 };
	enum { enSizeDEL = 1 };
	enum { enSizeTRQ = 1 };
	enum { enSizeAGQ = 1 };
	enum { enSizeHOS = 2 };
	enum { enSizeTEX = 3 };
};

// CRfidCmdMsg
//
class CRfidCmdMsg : public CCmdMsg
{
public:
	CRfidCmdMsg() {}
	virtual ~CRfidCmdMsg() {}

public:
	enum EN_COMMAND { enCmdSuspend, enCmdRead, enCmdWrite, enCmdWriteCrate, enCmdWriteBIDP, enCmdClear, enCmdErrorReset };
	enum { enSizeCmd = 2 };
};

// CBcrCmdMsg
//
class CBcrCmdMsg : public CCmdMsg
{
public:
	CBcrCmdMsg() {}
	virtual ~CBcrCmdMsg() {}

public:
	enum EN_COMMAND { enCmdSuspend, enCmdRead, enCmdErrorReset, enCmdRequestReset, enCmdManualReport, enCmdNotUsed };
	enum { enSizeManualReport = 3 };
};

// CDisplayCmdMsg
//
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
