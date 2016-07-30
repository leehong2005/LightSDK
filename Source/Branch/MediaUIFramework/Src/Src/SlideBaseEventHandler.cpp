/*!
* @file SlideBaseEventHandler.cpp
* 
* @brief This file defines the event for slide view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/18
*/

#include "SlideBaseEventHandler.h"
#include "SlideBase.h"

void SlideBaseEventHandler::OnOffsetChanged(SlideBase *pView, FLOAT offsetX, FLOAT offsetY)
{
    UNREFERENCED_PARAMETER(pView);
    UNREFERENCED_PARAMETER(offsetX);
    UNREFERENCED_PARAMETER(offsetY);
}

//////////////////////////////////////////////////////////////////////////

void SlideBaseEventHandler::OnFinishMoving(SlideBase *pView)
{
    UNREFERENCED_PARAMETER(pView);
}

//////////////////////////////////////////////////////////////////////////

void SlideBaseEventHandler::OnBeginMoving(SlideBase *pView)
{
    UNREFERENCED_PARAMETER(pView);
}
