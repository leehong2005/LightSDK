/*!
* @file CustomTextRenderer.cpp 
* 
* @brief This file defines the class CustomTextRenderer, is used to Generation text renderer.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang
* @date 2011/01/19
*/

#include "CustomTextRenderer.h"


CustomTextRenderer::CustomTextRenderer(ID2D1Factory *pD2DFactory, 
                                       ID2D1RenderTarget *pRT, 
                                       ID2D1Brush *pOutlineBrush) : n_cRefCount_(1), 
                                                                    m_pD2DFactory(pD2DFactory), 
                                                                    m_pRenderTarget(pRT), 
                                                                    m_pD2DBrush(pOutlineBrush)
{
    if (NULL != m_pD2DFactory)
    {
        m_pD2DFactory->AddRef();
    }

    if (NULL != m_pRenderTarget)
    {
        m_pRenderTarget->AddRef();
    }

    if (NULL != m_pD2DBrush)
    {
        m_pD2DBrush->AddRef();
    }
}

//////////////////////////////////////////////////////////////////////////

CustomTextRenderer::~CustomTextRenderer()
{
    SAFE_RELEASE(m_pD2DFactory);
    SAFE_RELEASE(m_pRenderTarget);
    SAFE_RELEASE(m_pD2DBrush);
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP CustomTextRenderer::DrawGlyphRun(__maybenull void* clientDrawingContext,
                                                FLOAT baselineOriginX,
                                                FLOAT baselineOriginY,
                                                DWRITE_MEASURING_MODE measuringMode,
                                                __in DWRITE_GLYPH_RUN const* glyphRun,
                                                __in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
                                                IUnknown* clientDrawingEffect)
{
    UNREFERENCED_PARAMETER(clientDrawingContext);
    UNREFERENCED_PARAMETER(measuringMode);
    UNREFERENCED_PARAMETER(glyphRunDescription);
    UNREFERENCED_PARAMETER(clientDrawingEffect);

    HRESULT hr = S_OK;

    // Create the path geometry.
    ID2D1PathGeometry* pPathGeometry = NULL;
    hr = m_pD2DFactory->CreatePathGeometry(&pPathGeometry);

    // Write to the path geometry using the geometry sink.
    ID2D1GeometrySink* pSink = NULL;
    if (SUCCEEDED(hr))
    {
        hr = pPathGeometry->Open(&pSink);
    }

    // Get the glyph run outline geometries back from DirectWrite and place them within the
    // geometry sink.
    if (SUCCEEDED(hr))
    {
        hr = glyphRun->fontFace->GetGlyphRunOutline(glyphRun->fontEmSize,
                                                    glyphRun->glyphIndices,
                                                    glyphRun->glyphAdvances,
                                                    glyphRun->glyphOffsets,
                                                    glyphRun->glyphCount,
                                                    glyphRun->isSideways,
                                                    glyphRun->bidiLevel % 2,
                                                    pSink);
    }

    // Close the geometry sink
    if (SUCCEEDED(hr))
    {
        hr = pSink->Close();
    }

    // Initialize a matrix to translate the origin of the glyph run.
    D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
        1.0f, 0.0f,
        0.0f, 1.0f,
        baselineOriginX, baselineOriginY
        );

    // Create the transformed geometry
    ID2D1TransformedGeometry* pTransformedGeometry = NULL;
    if (SUCCEEDED(hr))
    {
        hr = m_pD2DFactory->CreateTransformedGeometry(pPathGeometry,
                                                      &matrix,
                                                      &pTransformedGeometry);
    }

    // Draw the outline of the glyph run.
    //m_pRenderTarget->DrawGeometry(pTransformedGeometry, m_pD2DBrush);

    // Fill in the glyph run.
    m_pRenderTarget->FillGeometry(pTransformedGeometry, m_pD2DBrush);

    SAFE_RELEASE(pPathGeometry);
    SAFE_RELEASE(pSink);
    SAFE_RELEASE(pTransformedGeometry);

    return hr;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP CustomTextRenderer::DrawUnderline(__maybenull void* clientDrawingContext,
                                                 FLOAT baselineOriginX,
                                                 FLOAT baselineOriginY,
                                                 __in DWRITE_UNDERLINE const* underline,
                                                 IUnknown* clientDrawingEffect)
{
    UNREFERENCED_PARAMETER(clientDrawingContext);
    UNREFERENCED_PARAMETER(clientDrawingEffect);

    HRESULT hr;

    D2D1_RECT_F rect = D2D1::RectF(
        0,
        underline->offset,
        underline->width,
        underline->offset + underline->thickness);

    ID2D1RectangleGeometry* pRectangleGeometry = NULL;
    hr = m_pD2DFactory->CreateRectangleGeometry(&rect, &pRectangleGeometry);

    // Initialize a matrix to translate the origin of the underline
    D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
        1.0f, 0.0f,
        0.0f, 1.0f,
        baselineOriginX, baselineOriginY);

    ID2D1TransformedGeometry* pTransformedGeometry = NULL;
    if (SUCCEEDED(hr))
    {
        hr = m_pD2DFactory->CreateTransformedGeometry(
                                                      pRectangleGeometry,
                                                      &matrix,
                                                      &pTransformedGeometry);
    }

    // Draw the outline of the rectangle
    m_pRenderTarget->DrawGeometry(pTransformedGeometry, m_pD2DBrush);

    // Fill in the rectangle
    m_pRenderTarget->FillGeometry(pTransformedGeometry, m_pD2DBrush);

    SAFE_RELEASE(pRectangleGeometry);
    SAFE_RELEASE(pTransformedGeometry);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP CustomTextRenderer::DrawStrikethrough(__maybenull void* clientDrawingContext,
                                                     FLOAT baselineOriginX,
                                                     FLOAT baselineOriginY,
                                                     __in DWRITE_STRIKETHROUGH const* strikethrough,
                                                     IUnknown* clientDrawingEffect)
{
    UNREFERENCED_PARAMETER(clientDrawingContext);
    UNREFERENCED_PARAMETER(clientDrawingEffect);

    HRESULT hr;

    D2D1_RECT_F rect = D2D1::RectF(
        0,
        strikethrough->offset,
        strikethrough->width,
        strikethrough->offset + strikethrough->thickness
        );

    ID2D1RectangleGeometry* pRectangleGeometry = NULL;
    hr = m_pD2DFactory->CreateRectangleGeometry(&rect, &pRectangleGeometry);

    // Initialize a matrix to translate the origin of the strikethrough
    D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
        1.0f, 0.0f,
        0.0f, 1.0f,
        baselineOriginX, baselineOriginY);

    ID2D1TransformedGeometry* pTransformedGeometry = NULL;
    if (SUCCEEDED(hr))
    {
        hr = m_pD2DFactory->CreateTransformedGeometry(
            pRectangleGeometry,
            &matrix,
            &pTransformedGeometry);
    }

    // Draw the outline of the rectangle
    m_pRenderTarget->DrawGeometry(pTransformedGeometry, m_pD2DBrush);

    // Fill in the rectangle
    m_pRenderTarget->FillGeometry(pTransformedGeometry, m_pD2DBrush);

    SAFE_RELEASE(pRectangleGeometry);
    SAFE_RELEASE(pTransformedGeometry);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP CustomTextRenderer::DrawInlineObject(__maybenull void* clientDrawingContext,
                                                    FLOAT originX,
                                                    FLOAT originY,
                                                    IDWriteInlineObject* inlineObject,
                                                    BOOL isSideways,
                                                    BOOL isRightToLeft,
                                                    IUnknown* clientDrawingEffect)
{
    UNREFERENCED_PARAMETER(clientDrawingContext);
    UNREFERENCED_PARAMETER(inlineObject);
    UNREFERENCED_PARAMETER(isSideways);
    UNREFERENCED_PARAMETER(isRightToLeft);
    UNREFERENCED_PARAMETER(clientDrawingEffect);

    ID2D1StrokeStyle  *pD2D1StrokeStyle = NULL;
    FLOAT dashes[] = {1.0f,1.0f};
    m_pD2DFactory->CreateStrokeStyle(D2D1::StrokeStyleProperties(D2D1_CAP_STYLE_FLAT,
                                                                D2D1_CAP_STYLE_FLAT,
                                                                D2D1_CAP_STYLE_FLAT,
                                                                D2D1_LINE_JOIN_MITER,
                                                                2.0f,
                                                                D2D1_DASH_STYLE_CUSTOM,
                                                                0.0f),
                                                                dashes,
                                                                ARRAYSIZE(dashes),
                                                                &pD2D1StrokeStyle);

    m_pRenderTarget->DrawLine(D2D1::Point2F(originX+2, originY),
                              D2D1::Point2F(originX+12, originY),
                              m_pD2DBrush, 
                              2.0f, 
                              pD2D1StrokeStyle);

    SAFE_RELEASE(pD2D1StrokeStyle);
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP_(unsigned long) CustomTextRenderer::AddRef()
{
    return InterlockedIncrement(&n_cRefCount_);
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP_(unsigned long) CustomTextRenderer::Release()
{
    unsigned long newCount = InterlockedDecrement(&n_cRefCount_);
    if (newCount == 0)
    {
        delete this;
        return 0;
    }

    return newCount;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP CustomTextRenderer::IsPixelSnappingDisabled(__maybenull void* clientDrawingContext,
                                                           __out BOOL* isDisabled)
{
    UNREFERENCED_PARAMETER(clientDrawingContext);

    *isDisabled = FALSE;
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP CustomTextRenderer::GetCurrentTransform(__maybenull void* clientDrawingContext,
                                                       __out DWRITE_MATRIX* transform)
{
    UNREFERENCED_PARAMETER(clientDrawingContext);

    //forward the render target's transform
    m_pRenderTarget->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP CustomTextRenderer::GetPixelsPerDip(__maybenull void* clientDrawingContext,
                                                   __out FLOAT* pixelsPerDip)
{
    UNREFERENCED_PARAMETER(clientDrawingContext);

    *pixelsPerDip = 1.0f;
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP CustomTextRenderer::QueryInterface(IID const& riid,void** ppvObject)
{
    static const QITAB qit[] = 
    {
        QITABENT(CustomTextRenderer, IDWriteTextRenderer),
        QITABENT(CustomTextRenderer, IDWritePixelSnapping),
        QITABENT(CustomTextRenderer, IUnknown),
        { 0 },
    };

    return QISearch(this, qit, riid, ppvObject);
}
