/*!
* @file Common.h 
* 
* @brief This file defines the common module header file,
*        which shall be used for the applications.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#ifdef __cplusplus
#ifndef _COMMON_H_
#define _COMMON_H_

//////////////////////////////////////////////////////////////////////////
//
// Include section
//

#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <tchar.h>
#include <objbase.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <UIAnimation.h>
#include <wincodec.h>
#include <dwrite.h>
#include <shobjidl.h>
#include <oleidl.h>
#include <strsafe.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <propvarutil.h>
#include <structuredquery.h>
#include <knownfolders.h>
#include <propkey.h>
#include <MMSystem.h>
#include <WindowsX.h>

using namespace std;
using namespace D2D1;


//////////////////////////////////////////////////////////////////////////
//
// Library section
//

// For Windows Imaging Component.
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "strsafe.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "structuredquery.lib")
#pragma comment(lib, "propsys.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Winmm.lib")

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#pragma warning(disable : 4995)     // functions were marked as #pragma deprecated.


//////////////////////////////////////////////////////////////////////////
// 
// The defines switch of flags dllexport and dllimport
//
//////////////////////////////////////////////////////////////////////////

// Defines the switch of DLL export and import
#define COMMON_EXPORT

#ifdef COMMON_EXPORT
#   define CLASS_DECLSPEC __declspec(dllexport)
#   define EXPIMP_TEMPLATE
#else
#   define CLASS_DECLSPEC __declspec(dllimport)
#   define EXPIMP_TEMPLATE extern
#endif



#endif // _COMMON_H_
#endif // __cplusplus


//////////////////////////////////////////////////////////////////////////
//
// END of file
//
//////////////////////////////////////////////////////////////////////////