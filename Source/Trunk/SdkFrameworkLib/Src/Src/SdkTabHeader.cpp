/*!
* @file SdkTabHeader.cpp
* 
* @brief This file defines the SdkTabView class.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#include "StdAfx.h"
#include "SdkTabHeader.h"
#include "SdkD2DTheme.h"
#include "SdkButton.h"
#include "SdkResManager.h"
#include "D2DRectUtility.h"
#include "D2DTextLayout.h"
#include "ITabHeaderEventHandler.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_VIEWS

#define TAB_BAR_HEIGHT          1                       // The tab bar height
#define TAB_MIN_WIDTH           32                      // The minimum width of tab.
#define TAB_RIGHT_MARGIN        40                      // The right margin.
#define TAB_ADD_SIZE            26                      // The size of add tab button.
#define TAB_NEXTPREV_SIZE       25                      // The size of next or prev button.

#define IDB_BUTTON_NEXT_TAB     1000                    // The id of next tab button.
#define IDB_BUTTON_PREV_TAB     1001                    // The id of previous tab button.

/*!
* @brief The internal data of tab header view.
*/
struct NAMESPACE_VIEWS::SdkTabHeader::_TABHEADER_INTERNALDATA
{
    FLOAT                   m_nFirstTabOffset;          // The first tab position offset.
    int                     m_nFocusTabIndex;           // The focus tab based zero index (mouse hover or press).
    int                     m_nSelTabIndex;             // The selected tab index.
    BOOL                    m_fMouseInAddTabRect;       // Indicate the mouse is in add tag rectangle.
    BOOL                    m_fAddTabVisible;           // Show add tab button or not.
    D2D1_SIZE_F             m_tabSize;                  // The tab size.
    D2DBitmap              *m_pNormalBitmap;            // The normal bitmap.
    D2DBitmap              *m_pHoverBitmap;             // The normal hover bitmap.
    D2DBitmap              *m_pPressBitmap;             // The press bitmap.
    D2DBitmap              *m_pSelectBitmap;            // The select bitmap.
    D2DBitmap              *m_pAddTabBitmap;            // The add button bitmap.
    D2DBitmap              *m_pTabBarBitmap;            // The tab bar bitmap.
    D2DBitmap              *m_pPrevTabBitmap;           // The left move bitmap.
    D2DBitmap              *m_pNextTabBitmap;           // The right move bitmap.
    D2DTextLayout          *m_pD2DTextLayout;           // The instance to draw text.
    SdkButton              *m_pNextTabButton;           // The next tab button.
    SdkButton              *m_pPrevTabButton;           // The previous tab button.
    ITabHeaderEventHandler *m_pTabEventHandler;         // The callback pointer.
    vector<TAB_INFO>        m_vctTabsList;              // The tab information list.
};


//////////////////////////////////////////////////////////////////////////

SdkTabHeader::SdkTabHeader()
{
    m_pTabHeaderData = new _TABHEADER_INTERNALDATA();

    m_pTabHeaderData->m_nFirstTabOffset    = 0,
    m_pTabHeaderData->m_nFocusTabIndex     = -1;
    m_pTabHeaderData->m_nSelTabIndex       = -1,
    m_pTabHeaderData->m_fMouseInAddTabRect = FALSE;
    m_pTabHeaderData->m_fAddTabVisible     = FALSE;
    m_pTabHeaderData->m_pTabEventHandler   = NULL;
    m_pTabHeaderData->m_tabSize.width      = 132;
    m_pTabHeaderData->m_tabSize.height     = 32;

    InitializeViews();
    SetClassName(CLASSNAME_TABHEADER);
    SetMinWidth(132);
    SetMaxWidth(132 * 20);
    SetMinHeight(TAB_MIN_WIDTH);
    SetMaxHeight(35);
    SetViewSize(280, 36);
}

//////////////////////////////////////////////////////////////////////////

SdkTabHeader::~SdkTabHeader()
{
    SAFE_DELETE(m_pTabHeaderData->m_pNormalBitmap);
    SAFE_DELETE(m_pTabHeaderData->m_pHoverBitmap);
    SAFE_DELETE(m_pTabHeaderData->m_pPressBitmap);
    SAFE_DELETE(m_pTabHeaderData->m_pSelectBitmap);
    SAFE_DELETE(m_pTabHeaderData->m_pAddTabBitmap);
    SAFE_DELETE(m_pTabHeaderData->m_pTabBarBitmap);
    SAFE_DELETE(m_pTabHeaderData->m_pPrevTabBitmap);
    SAFE_DELETE(m_pTabHeaderData->m_pNextTabBitmap);

    // Delete the tab information.
    int nSize = (int)m_pTabHeaderData->m_vctTabsList.size();

    for (int i = 0; i < nSize; ++i)
    {
        TAB_INFO &tabInfo = m_pTabHeaderData->m_vctTabsList[i];
        // Delete the text.
        SAFE_DELETE_ARRAY(tabInfo.lpszText);
    }

    SAFE_DELETE(m_pTabHeaderData);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabHeader::InsertTab(int nIndex, IN LPCTSTR lpText, LONG_PTR dwData)
{
    nIndex = min(0, nIndex);
    nIndex = max(0, GetTabCount());

    TAB_INFO tabInfo = { 0 };
    tabInfo.cchTextMax = MAX_PATH;
    tabInfo.cchTipTextMax = MAX_PATH;
    tabInfo.dwData = dwData;
    tabInfo.isSelected = FALSE;
    tabInfo.nMinWidth = TAB_MIN_WIDTH;

    if ( NULL != lpText )
    {
        int length = _tcslen(lpText) + 1;
        tabInfo.lpszText = new TCHAR[length];
        memset(tabInfo.lpszText, 0, length * sizeof(TCHAR));
        _tcscpy_s(tabInfo.lpszText, length, lpText);
    }

    m_pTabHeaderData->m_vctTabsList.insert(m_pTabHeaderData->m_vctTabsList.begin() + nIndex, tabInfo);

    // Show or hide next and prev tab buttons.
    ShowNextPrevTabButtons();
    Invalidate();

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabHeader::DeleteTab(int nIndex)
{
    int nTabCount = GetTabCount();
    BOOL fRetVal = (nIndex >= 0 && nIndex < nTabCount) ? TRUE : FALSE;

    // The tab count should have one tab at least.
    if ( fRetVal && nTabCount > 1 )
    {
        // First delete the memory pointed by lpszText.
        SAFE_DELETE_ARRAY(m_pTabHeaderData->m_vctTabsList[nIndex].lpszText);
        // Erase the item from the vector.
        m_pTabHeaderData->m_vctTabsList.erase(m_pTabHeaderData->m_vctTabsList.begin() + nIndex);

        nTabCount = GetTabCount();

        // The delete tag is the selected tab.
        if ( nTabCount == m_pTabHeaderData->m_nSelTabIndex )
        {
            SetSelTab(m_pTabHeaderData->m_nSelTabIndex - 1);
        }

        // Show or hide next and prev tab buttons.
        ShowNextPrevTabButtons();
        Invalidate();
    }

    return fRetVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabHeader::SetTabData(int nIndex, LONG_PTR dwData)
{
    BOOL fRetVal = (nIndex >= 0 && nIndex < GetTabCount()) ? TRUE : FALSE;

    if ( fRetVal )
    {
        TAB_INFO &tabInfo = m_pTabHeaderData->m_vctTabsList[nIndex];
        tabInfo.dwData = dwData;
    }

    return fRetVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabHeader::SetTabText(int nIndex, IN LPCTSTR lpText)
{
    BOOL fRetVal = ((nIndex >= 0 && nIndex < GetTabCount()) && (NULL != lpText)) ? TRUE : FALSE;

    if ( fRetVal )
    {
        TAB_INFO &tabInfo = m_pTabHeaderData->m_vctTabsList[nIndex];
        // First delete the old memory.
        SAFE_DELETE_ARRAY(tabInfo.lpszText);

        int length = _tcslen(lpText) + 1;
        tabInfo.lpszText = new TCHAR[length];
        memset(tabInfo.lpszText, 0, length * sizeof(TCHAR));
        _tcscpy_s(tabInfo.lpszText, length, lpText);
    }

    return fRetVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabHeader::SetTabInfo(int nIndex, IN const LPTAB_INFO lpTabInfo)
{
    BOOL fRetVal = ((nIndex >= 0 && nIndex < GetTabCount()) && (NULL != lpTabInfo)) ? TRUE : FALSE;

    if ( fRetVal )
    {
        int length = _tcslen(lpTabInfo->lpszText);
        TAB_INFO &tabInfo = m_pTabHeaderData->m_vctTabsList[nIndex];
        SAFE_DELETE_ARRAY(tabInfo.lpszText);
        tabInfo = *lpTabInfo;
        tabInfo.lpszText = new TCHAR[length + 1];
        _tcscpy_s(tabInfo.lpszText, length, lpTabInfo->lpszText);
    }

    return fRetVal;
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::SetTabOffset(int nTabNum)
{
    D2D_SIZE_F tabSize = m_pTabHeaderData->m_tabSize;
    FLOAT fAllTabsWidth = (GetTabCount() * tabSize.width + TAB_ADD_SIZE);
    FLOAT fMaxOffset = 0;
    FLOAT fMinOffset = -1 * (fAllTabsWidth + 2 * TAB_NEXTPREV_SIZE - GetWidth());
    FLOAT fFirstTabOffset = m_pTabHeaderData->m_nFirstTabOffset;

    // If the minimum offset value is bigger than zero, meaning all tabs width is less
    // than the tab header's width, so do not offset.
    if ( fMinOffset > 0 )
    {
        return;
    }

    fFirstTabOffset += (nTabNum * tabSize.width);
    fFirstTabOffset = min(fFirstTabOffset, fMaxOffset);
    fFirstTabOffset = max(fFirstTabOffset, fMinOffset);

    FLOAT fSubVal = (FLOAT)abs((int)(fFirstTabOffset - fMinOffset));
    if ( fSubVal < TAB_ADD_SIZE )
    {
        //fFirstTabOffset = fMinOffset;
    }

    m_pTabHeaderData->m_nFirstTabOffset = fFirstTabOffset;

    Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::SetSelTab(int nIndex)
{
    if ( nIndex < 0 || nIndex >= GetTabCount() )
    {
        return;
    }

    int nOldSelIndex = m_pTabHeaderData->m_nSelTabIndex;

    if ( nOldSelIndex != nIndex )
    {
        // De-selected the old selected tab.
        if ( nOldSelIndex >= 0 && nOldSelIndex < GetTabCount() )
        {
            m_pTabHeaderData->m_vctTabsList[nOldSelIndex].isSelected = FALSE;
        }

        m_pTabHeaderData->m_vctTabsList[nIndex].isSelected = TRUE;
        m_pTabHeaderData->m_nSelTabIndex = nIndex;

        // Perform the selected tab changed notification.
        if ( NULL != m_pTabHeaderData->m_pTabEventHandler )
        {
            m_pTabHeaderData->m_pTabEventHandler->OnSelectedChanged(
                this,
                nOldSelIndex,
                nIndex);
        }

        Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::SetTabWidth(int nTabWidth)
{
    nTabWidth = min(nTabWidth, (int)(GetWidth() - TAB_RIGHT_MARGIN));
    nTabWidth = max(nTabWidth, TAB_MIN_WIDTH);

    m_pTabHeaderData->m_tabSize.width = (FLOAT)nTabWidth;
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::SetAddTabVisible(BOOL fVisible)
{
    if ( m_pTabHeaderData->m_fAddTabVisible != fVisible )
    {
        m_pTabHeaderData->m_fAddTabVisible = fVisible;

        if ( m_pTabHeaderData->m_fAddTabVisible )
        {
            FLOAT width  = GetWidth();
            FLOAT height = GetHeight();
            FLOAT fMod = (FLOAT)((int)width % (int)m_pTabHeaderData->m_tabSize.width);

            if ( fMod < TAB_RIGHT_MARGIN )
            {
                width += TAB_RIGHT_MARGIN;
            }

            SetViewSize(width, height);
        }

        Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::SetTabHeaderEventHandler(ITabHeaderEventHandler *pEventHandler)
{
    m_pTabHeaderData->m_pTabEventHandler = pEventHandler;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabHeader::GetTabInfo(int nIndex, OUT LPTAB_INFO lpTabInfo)
{
    BOOL fRetVal = ((nIndex >= 0 && nIndex < GetTabCount()) && (NULL != lpTabInfo));

    if ( fRetVal )
    {
        TAB_INFO &tabInfo = m_pTabHeaderData->m_vctTabsList[nIndex];
        lpTabInfo->cchTextMax    = tabInfo.cchTextMax;
        lpTabInfo->cchTipTextMax = tabInfo.cchTipTextMax;
        lpTabInfo->dwData        = tabInfo.dwData;
        lpTabInfo->isSelected    = tabInfo.isSelected;
        lpTabInfo->nMinWidth     = tabInfo.nMinWidth;
        _tcscpy_s(lpTabInfo->lpszText, lpTabInfo->cchTextMax, tabInfo.lpszText);
    }

    return fRetVal;
}

//////////////////////////////////////////////////////////////////////////

LONG_PTR SdkTabHeader::GetTabData(int nIndex)
{
    LONG_PTR dwData = 0;

    BOOL fRetVal = (nIndex >= 0 && nIndex < GetTabCount()) ? TRUE : FALSE;
    if ( fRetVal )
    {
        TAB_INFO &tabInfo = m_pTabHeaderData->m_vctTabsList[nIndex];
        dwData = tabInfo.dwData;
    }

    return dwData;
}

//////////////////////////////////////////////////////////////////////////

int SdkTabHeader::GetSelTab() const
{
    return (int)m_pTabHeaderData->m_nSelTabIndex;
}

//////////////////////////////////////////////////////////////////////////

int SdkTabHeader::GetTabCount() const
{
    return (int)m_pTabHeaderData->m_vctTabsList.size();
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::InitializeViews()
{
    m_pTabHeaderData->m_pNormalBitmap  = new D2DBitmap();
    m_pTabHeaderData->m_pHoverBitmap   = new D2DBitmap();
    m_pTabHeaderData->m_pSelectBitmap  = new D2DBitmap();
    m_pTabHeaderData->m_pPressBitmap   = new D2DBitmap();
    m_pTabHeaderData->m_pTabBarBitmap  = new D2DBitmap();
    m_pTabHeaderData->m_pAddTabBitmap  = new D2DBitmap();
    m_pTabHeaderData->m_pPrevTabBitmap = new D2DBitmap();
    m_pTabHeaderData->m_pNextTabBitmap = new D2DBitmap();

    m_pTabHeaderData->m_pNormalBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_TAB_NORMAL));
    m_pTabHeaderData->m_pHoverBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_TAB_NORMAL_HOVER));
    m_pTabHeaderData->m_pSelectBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_TAB_SELECT));
    m_pTabHeaderData->m_pPressBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_TAB_NORMAL_PRESS));
    m_pTabHeaderData->m_pTabBarBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_TAB_BAR));
    m_pTabHeaderData->m_pAddTabBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_TAB_ADD));
    m_pTabHeaderData->m_pPrevTabBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_TAB_ARROW_LEFT));
    m_pTabHeaderData->m_pNextTabBitmap->LoadFromHBITMAP(SdkResManager::GetImage(IDB_TAB_ARROW_RIGHT));

    // The D2D Text layout object is used to draw text.
    m_pTabHeaderData->m_pD2DTextLayout = new D2DTextLayout();
    m_pTabHeaderData->m_pD2DTextLayout->Initialize();
    m_pTabHeaderData->m_pD2DTextLayout->SetTextFontSize(18.0f);
    m_pTabHeaderData->m_pD2DTextLayout->SetTextColor(ColorF(ColorF::White));

    // The next and previous tab button .
    m_pTabHeaderData->m_pNextTabButton = new SdkButton(FALSE);
    m_pTabHeaderData->m_pPrevTabButton = new SdkButton(FALSE);
    m_pTabHeaderData->m_pNextTabButton->SetId(IDB_BUTTON_NEXT_TAB);
    m_pTabHeaderData->m_pPrevTabButton->SetId(IDB_BUTTON_PREV_TAB);
    m_pTabHeaderData->m_pPrevTabButton->SetViewSize(TAB_NEXTPREV_SIZE, TAB_NEXTPREV_SIZE);
    m_pTabHeaderData->m_pNextTabButton->SetViewSize(TAB_NEXTPREV_SIZE, TAB_NEXTPREV_SIZE);
    m_pTabHeaderData->m_pNextTabButton->SetOnClickHandler(this);
    m_pTabHeaderData->m_pPrevTabButton->SetOnClickHandler(this);
    m_pTabHeaderData->m_pNextTabButton->SetImage(SdkResManager::GetImage(IDB_TAB_ARROW_RIGHT),  VIEW_STYLE_OVERLAY);
    m_pTabHeaderData->m_pPrevTabButton->SetImage(SdkResManager::GetImage(IDB_TAB_ARROW_LEFT),   VIEW_STYLE_OVERLAY);
    m_pTabHeaderData->m_pNextTabButton->SetImage(SdkResManager::GetImage(IDB_TAB_NORMAL),       VIEW_STYLE_NORMAL);
    m_pTabHeaderData->m_pPrevTabButton->SetImage(SdkResManager::GetImage(IDB_TAB_NORMAL),       VIEW_STYLE_NORMAL);
    m_pTabHeaderData->m_pNextTabButton->SetImage(SdkResManager::GetImage(IDB_TAB_NORMAL_HOVER), VIEW_STYLE_MOUSEHOVER);
    m_pTabHeaderData->m_pPrevTabButton->SetImage(SdkResManager::GetImage(IDB_TAB_NORMAL_HOVER), VIEW_STYLE_MOUSEHOVER);
    m_pTabHeaderData->m_pNextTabButton->SetImage(SdkResManager::GetImage(IDB_TAB_NORMAL_PRESS), VIEW_STYLE_MOUSEDOWN);
    m_pTabHeaderData->m_pPrevTabButton->SetImage(SdkResManager::GetImage(IDB_TAB_NORMAL_PRESS), VIEW_STYLE_MOUSEDOWN);
    m_pTabHeaderData->m_pNextTabButton->SetOverlayInfo(0, 0, TAB_NEXTPREV_SIZE, TAB_NEXTPREV_SIZE);
    m_pTabHeaderData->m_pPrevTabButton->SetOverlayInfo(0, 0, TAB_NEXTPREV_SIZE, TAB_NEXTPREV_SIZE);
    m_pTabHeaderData->m_pNextTabButton->SetVisible(FALSE);
    m_pTabHeaderData->m_pPrevTabButton->SetVisible(FALSE);

    AddView(m_pTabHeaderData->m_pNextTabButton);
    AddView(m_pTabHeaderData->m_pPrevTabButton);
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::ShowNextPrevTabButtons()
{
    D2D_SIZE_F tabSize  = m_pTabHeaderData->m_tabSize;
    FLOAT fAllTabsWidth = (GetTabCount() * tabSize.width + TAB_ADD_SIZE);
    FLOAT fTabsWidth = (GetWidth() - 2 * TAB_NEXTPREV_SIZE);
    BOOL isVisible = (fAllTabsWidth > fTabsWidth);

    m_pTabHeaderData->m_pNextTabButton->SetVisible(isVisible);
    m_pTabHeaderData->m_pPrevTabButton->SetVisible(isVisible);
}

//////////////////////////////////////////////////////////////////////////

int SdkTabHeader::GetTabIndexFromPoint(FLOAT xPos, FLOAT yPos)
{
    int nTabIndex = -1;

    // Convert the specified point to another point relative the view, the new point
    // is got through multiple the invert matrix of the view's current matrix.
    // The returned is absolute point, because the second parameter of ConvertPoint is FALSE.
    D2D1_POINT_2F newPt = ConvertPoint(D2D1::Point2F(xPos, yPos), FALSE);
    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);

    rc.left += m_pTabHeaderData->m_nFirstTabOffset;
    rc.right = rc.left + m_pTabHeaderData->m_tabSize.width;

    int nTabCount = GetTabCount();
    for (int i = 0; i < nTabCount; ++i)
    {
        if ( D2DRectUtility::PtInD2DRect(rc, newPt) )
        {
            nTabIndex = i;
            break;
        }

        D2DRectUtility::OffsetD2DRectF(rc, m_pTabHeaderData->m_tabSize.width, 0);
    }

    // Check the mouse is located at add tab button rectangle.
    if ( m_pTabHeaderData->m_fAddTabVisible && (-1 == nTabIndex) )
    {
        GetAbsoluteRect(rc);
        rc.left += (GetTabCount() * m_pTabHeaderData->m_tabSize.width + m_pTabHeaderData->m_nFirstTabOffset);
        rc.right = rc.left + TAB_ADD_SIZE;
        m_pTabHeaderData->m_fMouseInAddTabRect = D2DRectUtility::PtInD2DRect(rc, newPt);
    }

    return nTabIndex;
}

//////////////////////////////////////////////////////////////////////////

D2DBitmap* SdkTabHeader::GetDrawingTabBitmap(int nTabIndex)
{
    D2DBitmap *pD2DBitmap = m_pTabHeaderData->m_pNormalBitmap;

    if ( nTabIndex >= 0 && nTabIndex < GetTabCount() )
    {
        // If the tab to be drawing is the focus tab.
        if ( nTabIndex == m_pTabHeaderData->m_nFocusTabIndex )
        {
            // The focus tab is the select tab.
            if ( m_pTabHeaderData->m_nFocusTabIndex == (int)m_pTabHeaderData->m_nSelTabIndex )
            {
                pD2DBitmap = m_pTabHeaderData->m_pSelectBitmap;
            }
            else
            {
                // The left button is pressed.
                BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
                pD2DBitmap = isPressed ? m_pTabHeaderData->m_pPressBitmap : m_pTabHeaderData->m_pHoverBitmap;
            }
        }
        else
        {
            // Draw the select or normal.
            pD2DBitmap = (nTabIndex == (int)m_pTabHeaderData->m_nSelTabIndex) ? m_pTabHeaderData->m_pSelectBitmap : m_pTabHeaderData->m_pNormalBitmap;
        }
    }

    return pD2DBitmap;
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::OnAddTabButtonClick()
{
    // Call the callback to notify client side to insert a tab.
    if ( NULL != m_pTabHeaderData->m_pTabEventHandler )
    {
        if ( m_pTabHeaderData->m_pTabEventHandler->OnInsertTab(this) )
        {
            SetTabOffset(-1);
        }
    }
    else
    {
        InsertTab(GetTabCount(), _T(""), 0);
        SetTabOffset(-1);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    SdkViewElement::OnDrawItem(pRenderTarget);

    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);
    D2D1_RECT_F rcClip = rc;
    rcClip.right -= (2 * TAB_NEXTPREV_SIZE + 3);

    // Clip the render target to make sure no shapes are drawn out of the view's rectangle.
    pRenderTarget->PushAxisAlignedClip(rcClip, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

    // Draw tabs.
    OnDrawTabs(pRenderTarget, rc);

    pRenderTarget->PopAxisAlignedClip();

    // Draw the tab bar.
    D2D1_RECT_F tabRc = rc;
    tabRc.top = tabRc.bottom - TAB_BAR_HEIGHT;

    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
    // Draw tab bar.
    pD2DTheme->OnDrawBackground(this, pRenderTarget, m_pTabHeaderData->m_pTabBarBitmap, tabRc);
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::OnDrawTabs(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F &rc)
{
    D2D1_SIZE_F tabSize = m_pTabHeaderData->m_tabSize;
    D2D1_RECT_F tabRc = rc;

    tabRc.left += m_pTabHeaderData->m_nFirstTabOffset;
    tabRc.top = tabRc.bottom - tabSize.height - (TAB_BAR_HEIGHT - 1);
    tabRc.bottom = tabRc.top + tabSize.height;
    tabRc.right = tabRc.left + tabSize.width;

    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
    int nTabCount = GetTabCount();

    for (int i = 0; i < nTabCount; ++i)
    {
        if ( tabRc.right <= rc.left || tabRc.left >= rc.right )
        {
            D2DRectUtility::OffsetD2DRectF(tabRc, tabSize.width, 0);
            continue;
        }

        // Draw tabs.
        pD2DTheme->OnDrawTabViewTab(
            this,
            pRenderTarget,
            GetDrawingTabBitmap(i),
            tabRc);

        // Draw text of tab.
        OnDrawTabText(pRenderTarget, i, tabRc);

        // Offset the tab rectangle.
        D2DRectUtility::OffsetD2DRectF(tabRc, tabSize.width, 0);
    }

    // Draw the add button, the width is equal to the height.
    OnDrawAddTabButton(pRenderTarget, tabRc);
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::OnDrawTabText(ID2D1RenderTarget *pRenderTarget, int nTabIndex, const D2D1_RECT_F &rc)
{
    int nTabCount = GetTabCount();

    BOOL isValid = (nTabCount >= 0 && nTabIndex < nTabCount) && (NULL != pRenderTarget);
    if ( !isValid )
    {
        return;
    }

    TAB_INFO &tabInfo = m_pTabHeaderData->m_vctTabsList[nTabIndex];

    int length = _tcslen(tabInfo.lpszText);
    if ( length > 0 )
    {
        D2D1_RECT_F tabRc = rc;
        tabRc.left  += 2;
        tabRc.right -= 2;
        m_pTabHeaderData->m_pD2DTextLayout->SetMaxWidth((tabRc.right - tabRc.left));
        m_pTabHeaderData->m_pD2DTextLayout->SetMaxHeight((tabRc.bottom - tabRc.top));
        m_pTabHeaderData->m_pD2DTextLayout->SetTextStartPoint(D2D1::Point2F(tabRc.left, tabRc.top));
        m_pTabHeaderData->m_pD2DTextLayout->SetText(tabInfo.lpszText);
        m_pTabHeaderData->m_pD2DTextLayout->DrawText(pRenderTarget);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::OnDrawAddTabButton(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F &rc)
{
    if ( m_pTabHeaderData->m_fAddTabVisible )
    {
        D2D1_RECT_F tabRc = rc;
        tabRc.top   = tabRc.bottom - TAB_ADD_SIZE;
        tabRc.right = tabRc.left + TAB_ADD_SIZE;

        // Get the bitmap as the background for the add tab bitmap.
        D2DBitmap *pD2DBitmap = m_pTabHeaderData->m_pNormalBitmap;

        if ( m_pTabHeaderData->m_fMouseInAddTabRect )
        {
            BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));
            pD2DBitmap = isPressed ? m_pTabHeaderData->m_pPressBitmap : m_pTabHeaderData->m_pHoverBitmap;
        }

        SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance();
        pD2DTheme->OnDrawTabViewTab(this, pRenderTarget, pD2DBitmap, tabRc);
        pD2DTheme->OnDrawBackground(this, pRenderTarget, m_pTabHeaderData->m_pAddTabBitmap, tabRc);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::OnClick(SdkViewElement *pView)
{
    switch (pView->GetId())
    {
    case IDB_BUTTON_PREV_TAB:
        SetTabOffset(1);
        break;

    case IDB_BUTTON_NEXT_TAB:
        SetTabOffset(-1);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabHeader::OnLongClick(SdkViewElement *pView)
{
    UNREFERENCED_PARAMETER(pView);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkTabHeader::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    if ( fChanged )
    {
        // Set the layout of next and previous buttons.
        FLOAT fLeft = width  - TAB_NEXTPREV_SIZE;
        FLOAT fTop  = height - TAB_NEXTPREV_SIZE - 1;

        if ( NULL != m_pTabHeaderData->m_pNextTabButton )
        {
            m_pTabHeaderData->m_pNextTabButton->SetViewPos(fLeft, fTop);
        }

        if ( NULL != m_pTabHeaderData->m_pPrevTabButton )
        {
            m_pTabHeaderData->m_pPrevTabButton->SetViewPos(fLeft - TAB_NEXTPREV_SIZE, fTop);
        }

        // When the size is changed, re-decide show or hide next prev tab buttons.
        ShowNextPrevTabButtons();
    }

    SdkViewLayout::OnLayout(fChanged, left, top, width, height);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabHeader::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL handled = TRUE;

    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
    BOOL isPressed = (VIEW_STATE_PRESSED == (GetState() & VIEW_STATE_PRESSED));

    // When a mouse message is reached, set this member to FALSE, this member will be
    // re-calculated in GetTabIndexFromPoint method.
    m_pTabHeaderData->m_fMouseInAddTabRect = FALSE;

    switch (message)
    {
    case WM_LBUTTONDOWN:
        m_pTabHeaderData->m_nFocusTabIndex = GetTabIndexFromPoint(xPos, yPos);
        Invalidate();
        break;

    case WM_MOUSELEAVE:
        // Set the focus tab index t0 -1 when the mouse leaves the view.
        m_pTabHeaderData->m_nFocusTabIndex = -1;
        Invalidate();
        break;

    case WM_MOUSEMOVE:
        {
            // If the left button is pressed, do nothing.
            if ( !isPressed )
            {
                m_pTabHeaderData->m_nFocusTabIndex = GetTabIndexFromPoint(xPos, yPos);
                Invalidate();
            }
        }
        break;

    case WM_LBUTTONUP:
        {
            BOOL isTouchOn = IsPtInRect(xPos, yPos);
            if ( isPressed && isTouchOn )
            {
                int nTabIndex = GetTabIndexFromPoint(xPos, yPos);
                // If the current focus tab index is the same tab on which the left button up, we
                // think the button down and button up acts at the same tab.
                if ( nTabIndex >= 0 && nTabIndex == m_pTabHeaderData->m_nFocusTabIndex )
                {
                    SetSelTab(nTabIndex);
                }

                // If the mouse in add tab rectangle, so we think the use clicks on this button.
                if ( m_pTabHeaderData->m_fMouseInAddTabRect )
                {
                    OnAddTabButtonClick();
                }
            }

            m_pTabHeaderData->m_nFocusTabIndex = -1;
            Invalidate();
        }
        break;

    default:
        handled = FALSE;
        break;
    }

    SdkViewElement::OnMouseEvent(message, wParam, lParam);

    return handled;
}
