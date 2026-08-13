// dllmain.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE XmlLIbDLL = { NULL, NULL };

// Make DllMain internal to avoid duplicate external symbol when merging modules into one output
static int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("XmlLib.DLL 초기화\n");
        if (!AfxInitExtensionModule(XmlLIbDLL, hInstance))
            return 0;
        new CDynLinkLibrary(XmlLIbDLL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("XmlLib.DLL 종료\n");
        AfxTermExtensionModule(XmlLIbDLL);
    }
    return 1;
}
