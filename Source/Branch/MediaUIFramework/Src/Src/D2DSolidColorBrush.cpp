/*!
* @file D2DSolidColorBrush.cpp
* 
* @brief The file defines D2DSolidColorBrush class, inherited from D2DBrush.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#include "D2DSolidColorBrush.h"

D2DSolidColorBrush::D2DSolidColorBrush() : m_pSolidColorBrush(NULL)
{
}

//////////////////////////////////////////////////////////////////////////

D2DSolidColorBrush::~D2DSolidColorBrush()
{
    SAFE_RELEASE(m_pSolidColorBrush);

    D2DDevice *pD2DDevice = GetD2DDevice();
    if (NULL != pD2DDevice)
    {
        pD2DDevice->UnregisterD2DDeviceNotification(this);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DSolidColorBrush::CreateBrush(D2DDevice *pD2DDevice)
{
    BOOL retVal = FALSE;

    if (NULL != pD2DDevice)
    {
        ID2D1RenderTarget *pRenderTarget = NULL;
        pD2DDevice->GetRenderTarget(&pRenderTarget);
        retVal = CreateBrush(pRenderTarget);
        SAFE_RELEASE(pRenderTarget);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DSolidColorBrush::CreateBrush(IN ID2D1RenderTarget *pRenderTarget)
{
    if (NULL != pRenderTarget)
    {
        D2DDevice *pD2DDevice = D2DDevice::FromD2DRenderTarget(pRenderTarget);
        if (NULL != pD2DDevice)
        {
            SetD2DDevice(pD2DDevice);
            pD2DDevice->RegisterD2DDeviceNotification(this);
        }

        SAFE_RELEASE(m_pSolidColorBrush);
        HRESULT hr = pRenderTarget->CreateSolidColorBrush(ColorF(0, 0, 0), &m_pSolidColorBrush);
        return SUCCEEDED(hr) ? TRUE : FALSE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DSolidColorBrush::HasCreatedBrush()
{
    return (NULL != m_pSolidColorBrush);
}

//////////////////////////////////////////////////////////////////////////

void D2DSolidColorBrush::GetD2DBrush(OUT ID2D1Brush **ppD2DBrush)
{
    if ( (NULL != ppD2DBrush) && (NULL != m_pSolidColorBrush) )
    {
        (*ppD2DBrush) = m_pSolidColorBrush;
        (*ppD2DBrush)->AddRef();
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DSolidColorBrush::SetColor(IN const D2D1_COLOR_F &color)
{
    if (NULL != m_pSolidColorBrush)
    {
        m_pSolidColorBrush->SetColor(color);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DSolidColorBrush::GetColor(OUT D2D1_COLOR_F *color)
{
    if (NULL != m_pSolidColorBrush && NULL != color)
    {
        *color = m_pSolidColorBrush->GetColor();
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DSolidColorBrush::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    D2DBrush::OnD2DDeviceStateChange(pDevice, stateVal);

    CreateBrush(pDevice);
}

//////////////////////////////////////////////////////////////////////////

void D2DSolidColorBrush::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    D2DBrush::OnD2DDeviceResize(pDevice, stateVal);
}
