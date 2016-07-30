/*!
* @file WICAnimatedGif.cpp 
* 
* @brief The implementation of functions defined in WICAnimatedGif class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/03
*/

#include "WICAnimatedGif.h"

USING_NAMESPACE_COMMON

#define GIF_PATH_GLOBALCOLORTABLEFLAG           L"/logscrdesc/GlobalColorTableFlag"
#define GIF_PATH_BACKGROUNDCOLORINDEX           L"/logscrdesc/BackgroundColorIndex"
#define GIF_PATH_PIXELASPECTRATIO               L"/logscrdesc/PixelAspectRatio"
#define GIF_PATH_GLOBAL_WIDTH                   L"/logscrdesc/Width"
#define GIF_PATH_GLOBAL_HEIGHT                  L"/logscrdesc/Height"
#define GIF_PATH_GLOBAL_APPLICATION             L"/appext/Application"
#define GIF_PATH_GLOBAL_DATA                    L"/appext/Data"
#define GIF_PATH_FRAME_LEFT                     L"/imgdesc/Left"
#define GIF_PATH_FRAME_TOP                      L"/imgdesc/Top"
#define GIF_PATH_FRAME_WIDTH                    L"/imgdesc/Width"
#define GIF_PATH_FRAME_HEIGHT                   L"/imgdesc/Height"
#define GIF_PATH_FRAME_DELAY                    L"/grctlext/Delay"
#define GIF_PATH_FRAME_DISPOSAL                 L"/grctlext/Disposal"


#ifndef TYPE_TO_TYPENAME
#define TYPE_TO_TYPENAME(type, name)        \
{                                           \
    switch(type)                            \
    {                                       \
    case GIF_RECT_VALUETYPE_LEFT:           \
        name = GIF_PATH_FRAME_LEFT;         \
        break;                              \
    case GIF_RECT_VALUETYPE_TOP:            \
        name = GIF_PATH_FRAME_TOP;          \
        break;                              \
    case GIF_RECT_VALUETYPE_RIGHT:          \
        name = GIF_PATH_FRAME_WIDTH;        \
        break;                              \
    case GIF_RECT_VALUETYPE_BOTTOM:         \
        name = GIF_PATH_FRAME_HEIGHT;       \
        break;                              \
    default:                                \
        name = L"";                         \
    }                                       \
}
#endif // TYPE_TO_TYPENAME

#ifndef TYPE_TO_GIFTYPENAME
#define TYPE_TO_GIFTYPENAME(type, name)    \
{                                          \
    switch(type)                           \
    {                                      \
    case WIC_GIF_TYPE_GIF:                 \
        name = L"GIF";                     \
        break;                             \
    case WIC_GIF_TYPE_IMAGE:               \
        name = L"PNG";                     \
        break;                             \
    case WIC_GIF_TYPE_PNG:                 \
        name = L"PNG";                     \
        break;                             \
    case WIC_GIF_TYPE_BITMAP:              \
        name = L"BMP";                     \
        break;                             \
    default:                               \
        name = L"PNG";                     \
        break;                             \
    }                                      \
}
#endif // TYPE_TO_GIFTYPENAME

IWICImagingFactory* WICAnimatedGif::s_pImagingFactory = NULL;

HRESULT WICAnimatedGif::WICInitialize()
{
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

void WICAnimatedGif::WICUninitialize()
{
    SAFE_RELEASE(s_pImagingFactory);
}

//////////////////////////////////////////////////////////////////////////

WICAnimatedGif::WICAnimatedGif() : m_pWicBitmapDecoder(NULL),
                                   m_pFormatConverter(NULL),
                                   m_uFrameDisposal(DM_NONE),
                                   m_uNextFrameIndex(0),
                                   m_uTotalLoopCount(0),
                                   m_uLoopNumber(0),
                                   m_uFrameDelay(0),
                                   m_cxGifImagePixel(0),
                                   m_cyGifImagePixel(0),
                                   m_uFrameCount(0),
                                   m_bkColorRGB(0),
                                   m_bkColorAlpha(1.0f),
                                   m_fHasLoop(FALSE),
                                   m_curGifType(WIC_GIF_TYPE_GIF)
{
    SetRect(&m_frameRect, 0, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////

WICAnimatedGif::~WICAnimatedGif()
{
    SAFE_RELEASE(m_pWicBitmapDecoder);
    SAFE_RELEASE(m_pFormatConverter);
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::LoadFromFile(LPCWSTR lpfile)
{
    if (NULL == s_pImagingFactory)
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    SAFE_RELEASE(m_pWicBitmapDecoder);
    hr = s_pImagingFactory->CreateDecoderFromFilename(
                            lpfile,                             // Image to be decoded.
                            NULL,                               // Do not prefer a particular vendor.
                            GENERIC_READ,                       // Desired read a access to the file.
                            WICDecodeMetadataCacheOnDemand,     // Cache metadata when needed.
                            &m_pWicBitmapDecoder                // Pointer to the decoder.
                            );

    if (SUCCEEDED(hr))
    {
        hr = m_pWicBitmapDecoder->GetFrameCount(&m_uFrameCount);
        if (IsGif(m_pWicBitmapDecoder))
        {
            hr = (TRUE == GetGlobalMetaData()) ? S_OK : E_FAIL;
        }
        else
        {
            if (m_uFrameCount > 1)
            {
                m_uFrameCount = 1;
            }
            IWICBitmapFrameDecode *pFrameDecode = NULL;
            hr = m_pWicBitmapDecoder->GetFrame(0, &pFrameDecode);
            if (SUCCEEDED(hr))
            {
                pFrameDecode->GetSize(&m_cxGifImagePixel, &m_cyGifImagePixel);
            }
            SAFE_RELEASE(pFrameDecode);
        }
    }

    return (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::LoadFromResource(UINT uResId, HMODULE hModule)
{
    if (NULL == s_pImagingFactory)
    {
        return E_FAIL;
    }

    PVOID pData = NULL;
    DWORD dwSize = LoadResourceData(uResId, hModule, &pData);
    if (dwSize <= 0)
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    IWICStream *pWicStream = NULL;
    hr = s_pImagingFactory->CreateStream(&pWicStream);
    if (SUCCEEDED(hr))
    {
        hr = pWicStream->InitializeFromMemory(reinterpret_cast<BYTE*>(pData), dwSize);
        if (SUCCEEDED(hr))
        {
            SAFE_RELEASE(m_pWicBitmapDecoder);
            hr = s_pImagingFactory->CreateDecoderFromStream(
                                    pWicStream,                         // Image to be decoded.
                                    NULL,                               // Do not prefer a particular vendor.
                                    WICDecodeMetadataCacheOnDemand,     // Desired read a access to the file.
                                    &m_pWicBitmapDecoder                // Cache metadata when needed.
                                    );                                  // Pointer to the decoder.
            if (SUCCEEDED(hr))
            {
                hr = m_pWicBitmapDecoder->GetFrameCount(&m_uFrameCount);
                if (IsGif(m_pWicBitmapDecoder))
                {
                    hr = (TRUE == GetGlobalMetaData()) ? S_OK : E_FAIL;
                }
                else
                {
                    IWICBitmapFrameDecode *pFrameDecode = NULL;
                    hr = m_pWicBitmapDecoder->GetFrame(0, &pFrameDecode);
                    if (SUCCEEDED(hr))
                    {
                        pFrameDecode->GetSize(&m_cxGifImagePixel, &m_cyGifImagePixel);
                    }
                    SAFE_RELEASE(pFrameDecode);
                }
            }
        }
    }

    SAFE_RELEASE(pWicStream);

    return (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::IsGif(IWICBitmapDecoder *pWicBitmapDecoder)
{
    if (NULL == pWicBitmapDecoder)
    {
        return FALSE;
    }

    IWICMetadataQueryReader *pMetadataQueryReader = NULL;
    pWicBitmapDecoder->GetMetadataQueryReader(&pMetadataQueryReader);
    if (NULL == pMetadataQueryReader)
    {
        return FALSE;
    }

    SAFE_RELEASE(pMetadataQueryReader);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void WICAnimatedGif::SetImageType(WIC_GIF_TYPE type)
{
    m_curGifType = type;
}

//////////////////////////////////////////////////////////////////////////

WIC_GIF_TYPE WICAnimatedGif::GetImageType()
{
    return m_curGifType;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::GetFormatConverter(OUT IWICFormatConverter **ppConvertedSourceBitmap)
{
    if (NULL != ppConvertedSourceBitmap && NULL != m_pFormatConverter)
    {
        (*ppConvertedSourceBitmap) = m_pFormatConverter;
        (*ppConvertedSourceBitmap)->AddRef();
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::GetNextFrame(OUT IWICFormatConverter **ppConvertedSourceBitmap)
{
    BOOL retVal = GetFrameAt(m_uNextFrameIndex, ppConvertedSourceBitmap);
    if (retVal)
    {
        // If the frame index becomes zero, the loop number increases one.
        if (0 == m_uNextFrameIndex)
        {
            m_uLoopNumber++;
        }

        m_uNextFrameIndex = (++m_uNextFrameIndex) % m_uFrameCount;
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::GetFrameAt(UINT uFrameIndex, OUT IWICFormatConverter **ppConvertedSourceBitmap)
{
    if (NULL != ppConvertedSourceBitmap)
    {
        // If the frame index is larger than frame count.
        if (uFrameIndex > m_uFrameCount)
        {
            return FALSE;
        }

        GetRawFrame(uFrameIndex);
        if (NULL != m_pFormatConverter)
        {
            (*ppConvertedSourceBitmap) = m_pFormatConverter;
            (*ppConvertedSourceBitmap)->AddRef();
        }

        return (NULL != m_pFormatConverter);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::GetFrameRect(OUT RECT& rc)
{
    rc = m_frameRect;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

UINT WICAnimatedGif::GetFrameDelay()
{
    return m_uFrameDelay;
}

//////////////////////////////////////////////////////////////////////////

UINT WICAnimatedGif::GetFrameCount()
{
    return m_uFrameCount;
}

//////////////////////////////////////////////////////////////////////////

void WICAnimatedGif::GetBackgroundColor(OUT UINT &rgb, OUT FLOAT &alpha)
{
    rgb     = m_bkColorRGB;
    alpha   = m_bkColorAlpha;
}

//////////////////////////////////////////////////////////////////////////

void WICAnimatedGif::GetSize(OUT UINT &uWidth, OUT UINT &uHeight)
{
    uWidth  = m_cxGifImagePixel;
    uHeight = m_cyGifImagePixel;
}

//////////////////////////////////////////////////////////////////////////

UINT WICAnimatedGif::GetFrameIndex()
{
    return m_uNextFrameIndex;
}

//////////////////////////////////////////////////////////////////////////

void WICAnimatedGif::SetFrameIndex(UINT uNextFrameIndex)
{
    if (uNextFrameIndex >= 0 && uNextFrameIndex < m_uFrameCount)
    {
        m_uNextFrameIndex = uNextFrameIndex;
    }
}

//////////////////////////////////////////////////////////////////////////

UINT WICAnimatedGif::GetFrameDisposal()
{
    return m_uFrameDisposal;
}

//////////////////////////////////////////////////////////////////////////

HRESULT WICAnimatedGif::CreateScalerWICAnimatedGifSource(UINT32 uWidth, UINT32 uHeight)
{
    if ( (NULL == m_pFormatConverter) || (NULL == s_pImagingFactory) )
    {
        return E_FAIL;
    }

    HRESULT hr = E_FAIL;

    IWICBitmapScaler *pScaler = CreateBitmapScaler(m_pFormatConverter, uWidth, uHeight);
    if (NULL != pScaler)
    {
        SAFE_RELEASE(m_pFormatConverter);
        hr = s_pImagingFactory->CreateFormatConverter(&m_pFormatConverter);
        if (SUCCEEDED(hr))
        {
            hr = m_pFormatConverter->Initialize(
                pScaler,                         // Input bitmap to convert.
                GUID_WICPixelFormat32bppPBGRA,   // Input bitmap to convert.
                WICBitmapDitherTypeNone,         // Destination pixel format.
                NULL,                            // Specified dither pattern.
                0.0f,                            // Specify a particular palette.
                WICBitmapPaletteTypeCustom);     // Palette translation type.

            m_pFormatConverter->GetSize(&m_cxGifImagePixel, &m_cyGifImagePixel);
        }
    }

    SAFE_RELEASE(pScaler);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

void WICAnimatedGif::ClearData()
{
    SAFE_RELEASE(m_pWicBitmapDecoder);
    SAFE_RELEASE(m_pFormatConverter);
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::IsEndAnimation()
{
    return (m_fHasLoop && IsLastFrame() && (m_uLoopNumber == m_uTotalLoopCount + 1));
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::GetRawFrame(UINT uFrameIndex)
{
    if (NULL == m_pWicBitmapDecoder)
    {
        return FALSE;
    }

    SAFE_RELEASE(m_pFormatConverter);
    IWICBitmapFrameDecode *pWicFrame = NULL;
    IWICMetadataQueryReader *pFrameMetadataQueryReader = NULL;

    PROPVARIANT propValue;
    PropVariantInit(&propValue);
    HRESULT hr = m_pWicBitmapDecoder->GetFrame(uFrameIndex, &pWicFrame);
    if (SUCCEEDED(hr))
    {
        hr = s_pImagingFactory->CreateFormatConverter(&m_pFormatConverter);
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pFormatConverter->Initialize(
                                 pWicFrame,
                                 GUID_WICPixelFormat32bppPBGRA,
                                 WICBitmapDitherTypeNone,
                                 NULL,
                                 0.0f,
                                 WICBitmapPaletteTypeCustom);
    }

    if (SUCCEEDED(hr))
    {
        hr = pWicFrame->GetMetadataQueryReader(&pFrameMetadataQueryReader);
        if (SUCCEEDED(hr))
        {
            UINT left   = 0;
            UINT top    = 0;
            UINT width  = 0;
            UINT height = 0;
            GetGifRectValue(pFrameMetadataQueryReader, GIF_RECT_VALUETYPE_LEFT, left);
            GetGifRectValue(pFrameMetadataQueryReader, GIF_RECT_VALUETYPE_TOP, top);
            GetGifRectValue(pFrameMetadataQueryReader, GIF_RECT_VALUETYPE_RIGHT, width);
            GetGifRectValue(pFrameMetadataQueryReader, GIF_RECT_VALUETYPE_BOTTOM, height);

            SetRect(&m_frameRect, (int)left, (int)top, (int)(left + width), (int)(top + height));
        }
    }

    if (SUCCEEDED(hr))
    {
        PropVariantClear(&propValue);
        hr = pFrameMetadataQueryReader->GetMetadataByName(GIF_PATH_FRAME_DELAY, &propValue);
        if (SUCCEEDED(hr) && (VT_UI2 == propValue.vt))
        {
            hr = UIntMult(propValue.uiVal, 10, &m_uFrameDelay);
            if (SUCCEEDED(hr))
            {
                m_uFrameDelay = (m_uFrameDelay < 90) ? 90 : m_uFrameDelay;
            }
        }
        else
        {
            m_uFrameDelay = 0;
        }
    }

    if (SUCCEEDED(hr))
    {
        PropVariantClear(&propValue);
        hr = pFrameMetadataQueryReader->GetMetadataByName(GIF_PATH_FRAME_DISPOSAL, &propValue);
        if (SUCCEEDED(hr) && (VT_UI1 == propValue.vt))
        {
            m_uFrameDisposal = propValue.bVal;
        }
        else
        {
            m_uFrameDisposal = DM_UNDEFINED;
        }
    }

    PropVariantClear(&propValue);
    SAFE_RELEASE(pWicFrame);
    SAFE_RELEASE(pFrameMetadataQueryReader);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::GetGlobalMetaData()
{
    // Get background color of the GIF image.
    BOOL retVal1 = GetGifBackgroundColor();
    BOOL retVal2 = GetGifSizeInfo();
    BOOL retVal3 = GetGifLoopingInfo();

    return (retVal1 || retVal2 || retVal3);
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::GetGifBackgroundColor()
{
    if (NULL == m_pWicBitmapDecoder)
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    IWICMetadataQueryReader *pMetadataQueryReader = NULL;
    hr = m_pWicBitmapDecoder->GetMetadataQueryReader(&pMetadataQueryReader);
    if (FAILED(hr))
    {
        return FALSE;
    }

    BYTE backgroundIndex = 0;
    PROPVARIANT propVariant;
    PropVariantInit(&propVariant);
    hr = pMetadataQueryReader->GetMetadataByName(GIF_PATH_GLOBALCOLORTABLEFLAG, &propVariant);
    if (SUCCEEDED(hr))
    {
        hr = (VT_BOOL != propVariant.vt || !propVariant.boolVal) ? E_FAIL : S_OK;
        PropVariantClear(&propVariant);
    }

    if (SUCCEEDED(hr))
    {
        hr = pMetadataQueryReader->GetMetadataByName(GIF_PATH_BACKGROUNDCOLORINDEX, &propVariant);
        if (SUCCEEDED(hr) && (VT_UI1 == propVariant.vt))
        {
            backgroundIndex = propVariant.bVal;
        }
        PropVariantClear(&propVariant);
    }

    IWICPalette *pWicPalette = NULL;
    if (SUCCEEDED(hr))
    {
        hr = s_pImagingFactory->CreatePalette(&pWicPalette);
        if (SUCCEEDED(hr) && SUCCEEDED(m_pWicBitmapDecoder->CopyPalette(pWicPalette)))
        {
            UINT cColorsCopied = 0;
            WICColor rgColors[256] = { 0 };
            hr = pWicPalette->GetColors(ARRAYSIZE(rgColors), rgColors, &cColorsCopied);
            if (SUCCEEDED(hr) && (cColorsCopied > backgroundIndex))
            {
                // Get the color in ARGB format
                DWORD dwBGColor = rgColors[backgroundIndex];
                // The background color is in ARGB format, and we want to extract the alpha value and convert it in FLOAT.
                FLOAT alpha = (dwBGColor >> 24) / 255.0f;
                m_bkColorRGB = dwBGColor;
                m_bkColorAlpha = alpha;
            }
        }
    }

    SAFE_RELEASE(pWicPalette);
    SAFE_RELEASE(pMetadataQueryReader);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::GetGifSizeInfo()
{
    if (NULL == m_pWicBitmapDecoder)
    {
        return FALSE;
    }

    IWICMetadataQueryReader *pMetadataQueryReader = NULL;
    HRESULT hr = m_pWicBitmapDecoder->GetMetadataQueryReader(&pMetadataQueryReader);
    if (FAILED(hr))
    {
        return FALSE;
    }

    UINT cxGifImage = 0;
    UINT cyGifImage = 0;
    PROPVARIANT propValue;
    PropVariantInit(&propValue);

    // Get the size information from the GIF meta data.
    hr = pMetadataQueryReader->GetMetadataByName(GIF_PATH_GLOBAL_WIDTH, &propValue);
    if (SUCCEEDED(hr) && (VT_UI2 == propValue.vt))
    {
        cxGifImage = propValue.uiVal;
    }

    PropVariantClear(&propValue);
    hr = pMetadataQueryReader->GetMetadataByName(GIF_PATH_GLOBAL_HEIGHT, &propValue);
    if (SUCCEEDED(hr) && (VT_UI2 == propValue.vt))
    {
        cyGifImage = propValue.uiVal;
    }

    PropVariantClear(&propValue);
    hr = pMetadataQueryReader->GetMetadataByName(GIF_PATH_PIXELASPECTRATIO, &propValue);
    if (SUCCEEDED(hr) && (VT_UI1 == propValue.vt))
    {
        UINT uPixelAspRatio = propValue.bVal;
        if (uPixelAspRatio != 0)
        {
            // Need to calculate the ratio. The value in uPixelAspRatio 
            // allows specifying widest pixel 4:1 to the tallest pixel of 
            // 1:4 in increments of 1/64th.
            FLOAT pixelAspRatio = (uPixelAspRatio + 15.f) / 64.f;

            // Calculate the image width and height in pixel based on the
            // pixel aspect ratio. Only shrink the image.
            if (pixelAspRatio > 1.f)
            {
                m_cxGifImagePixel = cxGifImage;
                m_cyGifImagePixel = (UINT)(cyGifImage / pixelAspRatio);
            }
            else
            {
                m_cxGifImagePixel = (UINT)(cxGifImage * pixelAspRatio);
                m_cyGifImagePixel = cyGifImage;
            }
        }
        else
        {
            // The value is 0, so its ratio is 1
            m_cxGifImagePixel = cxGifImage;
            m_cyGifImagePixel = cyGifImage;
        }
    }

    SAFE_RELEASE(pMetadataQueryReader);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::GetGifLoopingInfo()
{
    if (NULL == m_pWicBitmapDecoder)
    {
        return FALSE;
    }

    IWICMetadataQueryReader *pMetadataQueryReader = NULL;
    HRESULT hr = m_pWicBitmapDecoder->GetMetadataQueryReader(&pMetadataQueryReader);
    if (FAILED(hr))
    {
        return FALSE;
    }

    PROPVARIANT propValue;
    PropVariantInit(&propValue);

    // Get the looping information from GIF meta data.
    // First check to see if the application block in the Application Extension
    // contains "NETSCAPE2.0" and "ANIMEXTS1.0", which indicates the gif animation
    // has looping information associated with it.
    // If we fail to get the looping information, loop the animation infinitely.
    hr = pMetadataQueryReader->GetMetadataByName(GIF_PATH_GLOBAL_APPLICATION, &propValue);
    if ( SUCCEEDED(hr) && ((VT_UI1 | VT_VECTOR) == propValue.vt) && (11 == propValue.caub.cElems) &&
         (!memcmp(propValue.caub.pElems, "NETSCAPE2.0", propValue.caub.cElems) ||
          !memcmp(propValue.caub.pElems, "ANIMEXTS1.0", propValue.caub.cElems)) )
    {
        PropVariantClear(&propValue);
        hr = pMetadataQueryReader->GetMetadataByName(GIF_PATH_GLOBAL_DATA, &propValue);
        if ( (SUCCEEDED(hr)) && ((VT_UI1 | VT_VECTOR) == propValue.vt) && 
             (propValue.caub.cElems >= 4) && (propValue.caub.pElems[0] > 0) &&
             (propValue.caub.pElems[1] == 1) )
        {
            m_uTotalLoopCount = MAKEWORD(propValue.caub.pElems[2], propValue.caub.pElems[3]);
            // If the total loop count is not zero, we then have a loop count. If it is 0, then we repeat infinitely
            if (m_uTotalLoopCount != 0) 
            {
                m_fHasLoop = TRUE;
            }
        }
    }

    PropVariantClear(&propValue);
    SAFE_RELEASE(pMetadataQueryReader);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::GetGifRectValue(IWICMetadataQueryReader *pFrameMetadataQueryReader, GIF_RECT_VALUETYPE type, UINT& value)
{
    if (NULL == pFrameMetadataQueryReader)
    {
        return FALSE;
    }

    PROPVARIANT propValue;
    PropVariantInit(&propValue);
    LPCWSTR lpDataName = NULL;
    TYPE_TO_TYPENAME(type, lpDataName);

    HRESULT hr = pFrameMetadataQueryReader->GetMetadataByName(lpDataName, &propValue);
    if (SUCCEEDED(hr) && (VT_UI2 == propValue.vt))
    {
        value = (UINT)propValue.uiVal;
    }

    PropVariantClear(&propValue);

    return (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL WICAnimatedGif::IsLastFrame()
{
    return (0 == m_uNextFrameIndex);
}

//////////////////////////////////////////////////////////////////////////

DWORD WICAnimatedGif::LoadResourceData(UINT uResId, HMODULE hModule, PVOID *pData)
{
    LPCTSTR lpName = MAKEINTRESOURCE(uResId);
    if (!IS_INTRESOURCE(lpName))
    {
        return 0;
    }

    HMODULE hResInst = (NULL == hModule) ? GetModuleHandle(NULL) : hModule;
    if (NULL == hResInst)
    {
        return 0;
    }

    WCHAR *pTypeName = NULL;
    TYPE_TO_GIFTYPENAME(m_curGifType, pTypeName);
    HRSRC hrSrc = FindResource(hResInst, lpName, pTypeName);
    if (NULL == hrSrc)
    {
        return 0;
    }

    DWORD dwSize = SizeofResource(hResInst, hrSrc);
    if (dwSize <= 0)
    {
        return 0;
    }

    HGLOBAL hGlobal = LoadResource(hResInst, hrSrc);
    *pData = (BYTE*)LockResource(hGlobal);

    return dwSize;
}

//////////////////////////////////////////////////////////////////////////

IWICBitmapScaler* WICAnimatedGif::CreateBitmapScaler(IN IWICBitmapSource* pWICBitmapSource, UINT32 uDestWidth, UINT32 uDestHeight)
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
    if (SUCCEEDED(hr))
    {
        hr = pBitmapScaler->Initialize(pWICBitmapSource, uDestWidth, uDestHeight, WICBitmapInterpolationModeCubic);
        if (SUCCEEDED(hr))
        {
            return pBitmapScaler;
        }
    }

    SAFE_RELEASE(pBitmapScaler);

    return NULL;
}
