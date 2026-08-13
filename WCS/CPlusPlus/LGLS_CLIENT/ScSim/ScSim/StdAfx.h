// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__18002C02_B2DD_4E1A_94D4_24803E9DC189__INCLUDED_)
#define AFX_STDAFX_H__18002C02_B2DD_4E1A_94D4_24803E9DC189__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxmt.h>
#include <afxtempl.h>
#include <comdef.h>
#include <strsafe.h>

#pragma warning(disable:4786)

// XmlLib.dll
//#include "XmlDom.h"
#include "Convert.h"

// EcsLib.dll
#include "ShadeButtonST.h"
#include "SxButton.h"
#include "FooButton.h"
#include "EditEx.h"
#include "Label.h"
#include "ColorPicker.h"
#include "ReportCtrl.h"

// Ecs.exe
#include "EcsEnv.h"
#include "EcsDef.h"
#include "Color.h"
#include "Lib.h"
#include "Debuger.h"

extern CString g_strEcsPath;
extern int g_nTabCount;
extern int g_nPlcCount;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__18002C02_B2DD_4E1A_94D4_24803E9DC189__INCLUDED_)
