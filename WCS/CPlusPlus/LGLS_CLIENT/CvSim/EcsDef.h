#ifndef __ECSDEF_H__
#define __ECSDEF_H__


////////////////////////////////////////////////////////////////
// <File Definition>
#define ECS_SERVER_ID			_T("SKI")
#define ECS_CLIENT_VERSION		_T("1.0.0")
// </File Definition>
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// <File Definition>
#define ECS_DEFINE_FILE			            _T(".\\EcsDefine.xml")
#define ECS_LAYOUT_FILE_LAOUT		        _T(".\\EcsLayout.xml")
#define ECS_LAYOUT_FILE_LAOUT_G		        _T(".\\EcsLayout_G.xml")
#define ECS_LAYOUT_FILE_LAOUT_G_DECK		_T(".\\EcsLayout_G_DECK.xml")
#define ECS_LAYOUT_FILE_LAOUT_DECK		    _T(".\\EcsLayout_DECK.xml")

#define ECS_LAYOUT_FILE_LAOUT_1				_T(".\\EcsLayout1.xml")
#define ECS_LAYOUT_FILE_LAOUT_2				_T(".\\EcsLayout2.xml")
#define ECS_LAYOUT_FILE_LAOUT_Assemble		_T(".\\EcsLayoutAssemble.xml")

#define DEVICE_MAP_FILE			            _T(".\\DeviceMap.xml")
#define KEY_WORD_FILE			            _T(".\\KeyWord.xml")
#define LOGIC_FILE							_T(".\\Logic.xml")

#define ECS_CODE_FILE			g_strEcsPath + _T("\\EcsCode.dat")
#define ECS_USER_FILE			g_strEcsPath + _T("\\Data\\EcsUser.dat")
#define ECS_USER_BACK_FILE		g_strEcsPath + _T("\\Data\\EcsUser.bak")
#define ECS_INI_FILE			g_strEcsPath + _T("\\Ecs.ini")
// </File Definition>
////////////////////////////////////////////////////////////////


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
	enHostCommandUnpredict			= 'X'
};
enum EN_HOST_ERROR_CODE {
	enHostErrorNone = 0,
	enHostError9001	= 9001,
	enHostError9002	= 9002,
	enHostError9003	= 9003,
	enHostError9004	= 9004,
	enHostError9005	= 9005,
	enHostError9006	= 9006,
	enHostError9007	= 9007,
	enHostError9008	= 9008,
	enHostError9009	= 9009,
	enHostError9010	= 9010,
	enHostError9011	= 9011,
	enHostError9012	= 9012,	
	enHostError9013	= 9013,
	enHostError9014	= 9014,
	enHostError9015	= 9015,
	enHostError9016	= 9016,
	enHostError9017	= 9017,
	enHostError9018	= 9018,
	enHostError9019	= 9019,
	enHostError9020	= 9020,
	enHostError9021	= 9021,
	enHostError9022	= 9022,
	enHostError9023	= 9023,
	enHostError9024	= 9024,
	enHostError9025	= 9025,
	enHostError9026	= 9026,
	enHostError9027	= 9027,
	enHostError9029	= 9029,
	enHostError9030	= 9030,
	enHostError9031	= 9031,
	enHostError9032	= 9032,
	enHostError9033	= 9033,
	enHostError9034	= 9034,
	enHostError9035	= 9035, 
	enHostError9036	= 9036,
	enHostError9037	= 9037,
	enHostError9038	= 9038,
	enHostError9039	= 9039,
	enHostError9200	= 9200,
	enHostError9203	= 9203,
	enHostError9207	= 9207,
	enHostError9209	= 9209,
	enHostError9210	= 9210,
	enHostError9211	= 9211,
	enHostError9212	= 9212,
	enHostError9217	= 9217,
	enHostError9240	= 9240,
	enHostError9241	= 9241
};


// </HOST Definition>
////////////////////////////////////////////////////////////////
#define PLC_CONN_PORT_CNT		18


////////////////////////////////////////////////////////////////
// <CV Definition>
#define CV1_PLC					0
#define CV2_PLC					1
#define CV3_PLC					2
#define CV4_PLC					3
#define CV5_PLC					4
#define CV6_PLC					5
#define CV7_PLC					6
#define CV8_PLC					7
#define CV_PLC_CNT				8
// </CV Definition>
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// <SC Definition>
#define SC1_PLC					0
#define SC2_PLC					1
#define SC3_PLC					2
#define SC_PLC_CNT				2

#define RACK_BANK_LEN			2
#define RACK_BAY_LEN			2
#define RACK_LEVEL_LEN			2
#define RACK_LOC_LEN			6
// </SC Definition>
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
#define	IMS_STN_CNT						11

////////////////////////////////////////////////////////////////
// <Log Definition>
#define LOG_DISP_MAX	50000

#define LOG_SYSTEM		9999

#define LOG_POS_CNT			8

#define LOG_POS_ALL			0
#define LOG_POS_SYSTEM		1
#define LOG_POS_HOST		2
#define LOG_POS_CV			3
#define LOG_POS_BCR			4
#define LOG_POS_SC			5
#define LOG_POS_THS			6
#define LOG_POS_MONITOR		7
#define LOG_POS_RTV			8
#define LOG_POS_LGV			9
#define LOG_POS_LIFTER		10
#define LOG_POS_RFID		11
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

//#define CV_PLC_CNT			6

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
	enJobTypeAutoSto =  1, enJobTypeAutoRet =  2, enJobTypeAutoPR =  3, enJobTypeAutoR2R =  4, enJobTypeAutoW2W =  5, enJobTypeAutoMove =  6, 
//	enJobTypeSemiSto = 11, enJobTypeSemiRet = 12, enJobTypeSemiPR = 13, enJobTypeSemiR2R = 14, enJobTypeSemiW2W = 15, enJobTypeSemiMove = 10, 
	enJobTypeSemiSto =  7, enJobTypeSemiRet =  8, enJobTypeSemiPR =  9, enJobTypeSemiR2R = 10, enJobTypeSemiW2W = 11, enJobTypeSemiMove = 12,
	enJobTypeManual = 21
};

enum EN_JOB_PATTERN { 
	enJobPatternNone,
	enJobPatternSto, enJobPatternRet, enJobPatternPR, enJobPatternR2R, enJobPatternW2W, enJobPatternMove
};

enum EN_JOB_STATUS {
	enJobStatusAll, enJobStatusNew,
	enJobStatusComplete, enJobStatusArrived, enJobStatusCancel,
	enJobStatusErrorEmptyRetrieve, enJobStatusErrorDualStore, 
	enJobStatusErrorEmptyRetrieve2, enJobStatusErrorDualStore2, 
	enJobStatusDualStoreRetry, enJobStatusEmptyRetrieveRetry,
	enJobStatusCvNew, enJobStatusCvInvoke,
	enJobStatusScRequest, enJobStatusScInvoke, enJobStatusScComplete,
	enJobStatusRtvInvoke, enJobStatusRtvComplete,
//	enJobStatusLifterInvoke, enJobStatusLifterComplete, enJobStatusLifterCvInvoke,
//	enJobStatusLgvInvoke, enJobStatusLgvComplete,
	enJobStatusResend,
	enJobStatusCompleteRequest, enJobStatusRefuseComplete,
	enJobStatusError, enJobStatusRefuseError,
	enJobStatusRefuseArrived, 
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

// Define Tray Type
enum EN_TRAY_TYPE
{
	enTrayTypeNone = 0,
	enTrayType1 = 49,
	enTrayType2 = 50,
	enTrayType3 = 51,
	enTrayType4 = 52,
	enTrayType5 = 53,
	enTrayType6 = 54,
	enTrayType7 = 55,
	enTrayType8 = 56,
	enTrayType9 = 57,
	enTrayTypeA = 65,
	enTrayTypeB = 66,
	enTrayTypeC = 67,
	enTrayTypeD = 68,
	enTrayTypeE = 69,
	enTrayTypeF = 70,
	enTrayTypeG = 71,
	enTrayTypeH = 72,
	enTrayTypeI = 73,
	enTrayTypeZ = 90
};

enum EN_COMM_METHOD {
	enCommMethodNone = 0,
	enCommMethodTCP = 1, 
	enCommMethodUDP = 2, 
	enCommMethodSerial = 3, 
	enCommMethodXCom = 4, 
	enCommMethodRFC = 5
};
enum EN_CPU_KIND {
	enCpuKindNone = 0,
	enCpuKindMelsec = 1,
	enCpuKindAllenBradley = 2,
	enCpuKindSimens = 3,
	enCpuKindXgt = 4,
	enCpuKindFara = 5
};
enum EN_ADDRESS_AREA {
	enAddressAreaNone = 0,
	enAddressAreaD = 1,
	enAddressAreaR = 2
};

// Define Bit Field
enum EN_BYTE {
	enByteL = 0x00FF,
	enByteH = 0xFF00
};

enum EN_NIBBLE {
	enNibble1 = 0x000F,
	enNibble2 = 0x00F0,
	enNibble3 = 0x0F00,
	enNibble4 = 0xF000
};

enum EN_BIT {
	enBit11 = 0x0001, enBit12 = 0x0002, enBit13 = 0x0004, enBit14 = 0x0008,
	enBit15 = 0x0010, enBit16 = 0x0020, enBit17 = 0x0040, enBit18 = 0x0080,
	enBit21 = 0x0100, enBit22 = 0x0200, enBit23 = 0x0400, enBit24 = 0x0800,
	enBit25 = 0x1000, enBit26 = 0x2000, enBit27 = 0x4000, enBit28 = 0x8000
};

// Define Bit Field
union BIT_FIELD
{
	struct tagBIT
	{
		unsigned int	Bit1	:1;		// Aging 1
		unsigned int	Bit2	:1;		// Aging 2
		unsigned int	Bit3	:1;		// Aging 3
		unsigned int	Bit4	:1;		// Aging 4
		unsigned int	Bit5	:1;		// Aging 5
		unsigned int	Bit6	:1;		// Aging 6
		unsigned int	Bit7	:1;		// Cde 1
		unsigned int	Bit8	:1;		// Cde 2
		unsigned int	Bit9	:1;		// Aging 7
		unsigned int	Bit10	:1;		// Aging 8
		unsigned int	Bit11	:1;		// Aging 9
		unsigned int	Bit12	:1;		// Aging 10
		unsigned int	Bit13	:1;		// Cde 3
		unsigned int	Bit14	:1;		// Cde 4
	} tagBit;

	BYTE ucBit;
};

#endif	// __ECSDEF_H__
