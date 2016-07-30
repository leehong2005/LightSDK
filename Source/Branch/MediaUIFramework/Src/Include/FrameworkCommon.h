/*!
* @file FrameworkCommon.h 
* 
* @brief This file defines common utility class used for framework.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/22
*/

#ifdef __cplusplus
#ifndef _FRAMEWORKCOMMON_H_
#define _FRAMEWORKCOMMON_H_

#include "Common.h"


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


#endif // _FRAMEWORKCOMMON_H_
#endif // __cplusplus
