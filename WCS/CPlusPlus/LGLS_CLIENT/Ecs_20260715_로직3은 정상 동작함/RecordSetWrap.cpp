#include "StdAfx.h"
#include "RecordSetWrap.h"


CRecordSetWrap::CRecordSetWrap(_RecordsetPtr precordSet)
{
	m_pRecordSet = precordSet;
}



CRecordSetWrap::~CRecordSetWrap(void)
{
	if(m_pRecordSet != NULL)
	{
		m_pRecordSet->Close();
	}
	m_pRecordSet = NULL;
}

BOOL CRecordSetWrap::MoveNext()
{
	if(m_pRecordSet == NULL)
	{
		return FALSE;
	}

	if(m_pRecordSet->adoEOF == TRUE)
	{
		return FALSE;
	}

	m_pRecordSet->MoveNext();

	return TRUE;
}


BOOL CRecordSetWrap::MovePrevious()
{
	if(m_pRecordSet == NULL)
	{
		return FALSE;
	}

	if(m_pRecordSet->adoEOF == TRUE)
	{
		return FALSE;
	}

	m_pRecordSet->MovePrevious();

	return TRUE;
}


BOOL CRecordSetWrap::MoveFirst()
{
	if(m_pRecordSet == NULL)
	{
		return FALSE;
	}

	if(m_pRecordSet->adoEOF == TRUE)
	{
		return FALSE;
	}

	m_pRecordSet->MoveFirst();

	return TRUE;
}

CString CRecordSetWrap::GetItem(CString strFiledName)
{
	if(m_pRecordSet == NULL)
	{
		return _T("");
	}

	if(m_pRecordSet->adoEOF == TRUE)
	{
		return _T("");
	}

	TRY
	{
		if(strFiledName.GetLength() > 40)
			int a =0;
		
		//return strFiledName;
		//m_pRecordSet->Fields->GetItem(_variant_t(strFiledName))->Value;
		return m_pRecordSet->Fields->GetItem(_variant_t(strFiledName))->Value;
	}
	CATCH (COleException, exOle)
	{
		return _T("");
	}
	CATCH (CException, e)
	{
		return _T("");
	}
	END_CATCH
		
	return _T("");
}
