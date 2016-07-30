/*!
* @file D2DRadialGradientBrush.h
* 
* @brief This file define a RadialGradientBrush
*        which use for Painting an area with a radial gradient 
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Zhou Yuan Qi
* @date 2010/02/10
*/

#include "stdafx.h"
#include "D2DRadialGradientBrush.h"

USING_NAMESPACE_D2D

D2DRadialGradientBrush::D2DRadialGradientBrush(): m_nGradientStopsCount(2),
                                                  m_pGradientStops(NULL),
                                                  m_pRadialGradientBrush(NULL)
{
    D2D1_GRADIENT_STOP *gradientStops = new D2D1_GRADIENT_STOP[2];
    gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White, 1);
    gradientStops[0].position = 0.0f;
    gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Black, 1);
    gradientStops[1].position =0.5f;
    
    m_gradientCenterPT.x  = 50;
    m_gradientCenterPT.y  = 50;

    m_gradientOffsetPT.x  = 0;
    m_gradientOffsetPT.y  = 0;

    m_gradientRadiusX     = 50;
    m_gradientRadiusY     = 50;
    m_nGradientStopsCount = 2;
    m_pSzGradientStops = gradientStops;
}

/////////////////////////////////////////////////////////////////////////

D2DRadialGradientBrush::D2DRadialGradientBrush(
    D2D1_GRADIENT_STOP *pGradientStops, 
    UINT gradientStopsCount)
{
    m_pGradientStops = NULL;

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

D2DRadialGradientBrush::~D2DRadialGradientBrush()
{
    SAFE_RELEASE(m_pRadialGradientBrush);
    SAFE_RELEASE(m_pGradientStops);
    SAFE_DELETE_ARRAY(m_pSzGradientStops);

    D2DDevice *pD2DDevice = GetD2DDevice();
    if (NULL != pD2DDevice)
    {
        pD2DDevice->UnregisterD2DDeviceNotification(this);
    }
}

/////////////////////////////////////////////////////////////////////////

BOOL D2DRadialGradientBrush::CreateBrush(D2DDevice *pD2DDevice)
{
    BOOL retVal = FALSE;

    if(NULL != pD2DDevice && NULL != m_pSzGradientStops)
    {
        SAFE_RELEASE(m_pRadialGradientBrush);
        SAFE_RELEASE(m_pGradientStops);
        ID2D1RenderTarget *pRenderTarget = NULL;
        pD2DDevice->GetRenderTarget(&pRenderTarget);
        retVal = CreateBrush(pRenderTarget);
        SAFE_RELEASE(pRenderTarget);
    }

    return retVal;
}

/////////////////////////////////////////////////////////////////////////

BOOL D2DRadialGradientBrush::CreateBrush(IN ID2D1RenderTarget *pRenderTarget)
{
    if (NULL != pRenderTarget && NULL != m_pSzGradientStops)
    {
        D2DDevice *pD2DDevice = D2DDevice::FromD2DRenderTarget(pRenderTarget);
        if (NULL != pD2DDevice)
        {
            SetD2DDevice(pD2DDevice);
            pD2DDevice->RegisterD2DDeviceNotification(this);
        }
        SAFE_RELEASE(m_pRadialGradientBrush);
        SAFE_RELEASE(m_pGradientStops);

        HRESULT hr1 = pRenderTarget->CreateGradientStopCollection(
                                     m_pSzGradientStops,
                                     m_nGradientStopsCount,
                                     D2D1_GAMMA_2_2,
                                     D2D1_EXTEND_MODE_CLAMP,
                                     &m_pGradientStops);

        HRESULT hr2 = pRenderTarget->CreateRadialGradientBrush(
                                     D2D1::RadialGradientBrushProperties(
                                     m_gradientCenterPT,
                                     m_gradientOffsetPT,
                                     m_gradientRadiusX,
                                     m_gradientRadiusY),
                                     m_pGradientStops,
                                     &m_pRadialGradientBrush);
        return (SUCCEEDED(hr1) &&SUCCEEDED(hr2)) ? TRUE : FALSE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DRadialGradientBrush::HasCreatedBrush()
{
    return (NULL != m_pRadialGradientBrush);
}

/////////////////////////////////////////////////////////////////////////

void D2DRadialGradientBrush::GetD2DBrush(OUT ID2D1Brush **ppD2DBrush)
{
    if(NULL != ppD2DBrush && NULL != m_pRadialGradientBrush)
    {
        (*ppD2DBrush) = m_pRadialGradientBrush;
        (*ppD2DBrush)->AddRef();
    }
}

/////////////////////////////////////////////////////////////////////////

void D2DRadialGradientBrush::SetCenter(IN FLOAT centerX,IN FLOAT centerY)
{
    m_gradientCenterPT.x = centerX;
    m_gradientCenterPT.y = centerY;

    if(NULL != m_pRadialGradientBrush)
    {
        m_pRadialGradientBrush->SetCenter(m_gradientCenterPT);
    }
}
/////////////////////////////////////////////////////////////////////////

void D2DRadialGradientBrush::SetGradientOriginOffset(IN FLOAT dx,IN FLOAT dy)
{
    m_gradientOffsetPT.x = dx;
    m_gradientOffsetPT.y = dy;

    if(NULL != m_pRadialGradientBrush)
    {
        m_pRadialGradientBrush->SetGradientOriginOffset(m_gradientOffsetPT);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DRadialGradientBrush::SetRadiuXandRadiuY(IN FLOAT radiuX,IN FLOAT radiuY)
{
   
    m_gradientRadiusX = radiuX;
    m_gradientRadiusY = radiuY;

    if(NULL != m_pRadialGradientBrush)
    {
        m_pRadialGradientBrush->SetRadiusX(m_gradientRadiusX);
        m_pRadialGradientBrush->SetRadiusY(m_gradientRadiusY);
    }

}

//////////////////////////////////////////////////////////////////////////

void D2DRadialGradientBrush::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    D2DBrush::OnD2DDeviceStateChange(pDevice, stateVal);

    CreateBrush(pDevice);
}

//////////////////////////////////////////////////////////////////////////

void D2DRadialGradientBrush::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    D2DBrush::OnD2DDeviceResize(pDevice, stateVal);
}

