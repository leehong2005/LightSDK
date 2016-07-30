/*!
* @file MenuList.cpp
* 
* @brief This file defines the MenuList class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/14
*/

#include "stdafx.h"
#include "Resource.h"
#include "SdkListBox.h"
#include "SdkD2DTheme.h"
#include "SdkButton.h"
#include "D2DRectUtility.h"
#include "D2DBitmapBrush.h"
#include "SdkResManager.h"
#include "IListBoxEventHandler.h"
#include "IViewOnMouseHandler.h"

#define MENU_ITEM_HEIGHT    26

USING_NAMESPACE_VIEWS

/*!
* @brief The internal data of list box.
*/
struct NAMESPACE_VIEWS::SdkListBox::_LISTBOX_INTERNALDATA
{
    FLOAT                   m_fItemMargin;          // Item margin
    int                     m_nPressIndex;          // Temp selected index.
    int                     m_nHoverIndex;          // The hover item.
    int                     m_nSelIndex;            // The focused child's Id.
    D2DBitmap              *m_pItemPressBitmap;     // The press item bitmap.
    D2DBitmap              *m_pItemHoverBitmap;     // The hover item bitmap.
    D2DBitmap              *m_pItemSelectBitmap;    // The selected item bitmap.
    D2DBitmap              *m_pItemNormapBitmap;    // The normal item bitmap.
    IListBoxEventHandler   *m_pListEventHandler;    // The eventHandler.
};

//////////////////////////////////////////////////////////////////////////

SdkListBox::SdkListBox()
{
    m_pListBoxData = new _LISTBOX_INTERNALDATA();
    ZeroMemory(m_pListBoxData, sizeof(_LISTBOX_INTERNALDATA));

    m_pListBoxData->m_nSelIndex     = -1;
    m_pListBoxData->m_nPressIndex   = -1;
    m_pListBoxData->m_nHoverIndex   = -1;
    m_pListBoxData->m_fItemMargin   = 1.0f;

    m_pListBoxData->m_pItemNormapBitmap = new D2DBitmap();
    m_pListBoxData->m_pItemSelectBitmap = new D2DBitmap();
    m_pListBoxData->m_pItemHoverBitmap  = new D2DBitmap();
    m_pListBoxData->m_pItemPressBitmap  = new D2DBitmap();
    m_pListBoxData->m_pItemNormapBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_LISTBOX_ITEM_BK));
    m_pListBoxData->m_pItemSelectBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_LISTBOX_ITEM_SEL));
    m_pListBoxData->m_pItemHoverBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_LISTBOX_ITEM_HOVER));
    m_pListBoxData->m_pItemPressBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_LISTBOX_ITEM_DOWN));

    SetClassName(CLASSNAME_LISTBOX);
    SetClickable(FALSE);
    SetRoundCornerEnable(TRUE);
    SetRoundCornerRadius(5, 5);
    SetBorderWidth(2.0f);
    SetViewSize(164, 120);
    // Set the background color, so that we can set the item separator.
    SetBkColor(ColorF::DarkGray);
    // Add the flag to clip the view with the round corner.
    //AddFlag(VIEW_STATE_CLIPVIEW);
}

//////////////////////////////////////////////////////////////////////////

SdkListBox::~SdkListBox()
{
    SAFE_DELETE(m_pListBoxData->m_pItemNormapBitmap);
    SAFE_DELETE(m_pListBoxData->m_pItemSelectBitmap);
    SAFE_DELETE(m_pListBoxData->m_pItemHoverBitmap);
    SAFE_DELETE(m_pListBoxData->m_pItemPressBitmap);

    SAFE_DELETE(m_pListBoxData);
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::ShowView(BOOL isShow)
{
    // Visible changed.
    BOOL isVisibleChanged = (IsVisible() != isShow);

    // The view is pop-up view.
    BOOL isPopup = (VIEW_STATE_POPUP == (GetState() & VIEW_STATE_POPUP));
    if ( isPopup )
    {
        // If show, bring the list box to front.
        if ( isShow )
        {
            SetVisible(isShow);
            BringToFront();
        }

        // Save the current pop-up view.
        SdkWindow *pWindow = GetWindow();
        if (NULL != pWindow)
        {
            pWindow->WindowViews.CurrentPopupView = isShow ? this : NULL;
        }
    }

    SdkViewLayout::ShowView(isShow);

    // Perform the visible changed handler.
    if ( isVisibleChanged )
    {
        if ( NULL != m_pListBoxData->m_pListEventHandler )
        {
            m_pListBoxData->m_pListEventHandler->OnVisibleChanged(this);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::SetPopup(BOOL isPopup)
{
    if ( isPopup )
    {
        AddFlag(VIEW_STATE_POPUP);
    }
    else
    {
        RemoveFlag(VIEW_STATE_POPUP);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::SelecteItem(int nIndex)
{
    m_pListBoxData->m_nSelIndex = nIndex;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkListBox::AddItem(IN LPCWSTR lpText)
{
    // The view item is the image button, not show default background image.
    SdkButton *pChildItem = new SdkButton(FALSE);

    pChildItem->SetText((NULL != lpText) ? lpText : _T(""));
    //pChildItem->SetFontWeight(DWRITE_FONT_WEIGHT_BLACK);
    pChildItem->SetTextColor(ColorF(ColorF::White));
    pChildItem->SetTextSize(16);
    pChildItem->SetOnMouseHandler(this);
    pChildItem->SetOnClickHandler(this);

    BOOL bValue = AddView(pChildItem);
    // Succeed to add view, update the view's layout.
    if ( bValue )
    {
        UpdateLayout();
    }

    return bValue;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkListBox::AddItem(UINT32 uResId, HMODULE hInst)
{
    TCHAR szBuffer[1024] = { 0 };
    hInst = (NULL == hInst) ? GetModuleHandle(NULL) : hInst;
    INT32 nLen = LoadString(hInst, uResId, szBuffer, 1024);

    if ( nLen > 0 )
    {
        return AddItem(szBuffer);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkListBox::AddItem(IN LPCWSTR lpText, UINT32 uIndex)
{
    UNREFERENCED_PARAMETER(lpText);
    UNREFERENCED_PARAMETER(uIndex);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkListBox::DeleteItem(UINT32 uIndex)
{
    INT32 nSize = (INT32)m_vctChildren.size();
    if (m_pListBoxData->m_nSelIndex > (INT32)uIndex)
    {
        m_pListBoxData->m_nSelIndex--;
    }
    else if ((INT32)uIndex == nSize - 1 && m_pListBoxData->m_nSelIndex == (INT32)uIndex)
    {
        m_pListBoxData->m_nSelIndex--;
    }

    // Remote child from the list box.
    BOOL bValue = RemoveChildAt(uIndex);
    if ( bValue )
    {
        UpdateLayout();
    }

    return bValue;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkListBox::RemoveAllItems()
{
    return RemoveAllChildren();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkListBox::SetItemText(IN UINT32 index, IN LPCWSTR lpText)
{
    if (index >= (UINT32)m_vctChildren.size() || (NULL == lpText))
    {
        return FALSE;
    }

    dynamic_cast<SdkButton*>(m_vctChildren[index])->SetText(lpText);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkListBox::GetSelText(OUT LPWSTR lpText, IN UINT32 uSize)
{
    return GetItemText(GetSelIndex(), lpText, uSize);
}

//////////////////////////////////////////////////////////////////////////

UINT32 SdkListBox::GetSelTextLength()
{
    SdkViewElement *pView = NULL;
    BOOL retVal = GetChildAt(m_pListBoxData->m_nSelIndex, &pView);

    if ( retVal && (NULL != pView) )
    {
        return dynamic_cast<SdkButton*>(pView)->GetTextLength();
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkListBox::GetItemText(IN UINT32 index, OUT LPWSTR lpText, IN UINT32 uSize)
{
    SdkViewElement *pView = NULL;
    BOOL retVal = GetChildAt(index, &pView);

    if ( NULL != pView )
    {
        dynamic_cast<SdkButton*>(pView)->GetText(lpText, uSize);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

UINT32 SdkListBox::GetItemCount()
{
    return (UINT32)GetChildCount();
}

//////////////////////////////////////////////////////////////////////////

INT32 SdkListBox::GetSelIndex()
{
    return m_pListBoxData->m_nSelIndex;
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::SetCurSel(INT32 nIndex)
{
    if ( nIndex >= 0 && (UINT32)m_vctChildren.size() > (UINT32)nIndex )
    {
        m_pListBoxData->m_nSelIndex = nIndex;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::SetParent(SdkViewLayout *pParentView)
{
    SdkViewLayout::SetParent(pParentView);

    // When view add to a view layout, update the layout information.
    UpdateLayout();
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::SetListBoxEventHandler(IN IListBoxEventHandler *pListEventHandler)
{
    m_pListBoxData->m_pListEventHandler = pListEventHandler;
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::UpdateLayout()
{
    // The list box has parent view, mean it has been added to a view layout.
    if ( NULL != GetParent() )
    {
        INT32 nChildSize = (INT32)m_vctChildren.size();
        nChildSize = (0 == nChildSize) ? 1 : nChildSize;

        LAYOUTINFO layoutInfo = { 0 };
        GetLayoutInfo(&layoutInfo);
        layoutInfo.height = (FLOAT)(nChildSize * MENU_ITEM_HEIGHT + 2 * m_pListBoxData->m_fItemMargin) + nChildSize - 1;
        SetLayoutInfo(layoutInfo.x, layoutInfo.y, layoutInfo.width, layoutInfo.height);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkListBox::OnMouse(const LPMSG lpMsg, SdkViewElement *pSender, SdkViewElement *pSource)
{
    switch (lpMsg->message)
    {
    case WM_LBUTTONDOWN:
        {
            AddFlag(VIEW_STATE_PRESSED);
            // Save the temp selected index when mouse left button pressed down.
            m_pListBoxData->m_nPressIndex = GetIndexOfChild(pSource);
        }
        break;

    case WM_LBUTTONUP:
        {
            RemoveFlag(VIEW_STATE_PRESSED);
            // When mouse up, set the temp selected index to -1.
            m_pListBoxData->m_nPressIndex = -1;
            Invalidate();
        }
        break;

    case WM_MOUSEHOVER:
        {
            // If the left button is pressed, do not response the mouse hover message.
            BOOL isPress = (GetState() & VIEW_STATE_PRESSED);
            if ( !isPress )
            {
                m_pListBoxData->m_nHoverIndex = GetIndexOfChild(pSource);
                Invalidate();
            }
        }
        break;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F absRc = { 0, 0 };
    GetAbsoluteRect(absRc);

    // Draw the background.
    SdkViewElement::OnDrawBackground(pRenderTarget, absRc);

    // Draw all items.
    OnDrawAllItems(pRenderTarget);

    // Draw the hover item's background.
    OnDrawHoverBk(m_pListBoxData->m_nHoverIndex, pRenderTarget);

    // Draw the pressed item's background.
    OnDrawPressBk(m_pListBoxData->m_nPressIndex, pRenderTarget);

    // Draw the selected item's background.
    OnDrawSelectedBk(m_pListBoxData->m_nSelIndex, pRenderTarget);

    // Draw the view's border.
    OnDrawBoarder(pRenderTarget, absRc);
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    UNREFERENCED_PARAMETER(left);
    UNREFERENCED_PARAMETER(top);
    UNREFERENCED_PARAMETER(height);

    FLOAT fItemLeft   = m_pListBoxData->m_fItemMargin;
    FLOAT fItemTop    = m_pListBoxData->m_fItemMargin;
    FLOAT fItemWidth  = width - m_pListBoxData->m_fItemMargin * 2;
    FLOAT fItemHeight = MENU_ITEM_HEIGHT;
    LAYOUTINFO info = { 0 };
    INT32 nChildCount = GetChildCount();

    // Layout each child view.
    for (int i = 0; i < nChildCount; ++i)
    {
        SdkViewElement *pChild = m_vctChildren[i];
        pChild->GetLayoutInfo(&info);
        info.x = fItemLeft;
        info.y = fItemTop;
        info.width  = fItemWidth;
        info.height = fItemHeight;
        pChild->SetLayoutInfo(info.x, info.y, info.width, info.height);

        fItemTop += fItemHeight + 1;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::OnClick(SdkViewElement* view)
{
    UINT index = GetIndexOfChild(view);
    BOOL isSelChange = (m_pListBoxData->m_nSelIndex != (INT32)index);
    m_pListBoxData->m_nSelIndex = index;

    if ( isSelChange && (NULL != m_pListBoxData->m_pListEventHandler) )
    {
        m_pListBoxData->m_pListEventHandler->OnSelectedChanged(this);
    }

    Invalidate();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkListBox::OnLongClick(SdkViewElement *pView)
{
    UNREFERENCED_PARAMETER(pView);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::OnDrawAllItems(IN ID2D1RenderTarget *pRenderTarget)
{
    if ( !m_pListBoxData->m_pItemNormapBitmap->HasInitialized() )
    {
        m_pListBoxData->m_pItemNormapBitmap->Initialize(pRenderTarget);
    }

    // Draw child's normal bitmap.
    ID2D1Bitmap *pD2DBitmap = NULL;
    m_pListBoxData->m_pItemNormapBitmap->GetD2DBitmap(&pD2DBitmap);

    if ( NULL != pD2DBitmap )
    {
        int nChildCount = GetChildCount();
        for (int i = 0; i < nChildCount; ++i)
        {
            SdkViewElement *pChild = m_vctChildren[i];
            D2D1_RECT_F itemRc = pChild->GetDrawingRect();
            pRenderTarget->DrawBitmap(pD2DBitmap, itemRc);
        }
    }

    SAFE_RELEASE(pD2DBitmap);
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::OnDrawSelectedBk(INT32 nIndex, IN ID2D1RenderTarget *pRenderTarget)
{
    if ( (NULL != pRenderTarget) && (nIndex >= 0 && nIndex < (INT32)GetItemCount()) )
    {
        // Get the selected child.
        SdkViewElement *pSelChild = m_vctChildren[nIndex];
        D2D1_RECT_F selRc = pSelChild->GetDrawingRect();
        // Draw the bitmap.
        SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
        pD2DTheme->OnDrawBitmap(pSelChild, pRenderTarget, m_pListBoxData->m_pItemSelectBitmap, selRc);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::OnDrawHoverBk(INT32 nIndex, IN ID2D1RenderTarget *pRenderTarget)
{
    // If the hover item is the selected item, do nothing.
    if ( nIndex == m_pListBoxData->m_nSelIndex )
    {
        return;
    }

    if ( (NULL != pRenderTarget) && (nIndex >= 0 && nIndex < (INT32)GetItemCount()) )
    {
        // Get the selected child.
        SdkViewElement *pSelChild = m_vctChildren[nIndex];
        D2D1_RECT_F selRc = pSelChild->GetDrawingRect();
        // Draw the bitmap.
        SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
        pD2DTheme->OnDrawBitmap(pSelChild, pRenderTarget, m_pListBoxData->m_pItemHoverBitmap, selRc);
    }

    m_pListBoxData->m_nHoverIndex = -1;
}

//////////////////////////////////////////////////////////////////////////

void SdkListBox::OnDrawPressBk(INT32 nIndex, IN ID2D1RenderTarget *pRenderTarget)
{
    // If the hover item is the selected item, do nothing.
    if ( nIndex == m_pListBoxData->m_nSelIndex )
    {
        return;
    }

    if ( (NULL != pRenderTarget) && (nIndex >= 0 && nIndex < (INT32)GetItemCount()) )
    {
        // Get the selected child.
        SdkViewElement *pSelChild = m_vctChildren[nIndex];
        D2D1_RECT_F selRc = pSelChild->GetDrawingRect();
        // Draw the bitmap.
        SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
        pD2DTheme->OnDrawBitmap(pSelChild, pRenderTarget, m_pListBoxData->m_pItemPressBitmap, selRc);
    }
}
