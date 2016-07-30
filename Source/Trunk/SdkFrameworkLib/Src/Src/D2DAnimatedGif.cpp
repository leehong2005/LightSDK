/*!
* @file D2DAnimatedGif.cpp
* 
* @brief This file defines methods for operating GIF.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/03
*/

#include "stdafx.h"
#include "D2DAnimatedGif.h"
#include "D2DRectUtility.h"
#include "D2DUtility.h"
#include "D2DDevice.h"

USING_NAMESPACE_D2D

D2DAnimatedGif::D2DAnimatedGif() : m_pD2DBitmap(NULL),
                                   m_pRenderTarget(NULL),
                                   m_pBitmapRenderTarget(NULL),
                                   m_bkColor(ColorF(0, 0)),
                                   m_pWicAnimatedGif(new SdkWICAnimatedGif())
{
}

//////////////////////////////////////////////////////////////////////////

D2DAnimatedGif::~D2DAnimatedGif()
{
    SAFE_RELEASE(m_pD2DBitmap);
    SAFE_RELEASE(m_pRenderTarget);
    SAFE_RELEASE(m_pBitmapRenderTarget);

    SAFE_DELETE(m_pWicAnimatedGif);

    D2DDevice *pD2DDevice = GetD2DDevice();
    if (NULL != pD2DDevice)
    {
        pD2DDevice->UnregisterD2DDeviceNotification(this);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::Initialize(D2DDevice *pD2DDevice)
{
    if (NULL != pD2DDevice)
    {
        SetD2DDevice(pD2DDevice);
        SAFE_RELEASE(m_pRenderTarget);
        pD2DDevice->RegisterD2DDeviceNotification(this);
        pD2DDevice->GetRenderTarget(&m_pRenderTarget);
    }

    return (NULL != m_pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::Initialize(ID2D1RenderTarget *pRenderTarget)
{
    if (NULL != pRenderTarget)
    {
        D2DDevice *pD2DDevice = D2DDevice::FromD2DRenderTarget(pRenderTarget);
        if (NULL != pD2DDevice)
        {
            SetD2DDevice(pD2DDevice);
            pD2DDevice->RegisterD2DDeviceNotification(this);
        }

        SAFE_RELEASE(m_pRenderTarget);
        m_pRenderTarget = pRenderTarget;
        m_pRenderTarget->AddRef();
    }

    return (NULL != m_pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::HasInitialized()
{
    return (NULL != m_pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::LoadFromFile(LPCWSTR lpfile)
{
    BOOL retVal = FALSE;

    if (NULL != m_pWicAnimatedGif)
    {
        retVal = m_pWicAnimatedGif->LoadFromFile(lpfile);
        if (retVal)
        {
            SAFE_RELEASE(m_pD2DBitmap);
            SAFE_RELEASE(m_pBitmapRenderTarget);
        }
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::LoadFromResource(UINT uResId, HMODULE hModule)
{
    BOOL retVal = FALSE;

    if (NULL != m_pWicAnimatedGif)
    {
        retVal = m_pWicAnimatedGif->LoadFromResource(uResId, hModule);
        if (retVal)
        {
            SAFE_RELEASE(m_pD2DBitmap);
            SAFE_RELEASE(m_pBitmapRenderTarget);
        }
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

void D2DAnimatedGif::SetImageType(WIC_GIF_TYPE type)
{
    if (NULL != m_pWicAnimatedGif)
    {
        m_pWicAnimatedGif->SetImageType(type);
    }
}

//////////////////////////////////////////////////////////////////////////

WIC_GIF_TYPE D2DAnimatedGif::GetImageType()
{
    return (NULL != m_pWicAnimatedGif) ? m_pWicAnimatedGif->GetImageType() : WIC_GIF_TYPE_GIF;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::OnPaintNextFrame()
{
    if (NULL == m_pBitmapRenderTarget)
    {
        D2DDevice *pD2DDevice = GetD2DDevice();
        if (NULL != pD2DDevice)
        {
            UINT uWidth = 0, uHeight = 0;
            m_pWicAnimatedGif->GetSize(uWidth, uHeight);

            ID2D1RenderTarget *pTarget = NULL;
            pD2DDevice->GetRenderTarget(&pTarget);
            HRESULT hr = pTarget->CreateCompatibleRenderTarget(
                D2D1::SizeF((FLOAT)uWidth, (FLOAT)uHeight),
                &m_pBitmapRenderTarget);

            if (FAILED(hr))
            {
                D2DUtility::CreateScalerRect(uWidth, uHeight);

                hr = pTarget->CreateCompatibleRenderTarget(
                    D2D1::SizeF((FLOAT)uWidth, (FLOAT)uHeight),
                    &m_pBitmapRenderTarget);
            }
            SAFE_RELEASE(pTarget);

        }
    }

    if (NULL != m_pBitmapRenderTarget)
    {
        UINT uFrameIndex = m_pWicAnimatedGif->GetFrameIndex();
        DisposeCurrentFrame(m_pWicAnimatedGif->GetFrameDisposal());

        ID2D1Bitmap *pFrameBitmap = NULL;
        GetNextFrame(&pFrameBitmap);
        if (NULL != pFrameBitmap)
        {
            D2D1_RECT_F frameRc = { 0 };
            GetFrameRect(frameRc);
            if (D2DRectUtility::IsEmpty(frameRc))
            {
                D2D1_SIZE_F fSize = m_pBitmapRenderTarget->GetSize();

                frameRc.left   = 0;
                frameRc.top    = 0;
                frameRc.right = fSize.width;
                frameRc.bottom = fSize.height;
            }

            m_pBitmapRenderTarget->BeginDraw();
            if (0 == uFrameIndex)
            {
                m_pBitmapRenderTarget->Clear(ColorF(0, 0));
            }
            m_pBitmapRenderTarget->DrawBitmap(pFrameBitmap, frameRc);
            m_pBitmapRenderTarget->EndDraw();
        }
        SAFE_RELEASE(pFrameBitmap);
    }

    return (NULL != m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::GetPaintBitmap(ID2D1Bitmap **ppD2DBitmap)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pBitmapRenderTarget)
    {
        hr = m_pBitmapRenderTarget->GetBitmap(ppD2DBitmap);
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

UINT D2DAnimatedGif::GetFrameDelay()
{
    return (NULL != m_pWicAnimatedGif) ? m_pWicAnimatedGif->GetFrameDelay() : 0;
}

//////////////////////////////////////////////////////////////////////////

UINT D2DAnimatedGif::GetFrameCount()
{
    return (NULL != m_pWicAnimatedGif) ? m_pWicAnimatedGif->GetFrameCount() : 0;
}

//////////////////////////////////////////////////////////////////////////

void D2DAnimatedGif::GetSize(OUT UINT &uWidth, OUT UINT &uHeight)
{
    if (NULL != m_pWicAnimatedGif)
    {
        m_pWicAnimatedGif->GetSize(uWidth, uHeight);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DAnimatedGif::SetClearColor(const ColorF &color)
{
    m_bkColor = color;
}

//////////////////////////////////////////////////////////////////////////

UINT D2DAnimatedGif::GetFrameIndex()
{
    return (NULL != m_pWicAnimatedGif) ? m_pWicAnimatedGif->GetFrameIndex() : 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::IsEndAnimation()
{
    return (NULL != m_pWicAnimatedGif) ? m_pWicAnimatedGif->IsEndAnimation() : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::DisposeCurrentFrame(UINT uFrameDisposal)
{
    switch (uFrameDisposal)
    {
    case DM_UNDEFINED:
    case DM_NONE:
        // We simply draw on the previous frames. Do nothing here.
        break;

    case DM_BACKGROUND:
        // Clear the area covered by the current raw frame with background color
        ClearCurrentFrameArea();
        break;

    case DM_PREVIOUS:
        break;
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::ClearCurrentFrameArea(BOOL isUsingCustomColor)
{
    if (NULL != m_pBitmapRenderTarget)
    {
        D2D1_RECT_F frameRc = { 0 };
        GetFrameRect(frameRc);
        m_pBitmapRenderTarget->BeginDraw();
        m_pBitmapRenderTarget->PushAxisAlignedClip(frameRc, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
        m_pBitmapRenderTarget->Clear(isUsingCustomColor ? m_bkColor : GetBackgroundColor());
        m_pBitmapRenderTarget->PopAxisAlignedClip();
        m_pBitmapRenderTarget->EndDraw();
    }

    return (NULL != m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::GetNextFrame(ID2D1Bitmap **ppD2DBitmap)
{
    if (NULL == ppD2DBitmap || NULL == m_pWicAnimatedGif)
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    BOOL isImage = (1 == m_pWicAnimatedGif->GetFrameCount());
    if (isImage)
    {
        if (NULL == m_pD2DBitmap)
        {
            GetFrameBitmap(ppD2DBitmap);
            m_pWicAnimatedGif->ClearData();
        }
    }
    else
    {
        GetFrameBitmap(ppD2DBitmap);
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::GetFrameBitmap(OUT ID2D1Bitmap **ppD2DBitmap)
{
    if (NULL == ppD2DBitmap)
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    IWICFormatConverter *ppWicFormatConverter = NULL;
    BOOL retVal = m_pWicAnimatedGif->GetNextFrame(&ppWicFormatConverter);

    if (retVal && (NULL != m_pRenderTarget))
    {
        SAFE_RELEASE(m_pD2DBitmap);
        hr = m_pRenderTarget->CreateBitmapFromWicBitmap(ppWicFormatConverter, &m_pD2DBitmap);

        if (E_OUTOFMEMORY == hr)
        {
            UINT uWidth = 0;
            UINT uHeight = 0;
            m_pWicAnimatedGif->GetSize(uWidth, uHeight);
            D2DUtility::CreateScalerRect(uWidth, uHeight);

            m_pWicAnimatedGif->CreateScalerWICAnimatedGifSource(uWidth, uHeight);

            SAFE_RELEASE(ppWicFormatConverter);
            BOOL retValue = m_pWicAnimatedGif->GetFormatConverter(&ppWicFormatConverter);
            if ((TRUE == retValue) && (NULL != m_pRenderTarget))
            {
                SAFE_RELEASE(m_pD2DBitmap);
                hr = m_pRenderTarget->CreateBitmapFromWicBitmap(ppWicFormatConverter, &m_pD2DBitmap);
            }
        }

        if (SUCCEEDED(hr))
        {
            (*ppD2DBitmap) = m_pD2DBitmap;
            (*ppD2DBitmap)->AddRef();
        }
    }

    SAFE_RELEASE(ppWicFormatConverter);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::GetFrameAt(UINT uFrameIndex, ID2D1Bitmap **ppD2DBitmap)
{
    if (NULL == ppD2DBitmap || NULL == m_pWicAnimatedGif)
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    IWICFormatConverter *ppWicFormatConverter = NULL;
    BOOL retVal = m_pWicAnimatedGif->GetFrameAt(uFrameIndex, &ppWicFormatConverter);
    if ( (TRUE == retVal) && (NULL != m_pRenderTarget) )
    {
        SAFE_RELEASE(m_pD2DBitmap);
        hr = m_pRenderTarget->CreateBitmapFromWicBitmap(ppWicFormatConverter, &m_pD2DBitmap);

        if (SUCCEEDED(hr))
        {
            (*ppD2DBitmap) = m_pD2DBitmap;
            (*ppD2DBitmap)->AddRef();
        }
    }

    SAFE_RELEASE(ppWicFormatConverter);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DAnimatedGif::GetFrameRect(OUT D2D1_RECT_F& rc)
{
    if (NULL != m_pWicAnimatedGif)
    {
        RECT rect = { 0 };
        m_pWicAnimatedGif->GetFrameRect(rect);
        rc.left     = (FLOAT)rect.left;
        rc.top      = (FLOAT)rect.top;
        rc.right    = (FLOAT)rect.right;
        rc.bottom   = (FLOAT)rect.bottom;

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void D2DAnimatedGif::SetFrameIndex(UINT uNextFrameIndex)
{
    if (NULL != m_pWicAnimatedGif)
    {
        m_pWicAnimatedGif->SetFrameIndex(uNextFrameIndex);
    }
}

//////////////////////////////////////////////////////////////////////////

D2D1_COLOR_F D2DAnimatedGif::GetBackgroundColor()
{
    if (NULL != m_pWicAnimatedGif)
    {
        UINT rgb = 0;
        FLOAT alpha = 1.0f;
        m_pWicAnimatedGif->GetBackgroundColor(rgb, alpha);
        return D2D1::ColorF(rgb, alpha);
    }
    else
    {
        return D2D1::ColorF(ColorF::White);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DAnimatedGif::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);

    SAFE_RELEASE(m_pRenderTarget);
    SAFE_RELEASE(m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void D2DAnimatedGif::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);

    SAFE_RELEASE(m_pRenderTarget);
    SAFE_RELEASE(m_pBitmapRenderTarget);
}
