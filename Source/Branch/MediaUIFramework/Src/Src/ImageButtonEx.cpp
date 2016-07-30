/*!
* @file ImageButtonEx.cpp
* 
* @brief This file defines the ImageButton extension class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang & Li Hong
* @date 2011/01/12
*/

#include "ImageButtonEx.h"

ImageButtonEx::ImageButtonEx(BOOL isShowDefBK) : ImageButton(isShowDefBK),
                                                 m_isKeepFocus(FALSE),
                                                 m_isPreSelected(FALSE),
                                                 m_isAutoRemoveFocus(TRUE)
{
}

//////////////////////////////////////////////////////////////////////////

ImageButtonEx::~ImageButtonEx()
{
}

//////////////////////////////////////////////////////////////////////////

void ImageButtonEx::KeepFocusState(IN BOOL keepFocus)
{
    m_isKeepFocus = keepFocus;
}

//////////////////////////////////////////////////////////////////////////

void ImageButtonEx::AutoRemoveFocusState(IN BOOL autoRemoveFocus)
{
    m_isAutoRemoveFocus = autoRemoveFocus;
}

//////////////////////////////////////////////////////////////////////////

void ImageButtonEx::SetFocusState()
{
    AddFlag(VIEW_STATE_SELECTED);
}

//////////////////////////////////////////////////////////////////////////

void ImageButtonEx::RemoveFocusState()
{
    RemoveFlag(VIEW_STATE_SELECTED);
}

//////////////////////////////////////////////////////////////////////////

BOOL ImageButtonEx::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
    BOOL isTouchOn = IsPtInRect(xPos, yPos);
    BOOL handled = FALSE;

    switch(message)
    {
    case WM_LBUTTONUP:
        {
            if (m_isKeepFocus)
            {
                BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
                if (isTouchOn && isPressed)
                {
                    AddFlag(VIEW_STATE_SELECTED);

                    if (m_isAutoRemoveFocus && m_isPreSelected)
                    {
                        RemoveFocusState();
                    }

                    if (m_isAutoRemoveFocus && m_isPreSelected)
                    {
                        RemoveFocusState();
                    }
                }
                else if (!m_isPreSelected)
                {
                    RemoveFocusState();
                }
            }
            else
            {
                RemoveFocusState();
            }

            handled = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
        }
        break;

    case WM_LBUTTONDOWN:
        {
            if (isTouchOn)
            {
                m_isPreSelected = (VIEW_STATE_SELECTED == (GetState() & VIEW_STATE_SELECTED));
                AddFlag(VIEW_STATE_PRESSED);
                Invalidate();
                handled = TRUE;
            }
        }
        break;
    }

    ImageButton::OnMouseEvent(message, wParam, lParam);

    return handled;
}

//////////////////////////////////////////////////////////////////////////
