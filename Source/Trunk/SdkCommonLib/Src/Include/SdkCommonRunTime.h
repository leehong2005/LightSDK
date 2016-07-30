/*!
* @file SdkCommonRunTime.h
* 
* @brief The SdkCommonRunTime class implements the run-time of this library.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#ifdef __cplusplus
#ifndef _SDKCOMMONRUNTIME_H_
#define _SDKCOMMONRUNTIME_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The SdkCommonRunTime run time, this class should be called before all
*        calling to this library, such as write log, initialize GDI plus, and so on.
*        typically, this class is used in entrance point of an application.
*/
class CLASS_DECLSPEC SdkCommonRunTime
{
public:

    /*!
    * @brief Initialize the run time of this library.
    *
    * @return TRUE if succeeds, otherwise FALSE.
    */
    static BOOL InitializeRunTime();

    /*!
    * @brief Uninitialize the run time of this library.
    */
    static void UninitializeRunTime();
};

END_NAMESPACE_COMMON

#endif // _SDKCOMMONRUNTIME_H_
#endif // __cplusplus
