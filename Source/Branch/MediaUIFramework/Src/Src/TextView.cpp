/*!
* @file TextView.cpp
* 
* @brief This file defines the class TextView, it's a control for display text.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang
* @date 2011/01/04
*/

#include "TextView.h"

#define MAX_LOADSTRING          1024

TextView::TextView() : m_isWrapContent(FALSE),
                       m_isClipOverflowText(FALSE),
                       m_isRequestLayout(FALSE),
                       m_textDockDir(TEXT_DOCK_DIRECTION_DEFAULT),
                       m_textColor(ColorF(ColorF::Black)),
                       m_pD2DTextLayout(new D2DTextLayout())
{
    m_pD2DTextLayout->Initialize();
}

//////////////////////////////////////////////////////////////////////////

TextView::~TextView()
{
    SAFE_DELETE(m_pD2DTextLayout);
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetWrapContent(BOOL isWrapContent)
{
    m_isWrapContent = isWrapContent;

    if (m_isWrapContent)
    {
        RequestLayout();
    }
}

//////////////////////////////////////////////////////////////////////////

FLOAT TextView::GetMeasureWidth()
{
    if (NULL == m_pD2DTextLayout)
    {
        return GetWidth();
    }

    FLOAT measuredW = GetWidth();
    IDWriteTextLayout *pDWriteTextLayout = NULL;
    m_pD2DTextLayout->GetIDWriteTextLayout(&pDWriteTextLayout);
    if (NULL != pDWriteTextLayout)
    {
        DWRITE_TEXT_METRICS metrics = { 0 };
        FLOAT maxW = pDWriteTextLayout->GetMaxWidth();
        pDWriteTextLayout->SetMaxWidth(10000);

        if (SUCCEEDED(pDWriteTextLayout->GetMetrics(&metrics)))
        {
            measuredW  = metrics.width + 10;
        }

        pDWriteTextLayout->SetMaxWidth(maxW);
    }

    SAFE_RELEASE(pDWriteTextLayout);

    return measuredW;
}

//////////////////////////////////////////////////////////////////////////

FLOAT TextView::GetMeasureHeight()
{
    if (NULL == m_pD2DTextLayout)
    {
        return GetHeight();
    }

    FLOAT measuredH = GetHeight();
    IDWriteTextLayout *pDWriteTextLayout = NULL;
    m_pD2DTextLayout->GetIDWriteTextLayout(&pDWriteTextLayout);
    if (NULL != pDWriteTextLayout)
    {
        DWRITE_TEXT_METRICS metrics = { 0 };
        FLOAT maxH = pDWriteTextLayout->GetMaxHeight();
        pDWriteTextLayout->SetMaxHeight(10000);

        if (SUCCEEDED(pDWriteTextLayout->GetMetrics(&metrics)))
        {
            measuredH = metrics.height + 10;
        }

        pDWriteTextLayout->SetMaxHeight(maxH);
    }

    SAFE_RELEASE(pDWriteTextLayout);

    return measuredH;
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetClipOverflowText(BOOL isClipText)
{
    m_isClipOverflowText = isClipText;
}

//////////////////////////////////////////////////////////////////////////

BOOL TextView::GetWrapContent()
{
    return m_isWrapContent;
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetTextDockDirection(TEXT_DOCK_DIRECTION dockDir)
{
    m_textDockDir = dockDir;
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    if (m_isWrapContent)
    {
        if (0 != m_pD2DTextLayout->GetTextLength())
        {
            m_pD2DTextLayout->SetMaxWidth(10000);
            m_pD2DTextLayout->SetMaxHeight(10000);

            IDWriteTextLayout *pDWriteTextLayout = NULL;
            m_pD2DTextLayout->GetIDWriteTextLayout(&pDWriteTextLayout);
            if (NULL != pDWriteTextLayout)
            {
                DWRITE_TEXT_METRICS matrics = { 0 };
                if (SUCCEEDED(pDWriteTextLayout->GetMetrics(&matrics)))
                {
                    width = matrics.width + 10;
                }
            }
            SAFE_RELEASE(pDWriteTextLayout);
        }
    }

    m_isRequestLayout = FALSE;

    ViewElement::SetLayoutInfo(x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetText(IN LPCWSTR lpText)
{
    if (NULL != lpText)
    {
        m_pD2DTextLayout->SetText(lpText);
        m_isRequestLayout = TRUE;
        //RequestLayout();
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetText(UINT32 resId, HMODULE hModule)
{
    TCHAR pData[MAX_LOADSTRING] = { 0 };
    HMODULE hResInt = (NULL == hModule) ? GetModuleHandle(NULL) : hModule;
    INT32 nLen = ::LoadStringW(hResInt, resId, pData, MAX_LOADSTRING);
    if (nLen > 0)
    {
        SetText(pData);
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::GetText(IN LPWSTR lpszText, UINT32 uSize)
{
    if (NULL != m_pD2DTextLayout)
    {
        m_pD2DTextLayout->GetText(lpszText, uSize);
    }
}

//////////////////////////////////////////////////////////////////////////

UINT32 TextView::GetTextLength()
{
    return (NULL != m_pD2DTextLayout) ? m_pD2DTextLayout->GetTextLength() : 0;
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetTextSize(IN FLOAT size)
{
    if (NULL != m_pD2DTextLayout)
    {
        m_pD2DTextLayout->SetTextFontSize(size);
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetTextColor(IN const D2D1_COLOR_F& color)
{
    m_textColor = color;
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetFontFamilyName(IN LPCWSTR familyName)
{
    if (NULL != m_pD2DTextLayout)
    {
        m_pD2DTextLayout->SetFontFamilyName(familyName);
        m_isRequestLayout = TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetFontStyle(IN DWRITE_FONT_STYLE fontStyle)
{
    if (NULL != m_pD2DTextLayout)
    {
        m_pD2DTextLayout->SetFontStyle(fontStyle);
        m_isRequestLayout = TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetFontStretch(IN DWRITE_FONT_STRETCH fontStretch)
{
    if (NULL != m_pD2DTextLayout)
    {
        m_pD2DTextLayout->SetFontStretch(fontStretch);
        m_isRequestLayout = TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetFontWeight(IN DWRITE_FONT_WEIGHT fontWeight)
{
    if (NULL != m_pD2DTextLayout)
    {
        m_pD2DTextLayout->SetFontWeight(fontWeight);
        m_isRequestLayout = TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetTextAlign(IN DWRITE_TEXT_ALIGNMENT textAlign)
{
    if (NULL != m_pD2DTextLayout)
    {
        m_pD2DTextLayout->SetTextAlignment(textAlign);
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetTextParagraphAlign(DWRITE_PARAGRAPH_ALIGNMENT textParagraph)
{
    if (NULL != m_pD2DTextLayout)
    {
        m_pD2DTextLayout->SetTextParagraphAlign(textParagraph);
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetTextWrap(IN DWRITE_WORD_WRAPPING textWrap)
{
    if (NULL != m_pD2DTextLayout)
    {
        m_pD2DTextLayout->SetTextWrap(textWrap);
        m_isRequestLayout = TRUE;
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::ClearAssocData()
{
    ViewElement::ClearAssocData();
}

//////////////////////////////////////////////////////////////////////////

void TextView::SetD2DTextLayoutInfo()
{
    POINT outPt = { (LONG)GetLeft(), (LONG)GetTop() };
    GetAbsolutePoint(outPt);
    FLOAT fStartX = (FLOAT)outPt.x;
    FLOAT fStartY = (FLOAT)outPt.y;
    IDWriteTextLayout *pDWriteTextLayout = NULL;

    m_pD2DTextLayout->SetMaxWidth(GetWidth());
    m_pD2DTextLayout->SetMaxHeight(GetHeight());
    m_pD2DTextLayout->SetTextColor(m_textColor);
    m_pD2DTextLayout->GetIDWriteTextLayout(&pDWriteTextLayout);
    if (NULL != pDWriteTextLayout)
    {
        DWRITE_OVERHANG_METRICS overhangMetrics = { 0 };
        pDWriteTextLayout->GetOverhangMetrics(&overhangMetrics);

        switch (m_textDockDir)
        {
        case TEXT_DOCK_DIRECTION_BOTTOM:
            fStartY += (-overhangMetrics.bottom);
            break;

        case TEXT_DOCK_DIRECTION_TOP:
            fStartY += (overhangMetrics.top);
            break;
        }
    }
    SAFE_RELEASE(pDWriteTextLayout);

    m_pD2DTextLayout->SetTextStartPoint(D2D1::Point2F(fStartX, fStartY));
}

//////////////////////////////////////////////////////////////////////////

void TextView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    m_pD2DTextLayout->SetD2DDevice(D2DDevice::FromD2DRenderTarget(pRenderTarget));

    if (m_isRequestLayout)
    {
        RequestLayout();
        m_isRequestLayout = FALSE;
    }

    ViewElement::OnDrawItem(pRenderTarget);

    if (NULL != m_pD2DTextLayout)
    {
        // Clip the overflow text when text view size is smaller the text metrics.
        if (m_isClipOverflowText)
        {
            D2D1_RECT_F absRc = { 0 };
            GetAbsoluteRect(absRc);
            pRenderTarget->PushAxisAlignedClip(absRc, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            SetD2DTextLayoutInfo();
            m_pD2DTextLayout->DrawText(pRenderTarget);
            pRenderTarget->PopAxisAlignedClip();
        }
        else
        {
            SetD2DTextLayoutInfo();
            m_pD2DTextLayout->DrawText(pRenderTarget);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void TextView::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    ViewElement::OnLayout(fChanged, left, top, width, height);

    if (fChanged)
    {
        if (NULL != m_pD2DTextLayout)
        {
            POINT outPt = { (LONG)left, (LONG)top };
            GetAbsolutePoint(outPt);

            m_pD2DTextLayout->SetTextStartPoint(D2D1::Point2F((FLOAT)outPt.x, (FLOAT)outPt.y));
            m_pD2DTextLayout->SetMaxWidth(width);
            m_pD2DTextLayout->SetMaxHeight(height);
        }
    }
}
