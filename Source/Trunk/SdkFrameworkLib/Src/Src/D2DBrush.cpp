/*!
* @file D2DBrush.cpp 
* 
* @brief This file defines the class D2DBrush.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#include "stdafx.h"
#include "D2DBrush.h"

USING_NAMESPACE_D2D

D2DBrush::D2DBrush()
{
}

//////////////////////////////////////////////////////////////////////////

D2DBrush::~D2DBrush()
{
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBrush::CreateBrush(D2DDevice *pD2DDevice)
{
    UNREFERENCED_PARAMETER(pD2DDevice);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBrush::CreateBrush(IN ID2D1RenderTarget *pRenderTarget)
{
    UNREFERENCED_PARAMETER(pRenderTarget);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DBrush::HasCreatedBrush()
{
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void D2DBrush::SetColor(IN const D2D1_COLOR_F &color)
{
    UNREFERENCED_PARAMETER(color);
}

//////////////////////////////////////////////////////////////////////////

void D2DBrush::GetColor(OUT D2D1_COLOR_F *color)
{
    UNREFERENCED_PARAMETER(color);
}

//////////////////////////////////////////////////////////////////////////

void D2DBrush::SetOpacity(FLOAT opacity)
{
    UNREFERENCED_PARAMETER(opacity);
}

//////////////////////////////////////////////////////////////////////////

FLOAT D2DBrush::GetOpacity()
{
    return 1.0f;
}

//////////////////////////////////////////////////////////////////////////

void D2DBrush::GetD2DBrush(OUT ID2D1Brush **ppD2DBrush)
{
    UNREFERENCED_PARAMETER(ppD2DBrush);
}

//////////////////////////////////////////////////////////////////////////

void D2DBrush::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);
}

//////////////////////////////////////////////////////////////////////////

void D2DBrush::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);
}
