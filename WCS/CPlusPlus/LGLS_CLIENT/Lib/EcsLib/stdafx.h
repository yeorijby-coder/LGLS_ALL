// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#define BLACK			RGB(  0,  0,  0)
#define BLUE			RGB(  0,  0,255)
#define GREEN			RGB(  0,255,  0)
#define CYAN			RGB(  0,255,255)
#define RED				RGB(255,  0,  0)
#define ORANGE			RGB(255,128,  0)
#define MAGENTA			RGB(255,  0,255)
#define YELLOW			RGB(255,255,  0)
#define WHITE			RGB(255,255,255)
#define SITE			RGB(128,  0,255)
#define DARK_BLUE		RGB(  0,  0,128)s
#define DARK_GREEN		RGB(  0,128,  0)
#define DARK_CYAN		RGB(  0,128,128)
#define DARK_RED		RGB(128,  0,  0)
#define DARK_MAGENTA	RGB(128,  0,128)
#define DARK_YELLOW		RGB(128,128,  0)
#define DARK_GRAY		RGB(128,128,128)
#define DARK_SITE		RGB( 64,  0,128)
#define LIGHT_BLUE		RGB(  0,  0,192)
#define LIGHT_GREEN		RGB(  0,192,  0)
#define LIGHT_CYAN		RGB(  0,192,192)
#define LIGHT_RED		RGB(192,  0,  0)
#define LIGHT_MAGENTA	RGB(192,  0,192)
#define LIGHT_YELLOW	RGB(192,192,  0)
#define LIGHT_GRAY		RGB(192,192,192)
#define PALE_GRAY		RGB(220,220,220)

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 클래스입니다.
#include <afxodlgs.h>       // MFC OLE 대화 상자 클래스입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 데이터베이스 클래스입니다.
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 데이터베이스 클래스입니다.
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afx.h>
#include <afxcontrolbars.h>
