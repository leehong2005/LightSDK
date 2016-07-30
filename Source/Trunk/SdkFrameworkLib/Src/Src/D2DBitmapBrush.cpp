/*!
* @file D2DBitmapBrush.h
* 
* @brief The file defines D2DBitmapBrush class, inherited from D2DBrush.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/01
*/

#include "stdafx.h"
#include "D2DBitmapBrush.h"

USING_NAMESPACE_D2D

D2DBitmapBrush::D2DBitmapBrush(UINT uResId, HMODULE hModule) : m_pBitmapBrush(NULL)
{
    m_pD2DBitmap = new D2DBitmap();
    m_pD2DBitmap->LoadFromResource(uResId, hModule);
}

//////////////////////////////////////////////////////////////////////////

D2DBitmapBrush::D2DBitmapBrush(HBITMAP hBitmap) : m_pBitmapBrush(NULL)
{
    m_pD2DBitmap = new D2DBitmap();
    m_pD2DBitmap->LoadFromHBITMAP(hBitmap);
}

//////////////////////////////////////////////////////////////////////////

D2DBitmapBrush::D2DBitmapBrush(LPCTSTR lpfilePath) : m_pBitmapBrush(NULL)
{
    m_pD2DBitmap = new D2DBitmap();
    m_pD2DBitmap->LoadFromFile(lpfilePath);
}

//////////////////////////////////////////////////////////////////////////

D2DBitmapBrush::~D2DBitmapBrush()
{
    SAFE_DELETE(m_pD2DBitmap);
    SAFE_RELEASE(m_pBitmapBrush);

    D2DDevice *pD2DDevice = D2DBrush::GetD2DDevice();
    if (NULL != pD2DDevice)
    {
        pD2DDevice->UnregisterD2DDeviceNotification(this);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBitmapBrush::CreateBrush(IN D2DDevice *pD2DDevice)
{
    BOOL retVal = FALSE;

    if (NULL != pD2DDevice)
    {
        ID2D1RenderTarget *pTarget = NULL;
        pD2DDevice->GetRenderTarget(&pTarget);
        retVal = CreateBrush(pTarget);
        SAFE_RELEASE(pTarget);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBitmapBrush::CreateBrush(IN ID2D1RenderTarget *pRenderTarget)
{
    if (NULL == pRenderTarget)
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    D2DDevice *pD2DDevice = D2DDevice::FromD2DRenderTarget(pRenderTarget);
    if (NULL != pD2DDevice)
    {
        D2DBrush::SetD2DDevice(pD2DDevice);
        pD2DDevice->RegisterD2DDeviceNotification(this);
    }

    if (!m_pD2DBitmap->HasInitialized())
    {
        m_pD2DBitmap->Initialize(pRenderTarget);
    }

    D2D1_BITMAP_BRUSH_PROPERTIES propertiesXWrapYWrap =
    {
        D2D1_EXTEND_MODE_WRAP,
        D2D1_EXTEND_MODE_WRAP,
        D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
    };

    SAFE_RELEASE(m_pBitmapBrush);
    ID2D1Bitmap *pD2DBitmap = NULL;
    m_pD2DBitmap->GetD2DBitmap(&pD2DBitmap);
    if (NULL != pD2DBitmap)
    {
        hr = pRenderTarget->CreateBitmapBrush(
            pD2DBitmap,
            propertiesXWrapYWrap,
            &m_pBitmapBrush);
    }
    SAFE_RELEASE(pD2DBitmap);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBitmapBrush::HasCreatedBrush()
{
    return (NULL != m_pBitmapBrush);
}

//////////////////////////////////////////////////////////////////////////

void D2DBitmapBrush::GetD2DBrush(OUT ID2D1Brush **ppD2DBrush)
{
    if ((NULL != ppD2DBrush) && (NULL != m_pBitmapBrush))
    {
        (*ppD2DBrush) = m_pBitmapBrush;
        (*ppD2DBrush)->AddRef();
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DBitmapBrush::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    D2DBrush::OnD2DDeviceStateChange(pDevice, stateVal);
}

//////////////////////////////////////////////////////////////////////////

void D2DBitmapBrush::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    D2DBrush::OnD2DDeviceResize(pDevice, stateVal);
}
