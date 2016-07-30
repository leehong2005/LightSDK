/*!
* @file PopupView.cpp
* 
* @brief This file defines PopupView class which is popup view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/18
*/

#include "PopupView.h"

PopupView::PopupView()
{
}

//////////////////////////////////////////////////////////////////////////

PopupView::~PopupView()
{
}

//////////////////////////////////////////////////////////////////////////

void PopupView::ShowView(BOOL isShow)
{
    ViewLayout::ShowView(isShow);

    BOOL isPopup = (VIEW_STATE_POPUP == (GetState() & VIEW_STATE_POPUP));
    if (isPopup)
    {
        if (isShow)
        {
            BringToFront();
        }

        Window *pWindow = GetWindow();
        if (NULL != pWindow)
        {
            pWindow->WindowViews.CurrentPopupView = isShow ? this : NULL;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void PopupView::SetPopupEnable(BOOL isPopupEnable)
{
    if (isPopupEnable)
    {
        AddFlag(VIEW_STATE_POPUP);
    }
    else
    {
        RemoveFlag(VIEW_STATE_POPUP);
    }
}
