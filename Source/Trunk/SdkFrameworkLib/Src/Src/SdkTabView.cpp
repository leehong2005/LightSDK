/*!
* @file SdkTabView.cpp
* 
* @brief This file defines the SdkTabView class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#include "StdAfx.h"
#include "SdkTabView.h"
#include "SdkTabHeader.h"
#include "SdkD2DTheme.h"
#include "D2DRectUtility.h"
#include "SdkResManager.h"
#include "ITabViewEventHandler.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_VIEWS

#define TAB_HEADER_HEIGHT       32              // The height of tab header.

/*!
* @brief The Tab view internal data.
*/
struct NAMESPACE_VIEWS::SdkTabView::_TABVIEW_INTERNALDATA
{
    D2D1_RECT_F           m_rcTabPage;          // The rectangle of tab page.
    TAB_DOCK_DIR          m_eDockDir;           // The dock direction.
    D2DBitmap            *m_pBkImage;           // The background image.
    ITabViewEventHandler *m_pEventHalder;       // The event handler.
};


//////////////////////////////////////////////////////////////////////////

SdkTabView::SdkTabView()
{
    m_pTabHeader = new SdkTabHeader();
    m_pTabHeader->SetTabHeaderEventHandler(this);
    m_pTabHeader->SetTabWidth(100);

    m_pInternalData = new _TABVIEW_INTERNALDATA();
    ZeroMemory(m_pInternalData, sizeof(_TABVIEW_INTERNALDATA));
    m_pInternalData->m_eDockDir   = TAB_DOCK_NONE;
    m_pInternalData->m_pBkImage   = new D2DBitmap();

    // This is the child of tab view, so it needs not to delete.
    AddView(m_pTabHeader);
    SetMinHeight(50);
    SetMinWidth(100);
    SetViewSize(150, 150);
    SetTabDockDir(TAB_DOCK_TOP);
    SetClassName(CLASSNAME_TABVIEW);

    // Insert a default tab.
    InsertTab(0, _T(""));
    SetSelTab(0);

    m_pInternalData->m_pBkImage->LoadFromHBITMAP(SdkResManager::GetImage(IDB_TAB_BK));
}

//////////////////////////////////////////////////////////////////////////

SdkTabView::~SdkTabView()
{
    SAFE_DELETE(m_pInternalData->m_pBkImage);
    SAFE_DELETE(m_pInternalData);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabView::InsertTab(int nIndex, IN LPCTSTR lpTabText)
{
    BOOL retVal = m_pTabHeader->InsertTab(nIndex, lpTabText, 0);

    // If succeed to insert tab, add the tab page.
    if ( retVal )
    {
        D2D1_RECT_F rcTabPage = SetTabHeaderLayout(GetWidth(), GetHeight());
        SdkViewLayout *pTabPage = new SdkViewLayout();
        pTabPage->SetVisible(FALSE);
        pTabPage->SetViewPos(rcTabPage.left, rcTabPage.top);
        pTabPage->SetViewSize(RECT_WIDTH(rcTabPage), RECT_HEIGHT(rcTabPage));
        AddView(pTabPage);

        m_pTabHeader->SetTabData(nIndex, (LONG_PTR)pTabPage);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabView::DeleteTab(int nIndex)
{
    LONG_PTR lpData = m_pTabHeader->GetTabData(nIndex);

    // Delete the tab page.
    BOOL retVal = m_pTabHeader->DeleteTab(nIndex);
    if ( retVal )
    {
        SdkViewLayout *pTabPage = static_cast<SdkViewLayout*>((LPVOID)lpData);
        retVal = RemoveChild(pTabPage);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabView::AddTabView(int nIndex, SdkViewElement *pTabView)
{
    LONG_PTR lpData = m_pTabHeader->GetTabData(nIndex);
    BOOL retVal = (0 != lpData);

    if ( retVal )
    {
        SdkViewLayout *pTabPage = static_cast<SdkViewLayout*>((LPVOID)lpData);
        retVal = pTabPage->AddView(pTabView);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabView::DeleteTabView(int nIndex, SdkViewElement *pTabView)
{
    LONG_PTR lpData = m_pTabHeader->GetTabData(nIndex);

    if ( 0 != lpData )
    {
        SdkViewLayout *pTabPage = static_cast<SdkViewLayout*>((LPVOID)lpData);
        pTabPage->RemoveChild(pTabView);
    }

    return (0 != lpData);
}

//////////////////////////////////////////////////////////////////////////

void SdkTabView::SetSelTab(int nIndex)
{
    m_pTabHeader->SetSelTab(nIndex);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabView::SetTabText(int nIndex, IN LPCTSTR lpText)
{
    return m_pTabHeader->SetTabText(nIndex, lpText);
}

//////////////////////////////////////////////////////////////////////////

void SdkTabView::SetTabWidth(int nTabWidth)
{
    m_pTabHeader->SetTabWidth(nTabWidth);
}

//////////////////////////////////////////////////////////////////////////

void SdkTabView::SetAddTabVisible(BOOL fVisible)
{
    m_pTabHeader->SetAddTabVisible(fVisible);
}

//////////////////////////////////////////////////////////////////////////

void SdkTabView::SetTabDockDir(TAB_DOCK_DIR eDockDir)
{
    if ( m_pInternalData->m_eDockDir != eDockDir )
    {
        m_pInternalData->m_eDockDir = eDockDir;
        // Re-layout the tab header according to the dock direction.
        SetTabHeaderLayout(GetWidth(), GetHeight());
        RequestLayout();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkTabView::SetTabViewEventHandler(ITabViewEventHandler *pEventHandler)
{
    m_pInternalData->m_pEventHalder = pEventHandler;
}

//////////////////////////////////////////////////////////////////////////

int SdkTabView::GetSelTab() const
{
    return m_pTabHeader->GetSelTab();
}

//////////////////////////////////////////////////////////////////////////

int SdkTabView::GetTabCount()
{
    return m_pTabHeader->GetTabCount();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabView::GetTabText(int nIndex, LPTSTR lpTabText)
{
    BOOL fRetVal = (NULL != lpTabText);

    if ( fRetVal )
    {
        TAB_INFO tabInfo = { 0 };
        tabInfo.lpszText = lpTabText;
        fRetVal = m_pTabHeader->GetTabInfo(nIndex, &tabInfo);
    }

    return fRetVal;
}

//////////////////////////////////////////////////////////////////////////

SdkViewLayout* SdkTabView::GetTabPage(int nIndex)
{
    LONG_PTR lpData = m_pTabHeader->GetTabData(nIndex);

    if ( 0 != lpData )
    {
        return static_cast<SdkViewLayout*>((LPVOID)lpData);
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////

D2D1_RECT_F SdkTabView::SetTabHeaderLayout(FLOAT width, FLOAT height)
{
    // The view rectangle.
    D2D1_RECT_F rcTabPage = { 0, 0, width, height };

    switch (m_pInternalData->m_eDockDir)
    {
    case TAB_DOCK_TOP:
        {
            m_pTabHeader->SetViewPos(0, 0);
            m_pTabHeader->SetViewSize(width, TAB_HEADER_HEIGHT);
            m_pTabHeader->Rotate(0);
            rcTabPage.top += TAB_HEADER_HEIGHT - 1;
        }
        break;

    case TAB_DOCK_RIGHT:
        {
            m_pTabHeader->SetViewPos(width - 1, 0);
            m_pTabHeader->SetViewSize(height, TAB_HEADER_HEIGHT);
            m_pTabHeader->Rotate(90);
            rcTabPage.right -= TAB_HEADER_HEIGHT - 1;
        }
        break;

    case TAB_DOCK_LEFT:
        {
            m_pTabHeader->SetViewPos(0, height - 1);
            m_pTabHeader->SetViewSize(height, TAB_HEADER_HEIGHT);
            m_pTabHeader->Rotate(-90);
            rcTabPage.left += TAB_HEADER_HEIGHT - 1;
        }
        break;

    case TAB_DOCK_BOTTOM:
        {
            POINTF pt = { width / 2, height / 2 };
            m_pTabHeader->SetViewSize(width, TAB_HEADER_HEIGHT);
            m_pTabHeader->Rotate(180, pt);
            rcTabPage.bottom -= TAB_HEADER_HEIGHT - 2;
        }
        break;
    }

    m_pInternalData->m_rcTabPage = rcTabPage;

    return rcTabPage;
}

//////////////////////////////////////////////////////////////////////////

void SdkTabView::OnDrawItem(ID2D1RenderTarget *pRenderTarget)
{
    D2D1_RECT_F rcTabPage = m_pInternalData->m_rcTabPage;
    D2D1_RECT_F rc = { 0 };
    GetAbsoluteRect(rc);
    D2DRectUtility::OffsetD2DRectF(rcTabPage, rc.left, rc.top);

    SdkD2DTheme *pD2DTheme = SdkD2DTheme::GetD2DThemeInstance(); 
    pD2DTheme->OnDrawButton(this, pRenderTarget, m_pInternalData->m_pBkImage, rcTabPage);
}

//////////////////////////////////////////////////////////////////////////

void SdkTabView::OnSelectedChanged(SdkTabHeader *pTabHeader, int nOldIndex, int nCurIndex)
{
    SdkViewLayout *pOldTabPage = GetTabPage(nOldIndex);
    SdkViewLayout *pNewTabPage = GetTabPage(nCurIndex);

    if ( NULL != pOldTabPage )
    {
        pOldTabPage->SetVisible(FALSE);
    }

    if ( NULL != pNewTabPage )
    {
        pNewTabPage->SetVisible(TRUE);
    }

    if ( NULL != m_pInternalData->m_pEventHalder )
    {
        m_pInternalData->m_pEventHalder->OnSelectedChanged(this, nOldIndex, nCurIndex);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabView::OnInsertTab(SdkTabHeader *pTabHeader)
{
    if ( NULL != m_pInternalData->m_pEventHalder )
    {
        m_pInternalData->m_pEventHalder->OnInsertTab(this);
    }

    // Insert a tab as the last tab.
    return InsertTab(pTabHeader->GetTabCount(), _T(""));
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkTabView::OnDeleteTab(SdkTabHeader *pTabHeader, int nDelIndex)
{
    UNREFERENCED_PARAMETER(pTabHeader);
    UNREFERENCED_PARAMETER(nDelIndex);

    if ( NULL != m_pInternalData->m_pEventHalder )
    {
        m_pInternalData->m_pEventHalder->OnDeleteTab(this, nDelIndex);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkTabView::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    if ( fChanged )
    {
        // The page size.
        D2D1_RECT_F rcTabPage = SetTabHeaderLayout(width, height);
        FLOAT fChildW = RECT_WIDTH(rcTabPage);
        FLOAT fChildH = RECT_HEIGHT(rcTabPage);
        int nChildCount = GetChildCount();

        for (int i = 0; i < nChildCount; ++i)
        {
            SdkViewElement *pChild = NULL;
            GetChildAt(i, &pChild);

            if ( pChild == m_pTabHeader )
            {
                continue;
            }

            pChild->SetViewPos(rcTabPage.left, rcTabPage.top);
            pChild->SetViewSize(fChildW, fChildH);
        }
    }

    SdkViewLayout::OnLayout(fChanged, left, top, width, height);
}
