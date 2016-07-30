/*!
* @file FrameworkHelper.h 
* 
* @brief This file defines common utility class used for framework.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/22
*/

#ifdef __cplusplus
#ifndef _FRAMEWORKHELPER_H_
#define _FRAMEWORKHELPER_H_

#include "FrameworkCommon.h"

class CLASS_DECLSPEC FrameworkHelper
{
public:

    /*!
    * @brief Get instance of current process, if this function is in DLL project,
    *        returned HMODULE is the instance of DLL.
    *
    * @return The handle to current process if succeeds, otherwise return NULL.
    */
    static HMODULE GetCurrentModuleHandle();
};

#endif // _FRAMEWORKHELPER_H_
#endif // __cplusplus
