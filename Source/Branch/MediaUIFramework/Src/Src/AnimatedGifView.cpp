/*!
* @file AnimatedGifView.cpp
* 
* @brief This file AnimatedGifView class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#include "AnimatedGifView.h"
#include "D2DUtility.h"

UINT_PTR AnimatedGifView::s_uTimerID = 0x1000;

AnimatedGifView::TimerIDGifViewMap AnimatedGifView::s_timerIdGifViewMap;

AnimatedGifView::AnimatedGifView() : m_isAutoStart(TRUE),
                                     m_pBitmapRenderTarget(NULL),
                                     m_isPlaying(FALSE),
                                     m_uFrameDelay(0),
                                     m_stretchMode(IMAGE_STRETCH_MODE_CENTER),
                                     m_pD2DAnimatedGif(new D2DAnimatedGif())
{
    s_uTimerID++;

    m_curTimerID = s_uTimerID;
    s_timerIdGifViewMap.insert(TImerIDGifViewPair(s_uTimerID, this));
}

//////////////////////////////////////////////////////////////////////////

AnimatedGifView::~AnimatedGifView()
{
    SAFE_DELETE(m_pD2DAnimatedGif);
    SAFE_RELEASE(m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::Start()
{
    if (NULL != m_pWindow)
    {
        m_uFrameDelay = 0;
        m_isPlaying = TRUE;
        OnPaintFrame();
        Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::Pause()
{
    if (NULL != m_pWindow)
    {
        KillTimer(m_pWindow->GetHWND(), m_curTimerID);
        m_isPlaying = FALSE;
        m_uFrameDelay = 0;
    }
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::Stop()
{
    if (NULL != m_pD2DAnimatedGif)
    {
        m_pD2DAnimatedGif->SetFrameIndex(0);
    }

    if (NULL != m_pWindow)
    {
        KillTimer(m_pWindow->GetHWND(), m_curTimerID);
    }

    m_isPlaying = FALSE;
    m_uFrameDelay = 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL AnimatedGifView::IsPlaying()
{
    return m_isPlaying;
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::SetAutoStart(BOOL fAutoStart)
{
    m_isAutoStart = fAutoStart;
}

//////////////////////////////////////////////////////////////////////////

BOOL AnimatedGifView::LoadFromFile(LPCWSTR lpfile)
{
    Stop();
    SAFE_RELEASE(m_pBitmapRenderTarget);
    BOOL isSucceed = (NULL != m_pD2DAnimatedGif) ? m_pD2DAnimatedGif->LoadFromFile(lpfile) : FALSE;
    return isSucceed;
}

//////////////////////////////////////////////////////////////////////////

BOOL AnimatedGifView::LoadFromResource(UINT uResId, HMODULE hModule)
{
    Stop();
    SAFE_RELEASE(m_pBitmapRenderTarget);
    BOOL isSucceed =  (NULL != m_pD2DAnimatedGif) ? m_pD2DAnimatedGif->LoadFromResource(uResId, hModule) : FALSE;
    return isSucceed;
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::SetImageType(WIC_GIF_TYPE type)
{
    if (NULL != m_pD2DAnimatedGif)
    {
        m_pD2DAnimatedGif->SetImageType(type);
    }
}

//////////////////////////////////////////////////////////////////////////

WIC_GIF_TYPE AnimatedGifView::GetImageType()
{
    return (NULL != m_pD2DAnimatedGif) ? m_pD2DAnimatedGif->GetImageType() : WIC_GIF_TYPE_GIF;
}

//////////////////////////////////////////////////////////////////////////

IMAGEFILE_TYPE AnimatedGifView::GetImageFileType()
{
    IMAGEFILE_TYPE imageFileType = IMAGEFILE_TYPE_UNKNOWN;

    UINT uFrameCount = m_pD2DAnimatedGif->GetFrameCount();
    switch (uFrameCount)
    {
    case 0:
        imageFileType = IMAGEFILE_TYPE_UNKNOWN;
        break;

    case 1:
        imageFileType = IMAGEFILE_TYPE_IMAGE;
        break;

    default:
        imageFileType = IMAGEFILE_TYPE_GIF;
        break;
    }

    return imageFileType;
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::AutoAdjustSize(BOOL isAutoSize)
{
    m_isAutoSize = isAutoSize;

    if (m_isAutoSize && NULL != m_pD2DAnimatedGif)
    {
        UINT uWidth = 0, uHeight = 0;
        m_pD2DAnimatedGif->GetSize(uWidth, uHeight);
        if (uWidth > 0 && uHeight > 0)
        {
            if ( ((FLOAT)uWidth != GetWidth()) || ((FLOAT)uHeight != GetHeight()) )
            {
                SetLayoutInfo(m_layoutInfo.x, m_layoutInfo.y, (FLOAT)uWidth, (FLOAT)uHeight);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    if (m_isAutoSize && NULL != m_pD2DAnimatedGif)
    {
        UINT uWidth = 0, uHeight = 0;
        m_pD2DAnimatedGif->GetSize(uWidth, uHeight);
        width  = (uWidth > 0) ? uWidth : width;
        height = (uHeight > 0) ? uHeight : height;
    }

    ViewElement::SetLayoutInfo(x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::SetImageStretchMode(IN IMAGE_STRETCH_MODE stretchMode)
{
    m_stretchMode = stretchMode;
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::ClearAssocData()
{
    SAFE_DELETE(m_pD2DAnimatedGif);
    SAFE_RELEASE(m_pBitmapRenderTarget);
    ViewElement::ClearAssocData();

    m_pD2DAnimatedGif = new D2DAnimatedGif();
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F AnimatedGifView::GetDrawRect(const D2D1_RECT_F& viewRc, ID2D1Bitmap *pID2D1Bitmap)
{
    if (NULL == pID2D1Bitmap)
    {
        return viewRc;
    }

    D2D1_RECT_F retRc = viewRc;

    switch (m_stretchMode)
    {
    case IMAGE_STRETCH_MODE_CENTER:
        {
            D2D_SIZE_F bitmapSize =  pID2D1Bitmap->GetSize();
            FLOAT width = viewRc.right - viewRc.left;
            FLOAT height = viewRc.bottom - viewRc.top;
            FLOAT srcWidth = bitmapSize.width;
            FLOAT srcHeight = bitmapSize.height;

            srcWidth = width < srcWidth ? width : srcWidth;
            srcHeight = height < srcHeight ? height : srcHeight;

            retRc.left += (width - srcWidth) / 2;
            retRc.top  += (height - srcHeight) / 2;
            retRc.right = retRc.left + srcWidth;
            retRc.bottom = retRc.top + srcHeight;
        }
        break;

    case IMAGE_STRETCH_MODE_FILL:
        // Target rectangle is same with source rectangle.
        break;

    case IMAGE_STRETCH_MODE_MARGIN:
        // Not implements.
        break;

    case IMAGE_STRETCH_MODE_FIT:
        {
            D2D_SIZE_F bitmapSize = pID2D1Bitmap->GetSize();

            FLOAT width = viewRc.right - viewRc.left;
            FLOAT height = viewRc.bottom - viewRc.top;
            FLOAT srcWidth = bitmapSize.width;
            FLOAT srcHeight = bitmapSize.height;

            FLOAT fsrcRatio = srcWidth / srcHeight;
            if (fsrcRatio > 1)
            {
                srcWidth = width;
                srcHeight = srcWidth / fsrcRatio;

                if (srcHeight > height)
                {
                    FLOAT fHeightRatio = srcHeight / height;
                    srcHeight = height;
                    srcWidth = width / fHeightRatio;
                    retRc.left += (width - srcWidth) / 2;
                    retRc.right = retRc.left + srcWidth;
                }
                else
                {
                    retRc.top += (height - srcHeight) / 2;
                    retRc.bottom = retRc.top + srcHeight;
                }
            }
            else
            {
                srcHeight = height;
                srcWidth = srcHeight * fsrcRatio;

                if (srcWidth > width)
                {
                    FLOAT fWidthRatio = srcWidth / height;
                    srcWidth = width;
                    srcHeight = height / fWidthRatio;

                    retRc.top += (height - srcHeight) / 2;
                    retRc.bottom = retRc.top + srcHeight;
                }
                else
                {
                    retRc.left += (width - srcWidth) / 2;
                    retRc.right = retRc.left + srcWidth;
                }
            }
        }
        break;
    }

    return retRc;
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    ViewElement::OnD2DDeviceStateChange(pDevice, stateVal);
    SAFE_RELEASE(m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    if ( NULL != m_pBitmapRenderTarget )
    {
        D2D1_RECT_F rc = { 0 };
        GetAbsoluteRect(rc);

        // Draw view's background.
        OnDrawBackground(pRenderTarget, rc);

        // Get the bitmap just drawing from compatible render target.
        ID2D1Bitmap *pFrameBitmap = NULL;
        if (SUCCEEDED(m_pBitmapRenderTarget->GetBitmap(&pFrameBitmap)))
        {
            // Draw the bitmap to HWND render target.
            if (NULL != pFrameBitmap)
            {
                pRenderTarget->DrawBitmap(pFrameBitmap, rc);
            }
        }
        SAFE_RELEASE(pFrameBitmap);
    }
}

//////////////////////////////////////////////////////////////////////////

void AnimatedGifView::OnPaintFrame(BOOL fStartTimer)
{
    if (NULL != m_pWindow)
    {
        if (!m_pD2DAnimatedGif->HasInitialized())
        {
            m_pD2DAnimatedGif->Initialize(m_pWindow->GetD2DDevices());
        }

        if (NULL == m_pBitmapRenderTarget)
        {
            FLOAT width  = GetWidth();
            FLOAT height = GetHeight();

            ID2D1RenderTarget *pTarget = NULL;
            m_pWindow->GetD2DDevices()->GetRenderTarget(&pTarget);
            HRESULT hr = pTarget->CreateCompatibleRenderTarget(D2D1::SizeF(width, height),
                                                               &m_pBitmapRenderTarget);

            if (FAILED(hr))
            {
                UINT uWidth  = (UINT)width;
                UINT uHeight = (UINT)height;
                D2DUtility::CreateScalerRect(uWidth, uHeight);

                hr = pTarget->CreateCompatibleRenderTarget(D2D1::SizeF((FLOAT)uWidth, (FLOAT)uHeight),
                                                           &m_pBitmapRenderTarget);

            }
            SAFE_RELEASE(pTarget);
        }
    }

    if (NULL == m_pBitmapRenderTarget)
    {
        return;
    }

    ID2D1Bitmap *pFrameBitmap = NULL;
    m_pD2DAnimatedGif->OnPaintNextFrame();
    m_pD2DAnimatedGif->GetPaintBitmap(&pFrameBitmap);
    if (NULL != pFrameBitmap)
    {
        D2D1_SIZE_F rtSize = m_pBitmapRenderTarget->GetSize();
        D2D1_RECT_F rcPaint = { 0, 0, rtSize.width, rtSize.height };

        m_pBitmapRenderTarget->BeginDraw();
        m_pBitmapRenderTarget->Clear(ColorF(0, 0));
        m_pBitmapRenderTarget->DrawBitmap(pFrameBitmap, GetDrawRect(rcPaint, pFrameBitmap));
        m_pBitmapRenderTarget->EndDraw();
    }
    SAFE_RELEASE(pFrameBitmap);

    // Start timer to play the GIF.
    if ((NULL != m_pWindow) && fStartTimer && m_pD2DAnimatedGif->GetFrameCount() > 1)
    {
        UINT frameDelay = m_pD2DAnimatedGif->GetFrameDelay();
        frameDelay = (0 == frameDelay) ? 200 : frameDelay;
        if ( (frameDelay > 1) && (m_uFrameDelay != frameDelay) )
        {
            m_uFrameDelay = frameDelay;
            HWND hWnd = m_pWindow->GetHWND();
            KillTimer(hWnd, m_curTimerID);
            SetTimer(hWnd, m_curTimerID, frameDelay, AnimatedGifView::OnTimerProc);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void CALLBACK AnimatedGifView::OnTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    UNREFERENCED_PARAMETER(hwnd);
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(dwTime);

    AnimatedGifView::TimerIDGifViewMap::iterator itor = s_timerIdGifViewMap.find(idEvent);
    if (itor != s_timerIdGifViewMap.end())
    {
        AnimatedGifView *pView = static_cast<AnimatedGifView*>(itor->second);
        if (NULL != pView)
        {
            Window *pWindow = pView->GetWindow();
            if (NULL != pWindow)
            {
                HWND hWnd = pWindow->GetHWND();
                BOOL isVisible = ::IsWindowVisible(hWnd);
                BOOL isWindow  = ::IsWindow(hWnd);
                BOOL isIconic  = ::IsIconic(hWnd);

                if (isVisible && isWindow && !isIconic)
                {
                    pView->OnPaintFrame();
                    pView->ForceInvalidate();
                }
            }
        }
    }
}
