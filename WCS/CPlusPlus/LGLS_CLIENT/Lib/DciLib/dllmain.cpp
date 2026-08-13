// dllmain.cpp : DLL 초기화 루틴입니다.
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>
#include "ControlGroup.h"
#include "Convert.h"
#include "DciButtonCtrl.h"
#include "DciControl.h"
#include "DciGridCtrl.h"
#include "DciMaster.h"
#include "DciOpboxCtrl.h"
#include "DciProperty.h"
#include "DciRackCtrl.h"
#include "DciRvCtrl.h"
#include "DciStaticCtrl.h"
#include "DciStatusCtrl.h"
#include "DciTrackCtrl.h"
#include "XmlDom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE DciLibDLL = { NULL, NULL };

#ifndef MERGE_DLLS
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("DciLib.DLL initializing!\n");
        if (!AfxInitExtensionModule(DciLibDLL, hInstance))
            return 0;
        new CDynLinkLibrary(DciLibDLL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("DciLib.DLL terminating!\n");
        AfxTermExtensionModule(DciLibDLL);
    }
    return 1;   // ok
}
#else
extern "C" void DciLib_Initialize(HINSTANCE hInstance, DWORD dwReason)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("DciLib initialize for merged build\n");
        // Initialize extension module for merged build
        if (!AfxInitExtensionModule(DciLibDLL, hInstance))
            return;
        new CDynLinkLibrary(DciLibDLL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("DciLib terminate for merged build\n");
        AfxTermExtensionModule(DciLibDLL);
    }
}
#endif
