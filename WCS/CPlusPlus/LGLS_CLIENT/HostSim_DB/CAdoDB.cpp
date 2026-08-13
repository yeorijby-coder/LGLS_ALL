

#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "AdoDB.h"



///////////////////////////////////////////////
// CDBProc

IMPLEMENT_DYNCREATE(CAdoDB, CObject)

CAdoDB::CAdoDB()
{
	m_pDoc = NULL;
	m_pWmsDb = NULL;

	m_strErrMsg = "";

	m_bConnected = FALSE;
}

CAdoDB::CAdoDB(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	m_pWmsDb = NULL;

	m_strErrMsg = "";

	m_bConnected = FALSE;
}


CAdoDB::~CAdoDB()
{
	m_bConnected = FALSE;
}

BOOL CAdoDB::BeginTrans()
{
	m_pWmsDb->BeginTrans();

	return true;
}

BOOL CAdoDB::CommitTrans()
{
	m_pWmsDb->CommitTrans();

	return true;
}

BOOL CAdoDB::RollbackTrans()
{
	m_pWmsDb->RollbackTrans();

	return true;
}
///////////////////////////////////////////////
//
BOOL CAdoDB::ConnectDB() //보류6
{
	CString strConnet;

	if(m_pDoc == NULL || m_pDoc->m_pConfig == NULL)
	{
		return FALSE;	
	}

#if ORACLE
	strConnet.Format(_T("DRIVER=%s;Dbq=%s;UID=%s;PWD=%s"),
	m_pDoc->m_pConfig->m_strDATABASE_DRIVER, 
	m_pDoc->m_pConfig->m_strDATABASE_SERVER, 
	m_pDoc->m_pConfig->m_strDATABASE_USERID, 
	m_pDoc->m_pConfig->m_strDATABASE_USERPASSWORD);
#elif POSTGRESQL
//	strConnet.Format(_T("Driver=%s;Server=%s;uid=%s;pwd=%s;Database=%s"), //Provider=MSDASQL; 
//		m_pDoc->m_pConfig->m_strDATABASE_DRIVER,
//		m_pDoc->m_pConfig->m_strDATABASE_SERVER, 
//		m_pDoc->m_pConfig->m_strDATABASE_USERID, 
//		m_pDoc->m_pConfig->m_strDATABASE_USERPASSWORD,
//		m_pDoc->m_pConfig->m_strDATABASE_DATABASE);

	// 6.0 버전을 기준으로 커넥션 스트링을 만들었음!
	strConnet.Format(_T("Provider=MSDASQL.1;Driver={%s};Server=%s;Port=5432;uid=%s;pwd=%s;Database=%s;SSLMode=prefer;Client Encoding=UTF8;"), //Provider=MSDASQL; 
		m_pDoc->m_pConfig->m_strDATABASE_DRIVER,
		m_pDoc->m_pConfig->m_strDATABASE_SERVER,
		m_pDoc->m_pConfig->m_strDATABASE_USERID,
		m_pDoc->m_pConfig->m_strDATABASE_USERPASSWORD,
		m_pDoc->m_pConfig->m_strDATABASE_DATABASE);

#endif

	m_strErrMsg = "";
	try
	{
		m_pWmsDb.CreateInstance(_uuidof(Connection));
		m_pWmsDb->ConnectionTimeout = 10;
		m_pWmsDb->CursorLocation = adUseClient;
		m_pWmsDb->Open(strConnet.AllocSysString(), "", "", NULL);
		m_pWmsDb->Errors->Clear();
	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());
		::WritePrivateProfileString(_T("CONNECT"), _T("CONNECT"), strConnet , ECS_INI_FILE);
		m_strErrMsg.Format(_T("ConnectDB:%s\n%s\n%s"), (LPCTSTR)bstrSource,(LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg );
		AfxMessageBox(m_strErrMsg);
		return FALSE;
	}

	catch(...)
	{
		m_strErrMsg = "ConnectDB:DataBase Connect Failed!!!(Check ODBC)";
		AfxMessageBox(m_strErrMsg);
		return FALSE;
	}

	m_bConnected = TRUE;
	return TRUE;
}

void CAdoDB::CrateInputParameter(_CommandPtr pCmd, CObList *plistIn)
{
	for (POSITION pos = plistIn->GetHeadPosition(); pos != NULL;)
	{
		CIOParam *pParam = (CIOParam *)plistIn->GetNext( pos );

		_bstr_t bstrName = pParam->m_strName;
		_bstr_t bstrValue = pParam->m_strData;

		pCmd->Parameters->Append( pCmd->CreateParameter(bstrName,
													    pParam->m_nType,
													    adParamInput,
													    1000,
													   _variant_t(bstrValue)) ); 
	}
}

void CAdoDB::CrateOutputParameter(_CommandPtr pCmd, CObList *plistOut)
{
	for (POSITION pos = plistOut->GetHeadPosition(); pos != NULL;)
	{
		CIOParam *pParam = (CIOParam *)plistOut->GetNext( pos );

		_bstr_t bstrName = pParam->m_strName;
		pCmd->Parameters->Append(pCmd->CreateParameter(bstrName,
													   pParam->m_nType,
													   adParamOutput,
													   pParam->m_nSize) );
	}
}

void CAdoDB::ExtractOutputParameter(_CommandPtr pCmd, CObList *plistOut)
{
	for (POSITION pos = plistOut->GetHeadPosition(); pos != NULL;)
	{
		CIOParam *pParam = (CIOParam *)plistOut->GetNext( pos );
		_bstr_t bstrName = pParam->m_strName;

		_variant_t vValue = pCmd->GetParameters()->GetItem(bstrName)->Value;
		pParam->SetData( (LPCTSTR)_bstr_t(vValue) );

		//pParam->JustShowData();
	}

}

BOOL CAdoDB::ExecuteStoredProc(CAdoDbIO *pAdoDbIO, int nScNum)
{
	m_pDoc->EnterBlcokingSection();

	_CommandPtr pCmd = NULL;
	CString strProcName = pAdoDbIO->m_strProcName;
	

//	POSITION pos = pAdoDbIO->m_listInput.GetHeadPosition();
//	CIOParam *pParam = (CIOParam *)pAdoDbIO->m_listInput.GetNext( pos );
//	CString strScNum =  pParam->m_strData;
///		_bstr_t bstrName = pParam->m_strName;
///		_bstr_t bstrValue = pParam->m_strData;
///		pParam->JustShowData();

	CString strScNum;
	strScNum.Format(_T("%d"),nScNum+1);
	//m_pDoc->strName = strScNum;
	CTime tStartTime = CTime::GetCurrentTime();
	//m_pDoc->strStartTime = CTime::GetCurrentTime().Format("%Y/%m/%d-%H:%M:%S");
	
	m_strErrMsg = "";

	try
	{
		pCmd.CreateInstance(__uuidof(Command));		
		pCmd->ActiveConnection = m_pWmsDb;		
		pCmd->CommandText = strProcName.AllocSysString();
		pCmd->CommandType = adCmdStoredProc;
		
		// Input
		CrateInputParameter(pCmd, &(pAdoDbIO->m_listInput));
		CrateOutputParameter(pCmd, &(pAdoDbIO->m_listOutput));

		pCmd->CommandTimeout=20;
		pCmd->Execute(NULL, NULL, NULL);

		ExtractOutputParameter(pCmd, &(pAdoDbIO->m_listOutput));
	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		m_strErrMsg.Format(_T("[프로시저명]=%s\n [SOURCE]=%s\n [DESCRIPTION]=%s\n [MSG]=%s"), 
							strProcName, (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);

		if ( err.Error() == E_FAIL )
			m_bConnected = FALSE;

		m_pDoc->LeaveBlcokingSection();
		return FALSE;
	}
	catch(...)
	{
		m_strErrMsg.Format(_T("[프로시저명]=%s\n 처리 되지 않은 예외 발생.. 관리자에게 문의"),
							strProcName);

		m_pDoc->LeaveBlcokingSection();
		return FALSE;
	}

	CTime tEndTime = CTime::GetCurrentTime();
	//m_pDoc->strEndTime = CTime::GetCurrentTime().Format(_T("%Y/%m/%d-%H:%M:%S"));
	CTimeSpan tDiffTime = tEndTime - tStartTime;
	//m_pDoc->lDiffTime = tDiffTime.GetTotalSeconds( );

	m_pDoc->LeaveBlcokingSection();
	return TRUE;
}

BOOL CAdoDB::ExecuteQueryString(CString strSql)
{
	m_pDoc->EnterBlcokingSection(); 
	try
	{
		m_pWmsDb->CommandTimeout=20;
		m_pWmsDb->Execute(strSql.AllocSysString(), NULL, adCmdText);
	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		m_strErrMsg.Format(_T("[ExecuteQueryString] [SOURCE]=%s\n [DESCRIPTION]=%s\n [MSG]=%s"), 
					   (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);

		if ( err.Error() == E_FAIL )
			m_bConnected=FALSE;
		
		m_pDoc->LeaveBlcokingSection();
		return FALSE;
	}
	catch(...)
	{
		m_strErrMsg.Format(_T("[ExecuteQueryString]\n 처리 되지 않은 예외 발생.. 관리자에게 문의"));

		m_pDoc->LeaveBlcokingSection();
		return FALSE;
	}

	m_pDoc->LeaveBlcokingSection();
	return TRUE;
}

void CAdoDB::UTF8toANSI(LPSTR src, CString &dst)
{
	//determine length of UTF-8 encoded string


//	dst = CString(lpszA);


	//delete[] lpszW;
	//delete[] lpszA;
}
////////////////////////////////////////////////////////////////////////////////////////////////
// RecordSet


_RecordsetPtr CAdoDB::SelectSqlForThread_RecordSet(CString strSql, int &nRowCnt, CString &strMsg)
{
	m_pWmsDb->CommandTimeout=60;

	CString		  strTemp;
	_RecordsetPtr rsPtr;

	strMsg = "";
	rsPtr.CreateInstance(__uuidof(Recordset));
	try
	{		
		long lTemp = rsPtr->Open(_variant_t(strSql), m_pWmsDb.GetInterfacePtr(), 
			adOpenForwardOnly, adLockReadOnly, adCmdText);
			//adOpenDynamic, adLockReadOnly, adCmdText);

		if (rsPtr->adoEOF)
		{
			nRowCnt = 0;
			rsPtr = NULL;
			return NULL;
		}
		
		nRowCnt = rsPtr->RecordCount; 
		_RecordsetPtr rtrsPtr = rsPtr->Clone(adLockReadOnly);
		rsPtr->Close();
		return rtrsPtr;
	}
	// Error 발생 시 처리
	catch(_com_error &err){
 		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		strMsg.Format(_T("SelectSqlForThread:%s\n\n%s\n\n%s"), 
			(LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, strMsg);
		rsPtr  = NULL;

		// err가 E_FAIL일때 처리..
		if ( err.Error() == E_FAIL ) 	
			m_bConnected=FALSE;

		return NULL;

	}
	catch(...){
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, "SelectSqlForThread:SelectSQl 처리 중 오류 발생..");
		rsPtr  = NULL;	
		return FALSE;
	}

	// Always set these pointers to null when you are done with them!
	rsPtr  = NULL;
	return NULL;
}

BOOL CAdoDB::SelectSqlForThread(CString strSql, CStringList &strTempList, int nRtRecord, int &nRowCnt, CString &strMsg)
{
	CString		  strTemp;
	_RecordsetPtr rsPtr;

	strMsg = "";
	rsPtr.CreateInstance(__uuidof(Recordset));
	try
	{		
		long lTemp = rsPtr->Open(_variant_t(strSql), m_pWmsDb.GetInterfacePtr(), \
										adOpenDynamic, adLockReadOnly, adCmdText);

		if (rsPtr->adoEOF)
		{
			rsPtr = NULL;
			return TRUE;
		}
		
		nRowCnt = rsPtr->RecordCount; 

		for(int i=0; i < nRowCnt; i++) 
		{
			FieldsPtr fdsPtr = rsPtr->GetFields();
			CString strStream;
			
			
			for(int j=0; j<fdsPtr->Count; j++)
			{
				FieldPtr fdPtr = fdsPtr->GetItem(_variant_t((long)j));
				_bstr_t bstrFieldName  = fdPtr->Name;
				_bstr_t bstrFieldValue;
				
				if (fdPtr->Value.vt == VT_NULL)
				{
					bstrFieldValue = "";
				}
				else
				{
					bstrFieldValue = fdPtr->Value;
				}


			//	strTemp.Format(_T("%s;%s"), (LPCTSTR)bstrFieldName, (LPCTSTR)bstrFieldValue);
			//	strTempList.AddTail(strTemp);
			}

			if (i+1 ==nRtRecord)
			{
				rsPtr->Close();
				rsPtr  = NULL;
				return TRUE;

			}
			//AfxMessageBox(strStream);
			rsPtr->MoveNext();

		}
		rsPtr->Close();

	}
	// Error 발생 시 처리
	catch(_com_error &err){
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		strMsg.Format(_T("SelectSqlForThread:%s\n\n%s\n\n%s"), 
					(LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, strMsg);
		rsPtr  = NULL;

		// err가 E_FAIL일때 처리..
		if ( err.Error() == E_FAIL ) 	m_bConnected=FALSE;
		
		return FALSE;

	}
	catch(...){
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, "SelectSqlForThread:SelectSQl 처리 중 오류 발생..");
		rsPtr  = NULL;	
		return FALSE;
	}

	// Always set these pointers to null when you are done with them!
	rsPtr  = NULL;
	return TRUE;
}

BOOL CAdoDB::SelectSqlForThread(CString strSql, int &nRowCnt, CString &strMsg)
{
	CString		  strTemp;
	_RecordsetPtr rsPtr;

	strMsg = "";
	rsPtr.CreateInstance(__uuidof(Recordset));
	try
	{		
		long lTemp = rsPtr->Open(_variant_t(strSql), m_pWmsDb.GetInterfacePtr(), \
			adOpenForwardOnly, adLockPessimistic, adCmdText);

		if (rsPtr->adoEOF)
		{
			rsPtr = NULL;
			return TRUE;
		}
		nRowCnt = rsPtr->RecordCount; 
		rsPtr->Close();
	}
	// Error 발생 시 처리
	catch(_com_error &err){
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		strMsg.Format(_T("SelectSqlForThread:%s\n\n%s\n\n%s"), 
			(LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, strMsg);
		rsPtr  = NULL;
		return FALSE;

	}
	catch(...){
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, "SelectSqlForThread:SelectSQl 처리 중 오류 발생..");
		rsPtr  = NULL;	
		return FALSE;
	}

	// Always set these pointers to null when you are done with them!
	rsPtr  = NULL;
	return TRUE;
}


BOOL CAdoDB::ExecuteSqlForMainPGM(CString strSql)
{
	_bstr_t bstrValue;
	_variant_t vRecsAffected(0L);
	CString strTemp=_T("");
    
    bstrValue = strSql; 

	try
	{
		m_pWmsDb->CommandTimeout=20;
		m_pWmsDb->Execute(bstrValue, NULL, adCmdText);
	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		CString strMsg;
		strMsg.Format(_T("ExecuteSqlForMainPGM:%s\n\n%s\n\n%s"), 
					(LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg);
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, strMsg);

		// err가 E_FAIL일때 처리..
		if ( err.Error() == E_FAIL ) 	m_bConnected=FALSE;
		
		return FALSE;
	}
	catch(...)
	{
		//LOG_ERROR(LOG_POS_HOST, LOG_SYSTEM, IMS_TO_ECS, "ExecuteSqlForMainPGM:데이타 베이스 관리자에게 문의 하십시요");
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Procedure 실행
BOOL CAdoDB::ExecuteProcForInputOrder(CString strStorProc, CStringList &strTempList, int &nRetCd, CString &strRetMsg)
{
	_bstr_t		bstrValue;
	_CommandPtr pCmd = NULL;
    _variant_t	vPoRetCd, vPoMsg, vPoDBErr, vPoJobNo;

	//
    bstrValue = strStorProc; 

	try{
		
		pCmd.CreateInstance(__uuidof(Command));		
		pCmd->ActiveConnection = m_pWmsDb;		
		pCmd->CommandText = bstrValue;
		pCmd->CommandType  = adCmdStoredProc;
		
		// Input
		for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
		{
			CString strTempArg = strTempList.GetNext(pos);
			_bstr_t bstrTempParam = strTempArg.Mid(1, strTempArg.Find(';'));
			_bstr_t bstrTempValue = strTempArg.Mid(strTempArg.Find(';')+1);
			
			if (strTempArg.Mid(0,1) == "I")
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adInteger, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
			else
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adChar, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
		}
		
		// Output
		pCmd->Parameters->Append(pCmd->CreateParameter("poRetCd", adInteger, adParamOutput, 4));
		pCmd->Parameters->Append(pCmd->CreateParameter("poMsg", adChar, adParamOutput, 1000));
		pCmd->Parameters->Append(pCmd->CreateParameter("poDBErr", adChar, adParamOutput, 1000));

		pCmd->CommandTimeout=20;
		pCmd->Execute(NULL,NULL,NULL);
		
		// Return Value
		vPoRetCd = pCmd->GetParameters()->GetItem("poRetCd")->Value; 
		vPoMsg   = pCmd->GetParameters()->GetItem("poMsg")->Value; 
		vPoDBErr = pCmd->GetParameters()->GetItem("poDBErr")->Value; 

		nRetCd = atoi((LPCSTR)_bstr_t(vPoRetCd));
		strRetMsg.Format(_T("%s"), (LPCTSTR)_bstr_t(vPoMsg));
		// Not Found 처리
		//if (atoi(_bstr_t(vPoRetCd)) == -2) 
		if (atoi((LPCSTR)_bstr_t(vPoRetCd)) < 0)
		{
			CString strMsg;
			strMsg.Format(_T("ExecuteProcForInputOrder:%s\n\n%s"), 
					(LPCTSTR)_bstr_t(vPoMsg), strStorProc );
			//AfxMessageBox(strMsg);
			strRetMsg.Format(_T("%s"), strMsg);
			return FALSE;
		
		}

	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		CString strMsg;
		strMsg.Format(_T("ExecuteProcForInputOrder:%s\n\n%s\n\n%s\n\n%s"), 
					  (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg, strStorProc);
		//AfxMessageBox(strMsg);
		strRetMsg.Format(_T("%s"), strMsg);

		// err가 E_FAIL일때 처리..
		nRetCd = -1;
		if ( err.Error() == E_FAIL ) 	m_bConnected = FALSE;

		return FALSE;
	}
	catch(...)
	{
		CString strMsg;
		strMsg.Format(_T("ExecuteProcForInputOrder:데이타 베이스 관리자에게 문의 하십시요"));
		strRetMsg.Format(_T("%s"), strMsg);
		nRetCd = -1;
		return FALSE;
	}

	return TRUE;
}

BOOL CAdoDB::ExecuteProcForOutputOrder(CString strStorProc, CStringList &strTempList, int &nRetCd, CString &strRetMsg, CString &strJobNo)
{
	_bstr_t		bstrValue;
	_CommandPtr pCmd = NULL;
    _variant_t	vPoRetCd, vPoMsg, vPoDBErr, vPoJobNo;

	//
    bstrValue = strStorProc; 

	try{
		
		pCmd.CreateInstance(__uuidof(Command));		
		pCmd->ActiveConnection = m_pWmsDb;		
		pCmd->CommandText = bstrValue;
		pCmd->CommandType  = adCmdStoredProc;
		
		// Output
		pCmd->Parameters->Append(pCmd->CreateParameter("poRetCd", adInteger, adParamOutput, 4));
		pCmd->Parameters->Append(pCmd->CreateParameter("poMsg", adChar, adParamOutput, 1000));
		pCmd->Parameters->Append(pCmd->CreateParameter("poDBErr", adChar, adParamOutput, 1000));

		// Input
		for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
		{
			CString strTempArg = strTempList.GetNext(pos);
			_bstr_t bstrTempParam = strTempArg.Mid(1, strTempArg.Find(';'));
			_bstr_t bstrTempValue = strTempArg.Mid(strTempArg.Find(';')+1);
			
			if (strTempArg.Mid(0,1) == "I")
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adInteger, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
			else
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adChar, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
		}
		
		// Output
		pCmd->Parameters->Append(pCmd->CreateParameter("poJobNo", adChar, adParamOutput, 5));

		pCmd->CommandTimeout=20;
		pCmd->Execute(NULL,NULL,NULL);
		
		// Return Value
		vPoRetCd = pCmd->GetParameters()->GetItem("poRetCd")->Value; 
		vPoMsg   = pCmd->GetParameters()->GetItem("poMsg")->Value; 
		vPoDBErr = pCmd->GetParameters()->GetItem("poDBErr")->Value; 
		vPoJobNo = pCmd->GetParameters()->GetItem("poJobNo")->Value; 

		nRetCd = atoi((LPCSTR)_bstr_t(vPoRetCd));
		strRetMsg.Format(_T("%s"), (LPCTSTR)_bstr_t(vPoMsg));
		strJobNo.Format(_T("%s"), (LPCTSTR)_bstr_t(vPoJobNo));
		// Not Found 처리
		//if (atoi(_bstr_t(vPoRetCd)) == -2) 
		if (atoi((LPCSTR)_bstr_t(vPoRetCd)) < 0)
		{
			CString strMsg;
			strMsg.Format(_T("ExecuteProcForOutputOrder:%s\n\n%s"), 
					(LPCTSTR)_bstr_t(vPoMsg), strStorProc );
			//AfxMessageBox(strMsg);
			strRetMsg.Format(_T("%s"), strMsg);
			return FALSE;
		
		}

	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		CString strMsg;
		strMsg.Format(_T("ExecuteProcForOutputOrder:%s\n\n%s\n\n%s\n\n%s"), 
					  (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg, strStorProc);
		//AfxMessageBox(strMsg);
		strRetMsg.Format(_T("%s"), strMsg);

		// err가 E_FAIL일때 처리..
		nRetCd = -1;
		if ( err.Error() == E_FAIL ) 	m_bConnected = FALSE;

		return FALSE;
	}
	catch(...)
	{
		CString strMsg;
		strMsg.Format(_T("ExecuteProcForOutputOrder:데이타 베이스 관리자에게 문의 하십시요"));
		strRetMsg.Format(_T("%s"), strMsg);
		nRetCd = -1;
		return FALSE;
	}

	return TRUE;
}

// 이상시 처리
BOOL CAdoDB::ExecuteProcForManualOrder(CString strStorProc, CStringList &strTempList, int &nRetCd, CString &strRetMsg)
{
	_bstr_t		bstrValue;
	_CommandPtr pCmd = NULL;
    _variant_t	vPoRetCd, vPoMsg, vPoDBErr, vPoJobNo;

	//
    bstrValue = strStorProc; 

	try{
		
		pCmd.CreateInstance(__uuidof(Command));		
		pCmd->ActiveConnection = m_pWmsDb;		
		pCmd->CommandText = bstrValue;
		pCmd->CommandType  = adCmdStoredProc;
		
		// Output
		pCmd->Parameters->Append(pCmd->CreateParameter("poRetCd", adInteger, adParamOutput, 4));
		pCmd->Parameters->Append(pCmd->CreateParameter("poMsg", adChar, adParamOutput, 1000));
		pCmd->Parameters->Append(pCmd->CreateParameter("poDBErr", adChar, adParamOutput, 1000));

		// Input
		for (POSITION pos = strTempList.GetHeadPosition(); pos != NULL;)
		{
			CString strTempArg = strTempList.GetNext(pos);
			_bstr_t bstrTempParam = strTempArg.Mid(1, strTempArg.Find(';'));
			_bstr_t bstrTempValue = strTempArg.Mid(strTempArg.Find(';')+1);
			
			if (strTempArg.Mid(0,1) == "I")
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adInteger, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
			else
			{
				pCmd->Parameters->Append(pCmd->CreateParameter(bstrTempParam, adChar, adParamInput, 1000, _variant_t(bstrTempValue))); 
			}
		}
		
		pCmd->CommandTimeout=20;
		pCmd->Execute(NULL,NULL,NULL);
		
		// Return Value
		vPoRetCd = pCmd->GetParameters()->GetItem("poRetCd")->Value; 
		vPoMsg   = pCmd->GetParameters()->GetItem("poMsg")->Value; 
		vPoDBErr = pCmd->GetParameters()->GetItem("poDBErr")->Value; 

		nRetCd = atoi((LPCSTR)_bstr_t(vPoRetCd));
		strRetMsg.Format(_T("%s"), (LPCTSTR)_bstr_t(vPoMsg));
		// Not Found 처리
		//if (atoi(_bstr_t(vPoRetCd)) == -2) 
		if (atoi((LPCSTR)_bstr_t(vPoRetCd)) < 0)
		{
			CString strMsg;
			strMsg.Format(_T("ExecuteProcForManualOrder:%s\n\n%s"), 
					(LPCTSTR)_bstr_t(vPoMsg), strStorProc );
			//AfxMessageBox(strMsg);
			strRetMsg.Format(_T("%s"), strMsg);
			return FALSE;
		
		}

	}
	catch(_com_error &err)
	{
		_bstr_t bstrSource(err.Source());
		_bstr_t bstrDescription(err.Description());
		_bstr_t bstrErrMsg(err.ErrorMessage());

		CString strMsg;
		strMsg.Format(_T("ExecuteProcForManualOrder:%s\n\n%s\n\n%s\n\n%s"), 
					  (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription, (LPCTSTR)bstrErrMsg, strStorProc);
		//AfxMessageBox(strMsg);
		strRetMsg.Format(_T("%s"), strMsg);

		// err가 E_FAIL일때 처리..
		nRetCd = -1;
		if ( err.Error() == E_FAIL ) 	m_bConnected=FALSE;

		return FALSE;
	}
	catch(...)
	{
		CString strMsg;
		strMsg.Format(_T("ExecuteProcForManualOrder:데이타 베이스 관리자에게 문의 하십시요"));
		

		strRetMsg.Format(_T("%s"), strMsg);
		nRetCd = -1;
		return FALSE;
	}

	return TRUE;
}

