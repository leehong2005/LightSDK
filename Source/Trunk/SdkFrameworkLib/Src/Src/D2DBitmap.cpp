/*!
* @file D2DBitmap.cpp
* 
* @brief This file defines the class D2DBitmap, is used to operate image from file or resource.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#include "stdafx.h"
#include "D2DBitmap.h"
#include "D2DUtility.h"
#include "D2DDevice.h"

USING_NAMESPACE_D2D

D2DBitmap::D2DBitmap() : m_pD2DBitmap(NULL),
                         m_pRenderTarget(NULL),
                         m_pWICImageHelper(new SdkWICImageHelper())
{
}

//////////////////////////////////////////////////////////////////////////

D2DBitmap::~D2DBitmap()
{
    SAFE_RELEASE(m_pD2DBitmap);
    SAFE_RELEASE(m_pRenderTarget);
    SAFE_DELETE(m_pWICImageHelper);

    D2DDevice *pD2DDevice = GetD2DDevice();
    if ( NULL != pD2DDevice )
    {
        pD2DDevice->UnregisterD2DDeviceNotification(this);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBitmap::Initialize(D2DDevice *pD2DDevice)
{
    if ( NULL != pD2DDevice )
    {
        SetD2DDevice(pD2DDevice);

        SAFE_RELEASE(m_pRenderTarget);
        pD2DDevice->RegisterD2DDeviceNotification(this);
        pD2DDevice->GetRenderTarget(&m_pRenderTarget);
    }

    return (NULL != m_pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBitmap::Initialize(ID2D1RenderTarget *pRenderTarget)
{
    if ( NULL != pRenderTarget )
    {
        D2DDevice *pD2DDevice = D2DDevice::FromD2DRenderTarget(pRenderTarget);
        if ( NULL != pD2DDevice )
        {
            SetD2DDevice(pD2DDevice);
            pD2DDevice->RegisterD2DDeviceNotification(this);
        }

        SAFE_RELEASE(m_pRenderTarget);
        m_pRenderTarget = pRenderTarget;
        SAFE_ADDREF(m_pRenderTarget);
    }

    return ( NULL != m_pRenderTarget );
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBitmap::HasInitialized()
{
    return (NULL != m_pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBitmap::LoadFromFile(LPCWSTR lpfile, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = FALSE;

    if (NULL != m_pWICImageHelper)
    {
        retVal = m_pWICImageHelper->LoadFromFile(lpfile, uDestWidth, uDestHeight);
    }

    // Delete the old bitmap.
    SAFE_RELEASE(m_pD2DBitmap);

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBitmap::LoadFromHBITMAP(HBITMAP hBitmap, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = FALSE;

    if (NULL != m_pWICImageHelper)
    {
        retVal = m_pWICImageHelper->LoadFromHBITMAP(hBitmap, uDestWidth, uDestHeight);
    }

    // Delete the old bitmap.
    SAFE_RELEASE(m_pD2DBitmap);

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBitmap::LoadFromResource(UINT uResId, HMODULE hModule, UINT32 uDestWidth, UINT32 uDestHeight)
{
    BOOL retVal = FALSE;

    if (NULL != m_pWICImageHelper)
    {
        retVal = m_pWICImageHelper->LoadFromResource(uResId, hModule, uDestWidth, uDestHeight);
    }

    // Delete the old bitmap.
    SAFE_RELEASE(m_pD2DBitmap);

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBitmap::GetD2DBitmap(OUT ID2D1Bitmap **ppD2DBitmap)
{
    if ( NULL == ppD2DBitmap || NULL == m_pWICImageHelper )
    {
        return FALSE;
    }

    HRESULT hr = S_OK;

    if ( NULL == m_pD2DBitmap )
    {
        IWICFormatConverter *pWicFormatConverter = NULL;
        m_pWICImageHelper->GetFormatConverter(&pWicFormatConverter);

        if ( NULL != pWicFormatConverter && NULL != m_pRenderTarget )
        {
            hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pWicFormatConverter, &m_pD2DBitmap);
        }

        // Fail to create bitmap from WIC Bitmap, out of memory.
        if ( E_OUTOFMEMORY == hr )
        {
            UINT uWidth = m_pWICImageHelper->GetWidth();
            UINT uHeight = m_pWICImageHelper->GetHeight();
            // Calculate the appropriate bitmap size.
            D2DUtility::CreateScalerRect(uWidth, uHeight);

            SAFE_RELEASE(pWicFormatConverter);
            m_pWICImageHelper->CreateScalerWICBitmapSource(uWidth, uHeight);
            m_pWICImageHelper->GetFormatConverter(&pWicFormatConverter);

            if ( NULL != pWicFormatConverter && NULL != m_pRenderTarget )
            {
                hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pWicFormatConverter, &m_pD2DBitmap);
            }
        }

        SAFE_RELEASE(pWicFormatConverter);

        hr = (NULL != m_pD2DBitmap) ? S_OK : E_FAIL;

        // If succeed to create bitmap, clear the original image data, so get width, height will
        // not succeed.
        if ( SUCCEEDED(hr) )
        {
            m_pWICImageHelper->ClearImageData();
        }
    }

    if ( NULL != m_pD2DBitmap )
    {
        (*ppD2DBitmap) = m_pD2DBitmap;
        SAFE_ADDREF((*ppD2DBitmap));
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

UINT D2DBitmap::GetWidth()
{
    if ( NULL != m_pD2DBitmap )
    {
        D2D1_SIZE_F size = m_pD2DBitmap->GetSize();

        return (UINT)size.width;
    }

    return m_pWICImageHelper->GetWidth();
}

//////////////////////////////////////////////////////////////////////////

UINT D2DBitmap::GetHeight()
{
    if ( NULL != m_pD2DBitmap )
    {
        D2D1_SIZE_F size = m_pD2DBitmap->GetSize();

        return (UINT)size.height;
    }

    return m_pWICImageHelper->GetHeight();
}

//////////////////////////////////////////////////////////////////////////

void D2DBitmap::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);

    SAFE_RELEASE(m_pD2DBitmap);
    SAFE_RELEASE(m_pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void D2DBitmap::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);
}
