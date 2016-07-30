/*!
* @file ProcessBar.cpp 
* 
* @brief This file defines the class ProcessBar, it's a control for display process.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/24
*/

#include "stdafx.h"
#include "Resource.h"
#include "SdkResManager.h"
#include "SdkD2DTheme.h"
#include "SdkProgressBar.h"
#include "IProgressBarEventHandler.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_VIEWS

/*!
* @brief The internal data of seek bar.
*/
struct NAMESPACE_VIEWS::SdkProgressBar::_PROGRESSBAR_INTERNALDATA
{
    BOOL                      m_isInThumbRect;              // Indicates the point is in thumb rectangle.
    FLOAT                     m_fMinRange;                  // The minimum range.
    FLOAT                     m_fMaxRange;                  // The maximum range.
    FLOAT                     m_fCurPercent;                // The current percent value.
    D2DBitmap                *m_pThumbBitmap;               // The thumb bitmap.
    D2DBitmap                *m_pTrackBitmap;               // The track bitmap.
    D2DBitmap                *m_pProgressBitmap;            // The progress bitmap.
    IProgressBarEventHandler *m_pProgressBarEventHandler;   // The seek bar callback interface.
};

//////////////////////////////////////////////////////////////////////////

SdkProgressBar::SdkProgressBar()
{
    m_pProgressBarData = new _PROGRESSBAR_INTERNALDATA();
    ZeroMemory(m_pProgressBarData, sizeof(_PROGRESSBAR_INTERNALDATA));

    SetClassName(CLASSNAME_PROGRESS);
    SetMinHeight(15);
    SetMaxHeight(30);
    SetMinWidth(40);
    SetViewSize(200, 15);

    m_pProgressBarData->m_fMaxRange         = 100;
    m_pProgressBarData->m_pThumbBitmap      = new D2DBitmap();
    m_pProgressBarData->m_pTrackBitmap      = new D2DBitmap();
    m_pProgressBarData->m_pProgressBitmap   = new D2DBitmap();

    m_pProgressBarData->m_pThumbBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_SEEKBAR_THUMB));
    m_pProgressBarData->m_pTrackBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_SEEKBAR_TRACK));
    m_pProgressBarData->m_pProgressBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_SEEKBAR_PROGRESS));
}

//////////////////////////////////////////////////////////////////////////

SdkProgressBar::~SdkProgressBar()
{
    SAFE_DELETE(m_pProgressBarData->m_pThumbBitmap);
    SAFE_DELETE(m_pProgressBarData->m_pTrackBitmap);
    SAFE_DELETE(m_pProgressBarData->m_pProgressBitmap);

    SAFE_DELETE(m_pProgressBarData);
}

//////////////////////////////////////////////////////////////////////////

void SdkProgressBar::SetPercent(FLOAT fPercent)
{
    fPercent = MAX(fPercent, 0);
    fPercent = MIN(fPercent, 100);

    if ( m_pProgressBarData->m_fCurPercent != fPercent )
    {
        m_pProgressBarData->m_fCurPercent = fPercent;

        OnPercentChanged(fPercent);
        Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkProgressBar::GetPercent() const
{
    return m_pProgressBarData->m_fCurPercent;
}

//////////////////////////////////////////////////////////////////////////

void SdkProgressBar::SetProgressBarEventHandler(IProgressBarEventHandler *pProgressBarHandler)
{
    m_pProgressBarData->m_pProgressBarEventHandler = pProgressBarHandler;
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F SdkProgressBar::OnCalcTrackRect()
{
    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);

    return rc;
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkProgressBar::OnGetProgressWidth()
{
    FLOAT fWidth = (GetPercent() / 100.0f) * (GetWidth());

    return fWidth;
}

//////////////////////////////////////////////////////////////////////////

void SdkProgressBar::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    // First call base class function to draw basic elements.
    SdkViewElement::OnDrawItem(pRenderTarget);

    // The absolute rectangle of the progress bar track.
    D2D1_RECT_F rc = OnCalcTrackRect();

    // Use this object to draw stuffs.
    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();

    // Draw the track bitmap.
    pD2DTheme->OnDrawProgressBar(
        this,
        pRenderTarget,
        m_pProgressBarData->m_pTrackBitmap,
        rc);

    // Draw the progress bitmap.
    if ( m_pProgressBarData->m_fCurPercent > 0.0f )
    {
        // Get the progress width.
        FLOAT fWidth = OnGetProgressWidth();
        // The progress rectangle.
        rc.right = rc.left + fWidth;

        pD2DTheme->OnDrawProgressBar(
            this,
            pRenderTarget,
            m_pProgressBarData->m_pProgressBitmap,
            rc);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkProgressBar::OnPercentChanged(FLOAT fCurPercent)
{
    // Call the callback to notify client that the percent is changed.
    if ( NULL != m_pProgressBarData->m_pProgressBarEventHandler )
    {
        m_pProgressBarData->m_pProgressBarEventHandler->OnPercentChanged(
            this,
            fCurPercent);
    }
}
