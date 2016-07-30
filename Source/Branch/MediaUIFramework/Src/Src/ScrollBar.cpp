/*!
* @file ScrollBar.cpp
* 
* @brief This file defines the ScrollBar class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Guo Jianbin
* @date 2011/02/12
*/

#include "ScrollBar.h"

#define SCROLLBAR_THUMB_MIN_SIZE       10      // The minimum size of scroll bar thumb.

ScrollBar::ScrollBar() : m_pTrackBrush(new D2DSolidColorBrush()),
                         m_pThumbBrush(new D2DSolidColorBrush()),
                         m_scrollOrientation(SCROLLBAR_ORIENTATION_VERTICAL),
                         m_scrollViewStyle(SCROLLBAR_VIEWSTYLE_SQUARE),
                         m_thumbPos(0),
                         m_scrollPage(0),
                         m_thumbPosRatio(0),
                         m_thumbRatio(0),
                         m_thumbColor(ColorF(ColorF::White)),
                         m_trackColor(ColorF(ColorF::Gray))
{
    ZeroMemory(&m_thumbSize, sizeof(D2D1_SIZE_F));
    SetOrientation(m_scrollOrientation);
    SetViewStyle(m_scrollViewStyle);
}

//////////////////////////////////////////////////////////////////////////

ScrollBar::~ScrollBar()
{
    SAFE_DELETE(m_pTrackBrush);
    SAFE_DELETE(m_pThumbBrush);
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::SetOrientation(SCROLLBAR_ORIENTATION orientation)
{
    if (m_scrollOrientation != orientation)
    {
        m_scrollOrientation = orientation;
        switch (orientation)
        {
        case SCROLLBAR_ORIENTATION_VERTICAL:
            SetViewSize(GetHeight(), GetWidth());
            break;

        case SCROLLBAR_ORIENTATION_HORIZONTAL:
            SetViewSize(GetHeight(), GetWidth());
            break;
        }

        CalcThumbSize();
    }
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::SetViewStyle(SCROLLBAR_VIEWSTYLE viewStyle)
{
    m_scrollViewStyle = viewStyle;
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::SetThumbPos(FLOAT fThumbPos)
{
    CalcThumbSize();
    m_thumbPos = ConvertThumbPos(fThumbPos);
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::SetThumbSizeRatio(FLOAT fSizeRatio)
{
    m_thumbRatio = fSizeRatio;
    if ( m_thumbRatio < 0 )
    {
        m_thumbRatio = 0;
    }
    else if ( m_thumbRatio > 1 )
    {
        m_thumbRatio = 1;
    }
    CalcThumbSize();
    CalcThumbPos();
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::SetThumbPosRatio(FLOAT fPosRatio)
{
    m_thumbPosRatio = fPosRatio;
    if ( m_thumbPosRatio < 0 )
    {
        m_thumbPosRatio = 0;
    }
    else if ( m_thumbPosRatio > 1 )
    {
        m_thumbPosRatio = 1;
    }
    //CalcThumbSize();
    m_thumbPos = (FLOAT)ConvertThumbPos2(fPosRatio);

}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::SetScrollPage(FLOAT fScrollPage)
{
    m_scrollPage = fScrollPage;
    CalcThumbSize();
    CalcThumbPos();
}

//////////////////////////////////////////////////////////////////////////

SCROLLBAR_ORIENTATION ScrollBar::GetScrollBarStyle()
{
    return m_scrollOrientation;
}

//////////////////////////////////////////////////////////////////////////

FLOAT ScrollBar::GetThumbPos()
{
    return m_thumbPos;
}

//////////////////////////////////////////////////////////////////////////

FLOAT ScrollBar::GetScrollPage()
{
    return m_scrollPage;
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::SetThumbColor(const ColorF &color)
{
    m_thumbColor = color;
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::SetTrackColor(const ColorF &color)
{
    m_trackColor = color;
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    ViewElement::SetLayoutInfo(x, y, width, height);
    //CalcThumbSize();
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::CalcThumbSize()
{
    FLOAT width = GetWidth();
    FLOAT height = GetHeight();
    FLOAT thumbWidth = width;
    FLOAT thumbHeight = height;

    switch (m_scrollOrientation)
    {
    case SCROLLBAR_ORIENTATION_VERTICAL:
        if (0 != m_scrollPage)
        {
            m_scrollPage = (m_scrollPage < height || m_scrollPage <= 0) ? height : m_scrollPage;
            m_thumbRatio = height / m_scrollPage;
        }

        if (0 != m_thumbRatio)
        {
            thumbHeight = height * m_thumbRatio;
        }
        break;


    case SCROLLBAR_ORIENTATION_HORIZONTAL:
        if (0 != m_scrollPage)
        {
            m_scrollPage = (m_scrollPage < width || m_scrollPage <= 0) ? width : m_scrollPage;
            m_thumbRatio = width / m_scrollPage;
        }

        if (0 != m_thumbRatio)
        {
            thumbWidth = width * m_thumbRatio;
        }
        break;
    }

    m_thumbSize.width = thumbWidth;
    m_thumbSize.height = thumbHeight;
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::CalcThumbPos()
{
    switch (m_scrollOrientation)
    {
    case SCROLLBAR_ORIENTATION_HORIZONTAL:
        {
            if (m_thumbPos + m_thumbSize.width > GetWidth())
            {
                m_thumbPos = GetWidth() - m_thumbSize.width;
            }
        }
        break;

    case SCROLLBAR_ORIENTATION_VERTICAL:
        {
            if (m_thumbPos + m_thumbSize.height > GetHeight())
            {
                m_thumbPos = GetHeight() - m_thumbSize.height;
            }
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

FLOAT ScrollBar::ConvertThumbPos(FLOAT fThumbPos)
{
    switch(m_scrollOrientation)
    {
    case SCROLLBAR_ORIENTATION_VERTICAL:
        {
            fThumbPos = fThumbPos * m_thumbRatio;
        }
        break;

    case SCROLLBAR_ORIENTATION_HORIZONTAL:
        {
            fThumbPos = fThumbPos * m_thumbRatio;
        }
        break;
    }

    return fThumbPos;
}

//////////////////////////////////////////////////////////////////////////

FLOAT ScrollBar::ConvertThumbPos2(FLOAT fThumbPosRatio)
{
    FLOAT width = GetWidth();
    FLOAT height = GetHeight();
    FLOAT convertThumbPos = 0.0f;
    switch(m_scrollOrientation)
    {
    case SCROLLBAR_ORIENTATION_VERTICAL:
        {
            convertThumbPos = (height * fThumbPosRatio);
        }
        break;

    case SCROLLBAR_ORIENTATION_HORIZONTAL:
        {
            convertThumbPos = (width * fThumbPosRatio);
        }
        break;
    }

    return convertThumbPos;
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F ScrollBar::CalcThumbRect(const D2D1_RECT_F &absRc)
{
    D2D1_RECT_F destRc = absRc;

    switch (m_scrollOrientation)
    {
    case SCROLLBAR_ORIENTATION_HORIZONTAL:
        {
            destRc.left += m_thumbPos;
            destRc.right = destRc.left + m_thumbSize.width;

            destRc.left  = max((FLOAT)(destRc.left), (FLOAT)(absRc.left));
            destRc.left  = min((FLOAT)(destRc.left), (FLOAT)(absRc.right - SCROLLBAR_THUMB_MIN_SIZE));
            destRc.right = max((FLOAT)(destRc.right), (FLOAT)(absRc.left + SCROLLBAR_THUMB_MIN_SIZE));
            destRc.right = min((FLOAT)(destRc.right), (FLOAT)(absRc.right));
        }
        break;

    case SCROLLBAR_ORIENTATION_VERTICAL:
        {
            destRc.top += m_thumbPos;
            destRc.bottom = destRc.top + m_thumbSize.height;

            destRc.top    = max((FLOAT)(destRc.top), (FLOAT)(absRc.top));
            destRc.top    = min((FLOAT)(destRc.top), (FLOAT)(absRc.bottom - SCROLLBAR_THUMB_MIN_SIZE));
            destRc.bottom = max((FLOAT)(destRc.bottom), (FLOAT)(absRc.top + SCROLLBAR_THUMB_MIN_SIZE));
            destRc.bottom = min((FLOAT)(destRc.bottom), (FLOAT)(absRc.bottom));
        }
        break;
    }

    return destRc;
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::OnPaint()
{
    if ( m_thumbRatio >= 1 || m_thumbRatio <= 0 )
    {
        return;
    }

    if ( m_thumbPosRatio < 0 || m_thumbRatio < 0 )
    {
        return;
    }

    ViewElement::OnPaint();
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    if ( m_thumbPosRatio < 0.0f )
    {
        return;
    }

    if ( m_thumbRatio < 0.0f )
    {
        return;
    }
    ViewElement::OnDrawItem(pRenderTarget);
    OnDrawTrack(pRenderTarget);
    OnDrawThumb(pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::OnDrawTrack(ID2D1RenderTarget *pRenderTarget)
{
    if (!m_pTrackBrush->HasCreatedBrush())
    {
        m_pTrackBrush->CreateBrush(pRenderTarget);
    }

    ID2D1Brush *pD2DBrush = NULL;
    m_pTrackBrush->SetColor(m_trackColor);
    m_pTrackBrush->GetD2DBrush(&pD2DBrush);

    if (NULL != pD2DBrush)
    {
        D2D1_RECT_F absRc = { 0 };
        GetAbsoluteRect(absRc);
        OnDrawRect(pRenderTarget, absRc, pD2DBrush);
    }

    SAFE_RELEASE(pD2DBrush);
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::OnDrawThumb(ID2D1RenderTarget *pRenderTarget)
{
    if (!m_pThumbBrush->HasCreatedBrush())
    {
        m_pThumbBrush->CreateBrush(pRenderTarget);
    }

    ID2D1Brush *pD2DBrush = NULL;
    m_pThumbBrush->SetColor(m_thumbColor);
    m_pThumbBrush->GetD2DBrush(&pD2DBrush);

    if (NULL != pD2DBrush)
    {
        D2D1_RECT_F absRc = { 0 };
        GetAbsoluteRect(absRc);
        OnDrawRect(pRenderTarget, CalcThumbRect(absRc), pD2DBrush);
    }

    SAFE_RELEASE(pD2DBrush);
}

//////////////////////////////////////////////////////////////////////////

void ScrollBar::OnDrawRect(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F &destRc, ID2D1Brush *pBrush)
{
    if (NULL == pRenderTarget || NULL == pBrush)
    {
        return;
    }

    if (SCROLLBAR_VIEWSTYLE_ROUNDED == m_scrollViewStyle)
    {
        FLOAT radiuXY = 0.0f;
        switch (m_scrollOrientation)
        {
        case SCROLLBAR_ORIENTATION_VERTICAL:
            radiuXY = GetWidth() / 2;
            break;

        case SCROLLBAR_ORIENTATION_HORIZONTAL:
            radiuXY = GetHeight() / 2;
            break;
        }

        pRenderTarget->FillRoundedRectangle(RoundedRect(destRc, radiuXY, radiuXY), pBrush);
    }
    else
    {
        pRenderTarget->FillRectangle(destRc, pBrush);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ScrollBar::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    if (fChanged)
    {
        CalcThumbSize();
        m_thumbPos = (FLOAT)ConvertThumbPos2(m_thumbPosRatio);
    }
}
