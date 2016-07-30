/*!
* @file ISlideBaseEventHandler.h
* 
* @brief This file defines the event for slide view.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/18
*/

#ifdef __cplusplus
#ifndef _ISLIDEBASEEVENTHANDLER_H_
#define _ISLIDEBASEEVENTHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief ISlideBaseEventHandler class.
*/
class ISlideBaseEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ISlideBaseEventHandler() {};

    /*!
    * @brief Called when view's offset is changed.
    *
    * @param pView      [I/ ] The view calls this function.
    * @param offstX     [I/ ] The offset on x-coordinate.
    * @param offstY     [I/ ] The offset on y-coordinate.
    */
    virtual void OnOffsetChanged(SdkSlideBase *pView, FLOAT offsetX, FLOAT offsetY) = 0;

    /*!
    * @brief Called when view finishes moving.
    *
    * @param pView      [I/ ] The view calls this function.
    */
    virtual void OnFinishMoving(SdkSlideBase *pView) = 0;

    /*!
    * @brief Called when view begin moving.
    *
    * @param pView      [I/ ] The view calls this function.
    */
    virtual void OnBeginMoving(SdkSlideBase *pView) = 0;
};

END_NAMESPACE_CALLBACK

#endif //_ISLIDEBASEEVENTHANDLER_H_
#endif // __cplusplus
