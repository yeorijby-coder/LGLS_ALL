#pragma once

#include "XComProCtrl.h"

//========================================================================================
//	IMPORT LIBRARY (x86: XComPro_R40_01_32.lib, x64:XComPro_R40_01_64.lib)
//========================================================================================
// if you use xcompro api, delete the comment and input the library(XComPro_R40_01_32.lib).
//#pragma comment(lib, "..\\debug\\XComPro_R40_01_32.lib")
//========================================================================================

//========================================================================================
//	DEFINITION
//========================================================================================
#define __XCOMPRO_API_LIBRARY__
#ifdef __XCOMPRO_API_LIBRARY__
#define XCOMPRO_API_FUNCTION __declspec(dllexport)
#else
#define XCOMPRO_API_FUNCTION __declspec(dllimport)
#endif

//========================================================================================
//	DEFINITION EVENT TYPE : Event type of the PopEvent function
//========================================================================================
#define SECS_EVENT_TYPE				1000
#define SECS_MSG_TYPE				1001

//========================================================================================
//	DECLEARE CLASS
//========================================================================================
class CXComProCtrl;

//========================================================================================
//	DEFINITION FUNCTION POINTER
//========================================================================================
#ifndef __OpCallbackDefine__
#define __OpCallbackDefine__
typedef void (__cdecl *CALLBACK_SECS_EVENT)(void *lpParam, short nEventID, long lParam);
typedef void (__cdecl *CALLBACK_SECS_MSG)(void *lpParam);
#endif

//========================================================================================
//	DEFINITION for DATA TYPE
//========================================================================================
typedef unsigned long       ULONG;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      USHORT;
typedef float               FLOAT;
typedef __int64				LONGLONG;
typedef unsigned __int64	ULONGLONG;
typedef CXComProCtrl		*XCM_Handle;

extern "C"
{
	XCM_Handle XCOMPRO_API_FUNCTION XCM_CreateObject();
	void XCOMPRO_API_FUNCTION XCM_DeleteObject(XCM_Handle pXCom);
	
	//property
	short XCOMPRO_API_FUNCTION XCM_getDeviceID(XCM_Handle pXCom);
	short XCOMPRO_API_FUNCTION XCM_getStream(XCM_Handle pXCom);
	short XCOMPRO_API_FUNCTION XCM_getFunction(XCM_Handle pXCom);
	long XCOMPRO_API_FUNCTION XCM_getSystemBytes(XCM_Handle pXCom);
	short XCOMPRO_API_FUNCTION XCM_getWbit(XCM_Handle pXCom);
	short XCOMPRO_API_FUNCTION XCM_getRbit(XCM_Handle pXCom);
	short XCOMPRO_API_FUNCTION XCM_getEbit(XCM_Handle pXCom);
	short XCOMPRO_API_FUNCTION XCM_getPtype(XCM_Handle pXCom);
	short XCOMPRO_API_FUNCTION XCM_getStype(XCM_Handle pXCom);
	short XCOMPRO_API_FUNCTION XCM_getBlock(XCM_Handle pXCom);
	long XCOMPRO_API_FUNCTION XCM_getHeaderSize(XCM_Handle pXCom);
	long XCOMPRO_API_FUNCTION XCM_getMessageSize(XCM_Handle pXCom);

	//Function
	long XCOMPRO_API_FUNCTION XCM_Initialize(XCM_Handle pXCom, const char *pszCfgFile);
	long XCOMPRO_API_FUNCTION XCM_Close(XCM_Handle pXCom);
	long XCOMPRO_API_FUNCTION XCM_Start(XCM_Handle pXCom);
	long XCOMPRO_API_FUNCTION XCM_Stop(XCM_Handle pXCom);
	long XCOMPRO_API_FUNCTION XCM_MakeSecsMsg(XCM_Handle pXCom, long *plMsgId, short nDevId, short nStrm, short nFunc, long lSysByte);
	long XCOMPRO_API_FUNCTION XCM_Send(XCM_Handle pXCom, long lMsgId);
	long XCOMPRO_API_FUNCTION XCM_LoadSecsMsg(XCM_Handle pXCom, long *plMsgId, short *pnDevId, short *pnStrm, short *pnFunc, long *plSysByte, short *pnWbit);
	void XCOMPRO_API_FUNCTION XCM_CloseSecsMsg(XCM_Handle pXCom, long lMsgId);
	long XCOMPRO_API_FUNCTION XCM_GetListItem(XCM_Handle pXCom, long lMsgId, long *pnItems);
	long XCOMPRO_API_FUNCTION XCM_GetBinaryItem(XCM_Handle pXCom, long lMsgId, BYTE *pnValue, long *plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetBoolItem(XCM_Handle pXCom, long lMsgId, BYTE *pnValue, long *plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetAsciiItem(XCM_Handle pXCom, long lMsgId, char *pszValue, long *plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetJis8Item(XCM_Handle pXCom, long lMsgId, char *pszValue, long *plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetMbyteItem(XCM_Handle pXCom, long lMsgId, long* plEnCode, BYTE* pbValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetI1Item(XCM_Handle pXCom, long lMsgId, char* pnValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetI2Item(XCM_Handle pXCom, long lMsgId, short* pnValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetI4Item(XCM_Handle pXCom, long lMsgId, long* plValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetI8Item(XCM_Handle pXCom, long lMsgId, LONGLONG* plValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetU1Item(XCM_Handle pXCom, long lMsgId, BYTE* pnValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetU2Item(XCM_Handle pXCom, long lMsgId, USHORT* plValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetU4Item(XCM_Handle pXCom, long lMsgId, ULONG* plValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetU8Item(XCM_Handle pXCom, long lMsgId, ULONGLONG* plValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetF4Item(XCM_Handle pXCom, long lMsgId, FLOAT* prValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetF8Item(XCM_Handle pXCom, long lMsgId, double* prValue, long* plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_SetListItem(XCM_Handle pXCom, long lMsgId, long nItems);
	long XCOMPRO_API_FUNCTION XCM_SetBinaryItem(XCM_Handle pXCom, long lMsgId, BYTE* pnValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetBoolItem(XCM_Handle pXCom, long lMsgId, BYTE* pnValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetAsciiItem(XCM_Handle pXCom, long lMsgId, const char *pszValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetJis8Item(XCM_Handle pXCom, long lMsgId, const char *pszValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetMbyteItem(XCM_Handle pXCom, long lMsgId, long lEnCode, BYTE* pbValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetI1Item(XCM_Handle pXCom, long lMsgId, char* pnValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetI2Item(XCM_Handle pXCom, long lMsgId, short* pnValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetI4Item(XCM_Handle pXCom, long lMsgId, long* plValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetI8Item(XCM_Handle pXCom, long lMsgId, LONGLONG* plValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetU1Item(XCM_Handle pXCom, long lMsgId, BYTE* pnValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetU2Item(XCM_Handle pXCom, long lMsgId, USHORT* plValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetU4Item(XCM_Handle pXCom, long lMsgId, ULONG* pnValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetU8Item(XCM_Handle pXCom, long lMsgId, ULONGLONG* plValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetF4Item(XCM_Handle pXCom, long lMsgId, FLOAT* prValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_SetF8Item(XCM_Handle pXCom, long lMsgId, double* prValue, long lCount);
	long XCOMPRO_API_FUNCTION XCM_EnableLog(XCM_Handle pXCom, long nLogType, short nEnabled);
	long XCOMPRO_API_FUNCTION XCM_GetHeaderBytes(XCM_Handle pXCom, long lMsgId, BYTE* pnHeader, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetMsgBytes(XCM_Handle pXCom, long lMsgId, BYTE* pnMsg, long lSize);
	long XCOMPRO_API_FUNCTION XCM_SetHeaderBytes(XCM_Handle pXCom, long lMsgId, BYTE* pnHeader, long lSize);
	long XCOMPRO_API_FUNCTION XCM_SetMsgBytes(XCM_Handle pXCom, long lMsgId, BYTE* pnMsg, long lSize);
	long XCOMPRO_API_FUNCTION XCM_GetAlarmMsgInfo(XCM_Handle pXCom, long lMsgId, short* pnDevId, short* pnStrm, short* pnFunc, long* plSysByte, short* pnWbit);
	long XCOMPRO_API_FUNCTION XCM_DebugInfo(XCM_Handle pXCom, short* pnInfo);
	long XCOMPRO_API_FUNCTION XCM_GetSysBytes(XCM_Handle pXCom, long lMsgId);
	long XCOMPRO_API_FUNCTION XCM_SetWBit(XCM_Handle pXCom, long lMsgId, short nWBit);
	long XCOMPRO_API_FUNCTION XCM_GetInvalidMsgInfo(XCM_Handle pXCom, long lMsgId, short* pnDevId, short* pnStrm, short* pnFunc, long* plSysByte, short* pnWbit);
	long XCOMPRO_API_FUNCTION XCM_MakeSpoolStream(XCM_Handle pXCom, long lMsgId);
	const char XCOMPRO_API_FUNCTION *XCM_GetParam(XCM_Handle pXCom, const char *pszParamName);
	long XCOMPRO_API_FUNCTION XCM_SetParam(XCM_Handle pXCom, const char* psParamName, const char* pszValue);
	long XCOMPRO_API_FUNCTION XCM_GetCurrentItemType(XCM_Handle pXCom, long lMsgId);
	long XCOMPRO_API_FUNCTION XCM_GetCurrentItemCount(XCM_Handle pXCom, long lMsgId);
	long XCOMPRO_API_FUNCTION XCM_GetSecsLog(XCM_Handle pXCom, long lMsgId, char* pszValue, long lSize);
	long XCOMPRO_API_FUNCTION XCM_PopEvent(XCM_Handle pXCom, long *pIEventType, short* nEventID, long* lParam);
	long XCOMPRO_API_FUNCTION XCM_SetSecsEventHandler(XCM_Handle pXCom, void *lpParam, CALLBACK_SECS_EVENT pfn);
	long XCOMPRO_API_FUNCTION XCM_SetSecsMsgEventHandler(XCM_Handle pXCom, void *lpParam, CALLBACK_SECS_MSG pfn);
	long XCOMPRO_API_FUNCTION XCM_SetStreamMsg(XCM_Handle pXCom, long lMsgId, BYTE *pcData, long nSize);
	long XCOMPRO_API_FUNCTION XCM_GetStreamMsgSize(XCM_Handle pXCom, long lMsgId);
	long XCOMPRO_API_FUNCTION XCM_GetStreamMsg(XCM_Handle pXCom, long lMsgId, BYTE *pnData, long *plCount, long lSize);
	long XCOMPRO_API_FUNCTION XCM_MoveSecsItem(XCM_Handle pXCom, long lMsgId, long nIndex);
	long XCOMPRO_API_FUNCTION XCM_GetMsgSize(XCM_Handle pXCom, long lMsgId);
	long XCOMPRO_API_FUNCTION XCM_SetEventHandlerMode(XCM_Handle pXCom, short nParam);
}

class XComProCLR
{
private:
	XCM_Handle pXCom;
public:
	XComProCLR()
	{
		pXCom = XCM_CreateObject();
	}
	~XComProCLR()
	{
		XCM_DeleteObject(pXCom);
	}

	/*=========================================================================
			FUNCTION	: XComPro::getDeviceID()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	short getDeviceID()
	{
		return XCM_getDeviceID(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getStream()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	short getStream()
	{
		return XCM_getStream(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getFunction()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	short getFunction()
	{
		return XCM_getFunction(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getSystemBytes()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long getSystemBytes()
	{
		return XCM_getSystemBytes(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getWbit()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	short getWbit()
	{
		return XCM_getWbit(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getRbit()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	short getRbit()
	{
		return XCM_getRbit(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getEbit()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	short getEbit()
	{
		return XCM_getEbit(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getPtype()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	short getPtype()
	{
		return XCM_getPtype(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getStype()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	short getStype()
	{
		return XCM_getStype(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getBlock()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	short getBlock()
	{
		return XCM_getBlock(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getHeaderSize()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long getHeaderSize()
	{
		return XCM_getHeaderSize(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::getMessageSize()
			DESCRIPT	: PROPERTY 
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long getMessageSize()
	{
		return XCM_getMessageSize(pXCom);
	}


	/*=========================================================================
			FUNCTION	: XComPro::SetSecsEventHandler()
			DESCRIPT	: SetSecsEventHandler
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetSecsEventHandler(void *lpParam, CALLBACK_SECS_EVENT pfn)
	{
		return XCM_SetSecsEventHandler(pXCom, lpParam, pfn);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetSecsMsgEventHandler()
			DESCRIPT	: SetSecsMsgEventHandler
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetSecsMsgEventHandler(void *lpParam, CALLBACK_SECS_MSG pfn)
	{
		return XCM_SetSecsMsgEventHandler(pXCom, lpParam, pfn);
	}
	/*=========================================================================
			FUNCTION	: XComPro::Initialize()
			DESCRIPT	: Initialize
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long Initialize(const char *pszFile)
	{
		return XCM_Initialize(pXCom, pszFile);
	}
	/*=========================================================================
			FUNCTION	: XComPro::Start()
			DESCRIPT	: Start
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long Start()
	{
		return XCM_Start(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::Stop()
			DESCRIPT	: Stop
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long Stop()
	{
		return XCM_Stop(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::Close()
			DESCRIPT	: Close
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long Close()
	{
		return XCM_Close(pXCom);
	}
	/*=========================================================================
			FUNCTION	: XComPro::MakeSecsMsg()
			DESCRIPT	: MakeSecsMsg
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long MakeSecsMsg(long *plMsgId, short nDevId, short nStrm, short nFunc, long lSysByte)
	{
		return XCM_MakeSecsMsg(pXCom, plMsgId, nDevId, nStrm, nFunc, lSysByte);
	}
	/*=========================================================================
			FUNCTION	: XComPro::Send()
			DESCRIPT	: Send
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long Send(long lMsgId)
	{
		return XCM_Send(pXCom, lMsgId);
	}
	/*=========================================================================
			FUNCTION	: XComPro::LoadSecsMsg()
			DESCRIPT	: LoadSecsMsg
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long LoadSecsMsg(long *plMsgId, short *pnDevId, short *pnStrm, short *pnFunc, long *plSysByte, short *pnWbit)
	{
		return XCM_LoadSecsMsg(pXCom, plMsgId, pnDevId, pnStrm, pnFunc, plSysByte, pnWbit);
	}
	/*=========================================================================
			FUNCTION	: XComPro::CloseSecsMsg()
			DESCRIPT	: CloseSecsMsg
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	void CloseSecsMsg(long lMsgId)
	{
		XCM_CloseSecsMsg(pXCom, lMsgId);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetListItem()
			DESCRIPT	: GetListItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetListItem(long lMsgId, long *pnItems)
	{
		return XCM_GetListItem(pXCom, lMsgId, pnItems);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetBinaryItem()
			DESCRIPT	: GetBinaryItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetBinaryItem(long lMsgId, BYTE *pnValue, long *plCount, long lSize)
	{
		return XCM_GetBinaryItem(pXCom, lMsgId, pnValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetBoolItem()
			DESCRIPT	: GetBoolItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetBoolItem(long lMsgId, BYTE *pnValue, long *plCount, long lSize)
	{
		return XCM_GetBoolItem(pXCom, lMsgId, pnValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetAsciiItem()
			DESCRIPT	: GetAsciiItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetAsciiItem(long lMsgId, char *pszValue, long *plCount, long lSize)
	{
		return XCM_GetAsciiItem(pXCom, lMsgId, pszValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetJis8Item()
			DESCRIPT	: GetJis8Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetJis8Item(long lMsgId, char *pszValue, long *plCount, long lSize)
	{
		return XCM_GetJis8Item(pXCom, lMsgId, pszValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetMbyteItem()
			DESCRIPT	: GetMbyteItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/10/10
	==========================================================================*/
	long GetMbyteItem(long lMsgId, long* plEnCode, unsigned char* pbValue, long* plCount, long lSize)
	{
		return XCM_GetMbyteItem(pXCom, lMsgId, plEnCode, pbValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetI1Item()
			DESCRIPT	: GetI1Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetI1Item(long lMsgId, char* pnValue, long* plCount, long lSize)
	{
		return XCM_GetI1Item(pXCom, lMsgId, pnValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetI2Item()
			DESCRIPT	: GetI2Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetI2Item(long lMsgId, short* pnValue, long* plCount, long lSize)
	{
		return XCM_GetI2Item(pXCom, lMsgId, pnValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetI4Item()
			DESCRIPT	: GetI4Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetI4Item(long lMsgId, long* plValue, long* plCount, long lSize)
	{
		return XCM_GetI4Item(pXCom, lMsgId, plValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetI8Item()
			DESCRIPT	: GetI8Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetI8Item(long lMsgId, LONGLONG* plValue, long* plCount, long lSize)
	{
		return XCM_GetI8Item(pXCom, lMsgId, plValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetU1Item()
			DESCRIPT	: GetU1Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetU1Item(long lMsgId, BYTE* pnValue, long* plCount, long lSize) 
	{
		return XCM_GetU1Item(pXCom, lMsgId, pnValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetU2Item()
			DESCRIPT	: GetU2Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetU2Item(long lMsgId, USHORT* plValue, long* plCount, long lSize) 
	{
		return XCM_GetU2Item(pXCom, lMsgId, plValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetU4Item()
			DESCRIPT	: GetU4Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetU4Item(long lMsgId, ULONG* plValue, long* plCount, long lSize) 
	{
		return XCM_GetU4Item(pXCom, lMsgId, plValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetU8Item()
			DESCRIPT	: GetU8Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetU8Item(long lMsgId, ULONGLONG* plValue, long* plCount, long lSize) 
	{
		return XCM_GetU8Item(pXCom, lMsgId, plValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetF4Item()
			DESCRIPT	: GetF4Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetF4Item(long lMsgId, FLOAT* prValue, long* plCount, long lSize) 
	{
		return XCM_GetF4Item(pXCom, lMsgId, prValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetF8Item()
			DESCRIPT	: GetF8Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetF8Item(long lMsgId, double* prValue, long* plCount, long lSize) 
	{
		return XCM_GetF8Item(pXCom, lMsgId, prValue, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetListItem()
			DESCRIPT	: SetListItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetListItem(long lMsgId, long nItems) 
	{
		return XCM_SetListItem(pXCom, lMsgId, nItems);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetBinaryItem()
			DESCRIPT	: SetBinaryItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetBinaryItem(long lMsgId, BYTE* pnValue, long lCount) 
	{
		return XCM_SetBinaryItem(pXCom, lMsgId, pnValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetBoolItem()
			DESCRIPT	: SetBoolItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetBoolItem(long lMsgId, BYTE* pnValue, long lCount) 
	{
		return XCM_SetBoolItem(pXCom, lMsgId, pnValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetAsciiItem()
			DESCRIPT	: SetAsciiItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetAsciiItem(long lMsgId, const char *pszValue, long lCount) 
	{
		return XCM_SetAsciiItem(pXCom, lMsgId, pszValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetJis8Item()
			DESCRIPT	: SetJis8Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetJis8Item(long lMsgId, const char *pszValue, long lCount) 
	{
		return XCM_SetJis8Item(pXCom, lMsgId, pszValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetMbyteItem()
			DESCRIPT	: SetMbyteItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/10/10
	==========================================================================*/
	long SetMbyteItem(long lMsgId, long lEnCode, unsigned char *pbValue, long lCount) 
	{
		return XCM_SetMbyteItem(pXCom, lMsgId, lEnCode, pbValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetI1Item()
			DESCRIPT	: SetI1Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetI1Item(long lMsgId, char* pnValue, long lCount) 
	{
		return XCM_SetI1Item(pXCom, lMsgId, pnValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetI2Item()
			DESCRIPT	: SetI2Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetI2Item(long lMsgId, short* pnValue, long lCount) 
	{
		return XCM_SetI2Item(pXCom, lMsgId, pnValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetI4Item()
			DESCRIPT	: SetI4Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetI4Item(long lMsgId, long* plValue, long lCount) 
	{
		return XCM_SetI4Item(pXCom, lMsgId, plValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetI8Item()
			DESCRIPT	: SetI8Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetI8Item(long lMsgId, LONGLONG* plValue, long lCount) 
	{
		return XCM_SetI8Item(pXCom, lMsgId, plValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetU1Item()
			DESCRIPT	: SetU1Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetU1Item(long lMsgId, BYTE* pnValue, long lCount) 
	{
		return XCM_SetU1Item(pXCom, lMsgId, pnValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetU2Item()
			DESCRIPT	: SetU2Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetU2Item(long lMsgId, USHORT* plValue, long lCount) 
	{
		return XCM_SetU2Item(pXCom, lMsgId, plValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetU4Item()
			DESCRIPT	: SetU4Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetU4Item(long lMsgId, ULONG* pnValue, long lCount) 
	{
		return XCM_SetU4Item(pXCom, lMsgId, pnValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetU8Item()
			DESCRIPT	: SetU8Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetU8Item(long lMsgId, ULONGLONG* plValue, long lCount) 
	{
		return XCM_SetU8Item(pXCom, lMsgId, plValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetF4Item()
			DESCRIPT	: SetF4Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetF4Item(long lMsgId, FLOAT* prValue, long lCount) 
	{
		return XCM_SetF4Item(pXCom, lMsgId, prValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetF8Item()
			DESCRIPT	: SetF8Item
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetF8Item(long lMsgId, double* prValue, long lCount) 
	{
		return XCM_SetF8Item(pXCom, lMsgId, prValue, lCount);
	}
	/*=========================================================================
			FUNCTION	: XComPro::EnableLog()
			DESCRIPT	: EnableLog
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long EnableLog(long nLogType, short nEnabled) 
	{
		return XCM_EnableLog(pXCom, nLogType, nEnabled);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetHeaderBytes()
			DESCRIPT	: GetHeaderBytes
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetHeaderBytes(long lMsgId, BYTE* pnHeader, long lSize) 
	{
		return XCM_GetHeaderBytes(pXCom, lMsgId, pnHeader, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetMsgBytes()
			DESCRIPT	: GetMsgBytes
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetMsgBytes(long lMsgId, BYTE* pnMsg, long lSize) 
	{
		return XCM_GetMsgBytes(pXCom, lMsgId, pnMsg, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetHeaderBytes()
			DESCRIPT	: SetHeaderBytes
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetHeaderBytes(long lMsgId, BYTE* pnHeader, long lSize) 
	{
		return XCM_SetHeaderBytes(pXCom, lMsgId, pnHeader, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetMsgBytes()
			DESCRIPT	: SetMsgBytes
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetMsgBytes(long lMsgId, BYTE* pnMsg, long lSize) 
	{
		return XCM_SetMsgBytes(pXCom, lMsgId, pnMsg, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetAlarmMsgInfo()
			DESCRIPT	: GetAlarmMsgInfo
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetAlarmMsgInfo(long lMsgId, short* pnDevId, short* pnStrm, short* pnFunc, long* plSysByte, short* pnWbit) 
	{
		return XCM_GetAlarmMsgInfo(pXCom, lMsgId, pnDevId, pnStrm, pnFunc, plSysByte, pnWbit);
	}
	/*=========================================================================
			FUNCTION	: XComPro::DebugInfo()
			DESCRIPT	: DebugInfo
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long DebugInfo(short* pnInfo) 
	{
		return XCM_DebugInfo(pXCom, pnInfo);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetSysBytes()
			DESCRIPT	: GetSysBytes
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetSysBytes(long lMsgId) 
	{
		return XCM_GetSysBytes(pXCom, lMsgId);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetWBit()
			DESCRIPT	: SetWBit
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetWBit(long lMsgId, short nWBit) 
	{
		return XCM_SetWBit(pXCom, lMsgId, nWBit);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetInvalidMsgInfo()
			DESCRIPT	: GetInvalidMsgInfo
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetInvalidMsgInfo(long lMsgId, short* pnDevId, short* pnStrm, short* pnFunc, long* plSysByte, short* pnWbit) 
	{
		return XCM_GetInvalidMsgInfo(pXCom, lMsgId, pnDevId, pnStrm, pnFunc, plSysByte, pnWbit);
	}
	/*=========================================================================
			FUNCTION	: XComPro::MakeSpoolStream()
			DESCRIPT	: MakeSpoolStream
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long MakeSpoolStream(long lMsgId) 
	{
		return XCM_MakeSpoolStream(pXCom, lMsgId);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetParam()
			DESCRIPT	: GetParam
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	const char* GetParam(const char *pszParamName) 
	{
		return XCM_GetParam(pXCom, pszParamName);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetParam()
			DESCRIPT	: SetParam
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetParam(const char* psParamName, const char* pszValue) 
	{
		return XCM_SetParam(pXCom, psParamName, pszValue);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetCurrentItemType()
			DESCRIPT	: GetCurrentItemType
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetCurrentItemType(long lMsgId) 
	{
		return XCM_GetCurrentItemType(pXCom, lMsgId);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetCurrentItemCount()
			DESCRIPT	: GetCurrentItemCount
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetCurrentItemCount(long lMsgId) 
	{
		return XCM_GetCurrentItemCount(pXCom, lMsgId);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetSecsLog()
			DESCRIPT	: GetSecsLog
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetSecsLog(long lMsgId, char *pszBuff, long lSize) 
	{
		return XCM_GetSecsLog(pXCom, lMsgId, pszBuff, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::PopEvent()
			DESCRIPT	: PopEvent
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long PopEvent(long *pIEventType, short* nEventID, long* lParam)
	{
		return XCM_PopEvent(pXCom, pIEventType, nEventID, lParam);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetStreamMsg()
			DESCRIPT	: SetStreamMsg
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetStreamMsg(long lMsgId, BYTE *pcData, long nSize) 
	{
		return XCM_SetStreamMsg(pXCom, lMsgId, pcData, nSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetStreamMsgSize()
			DESCRIPT	: GetStreamMsgSize
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetStreamMsgSize(long lMsgId) 
	{
		return XCM_GetStreamMsgSize(pXCom, lMsgId);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetStreamMsg()
			DESCRIPT	: GetStreamMsg
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetStreamMsg(long lMsgId, BYTE *pnData, long *plCount, long lSize) 
	{
		return XCM_GetStreamMsg(pXCom, lMsgId, pnData, plCount, lSize);
	}
	/*=========================================================================
			FUNCTION	: XComPro::MoveSecsItem()
			DESCRIPT	: MoveSecsItem
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long MoveSecsItem(long lMsgId, long nIndex) 
	{
		return XCM_MoveSecsItem(pXCom, lMsgId, nIndex);
	}
	/*=========================================================================
			FUNCTION	: XComPro::GetMsgSize()
			DESCRIPT	: SetSecsEventHandler
			RETURN		: GetMsgSize
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long GetMsgSize(long lMsgId) 
	{
		return XCM_GetMsgSize(pXCom, lMsgId);
	}
	/*=========================================================================
			FUNCTION	: XComPro::SetEventHandlerMode()
			DESCRIPT	: SetEventHandlerMode
			RETURN		: None
			ARGUMENT	: 
			UPDATE		: 2016/04/27
	==========================================================================*/
	long SetEventHandlerMode(short nParam) 
	{
		return XCM_SetEventHandlerMode(pXCom, nParam);
	}

};