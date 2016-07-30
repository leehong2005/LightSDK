/*!
* @file IViewOnKeyHandler.h 
* 
* @brief This file defines the key event for view.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/16
*/

#ifdef __cplusplus
#ifndef _IVIEWONKEYHANDLER_H_
#define _IVIEWONKEYHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief The key event definition.
*/
class IViewOnKeyHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IViewOnKeyHandler() {};

    /*!
    * @brief Called when a key event is dispatched to a view.
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSender    [I/ ] The pointer to SdkViewElement specifies event's sender.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL OnKey(const LPMSG lpMsg, SdkViewElement *pSender, SdkViewElement *pSource) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _IVIEWONKEYHANDLER_H_
#endif // __cplusplus
