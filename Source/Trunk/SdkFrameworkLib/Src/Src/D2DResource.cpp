/*!
* @file D2DResource.cpp
* 
* @brief This file defines class D2DResource, which is the base class of all D2D resources class,
*        such as Bitmap, Brush, Layer, ,etc.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/27
*/

#include "stdafx.h"
#include "D2DResource.h"
#include "D2DDevice.h"

USING_NAMESPACE_D2D

D2DResource::D2DResource() : m_pD2DDevice(NULL)
{
}

//////////////////////////////////////////////////////////////////////////

D2DResource::~D2DResource()
{
}

//////////////////////////////////////////////////////////////////////////

void D2DResource::SetD2DDevice(IN D2DDevice *pD2DDevice)
{
    m_pD2DDevice = pD2DDevice;
}

//////////////////////////////////////////////////////////////////////////

D2DDevice* D2DResource::GetD2DDevice() const
{
    return m_pD2DDevice;
}
