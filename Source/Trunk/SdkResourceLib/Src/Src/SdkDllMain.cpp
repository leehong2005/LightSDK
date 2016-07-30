/*!
* @file SdkDllMain.h
* 
* @brief The entrance of this project.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/21
*/

#ifdef __cplusplus
#ifndef _SDKDLLMAIN_H_
#define _SDKDLLMAIN_H_

#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwLoadReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);

    switch (dwLoadReason)
    {
    case DLL_PROCESS_ATTACH:
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

#endif // _SDKDLLMAIN_H_
#endif // __cplusplus
