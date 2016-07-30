/*!
* @file SdkUIRunTime.h 
* 
* @brief This file defines the class SdkUIRunTime, which is used to initialize UI runtime.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/07/12
*/

#ifdef __cplusplus
#ifndef _SDKUIRUNTIME_H_
#define _SDKUIRUNTIME_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_UILIB

/*!
* @brief This class provides methods to initialize UI run-times.
*/
class CLASS_DECLSPEC SdkUIRunTime
{
public:

    /*!
    * @brief Initialize UI run-time. This method should be called before all operations
    *        about the user interface, typically it is called at the beginning of application
    *        entrance, WinMain, for example.
    *
    * @return TRUE if succeed, otherwise FLASE.
    */
    static BOOL InitializeUIRunTime();

    /*!
    * @brief This function should be called before the application exist.
    */
    static void UninitializeUIRunTime();
};

END_NAMESPACE_UILIB

#endif // _SDKUIRUNTIME_H_
#endif // __cplusplus
