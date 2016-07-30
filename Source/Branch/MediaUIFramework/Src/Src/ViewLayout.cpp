/*!
* @file IViewEvent.cpp
* 
* @brief This file defines events interfaces for ViewElement class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#include "ViewLayout.h"

//////////////////////////////////////////////////////////////////////////

ViewLayout::ViewLayout() : m_isHScrollbarVisible(FALSE),
                           m_isVScrollbarVisible(FALSE)
{
}

//////////////////////////////////////////////////////////////////////////

ViewLayout::~ViewLayout()
{
    RemoveAllChildren(TRUE);
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::OnPaint()
{
    ViewElement::OnPaint();

    if (IsVisible())
    {
        OnDrawChildren();
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::OnPaintEx(HDC hDC, const LPRECT lpRect)
{
    if ((NULL == hDC) || (NULL == lpRect))
    {
        return;
    }

    D2DDevice *pD2DDevice = (NULL != m_pWindow) ? m_pWindow->GetD2DDevices() : NULL;
    if (NULL == pD2DDevice || !IsVisible())
    {
        return;
    }

    FLOAT left   = GetLeft();
    FLOAT top    = GetTop();
    FLOAT width  = GetWidth();
    FLOAT height = GetHeight();

    RECT bindRc   = { 0 };
    bindRc.right  = (lpRect->right  - lpRect->left);
    bindRc.bottom = (lpRect->bottom - lpRect->top);

    m_layoutInfo.x = (FLOAT)-lpRect->left;
    m_layoutInfo.y = (FLOAT)-lpRect->top;
    m_layoutInfo.width  = (FLOAT)bindRc.right;
    m_layoutInfo.height = (FLOAT)bindRc.bottom;

    pD2DDevice->SetPaintTargetType(DEVICE_TARGET_TYPE_DC);
    pD2DDevice->BeginDraw(hDC, &bindRc);
    ViewLayout *pParent = GetParent(NULL);
    AddFlag(VIEW_STATE_PAINTALLVIEWS);
    SetParent(NULL);
    ViewElement::OnPaint();
    OnDrawChildrenEx(&bindRc);
    SetParent(pParent);
    RemoveFlag(VIEW_STATE_PAINTALLVIEWS);
    pD2DDevice->EndDraw();

    m_layoutInfo.x = left;
    m_layoutInfo.y = top;
    m_layoutInfo.width  = width;
    m_layoutInfo.height = height;
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::OnDrawChildren()
{
    ID2D1RenderTarget *pRenderTarget = NULL;
    m_pWindow->GetD2DDevices()->GetRenderTarget(&pRenderTarget);
    if (NULL == pRenderTarget)
    {
        return;
    }

    D2D1_RECT_F layoutRc = GetDrawingRect();
    D2D1_RECT_F intersectRc = { 0.0f };
    BOOL isAlwaysPaintView = (VIEW_STATE_ALWAYSPAINTVIEW == (GetState() & VIEW_STATE_ALWAYSPAINTVIEW));

    pRenderTarget->PushAxisAlignedClip(layoutRc, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

    for each(ViewElement* pChild in m_vctChildren)
    {
        if (!pChild->IsVisible())
        {
            continue;
        }

        D2D1_RECT_F childRc = pChild->GetDrawingRect();
        // Clip the view when part of view is out of its parent view.
        BOOL isIntersected = D2DRectUtility::IntersectD2DRectF(intersectRc, layoutRc, childRc);
        if (!isIntersected && !isAlwaysPaintView)
        {
            continue;
        }

        pChild->OnPaint();
    }

    pRenderTarget->PopAxisAlignedClip();
    SAFE_RELEASE(pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::OnDrawChildrenEx(const LPRECT lpRect)
{
    ID2D1RenderTarget *pRenderTarget = NULL;
    m_pWindow->GetD2DDevices()->GetRenderTarget(&pRenderTarget);
    if (NULL == pRenderTarget)
    {
        return;
    }

    BOOL isAlwaysPaintView = (VIEW_STATE_ALWAYSPAINTVIEW == (GetState() & VIEW_STATE_ALWAYSPAINTVIEW));
    D2D1_RECT_F layoutRc = RECT_TO_D2DRECT(*lpRect);
    D2D1_RECT_F intersectRc = { 0.0f };

    pRenderTarget->PushAxisAlignedClip(layoutRc, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

    for each(ViewElement* pChild in m_vctChildren)
    {
        if (!pChild->IsVisible())
        {
            continue;
        }

        D2D1_RECT_F childRc = pChild->GetDrawingRect();
        // Clip the view when part of view is out of its parent view.
        BOOL isIntersected = D2DRectUtility::IntersectD2DRectF(intersectRc, layoutRc, childRc);
        if (!isIntersected && !isAlwaysPaintView)
        {
            continue;
        }

        pChild->OnPaint();
    }

    pRenderTarget->PopAxisAlignedClip();
    SAFE_RELEASE(pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::AddView(const ViewElement *pChild)
{
    if (NULL == pChild || pChild == this)
    {
        return FALSE;
    }

    ViewElement *child = const_cast<ViewElement*>(pChild);

    // The child is already existed in this view layout.
    if (GetIndexOfChild(child) >= 0)
    {
        return FALSE;
    }

    // The child already has parent layout, then remove it from its parent.
    ViewLayout *pParentLayout = child->GetParent();
    if (NULL != pParentLayout)
    {
        // Here may lead bug, because remove child dose not delete child's memory.
        pParentLayout->RemoveChild(child);
        //pParentLayout->ClearCacheView();
    }

    INT32 nIndex = (INT32)m_vctChildren.size();
    child->SetParent(this);
    child->SetWindow(m_pWindow);
    child->SetViewIndex(nIndex);
    m_vctChildren.push_back(child);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::AddView(const ViewElement *pChild, UINT uIndex)
{
    if (NULL == pChild)
    {
        return FALSE;
    }

    ViewElement *child = const_cast<ViewElement*>(pChild);

    // The child is already existed in this view layout.
    if (GetIndexOfChild(child) >= 0)
    {
        return FALSE;
    }

    // The child already has parent layout, then remove it from its parent.
    ViewLayout *pParentLayout = child->GetParent();
    if (NULL != pParentLayout)
    {
        // Here may lead bug, because remove child dose not delete child's memory.
        pParentLayout->RemoveChild(child);
        //pParentLayout->ClearCacheView();
    }

    INT32 nCount = (INT32)m_vctChildren.size();
    uIndex = (uIndex > (UINT)nCount) ? (UINT)nCount : uIndex;
    child->SetParent(this);
    child->SetWindow(m_pWindow);

    if ((UINT)nCount == uIndex)
    {
        child->SetViewIndex(nCount);
        m_vctChildren.push_back(child);
    }
    else
    {
        m_vctChildren.insert(m_vctChildren.begin() + uIndex, child);
        ResetChildrenIndex();
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::BringViewToFront(ViewElement *pChild)
{
    if (NULL == pChild)
    {
        return FALSE;
    }

    INT32 findIndex = GetIndexOfChild(const_cast<ViewElement*>(pChild));
    if (findIndex >= 0 && (findIndex < ((INT32)m_vctChildren.size() - 1)))
    {
        // First erase the item in vector at specified position.
        m_vctChildren.erase(m_vctChildren.begin() + findIndex);
        // Add current child to end of vector.
        m_vctChildren.push_back(const_cast<ViewElement*>(pChild));

        ResetChildrenIndex();
        Invalidate();
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

int ViewLayout::GetChildCount()
{
    return (int)m_vctChildren.size();
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::GetChildAt(UINT index, ViewElement **ppChild)
{
    int childCount = GetChildCount();
    if (index >= 0 && index < (UINT)childCount && NULL != ppChild)
    {
        (*ppChild) = m_vctChildren.at(index);
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::GetChildById(int id, ViewElement **ppChild)
{
    for (vector<ViewElement*>::iterator itor = m_vctChildren.begin();
         itor != m_vctChildren.end();
         ++itor)
    {
        if ( id == (static_cast<ViewElement*>(*itor))->GetId() )
        {
            (*ppChild) = static_cast<ViewElement*>(*itor);
            return TRUE;
        }
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

INT32 ViewLayout::GetIndexOfChild(IN ViewElement * pChild)
{
    INT32 nSize = (int)m_vctChildren.size();
    INT32 nIndex = -1;

    for (INT32 i = 0; i < nSize; ++i)
    {
        if (pChild == m_vctChildren[i])
        {
            nIndex = i;
            break;
        }
    }

    return nIndex;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::RemoveAllChildren(BOOL isClearCache)
{
    if (isClearCache)
    {
        for each (ViewElement *pChild in m_vctChildren)
        {
            ClearEventTargetAndSource(pChild);
            SAFE_DELETE(pChild);
        }

        for each (ViewElement *pChild in m_vctRemovedChildren)
        {
            ClearEventTargetAndSource(pChild);
            SAFE_DELETE(pChild);
        }

        m_vctChildren.clear();
        m_vctRemovedChildren.clear();
    }
    else
    {
        for each (ViewElement *pChild in m_vctChildren)
        {
            ClearEventTargetAndSource(pChild);
            m_vctRemovedChildren.push_back(pChild);
        }
        m_vctChildren.clear();
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::RemoveChild(ViewElement * pChild)
{
    // We should not delete the memory pointed by child to be deleted, because this
    // method may be called in child's OnClick event handler. so we always push deleted
    // child into temporary vector, which will be deleted at destructor function.
    int nIndex = GetIndexOfChild(pChild);
    if (nIndex >= 0 && nIndex < (int)m_vctChildren.size())
    {
        ClearEventTargetAndSource(pChild);
        m_vctRemovedChildren.push_back(m_vctChildren[nIndex]);
        m_vctChildren.erase(m_vctChildren.begin() + nIndex);
        ResetChildrenIndex();

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::RemoveChildAt(UINT index)
{
    if (index >= 0 && index < m_vctChildren.size())
    {
        m_vctRemovedChildren.push_back(m_vctChildren[index]);
        m_vctChildren.erase(m_vctChildren.begin() + index);
        ResetChildrenIndex();

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::RemoveChildById(UINT id)
{
    UNREFERENCED_PARAMETER(id);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::RemoveChildren(UINT statr, UINT count)
{
    UNREFERENCED_PARAMETER(statr);
    UNREFERENCED_PARAMETER(count);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::SetPadding(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
{
    UNREFERENCED_PARAMETER(left);
    UNREFERENCED_PARAMETER(top);
    UNREFERENCED_PARAMETER(right);
    UNREFERENCED_PARAMETER(bottom);
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::SetPadding(FLOAT paddingValue)
{
    UNREFERENCED_PARAMETER(paddingValue);
}

//////////////////////////////////////////////////////////////////////////

ViewElement* ViewLayout::FindEventSource(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!IsEnable() || !IsVisible())
    {
        return NULL;
    }

    ViewElement *pSource = NULL;
    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
    BOOL isTouchOn = IsPtInRect(xPos, yPos);
    if (isTouchOn)
    {
        for (vector<ViewElement*>::reverse_iterator itor = m_vctChildren.rbegin();
             itor != m_vctChildren.rend(); ++itor)
        {
            ViewElement *pChild = (ViewElement*)(*itor);
            pSource = pChild->FindEventSource(message, wParam, lParam);
            if (NULL != pSource)
            {
                break;
            }
        }
    }

    if (NULL == pSource)
    {
        pSource = ViewElement::FindEventSource(message, wParam, lParam);
    }

    return pSource;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::DispatchMessageEvent(const LPMSG lpMsg)
{
    BOOL handled = ViewElement::DispatchMessageEvent(lpMsg);
    if (!handled)
    {
        for (vector<ViewElement*>::reverse_iterator itor = m_vctChildren.rbegin();
             itor != m_vctChildren.rend(); ++itor)
        {
            ViewElement *pChild = (ViewElement*)(*itor);
            handled = pChild->DispatchMessageEvent(lpMsg);
            if (handled)
            {
                break;
            }
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::PreDispathMouseEvent(const LPMSG lpMsg, ViewElement *pSource)
{
    BOOL handled =  PerformOnMouseEvent(lpMsg, pSource);
    if (!handled)
    {
        for (vector<ViewElement*>::reverse_iterator itor = m_vctChildren.rbegin();
             itor != m_vctChildren.rend(); ++itor)
        {
            ViewElement *pChild = (ViewElement*)(*itor);
            handled = pChild->PreDispathMouseEvent(lpMsg, pSource);
            if (handled)
            {
                break;
            }
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::PreDispathTouchEvent(const LPMSG lpMsg, ViewElement *pSource)
{
    BOOL handled = PerformOnTouchEvent(lpMsg, pSource);
    if (!handled)
    {
        for (vector<ViewElement*>::reverse_iterator itor = m_vctChildren.rbegin();
             itor != m_vctChildren.rend(); ++itor)
        {
            ViewElement *pChild = (ViewElement*)(*itor);
            handled = pChild->PreDispathTouchEvent(lpMsg, pSource);
            if (handled)
            {
                break;
            }
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::SetHScrollBarVisible(BOOL isVisible)
{
    m_isHScrollbarVisible = isVisible;
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::SetVScrollBarVisible(BOOL isVisible)
{
    m_isVScrollbarVisible = isVisible;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::IsHScrollBarVisible()
{
    return m_isHScrollbarVisible;
}

//////////////////////////////////////////////////////////////////////////

BOOL ViewLayout::IsVScrollBarVisible()
{
    return m_isVScrollbarVisible;
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::ClearCacheView()
{
    int size = (int)m_vctRemovedChildren.size();
    for (int i = 0; i < size; ++i)
    {
        SAFE_DELETE(m_vctRemovedChildren[i]);
    }

    m_vctRemovedChildren.clear();
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::ClearAssocData()
{
    for each (ViewElement *pChild in m_vctChildren)
    {
        pChild->ClearAssocData();
    }

    for each (ViewElement *pChild in m_vctRemovedChildren)
    {
        pChild->ClearAssocData();
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::ClearEventTargetAndSource(ViewElement *pView)
{
    //Window *pWindow = GetWindow();
    //ViewElement *pTargetView = pWindow->GetEventTargetView();
    //ViewElement *pSourceView = pWindow->GetEventSourceView();

    //if (pTargetView == pView)
    //{
    //    pWindow->SetEventTargetView(NULL);
    //}

    //if (pSourceView == pView)
    //{
    //    pWindow->SetEventSourceView(NULL);
    //}
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::ResetChildrenIndex()
{
    int nCount = GetChildCount();
    for (int i = 0; i < nCount; ++i)
    {
        m_vctChildren[i]->SetViewIndex(i);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::SetWindow(Window *pWindow)
{
    ViewElement::SetWindow(pWindow);

    for (vector<ViewElement*>::iterator itor = m_vctChildren.begin();
        itor != m_vctChildren.end();
        ++itor)
    {
        (*itor)->SetWindow(pWindow);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::OnWindowDestroy(Window *pWindow)
{
    ViewElement::OnWindowDestroy(pWindow);

    int nSize = (int)m_vctChildren.size();
    for (int i = 0; i < nSize; ++i)
    {
        m_vctChildren[i]->OnWindowDestroy(pWindow);
    }
}

//////////////////////////////////////////////////////////////////////////

void ViewLayout::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    UNREFERENCED_PARAMETER(left);
    UNREFERENCED_PARAMETER(top);
    UNREFERENCED_PARAMETER(width);
    UNREFERENCED_PARAMETER(height);

    for each (ViewElement *pChild in m_vctChildren)
    {
        pChild->OnLayout(fChanged, pChild->GetLeft(), pChild->GetTop(), pChild->GetWidth(), pChild->GetHeight());
    }
}