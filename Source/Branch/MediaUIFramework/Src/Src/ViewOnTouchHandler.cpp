/*!
* @file ViewOnTouchHandler.cpp
* 
* @brief This file defines the key event for view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/16
*/

#include "ViewOnTouchHandler.h"

BOOL ViewOnTouchHandler::OnTouch(const LPMSG lpMsg, ViewElement *pSender, ViewElement *pSource)
{
    UNREFERENCED_PARAMETER(lpMsg);
    UNREFERENCED_PARAMETER(pSender);
    UNREFERENCED_PARAMETER(pSource);

    return FALSE;
}
