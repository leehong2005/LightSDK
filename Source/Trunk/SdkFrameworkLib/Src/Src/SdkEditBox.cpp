/*!
* @file SdkEditBox.cpp
* 
* @brief This file defines the SdkEditBox class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#include "StdAfx.h"
#include "SdkEditBox.h"
#include "SdkD2DTheme.h"
#include "SdkResManager.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_VIEWS

SdkEditBox::SdkEditBox() : m_pNormalBitmap(NULL),
                           m_pHoverBitmap(NULL),
                           m_pDisableBitmap(NULL)
{
    SetClassName(CLASSNAME_COMBOBOX);
    SetMinHeight(30);
    SetTextAlign(TEXT_ALIGNMENT_LEFT);
    SetViewSize(132, 30);
    SetClickable(TRUE);
    SetFocusable(TRUE);
    SetTextColor(ColorF::White);
    SetTextSize(16);
    SetTextStartPoint(3, -1);

    m_pNormalBitmap  = new D2DBitmap();
    m_pHoverBitmap   = new D2DBitmap();
    m_pDisableBitmap = new D2DBitmap();

    m_pNormalBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_EDITBOX_NORMAL));
    m_pHoverBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_EDITBOX_HOVER));
    m_pDisableBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_EDITBOX_DISABLE));
}

//////////////////////////////////////////////////////////////////////////

SdkEditBox::~SdkEditBox()
{
    SAFE_DELETE(m_pNormalBitmap);
    SAFE_DELETE(m_pHoverBitmap);
    SAFE_DELETE(m_pDisableBitmap);
}

//////////////////////////////////////////////////////////////////////////

D2DBitmap* SdkEditBox::GetStyleBitmap(VIEW_STYLE style)
{
    D2DBitmap *pBitmap = NULL;

    switch (style)
    {
    case VIEW_STYLE_NORMAL:
    case VIEW_STYLE_SELECTED:
        pBitmap = m_pNormalBitmap;
        break;

    case VIEW_STYLE_MOUSEHOVER:
    case VIEW_STYLE_MOUSEDOWN:
        pBitmap = m_pHoverBitmap;
        pBitmap = m_pHoverBitmap;
        break;

    case VIEW_STYLE_DISABLE:
        pBitmap = m_pDisableBitmap;

        break;
    }

    return pBitmap;
}

//////////////////////////////////////////////////////////////////////////

void SdkEditBox::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);

    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();

    pD2DTheme->OnDrawButton(
        this,
        pRenderTarget,
        GetStyleBitmap(GetStyle()),
        rc);

    // Draw the text
    SdkTextView::OnDrawText(pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkEditBox::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    SdkViewElement::OnMouseEvent(message, wParam, lParam);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkEditBox::OnKeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkEditBox::OnFocusChanged(SdkViewElement *pView, BOOL fGetFocus)
{
    if ( fGetFocus )
    {
        HWND hwnd = GetWindow()->GetHwnd();
        SIZE size = GetMeasureSize();
        D2D1_RECT_F rc = { 0 };
        GetAbsoluteRect(rc);

        ::CreateCaret(hwnd, 0, 1, size.cy);
        ::SetCaretPos((int)rc.left + 8, (int)rc.top + (int)(GetHeight() - size.cy) / 2);
        ::ShowCaret(hwnd);
    }
    else
    {
        ::DestroyCaret();
    }
}
