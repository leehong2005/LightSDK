/*!
* @file SdkD2DTheme.cpp
* 
* @brief This file defines SdkD2DTheme class for managing the drawing for views.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/22
*/

#include "stdafx.h"
#include "SdkD2DTheme.h"
#include "D2DRectUtility.h"
#include "SdkViewElement.h"
#include "SdkCommonInclude.h"

USING_NAMESPACE_THEME

SdkD2DTheme* SdkD2DTheme::s_pD2DTheme = NULL;

//////////////////////////////////////////////////////////////////////////

BOOL SdkD2DTheme::CreateD2DThemeInstance()
{
    if ( NULL == s_pD2DTheme)
    {
        s_pD2DTheme = new SdkD2DTheme();
    }

    return (NULL != s_pD2DTheme);
}

//////////////////////////////////////////////////////////////////////////

SdkD2DTheme* SdkD2DTheme::GetD2DThemeInstance()
{
    if ( NULL == s_pD2DTheme)
    {
        s_pD2DTheme = new SdkD2DTheme();
    }

    return s_pD2DTheme;
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::DeleteD2DThemeInstance()
{
    SAFE_DELETE(s_pD2DTheme);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnPushClip(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    ID2D1Layer *pLayer,
    FLOAT fRadiusX,
    FLOAT fRadiusY)
{
    INT32 nState = pView->GetState();
    BOOL isDrawRC = (VIEW_STATE_ROUNDCORNERENABLE == (nState & VIEW_STATE_ROUNDCORNERENABLE));
    BOOL isClip   = (VIEW_STATE_CLIPVIEW == (nState & VIEW_STATE_CLIPVIEW));

    if ( isDrawRC && isClip )
    {
        D2D1_RECT_F rc = { 0 };
        pView->GetAbsoluteRect(rc);

        ID2D1Factory *pD2DFactory = NULL;
        ID2D1RoundedRectangleGeometry *pRoundRcGeometry = NULL;

        HRESULT hr = pView->GetWindow()->GetD2DDevices()->GetD2DFactory(&pD2DFactory);
        if ( SUCCEEDED(hr) )
        {
            D2D1_ROUNDED_RECT roundRc = { rc, fRadiusX, fRadiusY };
            hr = pD2DFactory->CreateRoundedRectangleGeometry(roundRc, &pRoundRcGeometry);
            if ( SUCCEEDED(hr) )
            {
                pRT->PushLayer(
                    LayerParameters(D2D1::InfiniteRect(), pRoundRcGeometry),
                    pLayer);
            }
        }

        SAFE_RELEASE(pD2DFactory);
        SAFE_RELEASE(pRoundRcGeometry);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnPopClip(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT)
{
    INT32 nState = pView->GetState();
    BOOL isDrawRC = (VIEW_STATE_ROUNDCORNERENABLE == (nState & VIEW_STATE_ROUNDCORNERENABLE));
    BOOL isClip   = (VIEW_STATE_CLIPVIEW == (nState & VIEW_STATE_CLIPVIEW));

    if ( isDrawRC && isClip )
    {
        pRT->PopLayer();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawBackground(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBrush *pBrush,
    const D2D1_RECT_F& rc,
    FLOAT fRadiusX,
    FLOAT fRadiusY)
{
    UNREFERENCED_PARAMETER(pView);

    if ( !pBrush->HasCreatedBrush() )
    {
        pBrush->CreateBrush(pRT);
    }

    ID2D1Brush *pD2DBrush = NULL;
    pBrush->GetD2DBrush(&pD2DBrush);

    BOOL isDrawRC = (fRadiusX > 0 || fRadiusY > 0);
    if ( isDrawRC )
    {
        D2D1_ROUNDED_RECT roundRc = { rc, fRadiusX, fRadiusY };
        pRT->FillRoundedRectangle(roundRc, pD2DBrush);
    }
    else
    {
        pRT->FillRectangle(rc, pD2DBrush);
    }

    SAFE_RELEASE(pD2DBrush);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawBackground(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    const D2D1_RECT_F& rc)
{
    UNREFERENCED_PARAMETER(pView);

    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        pRT->DrawBitmap(pD2DBitmap, rc);
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawBorder(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBrush *pBorderBrush,
    const D2D1_RECT_F& rc,
    FLOAT fBorderW,
    FLOAT fRadiusX,
    FLOAT fRadiusY)
{
    UNREFERENCED_PARAMETER(pView);

    if ( !pBorderBrush->HasCreatedBrush() )
    {
        pBorderBrush->CreateBrush(pRT);
    }

    ID2D1Brush *pD2DBrush = NULL;
    pBorderBrush->GetD2DBrush(&pD2DBrush);

    BOOL isDrawRC = (fRadiusX > 0 || fRadiusY > 0);
    if ( isDrawRC )
    {
        D2D1_ROUNDED_RECT roundRc = { rc, fRadiusX, fRadiusY };
        pRT->DrawRoundedRectangle(roundRc, pD2DBrush, fBorderW);
    }
    else
    {
        pRT->DrawRectangle(rc, pD2DBrush, fBorderW);
    }

    SAFE_RELEASE(pD2DBrush);
} 

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawBitmap(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    const D2D1_RECT_F& rc)
{
    UNREFERENCED_PARAMETER(pView);

    if ( NULL == pBitmap )
    {
        return;
    }

    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        pRT->DrawBitmap(pD2DBitmap, rc);
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawOverlayBitmap(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    const D2D1_RECT_F& rc)
{
    if ( NULL != pView )
    {
        // Calculate the absolute rectangle.
        D2D1_RECT_F rcAbs = rc;
        D2D1_POINT_2F ptAbs = { pView->GetLeft(), pView->GetTop() };
        pView->GetAbsolutePoint(ptAbs);
        D2DRectUtility::OffsetD2DRectF(rcAbs, ptAbs.x - 1, ptAbs.y);

        // Draw the bitmap.
        OnDrawBitmap(pView, pRT, pBitmap, rcAbs);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawButton(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    const D2D1_RECT_F& rc)
{
    UNREFERENCED_PARAMETER(pView);

    if ( NULL == pBitmap )
    {
        return;
    }

    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        FLOAT fRCRadiusX = 5;
        FLOAT fRCRadiusY = 5;
        D2D1_SIZE_F size = pD2DBitmap->GetSize();

        // Left-top round corner.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left,
            rc.top,
            fRCRadiusX,
            fRCRadiusY,
            0,
            0,
            fRCRadiusX,
            fRCRadiusY);

        // Left-bottom round corner.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left,
            rc.bottom - fRCRadiusY,
            fRCRadiusX,
            fRCRadiusY,
            0,
            size.height - fRCRadiusY,
            fRCRadiusX,
            fRCRadiusY);

        // Right-top round corner.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.right - fRCRadiusX,
            rc.top,
            fRCRadiusX,
            fRCRadiusY,
            size.width - fRCRadiusX,
            0,
            fRCRadiusX,
            fRCRadiusY);

        // Right-bottom round corner.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.right - fRCRadiusX,
            rc.bottom - fRCRadiusY,
            fRCRadiusX,
            fRCRadiusY,
            size.width - fRCRadiusX,
            size.height - fRCRadiusY,
            fRCRadiusX,
            fRCRadiusY);

        // Left side.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left,
            rc.top + fRCRadiusY,
            fRCRadiusX,
            (rc.bottom - rc.top) - 2 * fRCRadiusY,
            0,
            fRCRadiusY,
            fRCRadiusX,
            size.height - 2 * fRCRadiusY);

        // Top side.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left + fRCRadiusX,
            rc.top,
            (rc.right - rc.left) - 2 * fRCRadiusX,
            fRCRadiusY,
            fRCRadiusY,
            0,
            size.width - 2 * fRCRadiusX,
            fRCRadiusY);

        // Right side.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.right - fRCRadiusX,
            rc.top + fRCRadiusY,
            fRCRadiusX,
            (rc.bottom - rc.top) - 2 * fRCRadiusY,
            size.width - fRCRadiusX,
            fRCRadiusY,
            fRCRadiusX,
            size.height - 2 * fRCRadiusY);

        // Bottom side.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left + fRCRadiusX,
            rc.bottom - fRCRadiusY,
            (rc.right - rc.left) - 2 * fRCRadiusX,
            fRCRadiusY,
            fRCRadiusX,
            size.height - fRCRadiusY,
            size.width - 2 * fRCRadiusX,
            fRCRadiusY);

        // Center part.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left + fRCRadiusX,
            rc.top + fRCRadiusY,
            (rc.right - rc.left) - 2 * fRCRadiusX,
            (rc.bottom - rc.top) - 2 * fRCRadiusY,
            fRCRadiusX,
            fRCRadiusY,
            size.width - 2 * fRCRadiusX,
            size.height - 2 * fRCRadiusY);
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawComboBox(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    const D2D1_RECT_F& rc)
{
    if ( NULL == pBitmap )
    {
        return;
    }

    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        FLOAT fRCRadiusX = 5;
        FLOAT fRCRadiusY = 5;
        FLOAT fRightSize = 30;
        D2D1_SIZE_F size = pD2DBitmap->GetSize();

        UNREFERENCED_PARAMETER(fRCRadiusY);

        // Left side.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left,
            rc.top,
            fRCRadiusX,
            (rc.bottom - rc.top),
            0,
            0,
            fRCRadiusX,
            size.height);

        // Left side.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.right - fRightSize,
            rc.top,
            fRightSize,
            (rc.bottom - rc.top),
            size.width - fRightSize,
            0,
            fRightSize,
            size.height);

        // Left side.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left + fRCRadiusX,
            rc.top,
            (rc.right - rc.left) - fRCRadiusX - fRightSize,
            (rc.bottom - rc.top),
            fRCRadiusX,
            0,
            size.width - fRCRadiusX - fRightSize,
            size.height);
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawRadioButton(
        SdkViewElement *pView,
        ID2D1RenderTarget *pRT,
        D2DBitmap *pBitmap,
        D2DBrush *pBrush,
        BOOL isChecked,
        BOOL isEnable,
        const D2D1_RECT_F& rc)
{
    if ( NULL == pBitmap )
    {
        return;
    }

    // Initialize the bitmap with the render target.
    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    // Draw the bitmap.
    if ( NULL != pD2DBitmap )
    {
        D2D1_SIZE_F size = pD2DBitmap->GetSize();
        FLOAT left = rc.left;
        FLOAT top = rc.top + ((rc.bottom - rc.top) - size.height) / 2;

        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            left,
            top,
            size.width,
            size.height,
            0,
            0,
            size.width,
            size.height);
    }

    // Draw the check inner round.
    if ( isChecked && NULL != pBrush )
    {
        // Create the brush for the checked inner round.
        if ( !pBrush->HasCreatedBrush() )
        {
            pBrush->CreateBrush(pRT);
        }
        // Set the color of the brush.
        pBrush->SetColor(isEnable ? ColorF(ColorF::White) : ColorF(ColorF::DarkGray));

        // Get the ID2D1Brush interface instance.
        ID2D1Brush *pD2DBrush = NULL;
        pBrush->GetD2DBrush(&pD2DBrush);

        if ( NULL != pD2DBrush )
        {
            D2D1_SIZE_F size = { 20, 20 };
            if ( NULL != pD2DBitmap )
            {
                size = pD2DBitmap->GetSize();
            }

            D2D1_ELLIPSE ellipse;
            ellipse.radiusX = 3;
            ellipse.radiusY = 3;
            ellipse.point.x = rc.left + size.width / 2;
            ellipse.point.y = rc.top + (rc.bottom - rc.top) / 2;

            pRT->FillEllipse(ellipse, pD2DBrush);
        }
        SAFE_RELEASE(pD2DBrush);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawRadioButton(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    const D2D1_RECT_F& rc)
{
    if ( NULL == pBitmap )
    {
        return;
    }

    // Initialize the bitmap with the render target.
    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        D2D1_SIZE_F size = pD2DBitmap->GetSize();
        FLOAT left = rc.left;
        FLOAT top = rc.top + ((rc.bottom - rc.top) - size.height) / 2;

        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            left,
            top,
            size.width,
            size.height,
            0,
            0,
            size.width,
            size.height);
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawCheckBox(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    const D2D1_RECT_F& rc)
{
    if ( NULL == pBitmap )
    {
        return;
    }

    // Initialize the bitmap with the render target.
    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        D2D1_SIZE_F size = pD2DBitmap->GetSize();
        FLOAT left = rc.left;
        FLOAT top = rc.top + ((rc.bottom - rc.top) - size.height) / 2;

        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            left,
            top,
            size.width,
            size.height,
            0,
            0,
            size.width,
            size.height);
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawProgressBar(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    const D2D1_RECT_F& rc)
{
    if ( NULL == pBitmap )
    {
        return;
    }

    // Initialize the bitmap with the render target.
    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        D2D1_SIZE_F size = pD2DBitmap->GetSize();
        FLOAT left = rc.left;
        FLOAT top  = rc.top + ((rc.bottom - rc.top) - size.height) / 2;
        FLOAT offset = 5;

        // Draw the left conner.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            left,
            top,
            offset,
            size.height,
            0,
            0,
            offset,
            size.height);

        left += (offset - 0.5f);

        // Draw the middle
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            left,
            top,
            (rc.right - rc.left) - offset * 2,
            size.height,
            offset,
            0,
            size.width - offset * 2,
            size.height);

        left += (((rc.right - rc.left) - offset * 2) - 0.5f);

        // Draw the right conner.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            left,
            top,
            offset,
            size.height,
            size.width - offset,
            0,
            offset,
            size.height);
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawProgressBarThumb(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    FLOAT fCurPercent,
    const D2D1_RECT_F& rc)
{
    if ( NULL == pBitmap )
    {
        return;
    }

    // Initialize the bitmap with the render target.
    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        D2D1_SIZE_F size = pD2DBitmap->GetSize();
        FLOAT fCurPos = (fCurPercent / 100.0f) * (pView->GetWidth() - size.width);
        FLOAT top  = rc.top + ((rc.bottom - rc.top) - size.height) / 2;
        FLOAT left = rc.left + fCurPos - size.width / 2;

        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            left,
            top,
            size.width,
            size.height,
            0,
            0,
            size.width,
            size.height);
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawRatingView(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    FLOAT fBitmapSize,
    const D2D1_RECT_F& rc)
{
    if ( NULL == pBitmap )
    {
        return;
    }

    // Initialize the bitmap with the render target.
    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        D2D1_SIZE_F size = pD2DBitmap->GetSize();
        FLOAT width  = (rc.right - rc.left);
        FLOAT height = (rc.bottom - rc.top);
        FLOAT top  = rc.top + (height - fBitmapSize) / 2;
        FLOAT left = rc.left;

        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            left,
            top,
            width,
            width,
            0,
            0,
            size.width,
            size.height);
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawTabViewTab(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    const D2D1_RECT_F& rc)
{
    if ( NULL == pBitmap )
    {
        return;
    }

    // Initialize the bitmap with the render target.
    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        D2D1_SIZE_F size = pD2DBitmap->GetSize();
        FLOAT width  = (rc.right - rc.left);
        FLOAT height = (rc.bottom - rc.top);
        // The round corner radius is 5, but it has transparent edge.
        FLOAT fRCRadiusX = 8;
        FLOAT fRCRadiusY = 8;

        // Left-top round corner.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left,
            rc.top,
            fRCRadiusX,
            fRCRadiusY,
            0,
            0,
            fRCRadiusX,
            fRCRadiusY);

        // Right-top round corner.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.right - fRCRadiusX,
            rc.top,
            fRCRadiusX,
            fRCRadiusY,
            size.width - fRCRadiusX,
            0,
            fRCRadiusX,
            fRCRadiusY);

        // Top side
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left + fRCRadiusX,
            rc.top,
            width - 2 * fRCRadiusX,
            fRCRadiusY,
            fRCRadiusY,
            0,
            size.width - 2 * fRCRadiusX,
            fRCRadiusY);

        // Left side.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left,
            rc.top + fRCRadiusY,
            fRCRadiusX,
            height - fRCRadiusY,
            0,
            fRCRadiusY,
            fRCRadiusX,
            size.height - fRCRadiusY);

        // Right side.
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.right - fRCRadiusX,
            rc.top + fRCRadiusY,
            fRCRadiusX,
            height - fRCRadiusY,
            size.width - fRCRadiusX,
            fRCRadiusY,
            fRCRadiusX,
            size.height - fRCRadiusY);

        // Center part
        OnDrawButtonBitmap(
            pView,
            pRT,
            pD2DBitmap,
            rc.left + fRCRadiusX,
            rc.top + fRCRadiusY,
            width - 2 * fRCRadiusX,
            height - fRCRadiusY,
            fRCRadiusX,
            fRCRadiusY,
            size.width - 2 * fRCRadiusX,
            size.height - fRCRadiusY);
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawTabViewSingleRoundCorner(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    D2DBitmap *pBitmap,
    BOOL fLeftCorner,
    const D2D1_RECT_F& rc)
{
    if ( NULL == pBitmap )
    {
        return;
    }

    // Initialize the bitmap with the render target.
    if ( !pBitmap->HasInitialized() )
    {
        pBitmap->Initialize(pRT);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    pBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        D2D1_SIZE_F size = pD2DBitmap->GetSize();
        FLOAT width  = (rc.right - rc.left);
        FLOAT height = (rc.bottom - rc.top);
        // The round corner radius is 5, but it has transparent edge.
        FLOAT fRCRadiusX = 6;
        FLOAT fRCRadiusY = 6;

        if ( fLeftCorner )
        {
            OnDrawButtonBitmap(
                pView,
                pRT,
                pD2DBitmap,
                rc.left + fRCRadiusX,
                rc.top,
                width - fRCRadiusX,
                height,
                fRCRadiusY,
                0,
                size.width - fRCRadiusX,
                size.height);

            // Left-top round corner.
            OnDrawButtonBitmap(
                pView,
                pRT,
                pD2DBitmap,
                rc.left,
                rc.top,
                fRCRadiusX,
                fRCRadiusY,
                0,
                0,
                fRCRadiusX,
                fRCRadiusY);

            // Left side.
            OnDrawButtonBitmap(
                pView,
                pRT,
                pD2DBitmap,
                rc.left,
                rc.top + fRCRadiusY,
                fRCRadiusX,
                height - fRCRadiusY,
                0,
                fRCRadiusY,
                fRCRadiusX,
                size.height - fRCRadiusY);
        }
        else
        {
            fRCRadiusX = 10;
            fRCRadiusY = 10;
            // Right-top round corner.
            OnDrawButtonBitmap(
                pView,
                pRT,
                pD2DBitmap,
                rc.right - fRCRadiusX,
                rc.top,
                fRCRadiusX,
                fRCRadiusY,
                size.width - fRCRadiusX,
                0,
                fRCRadiusX,
                fRCRadiusY);

            // Right side.
            OnDrawButtonBitmap(
                pView,
                pRT,
                pD2DBitmap,
                rc.right - fRCRadiusX,
                rc.top + fRCRadiusY,
                fRCRadiusX,
                height - fRCRadiusY,
                size.width - fRCRadiusX,
                fRCRadiusY,
                fRCRadiusX,
                size.height - fRCRadiusY);

            // Top side
            OnDrawButtonBitmap(
                pView,
                pRT,
                pD2DBitmap,
                rc.left,
                rc.top,
                width - fRCRadiusX + 1,
                height,
                fRCRadiusY,
                0,
                size.width - fRCRadiusX,
                size.height);
        }
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkD2DTheme::OnDrawButtonBitmap(
    SdkViewElement *pView,
    ID2D1RenderTarget *pRT,
    ID2D1Bitmap *pD2DBitmap,
    FLOAT destX,
    FLOAT destY,
    FLOAT destWidth,
    FLOAT destHeight,
    FLOAT srcX,
    FLOAT srcY,
    FLOAT srcWidth,
    FLOAT srcHeight)
{
    D2D1_RECT_F destRc = { destX, destY, destX + destWidth, destY + destHeight };
    D2D1_RECT_F srcRc  = { srcX,  srcY,  srcX  + srcWidth,  srcY  + srcHeight  };

    pRT->DrawBitmap(pD2DBitmap, destRc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRc);
}
