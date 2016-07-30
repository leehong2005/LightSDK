/*!
* @file ImageButton.cpp
* 
* @brief This file defines the ImageButton class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#include "ImageButton.h"
#include "Resource.h"

ImageButton::ImageButton(BOOL isShowDefBk) : m_normalTextColor(ColorF(ColorF::White)),
                                             m_focusTextColor (ColorF(ColorF::White)),
                                             m_normalTextSize(16.0f),
                                             m_focusTextSize(16.0f),
                                             m_pFocusedD2DBitmap(new D2DBitmap())
{
    m_layoutInfo.width  = 100;
    m_layoutInfo.height = 33;

    if (isShowDefBk)
    {
        SetBackgroundImage(IDB_BUTTON_BK, HINST_THISCOMPONENT);
        SetFocusImage(IDB_BUTTON_BK_F, HINST_THISCOMPONENT);
    }

    SetClickable(TRUE);
    SetFocusable(TRUE);
    SetCancelClickDisable(TRUE);
}

//////////////////////////////////////////////////////////////////////////

ImageButton::~ImageButton()
{
    SAFE_DELETE(m_pFocusedD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

BOOL ImageButton::SetFocusImage(IN LPCWSTR lpFile, UINT32 uDestWidth, UINT32 uDestHeight)
{
    return m_pFocusedD2DBitmap->LoadFromFile(lpFile, uDestWidth, uDestHeight);
}

//////////////////////////////////////////////////////////////////////////

BOOL ImageButton::SetFocusImage(HBITMAP hBitmap, UINT32 uDestWidth, UINT32 uDestHeight)
{
    return m_pFocusedD2DBitmap->LoadFromHBITMPA(hBitmap, uDestWidth, uDestHeight);
}

//////////////////////////////////////////////////////////////////////////

BOOL ImageButton::SetFocusImage(UINT uResId, HMODULE hModule, UINT32 uDestWidth, UINT32 uDestHeight)
{
    return m_pFocusedD2DBitmap->LoadFromResource(uResId, hModule, uDestWidth, uDestHeight);
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::SetFocusTextColor(IN const D2D1_COLOR_F& color)
{
    m_focusTextColor = color;
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::SetFocusFontSize(IN FLOAT size)
{
    m_focusTextSize = size;
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::SetNormalFontSize(IN FLOAT size)
{
    m_normalTextSize = size;
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::SetTextColor( IN const D2D1_COLOR_F& color )
{
    m_normalTextColor = color;
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    TextView::SetLayoutInfo(x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::ClearAssocData()
{
    ImageView::ClearAssocData();
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    BOOL isPressed  = (VIEW_STATE_PRESSED  == (GetState() & VIEW_STATE_PRESSED));
    BOOL isChecked  = (VIEW_STATE_SELECTED  == (GetState() & VIEW_STATE_SELECTED));
    if (isPressed || isChecked)
    {
        BOOL drawFocus = OnPaintFocusImage(pRenderTarget);
        // If draw focused images, if no focus image, we draw background image.
        if (drawFocus)
        {
            // Make sure base class not paint background when the view is focused.
            AddFlag(VIEW_STATE_NOTPAINTBK);
            ImageView::OnDrawItem(pRenderTarget);
            // Remove the no paint background flag.
            RemoveFlag(VIEW_STATE_NOTPAINTBK);
        }
        else
        {
            ImageView::OnDrawItem(pRenderTarget);
        }
        OnPaintFocusText(pRenderTarget);
    }
    else
    {
        OnPaintNormalImage(pRenderTarget);
        OnPaintNormalText(pRenderTarget);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL ImageButton::OnPaintFocusImage(ID2D1RenderTarget *pRenderTarget)
{
    if (!m_pFocusedD2DBitmap->HasInitialized())
    {
        D2DDevice *pD2DDevice = m_pWindow->GetD2DDevices();
        m_pFocusedD2DBitmap->Initialize(pD2DDevice);
    }

    ID2D1Bitmap *pID2D1Bitmap = NULL;
    m_pFocusedD2DBitmap->GetD2DBitmap(&pID2D1Bitmap);
    if (NULL != pID2D1Bitmap)
    {
        if (NULL != pRenderTarget)
        {
            D2D1_RECT_F rc = { 0 };
            GetAbsoluteRect(rc);
            pRenderTarget->DrawBitmap(pID2D1Bitmap, rc);
            return TRUE;
        }
    }

    SAFE_RELEASE(pID2D1Bitmap);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::OnPaintFocusText(ID2D1RenderTarget *pRenderTarget)
{
    TextView::SetTextColor(m_focusTextColor);
    TextView::SetTextSize(m_focusTextSize);

    AddFlag(VIEW_STATE_NOTPAINTBK);
    TextView::OnDrawItem(pRenderTarget);
    RemoveFlag(VIEW_STATE_NOTPAINTBK);
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::OnPaintNormalImage(ID2D1RenderTarget *pRenderTarget)
{
    ImageView::OnDrawItem(pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::OnPaintNormalText(ID2D1RenderTarget *pRenderTarget)
{
    TextView::SetTextColor(m_normalTextColor);
    TextView::SetTextSize(m_normalTextSize);

    AddFlag(VIEW_STATE_NOTPAINTBK);
    TextView::OnDrawItem(pRenderTarget);
    RemoveFlag(VIEW_STATE_NOTPAINTBK);
}

//////////////////////////////////////////////////////////////////////////

void ImageButton::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    TextView::OnLayout(fChanged, left, top, width, height);
}

//////////////////////////////////////////////////////////////////////////

BOOL ImageButton::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL handled = FALSE;

    switch(message)
    {
    case WM_LBUTTONDOWN:
        AddFlag(VIEW_STATE_PRESSED);
        Invalidate();
        handled = TRUE;
        break;

    case WM_MOUSEMOVE:
        handled = TRUE;
        break;

    case WM_LBUTTONUP:
        handled = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
        RemoveFlag(VIEW_STATE_CANCELEVENT);
        Invalidate();
        break;
    }

    ViewElement::OnMouseEvent(message, wParam, lParam);

    return handled;
}
