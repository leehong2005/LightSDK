/*!
* @file SdkGifView.cpp
* 
* @brief This file SdkGifView class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#include "stdafx.h"
#include "SdkGifView.h"
#include "D2DUtility.h"
#include "D2DAnimatedGif.h"
#include "SdkCommonInclude.h"

USING_NAMESPACE_VIEWS

UINT_PTR SdkGifView::s_uTimerID = 0x1000;
SdkGifView::TimerIDGifViewMap SdkGifView::s_timerIdGifViewMap;

/*!
* @brief The internal data of gif view.
*/
struct NAMESPACE_VIEWS::SdkGifView::_GIFVIEW_INTERNALDATA
{
    BOOL                        m_isAutoStart;          // Tell object whether is first frame.
    BOOL                        m_isAutoSize;           // Tell whether auto adjust size or not.
    BOOL                        m_isPlaying;            // Indicates whether is playing.
    BOOL                        m_hasFirstDraw;         // Indicate whether called pain frame first.
    UINT                        m_uFrameDelay;          // Frame delay.
    UINT_PTR                    m_curTimerID;           // Current timer id.
    IMAGE_STRETCH_MODE          m_stretchMode;          // The flag whether to Stretch the bitmap to fill all view.
    D2DAnimatedGif             *m_pD2DAnimatedGif;      // The pointer to D2DAnimatedGif.
    ID2D1BitmapRenderTarget    *m_pBitmapRenderTarget;  // The compatible render target.
};

//////////////////////////////////////////////////////////////////////////

SdkGifView::SdkGifView()
{
    SetClassName(CLASSNAME_GIFVIEW);

    s_uTimerID++;
    s_timerIdGifViewMap.insert(make_pair(s_uTimerID, this));

    m_pGifViewData = new _GIFVIEW_INTERNALDATA();
    ZeroMemory(m_pGifViewData, sizeof(_GIFVIEW_INTERNALDATA));

    m_pGifViewData->m_curTimerID            = s_uTimerID;
    m_pGifViewData->m_isAutoStart           = TRUE;
    m_pGifViewData->m_pBitmapRenderTarget   = NULL;
    m_pGifViewData->m_hasFirstDraw          = FALSE;
    m_pGifViewData->m_isPlaying             = FALSE;
    m_pGifViewData->m_uFrameDelay           = 0;
    m_pGifViewData->m_stretchMode           = IMAGE_STRETCH_MODE_CENTER;
    m_pGifViewData->m_pD2DAnimatedGif       = new D2DAnimatedGif();
}

//////////////////////////////////////////////////////////////////////////

SdkGifView::~SdkGifView()
{
    SAFE_DELETE(m_pGifViewData->m_pD2DAnimatedGif);
    SAFE_RELEASE(m_pGifViewData->m_pBitmapRenderTarget);
    SAFE_DELETE(m_pGifViewData);

    // Should delete the map item from the map when the destructor function is called.
    map<UINT_PTR, SdkGifView*>::iterator iter;
    for (iter = s_timerIdGifViewMap.begin(); iter != s_timerIdGifViewMap.end(); ++iter)
    {
        if (iter->second == this)
        {
            break;
        }
    }

    if (iter != s_timerIdGifViewMap.end())
    {
        s_timerIdGifViewMap.erase(iter);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::Start()
{
    if ( NULL != m_pWindow )
    {
        m_pGifViewData->m_uFrameDelay = 0;
        m_pGifViewData->m_isPlaying = TRUE;

        OnPaintFrame();
        Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::Pause()
{
    if ( NULL != m_pWindow )
    {
        KillTimer(m_pWindow->GetHwnd(), m_pGifViewData->m_curTimerID);
        m_pGifViewData->m_isPlaying = FALSE;
        m_pGifViewData->m_uFrameDelay = 0;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::Stop()
{
    if ( NULL != m_pGifViewData->m_pD2DAnimatedGif )
    {
        m_pGifViewData->m_pD2DAnimatedGif->SetFrameIndex(0);
    }

    if ( NULL != m_pWindow )
    {
        KillTimer(m_pWindow->GetHwnd(), m_pGifViewData->m_curTimerID);
    }

    m_pGifViewData->m_isPlaying   = FALSE;
    m_pGifViewData->m_uFrameDelay = 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkGifView::IsPlaying()
{
    return m_pGifViewData->m_isPlaying;
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::SetAutoStart(BOOL fAutoStart)
{
    m_pGifViewData->m_isAutoStart = fAutoStart;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkGifView::LoadFromFile(LPCWSTR lpfile)
{
    Stop();
    SAFE_RELEASE(m_pGifViewData->m_pBitmapRenderTarget);

    if ( NULL != m_pGifViewData->m_pD2DAnimatedGif )
    {
        return m_pGifViewData->m_pD2DAnimatedGif->LoadFromFile(lpfile);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkGifView::LoadFromResource(UINT uResId, HMODULE hModule)
{
    Stop();
    SAFE_RELEASE(m_pGifViewData->m_pBitmapRenderTarget);

    if ( NULL != m_pGifViewData->m_pD2DAnimatedGif )
    {
        return m_pGifViewData->m_pD2DAnimatedGif->LoadFromResource(uResId, hModule);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::SetImageType(WIC_GIF_TYPE type)
{
    if ( NULL != m_pGifViewData->m_pD2DAnimatedGif )
    {
        m_pGifViewData->m_pD2DAnimatedGif->SetImageType(type);
    }
}

//////////////////////////////////////////////////////////////////////////

WIC_GIF_TYPE SdkGifView::GetImageType()
{
    if ( NULL != m_pGifViewData->m_pD2DAnimatedGif )
    {
        m_pGifViewData->m_pD2DAnimatedGif->GetImageType();
    }

    return WIC_GIF_TYPE_GIF;
}

//////////////////////////////////////////////////////////////////////////

IMAGEFILE_TYPE SdkGifView::GetImageFileType()
{
    IMAGEFILE_TYPE imageFileType = IMAGEFILE_TYPE_UNKNOWN;

    UINT uFrameCount = m_pGifViewData->m_pD2DAnimatedGif->GetFrameCount();
    switch ( uFrameCount )
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

void SdkGifView::AutoAdjustSize(BOOL isAutoSize)
{
    m_pGifViewData->m_isAutoSize = isAutoSize;

    if ( m_pGifViewData->m_isAutoSize && NULL != m_pGifViewData->m_pD2DAnimatedGif )
    {
        UINT uWidth = 0, uHeight = 0;
        m_pGifViewData->m_pD2DAnimatedGif->GetSize(uWidth, uHeight);

        if ( uWidth > 0 && uHeight > 0 )
        {
            if ( ((FLOAT)uWidth != GetWidth()) || ((FLOAT)uHeight != GetHeight()) )
            {
                SetLayoutInfo(m_layoutInfo.x, m_layoutInfo.y, (FLOAT)uWidth, (FLOAT)uHeight);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    if ( m_pGifViewData->m_isAutoSize && NULL != m_pGifViewData->m_pD2DAnimatedGif )
    {
        UINT uWidth = 0, uHeight = 0;
        m_pGifViewData->m_pD2DAnimatedGif->GetSize(uWidth, uHeight);

        width  = (uWidth > 0) ? uWidth : width;
        height = (uHeight > 0) ? uHeight : height;
    }

    SdkViewElement::SetLayoutInfo(x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::SetStretchMode(IN IMAGE_STRETCH_MODE stretchMode)
{
    m_pGifViewData->m_stretchMode = stretchMode;
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::ClearAssocData()
{
    SAFE_DELETE(m_pGifViewData->m_pD2DAnimatedGif);
    SAFE_RELEASE(m_pGifViewData->m_pBitmapRenderTarget);
    SdkViewElement::ClearAssocData();

    m_pGifViewData->m_pD2DAnimatedGif = new D2DAnimatedGif();
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F SdkGifView::GetDrawRect(const D2D1_RECT_F& viewRc, ID2D1Bitmap *pID2D1Bitmap)
{
    if (NULL == pID2D1Bitmap)
    {
        return viewRc;
    }

    D2D1_RECT_F retRc = viewRc;

    switch (m_pGifViewData->m_stretchMode)
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

    case IMAGE_STRETCH_MODE_FIT:
        {
            D2D_SIZE_F bitmapSize = pID2D1Bitmap->GetSize();

            FLOAT width = viewRc.right - viewRc.left;
            FLOAT height = viewRc.bottom - viewRc.top;
            FLOAT srcWidth = bitmapSize.width;
            FLOAT srcHeight = bitmapSize.height;

            FLOAT fsrcRatio = srcWidth / srcHeight;
            if ( fsrcRatio > 1 )
            {
                srcWidth = width;
                srcHeight = srcWidth / fsrcRatio;

                if ( srcHeight > height )
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

                if ( srcWidth > width )
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

    case IMAGE_STRETCH_MODE_FILL:
        // Target rectangle is same with source rectangle.
        break;

    case IMAGE_STRETCH_MODE_MARGIN:
        // Not implements.
        break;
    }

    return retRc;
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    SdkViewElement::OnD2DDeviceStateChange(pDevice, stateVal);

    SAFE_RELEASE(m_pGifViewData->m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    // If auto start and first time, we call Start function to start playing.
    if (m_pGifViewData->m_isAutoStart)
    {
        if (!m_pGifViewData->m_hasFirstDraw)
        {
            m_pGifViewData->m_hasFirstDraw = TRUE;
            Start();
        }
    }

    if ( NULL != m_pGifViewData->m_pBitmapRenderTarget )
    {
        SdkViewElement::OnDrawItem(pRenderTarget);

        // Get the bitmap just drawing from compatible render target.
        ID2D1Bitmap *pFrameBitmap = NULL;
        if ( SUCCEEDED(m_pGifViewData->m_pBitmapRenderTarget->GetBitmap(&pFrameBitmap)) )
        {
            // Draw the bitmap to HWND render target.
            if (NULL != pFrameBitmap)
            {
                D2D1_RECT_F rc = { 0 };
                GetAbsoluteRect(rc);
                pRenderTarget->DrawBitmap(pFrameBitmap, rc);
            }
        }
        SAFE_RELEASE(pFrameBitmap);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkGifView::OnPaintFrame(BOOL fStartTimer)
{
    if ( NULL != m_pWindow )
    {
        if ( !m_pGifViewData->m_pD2DAnimatedGif->HasInitialized() )
        {
            m_pGifViewData->m_pD2DAnimatedGif->Initialize(m_pWindow->GetD2DDevices());
        }

        if ( NULL == m_pGifViewData->m_pBitmapRenderTarget )
        {
            FLOAT width  = GetWidth();
            FLOAT height = GetHeight();

            ID2D1RenderTarget *pTarget = NULL;
            m_pWindow->GetD2DDevices()->GetRenderTarget(&pTarget);

            HRESULT hr = pTarget->CreateCompatibleRenderTarget(
                D2D1::SizeF(width, height),
                &(m_pGifViewData->m_pBitmapRenderTarget));

            if ( FAILED(hr) )
            {
                UINT uWidth  = (UINT)width;
                UINT uHeight = (UINT)height;
                D2DUtility::CreateScalerRect(uWidth, uHeight);

                hr = pTarget->CreateCompatibleRenderTarget(
                    D2D1::SizeF((FLOAT)uWidth, (FLOAT)uHeight),
                    &(m_pGifViewData->m_pBitmapRenderTarget));

            }
            SAFE_RELEASE(pTarget);
        }
    }

    if ( NULL == m_pGifViewData->m_pBitmapRenderTarget )
    {
        return;
    }

    // The temporary variables.
    ID2D1BitmapRenderTarget *pBitmapRenderTarget = m_pGifViewData->m_pBitmapRenderTarget;
    D2DAnimatedGif *pD2DAnimatedGif = m_pGifViewData->m_pD2DAnimatedGif;

    // Draw frame bitmap to compatible render target.
    ID2D1Bitmap *pFrameBitmap = NULL;
    pD2DAnimatedGif->OnPaintNextFrame();
    pD2DAnimatedGif->GetPaintBitmap(&pFrameBitmap);

    if ( NULL != pFrameBitmap )
    {
        D2D1_SIZE_F rtSize = pBitmapRenderTarget->GetSize();
        D2D1_RECT_F rcPaint = { 0, 0, rtSize.width, rtSize.height };

        pBitmapRenderTarget->BeginDraw();
        pBitmapRenderTarget->Clear(ColorF(0, 0));
        pBitmapRenderTarget->DrawBitmap(pFrameBitmap, GetDrawRect(rcPaint, pFrameBitmap));
        pBitmapRenderTarget->EndDraw();
    }

    SAFE_RELEASE(pFrameBitmap);

    // Start timer to play the GIF.
    if ( (NULL != m_pWindow) && fStartTimer && pD2DAnimatedGif->GetFrameCount() > 1 )
    {
        UINT frameDelay = pD2DAnimatedGif->GetFrameDelay();
        frameDelay = (0 == frameDelay) ? 200 : frameDelay;

        if ( (frameDelay > 1) && (m_pGifViewData->m_uFrameDelay != frameDelay) )
        {
            m_pGifViewData->m_uFrameDelay = frameDelay;
            HWND hWnd = m_pWindow->GetHwnd();

            KillTimer(hWnd, m_pGifViewData->m_curTimerID);
            SetTimer(hWnd, m_pGifViewData->m_curTimerID, frameDelay, SdkGifView::OnTimerProc);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void CALLBACK SdkGifView::OnTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    UNREFERENCED_PARAMETER(hwnd);
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(dwTime);

    SdkGifView::TimerIDGifViewMap::iterator itor = s_timerIdGifViewMap.find(idEvent);

    if ( itor != s_timerIdGifViewMap.end() )
    {
        SdkGifView *pView = static_cast<SdkGifView*>(itor->second);

        if ( NULL != pView )
        {
            HWND hWnd = pView->GetWindow()->GetHwnd();
            BOOL isVisible = ::IsWindowVisible(hWnd);
            BOOL isWindow  = ::IsWindow(hWnd);
            BOOL isIconic  = ::IsIconic(hWnd);

            // If the window is visible, not iconic, and window is valid.
            if ( isVisible && isWindow && !isIconic )
            {
                pView->OnPaintFrame();
                pView->ForceInvalidate();
            }
        }
    }
}
