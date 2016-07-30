/*!
* @file IViewOnMouseHandler.h 
* 
* @brief This file defines the key event for view.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/16
*/

#ifdef __cplusplus
#ifndef _IVIEWONMOUSEHANDLER_H_
#define _IVIEWONMOUSEHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief The mouse event definition.
*/
class IViewOnMouseHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IViewOnMouseHandler() {};

    /*!
    * @brief Called when a mouse event is dispatched to a view.
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSender    [I/ ] The pointer to SdkViewElement specifies event's sender.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL OnMouse(const LPMSG lpMsg, SdkViewElement *pSender, SdkViewElement *pSource) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _IVIEWONMOUSEHANDLER_H_
#endif // __cplusplus
