// Info.cpp: implementation of the CInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "Info.h"
#include "EcsDoc.h"
#include "Equipment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CInfo, CObject, 0)

CInfo::CInfo(CEquipment* pEquipment)
{
	m_pEquipment = pEquipment;
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	m_bModified = TRUE;
	m_bEtherNet = TRUE;
}

CInfo::~CInfo()
{

}

BOOL CInfo::Backup()
{
	m_csSyncBackup2.Lock();

	CString strFileName;
	strFileName.Format(_T("%s\\%s.ecs"), m_pEquipment->m_pDoc->m_pConfig->m_strInfoPath, m_pEquipment->m_port.m_strDevice);

	TRY
	{
		CFile f(strFileName, CFile::modeCreate | CFile::modeWrite);

		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();

		f.Close();
	}
	CATCH (CException, e)
	{
		CString strLog;
		strLog.Format(_T("%s 정보파일 백업 실패! [%s]"), m_pEquipment->m_port.m_strDevice, CLib::GetExceptionString(e));
		m_pEquipment->WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, strLog, _T("CInfo::Backup"));
		m_csSyncBackup2.Unlock();
		return FALSE;
	}
	END_CATCH

	m_csSyncBackup2.Unlock();

	return TRUE;
}

BOOL CInfo::Backup2(int nNo)
{
	m_csSyncBackup2.Lock();

	CString strFileName;
	strFileName.Format(_T("%s\\%s_%02d.ecs"), m_pEquipment->m_pDoc->m_pConfig->m_strInfoPath, m_pEquipment->m_port.m_strDevice, nNo);

	TRY
	{
		CFile f(strFileName, CFile::modeCreate | CFile::modeWrite);

		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();

		f.Close();
	}
	CATCH (CException, e)
	{
		CString strLog;
		strLog.Format(_T("%s 정보파일 백업 실패! [%s]"), m_pEquipment->m_port.m_strDevice, CLib::GetExceptionString(e));
		m_pEquipment->WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, strLog, _T("CInfo::Backup"));
		m_csSyncBackup2.Unlock();
		return FALSE;
	}
	END_CATCH

	m_csSyncBackup2.Unlock();

	return TRUE;
}

BOOL CInfo::Restore()
{
	CString strFileName;
	strFileName.Format(_T("%s\\%s.ecs"), m_pEquipment->m_pDoc->m_pConfig->m_strInfoPath, m_pEquipment->m_port.m_strDevice);

	TRY
	{
		CFile f(strFileName, CFile::modeRead);
		CArchive ar(&f, CArchive::load);
		Serialize(ar);
		ar.Close();

		f.Close();
	}
	CATCH (CException, e)
	{
		CString strLog;
		strLog.Format(_T("%s 정보파일 복원 실패! [%s]"), m_pEquipment->m_port.m_strDevice, CLib::GetExceptionString(e));
		m_pEquipment->WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, strLog, _T("CInfo::Restore"));
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

BOOL CInfo::Restore2(int nNo)
{
	CString strFileName;
	strFileName.Format(_T("%s\\%s_%02d.ecs"), m_pEquipment->m_pDoc->m_pConfig->m_strInfoPath, m_pEquipment->m_port.m_strDevice, nNo);

	TRY
	{
		CFile f(strFileName, CFile::modeRead);
		CArchive ar(&f, CArchive::load);
		Serialize(ar);
		ar.Close();

		f.Close();
	}
	CATCH (CException, e)
	{
		CString strLog;
		strLog.Format(_T("%s 정보파일 복원 실패! [%s]"), m_pEquipment->m_port.m_strDevice, CLib::GetExceptionString(e));
		m_pEquipment->WriteLog(LOG_TYPE_ERROR, LOG_SYSTEM, strLog, _T("CInfo::Restore"));
		return FALSE;
	}
	END_CATCH

	return TRUE;
}
