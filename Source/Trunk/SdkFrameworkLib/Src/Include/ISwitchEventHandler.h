/*!
* @file ISwitchEventHandler.h 
* 
* @brief This file defines the event for viewPreviewEx.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Liu Qiao
* @date 2011/2/12
*/

#ifdef __cplusplus
#ifndef _ISWITCHEVENTHANDLER_H_
#define _ISWITCHEVENTHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief The ISwitchEventHandler defines the event for viewpreviewEx.
*/
class CLASS_DECLSPEC ISwitchEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ISwitchEventHandler() {};

    /*!
    * @brief The destructor function.
    *
    * @param view   [I/ ] The pointer ViewElement, you should NOT delete the pointer.
    */
    virtual void OnSwitchEvent(SdkViewPreviewEx* viewPreviewEx) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _ISWITCHEVENTHANDLER_H_
#endif // __cplusplus