/*!
* @file SdkViewPreviewEx.cpp 
* 
* @brief This file defines the view preview layout.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2011/01/25
*/

//#include "stdafx.h"
#include "SdkViewPreviewEx.h"
#include <Windows.h>
#include <math.h>

USING_NAMESPACE_VIEWS

#define ZOOM_RATE               0.2f
#define MIN_ZOOM                1
#define MAX_ZOOM                20

#define MOUSE_WHEEL_ZDISTANCE   120

//////////////////////////////////////////////////////////////////////////

SdkViewPreviewEx::SdkViewPreviewEx() : m_fRealWidth(0),
                                 m_fOffsetX(0),
                                 m_fBackOffsetX(0),
                                 m_fZoomRate(1),
                                 m_bLbuttonDown(FALSE),
                                 m_bUpOrDown(TRUE),
                                 m_fElementHeight(DEFAULT_ELEMENT_HEIGHT),
                                 m_fElementWidth(DEFAULT_ELEMENT_WIDTH),
                                 m_bFitChildToWindow(FALSE),
                                 m_nCurShowIndex(-1) ,
                                 m_pSwitchEventHandler(NULL),
                                 m_pLeftTranslateAnimation(NULL),
                                 m_pRightTranslateAnimation(NULL),
                                 m_pDynamicTranslate(NULL),
                                 m_animationType(ANIMATION_UNIFORM),
                                 m_dAnimationTime(0),
                                 m_bMove(FALSE),
                                 m_isAnimationEnd(TRUE),
                                 m_bZoom(FALSE),
                                 m_bCanPanning(FALSE),
                                 m_bCanZoom(TRUE),
                                 m_bFront(FALSE),
                                 m_bButtonAnimationIsEnd(TRUE)
{
    this->SetMinHeight(MIN_HEIGHT);
    this->SetMinWidth(MIN_WIDTH);

    m_pLeftTranslateAnimation = new SdkTranslateAnimation();
    m_pLeftTranslateAnimation->AddAnimationListener(this);
    m_pLeftTranslateAnimation->AddAnimationTimerListener(this);

    m_pRightTranslateAnimation = new SdkTranslateAnimation();
    m_pRightTranslateAnimation->AddAnimationListener(this);
    m_pRightTranslateAnimation->AddAnimationTimerListener(this);

    m_pDynamicTranslate = new SdkTranslateAnimation();
    m_pDynamicTranslate->AddAnimationTimerListener(this);
    m_pDynamicTranslate->AddAnimationListener(this);
}

//////////////////////////////////////////////////////////////////////////

SdkViewPreviewEx::~SdkViewPreviewEx()
{
    m_pLeftTranslateAnimation->RemoveAnimationListener(this);
    m_pLeftTranslateAnimation->RemoveAnimationTimerListener(this);
    SAFE_DELETE(m_pLeftTranslateAnimation);

    m_pRightTranslateAnimation->RemoveAnimationListener(this);
    m_pRightTranslateAnimation->RemoveAnimationTimerListener(this);
    SAFE_DELETE(m_pRightTranslateAnimation);

    m_pDynamicTranslate->RemoveAnimationListener(this);
    m_pDynamicTranslate->RemoveAnimationTimerListener(this);
    SAFE_DELETE(m_pDynamicTranslate);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::SetEelementArea(FLOAT fWidth, FLOAT fHeight)
{
    m_fElementWidth = fabs(fWidth);
    m_fElementHeight = fabs(fHeight);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewPreviewEx::AddView(IN const SdkViewElement *pChild)
{
    SdkViewLayout *pLayout = new SdkViewLayout();
    m_vecFrame.push_back(pLayout);
    m_vecContent.push_back(const_cast<SdkViewElement *>(pChild));
    pLayout->AddView(pChild);
    return SdkViewLayout::AddView(pLayout);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewPreviewEx::RemoveAllChildren(BOOL isClearCache)
{
    this->StopAnimation();

    m_fRealWidth        = 0;
    m_fOffsetX          = 0;
    m_fZoomRate         = 1;
    m_bLbuttonDown      = FALSE;
    m_bUpOrDown         = TRUE;
    m_bZoom             = FALSE;
    m_bCanPanning       = FALSE;
    m_bCanZoom          = TRUE;
    m_bMove             = FALSE;
    m_bFront            = FALSE;

    m_vecFrame.clear();
    m_vecContent.clear();

    return SdkViewLayout::RemoveAllChildren();
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::SetCanZoom(BOOL bCanZoom)
{
    m_bCanZoom = bCanZoom;
    m_bMove = FALSE;
    if(!m_bCanZoom)
    {
        this->CancelZoom();
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewPreviewEx::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL bHandle = SdkViewLayout::OnMouseEvent(message, wParam, lParam);

    switch(message)
    {
    case WM_LBUTTONDOWN:
        bHandle = TRUE;
        this->OnLButtonDown(wParam, lParam);
        break;
    case WM_LBUTTONUP:
        this->OnLButtonUp(wParam, lParam);
        break;
    case WM_MOUSEMOVE:
        this->OnMouseMove(wParam, lParam);
        break;
    case WM_MOUSELEAVE:
        this->OnMouseLeave(wParam, lParam);
        break;
	case WM_MOUSEWHEEL:
        this->OnMouseWheel(wParam, lParam);
        break;
	default:
        break;
    }

    return bHandle;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewPreviewEx::OnTouchEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(wParam);

    GESTUREINFO gi;
    ZeroMemory(&gi, sizeof(GESTUREINFO));
    gi.cbSize = sizeof(GESTUREINFO);
    POINT pointCur = {0};
    BOOL bResult  = ::GetGestureInfo((HGESTUREINFO)lParam, &gi);
    if(bResult)
    {
        // Interpret the gesture
        switch (gi.dwID)
        {
           case GID_ZOOM:
               
               switch (gi.dwFlags)
               {
               case GF_BEGIN:
                   m_dwZoomArguments = LODWORD(gi.ullArguments);
                   m_lastPoint.x = gi.ptsLocation.x;
                   m_lastPoint.y = gi.ptsLocation.y;
                   break;

               default:
                   {
                       if(m_bCanZoom && m_isAnimationEnd)
                       {
                           POINT ptZoomCenter = {0};
                           double k = 0.0;
                           // We read here the second point of the gesture. This is middle point between 
                           // fingers in this new position.
                           pointCur.x = gi.ptsLocation.x;
                           pointCur.y = gi.ptsLocation.y;

                           // We have to calculate zoom center point 
                           ptZoomCenter.x = (m_lastPoint.x + pointCur.x)/2;
                           ptZoomCenter.y = (m_lastPoint.y + pointCur.y)/2;

                           // The zoom factor is the ratio between the new and the old distance. 
                           // The new distance between two fingers is stored in gi.ullArguments 
                           // (lower DWORD) and the old distance is stored in _dwArguments.
                           k = (double)(LODWORD(gi.ullArguments)) / (double)(m_dwZoomArguments);

                           // Now we process zooming in/out of the object
                           if(k > 1.0f)
                           {
                               ZoomOut((FLOAT)k, &ptZoomCenter);
                           }
                           else
                           {
                               ZoomIn((FLOAT)k, &ptZoomCenter);
                           }

                           // Now we have to store new information as a starting information 
                           // for the next step in this gesture.
                           m_lastPoint = pointCur;
                           m_dwZoomArguments = LODWORD(gi.ullArguments);
                       }

                       break;
                   }
               }
               break;

           case GID_PAN:
               {
                   switch (gi.dwFlags)
                   {
                   case GF_BEGIN:
                       m_lastPoint.x = gi.ptsLocation.x;
                       m_lastPoint.y = gi.ptsLocation.y;
                       this->OnLButtonDown(NULL, MAKELPARAM(m_lastPoint.x, m_lastPoint.y));
                       break;
                   default:

                       if(gi.dwFlags & GF_END)
                       {
                           this->OnLButtonUp(NULL, MAKELPARAM(gi.ptsLocation.x, gi.ptsLocation.y));
                           break;
                       }

                       // We read the second point of this gesture. It is a middle point
                       // between fingers in this new position
                       pointCur.x = gi.ptsLocation.x;
                       pointCur.y = gi.ptsLocation.y;

                       if(pointCur.x != m_lastPoint.x > 0)
                       {
                           m_bUpOrDown = (pointCur.x - m_lastPoint.x > 0) ? TRUE : FALSE;
                       }

                       if(m_bLbuttonDown && m_isAnimationEnd)
                       {
                           // We apply move operation of the object
                           if(m_bCanPanning)
                           {
                               DoPanning(m_lastPoint, pointCur);
                           }
                           else
                           {
                               this->OnMouseMove(NULL, MAKELPARAM(pointCur.x, pointCur.y));
                           }
                       }
                       
                       // We have to copy second point into first one to prepare
                       // for the next step of this gesture.
                       m_lastPoint = pointCur;
                       break;
                   }
                   break;
               }
               break;
           default:
               break;
        }
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::OnLayout(BOOL bChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    UNREFERENCED_PARAMETER(left);
    UNREFERENCED_PARAMETER(top);

    if (bChanged)
    {
        m_pLeftTranslateAnimation->SetTranslateXInfo(0, -width, m_dAnimationTime);
        m_pRightTranslateAnimation->SetTranslateXInfo(0, width, m_dAnimationTime);
    }
    ResizeFrame(width, height);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);

    m_bLbuttonDown = TRUE;
    m_lastPoint.x = GET_X_LPARAM(lParam);
    m_lastPoint.y = GET_Y_LPARAM(lParam);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    if(m_bLbuttonDown)
    {
        m_bLbuttonDown = FALSE;

        if(!m_bCanPanning)
        {
            FLOAT fTempStartX = 0.0f;
            FLOAT fTempEndX = 0.0f;
            this->CalcOffset(fTempStartX, fTempEndX);

            if (m_isAnimationEnd && (fTempStartX != fTempEndX))
            {
                m_isAnimationEnd = FALSE;
                m_bCanZoom = FALSE;
                m_bMove = TRUE;
                m_pDynamicTranslate->SetTranslateXInfo(fTempStartX, fTempEndX, m_dAnimationTime);
                m_pDynamicTranslate->Start();
            }
        }
        else
        {
            if (m_isAnimationEnd)
            {
                m_bMove = FALSE;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);

    if(m_bLbuttonDown && m_isAnimationEnd)
    {
        m_bMove = TRUE;

        POINT point = { 0 };
        point.x = GET_X_LPARAM(lParam);
        point.y = GET_Y_LPARAM(lParam);

        if(!m_bCanPanning)
        {
            if (labs( m_lastPoint.x - point.x) >= labs(m_lastPoint.y - point.y))
            {
                m_fOffsetX += point.x - m_lastPoint.x;

                if(point.x != m_lastPoint.x)
                {
                    m_bUpOrDown = (point.x - m_lastPoint.x > 0) ? TRUE : FALSE;
                }

                if(point.x - m_lastPoint.x != 0)
                {
                    this->RequestLayout();
                    this->Invalidate();
                }
            }
        }
        else
        {
            this->DoPanning(m_lastPoint, point);
        }

        m_lastPoint = point;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::DoPanning(const POINT &pointLast, const POINT &pointCur)
{
    if(m_bCanPanning)
    {
        if(m_nCurShowIndex >= 0 && m_nCurShowIndex < (INT32)m_vecContent.size())
        {
            SdkViewElement *pView = m_vecContent[m_nCurShowIndex];
            LAYOUTINFO layout = {0};
            pView->GetLayoutInfo(&layout);

            //
            FLOAT nNewX = layout.x;
            nNewX += pointCur.x - pointLast.x;
            if(nNewX > 0 )
            {
                nNewX = 0;
            }
            else if(nNewX < this->GetWidth() - layout.width)
            {
                nNewX = this->GetWidth() - layout.width;
            }

            //
            FLOAT nNewY = layout.y;
            nNewY += pointCur.y - pointLast.y;
            if(nNewY > 0 )
            {
                nNewY = 0;
            }
            else if(nNewY < this->GetHeight() - layout.height)
            {
                nNewY = this->GetHeight() - layout.height;
            }

            if(nNewX != layout.x || nNewY != layout.y)
            {
                layout.x = nNewX;
                layout.y = nNewY;
                pView->SetLayoutInfo(layout.x, layout.y, layout.width, layout.height);
                pView->Invalidate();
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::OnMouseLeave( WPARAM wParam, LPARAM lParam )
{
    this->OnLButtonUp(wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
    if(!m_bCanZoom || m_bMove)
    {
        return;
    }
    POINT point = {0};
    point.x = GET_X_LPARAM(lParam);
    point.y = GET_Y_LPARAM(lParam);

    if(m_nCurShowIndex >= 0 && m_nCurShowIndex < (INT32)m_vecContent.size())
    {
        if(this->IsPtInRect((FLOAT)point.x, (FLOAT)point.y))
        {
            INT32 zDistance = GET_WHEEL_DELTA_WPARAM(wParam);
            if(zDistance > 0)
            {
                this->ZoomOut(1 + (zDistance / MOUSE_WHEEL_ZDISTANCE) * ZOOM_RATE, &point);
            }
            else
            {
                this->ZoomIn(1 + (zDistance / MOUSE_WHEEL_ZDISTANCE) * ZOOM_RATE, &point);
            }
        }
    }
    
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkViewPreviewEx::CalcOffset()
{
    FLOAT fTempOffect = m_fOffsetX;
    FLOAT fWidth = m_fElementWidth;

    if(fWidth > 0)
    {
        FLOAT fOffsetX = fabs(fTempOffect);
        INT32 nDiv = (INT32)(fOffsetX / fWidth);

        if(fOffsetX != fWidth * nDiv)
        {
            BOOL bBig =
                ((fOffsetX - fWidth * nDiv) > fWidth / 2.0f) ? TRUE : FALSE;

            if(fTempOffect < 0)
            {
                if(m_bUpOrDown)
                {
                    if(bBig)
                    {
                        fTempOffect -= fWidth * (nDiv + 1) - fOffsetX;
                    }
                    else
                    {
                        fTempOffect += fabs(fOffsetX - fWidth * nDiv);
                    }
                }
                else
                {
                    if(bBig)
                    {
                        fTempOffect -= fWidth * (nDiv + 1) - fOffsetX;;
                    }
                    else
                    {
                        fTempOffect += fabs(fOffsetX - fWidth * nDiv);
                    }
                }
            }
            else
            {
                if(m_bUpOrDown)
                {
                    if(bBig)
                    {
                        fTempOffect += fWidth * (nDiv + 1) - fOffsetX;;
                    }
                    else
                    {
                        fTempOffect -= fabs(fOffsetX - fWidth * nDiv);
                    }
                }
                else
                {
                    if(bBig)
                    {
                        fTempOffect += fabs(fOffsetX - fWidth * nDiv);

                    }
                    else
                    {
                        fTempOffect -= fabs(fOffsetX - fWidth * nDiv);
                    }
                }
            }
        }
    }

    return fTempOffect;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::CalcOffset(FLOAT &fStartOffset, FLOAT &fEndOffset)
{
    fStartOffset = m_fOffsetX;
    fEndOffset = m_fOffsetX;
    FLOAT fTempOffect = m_fOffsetX;
    FLOAT fWidth = m_fElementWidth;

    if (fEndOffset > 0)
    {
        fEndOffset = 0;
        return;
    }
    else if(fEndOffset < fWidth - m_fRealWidth)
    {
        fEndOffset = fWidth - m_fRealWidth;
        return;
    }

    if(fWidth > 0)
    {
        FLOAT fOffsetX = fabs(fTempOffect);
        INT32 nDiv = (INT32)(fOffsetX / fWidth);

        if(fOffsetX != fWidth * nDiv)
        {
            if(fTempOffect < 0)
            {
                if(m_bUpOrDown)
                {

                    fEndOffset = fTempOffect - fWidth * (nDiv) + fOffsetX;
                }
                else
                {
                    fEndOffset = fTempOffect - fWidth * (nDiv + 1) + fOffsetX;
                }
            }
            else
            {
                if(m_bUpOrDown)
                {
                    fEndOffset = fTempOffect + fWidth * (nDiv + 1) - fOffsetX;
                }
                else
                {
                    fEndOffset = fTempOffect + fabs(fOffsetX - fWidth * nDiv);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::SetFitChildToWindow(BOOL bFit)
{
    m_bFitChildToWindow = bFit;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::SetStartChild(INT32 nChildIndex)
{
    m_nCurShowIndex = nChildIndex;
    INT32 nSize = (INT32)m_vctChildren.size();
    if(nChildIndex >= 0 || nChildIndex < nSize)
    {
        m_fOffsetX = -nChildIndex * this->GetWidth();
        this->PerformSwitch();
        this->Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::ScrollUp(BOOL bNeedInvalidate)
{
    switch(m_animationType)
    {
    case ANIMATION_UNIFORM:
        if (m_isAnimationEnd)
        {
            if(m_fOffsetX == m_fElementWidth - m_fRealWidth)
            {
                return;
            }
            m_fBackOffsetX = m_fOffsetX;
            this->StartAnimation(TRUE, TRUE);
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::ScrollDown(BOOL bNeedInvalidate)
{
    switch(m_animationType)
    {
    case ANIMATION_UNIFORM:
        if (m_isAnimationEnd)
        {
            if (m_fOffsetX == 0)
            {
                return;
            }
            m_fBackOffsetX = m_fOffsetX;
            this->StartAnimation(TRUE, FALSE);
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::CorrectOffset(BOOL bNeedInvalidate)
{
    FLOAT fWidth = this->GetWidth();
    if (m_fOffsetX > 0)
    {
        m_fOffsetX = 0;
    }
    else if(m_fOffsetX < fWidth - m_fRealWidth)
    {
        m_fOffsetX = fWidth - m_fRealWidth;
    }

	if(m_bZoom)
    {
        if(m_nCurShowIndex >= 0 && m_nCurShowIndex < (INT32)m_vecContent.size())
        {
            m_vecContent[m_nCurShowIndex]->SetLayoutInfo(
                m_curChildLayoutInfo.x,
                m_curChildLayoutInfo.y,
                m_curChildLayoutInfo.width,
                m_curChildLayoutInfo.height);
        }
        m_fZoomRate = 1;
        m_bZoom = FALSE;
        m_bCanPanning = FALSE;
    }
    
    this->RequestLayout();
    if(bNeedInvalidate)
    {
        m_nCurShowIndex = (INT32)fabs(m_fOffsetX / m_fElementWidth);
        PerformSwitch();
        this->Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::PerformSwitch()
{
    if (NULL != m_pSwitchEventHandler)
    {
        m_pSwitchEventHandler->OnSwitchEvent(this);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::SetSwitchEventHandler(IN ISwitchEventHandler* pEventHandler)
{
    m_pSwitchEventHandler = pEventHandler;
}

//////////////////////////////////////////////////////////////////////////

INT32 SdkViewPreviewEx::GetCurrentIndex()
{
    return m_nCurShowIndex;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::StartAnimation(IN BOOL isStart, IN BOOL bLeft)
{
    if (isStart)
    {
        m_isAnimationEnd = FALSE;
        m_bCanZoom = FALSE;
        m_bMove = TRUE;
        if(bLeft)
        {
            m_pLeftTranslateAnimation->Start();
        }
        else
        {
            m_pRightTranslateAnimation->Start();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::OnAnimationStart( SdkAnimation* pAnimation)
{
    UNREFERENCED_PARAMETER(pAnimation);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::OnAnimationPlaying(SdkAnimation* pAnimation)
{
    UNREFERENCED_PARAMETER(pAnimation);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::OnAnimationEnd(SdkAnimation* pAnimation)
{
    UNREFERENCED_PARAMETER(pAnimation);

    m_fOffsetX = CalcOffset();
    this->UpdateCurShowIndex();
    this->PerformSwitch();
    m_bCanZoom = TRUE;
    m_bMove = FALSE;
    m_isAnimationEnd = TRUE;
}

//////////////////////////////////////////////////////////////////////////

VOID SdkViewPreviewEx::OnAnimationTimerUpdate(SdkAnimation *pAnimation)
{
    if (NULL != pAnimation)
    {
        DOUBLE dDoubleX = 0;

        if ((m_pLeftTranslateAnimation == pAnimation) || (m_pRightTranslateAnimation == pAnimation))
        {
            (dynamic_cast<SdkTranslateAnimation*>(pAnimation))->GetValue(&dDoubleX, NULL);
            m_fOffsetX = m_fBackOffsetX + (FLOAT)dDoubleX;
        }
        else if (m_pDynamicTranslate == pAnimation)
        {
            m_pDynamicTranslate->GetValue(&dDoubleX, NULL);
            m_fOffsetX = (FLOAT)dDoubleX;
        }

        if (m_isAnimationEnd)
        {
            m_fOffsetX = this->CalcOffset();
            this->UpdateCurShowIndex();
            this->PerformSwitch();
        }

        this->RequestLayout();
        this->ForceInvalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::SetAnimationType(IN ANI_TYPE type)
{
    m_animationType = type;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::SetAnimationTime(IN DOUBLE nDSecond)
{
    m_dAnimationTime = nDSecond;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewPreviewEx::IsAnimationEnd()
{
    return m_isAnimationEnd;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::ResizeFrame(FLOAT fParentWidth, FLOAT fParentHeight)
{
    INT32 nCount = (INT32)m_vecFrame.size();
    if(m_bFitChildToWindow)
    {
        m_fElementWidth = fParentWidth;
        m_fElementHeight = fParentHeight;
    }

    m_fRealWidth = nCount * m_fElementWidth;

    LAYOUTINFO layoutInfo = {0};
    layoutInfo.x = m_fOffsetX;
    layoutInfo.y = 0;
    layoutInfo.width = m_fElementWidth;
    layoutInfo.height = m_fElementHeight;

    LAYOUTINFO layoutInfoContent = {0};
    layoutInfoContent.width = m_fElementWidth;
    layoutInfoContent.height = m_fElementHeight;

    for(INT32 nIndex = 0; nIndex < nCount; ++nIndex)
    {
        if(NULL != m_vecFrame[nIndex] && NULL != m_vecContent[nIndex])
        {
            m_vecFrame[nIndex]->SetLayoutInfo(
                layoutInfo.x,
                layoutInfo.y,
                layoutInfo.width,
                layoutInfo.height);
            if(m_bFitChildToWindow)
            {
                m_vecContent[nIndex]->GetLayoutInfo(&layoutInfoContent);
                layoutInfoContent.x = (fParentWidth - layoutInfoContent.width) / 2;
                layoutInfoContent.y = (fParentHeight - layoutInfoContent.height) / 2;
                
                if(0 != fParentHeight
                    && 0 != layoutInfoContent.height
                    && 0 != fParentWidth
                    && 0 != layoutInfoContent.width)
                {
                    if(layoutInfoContent.width > fParentWidth || layoutInfoContent.height > fParentHeight)
                    {
                        BOOL bBaseHeight = FALSE;
                        if(layoutInfoContent.width > fParentWidth)
                        {
                            if(layoutInfoContent.height > fParentHeight)
                            {
                                if(fParentHeight / fParentWidth <
                                    layoutInfoContent.height / layoutInfoContent.width)
                                {
                                    bBaseHeight = TRUE;
                                }
                            }
                        }
                        else
                        {
                            bBaseHeight = TRUE;
                        }

                        if(bBaseHeight)
                        {
                            layoutInfoContent.width = fParentHeight * layoutInfoContent.width / layoutInfoContent.height;
                            layoutInfoContent.height = fParentHeight;
                            layoutInfoContent.x = (fParentWidth - layoutInfoContent.width) / 2;
                            layoutInfoContent.y = 0;
                        }
                        else
                        {
                            layoutInfoContent.height = fParentWidth * layoutInfoContent.height / layoutInfoContent.width;
                            layoutInfoContent.width = fParentWidth;
                            layoutInfoContent.x = 0;
                            layoutInfoContent.y = (fParentHeight - layoutInfoContent.height) / 2;
                        }
                    }
                }
            }

            m_vecContent[nIndex]->SetLayoutInfo(
                layoutInfoContent.x,
                layoutInfoContent.y,
                layoutInfoContent.width,
                layoutInfoContent.height);
            if(nIndex == m_nCurShowIndex && !m_bZoom)
            {
                m_vecContent[nIndex]->GetLayoutInfo(&m_curChildLayoutInfo);
            }

            layoutInfo.x += layoutInfo.width;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::ZoomIn(FLOAT fRate, POINT *pPointCenter)
{
    if (m_fZoomRate > MIN_ZOOM && fRate > 0.0f)
    {
        FLOAT fZoomRate = m_fZoomRate;
        m_fZoomRate *= fRate;
        if(m_fZoomRate < MIN_ZOOM)
        {
            m_fZoomRate = MIN_ZOOM;
            if(m_bCanPanning)
            {
                SetGesture(FALSE);
            }
            m_bCanPanning = FALSE;
        }
        Zoom(m_fZoomRate / fZoomRate, pPointCenter);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::ZoomOut(FLOAT fRate, POINT *pPointCenter)
{
    if(m_fZoomRate < MAX_ZOOM && fRate > 0.0f)
    {
        FLOAT fZoomRate = m_fZoomRate;
        m_fZoomRate *= fRate;
        if(m_fZoomRate > MAX_ZOOM)
        {
            m_fZoomRate = MAX_ZOOM;
        }
        Zoom(m_fZoomRate / fZoomRate, pPointCenter);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::Zoom(FLOAT fRate, POINT *pPointCenter)
{
    INT32 nCount = (INT32)m_vecContent.size();
    if(m_nCurShowIndex >= 0 && m_nCurShowIndex < nCount)
    {
        SdkViewElement *pView = m_vecContent[m_nCurShowIndex];
        if(NULL != pView)
        {
            LAYOUTINFO layoutInfo = {0};
            pView->GetLayoutInfo(&layoutInfo);

            FLOAT fTempWidth = layoutInfo.width;
            FLOAT fTempHeight = layoutInfo.height;

            BOOL bCanPanning = m_bCanPanning;
            layoutInfo.width = m_curChildLayoutInfo.width * m_fZoomRate;
            layoutInfo.height = m_curChildLayoutInfo.height * m_fZoomRate;

            if(layoutInfo.width != m_curChildLayoutInfo.width
                || layoutInfo.height != m_curChildLayoutInfo.height)
            {
                m_bZoom = TRUE;
                if(layoutInfo.width > this->GetWidth() || layoutInfo.height > this->GetHeight())
                {
                    if(!m_bCanPanning)
                    {
                        SetGesture(TRUE);
                    }
                    m_bCanPanning = TRUE;
                }
                else
                {
                    if(m_bCanPanning)
                    {
                        SetGesture(FALSE);
                    }
                    m_bCanPanning = FALSE;
                }
            }
            else
            {
                m_bZoom = FALSE;
                if(m_bCanPanning)
                {
                    SetGesture(FALSE);
                }
                m_bCanPanning = FALSE;
            }

            if(NULL != pPointCenter)
            {
                if(bCanPanning && m_bCanPanning)
                {
                    layoutInfo.x = pPointCenter->x - (pPointCenter->x - layoutInfo.x) * fRate;
                    layoutInfo.y = pPointCenter->y - (pPointCenter->y - layoutInfo.y) * fRate;
                }
                else
                {
                    layoutInfo.x += (fTempWidth - layoutInfo.width) / 2.0f;
                    layoutInfo.y += (fTempHeight - layoutInfo.height) / 2.0f;
                }
            }
            else
            {
                layoutInfo.x += (fTempWidth - layoutInfo.width) / 2.0f;
                layoutInfo.y += (fTempHeight - layoutInfo.height) / 2.0f;
            }

            // Correct X
            if(layoutInfo.x > m_curChildLayoutInfo.x)
            {
                layoutInfo.x = m_curChildLayoutInfo.x;
            }
            else if(layoutInfo.x + layoutInfo.width < m_curChildLayoutInfo.x + m_curChildLayoutInfo.width)
            {
                layoutInfo.x = m_curChildLayoutInfo.x + m_curChildLayoutInfo.width - layoutInfo.width;
            }

            // Correct Y
            if(layoutInfo.y > m_curChildLayoutInfo.y)
            {
                layoutInfo.y = m_curChildLayoutInfo.y;
            }
            else if(layoutInfo.y + layoutInfo.height < m_curChildLayoutInfo.y + m_curChildLayoutInfo.height)
            {
                layoutInfo.y = m_curChildLayoutInfo.y + m_curChildLayoutInfo.height - layoutInfo.height;
            }

            pView->SetLayoutInfo(
                layoutInfo.x,
                layoutInfo.y,
                layoutInfo.width,
                layoutInfo.height);
            pView->Invalidate();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::ZoomTo(FLOAT fRate , POINT *pPointCenter)
{
    if(fRate < MIN_ZOOM)
    {
        fRate = MIN_ZOOM;
    }
    
    if(fRate / m_fZoomRate > 1.0f)
    {
        ZoomOut(fRate / m_fZoomRate, pPointCenter);
    }
    else
    {
        ZoomIn(fRate / m_fZoomRate, pPointCenter);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::CancelZoom()
{
    if(m_bZoom)
    {
        m_bZoom = FALSE;
        if(m_bCanPanning)
        {
            SetGesture(FALSE);
        }
        m_bCanPanning = FALSE;

        INT32 nCount = (INT32)m_vecContent.size();
        if(m_nCurShowIndex >= 0 && m_nCurShowIndex < nCount)
        {
            SdkViewElement *pView = m_vecContent[m_nCurShowIndex];
            if(NULL != pView)
            {
                pView->SetLayoutInfo(
                    m_curChildLayoutInfo.x,
                    m_curChildLayoutInfo.y,
                    m_curChildLayoutInfo.width,
                    m_curChildLayoutInfo.height);
            }
        }

        m_fZoomRate = MIN_ZOOM;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::UpdateCurShowIndex()
{
    INT32 nNewIndex = (INT32)fabs(m_fOffsetX / m_fElementWidth);;
    if(/*m_bZoom && */m_nCurShowIndex != nNewIndex)
    {
        if(m_nCurShowIndex >= 0 && m_nCurShowIndex < (INT32)m_vecContent.size())
        {
            m_vecContent[m_nCurShowIndex]->SetLayoutInfo(
                m_curChildLayoutInfo.x,
                m_curChildLayoutInfo.y,
                m_curChildLayoutInfo.width,
                m_curChildLayoutInfo.height);
        }
        m_fZoomRate = 1;
        m_bZoom = FALSE;
        if(m_bCanPanning)
        {
            SetGesture(FALSE);
        }
        m_bCanPanning = FALSE;
    }
    m_nCurShowIndex = nNewIndex;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::SetGesture(BOOL bInteria)
{
    SdkWindow *pWin = this->GetWindow();
    if(NULL != pWin)
    {
        // Config Gesture
        DWORD dwPanWant = GC_PAN | GC_PAN_WITH_SINGLE_FINGER_VERTICALLY | GC_PAN_WITH_SINGLE_FINGER_HORIZONTALLY;
        DWORD dwPanBlock = GC_PAN_WITH_GUTTER;

        if(bInteria)
        {
            dwPanWant |= GC_PAN_WITH_INERTIA;
        }
        else
        {
            dwPanBlock |= GC_PAN_WITH_INERTIA;
        }

        GESTURECONFIG gc[] = {{ GID_ZOOM, GC_ZOOM, 0 },
        { GID_ROTATE, GC_ROTATE, 0},
        { GID_PAN, dwPanWant , dwPanBlock}};
        ::SetGestureConfig(pWin->GetHwnd(), 0, 3, gc, sizeof(GESTURECONFIG));
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::StopAnimation()
{
    this->StopAnimation(m_pLeftTranslateAnimation);
    this->StopAnimation(m_pDynamicTranslate);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewPreviewEx::StopAnimation(SdkAnimation *pAnimation)
{
    if(NULL != pAnimation)
    {
        pAnimation->Stop();
    }
}
