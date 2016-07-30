/*!
* @file SdkWICImageHelper.cpp
* 
* @brief This file defines functions to operate image file with WIC.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/03
*/

#include "stdafx.h"
#include "SdkWICImageHelper.h"

USING_NAMESPACE_UTILITIES

#ifndef TYPE_TO_TYPENAME
#define TYPE_TO_TYPENAME(type, name)    \
{                                       \
    switch(type)                        \
    {                                   \
    case WIC_IMAGE_TYPE_PNG:            \
        name = L"PNG";                  \
        break;                          \
    case WIC_IMAGE_TYPE_IMAGE:          \
        name = L"IMAGE";                \
        break;                          \
    case WIC_IMAGE_TYPE_GIF:            \
        name = L"GIF";                  \
        break;                          \
    default:                            \
        name = L"PNG";                  \
        break;                          \
    }                                   \
}
#endif // TYPE_TO_TYPENAME

IWICImagingFactory* SdkWICImageHelper::s_pImagingFactory = NULL;

HRESULT SdkWICImageHelper::WICInitialize()
{
    SAFE_RELEASE(s_pImagingFactory);

    HRESULT hr = S_OK;

    if (NULL == s_pImagingFactory)
    {
        hr = CoCreateInstance(CLSID_WICImagingFactory, NULL,
                              CLSCTX_INPROC_SERVER,
                              IID_PPV_ARGS(&s_pImagingFactory));
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

void SdkWICImageHelper::WICUninitialize()
{
    SAFE_RELEASE(s_pImagingFactory);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWICImageHelper::SaveWICBitmapToFile(PCWSTR pFileName, IWICBitmapSource *pWICBitmap)
{
    if ( (NULL == pFileName) || (NULL == pWICBitmap) || (NULL == s_pImagingFactory) )
    {
        return FALSE;
    }

    WICPixelFormatGUID format = GUID_WICPixelFormatDontCare;
    IWICStream *pStream = NULL;
    IWICBitmapEncoder *pEncoder = NULL;
    IWICBitmapFrameEncode *pFrameEncode = NULL;

    HRESULT hr = s_pImagingFactory->CreateStream(&pStream);
    if (SUCCEEDED(hr))
    {
        hr = pStream->InitializeFromFilename(pFileName, GENERIC_WRITE);
    }

    if (SUCCEEDED(hr))
    {
        hr = s_pImagingFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
    }

    if (SUCCEEDED(hr))
    {
        hr = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
    }

    if (SUCCEEDED(hr))
    {
        hr = pEncoder->CreateNewFrame(&pFrameEncode, NULL);
    }

    if (SUCCEEDED(hr))
    {
        hr = pFrameEncode->Initialize(NULL);
    }

    if (SUCCEEDED(hr))
    {
        UINT uWidth = 0;
        UINT uHeight = 0;
        pWICBitmap->GetSize(&uWidth, &uHeight);
        hr = pFrameEncode->SetSize(uWidth, uHeight);
    }

    if (SUCCEEDED(hr))
    {
        hr = pFrameEncode->SetPixelFormat(&format);
    }

    if (SUCCEEDED(hr))
    {
        hr = pFrameEncode->WriteSource(pWICBitmap, NULL);
    }

    if (SUCCEEDED(hr))
    {
        hr = pFrameEncode->Commit();
    }

    if (SUCCEEDED(hr))
    {
        hr = pEncoder->Commit();
    }

    SAFE_RELEASE(pStream);
    SAFE_RELEASE(pEncoder);
    SAFE_RELEASE(pFrameEncode);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

HBITMAP SdkWICImageHelper::CreateHBITMAPFromIWICBitmap(HDC hDC, IWICBitmapSource *pWICBitmapSource)
{
    UINT uWidth = 0;
    UINT uHeight = 0;
    if ((NULL == pWICBitmapSource) || FAILED(pWICBitmapSource->GetSize(&uWidth, &uHeight)))
    {
        return NULL;
    }

    HBITMAP hBitmap = NULL;
    UINT uStride = uWidth * 4;
    UINT pixelBufSize = uStride * uHeight;
    BYTE *pbBuffer = new BYTE[pixelBufSize];
    memset(pbBuffer, 0, pixelBufSize);

    HRESULT hr = pWICBitmapSource->CopyPixels(NULL, uStride, pixelBufSize, pbBuffer);
    if (SUCCEEDED(hr))
    {
        BITMAPINFO bmi = 
        {
            sizeof(BITMAPINFOHEADER),   // biSize
            uWidth,                     // biWidth
            -int(uHeight),              // biHeight
            1,                          // biPlanes
            32,                         // biBitCount
            BI_RGB,                     // biCompression
            pixelBufSize,               // biSizeImage
            1,                          // biXPelsPerMeter
            1,                          // biYPelsPerMeter
            0,                          // biClrUsed
            0,                          // biClrImportant
            0                           // RGB QUAD
        };

        hBitmap = CreateCompatibleBitmap(hDC, uWidth, uHeight);
        if (NULL != hBitmap)
        {
            SetDIBits(hDC, hBitmap, 0, uHeight, (PVOID)&pbBuffer[0], &bmi, DIB_RGB_COLORS);
        }
    }

    SAFE_DELETE_ARRAY(pbBuffer);

    return hBitmap;
}

//////////////////////////////////////////////////////////////////////////

SdkWICImageHelper::SdkWICImageHelper() : m_curImageType(WIC_IMAGE_TYPE_PNG),
                                         m_pConvertedSourceBitmap(NULL),
                                         m_hBitmap(NULL),
                                         m_uImageWidth(0),
                                         m_uImageHeight(0)
{
}

//////////////////////////////////////////////////////////////////////////

SdkWICImageHelper::~SdkWICImageHelper()
{
    ClearImageData();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWICImageHelper::LoadFromFile(LPCWSTR lpfile, UINT32 uDestWidth, UINT32 uDestHeight)
{
    if (NULL == s_pImagingFactory)
    {
        return FALSE;
    }

    ClearImageData();

    IWICBitmapDecoder *pWicBitmapDecoder = NULL;
    HRESULT hr = s_pImagingFactory->CreateDecoderFromFilename(
        lpfile,                             // Image to be decoded.
        NULL,                               // Do not prefer a particular vendor.
        GENERIC_READ,                       // Desired read a access to the file.
        WICDecodeMetadataCacheOnDemand,     // Cache metadata when needed.
        &pWicBitmapDecoder                  // Pointer to the decoder.
        );

    // The corresponding file can be modified, including delete, move, until IWICFormatConverter
    // pointer is deleted which is initialized with this decoder created from this corresponding file.
    if ( SUCCEEDED(hr) )
    {
        IWICBitmapFrameDecode *pFrame = NULL;
        hr = pWicBitmapDecoder->GetFrame(0, &pFrame);
        if ( SUCCEEDED(hr) )
        {
            hr = s_pImagingFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
            if ( SUCCEEDED(hr) )
            {
                // Clone a new bitmap source from a specified bitmap source so that the file can be deleted.
                IWICBitmapSource *pTempSource = pFrame;
                pTempSource->AddRef();
                //hr = CloneWICBitmapSource(pFrame, &pTempSource);
                IWICBitmapScaler *pScaler = CreateBitmapScaler(pTempSource, uDestWidth, uDestHeight);
                IWICBitmapSource *pSource = (NULL != pScaler) ?
                    ((IWICBitmapSource*)pScaler) : ((IWICBitmapSource*)pTempSource);
                if ( NULL == pSource )
                {
                    pSource = pFrame;
                }

                hr = m_pConvertedSourceBitmap->Initialize(
                    pSource,                         // Input bitmap to convert.
                    GUID_WICPixelFormat32bppPBGRA,   // Input bitmap to convert.
                    WICBitmapDitherTypeNone,         // Destination pixel format.
                    NULL,                            // Specified dither pattern.
                    0.0f,                            // Specify a particular palette.
                    WICBitmapPaletteTypeCustom);     // Palette translation type.

                SAFE_RELEASE(pScaler);
                SAFE_RELEASE(pTempSource);
            }
        }

        SAFE_RELEASE(pFrame);
        SAFE_RELEASE(pWicBitmapDecoder);
    }

    return (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWICImageHelper::LoadFromHBITMAP(HBITMAP hBitmap, UINT32 uDestWidth, UINT32 uDestHeight)
{
    if ( NULL == s_pImagingFactory )
    {
        return FALSE;
    }

    ClearImageData();

    IWICBitmap *ppIBitmap = NULL;
    HRESULT hr = s_pImagingFactory->CreateBitmapFromHBITMAP(
        hBitmap,
        NULL,
        WICBitmapUseAlpha,
        &ppIBitmap);

    if ( SUCCEEDED(hr) )
    {
        hr = s_pImagingFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
        if ( SUCCEEDED(hr) )
        {
            IWICBitmapScaler *pScaler = CreateBitmapScaler(
                ppIBitmap, uDestWidth, uDestHeight);
            IWICBitmapSource *pSource = (NULL != pScaler) ?
                ((IWICBitmapSource*)pScaler) : ((IWICBitmapSource*)ppIBitmap);

            hr = m_pConvertedSourceBitmap->Initialize(
                pSource,                         // Input bitmap to convert.
                GUID_WICPixelFormat32bppPBGRA,   // Input bitmap to convert.
                WICBitmapDitherTypeNone,         // Destination pixel format.
                NULL,                            // Specified dither pattern.
                0.0f,                            // Specify a particular palette.
                WICBitmapPaletteTypeCustom);     // Palette translation type.

            SAFE_RELEASE(pScaler);
        }
    }

    SAFE_RELEASE(ppIBitmap);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWICImageHelper::LoadFromResource(UINT uResId, HMODULE hModule, UINT32 uDestWidth, UINT32 uDestHeight)
{
    if (NULL == s_pImagingFactory)
    {
        return FALSE;
    }

    PVOID pData = NULL;
    DWORD dwSize = LoadResourceData(uResId, hModule, &pData);
    if (dwSize <= 0)
    {
        return FALSE;
    }

    ClearImageData();

    HRESULT hr = E_FAIL;
    IWICStream *pWicStream = NULL;
    IWICBitmapDecoder *pWicBitmapDecoder = NULL;
    SAFE_RELEASE(m_pConvertedSourceBitmap);
    hr = s_pImagingFactory->CreateStream(&pWicStream);
    if ( SUCCEEDED(hr) )
    {
        hr = pWicStream->InitializeFromMemory(reinterpret_cast<BYTE*>(pData), dwSize);
        if ( SUCCEEDED(hr) )
        {
            hr = s_pImagingFactory->CreateDecoderFromStream(
                pWicStream,                         // Image to be decoded.
                NULL,                               // Do not prefer a particular vendor.
                WICDecodeMetadataCacheOnDemand,     // Desired read a access to the file.
                &pWicBitmapDecoder                  // Cache metadata when needed.
                );                                  // Pointer to the decoder.

            if ( SUCCEEDED(hr) )
            {
                IWICBitmapFrameDecode *pFrame = NULL;
                hr = pWicBitmapDecoder->GetFrame(0, &pFrame);
                if ( SUCCEEDED(hr) )
                {
                    hr = s_pImagingFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
                    if ( SUCCEEDED(hr) )
                    {
                        IWICBitmapScaler *pScaler = CreateBitmapScaler(
                            pFrame,
                            uDestWidth, uDestHeight);
                        IWICBitmapSource *pSource = (NULL != pScaler) ? 
                            ((IWICBitmapSource*)pScaler) : ((IWICBitmapSource*)pFrame);

                        hr = m_pConvertedSourceBitmap->Initialize(
                            pSource,                         // Input bitmap to convert.
                            GUID_WICPixelFormat32bppPBGRA,   // Input bitmap to convert.
                            WICBitmapDitherTypeNone,         // Destination pixel format.
                            NULL,                            // Specified dither pattern.
                            0.0f,                            // Specify a particular palette.
                            WICBitmapPaletteTypeCustom);     // Palette translation type.
                    }
                }
            }
        }
    }

    SAFE_RELEASE(pWicStream);
    SAFE_RELEASE(pWicBitmapDecoder);

    return (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

HBITMAP SdkWICImageHelper::GetHBITMAP()
{
    if ( NULL != m_pConvertedSourceBitmap )
    {
        HDC hdcScreen = CreateDC(L"DISPLAY", NULL, NULL, NULL);
        m_hBitmap = CreateHBITMAPFromIWICBitmap(hdcScreen, m_pConvertedSourceBitmap);
        DeleteDC(hdcScreen);
    }

    return m_hBitmap;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWICImageHelper::GetFormatConverter(IWICFormatConverter **ppConvertedSourceBitmap)
{
    if ( NULL != ppConvertedSourceBitmap && NULL != m_pConvertedSourceBitmap )
    {
        (*ppConvertedSourceBitmap) = m_pConvertedSourceBitmap;
        SAFE_ADDREF((*ppConvertedSourceBitmap));
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SdkWICImageHelper::SetImageType(WIC_IMAGE_TYPE type)
{
    m_curImageType = type;
}

//////////////////////////////////////////////////////////////////////////

UINT SdkWICImageHelper::GetWidth()
{
    if ( NULL != m_pConvertedSourceBitmap )
    {
        m_pConvertedSourceBitmap->GetSize(&m_uImageWidth, &m_uImageHeight);
    }

    return m_uImageWidth;
}

//////////////////////////////////////////////////////////////////////////

UINT SdkWICImageHelper::GetHeight()
{
    if ( NULL != m_pConvertedSourceBitmap )
    {
        m_pConvertedSourceBitmap->GetSize(&m_uImageWidth, &m_uImageHeight);
    }

    return m_uImageHeight;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkWICImageHelper::CreateScalerWICBitmapSource(UINT32 uWidth, UINT32 uHeight)
{
    if ( (NULL == m_pConvertedSourceBitmap) || (NULL == s_pImagingFactory) )
    {
        return E_FAIL;
    }

    HRESULT hr = E_FAIL;

    IWICBitmapScaler *pScaler = CreateBitmapScaler(m_pConvertedSourceBitmap, uWidth, uHeight);
    if ( NULL != pScaler )
    {
        SAFE_RELEASE(m_pConvertedSourceBitmap);
        hr = s_pImagingFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
        if ( SUCCEEDED(hr) )
        {
            hr = m_pConvertedSourceBitmap->Initialize(
                pScaler,                         // Input bitmap to convert.
                GUID_WICPixelFormat32bppPBGRA,   // Input bitmap to convert.
                WICBitmapDitherTypeNone,         // Destination pixel format.
                NULL,                            // Specified dither pattern.
                0.0f,                            // Specify a particular palette.
                WICBitmapPaletteTypeCustom);     // Palette translation type.
        }
    }
    SAFE_RELEASE(pScaler);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkWICImageHelper::ClearImageData()
{
    SAFE_RELEASE(m_pConvertedSourceBitmap);
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkWICImageHelper::CloneWICBitmapSource(IWICBitmapSource *pWICBitmapSource, IWICBitmapSource **ppWICBitmapDest)
{
    if ( (NULL == pWICBitmapSource) || (NULL == s_pImagingFactory) || (NULL == ppWICBitmapDest))
    {
        return NULL;
    }

    HRESULT hr = E_FAIL;
    UINT width  = 0;
    UINT height = 0;
    IWICBitmap *pWICBitmap = NULL;
    IWICBitmapLock *piLock = NULL;
    WICPixelFormatGUID pixelFormat;

    hr = pWICBitmapSource->GetSize(&width, &height);
    if ( SUCCEEDED(hr) )
    {
        hr = pWICBitmapSource->GetPixelFormat(&pixelFormat);
    }

    if ( SUCCEEDED(hr) )
    {
        hr = s_pImagingFactory->CreateBitmap(
            width,
            height,
            pixelFormat,
            WICBitmapCacheOnDemand,
            &pWICBitmap);
    }

    // Copy pixels data.
    if ( SUCCEEDED(hr) )
    {
        UINT cbBufferSize = 0;
        UINT cbStride = 0;
        BYTE *pv = NULL;        // This pointer is no necessary to release.
        WICRect rcLock = { 0, 0, width, height };

        hr = pWICBitmap->Lock(&rcLock, WICBitmapLockWrite, &piLock);
        if ( SUCCEEDED(hr) )
        {
            hr = piLock->GetStride(&cbStride);
        }

        if ( SUCCEEDED(hr) )
        {
            hr = piLock->GetDataPointer(&cbBufferSize, &pv);
        }

        if ( SUCCEEDED(hr) )
        {
            hr = pWICBitmapSource->CopyPixels(&rcLock, cbStride, cbBufferSize, pv);
        }

        if ( SUCCEEDED(hr) )
        {
            (*ppWICBitmapDest) = pWICBitmap;
            (*ppWICBitmapDest)->AddRef();
        }
    }

    // Copy the palette.
    if ( SUCCEEDED(hr) )
    {
        IWICPalette *pWICPalette = NULL;
        hr = s_pImagingFactory->CreatePalette(&pWICPalette);
        if ( SUCCEEDED(hr) )
        {
            hr = pWICBitmapSource->CopyPalette(pWICPalette);
        }

        if ( SUCCEEDED(hr) )
        {
            hr = pWICBitmap->SetPalette(pWICPalette);
        }

        SAFE_RELEASE(pWICPalette);
    }

    SAFE_RELEASE(piLock);
    SAFE_RELEASE(pWICBitmap);

    return hr;
}

//////////////////////////////////////////////////////////////////////////

void SdkWICImageHelper::GetImageInfos()
{
    GetWidth();
    GetHeight();
}

//////////////////////////////////////////////////////////////////////////

DWORD SdkWICImageHelper::LoadResourceData(UINT uResId, HMODULE hModule, OUT PVOID* pData)
{
    LPCTSTR lpName = MAKEINTRESOURCE(uResId);
    if ( !IS_INTRESOURCE(lpName) )
    {
        return 0;
    }

    //HMODULE hInst = ::FrameworkCommon::GetCurrentModuleHandle();
    HMODULE hResInst = (NULL == hModule) ? GetModuleHandle(NULL) : hModule;
    if ( NULL == hResInst )
    {
        return 0;
    }

    WCHAR *pTypeName = NULL;
    TYPE_TO_TYPENAME(m_curImageType, pTypeName);
    HRSRC hrSrc = FindResource(hResInst, lpName, pTypeName);
    if ( NULL == hrSrc )
    {
        return 0;
    }

    DWORD dwSize = SizeofResource(hResInst, hrSrc);
    if ( dwSize <= 0 )
    {
        return 0;
    }

    HGLOBAL hGlobal = LoadResource(hResInst, hrSrc);
    *pData = (BYTE*)LockResource(hGlobal);

    return dwSize;
}

//////////////////////////////////////////////////////////////////////////

IWICBitmapScaler* SdkWICImageHelper::CreateBitmapScaler(IN IWICBitmapSource* pWICBitmapSource, UINT32 uDestWidth, UINT32 uDestHeight)
{
    if ( (NULL == pWICBitmapSource) || (0 == uDestHeight && 0 == uDestWidth) )
    {
        return NULL;
    }

    UINT32 oriWidth  = 0;
    UINT32 oriHeight = 0;
    pWICBitmapSource->GetSize(&oriWidth, &oriHeight);

    uDestWidth  = (0 == uDestWidth) ? oriWidth : uDestWidth;
    uDestHeight = (0 == uDestHeight) ? oriHeight : uDestHeight;

    IWICBitmapScaler *pBitmapScaler = NULL;
    HRESULT hr = s_pImagingFactory->CreateBitmapScaler(&pBitmapScaler);
    if ( SUCCEEDED(hr) )
    {
        hr = pBitmapScaler->Initialize(
            pWICBitmapSource,
            uDestWidth,
            uDestHeight,
            WICBitmapInterpolationModeCubic);

        if ( SUCCEEDED(hr) )
        {
            return pBitmapScaler;
        }
    }

    SAFE_RELEASE(pBitmapScaler);

    return NULL;
}