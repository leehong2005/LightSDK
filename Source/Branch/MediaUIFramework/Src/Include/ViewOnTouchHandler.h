/*!
* @file ViewOnTouchHandler.h
* 
* @brief This file defines the key event for view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/16
*/

#ifdef __cplusplus
#ifndef _VIEWONTOUCHHANDLER_H_
#define _VIEWONTOUCHHANDLER_H_

class ViewElement;

#include "CommonInclude.h"

/*!
* @brief The touch event definition.
*/
class CLASS_DECLSPEC ViewOnTouchHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ViewOnTouchHandler() {};

    /*!
    * @brief Called when a touch event is dispatched to a view.
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSender    [I/ ] The pointer to ViewElement specifies event's sender.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL OnTouch(const LPMSG lpMsg, ViewElement *pSender, ViewElement *pSource);
};

#endif // _VIEWONTOUCHHANDLER_H_
#endif // __cplusplus
