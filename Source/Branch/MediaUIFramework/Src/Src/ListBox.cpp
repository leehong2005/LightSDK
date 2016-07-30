/*!
* @file MenuList.cpp
* 
* @brief This file defines the MenuList class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/14
*/

#include "ListBox.h"
#include "Resource.h"

#define MENU_ITEM_HEIGHT    30

ListBox::ListBox() : m_nSelIndex(-1),
                     m_nTempSelIndex(-1),
                     m_fItemMargin(1.0f),
                     m_isDrawTempSelBK(FALSE),
                     m_pListEventHandler(NULL)
{
    SetClickable(FALSE);
    SetRoundCornerEnable(TRUE);
    AddFlag(VIEW_STATE_CLIPVIEW);
    SetBorderWidth(2.0f);

    m_layoutInfo.width  = 200;
    m_pItemFocusBitmap  = new D2DBitmap();
    m_pItemNormapBitmap = new D2DBitmap();
    m_pItemFocusBitmap->LoadFromResource(IDB_PNG_DROPDOWN_FOCUS, HINST_THISCOMPONENT);
    m_pItemNormapBitmap->LoadFromResource(IDB_PNG_DROPDOWN_BASE, HINST_THISCOMPONENT);
}

//////////////////////////////////////////////////////////////////////////

ListBox::~ListBox()
{
    SAFE_DELETE(m_pItemFocusBitmap);
    SAFE_DELETE(m_pItemNormapBitmap);
}

//////////////////////////////////////////////////////////////////////////

void ListBox::ShowView(BOOL isShow)
{
    BOOL isVisibleChanged = (IsVisible() != isShow);
    PopupView::ShowView(isShow);
    if (isVisibleChanged && (NULL != m_pListEventHandler))
    {
        m_pListEventHandler->OnVisibleChanged(this);
    }
}

//////////////////////////////////////////////////////////////////////////

void ListBox::SelecteItem(UINT32 index)
{
    m_nSelIndex = index;
}

//////////////////////////////////////////////////////////////////////////

BOOL ListBox::AddItem(IN LPCWSTR lpText)
{
    LPCWSTR lpItemText = (NULL != lpText) ? lpText : _T("");

    ImageButton *pChildItem = new ImageButton();
    pChildItem->SetText(lpItemText);
    pChildItem->SetFontWeight(DWRITE_FONT_WEIGHT_BLACK);
    pChildItem->SetTextColor(ColorF(ColorF::White));
    pChildItem->SetFocusTextColor(ColorF(ColorF::White));
    pChildItem->SetOnClickHandler(this);
    pChildItem->SetBackgroundColor(ColorF(0, 0));
    pChildItem->SetOnMouseHandler(this);

    BOOL bValue = AddView(pChildItem);
    if (bValue)
    {
        UpdateLayout();
    }

    return bValue;
}

//////////////////////////////////////////////////////////////////////////

BOOL ListBox::AddItem(UINT32 uResId, HMODULE hInst)
{
    TCHAR szBuffer[1024] = { 0 };
    hInst = (NULL == hInst) ? GetModuleHandle(NULL) : hInst;
    INT32 nLen = LoadString(hInst, uResId, szBuffer, 1024);
    if (nLen > 0)
    {
        return AddItem(szBuffer);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ListBox::AddItem(IN LPCWSTR lpText, UINT32 uIndex)
{
    UNREFERENCED_PARAMETER(lpText);
    UNREFERENCED_PARAMETER(uIndex);
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ListBox::DeleteItem(UINT32 uIndex)
{
    INT32 nSize = (INT32)m_vctChildren.size();
    if (m_nSelIndex > (INT32)uIndex)
    {
        m_nSelIndex--;
    }
    else if ((INT32)uIndex == nSize - 1 && m_nSelIndex == (INT32)uIndex)
    {
        m_nSelIndex--;
    }

    BOOL bValue = RemoveChildAt(uIndex);
    if (bValue)
    {
        UpdateLayout();
    }

    return bValue;
}

//////////////////////////////////////////////////////////////////////////

BOOL ListBox::RemoveAllItem()
{
    return RemoveAllChildren();
}

//////////////////////////////////////////////////////////////////////////

BOOL ListBox::SetItemText(IN UINT32 index, IN LPCWSTR lpText)
{
    if (index >= (UINT32)m_vctChildren.size() || (NULL == lpText))
    {
        return FALSE;
    }

    dynamic_cast<ImageButton*>(m_vctChildren[index])->SetText(lpText);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ListBox::GetSelItemText(OUT LPWSTR lpText, IN UINT32 uSize)
{
    return GetItemText(GetSelIndex(), lpText, uSize);
}

//////////////////////////////////////////////////////////////////////////

UINT32 ListBox::GetSelItemTextLength()
{
    INT32 nSelIndex = GetSelIndex();
    if (nSelIndex >= 0 && nSelIndex < (INT32)GetItemCount())
    {
        return dynamic_cast<ImageButton*>(m_vctChildren[nSelIndex])->GetTextLength();
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL ListBox::GetItemText(IN UINT32 index, OUT LPWSTR lpText, IN UINT32 uSize)
{
    if (index >= (UINT32)m_vctChildren.size() || (NULL == lpText) || (0 == uSize))
    {
        return FALSE;
    }

    dynamic_cast<ImageButton*>(m_vctChildren[index])->GetText(lpText, uSize);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

UINT32 ListBox::GetItemCount()
{
    return (UINT32)m_vctChildren.size();
}

//////////////////////////////////////////////////////////////////////////

INT32 ListBox::GetSelIndex()
{
    return m_nSelIndex;
}

//////////////////////////////////////////////////////////////////////////

void ListBox::SetCurSel(INT32 nIndex)
{
    if (nIndex >= 0 && (UINT32)m_vctChildren.size() > (UINT32)nIndex)
    {
        m_nSelIndex = nIndex;
    }
}

//////////////////////////////////////////////////////////////////////////

void ListBox::SetParent(ViewLayout *pParentView)
{
    ViewLayout::SetParent(pParentView);
    UpdateLayout();
}

//////////////////////////////////////////////////////////////////////////

void ListBox::SetItemFocusColor(const D2D1_COLOR_F& color)
{
    UINT32 uSize = (UINT32)m_vctChildren.size();
    for (UINT32 i = 0; i < uSize; i++)
    {
        (dynamic_cast<ImageButton*>(m_vctChildren[i]))->SetFocusTextColor(color);
    }
}

//////////////////////////////////////////////////////////////////////////

void ListBox::SetListEventHandler(IN ListViewEventHandler *pListEventHandler)
{
    m_pListEventHandler = pListEventHandler;
}

//////////////////////////////////////////////////////////////////////////

void ListBox::UpdateLayout()
{
    if (NULL == GetParent(NULL))
    {
        return;
    }

    INT32 nChildSize = (INT32)m_vctChildren.size();
    nChildSize = (0 == nChildSize) ? 1 : nChildSize;

    LAYOUTINFO layoutInfo = { 0 };
    GetLayoutInfo(&layoutInfo);
    layoutInfo.height = (FLOAT)(nChildSize * MENU_ITEM_HEIGHT + 2 * m_fItemMargin);
    SetLayoutInfo(&layoutInfo);
}

//////////////////////////////////////////////////////////////////////////

void  ListBox::SetItemFocusState(INT32 nSelIndex, BOOL fAddState)
{
    if (nSelIndex >= 0 && nSelIndex < GetChildCount())
    {
        m_nSelIndex = nSelIndex;
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL ListBox::OnMouse(const LPMSG lpMsg, ViewElement *pSender, ViewElement *pSource)
{
    switch (lpMsg->message)
    {
    case WM_LBUTTONDOWN:
        {
            m_nTempSelIndex = GetIndexOfChild(pSource);
            m_isDrawTempSelBK = TRUE;
        }
        break;

    case WM_LBUTTONUP:
        {
            m_isDrawTempSelBK = FALSE;
        }
        break;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void ListBox::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    if (!m_pItemNormapBitmap->HasInitialized())
    {
        m_pItemNormapBitmap->Initialize(pRenderTarget);
    }

    ID2D1Bitmap *pD2DBitmap = NULL;
    m_pItemNormapBitmap->GetD2DBitmap(&pD2DBitmap);
    if (NULL != pD2DBitmap)
    {
        INT32 nChildCount = GetChildCount();
        for (INT32 i = 0; i < nChildCount; ++i)
        {
            ViewElement *pChild = m_vctChildren[i];
            D2D1_RECT_F itemRc = pChild->GetDrawingRect();
            pRenderTarget->DrawBitmap(pD2DBitmap, itemRc);
        }
    }
    SAFE_RELEASE(pD2DBitmap);

    OnDrawSelBackground(m_isDrawTempSelBK ? m_nTempSelIndex : m_nSelIndex, pRenderTarget);

    D2D1_RECT_F absRc = { 0, 0 };
    GetAbsoluteRect(absRc);
    OnDrawBoarder(pRenderTarget, absRc);
}

//////////////////////////////////////////////////////////////////////////

void ListBox::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    UNREFERENCED_PARAMETER(left);
    UNREFERENCED_PARAMETER(top);
    UNREFERENCED_PARAMETER(height);

    FLOAT fItemLeft   = m_fItemMargin;
    FLOAT fItemTop    = m_fItemMargin;
    FLOAT fItemWidth  = width - m_fItemMargin * 2;
    FLOAT fItemHeight = MENU_ITEM_HEIGHT;
    LAYOUTINFO info = { 0 };
    INT32 nChildCount = GetChildCount();

    for (INT32 i = 0; i < nChildCount; ++i)
    {
        ViewElement *pChild = m_vctChildren[i];
        pChild->GetLayoutInfo(&info);
        info.x = fItemLeft;
        info.y = fItemTop;
        info.width  = fItemWidth;
        info.height = fItemHeight;
        pChild->SetLayoutInfo(&info);
        fItemTop += fItemHeight;
    }
}

//////////////////////////////////////////////////////////////////////////

void ListBox::OnClick(ViewElement* view)
{
    UINT index = GetIndexOfChild(view);
    BOOL isSelChange = (m_nSelIndex != (INT32)index);
    m_nSelIndex = index;

    if (isSelChange && (NULL != m_pListEventHandler))
    {
        m_pListEventHandler->OnSelectedChanged(this);
    }

    Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void ListBox::OnDrawSelBackground(INT32 nIndex, IN ID2D1RenderTarget *pRenderTarget)
{
    if ((NULL != pRenderTarget) && (nIndex >= 0 && nIndex < (INT32)GetItemCount()))
    {
        if (!m_pItemFocusBitmap->HasInitialized())
        {
            m_pItemFocusBitmap->Initialize(pRenderTarget);
        }

        ID2D1Bitmap *pD2DBitmap = NULL;
        m_pItemFocusBitmap->GetD2DBitmap(&pD2DBitmap);
        if (NULL != pD2DBitmap)
        {
            ViewElement *pSelChild = m_vctChildren[nIndex];
            D2D1_RECT_F selRc = pSelChild->GetDrawingRect();
            D2DRectUtility::InflateD2DRectF(selRc, -1, 0);
            pRenderTarget->DrawBitmap(pD2DBitmap, selRc);
        }
        SAFE_RELEASE(pD2DBitmap);
    }
}
