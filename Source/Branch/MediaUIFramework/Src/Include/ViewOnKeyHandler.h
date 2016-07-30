/*!
* @file ViewOnKeyHandler.h 
* 
* @brief This file defines the key event for view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/16
*/

#ifdef __cplusplus
#ifndef _VIEWONKEYHANDLER_H_
#define _VIEWONKEYHANDLER_H_

class ViewElement;

#include "Common.h"

/*!
* @brief The key event definition.
*/
class CLASS_DECLSPEC ViewOnKeyHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ViewOnKeyHandler() {};

    /*!
    * @brief Called when a key event is dispatched to a view.
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSender    [I/ ] The pointer to ViewElement specifies event's sender.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL OnKey(const LPMSG lpMsg, ViewElement *pSender, ViewElement *pSource);
};

#endif // _VIEWONKEYHANDLER_H_
#endif // __cplusplus
