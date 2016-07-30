/*!
* @file SdkInkCommon.h
* 
* @brief Includes common header file for Ink.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2009/10/19
* @version 1.0.0
*/

#ifdef __cplusplus
#ifndef _SDKINKCOMMON_H_
#define _SDKINKCOMMON_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

//////////////////////////////////////////////////////////////////////////
//
// Ink header file
//
//////////////////////////////////////////////////////////////////////////

#ifndef _PENINPUTPANEL_I_C_
#define _PENINPUTPANEL_I_C_
#include <peninputpanel_i.c>            // IID_IHandwrittenTextInsertion
#endif // _PENINPUTPANEL_I_C_

#ifndef _MSINKAUT_I_C_
#define _MSINKAUT_I_C_
#include <msinkaut_i.c>                 // For IID_IInkCollector, CLSID_InkCollector .etc.
#endif // _MSINKAUT_I_C_

#ifndef _MSINKAUT_H_
#define _MSINKAUT_H_
#include <msinkaut.h>                   // For ink interfaces
#endif // _MSINKAUT_H_

#include <atlbase.h>                    // defines CComModule, CComPtr, CComVariant
#include <atlcom.h>                     // defines IDispEventSimpleImpl
#include <Peninputpanel.h>              // IHandWrittenTextInsertion

#endif // __cplusplus
#endif // _SDKINKCOMMON_H_
