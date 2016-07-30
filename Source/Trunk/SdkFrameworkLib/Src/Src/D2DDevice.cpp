/*!
* @file D2DDevice.cpp
* 
* @brief The file defines the class D2DDevice, which wraps the D2D device, such as Render Target, etc.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#include "stdafx.h"
#include "D2DDevice.h"
#include "SdkCommonInclude.h"
#include "ID2DDeviceStateChange.h"

USING_NAMESPACE_D2D

vector<D2DDevice*> D2DDevice::s_vctD2DDeviceList;

//////////////////////////////////////////////////////////////////////////

D2DDevice* D2DDevice::FromD2DRenderTarget(ID2D1RenderTarget *pRenderTarget)
{
    D2DDevice *pRetD2DDevice = NULL;
    int nSize = (int)s_vctD2DDeviceList.size();

    for (int i = 0; i < nSize; ++i)
    {
        D2DDevice *pD2DDevice = s_vctD2DDeviceList[i];
        if ( (pD2DDevice->m_pRenderTarget == pRenderTarget) || 
             (pD2DDevice->m_pWICBitmapRenderTarget == pRenderTarget) )
        {
            pRetD2DDevice = pD2DDevice;
            break;
        }
    }

    return pRetD2DDevice;
}

//////////////////////////////////////////////////////////////////////////

D2DDevice::D2DDevice() : m_hMemDC(NULL),
                         m_hWnd(NULL),
                         m_fOpacity(1.0f),
                         m_pD2DFactory(NULL),
                         m_pWICBitmap(NULL),
                         m_pRenderTarget(NULL),
                         m_pDCRenderTarget(NULL),
                         m_pWICBitmapRenderTarget(NULL),
                         m_pDWriteBitmapTarget(NULL),
                         m_isPaintModeChange(FALSE),
                         m_paintTargetType(DEVICE_TARGET_TYPE_NONE)
{
    AddD2DDeviceToList(this);

    CreateFactory();
}

//////////////////////////////////////////////////////////////////////////

D2DDevice::~D2DDevice()
{
    SAFE_RELEASE(m_pD2DFactory);
    SAFE_RELEASE(m_pRenderTarget);
    SAFE_RELEASE(m_pDCRenderTarget);
    SAFE_RELEASE(m_pWICBitmap);
    SAFE_RELEASE(m_pWICBitmapRenderTarget);
    SAFE_RELEASE(m_pDWriteBitmapTarget);

    SAFE_DELETE_DC(m_hMemDC);

    RemoveD2DDeviceFromList(this);
}

//////////////////////////////////////////////////////////////////////////

HRESULT D2DDevice::InitHwndDevice(HWND hWnd)
{
    HRESULT hr = (NULL != m_pD2DFactory) ? S_OK : E_FAIL;

    if ( SUCCEEDED(hr) )
    {
        if ( (NULL == m_pRenderTarget) && IsWindow(hWnd) )
        {
            RECT rc = { 0 };
            GetClientRect(hWnd, &rc);

            m_hWnd = hWnd;
            D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

            hr = m_pD2DFactory->CreateHwndRenderTarget(
                RenderTargetProperties(),
                HwndRenderTargetProperties(hWnd, size),
                &m_pRenderTarget);
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT D2DDevice::InitMemDevice(HDC hdc)
{
    HRESULT hr = (NULL != m_pD2DFactory) ? S_OK : E_FAIL;

    if ( SUCCEEDED(hr) )
    {
        if (NULL == m_pWICBitmapRenderTarget)
        {
            SAFE_RELEASE(m_pWICBitmap);
            SAFE_DELETE_DC(m_hMemDC);

            hr = CreateWICBitmapRenderTarget(
                hdc,
                m_hMemDC,
                &m_pWICBitmapRenderTarget,
                &m_pWICBitmap);
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT D2DDevice::InitDCDevice()
{
    HRESULT hr = (NULL != m_pD2DFactory) ? S_OK : E_FAIL;

    if ( SUCCEEDED(hr) )
    {
        if (NULL == m_pDCRenderTarget)
        {
            D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
                D2D1_RENDER_TARGET_TYPE_DEFAULT,
                D2D1::PixelFormat(
                DXGI_FORMAT_B8G8R8A8_UNORM,
                D2D1_ALPHA_MODE_PREMULTIPLIED),
                0,
                0,
                D2D1_RENDER_TARGET_USAGE_NONE
                );

            hr = m_pD2DFactory->CreateDCRenderTarget(&props, &m_pDCRenderTarget);
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::GetDesktopDpi(OUT FLOAT *dpiX, OUT FLOAT *dpiY)
{
    if (NULL != m_pD2DFactory)
    {
        m_pD2DFactory->GetDesktopDpi(dpiX, dpiY);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::SetOpacity(FLOAT fOpacity)
{
    m_fOpacity = fOpacity;
}

//////////////////////////////////////////////////////////////////////////

FLOAT D2DDevice::GetOpacity() const
{
    return m_fOpacity;
}

//////////////////////////////////////////////////////////////////////////

HRESULT D2DDevice::GetD2DFactory(OUT ID2D1Factory **ppD2DFactory)
{
    HRESULT hr = S_OK;

    if (NULL == m_pD2DFactory)
    {
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    }

    if (NULL != m_pD2DFactory && NULL != ppD2DFactory)
    {
        (*ppD2DFactory) = m_pD2DFactory;
        SAFE_ADDREF((*ppD2DFactory));
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HBITMAP D2DDevice::GetMemHBITMAP(OUT UINT32 *pWidth, OUT UINT32 *pHeight)
{
    HBITMAP hBitmap = SdkWICImageHelper::CreateHBITMAPFromIWICBitmap(m_hMemDC, m_pWICBitmap);

    if (NULL != m_pWICBitmap)
    {
        m_pWICBitmap->GetSize(pWidth, pHeight);
    }

    return hBitmap;
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::GetRenderTarget(OUT ID2D1RenderTarget **ppTarget)
{
    ID2D1RenderTarget *pTempRenderTarget = NULL;

    switch (m_paintTargetType)
    {
    case DEVICE_TARGET_TYPE_HWND:
        pTempRenderTarget = m_pRenderTarget;
        break;

    case DEVICE_TARGET_TYPE_MEMORY:
        pTempRenderTarget = m_pWICBitmapRenderTarget;
        break;

    case DEVICE_TARGET_TYPE_DC:
        pTempRenderTarget = m_pDCRenderTarget;
        break;
    }

    if (NULL != pTempRenderTarget)
    {
        (*ppTarget) = pTempRenderTarget;
        SAFE_ADDREF((*ppTarget));
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::BeginDraw(HDC hDC, const LPRECT lpRect, BOOL isClearRT)
{
    switch (m_paintTargetType)
    {
    case DEVICE_TARGET_TYPE_HWND:
        {
            if ( SUCCEEDED(InitHwndDevice(m_hWnd)) && (NULL != m_pRenderTarget) )
            {
                PerformDeviceChangeNotify(DEVICE_STATECHANGE_VALUE_CHANGE);
                m_pRenderTarget->BeginDraw();
                if (isClearRT)
                {
                    m_pRenderTarget->Clear(D2D1::ColorF(0, 1.0f));
                }
            }
        }
        break;

    case DEVICE_TARGET_TYPE_MEMORY:
        {
            if ( SUCCEEDED(InitMemDevice(hDC)) && (NULL != m_pWICBitmapRenderTarget) )
            {
                PerformDeviceChangeNotify(DEVICE_STATECHANGE_VALUE_CHANGE);
                m_pWICBitmapRenderTarget->BeginDraw();
                if (isClearRT)
                {
                    m_pWICBitmapRenderTarget->Clear(D2D1::ColorF(0, m_fOpacity));
                }
            }
        }
        break;

    case DEVICE_TARGET_TYPE_DC:
        {
            if ( SUCCEEDED(InitDCDevice() && (NULL != m_pDCRenderTarget)))
            {
                PerformDeviceChangeNotify(DEVICE_STATECHANGE_VALUE_CHANGE);
                if (NULL == lpRect)
                {
                    RECT rcPaint = { 0 };
                    GetClientRect(m_hWnd, &rcPaint);
                    m_pDCRenderTarget->BindDC(hDC, &rcPaint);
                }
                else
                {
                    m_pDCRenderTarget->BindDC(hDC, lpRect);
                }

                m_pDCRenderTarget->BeginDraw();
                if (isClearRT)
                {
                    m_pDCRenderTarget->Clear(D2D1::ColorF(0, m_fOpacity));
                }
            }
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::EndDraw()
{
    ID2D1RenderTarget *pRenderTarget = NULL;
    switch (m_paintTargetType)
    {
    case DEVICE_TARGET_TYPE_HWND:
        pRenderTarget = m_pRenderTarget;
        break;

    case DEVICE_TARGET_TYPE_MEMORY:
        pRenderTarget = m_pWICBitmapRenderTarget;
        break;

    case DEVICE_TARGET_TYPE_DC:
        pRenderTarget = m_pDCRenderTarget;
        break;
    }

    HRESULT hr = (NULL != pRenderTarget) ? pRenderTarget->EndDraw() : E_FAIL;
    if (D2DERR_RECREATE_TARGET == hr)
    {
        SAFE_RELEASE(m_pRenderTarget);
        SAFE_RELEASE(m_pWICBitmapRenderTarget);
        SAFE_RELEASE(m_pDCRenderTarget);
        PerformDeviceChangeNotify(DEVICE_STATECHANGE_VALUE_RESIZE);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::SetPaintTargetType(DEVICE_TARGET_TYPE paintMode)
{
    m_isPaintModeChange = (DEVICE_TARGET_TYPE_NONE != m_paintTargetType) &&
                          (m_paintTargetType != paintMode);

    if (m_paintTargetType != paintMode)
    {
        m_paintTargetType = paintMode;
    }
}

//////////////////////////////////////////////////////////////////////////

DEVICE_TARGET_TYPE D2DDevice::GetPaintTargetType()
{
    return m_paintTargetType;
}

//////////////////////////////////////////////////////////////////////////

D2D1_SIZE_F D2DDevice::GetSize()
{
    if (NULL != m_pRenderTarget)
    {
        return m_pRenderTarget->GetSize();
    }

    return D2D_SIZE_F();
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::Resize(UINT32 w, UINT32 h)
{
    if (DEVICE_TARGET_TYPE_HWND == m_paintTargetType)
    {
        if (NULL != m_pRenderTarget)
        {
            D2D1_SIZE_U size = { w, h };
            m_pRenderTarget->Resize(size);
            m_isPaintModeChange = TRUE;
            PerformDeviceChangeNotify(DEVICE_STATECHANGE_VALUE_RESIZE);
            m_isPaintModeChange = FALSE;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DDevice::IsWindowOccluded()
{
    if ( (DEVICE_TARGET_TYPE_HWND == m_paintTargetType) && 
        (NULL != m_pRenderTarget) )
    {
        return (D2D1_WINDOW_STATE_OCCLUDED & m_pRenderTarget->CheckWindowState());
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::RegisterD2DDeviceNotification(ID2DDeviceStateChange *pNotification)
{
    BOOL isExist = FALSE;

    for each (ID2DDeviceStateChange *pItem in m_vctDeviceChangeListeners)
    {
        if (pNotification == pItem)
        {
            isExist = TRUE;
            break;
        }
    }

    if (!isExist && NULL != pNotification)
    {
        m_vctDeviceChangeListeners.push_back(pNotification);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::UnregisterD2DDeviceNotification(ID2DDeviceStateChange *pNotification)
{
    int nIndex = -1;
    int nSize = (int)m_vctDeviceChangeListeners.size();

    for (int i = 0; i < nSize; ++i)
    {
        if (pNotification == m_vctDeviceChangeListeners[i])
        {
            nIndex = i;
            break;
        }
    }

    if (nIndex >= 0)
    {
        m_vctDeviceChangeListeners.erase(m_vctDeviceChangeListeners.begin() + nIndex);
    }
}

//////////////////////////////////////////////////////////////////////////

HRESULT D2DDevice::CreateFactory()
{
    SAFE_RELEASE(m_pD2DFactory);
    return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
}

//////////////////////////////////////////////////////////////////////////

HRESULT D2DDevice::CreateWICBitmapRenderTarget(IN HDC hDC, OUT HDC& hOutMemDC, OUT ID2D1RenderTarget **ppTarget, OUT IWICBitmap **ppWICBitmap)
{
    RECT rc = { 0 };
    GetClientRect(m_hWnd, &rc);
    UINT uWidth = rc.right;
    UINT uHeight = rc.bottom;

    SAFE_RELEASE(m_pDWriteBitmapTarget);
    IDWriteFactory *pDWriteFactory = NULL;
    IDWriteGdiInterop *pDWriteGdiInterop = NULL;
    ID2D1RenderTarget *pTarget = NULL;
    IWICImagingFactory *pWICFactory = NULL;
    IWICBitmap *pWICBitmap = NULL;

    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pWICFactory));

    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
                                 __uuidof(IDWriteFactory),
                                 reinterpret_cast<IUnknown**>(&pDWriteFactory));
        if (SUCCEEDED(hr))
        {
            hr = pWICFactory->CreateBitmap(
                uWidth,
                uHeight,
                GUID_WICPixelFormat32bppPBGRA,
                WICBitmapCacheOnLoad,
                &pWICBitmap);
        }
    }

    if (SUCCEEDED(hr))
    {
        D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
                                                    D2D1_RENDER_TARGET_TYPE_DEFAULT,
                                                    D2D1::PixelFormat(
                                                    DXGI_FORMAT_B8G8R8A8_UNORM,
                                                    D2D1_ALPHA_MODE_PREMULTIPLIED),
                                                    0,
                                                    0,
                                                    D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE
                                                    );
        hr = m_pD2DFactory->CreateWicBitmapRenderTarget(pWICBitmap, props, &pTarget);
        if (SUCCEEDED(hr))
        {
            (*ppTarget) = pTarget;
            (*ppTarget)->AddRef();

            (*ppWICBitmap) = pWICBitmap;
            (*ppWICBitmap)->AddRef();
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = pDWriteFactory->GetGdiInterop(&pDWriteGdiInterop);
        if (SUCCEEDED(hr))
        {
            hr = pDWriteGdiInterop->CreateBitmapRenderTarget(
                hDC,
                uWidth,
                uHeight,
                &m_pDWriteBitmapTarget);

            if (SUCCEEDED(hr))
            {
                hOutMemDC = m_pDWriteBitmapTarget->GetMemoryDC();
            }
        }
    }

    SAFE_RELEASE(pDWriteFactory);
    SAFE_RELEASE(pDWriteGdiInterop);
    SAFE_RELEASE(pWICFactory);
    SAFE_RELEASE(pTarget);
    SAFE_RELEASE(pWICBitmap);

    return hr;
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::PerformDeviceChangeNotify(DEVICE_STATECHANGE_VALUE val)
{
    if (!m_isPaintModeChange || (DEVICE_STATECHANGE_VALUE_NONE == val))
    {
        return;
    }

    if (DEVICE_STATECHANGE_VALUE_RESIZE == val)
    {
        for each (ID2DDeviceStateChange *pItem in m_vctDeviceChangeListeners)
        {
            if (NULL != pItem)
            {
                pItem->OnD2DDeviceResize(this, val);
            }
        }
    }
    else
    {
        for each (ID2DDeviceStateChange *pItem in m_vctDeviceChangeListeners)
        {
            if (NULL != pItem)
            {
                pItem->OnD2DDeviceStateChange(this, val);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::AddD2DDeviceToList(D2DDevice *pD2DDevice)
{
    s_vctD2DDeviceList.push_back(pD2DDevice);
}

//////////////////////////////////////////////////////////////////////////

void D2DDevice::RemoveD2DDeviceFromList(D2DDevice *pD2DDevice)
{
    int nSize = (int)s_vctD2DDeviceList.size();
    int nIndex = -1;

    for (int i = 0; i < nSize; ++i)
    {
        if (pD2DDevice == s_vctD2DDeviceList[i])
        {
            nIndex = i;
            break;
        }
    }

    if (nIndex >= 0)
    {
        s_vctD2DDeviceList.erase(s_vctD2DDeviceList.begin() + nIndex);
    }
}
