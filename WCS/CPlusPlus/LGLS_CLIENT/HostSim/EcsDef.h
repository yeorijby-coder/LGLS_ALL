#ifndef __ECSDEF_H__
#define __ECSDEF_H__


////////////////////////////////////////////////////////////////
// <File Definition>
#define ECS_SERVER_ID			_T("SCP2")
#define ECS_CLIENT_VERSION		_T("1.0.0")
// </File Definition>
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// <File Definition>
#define ECS_DEFINE_FILE			_T(".\\EcsDefine.xml")
#define ECS_LAYOUT_FILE			_T(".\\EcsLayout.xml")
#define ECS_LAYOUT_FILE1		_T(".\\EcsLayout1.xml")
#define ECS_LAYOUT_FILE2		_T(".\\EcsLayout2.xml")
#define ECS_CODE_FILE			g_strEcsPath + _T("\\EcsCode.dat")
#define ECS_INI_FILE			g_strEcsPath + _T("\\Ecs.ini")
#define ECS_USER_FILE			g_strEcsPath + _T("\\Data\\EcsUser.dat")
#define ECS_USER_BACK_FILE		g_strEcsPath + _T("\\Data\\EcsUser.bak")
// </File Definition>
////////////////////////////////////////////////////////////////

/////////////// Auto State //////////////////////////////
#define AUTO_STAT_INITIATE			1
#define AUTO_STAT_PAUSING			2
#define AUTO_STAT_PAUSE				3
#define AUTO_STAT_AUTO				4


////////////////////////////////////////////////////////////////
#define		ECS_STN_POS_PORT_1		1
#define		ECS_STN_POS_PORT_2		2
#define		ECS_STN_POS_PORT_3		3
#define		ECS_STN_POS_PORT_4		4


////////////////////////////////////////////////////////////////
// <HOST Definition>
enum EN_HOST_COMMAND {
	enHostCommandOrder				= 'O',
	enHostCommandOrderAnswer		= 'o',
	enHostCommandRedirect			= 'R',
	enHostCommandRedirectAnswer		= 'r',
	enHostCommandResendOrder		= 'Z',
	enHostCommandResendOrderAnswer	= 'z',
	enHostCommandStatus				= 'S',
	enHostCommandStatusAnswer		= 's',
	enHostCommandArrive				= 'A',
	enHostCommandArriveAnswer		= 'a',
	enHostCommandBarcode			= 'B',
	enHostCommandBarcodeAnswer		= 'b',
	enHostCommandComplete			= 'F',
	enHostCommandCompleteAnswer		= 'f',
	enHostCommandCancel				= 'D',
	enHostCommandCancelAnswer		= 'd',
	enHostCommandError				= 'E',
	enHostCommandErrorAnswer		= 'e',
	enHostCommandJobRequest			= 'Q',
	enHostCommandJobRequestAnswer	= 'q',
	enHostCommandTrouble			= 'T',
	enHostCommandTroubleAnswer		= 't',
	enHostCommandLeadTime			= 'L',
	enHostCommandLeadTimeAnswer		= 'l',
	enHostCommandReDirWeight		= 'W',
	enHostCommandReDirWeightAnswer	= 'w',
	enHostCommandUnpredict			= 'X'
};
enum EN_HOST_ERROR_CODE {
	enHostErrorNone						= 0,
	enHostErrorNoSTX					= 1,
	enHostErrorNoETX					= 2,
	enHostErrorDuplicatedLuggNum		= 3,
	enHostErrorNotExistJob				= 4,
	enHostErrorBufferFull				= 5,
	enHostErrorInvalidHeaderLength		= 6,
	enHostErrorInvalidMsgLength			= 7,
	enHostErrorInvalidLocation			= 8,
	enHostErrorInvalidStation			= 9,
	enHostErrorInvalidLugg				= 10,	//
	enHostErrorUndefinedCommandType		= 11,
	enHostErrorInhibitedLoc				= 12,
	enHostErrorInvalidMailboxName		= 13,
	enHostErrorAlreadyJob				= 14,
	enHostErrorInvalidContent			= 15,
	enHostErrorUnknownJobType			= 16,
	enHostErrorImproperHandshake		= 17,
	enHostErrorNoResponse				= 18,
	enHostErrorSocket					= 19,
	enHostErrorInternal					= 99 

};

//	enHostErrorInvalidStartWarehouse	= 9,
//	enHostErrorInvalidStartStation		= 9,
//	enHostErrorInvalidDestWarehouse		= 9,
//	enHostErrorInvalidDestStation		= 9,
//	enHostErrorInvalidStartLocation		= 8,
//	enHostErrorInvalidDestLocation		= 8,
//	enHostErrorRefusedEcsJob			= 19,
//	enHostErrorProtocolLength			= 20,
//	enHostErrorXmlParser				= 24,
//	enHostErrorXmlAttribute				= 25,
//	enHostErrorXmlElement				= 26,
//	enHostErrorXmlValue					= 27,
//	enHostErrorJobReport				= 28,
//	enHostErrorImproperHandshake		= 30,
//	enHostErrorInvalidHeaderLength		= 23,
//	enHostErrorInvalidMsgLength			= 24,
//	enHostErrorInvalidLuggNo			= 25,
//	enHostErrorInvalidMailboxName		= 26,
//	enHostErrorInhibitedLoc				= 28,
//	enHostErrorUnknownMsgType			= 29,
//	enHostErrorAlreadyJob				= 17,
//	enHostErrorNoSTX					= 11,
//	enHostErrorNoETX					= 12,
//	enHostErrorInvalidDestLocation		= 7,
//	enHostErrorDuplicatedLuggNum		= 8,
//	enHostErrorBufferFull				= 9,
//	enHostErrorInvalidStartLocation		= 4,


// </HOST Definition>
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// <CV Definition>
#define CV1_PLC					0
#define CV2_PLC					1
#define CV3_PLC					2
#define CV_PLC_CNT				3
// </CV Definition>
////////////////////////////////////////////////////////////////

#define SC_CNT					5
#define STO_STN_CNT				7

////////////////////////////////////////////////////////////////
// <SC Definition>
#define SC1_SRV					0
#define SC2_SRV					1
#define SC_SRV_CNT				2

#define SC1_PLC_CNT				3
#define SC2_PLC_CNT				3
#define SC_PLC_TOT_CNT			(SC1_PLC_CNT + SC2_PLC_CNT)

#define RACK_BANK_LEN			2
#define RACK_BAY_LEN			2
#define RACK_LEVEL_LEN			2
#define RACK_LOC_LEN			6
// </RTV Definition>
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// <RTV Definition>
#define RTV1_SRV					0
#define RTV2_SRV					1
#define RTV_SRV_CNT					2

#define RTV1_PLC_CNT				5
#define RTV2_PLC_CNT				8
#define RTV_PLC_TOT_CNT				(RTV1_PLC_CNT + RTV2_PLC_CNT)

// 작업구분
#define	RTV_JOB_TYPE_NONE				'0'
#define	RTV_JOB_TYPE_SITE_TO_SITE		'3'
#define	RTV_JOB_TYPE_CALL_TO_HOME		'5'
#define	RTV_JOB_TYPE_MANUAL				'6'

#define RTV_ERR_CODE_COMM_ERROR			999
// </RTV Definition>
////////////////////////////////////////////////////////////////


#define	ECS_STN_POS_101					101
#define	ECS_STN_POS_201					201
#define	ECS_STN_POS_202					202

#define	ECS_STO_STN_CNT					1
#define	ECS_RET_STN_CNT					2
#define	IMS_STN_CNT						32

////////////////////////////////////////////////////////////////
// <LGV Definition>
#define LGV_JOB_SUSPEND
////////////////////////////////////////////////////////////////
// <Log Definition>
#define LOG_DISP_MAX	50000

#define LOG_SYSTEM		9999

#define LOG_POS_ALL			0
#define LOG_POS_SYSTEM		1
#define LOG_POS_HOST		2
#define LOG_POS_CV			3
#define LOG_POS_SC			4
#define LOG_POS_RGV			5
#define LOG_POS_MONITOR		6
#define LOG_POS_CNT			7

#define LOG_POS_LGV			8
#define LOG_POS_LIFTER		9
#define LOG_POS_RFID		10
#define LOG_POS_BCR			11
#define LOG_POS_DISPLAY		12

#define	LOG_TYPE_ALL		0
#define	LOG_TYPE_JOB		1
#define	LOG_TYPE_EVENT		2
#define	LOG_TYPE_ERROR		3
#define	LOG_TYPE_ALARM		4
#define	LOG_TYPE_DEBUG		5
#define	LOG_TYPE_CNT		6

#define LOG_SORT_NONE		0
#define LOG_SORT_LUGGID		1
#define LOG_SORT_PRODID		2
#define LOG_SORT_STR		3

#define	ALARM				1
//////////////////////////////////////////////////////////////////////////
#define		INTBABO		0


#define		WORD_SIZE_OF_ECS2PLC		3
#define		WORD_SIZE_OF_PLC2ECS		4

#define		OFFSET_OF_CYCLE_STEP		2
#define		OFFSET_OF_PORTRESERVE		2

#define		START_ADDR_OF_ECS2PLC		2100
#define		START_ADDR_OF_PLC2ECS		2600

#define		WORD_SIZE_TO_READ			4

#define		OFFSET_OF_HORI_ERROR		24
#define		OFFSET_OF_VERT_ERROR		26
#define		OFFSET_OF_FORK_AIM_ERROR	28


////////////////////////////////////////////////////////////////

#define SIG_SCAN_UP_OR_DOWN			31	//	Scan Loc(U/D)
#define SIG_CYCLE_STOP				30	//	Cycle Stop
#define SIG_FORK2_RIGHT_BANK		29	//	Fork2 Bank(L/R)
#define SIG_FORK1_RIGHT_BANK		28	//	Fork1 Bank(L/R)
#define SIG_STARTUP_REQ				27	//	비정상 완료			SIG_ABNORMAL_COMPLETE
#define SIG_SELECT_UP_OR_DOWN		26	//	Up/Down 위치
#define SIG_SELECT_PND_OR_SHELF		25	//	P&D/Shelf 선택
#define SIG_FORKING_INVOKE			24	//	FORKING INVOKE (Half/Full)

#define SIG_READ_OR_FRONT			23	//	Rear/Front
#define SIG_BELT_MOVE				22	//	BELT 이동 (L/R)
#define SIG_SELECT_FORK2			21	//	Fork2 선택
#define SIG_SELECT_FORK1			20	//	Fork1 선택
//#define SIG_STARTUP_REQ				19	//	Start Up Req
#define SIG_CYCLE_BIT_2				18	//	Cycle Bit 2
#define SIG_CYCLE_BIT_1				17	//	Cycle Bit 1
#define SIG_CYCLE_BIT_0				16	//	Cycle Bit 0

#define SIG_PERMIT_PICKUP			15	//	Pick Up 허가
#define SIG_PERMIT_UNLOAD			14	//	Unload 허가
#define SIG_SPARE_13				13	//	감속모드
#define SIG_SPARE_12				12	//
#define SIG_SPARE_11				11	//
#define SIG_SPARE_10				10	//
#define SIG_SPARE_09				9	//
#define SIG_SPARE_08				8	//	안전센서 OFF

#define SIG_MAIN_POWER_ON_COMMAND	7	//	Main 전원 On
#define SIG_MAIN_POWER_OFF			6	//	Main 전원 Off
#define SIG_INVOKE_ACTIVE			5	//	Active 지시
#define SIG_COMPLETE_WORK_RECEIVE	4	//	작업완료 Receive
#define SIG_ERROR_RESET				3	//	Error Reset
#define SIG_BUZZER_STOP				2	//	Buzzer Stop
#define SIG_PICKUP_OK				1	//	Pick Up OK
#define SIG_UNLOAD_OK				0	//	Unload OK

/////////////////////////////////////////////////////////////////

#define	SIG_PLC_MAINPOWER_ON		7	//		7"Main 전원 On",		
#define	SIG_PLC_CYCLE_OK			6	//		6"Cycle Ok",			
#define	SIG_PLC_ACTIVE				5	//		5"Active",			
#define	SIG_PLC_JOB_COMPLETE		4	//		4"작업완료",			
#define	SIG_PLC_ERROR				3	//		3"Error",		
#define	SIG_PLC_UNLOADING			2	//		2"Unloading",	
#define	SIG_PLC_MOVE_COMPLETE		1	//		1"이동 완료",		
#define	SIG_PLC_MAINPOWERE_PUTIN	0	//		0"Main 전원 투입", 

#define	SIG_PLC_F2_FORKING_L	15	//		15"Fork2 Forking(L)", 
#define	SIG_PLC_F2_FORKING_R	14	//		14"Fork2 Forking(R)", 
#define	SIG_PLC_F1_FORKING_L	13	//		13"Fork1 Forking(L)", 
#define	SIG_PLC_F1_FORKING_R	12	//		12"Fork1 Forking(R)", 
#define	SIG_PLC_F2_ERROR		11	//		11"Fork2 ERROR",	
#define	SIG_PLC_F1_ERROR		10	//		10"Fork1 ERROR",	
#define	SIG_PLC_F2_ON_FREIGHT	9	//		9"Fork2 화물 감지",	
#define	SIG_PLC_F1_ON_FREIGHT	8	//		8"Fork1 화물 감지",

#define	SIG_PLC_SPARE_23		23	//		23"",					
#define	SIG_PLC_SPARE_22		22	//		22"",					
#define	SIG_PLC_SPARE_21		21	//		21"",					
#define	SIG_PLC_SPARE_20		20	//		20"TURN L/R",			
#define	SIG_PLC_ON_MOVING_MINUS	19	//		19"MINUS 이동",	
#define	SIG_PLC_ON_MOVING_PLUS	18	//		18"PLUS 이동",	
#define	SIG_PLC_SAFETY_SENSOR	17	//		17"안전센서 OFF",		
#define	SIG_PLC_INDEPENDENT_RUN	16	//		16"단독운전", 

#define	SIG_PLC_F2_L_SENSOR		31	//		31"Fork2(L)Sensor",	
#define	SIG_PLC_F2_R_SENSOR		30	//		30"Fork2(R)Sensor",	
#define	SIG_PLC_F1_L_SENSOR		29	//		29"Fork1(L)Sensor",	
#define	SIG_PLC_F1_R_SENSOR		28	//		28"Fork1(R)Sensor",	
#define	SIG_PLC_ON_MANUAL		27	//		27"수동",			
#define	SIG_PLC_BUFFER_EMPTY	26	//		26"Buffer Empty", 
#define	SIG_PLC_ON_WORKING		25	//		25"작업중",			
#define	SIG_PLC_ON_HOME			24	//		24"Home",

// </Log Definition>
////////////////////////////////////////////////////////////////

#define	SC_CYCLE_HOME		1
#define SC_CYCLE_PICKUP		2
#define	SC_CYCLE_UNLOAD		3
#define	SC_CYCLE_MOVE		4
#define SC_CYCLE_BMOVE		5
#define SC_CYCLE_CLEAN		6
#define SC_CYCLE_LEARN		7


////////////////////////////////////////////////////////////////
// <Job Definition>
enum EN_ERROR_GBN { enErrorDualStore=1, enErrorEmptyRetrieve=2 };
enum EN_EQUIP_GBN { enEquipSC=1, enEquipBothStn=2, enEquipStoStn=3, enEquipRetStn=4 };
enum EN_SC_STATUS_GBN { enScStatusOK=0, enScStatusNotSto=1, enScStatusNotJob=2 };
enum EN_COMPLETE_GBN { enCompleteStore=1, enCompleteRetrieve=2 };
enum EN_ERROR_CODE { 
	enScErrorNone=0, 
	enScErrorDualStore=28, enScErrorDualStore2=55, 
	enScErrorStoreObstacle=56, enScErrorStoreObstacle2=66, 
	enScErrorEmptyRetrieve=30, enScErrorEmptyRetrieve2=59, 
	enScErrorRetrieveObstacle=57, enScErrorRetrieveObstacle2=67, 
	enScErrorCommunication=999 };


enum EN_JOB_TYPE { 
	enJobTypeNone = 0, 
	enJobTypeAutoSto =  1, enJobTypeAutoRet =  2, enJobTypeAutoPR =  3, enJobTypeAutoR2R =  4, enJobTypeAutoW2W =  5, enJobTypeAutoMove =  6, 
	enJobTypeSemiSto = 11, enJobTypeSemiRet = 12, enJobTypeSemiPR = 13, enJobTypeSemiR2R = 14, enJobTypeSemiW2W = 15, enJobTypeSemiMove = 16, 
	enJobTypeManual = 21
};

enum EN_JOB_PATTERN { 
	enJobPatternNone,
	enJobPatternSto, enJobPatternRet, enJobPatternPR, enJobPatternR2R, enJobPatternW2W, enJobPatternMove
};

enum EN_JOB_STATUS {
	enJobStatusAll, enJobStatusNew,
	enJobStatusComplete, enJobStatusArrived, enJobStatusCancel,
	enJobStatusErrorEmptyRetrieve, enJobStatusErrorDualStore, enJobStatusDualStoreRetry, enJobStatusEmptyRetrieveRetry,
	enJobStatusCvNew, enJobStatusCvInvoke,
	enJobStatusScRequest, enJobStatusScInvoke, enJobStatusScComplete,
	enJobStatusRtvRequest, enJobStatusRtvInvoke, enJobStatusRtvComplete,
	enJobStatusLifterInvoke, enJobStatusLifterComplete, enJobStatusLifterCvInvoke,
	enJobStatusLgvInvoke, enJobStatusLgvComplete,
	enJobStatusResend, enJobStatusScResend, 
	enJobStatusCompleteRequest, enJobStatusRefuseComplete,
	enJobStatusError, enJobStatusRefuseError, enJobStatusInvokeError,
	enJobStatusRefuseArrived, enJobStatusWCError,
	enJobStatusSize
};

/*
enum EN_SC_CYCLE_STATUS {
	enScCycleStatusAll, 
	enScCycleStatusPikingRequest, enScCycleStatusPikingInvoke, enScCycleStatusPikingComplete,
	enScCycleStatusBMoveRequest, enScCycleStatusBMoveInvoke, enScCycleStatusBMoveComplete,
	enScCycleStatusUnloadRequest, enScCycleStatusUnloadInvoke, enScCycleStatusUnloadComplete,
	enScCycleStatusHomeReturnRequest, enScCycleStatusHomeReturnInvoke, enScCycleStatusHomeReturnComplete,
	enScCycleStatusSize
};
*/
// </Job Definition>
////////////////////////////////////////////////////////////////



#endif	// __ECSDEF_H__
