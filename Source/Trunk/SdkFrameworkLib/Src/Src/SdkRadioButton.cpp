/*!
* @file SdkRadioButton.h
* 
* @brief This file defines the SdkRadioButton class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#include "StdAfx.h"
#include "SdkRadioButton.h"
#include "SdkD2DTheme.h"
#include "SdkResManager.h"
#include "IRadioButtonEventHandler.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_VIEWS

#define RADIOBUTTON_MIN_WIDTH       20          // The min width of radio button.
#define RADIOBUTTON_MIN_HEIGHT      20          // The min height of radio button.
#define TEXT_STARTPOINT_OFFSET      25          // The start point offset of text.

/*!
* @brief The internal data of radio button.
*/
struct NAMESPACE_VIEWS::SdkRadioButton::_RADIOBUTTON_INTERNALDATA
{
    D2DBitmap                *m_pNormalBitmap;  // The normal bitmap.
    D2DBitmap                *m_pHoverBitmap;   // The hover bitmap.
    D2DBitmap                *m_pDownBitmap;    // The pressed down bitmap.
    D2DBitmap                *m_pCheckBitmap;   // The check inner round bitmap.
    D2DBitmap                *m_pDisableBitmap; // The disable bitmap.
    IRadioButtonEventHandler *m_pEventHandler;  // The event handler.
};


//////////////////////////////////////////////////////////////////////////

SdkRadioButton::SdkRadioButton()
{
    m_pRadioData = new _RADIOBUTTON_INTERNALDATA();
    ZeroMemory(m_pRadioData, sizeof(_RADIOBUTTON_INTERNALDATA));

    SetClassName(CLASSNAME_RADIOBUTTON);
    SetMinHeight(RADIOBUTTON_MIN_HEIGHT);
    SetMinWidth(RADIOBUTTON_MIN_WIDTH);
    SetTextAlign(TEXT_ALIGNMENT_LEFT);
    AddFlag(VIEW_STATE_TOGGLE);
    SetTextColor(ColorF::Black);
    SetTextSize(16);
    SetClickable(TRUE);
    SetWrapContent(TRUE);

    // Override this method to change the start point of text. Here the left of
    // start point is added TEXT_STARTPOINT_OFFSET (25), because the radio button
    // icon's size is 20 and 5 pixels gap.
    SetTextStartPoint(TEXT_STARTPOINT_OFFSET, 0);

    m_pRadioData->m_pNormalBitmap  = new D2DBitmap();
    m_pRadioData->m_pHoverBitmap   = new D2DBitmap();
    m_pRadioData->m_pDownBitmap    = new D2DBitmap();
    m_pRadioData->m_pDisableBitmap = new D2DBitmap();
    m_pRadioData->m_pCheckBitmap   = new D2DBitmap();

    m_pRadioData->m_pNormalBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RADIOBUTOTN_NORMAL));
    m_pRadioData->m_pHoverBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RADIOBUTOTN_HOVER));
    m_pRadioData->m_pDownBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RADIOBUTOTN_DOWN));
    m_pRadioData->m_pCheckBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RADIOBUTOTN_CHECKROUND));
    m_pRadioData->m_pDisableBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_RADIOBUTOTN_DISABLE));
}

//////////////////////////////////////////////////////////////////////////

SdkRadioButton::~SdkRadioButton()
{
    SAFE_DELETE(m_pRadioData->m_pNormalBitmap);
    SAFE_DELETE(m_pRadioData->m_pHoverBitmap);
    SAFE_DELETE(m_pRadioData->m_pDownBitmap);
    SAFE_DELETE(m_pRadioData->m_pDisableBitmap);
    SAFE_DELETE(m_pRadioData->m_pCheckBitmap);

    SAFE_DELETE(m_pRadioData);
}

//////////////////////////////////////////////////////////////////////////

void SdkRadioButton::SetChecked(BOOL isChecked)
{
    SetSelected(isChecked);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkRadioButton::IsChecked()
{
    return IsSelected();
}

//////////////////////////////////////////////////////////////////////////

void SdkRadioButton::SetEnable(BOOL isEnable)
{
    SdkTextView::SetEnable(isEnable);

    // Set the text color for disable style.
    SdkTextView::SetTextColor(isEnable ? ColorF::Black : ColorF::DarkGray);
}

//////////////////////////////////////////////////////////////////////////

void SdkRadioButton::SetRadioButtonEventHandler(IRadioButtonEventHandler *pEventHandler)
{
    m_pRadioData->m_pEventHandler = pEventHandler;
}

//////////////////////////////////////////////////////////////////////////

D2DBitmap* SdkRadioButton::GetStyleBitmap(VIEW_STYLE style) const
{
    D2DBitmap *pBitmap = NULL;

    switch (style)
    {
    case VIEW_STYLE_NORMAL:
        pBitmap = m_pRadioData->m_pNormalBitmap;
        break;

    case VIEW_STYLE_MOUSEHOVER:
        pBitmap = m_pRadioData->m_pHoverBitmap;
        break;

    case VIEW_STYLE_MOUSEDOWN:
        pBitmap = m_pRadioData->m_pDownBitmap;
        break;

    case VIEW_STYLE_DISABLE:
        pBitmap = m_pRadioData->m_pDisableBitmap;
        break;

    case VIEW_STYLE_SELECTED:
        pBitmap = m_pRadioData->m_pNormalBitmap;
        break;

    case VIEW_STYLE_SELECTEDOVER:
        pBitmap = m_pRadioData->m_pHoverBitmap;
        break;

    case VIEW_STYLE_SELECTEDDOWN:
        pBitmap = m_pRadioData->m_pDownBitmap;
        break;
    }

    return pBitmap;
}

//////////////////////////////////////////////////////////////////////////

void SdkRadioButton::OnSelectChenaged(SdkViewElement *pView, BOOL fSelected)
{
    if ( NULL != m_pRadioData->m_pEventHandler )
    {
        m_pRadioData->m_pEventHandler->OnSelectedChanged(this, fSelected);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkRadioButton::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    // Draw text of radio button view.
    SdkTextView::OnDrawItem(pRenderTarget);

    // Draw the radio button.
    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);

    // Draw the radio button bitmap.
    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
    pD2DTheme->OnDrawRadioButton(
        this,
        pRenderTarget,
        GetStyleBitmap(GetStyle()),
        rc);

    // Draw the checked bitmap.
    if ( IsChecked() )
    {
        pD2DTheme->OnDrawRadioButton(
            this,
            pRenderTarget,
            m_pRadioData->m_pCheckBitmap,
            rc);
    }
}
