/*!
* @file SdkButton.cpp
* 
* @brief This file defines the SdkButton class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#include "stdafx.h"
#include "SdkButton.h"
#include "SdkD2DTheme.h"
#include "Resource.h"
#include "SdkResManager.h"

USING_NAMESPACE_VIEWS

#define BUTTON_UAC_ICON_SIZE        22      // The UAC icon size

/*!
* @brief The internal data of button.
*/
struct NAMESPACE_VIEWS::SdkButton::_BUTTON_INTERNALDATA
{
    BOOL                     m_fShowUACIcon;        // Show UAC shield icon.
    BOOL                     m_fDrawOverlay;        // Draw overlay icon.
    BOOL                     m_fReCalcUACIcon;      // Indicate need to re-calculate UAC icon.
    BOOL                     m_isDrawDefBk;         // Draw default button background.
    D2D1_RECT_F              m_rcOverlay;           // The overlay rectangle.
    D2D1_RECT_F              m_rcUacIcon;           // The UAC icon rectangle.
    D2DBitmap               *m_pNormalBitmap;       // The normal style bitmap.
    D2DBitmap               *m_pHoverBitmap;        // The hover style bitmap.
    D2DBitmap               *m_pDownBitmap;         // The mouse down style bitmap.
    D2DBitmap               *m_pDisableBitmap;      // The disable style bitmap.
    D2DBitmap               *m_pSelectBitmap;       // The disable style bitmap.
    D2DBitmap               *m_pSelectHoverBitmap;  // The disable style bitmap.
    D2DBitmap               *m_pOverlayBitmap;      // The hover style bitmap.
    D2DBitmap               *m_pUACShieldBitmap;    // The UAC Shield bitmap.
};


//////////////////////////////////////////////////////////////////////////

SdkButton::SdkButton(BOOL isShowDefBk)
{
    m_pButtonData = new _BUTTON_INTERNALDATA();
    ZeroMemory(m_pButtonData, sizeof(_BUTTON_INTERNALDATA));

    m_pButtonData->m_fReCalcUACIcon    = TRUE;
    m_pButtonData->m_isDrawDefBk       = isShowDefBk;

    // The overlay icon rectangle.
    m_pButtonData->m_rcOverlay.left    = 0;
    m_pButtonData->m_rcOverlay.top     = 0;
    m_pButtonData->m_rcOverlay.right   = BUTTON_UAC_ICON_SIZE;
    m_pButtonData->m_rcOverlay.bottom  = BUTTON_UAC_ICON_SIZE;
    m_pButtonData->m_rcUacIcon.left    = 0;
    m_pButtonData->m_rcUacIcon.top     = 0;
    m_pButtonData->m_rcUacIcon.right   = BUTTON_UAC_ICON_SIZE;
    m_pButtonData->m_rcUacIcon.bottom  = BUTTON_UAC_ICON_SIZE;

    m_pButtonData->m_pNormalBitmap     = new D2DBitmap();
    m_pButtonData->m_pHoverBitmap      = new D2DBitmap();
    m_pButtonData->m_pDownBitmap       = new D2DBitmap();
    m_pButtonData->m_pDisableBitmap    = new D2DBitmap();
    m_pButtonData->m_pSelectBitmap     = new D2DBitmap();
    m_pButtonData->m_pSelectHoverBitmap= new D2DBitmap();
    m_pButtonData->m_pOverlayBitmap    = new D2DBitmap();

    if ( isShowDefBk )
    {
        SetImage(SdkResManager::GetImage(IDB_BUTTON_BK_NORMAL), VIEW_STYLE_NORMAL);
        SetImage(SdkResManager::GetImage(IDB_BUTTON_BK_HOVER),  VIEW_STYLE_MOUSEHOVER);
        SetImage(SdkResManager::GetImage(IDB_BUTTON_BK_DOWN),   VIEW_STYLE_MOUSEDOWN);
        SetImage(SdkResManager::GetImage(IDB_BUTTON_BK_DISABLE), VIEW_STYLE_DISABLE);
        SetImage(SdkResManager::GetImage(IDB_BUTTON_BK_SELECTED), VIEW_STYLE_SELECTED);
        SetImage(SdkResManager::GetImage(IDB_BUTTON_BK_SELECTEDHOVER), VIEW_STYLE_SELECTEDOVER);
    }

    SetClassName(CLASSNAME_BUTTON);
    SetClickable(TRUE);
    SetFocusable(TRUE);
    SetTextColor(ColorF(ColorF::White));
    SetViewSize(100, 32);
}

//////////////////////////////////////////////////////////////////////////

SdkButton::~SdkButton()
{
    SAFE_DELETE(m_pButtonData->m_pNormalBitmap);
    SAFE_DELETE(m_pButtonData->m_pHoverBitmap);
    SAFE_DELETE(m_pButtonData->m_pDownBitmap);
    SAFE_DELETE(m_pButtonData->m_pDisableBitmap);
    SAFE_DELETE(m_pButtonData->m_pSelectBitmap);
    SAFE_DELETE(m_pButtonData->m_pSelectHoverBitmap);
    SAFE_DELETE(m_pButtonData->m_pOverlayBitmap);
    SAFE_DELETE(m_pButtonData->m_pUACShieldBitmap);

    SAFE_DELETE(m_pButtonData);
}

//////////////////////////////////////////////////////////////////////////

void SdkButton::SetToggle(BOOL isToggle)
{
    if ( isToggle )
    {
        AddFlag(VIEW_STATE_TOGGLE);
    }
    else
    {
        RemoveFlag(VIEW_STATE_TOGGLE);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkButton::ShowUACIcon(BOOL fShow)
{
    if ( fShow != m_pButtonData->m_fShowUACIcon )
    {
        m_pButtonData->m_fShowUACIcon = fShow;
    }

    // If show the UAC icon.
    if ( fShow )
    {
        if ( NULL == m_pButtonData->m_pUACShieldBitmap )
        {
            m_pButtonData->m_pUACShieldBitmap = new D2DBitmap();
            m_pButtonData->m_pUACShieldBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_ICON_UAC_SHIELD));
        }
    }
    else
    {
        // If not show UAC icon, the start point should be (0, 0).
        SetTextStartPoint(0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkButton::IsToggle() const
{
    return (VIEW_STATE_TOGGLE == (GetState() & VIEW_STATE_TOGGLE));
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkButton::SetImage(IN LPCWSTR lpFile, VIEW_STYLE style, UINT32 uWidth, UINT32 uHeight)
{
    D2DBitmap *pBitmap = GetStyleBitmap(style);

    if ( NULL != pBitmap )
    {
        return pBitmap->LoadFromFile(lpFile, uWidth, uHeight);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkButton::SetImage(HBITMAP hBitmap, VIEW_STYLE style, UINT32 uWidth, UINT32 uHeight)
{
    D2DBitmap *pBitmap = GetStyleBitmap(style);

    if ( NULL != pBitmap )
    {
        return pBitmap->LoadFromHBITMAP(hBitmap, uWidth, uHeight);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkButton::SetImage(UINT uResId, VIEW_STYLE style, HMODULE hModule, UINT32 uWidth, UINT32 uHeight)
{
    D2DBitmap *pBitmap = GetStyleBitmap(style);

    if ( NULL != pBitmap )
    {
        return pBitmap->LoadFromResource(uResId, hModule, uWidth, uHeight);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkButton::SetOverlayInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
    m_pButtonData->m_rcOverlay.left   = x;
    m_pButtonData->m_rcOverlay.top    = y;
    m_pButtonData->m_rcOverlay.right  = x + width;
    m_pButtonData->m_rcOverlay.bottom = y + height;
}

//////////////////////////////////////////////////////////////////////////

void SdkButton::CalcUacIconRect()
{
    if ( m_pButtonData->m_fShowUACIcon )
    {
        // If there is no change of size, need not to calculate icon's rectangle.
        if ( m_pButtonData->m_fReCalcUACIcon )
        {
            // The UAC Icon size.
            const int nUacIconSize = BUTTON_UAC_ICON_SIZE;
            TEXT_METRICS textMetrics = SdkTextView::GetTextMetrics();
            FLOAT fLeft = 0;
            FLOAT fTop  = 0;

            // Calculate the UAC icon's rectangle.
            if ( textMetrics.left <= nUacIconSize )
            {
                FLOAT fOffset = (FLOAT)nUacIconSize - textMetrics.left + 1;
                SetTextStartPoint(fOffset, 0);
            }
            else
            {
                fLeft = textMetrics.left - nUacIconSize;
            }

            fTop = (GetHeight() - nUacIconSize) / 2;

            m_pButtonData->m_rcUacIcon.left   = fLeft;
            m_pButtonData->m_rcUacIcon.top    = fTop;
            m_pButtonData->m_rcUacIcon.right  = fLeft + nUacIconSize;
            m_pButtonData->m_rcUacIcon.bottom = fTop  + nUacIconSize;
        }

        m_pButtonData->m_fReCalcUACIcon = FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkButton::OnTextLayoutChanged()
{
    SdkTextView::OnTextLayoutChanged();

    m_pButtonData->m_fReCalcUACIcon = TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SdkButton::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);

    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
    // Default background's drawing is different the user custom background.
    if ( m_pButtonData->m_isDrawDefBk )
    {
        // Draw the default button background.
        pD2DTheme->OnDrawButton(
            this,
            pRenderTarget,
            GetStyleBitmap(GetStyle()),
            rc);
    }
    else
    {
        pD2DTheme->OnDrawBitmap(
            this,
            pRenderTarget,
            GetStyleBitmap(GetStyle()),
            rc);
    }

    // Draw the overlay icon.
    if ( m_pButtonData->m_fDrawOverlay )
    {
        pD2DTheme->OnDrawOverlayBitmap(
            this,
            pRenderTarget,
            m_pButtonData->m_pOverlayBitmap,
            m_pButtonData->m_rcOverlay);
    }

    // Draw the UAC shield icon.
    if ( m_pButtonData->m_fShowUACIcon )
    {
        // Calculate the UAC icon's rectangle.
        CalcUacIconRect();

        pD2DTheme->OnDrawOverlayBitmap(
            this,
            pRenderTarget,
            m_pButtonData->m_pUACShieldBitmap,
            m_pButtonData->m_rcUacIcon);
    }

    // Draw the text
    SdkTextView::OnDrawText(pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

D2DBitmap* SdkButton::GetStyleBitmap(VIEW_STYLE style)
{
    D2DBitmap *pBitmap = NULL;

    switch (style)
    {
    case VIEW_STYLE_NORMAL:
        pBitmap = m_pButtonData->m_pNormalBitmap;
        break;

    case VIEW_STYLE_MOUSEHOVER:
        pBitmap = m_pButtonData->m_pHoverBitmap;
        break;

    case VIEW_STYLE_MOUSEDOWN:
        pBitmap = m_pButtonData->m_pDownBitmap;
        break;

    case VIEW_STYLE_DISABLE:
        pBitmap = m_pButtonData->m_pDisableBitmap;
        break;

    case VIEW_STYLE_SELECTED:
        pBitmap = m_pButtonData->m_pSelectBitmap;
        break;

    case VIEW_STYLE_SELECTEDOVER:
        pBitmap = m_pButtonData->m_pSelectHoverBitmap;
        break;

    case VIEW_STYLE_SELECTEDDOWN:
        pBitmap = m_pButtonData->m_pDownBitmap;
        break;

    case VIEW_STYLE_OVERLAY:
        pBitmap = m_pButtonData->m_pOverlayBitmap;
        m_pButtonData->m_fDrawOverlay = TRUE;
        break;
    }

    return pBitmap;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkButton::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    SdkViewElement::OnMouseEvent(message, wParam, lParam);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkButton::OnKeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        {
            // If carriage return is pressed down, perform the click event.
            if ( VK_RETURN == wParam )
            {
                PerformClick();
            }
        }
        break;

    default:
        return FALSE;
    }

    return TRUE;
}
