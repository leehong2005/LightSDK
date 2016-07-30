/*!
* @file SdkTextView.cpp
* 
* @brief This file defines the class SdkTextView, it's a control for display text.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/04
*/

#include "stdafx.h"
#include "SdkTextView.h"
#include "D2DTextLayout.h"

#define MAX_LOADSTRING          1024

USING_NAMESPACE_VIEWS

/*!
* @brief The internal data of text view.
*/
struct NAMESPACE_VIEWS::SdkTextView::_TEXTVIEW_INTERNALDATA
{
    POINT                   m_txtStartPoint;            // The text start point.
    BOOL                    m_hasSetDevice;             // Indicate has set device.
    BOOL                    m_isClipOverflowText;       // Clip the overflow text.
    BOOL                    m_isWrapContent;            // The wrap content flag.
    BOOL                    m_isReCalcText;             // Should request layout.
    D2D1_COLOR_F            m_textColor;                // The normal status text color.
    TEXT_DOCK_DIRECTION     m_textDockDir;              // The text dock.
    D2DTextLayout          *m_pD2DTextLayout;           // The D2DTextLayout.
};

//////////////////////////////////////////////////////////////////////////

SdkTextView::SdkTextView()
{
    m_pTextViewData = new _TEXTVIEW_INTERNALDATA();
    ZeroMemory(m_pTextViewData, sizeof(_TEXTVIEW_INTERNALDATA));

    m_pTextViewData->m_pD2DTextLayout  = new D2DTextLayout();
    m_pTextViewData->m_textDockDir     = TEXT_DOCK_DIRECTION_DEFAULT;
    m_pTextViewData->m_textColor       = ColorF(ColorF::Black);
    m_pTextViewData->m_txtStartPoint.x = 0;
    m_pTextViewData->m_txtStartPoint.y = 0;
    m_pTextViewData->m_pD2DTextLayout->Initialize();

    SetClassName(CLASSNAME_TEXTVIEW);
    SetTextSize(18.0f);
    SetViewSize(130, 35);
    SetTextParagraphAlign(TEXT_PARAGRAPH_ALIGNMENT_CENTER);
}

//////////////////////////////////////////////////////////////////////////

SdkTextView::~SdkTextView()
{
    SAFE_DELETE(m_pTextViewData->m_pD2DTextLayout);
    SAFE_DELETE(m_pTextViewData);
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetWrapContent(BOOL isWrapContent)
{
    if ( isWrapContent != m_pTextViewData->m_isWrapContent )
    {
        m_pTextViewData->m_isWrapContent = isWrapContent;
        m_pTextViewData->m_isReCalcText  = TRUE;

        // If wrap content flag is TRUE, request layout.
        if ( m_pTextViewData->m_isWrapContent )
        {
            RequestLayout();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetClipText(BOOL isClipText)
{
    m_pTextViewData->m_isClipOverflowText = isClipText;
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetTextDockDir(TEXT_DOCK_DIRECTION dockDir)
{
    if ( TEXT_DOCK_DIRECTION_TOP == dockDir ||
         TEXT_DOCK_DIRECTION_BOTTOM == dockDir )
    {
        SetTextParagraphAlign(TEXT_PARAGRAPH_ALIGNMENT_CENTER);
    }

    m_pTextViewData->m_textDockDir = dockDir;
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetText(IN LPCTSTR lpText)
{
    if ( NULL != lpText )
    {
        m_pTextViewData->m_pD2DTextLayout->SetText(lpText);

        // If wrap content flag is TRUE, request layout.
        if ( m_pTextViewData->m_isWrapContent )
        {
            RequestLayout();
        }

        OnTextLayoutChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetText(UINT32 resId, HMODULE hModule)
{
    // Load string from resource.
    TCHAR pData[MAX_LOADSTRING] = { 0 };
    HMODULE hResInt = (NULL == hModule) ? GetModuleHandle(NULL) : hModule;
    INT32 nLen = ::LoadStringW(hResInt, resId, pData, MAX_LOADSTRING);

    if ( nLen > 0 )
    {
        SetText(pData);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetTextSize(IN FLOAT size)
{
    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        m_pTextViewData->m_pD2DTextLayout->SetTextFontSize(size);

        OnTextLayoutChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetTextColor(IN const ColorF& color)
{
    m_pTextViewData->m_textColor = color;
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetFontFamilyName(IN LPCWSTR familyName)
{
    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        m_pTextViewData->m_pD2DTextLayout->SetFontFamilyName(familyName);

        OnTextLayoutChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetFontStyle(IN DWRITE_FONT_STYLE fontStyle)
{
    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        m_pTextViewData->m_pD2DTextLayout->SetFontStyle(fontStyle);

        OnTextLayoutChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetFontStretch(IN DWRITE_FONT_STRETCH fontStretch)
{
    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        m_pTextViewData->m_pD2DTextLayout->SetFontStretch(fontStretch);

        OnTextLayoutChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetFontWeight(IN DWRITE_FONT_WEIGHT fontWeight)
{
    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        m_pTextViewData->m_pD2DTextLayout->SetFontWeight(fontWeight);

        OnTextLayoutChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetTextAlign(IN TEXT_ALIGNMENT textAlign)
{
    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        m_pTextViewData->m_pD2DTextLayout->SetTextAlignment(
            (DWRITE_TEXT_ALIGNMENT)textAlign);

        // When the text alignment is changed, should set the start point offset to (0, 0).
        SetTextStartPoint(0, 0);
        OnTextLayoutChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetTextParagraphAlign(TEXT_PARAGRAPH_ALIGNMENT textParagraph)
{
    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        m_pTextViewData->m_pD2DTextLayout->SetTextParagraphAlign(
            (DWRITE_PARAGRAPH_ALIGNMENT)textParagraph);

        OnTextLayoutChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetTextWrap(IN DWRITE_WORD_WRAPPING textWrap)
{
    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        m_pTextViewData->m_pD2DTextLayout->SetTextWrap(textWrap);

        OnTextLayoutChanged();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    // The view's size is wrap to the text's size.
    if ( m_pTextViewData->m_isWrapContent )
    {
        SIZE size = GetMeasureSize();
        if ( size.cx > 0 && size.cy > 0 )
        {
            width  = (FLOAT)size.cx + m_pTextViewData->m_txtStartPoint.x + 10;
            height = (FLOAT)size.cy + m_pTextViewData->m_txtStartPoint.y + 10;
        }
    }

    OnTextLayoutChanged();

    SdkViewElement::SetLayoutInfo(x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::GetText(IN LPTSTR lpszText, UINT32 uSize)
{
    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        m_pTextViewData->m_pD2DTextLayout->GetText(lpszText, uSize);
    }
}

//////////////////////////////////////////////////////////////////////////

UINT32 SdkTextView::GetTextLength()
{
    if (NULL != m_pTextViewData->m_pD2DTextLayout)
    {
        return m_pTextViewData->m_pD2DTextLayout->GetTextLength();
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTextView::GetWrapContent()
{
    return m_pTextViewData->m_isWrapContent;
}

//////////////////////////////////////////////////////////////////////////

SIZE SdkTextView::GetMeasureSize()
{
    SIZE measureSize = { (LONG)GetWidth(), (LONG)GetHeight() };

    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        TEXT_METRICS textMetrics = GetTextMetrics();
        measureSize.cx = (LONG)textMetrics.width;
        measureSize.cy = (LONG)textMetrics.height;
    }

    return measureSize;
}

//////////////////////////////////////////////////////////////////////////

TEXT_METRICS SdkTextView::GetTextMetrics()
{
    TEXT_METRICS textMetrics = { 0 };

    if ( NULL == m_pTextViewData->m_pD2DTextLayout )
    {
        return textMetrics;
    }

    // First calculate the text layout information, because before calling this
    // method, text alignment, size, font may have been changed.
    CalcTextLayoutInfo();

    IDWriteTextLayout *pDWriteTextLayout = NULL;
    m_pTextViewData->m_pD2DTextLayout->GetIDWriteTextLayout(&pDWriteTextLayout);

    if ( NULL != pDWriteTextLayout )
    {
        FLOAT fOldMaxW = pDWriteTextLayout->GetMaxWidth();
        FLOAT fOldMaxH = pDWriteTextLayout->GetMaxHeight();
        pDWriteTextLayout->SetMaxWidth(10000);
        pDWriteTextLayout->SetMaxHeight(10000);

        DWRITE_TEXT_METRICS dwriteTextMetrics  = { 0 };
        DWRITE_TEXT_METRICS dwriteTextMetrics2 = { 0 };

        // If want to get the width and height of text, the text should be
        // display fully, so we set the max width and max height a large value
        // so that the text can be hold fully, however, the left, top is not correct.
        pDWriteTextLayout->GetMetrics(&dwriteTextMetrics);
        pDWriteTextLayout->SetMaxWidth(fOldMaxW);
        pDWriteTextLayout->SetMaxHeight(fOldMaxH);
        // Call this method again to get text left, top(start point).
        pDWriteTextLayout->GetMetrics(&dwriteTextMetrics2);

        // Copy the values from DWRITE_TEXT_METRICS.
        textMetrics.width               = dwriteTextMetrics.width;
        textMetrics.height              = dwriteTextMetrics.height;
        textMetrics.left                = dwriteTextMetrics2.left;
        textMetrics.top                 = dwriteTextMetrics2.top;
        textMetrics.layoutWidth         = dwriteTextMetrics2.layoutWidth;
        textMetrics.layoutHeight        = dwriteTextMetrics2.layoutHeight;
        textMetrics.lineCount           = dwriteTextMetrics2.lineCount;
        textMetrics.maxBidiReorderingDepth = dwriteTextMetrics.maxBidiReorderingDepth;
        textMetrics.widthIncludingTrailingWhitespace = dwriteTextMetrics.widthIncludingTrailingWhitespace;
    }

    SAFE_RELEASE(pDWriteTextLayout);

    return textMetrics;
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::SetTextStartPoint(FLOAT x, FLOAT y)
{
    m_pTextViewData->m_txtStartPoint.x = (LONG)x;
    m_pTextViewData->m_txtStartPoint.y = (LONG)y;

    OnTextLayoutChanged();
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::CalcTextLayoutInfo()
{
    D2DTextLayout *pD2DTextLayout = m_pTextViewData->m_pD2DTextLayout;

    // If the flag indicate to need not to re-calculate text layout, or D2D text layout
    // is invalid, so do nothing.
    if ( !m_pTextViewData->m_isReCalcText ||  NULL == pD2DTextLayout )
    {
        return;
    }

    POINT outPt = { (LONG)GetLeft(), (LONG)GetTop() };
    GetAbsolutePoint(outPt);
    outPt.x += m_pTextViewData->m_txtStartPoint.x;
    outPt.y += m_pTextViewData->m_txtStartPoint.y;
    FLOAT fStartX = (FLOAT)outPt.x;
    FLOAT fStartY = (FLOAT)outPt.y;

    IDWriteTextLayout *pDWriteTextLayout = NULL;
    D2D1_COLOR_F textColor = IsEnable() ? m_pTextViewData->m_textColor : ColorF(ColorF::DarkGray);
    pD2DTextLayout->SetMaxWidth(GetWidth() - m_pTextViewData->m_txtStartPoint.x);
    pD2DTextLayout->SetMaxHeight(GetHeight() - m_pTextViewData->m_txtStartPoint.y);
    pD2DTextLayout->SetTextColor(textColor);
    pD2DTextLayout->GetIDWriteTextLayout(&pDWriteTextLayout);

    // Get the overhand metrics, so that we can set the text docks on any side.
    if ( NULL != pDWriteTextLayout )
    {
        // Get the overhang metrics.
        DWRITE_OVERHANG_METRICS overhangMetrics = { 0 };
        pDWriteTextLayout->GetOverhangMetrics(&overhangMetrics);

        switch ( m_pTextViewData->m_textDockDir )
        {
        case TEXT_DOCK_DIRECTION_TOP:
            fStartY += (overhangMetrics.top);
            break;

        case TEXT_DOCK_DIRECTION_BOTTOM:
            fStartY += (-overhangMetrics.bottom);
            break;

        case TEXT_DOCK_DIRECTION_LEFT:
            // Not implement.
            break;

        case TEXT_DOCK_DIRECTION_RIGHT:
            // Not implement.
            break;
        }
    }

    SAFE_RELEASE(pDWriteTextLayout);

    pD2DTextLayout->SetTextStartPoint(D2D1::Point2F(fStartX, fStartY));

    // After calculate text layout info, set the flag to FALSE to avoid re-calculating next time.
    m_pTextViewData->m_isReCalcText = FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::OnTextLayoutChanged()
{
    m_pTextViewData->m_isReCalcText = TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    // Call base class's onDrawItem to draw background color or background image.
    SdkViewElement::OnDrawItem(pRenderTarget);

    // Call this method to draw text.
    OnDrawText(pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void SdkTextView::OnDrawText(ID2D1RenderTarget *pRenderTarget)
{
    // Request layout, because text, size,etc may have been changed.
    // Set the device, do this at here because at this point we can get the target.
    if ( !m_pTextViewData->m_hasSetDevice )
    {
        m_pTextViewData->m_pD2DTextLayout->SetD2DDevice(
            D2DDevice::FromD2DRenderTarget(pRenderTarget));
        m_pTextViewData->m_hasSetDevice = TRUE;
    }

    if ( NULL != m_pTextViewData->m_pD2DTextLayout )
    {
        // Set the text layout information, such as the start point, because
        // we must specify the start point of the text layout.
        // In this method, we will check a member flag, if the flag is FALSE,
        // any operation will not perform.
        CalcTextLayoutInfo();

        // Set the color of text
        D2D1_COLOR_F textColor = IsEnable() ? m_pTextViewData->m_textColor : ColorF(ColorF::DarkGray);
        m_pTextViewData->m_pD2DTextLayout->SetTextColor(textColor);

        // Clip the overflow text when text view size is smaller the text metrics.
        if ( m_pTextViewData->m_isClipOverflowText )
        {
            D2D1_RECT_F absRc = { 0 };
            GetAbsoluteRect(absRc);

            // Push the clip.
            pRenderTarget->PushAxisAlignedClip(absRc, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            // Draw the text by calling method of m_pTextViewData->m_pD2DTextLayout member.
            m_pTextViewData->m_pD2DTextLayout->DrawText(pRenderTarget);
            // Pop the clip.
            pRenderTarget->PopAxisAlignedClip();
        }
        else
        {
            m_pTextViewData->m_pD2DTextLayout->DrawText(pRenderTarget);
        }
    }
}
