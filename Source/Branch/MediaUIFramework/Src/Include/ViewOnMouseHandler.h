/*!
* @file ViewOnMouseHandler.h 
* 
* @brief This file defines the key event for view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/16
*/

#ifdef __cplusplus
#ifndef _VIEWONMOUSEHANDLER_H_
#define _VIEWONMOUSEHANDLER_H_

class ViewElement;

#include "CommonInclude.h"

/*!
* @brief The mouse event definition.
*/
class CLASS_DECLSPEC ViewOnMouseHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ViewOnMouseHandler() {};

    /*!
    * @brief Called when a mouse event is dispatched to a view.
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSender    [I/ ] The pointer to ViewElement specifies event's sender.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL OnMouse(const LPMSG lpMsg, ViewElement *pSender, ViewElement *pSource);
};

#endif // _VIEWONMOUSEHANDLER_H_
#endif // __cplusplus
