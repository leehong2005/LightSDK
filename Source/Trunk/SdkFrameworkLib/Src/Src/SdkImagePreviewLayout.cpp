/*!
* @file SdkImagePreviewLayout.cpp
* 
* @brief This file defines the view preview layout.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Liu Qiao
* @date 2011/03/15
*/

//#include "stdafx.h"
#include "Resource.h"
#include "SdkImagePreviewLayout.h"
#include <Windows.h>

USING_NAMESPACE_VIEWS

#define ZOOM_RATE               0.2f
#define MIN_ZOOM                1
#define MAX_ZOOM                20

#define MOUSE_WHEEL_ZDISTANCE   120

#define DEFALUT_PICTURE_WIDTH             (185)
#define DEFALUT_PICTURE_HEIGHT            (104)

//////////////////////////////////////////////////////////////////////////

SdkImagePreviewLayout::SdkImagePreviewLayout() :
    m_pLastViewLayout(NULL),
    m_pCurViewLayout(NULL),
    m_pNextViewLayout(NULL),
    m_pLastImageView(NULL),
    m_pCurImageView(NULL),
    m_pNextImageView(NULL),
    m_pCurrentShowView(NULL),
    m_pLeftTranslateAnimation(NULL),
    m_pRightTranslateAnimation(NULL),
    m_pDynamicTranslate(NULL),
    m_pUpdateHandler(NULL),
    m_nCurIndex(1),
    m_nNewCurIndex(1),
    m_nMoveIndex(0),
    m_fOffsetX(0),
    m_fZoomRate(1),
    m_dAnimationTime(0),
    m_dwZoomArguments(0),
    m_fDisplayWidth(0),
    m_fDisplayHeight(0),
    m_isAnimationEnd(TRUE),
    m_bContinue(FALSE),
    m_bCanPanning(FALSE),
    m_bLbuttonDown(FALSE),
    m_bUpOrDown(FALSE),
    m_bMove(FALSE),
    m_bCanZoom(TRUE),
    m_bZoom(FALSE),
    m_isAutoplay(FALSE),
    m_isKeyLastOrUp(FALSE),
    m_isBottomIn(FALSE)
{
    ZeroMemory(m_tcCurFilePath, MAX_PATH * 2);
    ZeroMemory(m_tcLastFilePath, MAX_PATH * 2);
    ZeroMemory(m_tcNextFilePath, MAX_PATH * 2);

    m_pLeftTranslateAnimation = new SdkTranslateAnimation();
    m_pLeftTranslateAnimation->AddAnimationListener(this);
    m_pLeftTranslateAnimation->AddAnimationTimerListener(this);

    m_pRightTranslateAnimation = new SdkTranslateAnimation();
    m_pRightTranslateAnimation->AddAnimationListener(this);
    m_pRightTranslateAnimation->AddAnimationTimerListener(this);

    m_pDynamicTranslate = new SdkTranslateAnimation();
    m_pDynamicTranslate->AddAnimationTimerListener(this);
    m_pDynamicTranslate->AddAnimationListener(this);

    Initialize();
}

//////////////////////////////////////////////////////////////////////////

SdkImagePreviewLayout::~SdkImagePreviewLayout()
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

void SdkImagePreviewLayout::SetAnimationTime( IN DOUBLE nDSecond )
{
    m_dAnimationTime = nDSecond;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::GetCurFilePath(LPWSTR lpFilePath, INT32 nBufferLength)
{
    if(NULL != lpFilePath && nBufferLength > 0)
    {
        ZeroMemory(lpFilePath, nBufferLength * 2);
        switch (m_nCurIndex)
        {
        case 0:
            swprintf_s(lpFilePath, nBufferLength, L"%s", m_tcLastFilePath);
            break;
        case 1:
            swprintf_s(lpFilePath, nBufferLength, L"%s", m_tcCurFilePath);
            break;
        case 2:
            swprintf_s(lpFilePath, nBufferLength, L"%s", m_tcNextFilePath);
            break;
        default:
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::SetLastImage( IN LPCWSTR lpFile, INT32 nWidth, INT32 nHeight )
{
    SdkGifView *pImageView = NULL;
    switch (m_nCurIndex)
    {
    case 0:
        pImageView = m_pNextImageView;
        ZeroMemory(m_tcNextFilePath, MAX_PATH * 2);
        if(NULL != lpFile)
        {
            swprintf_s(m_tcNextFilePath, MAX_PATH, L"%s", lpFile);
        }
        break;
    case 1:
        pImageView = m_pLastImageView;
        ZeroMemory(m_tcLastFilePath, MAX_PATH * 2);
        if(NULL != lpFile)
        {
            swprintf_s(m_tcLastFilePath, MAX_PATH, L"%s", lpFile);
        }
        break;
    case 2:
        pImageView = m_pCurImageView;
        ZeroMemory(m_tcCurFilePath, MAX_PATH * 2);
        if(NULL != lpFile)
        {
            swprintf_s(m_tcCurFilePath, MAX_PATH, L"%s", lpFile);
        }
        break;
    default:
        break;
    }
    SetImageInfo(pImageView, lpFile, (FLOAT)nWidth, (FLOAT)nHeight);
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::SetCurImage( IN LPCWSTR lpFile, INT32 nWidth, INT32 nHeight )
{
    SdkGifView *pImageView = NULL;
    switch (m_nCurIndex)
    {
    case 0:
        pImageView = m_pLastImageView;
        ZeroMemory(m_tcLastFilePath, MAX_PATH * 2);
        if(NULL != lpFile)
        {
            swprintf_s(m_tcLastFilePath, MAX_PATH, L"%s", lpFile);
        }
        break;
    case 1:
        pImageView = m_pCurImageView;
        ZeroMemory(m_tcCurFilePath, MAX_PATH * 2);
        if(NULL != lpFile)
        {
            swprintf_s(m_tcCurFilePath, MAX_PATH, L"%s", lpFile);
        }
        break;
    case 2:
        pImageView = m_pNextImageView;
        ZeroMemory(m_tcNextFilePath, MAX_PATH * 2);
        if(NULL != lpFile)
        {
            swprintf_s(m_tcNextFilePath, MAX_PATH, L"%s", lpFile);
        }
        break;
    default:
        break;
    }
    SetImageInfo(pImageView, lpFile, (FLOAT)nWidth, (FLOAT)nHeight);
    if(NULL != m_pUpdateHandler)
    {
        m_pUpdateHandler->OnImageSwitchEvent(this);
        DeleteImageView();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::SetNextImage( IN LPCWSTR lpFile, INT32 nWidth, INT32 nHeight )
{
    SdkGifView *pImageView = NULL;
    switch (m_nCurIndex)
    {
    case 0:
        pImageView = m_pCurImageView;
        ZeroMemory(m_tcCurFilePath, MAX_PATH * 2);
        if(NULL != lpFile)
        {
            swprintf_s(m_tcCurFilePath, MAX_PATH, L"%s", lpFile);
        }
        break;
    case 1:
        pImageView = m_pNextImageView;
        ZeroMemory(m_tcNextFilePath, MAX_PATH * 2);
        if(NULL != lpFile)
        {
            swprintf_s(m_tcNextFilePath, MAX_PATH, L"%s", lpFile);
        }
        break;
    case 2:
        pImageView = m_pLastImageView;
        ZeroMemory(m_tcLastFilePath, MAX_PATH * 2);
        if(NULL != lpFile)
        {
            swprintf_s(m_tcLastFilePath, MAX_PATH, L"%s", lpFile);
        }
        break;
    default:
        break;
    }
    SetImageInfo(pImageView, lpFile, (FLOAT)nWidth, (FLOAT)nHeight);
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::Initialize()
{
    m_pLastViewLayout = new SdkViewLayout();
    this->AddView(m_pLastViewLayout);
    m_pLastImageView = new SdkGifView();
    m_pLastImageView->SetStretchMode(IMAGE_STRETCH_MODE_FILL);
    m_pLastImageView->AutoAdjustSize(FALSE);
    m_pLastViewLayout->AddView(m_pLastImageView);

    m_pCurViewLayout = new SdkViewLayout();
    this->AddView(m_pCurViewLayout);
    m_pCurImageView = new SdkGifView();
    m_pCurImageView->SetStretchMode(IMAGE_STRETCH_MODE_FILL);
    m_pCurImageView->AutoAdjustSize(FALSE);
    m_pCurViewLayout->AddView(m_pCurImageView);

    m_pNextViewLayout = new SdkViewLayout();
    this->AddView(m_pNextViewLayout);
    m_pNextImageView = new SdkGifView();
    m_pNextImageView->SetStretchMode(IMAGE_STRETCH_MODE_FILL);
    m_pNextImageView->AutoAdjustSize(FALSE);
    m_pNextViewLayout->AddView(m_pNextImageView);
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::OnLayout( BOOL bChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height )
{
    if(bChanged)
    {
        if(NULL != m_pUpdateHandler)
        {
            TCHAR tcPath[MAX_PATH] = {0};
            GetCurFilePath(tcPath, MAX_PATH);
            INT32 nImageWidth = 0, nImageHeight = 0;
            m_pUpdateHandler->RequestImageSize(tcPath, nImageWidth, nImageHeight);
            if(0 == nImageWidth || 0 == nImageHeight)
            {
                nImageWidth = DEFALUT_PICTURE_WIDTH;
                nImageHeight = DEFALUT_PICTURE_HEIGHT;
            }
            m_curChildLayoutInfo.height = (FLOAT)nImageHeight;
            m_curChildLayoutInfo.width = (FLOAT)nImageWidth;
            this->CalcLayoutInfo(m_curChildLayoutInfo, width, height);
        }

        if(m_bZoom)
        {
            this->CancelZoom();
        }
        else
        {
            if(NULL != m_pCurrentShowView)
            {
                //m_pCurrentShowView->SetLayoutInfo(&m_curChildLayoutInfo);
            }
        }
        m_pLeftTranslateAnimation->SetTranslateXInfo(0, -width, m_dAnimationTime);
        m_pRightTranslateAnimation->SetTranslateXInfo(0, width, m_dAnimationTime);
    }

    switch (m_nCurIndex)
    {
    case 0 :
        LayoutView(m_pNextViewLayout, m_pNextImageView, m_fOffsetX - width , 0, width, height);
        LayoutView(m_pLastViewLayout, m_pLastImageView, m_fOffsetX , 0, width, height);
        LayoutView(m_pCurViewLayout, m_pCurImageView, m_fOffsetX + width, 0, width, height);
        if(!m_bZoom)
        {
            m_pCurrentShowView = m_pLastImageView;
            m_pLastImageView->GetLayoutInfo(&m_curChildLayoutInfo);
        }
        break;
    case 1 :
        LayoutView(m_pLastViewLayout, m_pLastImageView, m_fOffsetX - width , 0, width, height);
        LayoutView(m_pCurViewLayout, m_pCurImageView, m_fOffsetX , 0, width, height);
        LayoutView(m_pNextViewLayout, m_pNextImageView, m_fOffsetX + width, 0, width, height);
        if(!m_bZoom)
        {
            m_pCurrentShowView = m_pCurImageView;
            m_pCurImageView->GetLayoutInfo(&m_curChildLayoutInfo);
        }
        break;
    case 2 :
        LayoutView(m_pCurViewLayout, m_pCurImageView, m_fOffsetX - width , 0, width, height);
        LayoutView(m_pNextViewLayout, m_pNextImageView, m_fOffsetX , 0, width, height);
        LayoutView(m_pLastViewLayout, m_pLastImageView, m_fOffsetX + width, 0, width, height);
        if(!m_bZoom)
        {
            m_pCurrentShowView = m_pNextImageView;
            m_pNextImageView->GetLayoutInfo(&m_curChildLayoutInfo);
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::ScrollUp(BOOL isAutoPlay)
{
    m_isAutoplay = isAutoPlay;
    m_isKeyLastOrUp = isAutoPlay;
    if(m_isAnimationEnd && NULL != m_pUpdateHandler)
    {
        m_nNewCurIndex = GetNewShowIndex(TRUE);

        m_isAnimationEnd = FALSE;
        m_bCanZoom = FALSE;
        DOUBLE dSurplusTime = 0;
        m_pUpdateHandler->GetSurplusTime(dSurplusTime);
        m_pLeftTranslateAnimation->Start(dSurplusTime);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::ScrollDown(BOOL isAutoPlay)
{
    m_isAutoplay = isAutoPlay;
    m_isKeyLastOrUp = isAutoPlay;
    if(m_isAnimationEnd && NULL != m_pUpdateHandler)
    {
        m_nNewCurIndex = GetNewShowIndex(FALSE);
        m_isAnimationEnd = FALSE;
        m_bCanZoom = FALSE;
        DOUBLE dSurplusTime = 0;
        m_pUpdateHandler->GetSurplusTime(dSurplusTime);
        m_pRightTranslateAnimation->Start(dSurplusTime);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagePreviewLayout::OnMouseEvent( UINT message, WPARAM wParam, LPARAM lParam )
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
    /*case WM_MOUSELEAVE:
        this->OnLButtonUp(wParam, lParam);
        break;*/
    case WM_MOUSEWHEEL:
        this->OnMouseWheel(wParam, lParam);
        break;
    default:
        break;
    }

    return bHandle;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::OnLButtonDown( WPARAM wParam, LPARAM lParam )
{
    m_bLbuttonDown = TRUE;
    m_lastPoint.x = GET_X_LPARAM(lParam);
    m_lastPoint.y = GET_Y_LPARAM(lParam);
    m_downPoint = m_lastPoint;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
    if(m_bLbuttonDown)
    {
        m_bLbuttonDown = FALSE;

        POINT point = { 0 };
        point.x = GET_X_LPARAM(lParam);
        point.y = GET_Y_LPARAM(lParam);

        if (/*0 != m_nMoveIndex &&*/ point.x == m_downPoint.x && point.y == m_downPoint.y)
        {
            m_isAutoplay = FALSE;
            m_nMoveIndex = 0;
            m_isBottomIn = TRUE;
        }

        if(NULL == m_pUpdateHandler || !m_isAnimationEnd)
        {
            if (point.x - m_downPoint.x > 0)
            {
                if (m_nMoveIndex < 0)
                {
                    m_nMoveIndex = 0;
                }
                m_nMoveIndex++;
            }
            else if (point.x - m_downPoint.x < 0)
            {
                if (m_nMoveIndex > 0)
                {
                    m_nMoveIndex = 0;
                }
                m_nMoveIndex--;
            }
            m_bContinue = FALSE;
            if (!m_isKeyLastOrUp)
            {
                m_isKeyLastOrUp = TRUE;
                m_nMoveIndex = 0;
            }
            return;
        }

        if(!m_bCanPanning && 0 != m_fOffsetX && 0 == m_nMoveIndex)
        {
            m_isBottomIn = FALSE;
            FLOAT fTempStartX = m_fOffsetX;
            FLOAT fTempEndX = 0.0f;
            if(m_bUpOrDown)
            {
                if(m_bContinue && m_fOffsetX > 0)
                {
                    fTempEndX = this->GetWidth();
                    m_nNewCurIndex = GetNewShowIndex(FALSE);
                }
            }
            else if(m_bContinue && m_fOffsetX < 0)
            {
                fTempEndX -= this->GetWidth();
                m_nNewCurIndex = GetNewShowIndex(TRUE);
            }
            if ((fTempStartX != fTempEndX))
            {
                m_isAnimationEnd = FALSE;
                m_bMove = TRUE;
                m_pDynamicTranslate->SetTranslateXInfo(fTempStartX, fTempEndX, m_dAnimationTime);
                DOUBLE dSurplusTime = 0;
                m_pUpdateHandler->GetSurplusTime(dSurplusTime);
                m_bCanZoom = TRUE;
                m_pDynamicTranslate->Start(dSurplusTime);
            }
        }
        else
        {
            m_bMove = FALSE;
        }
    }

    m_bContinue = FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::OnMouseMove( WPARAM wParam, LPARAM lParam )
{
    if(m_bLbuttonDown && m_isAnimationEnd && NULL != m_pUpdateHandler)
    {
        m_bMove = TRUE;
        m_isAutoplay = FALSE;

        POINT point = { 0 };
        point.x = GET_X_LPARAM(lParam);
        point.y = GET_Y_LPARAM(lParam);
        if(!m_bCanPanning)
        {
            if (labs( m_lastPoint.x - point.x) >= labs(m_lastPoint.y - point.y) )
            {
                if(point.x != m_lastPoint.x)
                {
                    FLOAT fBackOffset = m_fOffsetX;
                    m_fOffsetX += point.x - m_lastPoint.x;
                    m_bUpOrDown = (point.x - m_lastPoint.x > 0) ? TRUE : FALSE;

                    if(((0 == fBackOffset) && m_bUpOrDown)
                        || (fBackOffset < 0 && m_fOffsetX > 0))
                    {
                        m_bContinue = m_pUpdateHandler->UpdateImageView(this, FALSE);
                        if (!m_bContinue)
                        {
                            m_nNewCurIndex = m_nCurIndex;
                            m_nMoveIndex = 0;
                        }
                    }
                    else if(((0 == fBackOffset) && !m_bUpOrDown)
                        || (fBackOffset > 0 && m_fOffsetX < 0))
                    {
                        m_bContinue = m_pUpdateHandler->UpdateImageView(this, TRUE);
                        if (!m_bContinue)
                        {
                            m_nNewCurIndex = m_nCurIndex;
                            m_nMoveIndex = 0;
                        }
                    }
                    /*if(0 == fBackOffset)
                    {
                        if(m_bUpOrDown)
                        {
                            m_bContinue = m_pUpdateHandler->UpdateImageView(this, FALSE);
                        }
                        else
                        {
                            m_bContinue = m_pUpdateHandler->UpdateImageView(this, TRUE);
                        }
                    }
                    else
                    {
                        if(fBackOffset > 0 && m_fOffsetX < 0)
                        {
                            m_bContinue = m_pUpdateHandler->UpdateImageView(this, TRUE);
                        }
                        else if(fBackOffset < 0 && m_fOffsetX > 0)
                        {
                            m_bContinue = m_pUpdateHandler->UpdateImageView(this, FALSE);
                        }
                    }*/

                    if(point.x - m_lastPoint.x != 0)
                    {
                        this->RequestLayout();
                        this->Invalidate();
                    }
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

void SdkImagePreviewLayout::OnMouseWheel( WPARAM wParam, LPARAM lParam )
{
    if(!m_bCanZoom || m_bMove || this->IsGifZoom())
    {
        return;
    }
    POINT point = {0};
    point.x = GET_X_LPARAM(lParam);
    point.y = GET_Y_LPARAM(lParam);

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

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::OnAnimationStart( SdkAnimation * /*pAnimation*/ )
{
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::OnAnimationPlaying( SdkAnimation * /*pAnimation*/ )
{
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::OnAnimationEnd( SdkAnimation * pAnimation )
{
    if(m_pLeftTranslateAnimation == pAnimation && m_isAutoplay)
    {
        m_pUpdateHandler->OnAutoPlay();
    }
    m_isAnimationEnd = TRUE;
    m_fOffsetX = 0;
    m_bMove = FALSE;

    if(m_nCurIndex != m_nNewCurIndex)
    {
        m_nCurIndex = m_nNewCurIndex;
        //m_bCanZoom = TRUE;

        m_fZoomRate = 1;
        m_bZoom = FALSE;
        if(m_bCanPanning)
        {
            SetGesture(FALSE);
        }
        m_bCanPanning = FALSE;
        if(NULL != m_pUpdateHandler)
        {
            m_pUpdateHandler->OnImageSwitchEvent(this);
            DeleteImageView();
            DOUBLE dSurplus = 0;
            m_pUpdateHandler->GetSurplusTime(dSurplus);
        }
        this->ContinuousDramatic();
    }
}

//////////////////////////////////////////////////////////////////////////

VOID SdkImagePreviewLayout::OnAnimationTimerUpdate( SdkAnimation *pAnimation )
{
    if (NULL != pAnimation)
    {
        DOUBLE dDoubleX = 0;

        if(!m_isAnimationEnd)
        {
            if ((m_pLeftTranslateAnimation == pAnimation) || (m_pRightTranslateAnimation == pAnimation))
            {
                (dynamic_cast<SdkTranslateAnimation*>(pAnimation))->GetValue(&dDoubleX, NULL);
                m_fOffsetX = (FLOAT)dDoubleX;

            }
            else if (m_pDynamicTranslate == pAnimation)
            {
                m_pDynamicTranslate->GetValue(&dDoubleX, NULL);
                m_fOffsetX = (FLOAT)dDoubleX;
            }
        }
        else
        {
            if (!m_bContinue)
            {
                m_nMoveIndex = 0;
            }
        }

        this->RequestLayout();
        this->ForceInvalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::LayoutView(SdkViewLayout *pLayout, SdkGifView *pImageView, FLOAT fX, FLOAT fY, FLOAT fWidth, FLOAT fHeight)
{
    if(NULL != pImageView && NULL != pLayout)
    {
        pLayout->SetLayoutInfo(fX, fY, fWidth, fHeight);

        LAYOUTINFO layoutInfoContent = {0};
        layoutInfoContent.width = fWidth;
        layoutInfoContent.height = fHeight;

        pImageView->GetLayoutInfo(&layoutInfoContent);
        CalcLayoutInfo(layoutInfoContent, fWidth, fHeight);
        pImageView->SetLayoutInfo(layoutInfoContent.x, layoutInfoContent.y, layoutInfoContent.width, layoutInfoContent.height);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::CalcLayoutInfo(IN OUT LAYOUTINFO &layoutInfoContent, FLOAT fWidth, FLOAT fHeight)
{
    layoutInfoContent.x = (fWidth - layoutInfoContent.width) / 2;
    layoutInfoContent.y = (fHeight - layoutInfoContent.height) / 2;

    if(0 != fHeight
        && 0 != layoutInfoContent.height
        && 0 != fWidth
        && 0 != layoutInfoContent.width)
    {
        if(layoutInfoContent.width > fWidth || layoutInfoContent.height > fHeight)
        {
            BOOL bBaseHeight = FALSE;
            if(layoutInfoContent.width > fWidth)
            {
                if(layoutInfoContent.height > fHeight)
                {
                    if(fHeight / fWidth <
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
                layoutInfoContent.width = fHeight * layoutInfoContent.width / layoutInfoContent.height;
                layoutInfoContent.height = fHeight;
                layoutInfoContent.x = (fWidth - layoutInfoContent.width) / 2;
                layoutInfoContent.y = 0;
            }
            else
            {
                layoutInfoContent.height = fWidth * layoutInfoContent.height / layoutInfoContent.width;
                layoutInfoContent.width = fWidth;
                layoutInfoContent.x = 0;
                layoutInfoContent.y = (fHeight - layoutInfoContent.height) / 2;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::SetImageInfo( SdkGifView *pImageView, LPCWSTR lpFile, FLOAT fWidth, FLOAT fHeight )
{
    if(NULL != pImageView)
    {
        pImageView->SetLayoutInfo(0, 0, fWidth, fHeight);
        BOOL isImageLoadSuccess = pImageView->LoadFromFile(lpFile);
        if (!isImageLoadSuccess)
        {
            pImageView->SetLayoutInfo(0, 0, DEFALUT_PICTURE_WIDTH, DEFALUT_PICTURE_HEIGHT);
            WIC_GIF_TYPE gif_type = pImageView->GetImageType();
            if (WIC_GIF_TYPE_IMAGE != gif_type)
            {
                gif_type = WIC_GIF_TYPE_IMAGE;
            }
            pImageView->SetImageType(gif_type);
            pImageView->LoadFromResource(IDB_PNG_DEFAULT_PICTURE, HINST_THISCOMPONENT);
        }
        pImageView->Start();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::SetUpdateImageHandler(IImagePreviewUpdateHandler *pHandler)
{
    m_pUpdateHandler = pHandler;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::SetCanZoom(BOOL bCanZoom)
{
    if (this->IsGifZoom())
    {
        m_bCanZoom = FALSE;
    }
    else
    {
        m_bCanZoom = bCanZoom;
    }
    m_bMove = FALSE;
    if(!m_bCanZoom)
    {
        this->CancelZoom();
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagePreviewLayout::IsAnimationEnd()
{
    return m_isAnimationEnd;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::ZoomIn(FLOAT fRate /*= 1.0f*/, POINT *pPointCenter /*= NULL*/)
{
    if(m_fZoomRate > MIN_ZOOM && fRate > 0.0f)
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

void SdkImagePreviewLayout::ZoomOut(FLOAT fRate /*= 1.0f*/, POINT *pPointCenter /*= NULL*/)
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

void SdkImagePreviewLayout::Zoom(FLOAT fRate, POINT *pPointCenter /*= NULL*/)
{
    if(NULL != m_pCurrentShowView)
    {
        LAYOUTINFO layoutInfo = {0};
        m_pCurrentShowView->GetLayoutInfo(&layoutInfo);

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
                if (this->GetWidth() > layoutInfo.width)
                {
                    layoutInfo.x = (this->GetWidth() - layoutInfo.width) / 2;
                }
                else
                {
                    layoutInfo.x = pPointCenter->x - (pPointCenter->x - layoutInfo.x) * fRate;
                }
                if (this->GetHeight() > layoutInfo.height )
                {
                    layoutInfo.y = (this->GetHeight() - layoutInfo.height) / 2;
                }
                else
                {
                    layoutInfo.y = pPointCenter->y - (pPointCenter->y - layoutInfo.y) * fRate;
                }
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

        m_pCurrentShowView->SetLayoutInfo(layoutInfo.x, layoutInfo.y, layoutInfo.width, layoutInfo.height);
        m_pCurrentShowView->Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::ZoomTo( FLOAT fRate , POINT *pPointCenter /*= NULL*/)
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

void SdkImagePreviewLayout::CancelZoom()
{
    if(m_bZoom)
    {
        m_bZoom = FALSE;
        if(m_bCanPanning)
        {
            SetGesture(FALSE);
        }

        m_bCanPanning = FALSE;
        if (NULL != m_pCurrentShowView)
        {
            m_pCurrentShowView->SetLayoutInfo(
                m_curChildLayoutInfo.x,
                m_curChildLayoutInfo.y,
                m_curChildLayoutInfo.width,
                m_curChildLayoutInfo.height);
        }

        m_fZoomRate = MIN_ZOOM;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::SetGesture(BOOL bInteria)
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

void SdkImagePreviewLayout::StopAnimation()
{
    m_nMoveIndex = 0;
    this->StopAnimation(m_pLeftTranslateAnimation);
    this->StopAnimation(m_pRightTranslateAnimation);
    this->StopAnimation(m_pDynamicTranslate);
    if(0 != m_fOffsetX)
    {
        m_fOffsetX = 0;
        Invalidate(TRUE);
    }
    m_isAnimationEnd = TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::StopAnimation( SdkAnimation *pAnimation )
{
    if(NULL != pAnimation)
    {
        //pAnimation->Stop();
        pAnimation->Finish();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::DoPanning(const POINT &pointLast, const POINT &pointCur)
{
    if(m_bCanPanning)
    {
        if(NULL != m_pCurrentShowView)
        {
            LAYOUTINFO layout = {0};
            m_pCurrentShowView->GetLayoutInfo(&layout);

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
                if (layout.width > this->GetWidth())
                {
                    layout.x = nNewX;
                }
                if (layout.height > this->GetHeight())
                {
                    layout.y = nNewY;
                }
                m_pCurrentShowView->SetLayoutInfo(layout.x, layout.y, layout.width, layout.height);
                m_pCurrentShowView->Invalidate();
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagePreviewLayout::OnTouchEvent(UINT /*message*/, WPARAM /*wParam*/, LPARAM lParam)
{
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
                    if(m_bCanZoom && m_isAnimationEnd && !this->IsGifZoom())
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
                }
            }
            break;
        case GID_PAN:
            {
                switch (gi.dwFlags)
                {
                case GF_BEGIN:
                    this->OnLButtonDown(NULL, MAKELPARAM(gi.ptsLocation.x, gi.ptsLocation.y));
                    break;
                default:

                    if(gi.dwFlags & GF_END)
                    {
                        this->OnLButtonUp(NULL, MAKELPARAM(gi.ptsLocation.x, gi.ptsLocation.y));
                        break;
                    }

                    this->OnMouseMove(NULL, MAKELPARAM(gi.ptsLocation.x, gi.ptsLocation.y));
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

void SdkImagePreviewLayout::Reset()
{
    m_nMoveIndex        = 0;
    this->StopAnimation();
    m_fOffsetX          = 0;
    m_fZoomRate         = MIN_ZOOM;
    m_bLbuttonDown      = FALSE;
    m_bUpOrDown         = FALSE;
    m_bZoom             = FALSE;
    m_isAutoplay        = FALSE;
    m_isAnimationEnd    = TRUE;
    m_isKeyLastOrUp     = FALSE;
    m_isBottomIn        = FALSE;

    m_nNewCurIndex = m_nCurIndex = 1;
    if(m_bCanPanning)
    {
        SetGesture(FALSE);
    }
    m_bCanPanning       = FALSE;
    if (!this->IsGifZoom())
    {
        m_bCanZoom          = TRUE;
    }
    m_bMove             = FALSE;
    m_bContinue         = FALSE;
    m_pCurrentShowView  = NULL;

    m_pLastImageView->Stop();
    m_pCurImageView->Stop();
    m_pNextImageView->Stop();

    m_pLastImageView->ClearAssocData();
    m_pCurImageView->ClearAssocData();
    m_pNextImageView->ClearAssocData();
}

//////////////////////////////////////////////////////////////////////////

INT32 SdkImagePreviewLayout::GetNewShowIndex( BOOL bNext )
{
    INT32 nNewIndex = m_nCurIndex;
    if(bNext)
    {
        switch(m_nCurIndex)
        {
        case 0:
            nNewIndex = m_nCurIndex + 1;
            break;
        case 1:
            nNewIndex = m_nCurIndex + 1;
            break;
        case 2:
            nNewIndex = 0;
            break;
        }
    }
    else
    {
        switch(m_nCurIndex)
        {
        case 0:
            nNewIndex = 2;
            break;
        case 1:
            nNewIndex = m_nCurIndex - 1;
            break;
        case 2:
            nNewIndex = m_nCurIndex - 1;
            break;
        }
    }

    return nNewIndex;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::ContinuousDramatic()
{
    FLOAT fTempStartX = m_fOffsetX;
    FLOAT fTempEndX = 0.0f;
    if (m_nMoveIndex > 0)
    {
        fTempEndX = this->GetWidth();
        m_nNewCurIndex = GetNewShowIndex(FALSE);
        m_bContinue = m_pUpdateHandler->UpdateImageView(this, FALSE);
        if ((fTempStartX != fTempEndX)  && m_bContinue)
        {
            m_isAnimationEnd = FALSE;
            m_pDynamicTranslate->SetTranslateXInfo(fTempStartX, fTempEndX, m_dAnimationTime);
            m_bCanZoom = FALSE;
            m_pDynamicTranslate->Start();
            m_nMoveIndex--;
        }
    }
    else if (m_nMoveIndex < 0)
    {
        fTempEndX -= this->GetWidth();
        m_nNewCurIndex = GetNewShowIndex(TRUE);
        m_bCanZoom = FALSE;
        m_bContinue = m_pUpdateHandler->UpdateImageView(this, TRUE);
        if ((fTempStartX != fTempEndX) && m_bContinue)
        {
            m_isAnimationEnd = FALSE;
            m_pDynamicTranslate->SetTranslateXInfo(fTempStartX, fTempEndX, m_dAnimationTime);
            m_pDynamicTranslate->Start();
            m_nMoveIndex ++;
        }
    }
    else if (!m_isAutoplay)
    {
        m_pUpdateHandler->OnBottomAnimationIn(m_nMoveIndex, m_isBottomIn);
        if (!this->IsGifZoom())
        {
            m_bCanZoom = TRUE;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::DeleteImageView()
{
    SdkGifView *pLastImageView = NULL;
    SdkGifView *pNextImageView = NULL;

    switch (m_nNewCurIndex)
    {
    case 0:
        pLastImageView = m_pNextImageView;
        pNextImageView = m_pCurImageView;
        break;
    case 1:
        pLastImageView = m_pLastImageView;
        pNextImageView = m_pNextImageView;
        break;
    case 2:
        pLastImageView = m_pCurImageView;
        pNextImageView = m_pLastImageView;
        break;
    default:
        break;
    }

    if (NULL != pLastImageView)
    {
        pLastImageView->Stop();
        pLastImageView->ClearAssocData();
    }
    if (NULL != pNextImageView)
    {
        pNextImageView->Stop();
        pNextImageView->ClearAssocData();
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkImagePreviewLayout::IsGifZoom()
{
    SdkGifView *pCurImageView = NULL;

    switch (m_nNewCurIndex)
    {
    case 0:
        pCurImageView = m_pLastImageView;
        break;
    case 1:
        pCurImageView = m_pCurImageView;
        break;
    case 2:
        pCurImageView = m_pNextImageView;
        break;
    default:
        break;
    }
    BOOL bZoom = FALSE;
    IMAGEFILE_TYPE gtype = pCurImageView->GetImageFileType();
    if (IMAGEFILE_TYPE_GIF == gtype)
    {
        bZoom = TRUE;
    }
    return bZoom;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::SetMoveIndex(INT32 nMoveIndex)
{
    m_nMoveIndex = nMoveIndex;
}

//////////////////////////////////////////////////////////////////////////

void SdkImagePreviewLayout::SetAutoPlayType( BOOL isAutoPlay )
{
    m_isAutoplay = isAutoPlay;
}