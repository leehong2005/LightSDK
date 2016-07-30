/*!
* @file D2DLinearGradientBrush.cpp
* 
* @brief This file define a LinearGradientBrush
*        which use for Painting an area with a linear gradient 
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Zhou Yuan Qi
* @date 2010/12/31
*/

#include "stdafx.h"
#include "D2DLinearGradientBrush.h"

USING_NAMESPACE_D2D

D2DLinearGradientBrush::D2DLinearGradientBrush() : m_pLinearGradientBrush(NULL),
                                                    m_pGradientStops(NULL),
                                                    m_nGradientStopsCount(2)
{
    D2D1_GRADIENT_STOP *gradientStops = new D2D1_GRADIENT_STOP[2];
    gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White);
    gradientStops[0].position = 0.0f;
    gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Black);
    gradientStops[1].position =1.0f;

    m_nGradientStopsCount = 2;
    m_pSzGradientStops = gradientStops;

    m_gradientStartPT.x = 0;
    m_gradientStartPT.y = 0;
    m_gradientEndPT.x   = 100;
    m_gradientEndPT.y   = 100;
}

/////////////////////////////////////////////////////////////////////////

D2DLinearGradientBrush::D2DLinearGradientBrush(D2D1_GRADIENT_STOP *pGradientStops, UINT gradientStopsCount)
{
    m_pLinearGradientBrush = NULL;
    m_pGradientStops       = NULL;

    if (NULL != pGradientStops)
    {
        m_nGradientStopsCount = gradientStopsCount;
        m_pSzGradientStops = new D2D1_GRADIENT_STOP[gradientStopsCount];
        ZeroMemory(m_pSzGradientStops, gradientStopsCount * sizeof(D2D1_GRADIENT_STOP));
        UINT nSize = gradientStopsCount * sizeof(D2D1_GRADIENT_STOP);
        memcpy_s(m_pSzGradientStops, nSize, pGradientStops, nSize);
    }
}

/////////////////////////////////////////////////////////////////////////

D2DLinearGradientBrush::~D2DLinearGradientBrush()
{
    SAFE_RELEASE(m_pLinearGradientBrush);
    SAFE_RELEASE(m_pGradientStops);

    SAFE_DELETE_ARRAY(m_pSzGradientStops);

    D2DDevice *pD2DDevice = GetD2DDevice();
    if (NULL != pD2DDevice)
    {
        pD2DDevice->UnregisterD2DDeviceNotification(this);
    }
}

/////////////////////////////////////////////////////////////////////////

BOOL D2DLinearGradientBrush::CreateBrush(D2DDevice *pD2DDevice)
{
    BOOL retVal = FALSE;

    if (NULL != pD2DDevice && NULL != m_pSzGradientStops)
    {
        SAFE_RELEASE(m_pLinearGradientBrush);
        SAFE_RELEASE(m_pGradientStops);

        ID2D1RenderTarget *pRenderTarget = NULL;
        pD2DDevice->GetRenderTarget(&pRenderTarget);
        retVal = CreateBrush(pRenderTarget);

        SAFE_RELEASE(pRenderTarget);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DLinearGradientBrush::CreateBrush(IN ID2D1RenderTarget *pRenderTarget)
{
    if (NULL != pRenderTarget && NULL != m_pSzGradientStops)
    {
        D2DDevice *pD2DDevice = D2DDevice::FromD2DRenderTarget(pRenderTarget);
        if (NULL != pD2DDevice)
        {
            SetD2DDevice(pD2DDevice);
            pD2DDevice->RegisterD2DDeviceNotification(this);
        }

        SAFE_RELEASE(m_pLinearGradientBrush);
        SAFE_RELEASE(m_pGradientStops);

        HRESULT hr1 = pRenderTarget->CreateGradientStopCollection(
                                     m_pSzGradientStops,
                                     m_nGradientStopsCount,
                                     D2D1_GAMMA_2_2,
                                     D2D1_EXTEND_MODE_CLAMP,
                                     &m_pGradientStops);

        HRESULT hr2 = pRenderTarget->CreateLinearGradientBrush(
                                     D2D1::LinearGradientBrushProperties(
                                     m_gradientStartPT, m_gradientEndPT),
                                     m_pGradientStops,
                                     &m_pLinearGradientBrush);

        return (SUCCEEDED(hr1) &&SUCCEEDED(hr2)) ? TRUE : FALSE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DLinearGradientBrush::HasCreatedBrush()
{
    return (NULL != m_pLinearGradientBrush);
}

/////////////////////////////////////////////////////////////////////////

void D2DLinearGradientBrush::GetD2DBrush(OUT ID2D1Brush **ppD2DBrush)
{
    if (NULL != ppD2DBrush && NULL != m_pLinearGradientBrush)
    {
        (*ppD2DBrush) = m_pLinearGradientBrush;
        (*ppD2DBrush)->AddRef();
    }
}

///////////////////////////////////////////////////////////////////////////

void D2DLinearGradientBrush::SetStartAndEndPoint(IN FLOAT startX,
                                                 IN FLOAT startY,
                                                 IN FLOAT endX,
                                                 IN FLOAT EndY)
{
    m_gradientStartPT.x = startX;
    m_gradientStartPT.y = startY;
    m_gradientEndPT.x   = endX;
    m_gradientEndPT.y   = EndY;

    if (NULL != m_pLinearGradientBrush)
    {
        m_pLinearGradientBrush->SetStartPoint(m_gradientStartPT);
        m_pLinearGradientBrush->SetEndPoint(m_gradientEndPT);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DLinearGradientBrush::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    D2DBrush::OnD2DDeviceStateChange(pDevice, stateVal);

    CreateBrush(pDevice);
}

//////////////////////////////////////////////////////////////////////////

void D2DLinearGradientBrush::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    D2DBrush::OnD2DDeviceResize(pDevice, stateVal);
}
