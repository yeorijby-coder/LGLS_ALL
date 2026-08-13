//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************

// KeyHelper.h: interface for the CBCGKeyHelper class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CBCGKeyHelper : public CObject
{
public:
	CBCGKeyHelper();
	CBCGKeyHelper(LPACCEL lpAccel);
	virtual ~CBCGKeyHelper();

// Operations:
public:
	void Format (CString& str) const;

protected:
	void AddVirtKeyStr (CString& str, UINT uiVirtKey, BOOL bLast = FALSE) const;

// Atttributes:
public:
	void SetAccelerator (LPACCEL lpAccel)
	{
		m_lpAccel = lpAccel;
	}

protected:
	LPACCEL m_lpAccel;
};

