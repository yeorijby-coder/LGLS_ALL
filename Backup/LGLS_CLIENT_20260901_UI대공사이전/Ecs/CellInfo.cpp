#include "StdAfx.h"
#include "CellInfo.h"


CCellInfo::CCellInfo(void)
{
}


CCellInfo::~CCellInfo(void)
{
	for( POSITION pos = m_pCELL_DTL.GetHeadPosition(); pos != NULL; )
	{
		delete m_pCELL_DTL.GetNext( pos );
	}
	m_pCELL_DTL.RemoveAll();
}
