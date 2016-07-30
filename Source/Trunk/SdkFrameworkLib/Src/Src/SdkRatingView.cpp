/*!
* @file SdkRatingView.cpp
* 
* @brief This file defines methods to show rating for media files.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/08
*/

#include "stdafx.h"
#include "SdkRatingView.h"
#include "SdkD2DTheme.h"
#include "SdkResManager.h"
#include "D2DRectUtility.h"
#include "IRatingViewEventHandler.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_VIEWS

#define MARGIN_HORIZONTAL       5                           // The horizontal margin.
#define STARS_MIN_SIZE          20                          // The minimum size of stars.
#define STARS_MAX_SIZE          32                          // The maximum size of stars.

/*!
* @brief The internal data of rating view.
*/
struct NAMESPACE_VIEWS::SdkRatingView::_RATINGVIEW_INTERNALDATA
{
    int                         m_nFocusStarIndex;          // The focus star index (mouse hover or press).
    BOOL                        m_isAutoSize;               // Auto size or not.
    UINT32                      m_nStarsNumber;             // The ratingBar stars number.
    UINT32                      m_nRating;                  // The ratingBar rate value.
    FLOAT                       m_fStarsSize;               // The size of stars.
    D2DBitmap                  *m_pNormalBitmap;            // The normal bitmap.
    D2DBitmap                  *m_pNormalHoverBitmap;       // The normal hover bitmap.
    D2DBitmap                  *m_pNormalPressBitmap;       // The normal press bitmap.
    D2DBitmap                  *m_pCheckBitmap;             // The check bitmap.
    D2DBitmap                  *m_pCheckHoverBitmap;        // The check hover bitmap.
    D2DBitmap                  *m_pCheckPressBitmap;        // The check press bitmap.
    IRatingViewEventHandler    *m_pRatingEventHandler;      // The event handler.
    ID2D1BitmapRenderTarget    *m_pBitmapRenderTarget;      // The bitmap compatible renderTarget.Use it send bitmap to renderTarget.
};


//////////////////////////////////////////////////////////////////////////

SdkRatingView::SdkRatingView()
{
    m_pRatingViewData = new _RATINGVIEW_INTERNALDATA();
    ZeroMemory(m_pRatingViewData, sizeof(_RATINGVIEW_INTERNALDATA));

    m_pRatingViewData->m_nFocusStarIndex    = -1;
    m_pRatingViewData->m_fStarsSize         = 25;
    m_pRatingViewData->m_nStarsNumber       = 5;
    m_pRatingViewData->m_isAutoSize         = TRUE;
    m_pRatingViewData->m_pNormalBitmap      = new D2DBitmap();
    m_pRatingViewData->m_pCheckBitmap       = new D2DBitmap();
    m_pRatingViewData->m_pNormalHoverBitmap = new D2DBitmap();
    m_pRatingViewData->m_pNormalPressBitmap = new D2DBitmap();
    m_pRatingViewData->m_pCheckHoverBitmap  = new D2DBitmap();
    m_pRatingViewData->m_pCheckPressBitmap  = new D2DBitmap();

    m_pRatingViewData->m_pNormalBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RATING_NORMAL));
    m_pRatingViewData->m_pCheckBitmap ->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RATING_CHECK));
    m_pRatingViewData->m_pNormalHoverBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RATING_NORMAL_HOVER));
    m_pRatingViewData->m_pNormalPressBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RATING_NORMAL_PRESS));
    m_pRatingViewData->m_pCheckHoverBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RATING_CHECK_HOVER));
    m_pRatingViewData->m_pCheckPressBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RATING_CHECK_PRESS));

    SetClassName(CLASSNAME_RATINGVIEW);
    SetMinHeight(20);
    SetMaxHeight(50);
    SetViewSize(155, 30);
}

//////////////////////////////////////////////////////////////////////////

SdkRatingView::~SdkRatingView()
{
    SAFE_DELETE(m_pRatingViewData->m_pNormalBitmap);
    SAFE_DELETE(m_pRatingViewData->m_pCheckBitmap);
    SAFE_DELETE(m_pRatingViewData->m_pNormalHoverBitmap);
    SAFE_DELETE(m_pRatingViewData->m_pNormalPressBitmap);
    SAFE_DELETE(m_pRatingViewData->m_pCheckHoverBitmap);
    SAFE_DELETE(m_pRatingViewData->m_pCheckPressBitmap);

    SAFE_RELEASE(m_pRatingViewData->m_pBitmapRenderTarget);

    SAFE_DELETE(m_pRatingViewData);
}

//////////////////////////////////////////////////////////////////////////

void SdkRatingView::SetStarsCount(IN UINT32 numStars)
{
    if ( m_pRatingViewData->m_nStarsNumber != numStars )
    {
        m_pRatingViewData->m_nStarsNumber = numStars;

        // The rating number should be less or equal the stars number.
        m_pRatingViewData->m_nRating = min(
            m_pRatingViewData->m_nRating,
            m_pRatingViewData->m_nStarsNumber);

        ResetLayoutInfo();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkRatingView::SetRating(IN UINT32 rating)
{
    rating = min(rating, m_pRatingViewData->m_nStarsNumber);

    // If the rating is changed, update window.
    if ( m_pRatingViewData->m_nRating != rating )
    {
        UINT32 nOldRating = m_pRatingViewData->m_nRating;

        m_pRatingViewData->m_nRating = rating;

        if ( NULL != m_pRatingViewData->m_pRatingEventHandler )
        {
            m_pRatingViewData->m_pRatingEventHandler->OnRatingChanged(
                this,
                nOldRating,
                rating);
        }

        Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkRatingView::SetAutoSize(BOOL fAutoSize)
{
    if ( m_pRatingViewData->m_isAutoSize != fAutoSize )
    {
        m_pRatingViewData->m_isAutoSize = fAutoSize;

        ResetLayoutInfo();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkRatingView::SetStarsSize(FLOAT fStarSize)
{
    // The stars size is in the range from 20 to 32.
    fStarSize = max(STARS_MIN_SIZE, fStarSize);
    fStarSize = min(STARS_MAX_SIZE, fStarSize);

    if ( m_pRatingViewData->m_fStarsSize != fStarSize )
    {
        m_pRatingViewData->m_fStarsSize = fStarSize;

        ResetLayoutInfo();
    }
}

//////////////////////////////////////////////////////////////////////////

UINT32 SdkRatingView::GetRating() const
{
    return m_pRatingViewData->m_nRating;
}

//////////////////////////////////////////////////////////////////////////

void SdkRatingView::SetRatingEventHandler(IRatingViewEventHandler *pEventHandler)
{
    m_pRatingViewData->m_pRatingEventHandler = pEventHandler;
}

//////////////////////////////////////////////////////////////////////////

void SdkRatingView::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    UINT32 nStarsNumber = m_pRatingViewData->m_nStarsNumber;
    // Calculate the new size of the this view according to the number of stars.
    FLOAT fWidth = (FLOAT)(nStarsNumber * (m_pRatingViewData->m_fStarsSize + MARGIN_HORIZONTAL) +
                           MARGIN_HORIZONTAL);

    width = m_pRatingViewData->m_isAutoSize ? fWidth : max(width, fWidth);

    // The height can NOT be less than star's size.
    height = max(height, m_pRatingViewData->m_fStarsSize);

    SdkViewElement::SetLayoutInfo(x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////

void SdkRatingView::ResetLayoutInfo()
{
    // The start number is changed, so it will lead resizing the view, so release
    // the bitmap render target, it is re-created in the OnDrawItem method.
    SAFE_RELEASE(m_pRatingViewData->m_pBitmapRenderTarget);

    // Reset the size of the view to make the view can hold all stars.
    RequestLayout();
}

//////////////////////////////////////////////////////////////////////////

int SdkRatingView::GetStarIndexFromPoint(FLOAT xPos, FLOAT yPos)
{
    int uStarIndex = 0;

    // Convert the specified point to another point relative the view, the new point
    // is got through multiple the invert matrix of the view's current matrix.
    // The returned point is relative to view's bound.
    D2D1_POINT_2F newPt = ConvertPoint(D2D1::Point2F(xPos, yPos));
    D2D1_RECT_F viewRc = GetViewRect();
    D2DRectUtility::InflateD2DRectF(
        viewRc,
        MARGIN_HORIZONTAL,
        (GetHeight() - m_pRatingViewData->m_fStarsSize) / 2);

    uStarIndex = (int)(newPt.x / (MARGIN_HORIZONTAL + m_pRatingViewData->m_fStarsSize)) + 1;

    return uStarIndex;
}

//////////////////////////////////////////////////////////////////////////

D2DBitmap* SdkRatingView::GetDrawingBitmap(int nStarIndex)
{
    D2DBitmap *pD2DBitmap = m_pRatingViewData->m_pNormalBitmap;

    if ( nStarIndex >= 0 && nStarIndex < (int)m_pRatingViewData->m_nStarsNumber )
    {
        // If the star to be drawing is the focus star
        if ( (nStarIndex + 1) == m_pRatingViewData->m_nFocusStarIndex )
        {
            // If current mouse is pressed.
            BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
            if ( isPressed )
            {
                // Draw the check press or normal press star.
                pD2DBitmap = (nStarIndex < (int)m_pRatingViewData->m_nRating) ?
                    m_pRatingViewData->m_pCheckPressBitmap :
                    m_pRatingViewData->m_pNormalPressBitmap;
            }
            else
            {
                // Draw the check hover or normal hover star
                pD2DBitmap = (nStarIndex < (int)m_pRatingViewData->m_nRating) ? 
                    m_pRatingViewData->m_pCheckHoverBitmap : 
                    m_pRatingViewData->m_pNormalHoverBitmap;
            }
        }
        else
        {
            // Draw the check or normal star.
            pD2DBitmap = (nStarIndex < (int)m_pRatingViewData->m_nRating) ?
                m_pRatingViewData->m_pCheckBitmap :
                m_pRatingViewData->m_pNormalBitmap;
        }
    }

    return pD2DBitmap;
}

//////////////////////////////////////////////////////////////////////////

void SdkRatingView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    // Draw base class, such as background color.
    SdkViewElement::OnDrawItem(pRenderTarget);

    // Create compatible render target.
    if ( NULL == m_pRatingViewData->m_pBitmapRenderTarget )
    {
        HRESULT hr = pRenderTarget->CreateCompatibleRenderTarget(
            D2D1::SizeF(GetWidth(), GetHeight()),
            &(m_pRatingViewData->m_pBitmapRenderTarget));

        // Fail to create compatible render target.
        if ( FAILED(hr) )
        {
            return;
        }
    }

    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();

    ID2D1BitmapRenderTarget *pBitmapRenderTarget = m_pRatingViewData->m_pBitmapRenderTarget;

    // Draw the stars on the compatible render target.
    pBitmapRenderTarget->BeginDraw();
    pBitmapRenderTarget->Clear(ColorF(0, 0));

    const FLOAT fOffsetX = (FLOAT)(MARGIN_HORIZONTAL + m_pRatingViewData->m_fStarsSize);
    D2D1_RECT_F rc = { 0 };
    rc.left = MARGIN_HORIZONTAL;
    rc.top  = 0;
    rc.bottom = rc.top + GetHeight();
    rc.right = rc.left + m_pRatingViewData->m_fStarsSize;

    for (UINT32 i = 0; i < m_pRatingViewData->m_nStarsNumber; ++i)
    {
        // Translate the target to draw these subsequent stars.
        pBitmapRenderTarget->SetTransform(Matrix3x2F::Translation(fOffsetX * i, 0.0f));
        // Draw the bitmap to the compatible render target.
        pD2DTheme->OnDrawRatingView(
            this,
            pBitmapRenderTarget,
            GetDrawingBitmap(i),
            m_pRatingViewData->m_fStarsSize,
            rc);
    }

    // Set the identity matrix as the transform of render target.
    pBitmapRenderTarget->SetTransform(Matrix3x2F::Identity());
    pBitmapRenderTarget->EndDraw();

    // Draw the bitmap from compatible render target to the display render target.
    ID2D1Bitmap *pMemBitmap = NULL;
    HRESULT hr = pBitmapRenderTarget->GetBitmap(&pMemBitmap);

    if ( SUCCEEDED(hr) )
    {
        GetAbsoluteRect(rc);
        // Draw the bitmap which is retrieved from compatible render target to the render target.
        pRenderTarget->DrawBitmap(pMemBitmap, rc);
    }

    SAFE_RELEASE(pMemBitmap);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkRatingView::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL handled = TRUE;

    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
    BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
    m_pRatingViewData->m_nFocusStarIndex = -1;

    switch (message)
    {
    case WM_LBUTTONDOWN:
        {
            m_pRatingViewData->m_nFocusStarIndex = GetStarIndexFromPoint(xPos, yPos);
            Invalidate();
        }
        break;

    case WM_MOUSELEAVE:
        {
            Invalidate();
        }
        break;

    case WM_MOUSEMOVE:
        {
            if ( !isPressed )
            {
                m_pRatingViewData->m_nFocusStarIndex = GetStarIndexFromPoint(xPos, yPos);
                Invalidate();
            }
        }
        break;

    case WM_LBUTTONUP:
        {
            BOOL isTouchOn = IsPtInRect(xPos, yPos);
            if ( isPressed && isTouchOn )
            {
                UINT32 uStarIndex = GetStarIndexFromPoint(xPos, yPos);
                uStarIndex = (uStarIndex == (int)m_pRatingViewData->m_nRating) ? ((0 == uStarIndex) ? 1 : (uStarIndex - 1)) : uStarIndex;
                SetRating(uStarIndex);
            }

            m_pRatingViewData->m_nFocusStarIndex = -1;
            Invalidate();
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

void SdkRatingView::OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);

    SAFE_RELEASE(m_pRatingViewData->m_pBitmapRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void SdkRatingView::OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal)
{
    UNREFERENCED_PARAMETER(pDevice);
    UNREFERENCED_PARAMETER(stateVal);

    SAFE_RELEASE(m_pRatingViewData->m_pBitmapRenderTarget);
}
