/*!
* @file IRadioButtonEventHandler.h
* 
* @brief This file defines the event for SdkRadioButton.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/07/13
*/

#ifdef __cplusplus
#ifndef _IRADIOBUTTONEVENTHANDLER_H_
#define _IRADIOBUTTONEVENTHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief IRadioButtonEventHandler class.
*/
class IRadioButtonEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IRadioButtonEventHandler() {}

    /*!
    * @brief Called when the selected is changed.
    *
    * @param pView      [I/ ] The event view.
    * @param isChecked  [I/ ] TRUE if current check box is selected, otherwise FALSE.
    */
    virtual void OnSelectedChanged(SdkRadioButton *pView, BOOL isSelected) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _IRADIOBUTTONEVENTHANDLER_H_
#endif // __cplusplus
