#ifndef __ECSDEF_H__
#define __ECSDEF_H__

#define	SYS_OFFLINE			0
#define	SYS_LOCAL			1
#define	SYS_REMOTE			2

#define	SYS_FAULT			0
#define	SYS_NORMAL			1
#define	SYS_PM				3

#define	SYS_INIT			1
#define	SYS_PAUSED			2
#define	SYS_AUTO			3
#define	SYS_PAUSING			4

#define	SYS_BYHOST			1
#define	SYS_BYOPERATOR		2
#define	SYS_BYEQP			3

#define SYS_MODE_HOST		0
#define SYS_MODE_ECS		1
#define SYS_MODE_PM			9

////////////////////////////////////////////////////////////////
// <File Definition>
#define ECS_SERVER_ID			_T("SKI")
#define ECS_CLIENT_VERSION		_T("1.0.0")
#define SKI_ECS2				_T("SKI_ECS")
// </File Definition>
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// <File Definition>
#define ECS_DEFINE_FILE_HUN_HWASUNG						_T(".\\EcsDefine_Hun_Hwasung.xml")
#define ECS_DEFINE_FILE_HUN_CMPROD						_T(".\\EcsDefine_Hun_CmpProd.xml")


#define ECS_LAYOUT_FILE_HUN_COMPROD_1F  				_T(".\\EcsLayout_Hun_Cmp_1F.xml")
#define ECS_LAYOUT_FILE_HUN_HWASUNG_GF  				_T(".\\EcsLayout_Hun_Hwasung_GF.xml")
#define ECS_LAYOUT_FILE_HUN_HWASUNG_GF_DECK				_T(".\\EcsLayout_Hun_Hwasung_GF_DECK.xml")
#define ECS_LAYOUT_FILE_HUN_HWASUNG_1F  				_T(".\\EcsLayout_Hun_Hwasung_1F.xml")
#define ECS_LAYOUT_FILE_HUN_HWASUNG_1F_DECK  			_T(".\\EcsLayout_Hun_Hwasung_DECK.xml")
#define ECS_LAYOUT_FILE_HUN_HWASUNG_ASSEMBLE  			_T(".\\EcsLayout_Hun_Assemble.xml")

#define ECS_DEFINE_FILE_CHIN_HWASUNG					_T(".\\EcsDefine_CHIN_Hwasung.xml")
#define ECS_DEFINE_FILE_CHIN_CMPROD						_T(".\\EcsDefine_CHIN_CmpProd.xml")

#define ECS_LAYOUT_FILE_CHIN_COMPROD_1F  				_T(".\\EcsLayout_CHIN_CmpPord.xml")
#define ECS_LAYOUT_FILE_CHIN_HWASUNG_1F  				_T(".\\EcsLayout_CHIN_Hwasung_1F.xml")
#define ECS_LAYOUT_FILE_CHIN_HWASUNG_2F  				_T(".\\EcsLayout_CHIN_Hwasung_2F.xml")
#define ECS_LAYOUT_FILE_CHIN_HWASUNG_ASSEMBLE  			_T(".\\EcsLayout_CHIN_Assemble.xml")

//LFC G동 File Definition
#define ECS_DEFINE_FILE									_T(".\\EcsDefine.xml")
#define ECS_LAYOUT_FILE						  			_T(".\\EcsLayout1.xml")


#define ECS_CODE_FILE			g_strEcsPath + _T("\\EcsCode.dat")
#define ECS_INI_FILE			g_strEcsPath + _T("\\Ecs.ini")
#define ECS_CFG_FILE			g_strEcsPath + _T("\\SKI_SIM_EQP.cfg")

#define IP_INI_FILE				g_strEcsPath  + _T("\\KioskIp.ini")

#define KIOSK_COLOR  			_T("80FFFF")

#define MAKE_WINDOW_SIZE_X  			1920
#define MAKE_WINDOW_SIZE_Y  			1080

// </File Definition>
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// <LAYOUT DEFINITION>
enum EN_LAYOUT
{
	EN_CHIN_INIT = 0,
	EN_LFC_G,
	EN_CHIN_LAST
	//EN_CHIN_1F,
	//EN_CHIN_2F,
	//EN_CHIN_ASSEMBLE,
	
	//EN_LFC_G
};

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
	enHostCommandUnpredict			= 'X'
};
enum EN_HOST_ERROR_CODE {
	enHostErrorNone						= 0,
	enHostErrorNoSTX					= 1,
	enHostErrorNoETX					= 2,
	enHostErrorDuplicatedLuggNum		= 3,
	enHostErrorInvalidLuggNo			= 4,
	enHostErrorBufferFull				= 5,
	enHostErrorInvalidHeaderLength		= 6,
	enHostErrorInvalidMsgLength			= 7,
	enHostErrorUnknownMsgType			= 7,
	enHostErrorInvalidStartLocation		= 8,
	enHostErrorInvalidDestLocation		= 8,
	enHostErrorInvalidLugg				= 10,	//
	enHostErrorInhibitedLoc				= 12,
	enHostErrorInvalidMailboxName		= 13,
	enHostErrorAlreadyJob				= 14,
	enHostErrorInvalidStartWarehouse	= 15,
	enHostErrorInvalidStartStation		= 16,
	enHostErrorInvalidDestWarehouse		= 17,
	enHostErrorInvalidDestStation		= 18,
	enHostErrorRefusedEcsJob			= 19,
	enHostErrorProtocolLength			= 20,
	enHostErrorUndefinedCommandType		= 21,
	enHostErrorSocket					= 22,
	enHostErrorNotExistJob				= 23,
	enHostErrorXmlParser				= 24,
	enHostErrorXmlAttribute				= 25,
	enHostErrorXmlElement				= 26,
	enHostErrorXmlValue					= 27,
	enHostErrorJobReport				= 28,
	enHostErrorInvalidContent			= 29,
	enHostErrorImproperHandshake		= 30,
	enHostErrorNoResponse				= 31,
	enHostErrorUnknownJobType			= 32,
	enHostErrorInternal					= 99 
};

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


////////////////////////////////////////////////////////////////
// <BCR Definition>
#define BCR_NO58				58
#define BCR_NO62				62
#define BCR_NO69				69
#define BCR_NO17				17
#define BCR_NO28				28
#define BCR_NO42				42

// <RTV Definition>
#define SC1_PLC					0
#define SC2_PLC					1
#define SC3_PLC					2
#define SC4_PLC					3
#define SC_PLC_CNT				18

#define RACK_BANK_LEN			2
#define RACK_BAY_LEN			2
#define RACK_LEVEL_LEN			2
#define RACK_LOC_LEN			6
// </RTV Definition>
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// <RTV Definition>
#define RTV1_PLC				0
#define RTV2_PLC				1
#define RTV3_PLC				2
#define RTV_PLC_CNT				3

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
#define	IMS_STN_CNT						9

////////////////////////////////////////////////////////////////
// <Log Definition>
#define LOG_DISP_MAX	50000

#define LOG_SYSTEM		9999

#define LOG_POS_ALL			0
#define LOG_POS_SYSTEM		1
#define LOG_POS_HOST		2
#define LOG_POS_CV			3
#define LOG_POS_SC			4
#define LOG_POS_RTV			5
#define LOG_POS_BCR			6
#define LOG_POS_MONITOR		7
#define LOG_POS_CNT			8

#define LOG_POS_LGV			8
#define LOG_POS_LIFTER		9
#define LOG_POS_RFID		10
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
// </Log Definition>
////////////////////////////////////////////////////////////////
#define ECS_ALL_TAB_COUNT	4


////////////////////////////////////////////////////////////////
// <Job Definition>
enum EN_ERROR_GBN { enErrorDualStore=1, enErrorEmptyRetrieve=2 };
enum EN_EQUIP_GBN { enEquipSC=1, enEquipBothStn=2, enEquipStoStn=3, enEquipRetStn=4 };
enum EN_SC_STATUS_GBN { enScStatusOK=0, enScStatusNotSto=1, enScStatusNotJob=2 };
enum EN_COMPLETE_GBN { enCompleteStore=1, enCompleteRetrieve=2 };
enum EN_ERROR_CODE { 
	enScErrorNone=0, 
	enScErrorDualStore=54, enScErrorDualStore2=55, 
	enScErrorStoreObstacle=56, enScErrorStoreObstacle2=66, 
	enScErrorEmptyRetrieve=58, enScErrorEmptyRetrieve2=59, 
	enScErrorRetrieveObstacle=57, enScErrorRetrieveObstacle2=67, 
	enScErrorCommunication=999 };


enum EN_JOB_TYPE { 
	enJobTypeNone = 0, 
	enJobTypeAutoSto =  1, enJobTypeAutoRet =  2, enJobTypeAutoPR =  3, enJobTypeAutoR2R =  4, enJobTypeAutoA2A =  5, enJobTypeAutoMove =  6, 
	enJobTypeSemiSto = 11, enJobTypeSemiRet = 12, enJobTypeSemiPR = 13, enJobTypeSemiR2R = 14, enJobTypeSemiW2W = 15, enJobTypeSemiMove = 10, 
	enJobTypeManual = 21
};

// 필요시 수정요망 - 원래는 값이 없었음!
enum EN_JOB_PATTERN { 
	enJobPatternNone=0,
	enJobPatternSto=1, enJobPatternRet=2, enJobPatternPR=7, enJobPatternR2R=4, enJobPatternW2W=5, enJobPatternMove=3
};

enum EN_JOB_STATUS {
	enJobStatusAll, enJobStatusNew,
	enJobStatusComplete, enJobStatusArrived, enJobStatusCancel,
	enJobStatusStoRequset, enJobStatusFlantEquip,
//	enJobStatusErrorEmptyRetrieve, enJobStatusErrorDualStore, enJobStatusDualStoreRetry, enJobStatusEmptyRetrieveRetry,
	enJobStatusErrorEmptyRetrieve, enJobStatusErrorDualStore, 
	enJobStatusErrorEmptyRetrieve2, enJobStatusErrorDualStore2, 
	enJobStatusDualStoreRetry, enJobStatusEmptyRetrieveRetry,

	enJobStatusCvNew, enJobStatusCvInvoke,
	enJobStatusScRequest, enJobStatusScInvoke, enJobStatusScComplete,
	enJobStatusRtvInvoke, enJobStatusRtvComplete,
//	enJobStatusLifterInvoke, enJobStatusLifterComplete, enJobStatusLifterCvInvoke,
	enJobStatusLgvInvoke, enJobStatusLgvComplete, enJobStatusLgvInvokeFail, 
	enJobStatusResend,
	enJobStatusCompleteRequest, enJobStatusRefuseComplete,
	enJobStatusError, enJobStatusRefuseError,
	enJobStatusRefuseArrived, enJobStatusMESError,
	enJobStatusSize
};
// </Job Definition>
////////////////////////////////////////////////////////////////

// Define Center Equip Status
enum EN_CENTER_EQUIP_STATUS
{
	enCEquipStatusOff,
	enCEquipStatusWait,
	enCEquipStatusAutoRun,
	enCEquipStatusAutoStop,
	enCEquipStatusManual,
	enCEquipStatusError
};

enum EN_SEARCH_TYPE
{
	enLuggSearch, enTraySearch, enStationSearch, enTrackNumSearch 
};

// Define Bit Field
union BIT_FIELD
{
	struct tagBIT
	{
		// C/V PLC와 협의한 순서 (변경금지)
		unsigned int	Bit1	:1;		// 102. 상온 Aging 1호기
		unsigned int	Bit2	:1;		// 103. 상온 Aging 2호기
		unsigned int	Bit3	:1;		// 104. 상온 Aging 3호기
		unsigned int	Bit4	:1;		// 105. 상온 Aging 4호기
		unsigned int	Bit5	:1;		// 106. 상온 Aging 5호기
		unsigned int	Bit6	:1;		// 107. 상온 Aging 6호기
		unsigned int	Bit7	:1;		// 518. 충방전 1호기
		unsigned int	Bit8	:1;		// 519. 충방전 2호기
		unsigned int	Bit9	:1;		// 517. 충방전 3호기
		unsigned int	Bit10	:1;		// 516. 충방전 4호기
		unsigned int	Bit11	:1;		// 511. 상온 Aging 1호기
		unsigned int	Bit12	:1;		// 512. 상온 Aging 2호기
		unsigned int	Bit13	:1;		// 513. 상온 Aging 3호기
		unsigned int	Bit14	:1;		// 514. 상온 Aging 4호기
		unsigned int	Bit15	:1;		// 515. 고온 Aging 1호기
		unsigned int	Bit16	:1;		// 예약
	} Bits;

	BYTE ucBit;
};


#endif	// __ECSDEF_H__
