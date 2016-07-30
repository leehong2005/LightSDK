/*!
* @file SdkCommonDef.h 
* 
* @brief This file defines the common declaration for global module.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/18
*/

#ifdef __cplusplus
#ifndef _SDKCOMMONDEF_H_
#define _SDKCOMMONDEF_H_

#include <string>

using namespace std;


// The std string.
#ifdef UNICODE
typedef std::wstring CryptString;
#else
typedef std::string  CryptString;
#endif // !UNICODE


// The drop effect value
typedef DWORD DROPEFFECT;


// The instance of current module.
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


#endif // _SDKCOMMONDEF_H_
#endif // __cplusplus
