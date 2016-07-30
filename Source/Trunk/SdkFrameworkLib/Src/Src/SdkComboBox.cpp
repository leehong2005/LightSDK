/*!
* @file SdkComboBox.cpp
* 
* @brief This file defines the SdkComboBox class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/27
*/

#include "stdafx.h"
#include "SdkComboBox.h"
#include "SdkTextView.h"
#include "SdkButton.h"
#include "SdkListBox.h"
#include "SdkD2DTheme.h"
#include "SdkResManager.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_VIEWS

#define DEFAULT_WIDTH               150     // Default width of this view.
#define DEFAULT_HEIGHT              32      // Default height of this view.
#define DROP_BUTTON_ID              101     // The Drop_down button id.
#define VIEW_GAP                    3       // The gap of comboBox and MenuList.
#define PADDING_LEFT                0       // The distance of list box left edge to comboBox left edge.
#define COMBOBOX_DROPWIDTH          30      // The width of combo box header drop.

/*!
* @brief The combo box internal data.
*/
struct NAMESPACE_VIEWS::SdkComboBox::_COMBOBOX_INTERNALDATA
{
    FLOAT                    m_fDropDownWidth;          // The drop down list width.
    BOOL                     m_isDeleteList;            // Indicates whether delete list.
    SdkTextView             *m_pTextView;               // The view of show text.
    SdkListBox              *m_pListBox;                // THe MenuList add to comboBox.
    D2DBitmap               *m_pHoverBitmap;            // The hover style bitmap.
    D2DBitmap               *m_pNormalBitmap;           // The normal style bitmap.
    D2DBitmap               *m_pDownBitmap;             // The mouse down style bitmap.
    D2DBitmap               *m_pDisableBitmap;          // The disable style bitmap.
    D2DBitmap               *m_pDropBitmap;             // The disable style bitmap.
    D2DBitmap               *m_pDropHoverBitmap;        // The disable style bitmap.
    IComboBoxEventHandler   *m_pComboBoxEventHandler;   // The eventHandler.
};

//////////////////////////////////////////////////////////////////////////

SdkComboBox::SdkComboBox()
{
    m_pComboBoxData = new _COMBOBOX_INTERNALDATA();
    ZeroMemory(m_pComboBoxData, sizeof(_COMBOBOX_INTERNALDATA));

    m_pComboBoxData->m_isDeleteList   = TRUE;
    m_pComboBoxData->m_fDropDownWidth = DEFAULT_WIDTH + 20;

    SetClassName(CLASSNAME_COMBOBOX);
    SetRoundCornerRadius(5, 5);
    // Create the combo box and add initial views.
    InitializeViews();

    AddFlag(VIEW_STATE_TOGGLE);
    SetMinHeight(32);
    SetMinWidth(100);
    SetMaxHeight(DEFAULT_HEIGHT);
    SetMaxWidth(300);
    SetClickable(TRUE);

    // Set the view's default size.
    SetViewSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

//////////////////////////////////////////////////////////////////////////

SdkComboBox::~SdkComboBox()
{
    if ( m_pComboBoxData->m_isDeleteList )
    {
        SAFE_DELETE(m_pComboBoxData->m_pListBox);
    }

    SAFE_DELETE(m_pComboBoxData->m_pNormalBitmap);
    SAFE_DELETE(m_pComboBoxData->m_pHoverBitmap);
    SAFE_DELETE(m_pComboBoxData->m_pDownBitmap);
    SAFE_DELETE(m_pComboBoxData->m_pDisableBitmap);
    SAFE_DELETE(m_pComboBoxData->m_pDropBitmap);

    SAFE_DELETE(m_pComboBoxData);
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::AddItem(IN LPCWSTR lpItemText)
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        m_pComboBoxData->m_pListBox->AddItem(lpItemText);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::AddItem(UINT32 uResId, HMODULE hInst)
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        m_pComboBoxData->m_pListBox->AddItem(uResId, hInst);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::AddItem(IN LPCWSTR lpItemText, UINT32 uIndex)
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        m_pComboBoxData->m_pListBox->AddItem(lpItemText, uIndex);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::DeleteItem(UINT32 uIndex)
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        m_pComboBoxData->m_pListBox->DeleteItem(uIndex);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::RemoveAllItems()
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        m_pComboBoxData->m_pListBox->RemoveAllItems();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::SetDropDownWidth(FLOAT fDropDownWidth)
{
    m_pComboBoxData->m_fDropDownWidth = fDropDownWidth;
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::SetItemText(IN INT32 menuItemIndex, IN LPWSTR lpText)
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        m_pComboBoxData->m_pListBox->SetItemText(menuItemIndex, lpText);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::SetItemText(IN INT32 menuItemIndex, IN UINT32 uResId, HMODULE hInst)
{
    TCHAR szBuffer[1024] = { 0 };
    hInst = (NULL == hInst) ? GetModuleHandle(NULL) : hInst;
    INT32 nLen = LoadString(hInst, uResId, szBuffer, 1024);

    // If succeed to load string.
    if ( nLen > 0 )
    {
        SetItemText(menuItemIndex, szBuffer);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::SetCurSel(INT32 nIndex)
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        m_pComboBoxData->m_pListBox->SetCurSel(nIndex);

        // Set combobox displaying text.
        UINT32 nLength = m_pComboBoxData->m_pListBox->GetSelTextLength();
        if ( nLength > 0 )
        {
            nLength += 1;
            LPWSTR lpText = new TCHAR[nLength];
            ZeroMemory(lpText, nLength);
            m_pComboBoxData->m_pListBox->GetSelText(lpText, nLength);
            SetComboBoxText(lpText);
            SAFE_DELETE_ARRAY(lpText);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

INT32 SdkComboBox::GetCurSel()
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        return m_pComboBoxData->m_pListBox->GetSelIndex();
    }

    return -1;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkComboBox::GetSelText(OUT LPWSTR lpText, IN UINT32 uSize)
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        return m_pComboBoxData->m_pListBox->GetSelText(lpText, uSize);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

UINT32 SdkComboBox::GetSelItemLength()
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        return m_pComboBoxData->m_pListBox->GetSelTextLength();
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::SetParent(SdkViewLayout *pParentView)
{
    if ( NULL != pParentView )
    {
        if ( NULL != m_pComboBoxData->m_pListBox )
        {
            // Hide the list box.
            m_pComboBoxData->m_pListBox->SetVisible(FALSE);

            // Add the list box to combo box's parent.
            pParentView->AddView(m_pComboBoxData->m_pListBox);

            // If the list box is added to parent view, the memory is managed by the 
            // parent view, so set the m_isDeleteList to FALSE to indicates do not delete
            // the memory in destructor function.
            m_pComboBoxData->m_isDeleteList = FALSE;
        }
    }

    SdkViewElement::SetParent(pParentView);
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::ShowDropDown(BOOL isShow)
{
    if ( NULL != m_pComboBoxData->m_pListBox )
    {
        FLOAT listBoxPosX  = GetLeft()- PADDING_LEFT;
        FLOAT listBoxPoxY  = GetTop() + GetHeight() + VIEW_GAP;
        FLOAT listBoxWidth = MAX(m_pComboBoxData->m_fDropDownWidth, GetWidth());

        // First set the layout, because the combo box's layout may change, so the drop dawn
        // position should be updated when it is showing.
        m_pComboBoxData->m_pListBox->SetLayoutInfo(
            listBoxPosX,
            listBoxPoxY,
            listBoxWidth,
            m_pComboBoxData->m_pListBox->GetHeight());

        // Show the list box.
        m_pComboBoxData->m_pListBox->ShowView(isShow);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::SetEnable(BOOL isEnable)
{
    // If disable the combo box, we will close the drop down list.
    if ( !isEnable )
    {
        ShowDropDown(FALSE);
    }

    SdkViewLayout::SetEnable(isEnable);

    // Set enable or disable the list box.
    if (NULL != m_pComboBoxData->m_pListBox)
    {
        m_pComboBoxData->m_pListBox->SetEnable(isEnable);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::SetComboBoxEventHandler(IN IComboBoxEventHandler *pComboBoxEventHandler)
{
    // Save the combo box event handler to give notification to client.
    m_pComboBoxData->m_pComboBoxEventHandler = pComboBoxEventHandler;
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::SetComboBoxText(IN LPCWSTR lpText)
{
    if ( NULL != lpText )
    {
        m_pComboBoxData->m_pTextView->SetText(lpText);
        ShowDropDown(FALSE);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::InitializeViews()
{
    m_pComboBoxData->m_pTextView = new SdkTextView();
    m_pComboBoxData->m_pTextView->SetTextColor(ColorF::White);
    m_pComboBoxData->m_pTextView->SetFontWeight(DWRITE_FONT_WEIGHT_BLACK);

    m_pComboBoxData->m_pNormalBitmap    = new D2DBitmap();
    m_pComboBoxData->m_pHoverBitmap     = new D2DBitmap();
    m_pComboBoxData->m_pDownBitmap      = new D2DBitmap();
    m_pComboBoxData->m_pDropBitmap      = new D2DBitmap();
    m_pComboBoxData->m_pDisableBitmap   = new D2DBitmap();
    m_pComboBoxData->m_pDropHoverBitmap = new D2DBitmap();

    m_pComboBoxData->m_pNormalBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_COMBOBOX_NORMAL));
    m_pComboBoxData->m_pHoverBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_COMBOBOX_HOVER));
    m_pComboBoxData->m_pDownBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_COMBOBOX_DOWN));
    m_pComboBoxData->m_pDropBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_COMBOBOX_OPEN));
    m_pComboBoxData->m_pDropHoverBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_COMBOBOX_OPEN_HOVER));
    m_pComboBoxData->m_pDisableBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_COMBOBOX_DISABLE));

    m_pComboBoxData->m_pListBox = new SdkListBox();
    // Set the list box's property is pop-up.
    m_pComboBoxData->m_pListBox->SetPopup(TRUE);
    m_pComboBoxData->m_pListBox->SetListBoxEventHandler(this);

    SetId(DROP_BUTTON_ID);
    SetOnClickHandler(this);

    AddView(m_pComboBoxData->m_pTextView);
}

//////////////////////////////////////////////////////////////////////////

D2DBitmap* SdkComboBox::GetStyleBitmap(VIEW_STYLE style) const
{
    D2DBitmap *pBitmap = NULL;

    switch (style)
    {
    case VIEW_STYLE_NORMAL:
        pBitmap = m_pComboBoxData->m_pNormalBitmap;
        break;

    case VIEW_STYLE_MOUSEHOVER:
        pBitmap = m_pComboBoxData->m_pHoverBitmap;
        break;

    case VIEW_STYLE_MOUSEDOWN:
        pBitmap = m_pComboBoxData->m_pDownBitmap;
        break;

    case VIEW_STYLE_DISABLE:
        pBitmap = m_pComboBoxData->m_pDisableBitmap;
        break;

    case VIEW_STYLE_SELECTED:
        pBitmap = m_pComboBoxData->m_pDropBitmap;
        break;

    case VIEW_STYLE_SELECTEDOVER:
        pBitmap = m_pComboBoxData->m_pDropHoverBitmap;
        break;

    case VIEW_STYLE_SELECTEDDOWN:
        pBitmap = m_pComboBoxData->m_pDownBitmap;
        break;
    }

    return pBitmap;
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);

    // Draw the combo box's header.
    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();

    pD2DTheme->OnDrawComboBox(
        this,
        pRenderTarget,
        GetStyleBitmap(GetStyle()),
        rc);

    SdkViewLayout::OnDrawItem(pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    // Reset the text view's layout information.
    m_pComboBoxData->m_pTextView->SetLayoutInfo(
        0,
        0,
        width - COMBOBOX_DROPWIDTH,
        height);

    // Call base's method.
    SdkViewLayout::OnLayout(fChanged, left, top, width, height);
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::OnClick(SdkViewElement* view)
{
    switch(view->GetId())
    {
    case DROP_BUTTON_ID:
        {
            if ( NULL != m_pComboBoxData->m_pListBox )
            {
                // Show or hide the drop down (list box).
                ShowDropDown(!m_pComboBoxData->m_pListBox->IsVisible());
            }
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkComboBox::OnLongClick(SdkViewElement *pView)
{
    UNREFERENCED_PARAMETER(pView);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::OnSelectedChanged(SdkListBox *pView)
{
    // This method is called when the selected item is changed in list box.
    if ( NULL != pView )
    {
        UINT32 nLength = pView->GetSelTextLength();
        if ( nLength > 0 )
        {
            nLength += 1;
            LPTSTR lpText = new TCHAR[nLength];
            ZeroMemory(lpText, nLength);
            pView->GetSelText(lpText, nLength);
            SetComboBoxText(lpText);
            SAFE_DELETE_ARRAY(lpText);
        }
        else
        {
            SetComboBoxText(L"");
        }

        // Hide the list box.
        pView->ShowView(FALSE);

        // Call the combo box's event handler.
        if ( NULL != m_pComboBoxData->m_pComboBoxEventHandler )
        {
            m_pComboBoxData->m_pComboBoxEventHandler->OnSelectedChanged(this);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkComboBox::OnVisibleChanged(SdkListBox *pView)
{
    BOOL isVisible = pView->IsVisible();
    SetStyle(isVisible ? VIEW_STYLE_SELECTED : VIEW_STYLE_NORMAL);
    SetSelected(isVisible);
}
