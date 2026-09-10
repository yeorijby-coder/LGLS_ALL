#include "StdAfx.h"
#include "RecordSetWrap.h"


CRecordSetWrap::CRecordSetWrap(_RecordsetPtr precordSet)
{
	m_pRecordSet = precordSet;
}



CRecordSetWrap::~CRecordSetWrap(void)
{
	// [LGLS 2026-09-10] 이미 닫힌 레코드셋에 Close() 를 부르면 _com_error 가
	//   소멸자 밖으로 던져져 프로그램이 죽는다. 상태를 보고 닫고, 예외는 여기서 막는다.
	try
	{
		if(m_pRecordSet != NULL && m_pRecordSet->State != adStateClosed)
		{
			m_pRecordSet->Close();
		}
	}
	catch(_com_error&) { }
	catch(...) { }
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
