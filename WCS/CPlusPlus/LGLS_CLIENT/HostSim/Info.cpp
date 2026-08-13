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
//	m_nEquipNum = 0;
}

CInfo::~CInfo()
{

}

BOOL CInfo::Backup(int nIndex)
{
	m_csSyncBackup.Lock();

	CString strFileName;
//	strFileName.Format(_T("%s\\%s.ecs"), m_pEquipment->m_pDoc->m_pConfig->m_strInfoPath, m_pEquipment->m_port.m_strDevice);
	if (nIndex == 0)	
		strFileName.Format(_T("%s\\%s.ecs"), m_pEquipment->m_pDoc->m_pConfig->m_strInfoPath, m_pEquipment->m_port.m_strDevice);
	else
		strFileName.Format(_T("%s\\%s-%01d.ecs"), m_pEquipment->m_pDoc->m_pConfig->m_strInfoPath, m_pEquipment->m_port.m_strDevice, nIndex);

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
		m_csSyncBackup.Unlock();
		return FALSE;
	}
	END_CATCH

	m_csSyncBackup.Unlock();

	return TRUE;
}

BOOL CInfo::Restore(int nIndex)
{
	CString strFileName;
	if (nIndex == 0)	
		strFileName.Format(_T("%s\\%s.ecs"), m_pEquipment->m_pDoc->m_pConfig->m_strInfoPath, m_pEquipment->m_port.m_strDevice);
	else
		strFileName.Format(_T("%s\\%s-%01d.ecs"), m_pEquipment->m_pDoc->m_pConfig->m_strInfoPath, m_pEquipment->m_port.m_strDevice, nIndex);

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