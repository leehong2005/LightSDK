/*!
* @file SdkSeekBar.cpp
* 
* @brief This file defines the header file of the seekBar.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/07/04
*/

#include "stdafx.h"
#include "SdkSeekBar.h"
#include "Resource.h"
#include "SdkResManager.h"
#include "SdkD2DTheme.h"
#include "D2DRectUtility.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_VIEWS

/*!
* @brief The internal data of seek bar.
*/
struct NAMESPACE_VIEWS::SdkSeekBar::_SEEKBAR_INTERNALDATA
{
    BOOL                     m_isInThumbRect;        // Indicates the point is in thumb rectangle.
    FLOAT                    m_fMinRange;            // The minimum range.
    FLOAT                    m_fMaxRange;            // The maximum range.
    D2D1_POINT_2F            m_ptPrevPress;          // The point previous pressed.
    D2DBitmap               *m_pThumbBitmap;         // The thumb bitmap.
};


//////////////////////////////////////////////////////////////////////////

SdkSeekBar::SdkSeekBar()
{
    m_pSeekBarData = new _SEEKBAR_INTERNALDATA();
    ZeroMemory(m_pSeekBarData, sizeof(_SEEKBAR_INTERNALDATA));

    SetClassName(CLASSNAME_SEEKBAR);

    m_pSeekBarData->m_ptPrevPress.x     = 0;
    m_pSeekBarData->m_ptPrevPress.y     = 0;
    m_pSeekBarData->m_fMaxRange         = 100;
    m_pSeekBarData->m_pThumbBitmap      = new D2DBitmap();

    m_pSeekBarData->m_pThumbBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_SEEKBAR_THUMB));
}

/////////////////////////////////////////////////////////////////////////

SdkSeekBar::~SdkSeekBar()
{
    SAFE_DELETE(m_pSeekBarData->m_pThumbBitmap);
    SAFE_DELETE(m_pSeekBarData);
}

//////////////////////////////////////////////////////////////////////////

void SdkSeekBar::SetRange(FLOAT fMin, FLOAT fMax)
{
    fMin = max(0, fMin);
    fMax = fMin;
}

//////////////////////////////////////////////////////////////////////////

void SdkSeekBar::GetRange(FLOAT *pMin, FLOAT *pMax) const
{
    if ( NULL != pMin )
    {
        *pMin = m_pSeekBarData->m_fMinRange;
    }

    if ( NULL != pMax )
    {
        *pMax = m_pSeekBarData->m_fMaxRange;
    }
}

//////////////////////////////////////////////////////////////////////////\

SIZE SdkSeekBar::GetThumbSize() const
{
    LONG thumbW = 15;
    LONG thumbH = 15;

    if ( NULL != m_pSeekBarData->m_pThumbBitmap )
    {
        thumbW = (LONG)m_pSeekBarData->m_pThumbBitmap->GetWidth();
        thumbH = (LONG)m_pSeekBarData->m_pThumbBitmap->GetHeight();
    }

    SIZE size = { thumbW, thumbH };

    return size;
}

//////////////////////////////////////////////////////////////////////////

void SdkSeekBar::SetThumbPos(FLOAT xPos, FLOAT yPos)
{
    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);

    xPos -= rc.left;
    FLOAT fPercent = (xPos / GetWidth()) * 100.0f;

    SetPercent(fPercent);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkSeekBar::IsPtInThumbRect(FLOAT xPos, FLOAT yPos)
{
    D2D1_POINT_2F srcPT = D2D1::Point2F(xPos, yPos);
    D2D1_POINT_2F desPT = ConvertPoint(srcPT, FALSE);

    // Get the current rectangle of thumb.
    D2D1_RECT_F rcThumb = CalcThumbRect();

    if ( D2DRectUtility::PtInD2DRect(rcThumb, desPT) )
    {
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F SdkSeekBar::CalcThumbRect()
{
    SIZE thumbSize = GetThumbSize();
    FLOAT thumbW = (FLOAT)thumbSize.cx;
    FLOAT thumbH = (FLOAT)thumbSize.cy;

    FLOAT fWidth = (GetPercent() / 100.0f) * (GetWidth() - thumbW);

    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);

    rc.left += fWidth;
    rc.right = rc.left + thumbW;
    rc.top += ((GetHeight() - thumbH) / 2);
    rc.bottom = rc.top + thumbH;

    return rc;
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F SdkSeekBar::OnCalcTrackRect()
{
    D2D1_RECT_F rc = SdkProgressBar::OnCalcTrackRect();

    // Make the track rectangle shrink.
    SIZE thumbSize = GetThumbSize();
    const FLOAT thumbW = (FLOAT)thumbSize.cx;
    rc.left  += thumbW / 2;
    rc.right -= thumbW / 2;

    return rc;
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkSeekBar::OnGetProgressWidth()
{
    SIZE thumbSize = GetThumbSize();
    FLOAT fWidth = (GetPercent() / 100.0f) * (GetWidth() - thumbSize.cx);

    return fWidth;
}

//////////////////////////////////////////////////////////////////////////

void SdkSeekBar::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    SdkProgressBar::OnDrawItem(pRenderTarget);

    D2D1_RECT_F rc = OnCalcTrackRect();
    FLOAT fWidth = OnGetProgressWidth();
    rc.right = rc.left + fWidth;

    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
    // Draw the thumb bitmap.
    pD2DTheme->OnDrawProgressBarThumb(
        this,
        pRenderTarget,
        m_pSeekBarData->m_pThumbBitmap,
        GetPercent(),
        rc);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkSeekBar::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL handled = TRUE;

    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
    BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));

    switch (message)
    {
    case WM_LBUTTONDOWN:
        {
            m_pSeekBarData->m_isInThumbRect = IsPtInThumbRect(xPos, yPos);
            // The the point is not in the thumb rectangle, move thumb to the current position.
            if ( !m_pSeekBarData->m_isInThumbRect )
            {
                SetThumbPos(xPos, yPos);
            }
        }
        break;

    case WM_LBUTTONUP:
        {
            m_pSeekBarData->m_isInThumbRect = FALSE;
        }
        break;

    case WM_MOUSEMOVE:
        {
            // If the mouse left button is press and the cursor is pressed in the thumb rectangle.
            if ( isPressed && m_pSeekBarData->m_isInThumbRect )
            {
                SetThumbPos(xPos, yPos);
            }
        }
        break;

    default:
        handled = FALSE;
        break;
    }

    SdkViewElement::OnMouseEvent(message, wParam, lParam);

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkSeekBar::OnKeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    return TRUE;
}
