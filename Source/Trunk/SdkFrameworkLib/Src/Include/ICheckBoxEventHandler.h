/*!
* @file ICheckBoxEventHandler.h
* 
* @brief This file defines the event for SdkCheckBox.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/07/13
*/

#ifdef __cplusplus
#ifndef _ICHECKBOXEVENTHANDLER_H_
#define _ICHECKBOXEVENTHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief ICheckBoxEventHandler class.
*/
class ICheckBoxEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ICheckBoxEventHandler() {}

    /*!
    * @brief Called when the checking is changed.
    *
    * @param pView      [I/ ] The event view.
    * @param isChecked  [I/ ] TRUE if current check box is checked, otherwise FALSE.
    */
    virtual void OnCheckedChanged(SdkCheckBox *pView, BOOL isChecked) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _ICHECKBOXEVENTHANDLER_H_
#endif // __cplusplus
