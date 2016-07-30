/*!
* @file SlideLayout.cpp
* 
* @brief This file defines class SlideBase, which supports drag to slide and sliding animation after dragging.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/28
*/

#include "SlideBase.h"

SlideBase::SlideBase() : m_pTransAnim(NULL),
                         m_pEventHandler(NULL),
                         m_hasMoved(FALSE),
                         m_isSlideEnable(TRUE),
                         m_isSlideAniEnable(TRUE),
                         m_fSlideStep(-1.0f),
                         m_slideDirection(SLIDEDIRECTIOIN_VERTICAL)
{
    m_ptDown.x = 0;
    m_ptDown.y = 0;

    SetMaxWidth(2000000);
    SetMaxHeight(2000000);
    AddFlag(VIEW_STATE_DISABLEANIMMAT);
    AddFlag(VIEW_STATE_CLEARANIMATION);
}

//////////////////////////////////////////////////////////////////////////

SlideBase::~SlideBase()
{
    SAFE_DELETE(m_pTransAnim);
}

//////////////////////////////////////////////////////////////////////////

void SlideBase::SetSlideDirection(SLIDEDIRECTIOIN slideDirection)
{
    m_slideDirection = slideDirection;
}

//////////////////////////////////////////////////////////////////////////

SLIDEDIRECTIOIN SlideBase::GetSlideDirection()
{
    return m_slideDirection;
}

//////////////////////////////////////////////////////////////////////////

void SlideBase::SetSlideEnable(BOOL isSlideEnable)
{
    m_isSlideEnable = isSlideEnable;

    if (!m_isSlideEnable)
    {
        m_isSlideAniEnable = FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideBase::IsSlideEnable()
{
    return m_isSlideEnable;
}

//////////////////////////////////////////////////////////////////////////

void SlideBase::SetSlideAnimationEnable(BOOL isEnable)
{
    m_isSlideAniEnable = isEnable;
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideBase::IsSlideAnimationEnable()
{
    return m_isSlideAniEnable;
}

//////////////////////////////////////////////////////////////////////////

void SlideBase::SetOffsetChangedHandler(SlideBaseEventHandler *pEventHandler)
{
    m_pEventHandler = pEventHandler;
}

//////////////////////////////////////////////////////////////////////////

FLOAT SlideBase::GetSlideOffset()
{
    FLOAT offsetXY = 0;

    switch (m_slideDirection)
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        offsetXY = GetLeft();
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        offsetXY = GetTop();
        break;
    }

    return offsetXY;
}

//////////////////////////////////////////////////////////////////////////

void SlideBase::SetSlideStep(FLOAT fStep)
{
    m_fSlideStep = fStep;
}

//////////////////////////////////////////////////////////////////////////

FLOAT SlideBase::GetSlideStep()
{
    return m_fSlideStep;
}

//////////////////////////////////////////////////////////////////////////

void SlideBase::OffsetViewLayout(FLOAT offsetX, FLOAT offsetY)
{
    FLOAT x = GetLeft();
    FLOAT y  = GetTop();

    switch (m_slideDirection)
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        x = offsetX;
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        y = offsetY;
        break;
    }

    if (NULL != m_pEventHandler)
    {
        m_pEventHandler->OnOffsetChanged(this, offsetX, offsetY);
    }

    SetViewPos(x, y);
    Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void SlideBase::OnPaint()
{
    if ( !IsVisible() || (NULL == m_pWindow) )
    {
        return;
    }

    Animation *pAnimation = GetAnimation();
    if (NULL != pAnimation)
    {
        if (TRANSFORM_TYPE_TRANSLATE == pAnimation->GetAnimationType())
        {
            TranslateAnimation *pTransAnim = dynamic_cast<TranslateAnimation*>(pAnimation);
            if (NULL != pTransAnim)
            {
                DOUBLE offsetX = 0.0f, offsetY = 0.0f;
                pTransAnim->GetValue(&offsetX, &offsetY);

                OffsetViewLayout((FLOAT)offsetX, (FLOAT)offsetY);
            }
        }
    }

    // Do not call base class's onDrawItem, because we do NOT want draw background color or image.
    // Only draw view's children.
    ViewLayout::OnDrawChildren();

    if (NULL != pAnimation)
    {
        if (TRANSFORM_TYPE_TRANSLATE == pAnimation->GetAnimationType())
        {
            // If the animation does not finish, go on to invalidate window.
            BOOL isFinish = FALSE;
            pAnimation->IsFinish(&isFinish);
            if (isFinish)
            {
                BOOL isClearAnim = (VIEW_STATE_CLEARANIMATION == (VIEW_STATE_CLEARANIMATION & GetState()));
                if (isClearAnim)
                {
                    ClearAnimation();
                    StartRevertAnimation();
                }
            }
            else
            {
                ForceInvalidate();
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideBase::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL handled = FALSE;

    if (0 == GetChildCount())
    {
        return handled;
    }

    // Indicates whether has animation.
    BOOL hasAnimation = FALSE;
    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
    BOOL isTouchOn = IsPtInRect(xPos, yPos);

    switch (message)
    {
    case WM_LBUTTONDOWN:
        {
            if (isTouchOn)
            {
                m_ptDown.x = xPos;
                m_ptDown.y = yPos;
                handled = TRUE;
                m_hasMoved = FALSE;

                AddFlag(VIEW_STATE_PRESSED);
                RemoveFlag(VIEW_STATE_CANCELEVENT);
                OnBeginMoving();
                ClearAnimation();
            }
        }
        break;

    case WM_LBUTTONUP:
        {
            RemoveFlag(VIEW_STATE_CANCELEVENT);
            BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
            if (isPressed && m_hasMoved)
            {
                RemoveFlag(VIEW_STATE_PRESSED);
                hasAnimation = TRUE;

                FLOAT time = (FLOAT)m_pWindow->GetSpeedSampleTime();
                if (0 == time)
                {
                    time = 1;
                }

                FLOAT speedX = (FLOAT)m_pWindow->GetSpeedX();
                FLOAT speedY = (FLOAT)m_pWindow->GetSpeedY();

                // If the speed is too small, set to zero.
                if (abs(speedX) <= 0.5f)
                {
                    speedX = 0.0f;
                }

                if (abs(speedY) <= 0.5f)
                {
                    speedY = 0.0f;
                }

                FLOAT accerX = speedX / time;
                if (0 == accerX)
                {
                    accerX = 1;
                }

                FLOAT accerY = speedY / time;
                if (0 == accerY)
                {
                    accerY = 1;
                }

                if ((0.0f == speedX) && (SLIDEDIRECTIOIN_HORIZONTAL == m_slideDirection))
                {
                    hasAnimation = FALSE;
                }

                if ((0.0f == speedY) && (SLIDEDIRECTIOIN_VERTICAL == m_slideDirection))
                {
                    hasAnimation = FALSE;
                }

                speedX *= 3;
                speedY *= 3;
                FLOAT fromX = 0;
                FLOAT toX = (speedX * speedX) / (2 * accerX);
                FLOAT fromY = 0;
                FLOAT toY = (speedY * speedY) / (2 * accerY);

                if (hasAnimation)
                {
                    BOOL isSucceed = StartSlideAnimation(fromX, toX, fromY, toY);
                    if (!isSucceed)
                    {
                        OnFinishMoving();
                    }
                    handled = TRUE;
                }
                else
                {
                    OnFinishMoving();
                }
            }

            if (!handled)
            {
                StartRevertAnimation();
            }

            m_hasMoved = FALSE;
        }
        break;

    case WM_MOUSEMOVE:
        {
            BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
            if (isTouchOn && isPressed && m_isSlideEnable)
            {
                handled = TRUE;
                FLOAT offsetX = xPos - m_ptDown.x;
                FLOAT offsetY = yPos - m_ptDown.y;
                m_hasMoved = ((0 != offsetX) || (0 != offsetY));

                if (m_hasMoved)
                {
                    m_ptDown.x = xPos;
                    m_ptDown.y = yPos;
                    OffsetViewLayout(GetLeft() + offsetX, GetTop() + offsetY);
                }
            }
        }
        break;
    }

    if (!handled || !m_hasMoved && !hasAnimation)
    {
        ViewLayout::OnMouseEvent(message, wParam, lParam);
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

void SlideBase::OnAnimationEnd(Animation *pAnimation)
{
    UNREFERENCED_PARAMETER(pAnimation);

    OnFinishMoving();
}

//////////////////////////////////////////////////////////////////////////

void SlideBase::OnFinishMoving()
{
    if (NULL != m_pEventHandler)
    {
        m_pEventHandler->OnFinishMoving(this);
    }
}

//////////////////////////////////////////////////////////////////////////

void SlideBase::OnBeginMoving()
{
    if (NULL != m_pEventHandler)
    {
        m_pEventHandler->OnBeginMoving(this);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideBase::StartRevertAnimation()
{
    if (!m_isSlideEnable || !m_isSlideAniEnable)
    {
        return FALSE;
    }

    ViewLayout *pParentLayout = GetParent(NULL);
    if (NULL == pParentLayout)
    {
        return FALSE;
    }

    BOOL handled = FALSE;

    switch (m_slideDirection)
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        {
            FLOAT fromX = 0.0f, toX = 0.0f;
            if (GetLeft() > 0)
            {
                handled = StartSlideAnimation(0, -GetLeft(), 0, 0);
            }
            else if (GetRight() < pParentLayout->GetWidth())
            {
                fromX = 0;
                toX = pParentLayout->GetWidth() - GetRight();
                handled = StartSlideAnimation(fromX, toX, 0, 0);
            }
        }
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        {
            FLOAT fromY = 0.0f, toY = 0.0f;
            if (GetTop() > 0)
            {
                fromY = 0;
                toY = -GetTop();
                handled = StartSlideAnimation(0, 0, fromY, toY);
            }
            else if (GetBottom() < pParentLayout->GetHeight())
            {
                fromY = 0;
                toY = pParentLayout->GetHeight() - GetBottom();
                handled = StartSlideAnimation(0, 0, fromY, toY);
            }
        }
        break;
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideBase::StartSlideAnimation(FLOAT fromX, FLOAT toX, FLOAT fromY, FLOAT toY)
{
    if (!m_isSlideEnable || !m_isSlideAniEnable)
    {
        return FALSE;
    }

    if (NULL == m_pTransAnim)
    {
        m_pTransAnim = new TranslateAnimation();
    }

    switch (m_slideDirection)
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        {
            if (toX != fromX)
            {
                fromX += GetLeft();
                toX   += GetLeft();
                m_pTransAnim->SetTranslateXInfo(fromX, toX, 1.0f, 0, 1.0f);
            }
        }
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        {
            if (toY != fromY)
            {
                fromY += GetTop();
                toY   += GetTop();
                m_pTransAnim->SetTranslateYInfo(fromY, toY, 1.0f, 0, 1.0f);
            }
        }
        break;
    }

    SetAnimation(m_pTransAnim);
    StartAnimation();

    return TRUE;
}
