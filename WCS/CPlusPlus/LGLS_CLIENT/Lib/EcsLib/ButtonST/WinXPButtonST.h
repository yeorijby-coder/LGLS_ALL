//
//	Class:		CWinXPButtonST
//
//	Compiler:	Visual C++
//				eMbedded Visual C++
//	Tested on:	Visual C++ 6.0
//				Windows CE 3.0
//
//	Created:	03/September/2001
//	Updated:	25/November/2002
//
//	Author:		Davide Calabro'		davide_calabro@yahoo.com
//
//	Disclaimer
//	----------
//	THIS SOFTWARE AND THE ACCOMPANYING FILES ARE DISTRIBUTED "AS IS" AND WITHOUT
//	ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO REPONSIBILITIES FOR POSSIBLE
//	DAMAGES OR EVEN FUNCTIONALITY CAN BE TAKEN. THE USER MUST ASSUME THE ENTIRE
//	RISK OF USING THIS SOFTWARE.
//
//	Terms of use
//	------------
//	THIS SOFTWARE IS FREE FOR PERSONAL USE OR FREEWARE APPLICATIONS.
//	IF YOU USE THIS SOFTWARE IN COMMERCIAL OR SHAREWARE APPLICATIONS YOU
//	ARE GENTLY ASKED TO DONATE 5$ (FIVE U.S. DOLLARS) TO THE AUTHOR:
//
//		Davide Calabro'
//		P.O. Box 65
//		21019 Somma Lombardo (VA)
//		Italy
//
#ifndef _WINXPBUTTONST_H_
#define _WINXPBUTTONST_H_

#pragma once

#include "BtnST.h"

// CWinXPButtonST
//
class AFX_EXT_CLASS CWinXPButtonST : public CButtonST
{
public:
	CWinXPButtonST();
	virtual ~CWinXPButtonST();

	DWORD SetRounded(BOOL bRounded, BOOL bRepaint = TRUE);

protected:
	virtual DWORD OnDrawBackground(CDC* pDC, CRect* pRect);
	virtual DWORD OnDrawBorder(CDC* pDC, CRect* pRect);

private:
	BOOL	m_bIsRounded;		// Borders must be rounded?
};

#endif 
