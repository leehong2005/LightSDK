/*!
* @file SdkCheckBox.h
* 
* @brief This file defines the SdkRadioButton class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#include "StdAfx.h"
#include "SdkCheckBox.h"
#include "SdkD2DTheme.h"
#include "SdkResManager.h"
#include "ICheckBoxEventHandler.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_VIEWS

#define RADIOBUTTON_MIN_WIDTH       18
#define RADIOBUTTON_MIN_HEIGHT      18
#define TEXT_STARTPOINT_OFFSET      25

/*!
* @brief The internal data of check box.
*/
struct NAMESPACE_VIEWS::SdkCheckBox::_CHECKBOX_INTERNALDATA
{
    D2DBitmap               *m_pNormalBitmap;           // The normal bitmap.
    D2DBitmap               *m_pHoverBitmap;            // The hover bitmap.
    D2DBitmap               *m_pDownBitmap;             // The pressed down bitmap.
    D2DBitmap               *m_pCheckBitmap;            // The check inner round bitmap.
    D2DBitmap               *m_pDisableBitmap;          // The disable bitmap.
    ICheckBoxEventHandler   *m_pCheckBoxHandler;        // The pointer to ICheckBoxEventHandler.
};

//////////////////////////////////////////////////////////////////////////

SdkCheckBox::SdkCheckBox()
{
    m_pCheckBoxData = new _CHECKBOX_INTERNALDATA();
    ZeroMemory(m_pCheckBoxData, sizeof(_CHECKBOX_INTERNALDATA));

    SetClassName(CLASSNAME_CHECKBOX);
    SetMinHeight(RADIOBUTTON_MIN_HEIGHT);
    SetMinWidth(RADIOBUTTON_MIN_WIDTH);
    SetTextAlign(TEXT_ALIGNMENT_LEFT);
    SetWrapContent(TRUE);
    AddFlag(VIEW_STATE_TOGGLE);
    SetTextColor(ColorF::Black);
    SetClickable(TRUE);
    SetTextSize(16);
    SetViewSize(100, 32);

    // Override this method to change the start point of text. Here the left of
    // start point is added TEXT_STARTPOINT_OFFSET (25), because the radio button
    // icon's size is 20 and 5 pixels gap.
    SetTextStartPoint(TEXT_STARTPOINT_OFFSET, 0);

    m_pCheckBoxData->m_pNormalBitmap  = new D2DBitmap();
    m_pCheckBoxData->m_pHoverBitmap   = new D2DBitmap();
    m_pCheckBoxData->m_pDownBitmap    = new D2DBitmap();
    m_pCheckBoxData->m_pDisableBitmap = new D2DBitmap();
    m_pCheckBoxData->m_pCheckBitmap   = new D2DBitmap();

    m_pCheckBoxData->m_pNormalBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_CHECKBOX_NORMAL));
    m_pCheckBoxData->m_pHoverBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_CHECKBOX_HOVER));
    m_pCheckBoxData->m_pDownBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_CHECKBOX_DOWN));
    m_pCheckBoxData->m_pCheckBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_CHECKBOX_CHECKROUND));
    m_pCheckBoxData->m_pDisableBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_CHECKBOX_DISABLE));
}

//////////////////////////////////////////////////////////////////////////

SdkCheckBox::~SdkCheckBox()
{
    SAFE_DELETE(m_pCheckBoxData->m_pNormalBitmap);
    SAFE_DELETE(m_pCheckBoxData->m_pHoverBitmap);
    SAFE_DELETE(m_pCheckBoxData->m_pDownBitmap);
    SAFE_DELETE(m_pCheckBoxData->m_pDisableBitmap);
    SAFE_DELETE(m_pCheckBoxData->m_pCheckBitmap);

    SAFE_DELETE(m_pCheckBoxData);
}

//////////////////////////////////////////////////////////////////////////

void SdkCheckBox::SetChecked(BOOL isChecked)
{
    SetSelected(isChecked);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCheckBox::IsChecked()
{
    return IsSelected();
}

//////////////////////////////////////////////////////////////////////////

void SdkCheckBox::SetCheckBoxEventHandler(ICheckBoxEventHandler *pEventHandler)
{
    m_pCheckBoxData->m_pCheckBoxHandler = pEventHandler;
}

//////////////////////////////////////////////////////////////////////////

D2DBitmap* SdkCheckBox::GetStyleBitmap(VIEW_STYLE style) const
{
    D2DBitmap *pBitmap = NULL;

    switch (style)
    {
    case VIEW_STYLE_NORMAL:
        pBitmap = m_pCheckBoxData->m_pNormalBitmap;
        break;

    case VIEW_STYLE_MOUSEHOVER:
        pBitmap = m_pCheckBoxData->m_pHoverBitmap;
        break;

    case VIEW_STYLE_MOUSEDOWN:
        pBitmap = m_pCheckBoxData->m_pDownBitmap;
        break;

    case VIEW_STYLE_DISABLE:
        pBitmap = m_pCheckBoxData->m_pDisableBitmap;
        break;

    case VIEW_STYLE_SELECTED:
        pBitmap = m_pCheckBoxData->m_pNormalBitmap;
        break;

    case VIEW_STYLE_SELECTEDOVER:
        pBitmap = m_pCheckBoxData->m_pHoverBitmap;
        break;

    case VIEW_STYLE_SELECTEDDOWN:
        pBitmap = m_pCheckBoxData->m_pDownBitmap;
        break;
    }

    return pBitmap;
}

//////////////////////////////////////////////////////////////////////////

void SdkCheckBox::OnSelectChenaged(SdkViewElement *pView, BOOL fSelected)
{
    if ( NULL != m_pCheckBoxData->m_pCheckBoxHandler )
    {
        // Call the callback to notify client application that the checking is changed.
        m_pCheckBoxData->m_pCheckBoxHandler->OnCheckedChanged(this, fSelected);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkCheckBox::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    // Draw text of radio button view.
    SdkTextView::OnDrawItem(pRenderTarget);

    // Draw the radio button.
    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);

    // Draw the radio button bitmap.
    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();

    pD2DTheme->OnDrawCheckBox(
        this,
        pRenderTarget,
        GetStyleBitmap(GetStyle()),
        rc);

    // Draw the checked bitmap.
    if ( IsChecked() )
    {
        pD2DTheme->OnDrawCheckBox(
            this,
            pRenderTarget,
            m_pCheckBoxData->m_pCheckBitmap,
            rc);
    }
}
