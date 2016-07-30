/*!
* @file SlideBaseEventHandler.h
* 
* @brief This file defines the event for slide view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/18
*/

#ifdef __cplusplus
#ifndef _SLIDEBASEEVENTHANDLER_H_
#define _SLIDEBASEEVENTHANDLER_H_

#include "CommonInclude.h"

class SlideBase;

/*!
* @brief SlideBaseEventHandler class.
*/
class CLASS_DECLSPEC SlideBaseEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~SlideBaseEventHandler() {};

    /*!
    * @brief Called when view's offset is changed.
    *
    * @param pView      [I/ ] The view calls this function.
    * @param offstX     [I/ ] The offset on x-coordinate.
    * @param offstY     [I/ ] The offset on y-coordinate.
    */
    virtual void OnOffsetChanged(SlideBase *pView, FLOAT offsetX, FLOAT offsetY);

    /*!
    * @brief Called when view finishes moving.
    *
    * @param pView      [I/ ] The view calls this function.
    */
    virtual void OnFinishMoving(SlideBase *pView);

    /*!
    * @brief Called when view begin moving.
    *
    * @param pView      [I/ ] The view calls this function.
    */
    virtual void OnBeginMoving(SlideBase *pView);
};

#endif //_SLIDEBASEEVENTHANDLER_H_
#endif // __cplusplus
