/*!
* @file D2DTextLayout.cpp
* 
* @brief This file defines the class D2DTextLayout, is used to operate text.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang & Li Hong
* @date 2011/01/04
*/

#include "D2DTextLayout.h"
#include "CustomTextRenderer.h"

#define MAX_HEIGHT  100         // The default max height of textLayout.
#define MAX_WIDTH   30          // The default max width of textLayout.

D2DTextLayout::D2DTextLayout() : m_pText(NULL),
                                 m_pDWriteFactory(NULL),
                                 m_pDWriteTextLayout(NULL),
                                 m_pDWriteTextFormat(NULL),
                                 m_pD2DSolidBrush(NULL),
                                 m_pTextRender(NULL),
                                 m_hasInitialized(FALSE),
                                 m_isReCreateLayout(FALSE),
                                 m_fDefaultFontSize(18.0f),
                                 m_curTextColor(ColorF(ColorF::Black)),
                                 m_textStartPoint(D2D1::Point2F()),
                                 m_textFontFamily(NULL),
                                 m_fontStyle(DWRITE_FONT_STYLE_NORMAL),
                                 m_fontWeight(DWRITE_FONT_WEIGHT_NORMAL),
                                 m_fontStretch(DWRITE_FONT_STRETCH_NORMAL)
{
    ZeroMemory(&m_textRangle, sizeof(DWRITE_TEXT_RANGE));
}

//////////////////////////////////////////////////////////////////////////

D2DTextLayout::~D2DTextLayout()
{
    SAFE_DELETE_ARRAY(m_pText);
    SAFE_DELETE_ARRAY(m_textFontFamily);
    SAFE_RELEASE(m_pDWriteFactory);
    SAFE_RELEASE(m_pDWriteTextLayout);
    SAFE_RELEASE(m_pDWriteTextFormat);
    SAFE_RELEASE(m_pTextRender);
    SAFE_DELETE(m_pD2DSolidBrush);

    D2DDevice *pD2DDevice = GetD2DDevice();
    if (NULL != pD2DDevice)
    {
        pD2DDevice->UnregisterD2DDeviceNotification(this);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::Initialize()
{
    HRESULT hr = E_FAIL;
    LPWSTR fontFamily = NULL;

    // Create a DirectWrite factory.
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
                             __uuidof(m_pDWriteFactory),
                             reinterpret_cast<IUnknown**>(&m_pDWriteFactory));

    if (SUCCEEDED(hr))
    {
        fontFamily = L"Segoe UI";
        hr = m_pDWriteFactory->CreateTextFormat(fontFamily, 
                                                NULL,
                                                DWRITE_FONT_WEIGHT_NORMAL,
                                                DWRITE_FONT_STYLE_NORMAL,
                                                DWRITE_FONT_STRETCH_NORMAL,
                                                m_fDefaultFontSize,
                                                L"en-us",
                                                &m_pDWriteTextFormat);
        if (SUCCEEDED(hr))
        {
            m_pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            m_pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            m_pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

            IDWriteInlineObject *pEllipsis = NULL;
            hr = m_pDWriteFactory->CreateEllipsisTrimmingSign(m_pDWriteTextFormat, &pEllipsis);
            if (SUCCEEDED(hr))
            {
                static const DWRITE_TRIMMING sc_trimming =
                {
                    DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0
                };

                // Set the trimming back on the trimming format object.
                hr = m_pDWriteTextFormat->SetTrimming(&sc_trimming, pEllipsis);
                SAFE_RELEASE(pEllipsis);
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pDWriteFactory->CreateTextLayout(L"",
                                (UINT32)wcslen(L""),
                                m_pDWriteTextFormat,
                                MAX_WIDTH,
                                MAX_HEIGHT,
                                &m_pDWriteTextLayout);
    }

    m_hasInitialized = (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DTextLayout::HasInitialized()
{
    return m_hasInitialized;
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetD2DDevice(IN D2DDevice *pD2DDevice)
{
    if (NULL != pD2DDevice)
    {
        D2DResource::SetD2DDevice(pD2DDevice);
        pD2DDevice->RegisterD2DDeviceNotification(this);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetText(IN LPCWSTR lpText)
{
    if (NULL != lpText)
    {
        SAFE_DELETE_ARRAY(m_pText);

        int length = wcslen(lpText) + 1;
        m_pText = new WCHAR[length];
        ZeroMemory(m_pText, length * sizeof(WCHAR));
        StringCchCopy(m_pText, length, lpText);

        m_isReCreateLayout = TRUE;
        CreateTextLayout();
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::GetText(IN LPWSTR lpszText, UINT32 uSize)
{
    if ((NULL != m_pText) && (NULL != lpszText) && (uSize > 0))
    {
        wcscpy_s(lpszText, uSize, m_pText);
    }
}

//////////////////////////////////////////////////////////////////////////

UINT32 D2DTextLayout::GetTextLength()
{
    return (NULL != m_pText) ? wcslen(m_pText) : 0;
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetTextAlignment(IN DWRITE_TEXT_ALIGNMENT textAlign)
{
    if (NULL != m_pDWriteTextFormat)
    {
        m_pDWriteTextFormat->SetTextAlignment(textAlign);
        m_isReCreateLayout = TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetTextParagraphAlign(DWRITE_PARAGRAPH_ALIGNMENT textParagraph)
{
    if (NULL != m_pDWriteTextFormat)
    {
        if (textParagraph != m_pDWriteTextFormat->GetParagraphAlignment())
        {
            m_pDWriteTextFormat->SetParagraphAlignment(textParagraph);
            m_isReCreateLayout = TRUE;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetTextWrap(IN DWRITE_WORD_WRAPPING textWrap)
{
    if (NULL != m_pDWriteTextFormat)
    {
        m_pDWriteTextFormat->SetWordWrapping(textWrap);

        if (DWRITE_WORD_WRAPPING_WRAP == textWrap)
        {
            IDWriteInlineObject *pEllipsis = NULL;
            m_pDWriteFactory->CreateEllipsisTrimmingSign(m_pDWriteTextFormat, &pEllipsis);

            static const DWRITE_TRIMMING sc_trimming =
            {
                DWRITE_TRIMMING_GRANULARITY_NONE, 0, 0
            };

            // Set the trimming back on the trimming format object.
            m_pDWriteTextFormat->SetTrimming(&sc_trimming, pEllipsis);
            SAFE_RELEASE(pEllipsis);
        }
        else
        {
            IDWriteInlineObject *pEllipsis = NULL;
            m_pDWriteFactory->CreateEllipsisTrimmingSign(m_pDWriteTextFormat, &pEllipsis);

            static const DWRITE_TRIMMING sc_trimming =
            {
                DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0
            };

            // Set the trimming back on the trimming format object.
            m_pDWriteTextFormat->SetTrimming(&sc_trimming, pEllipsis);
            SAFE_RELEASE(pEllipsis);
        }

        m_isReCreateLayout = TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::CreateTextLayout()
{
    if (!HasInitialized() || !m_isReCreateLayout)
    {
        return;
    }

    LPCWSTR lpTempText = (NULL == m_pText) ? L"" : m_pText;
    UINT32 length = (UINT32)wcslen(lpTempText);
    m_textRangle.length = length;

    FLOAT maxHeight = (NULL != m_pDWriteTextLayout) ? m_pDWriteTextLayout->GetMaxHeight() : MAX_HEIGHT;
    FLOAT maxWidth  = (NULL != m_pDWriteTextLayout) ? m_pDWriteTextLayout->GetMaxWidth() : MAX_WIDTH;

    SAFE_RELEASE(m_pDWriteTextLayout);
    m_pDWriteFactory->CreateTextLayout(lpTempText,
                                       length,
                                       m_pDWriteTextFormat,
                                       maxWidth,
                                       maxHeight,
                                       &m_pDWriteTextLayout);

    m_pDWriteTextLayout->SetFontStyle(m_fontStyle, m_textRangle);
    m_pDWriteTextLayout->SetFontWeight(m_fontWeight, m_textRangle);
    m_pDWriteTextLayout->SetFontSize(m_fDefaultFontSize, m_textRangle);
    m_pDWriteTextLayout->SetFontFamilyName(m_textFontFamily, m_textRangle);
    m_pDWriteTextLayout->SetFontStretch(m_fontStretch, m_textRangle);

    m_isReCreateLayout = FALSE;
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::GetIDWriteTextLayout(OUT IDWriteTextLayout **ppDWriteTextLayout)
{
    if (NULL != ppDWriteTextLayout && NULL != m_pDWriteTextLayout)
    {
        (*ppDWriteTextLayout) = m_pDWriteTextLayout;
        (*ppDWriteTextLayout)->AddRef();
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetTextStartPoint(IN const D2D1_POINT_2F& startPoint)
{
    m_textStartPoint = startPoint;
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::DrawText(IN D2DDevice *pD2DDevice)
{
    if (NULL != pD2DDevice)
    {
        ID2D1RenderTarget *pRenderTarget = NULL;
        pD2DDevice->GetRenderTarget(&pRenderTarget);
        DrawText(pRenderTarget);
        SAFE_RELEASE(pRenderTarget);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::DrawText(IN ID2D1RenderTarget *pRenderTarget)
{
    if ( (NULL != pRenderTarget) && (NULL != m_pDWriteTextLayout) )
    {
        if (NULL == m_pD2DSolidBrush)
        {
            m_pD2DSolidBrush = new D2DSolidColorBrush();
            m_pD2DSolidBrush->CreateBrush(pRenderTarget);
        }

        CreateTextLayout();

        ID2D1Brush *pD2DBrush = NULL;
        m_pD2DSolidBrush->SetColor(m_curTextColor);
        m_pD2DSolidBrush->GetD2DBrush(&pD2DBrush);
        //pRenderTarget->DrawTextLayout(m_textStartPoint, m_pDWriteTextLayout, pD2DBrush,D2D1_DRAW_TEXT_OPTIONS_NONE);

        ID2D1Factory *pD2DFactory = NULL;
        pRenderTarget->GetFactory(&pD2DFactory);

        if (NULL == m_pTextRender)
        {
            m_pTextRender = new CustomTextRenderer(pD2DFactory, pRenderTarget, pD2DBrush);
        }

        m_pDWriteTextLayout->Draw(NULL, m_pTextRender, m_textStartPoint.x, m_textStartPoint.y);

        SAFE_RELEASE(pD2DBrush);
        SAFE_RELEASE(pD2DFactory);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetTextColor(IN const D2D1_COLOR_F& color)
{
    m_curTextColor = color;

    if (NULL != m_pD2DSolidBrush)
    {
        m_pD2DSolidBrush->SetColor(color);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetFontFamilyName(IN LPCWSTR familyName)
{
    if(NULL != familyName)
    {
        SAFE_DELETE_ARRAY(m_textFontFamily);

        int length = wcslen(familyName) + 1;
        m_textFontFamily = new WCHAR[length];
        ZeroMemory(m_textFontFamily, length * sizeof(WCHAR));
        wcscpy_s(m_textFontFamily, length, familyName);
    }

    if (NULL != m_pDWriteTextLayout)
    {
        m_pDWriteTextLayout->SetFontFamilyName(familyName, m_textRangle);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetTextFontSize(IN FLOAT size)
{
    m_fDefaultFontSize = size;

    if (NULL != m_pDWriteTextLayout)
    {
        m_pDWriteTextLayout->SetFontSize(size, m_textRangle);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetFontStretch(IN DWRITE_FONT_STRETCH fontStretch)
{
    m_fontStretch = fontStretch;

    if (NULL != m_pDWriteTextLayout)
    {
        m_pDWriteTextLayout->SetFontStretch(fontStretch, m_textRangle);
    }
}
//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetFontStyle(IN DWRITE_FONT_STYLE fontStyle)
{
    m_fontStyle = fontStyle;

    if (NULL != m_pDWriteTextLayout)
    {
        m_pDWriteTextLayout->SetFontStyle(fontStyle, m_textRangle);
    }
}
//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetFontWeight(IN DWRITE_FONT_WEIGHT fontWeight)
{
    m_fontWeight = fontWeight;

    if (NULL != m_pDWriteTextLayout)
    {
        m_pDWriteTextLayout->SetFontWeight(fontWeight, m_textRangle);
        m_isReCreateLayout = TRUE;
        //CreateTextLayout();
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetLocaleName(IN LPCWSTR pLocalName)
{
    if (NULL != m_pDWriteTextLayout)
    {
        m_pDWriteTextLayout->SetLocaleName(pLocalName, m_textRangle);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetMaxHeight(IN FLOAT maxHeight)
{
    if (NULL != m_pDWriteTextLayout)
    {
        m_pDWriteTextLayout->SetMaxHeight(maxHeight);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetMaxWidth(IN FLOAT maxWidth)
{
    if (NULL != m_pDWriteTextLayout)
    {
        m_pDWriteTextLayout->SetMaxWidth(maxWidth);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetStrikethrough(IN BOOL hasStrikethrough)
{
    if (NULL != m_pDWriteTextLayout)
    {
        m_pDWriteTextLayout->SetStrikethrough(hasStrikethrough, m_textRangle);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::SetTextUnderline(IN BOOL hasUnderLine)
{
    if (NULL != m_pDWriteTextLayout)
    {
        m_pDWriteTextLayout->SetUnderline(hasUnderLine, m_textRangle);
    }
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);

    SAFE_RELEASE(m_pTextRender);
}

//////////////////////////////////////////////////////////////////////////

void D2DTextLayout::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);
}
