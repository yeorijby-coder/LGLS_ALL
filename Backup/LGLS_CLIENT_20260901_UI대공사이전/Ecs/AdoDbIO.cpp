
#include "stdafx.h"
#include "Ecs.h"

#include "EcsDoc.h"
#include "AdoDbIO.h"


/////////////////////////////////////////////
//

CIOParam::CIOParam(CString strName, CString strData)
{
	//m_nType = adChar;

	m_strName = strName;
	m_strData = strData;

	m_nSize = 1000;
}

CIOParam::CIOParam(CString strName, int nData)
{
	//m_nType = adInteger;

	m_strName = strName;
	m_strData.Format(_T("%d"), nData);

	m_nSize = 1000;
}

CIOParam::CIOParam(CString strName, int nSize, enum DataTypeEnum nType)
{
	m_nType = nType;

	m_strName = strName;
	m_strData = "";
	m_nSize = nSize;
}

CIOParam::~CIOParam()
{

}

void CIOParam::SetData(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	m_strData = strData;
}

void CIOParam::JustShowData()
{
	CString strMsg;
	strMsg.Format(_T("NAME-[%s] DATA=[%s] SIZE=[%d]"), m_strName, m_strData, m_nSize);
	AfxMessageBox(strMsg);
}

///////////////////////////////////////////////
//
BOOL CIOParam::IsSameSame(CIOParam *pIOParam)
{
	if (m_nType == pIOParam->m_nType && 
		m_strName == pIOParam->m_strName )
		return true;

	return false;
}

BOOL CIOParam::IsResError()
{
	int nErrCode = CConvert::ToInt(m_strData);
	return nErrCode;
}

/////////////////////////////////////////////
//

CAdoDbIO::CAdoDbIO()
{
	m_strProcName = "";
	m_strErrMsg = "";
}

CAdoDbIO::~CAdoDbIO()
{
   for( POSITION pos = m_listInput.GetHeadPosition(); pos != NULL; )
   {
		delete m_listInput.GetNext( pos );
   }
   m_listInput.RemoveAll();

   for( POSITION pos = m_listOutput.GetHeadPosition(); pos != NULL; )
   {
		delete m_listOutput.GetNext( pos );
   }
   m_listOutput.RemoveAll();
}

////////////////////////////////////////////////////////////
//

BOOL CAdoDbIO::GetOutputData(CString strName, CString &strData)
{
	for (POSITION pos = m_listOutput.GetHeadPosition(); pos != NULL;)
	{
		CIOParam *pParam = (CIOParam *)m_listOutput.GetNext( pos );
		if ( pParam->m_strName == strName )
		{
			strData = pParam->m_strData;
			return true;
		}
	}

	return false;
}

BOOL CAdoDbIO::GetOutputData(CIOParam *pIOParam)
{
	for (POSITION pos = m_listOutput.GetHeadPosition(); pos != NULL;)
	{
		CIOParam *pParam = (CIOParam *)m_listOutput.GetNext( pos );
		if ( pParam->IsSameSame(pIOParam) )
		{
			pIOParam->m_strData = pParam->m_strData;
			return true;
		}
	}

	return false;
}

/////////////////////////////////////////////////////
//
BOOL CAdoDbIO::SetInputData(CString strName, CString strData)
{
	for (POSITION pos = m_listInput.GetHeadPosition(); pos != NULL;)
	{
		CIOParam *pParam = (CIOParam *)m_listInput.GetNext( pos );
		if ( pParam->m_strName == strName )
		{
			pParam->m_strData = strData;
			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////
//
BOOL CAdoDbIO::IsNormalJob()
{
	/*
	CIOParam IOParam(_T("o_result_cd"), 0, adInteger);
	if( !GetOutputData(&IOParam) )
		return false;

	if ( IOParam.IsResError() )
		return false;
		*/
	return true;
}



