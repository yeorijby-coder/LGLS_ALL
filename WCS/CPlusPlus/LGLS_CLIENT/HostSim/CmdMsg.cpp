// CmdMsg.cpp: implementation of the CCmdMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "CmdMsg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdMsg::CCmdMsg()
{
	m_nCommand = 0;
}

CCmdMsg::~CCmdMsg()
{

}
