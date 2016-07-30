/*!
* @file IViewEvent.cpp
* 
* @brief This file defines events interfaces for SdkViewElement class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#include "stdafx.h"
#include "SdkViewLayout.h"
#include "D2DRectUtility.h"

USING_NAMESPACE_VIEWS

//////////////////////////////////////////////////////////////////////////

SdkViewLayout::SdkViewLayout()
{
    SetClassName(CLASSNAME_VIEWLAYOUT);
}

//////////////////////////////////////////////////////////////////////////

SdkViewLayout::~SdkViewLayout()
{
    RemoveAllChildren(TRUE);
}

//////////////////////////////////////////////////////////////////////////

void SdkViewLayout::OnPaint()
{
    SdkViewElement::OnPaint();

    if ( IsVisible() )
    {
        OnDrawChildren();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewLayout::OnDrawChildren()
{
    ID2D1RenderTarget *pRenderTarget = NULL;
    m_pWindow->GetD2DDevices()->GetRenderTarget(&pRenderTarget);

    if ( NULL == pRenderTarget )
    {
        return;
    }

    D2D1_RECT_F layoutRc = GetDrawingRect();
    D2D1_RECT_F intersectRc = { 0.0f };
    BOOL isAlwaysPaintView = (VIEW_STATE_ALWAYSPAINTVIEW == (GetState() & VIEW_STATE_ALWAYSPAINTVIEW));

    // Push the aligned clip to clip the overflow views.
    pRenderTarget->PushAxisAlignedClip(layoutRc, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

    for each(SdkViewElement* pChild in m_vctChildren)
    {
        if ( !pChild->IsVisible() )
        {
            continue;
        }

        D2D1_RECT_F childRc = pChild->GetDrawingRect();
        // Clip the view when part of view is out of its parent view.
        BOOL isIntersected = D2DRectUtility::IntersectD2DRectF(intersectRc, layoutRc, childRc);
        // If the child is out of the view's rectangle, not draw it.
        if ( !isIntersected && !isAlwaysPaintView )
        {
            continue;
        }

        // Draw child view.
        pChild->OnPaint();
    }

    pRenderTarget->PopAxisAlignedClip();
    SAFE_RELEASE(pRenderTarget);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewLayout::AddView(const SdkViewElement *pChild)
{
    return AddView(pChild, GetChildCount());
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewLayout::AddView(const SdkViewElement *pChild, UINT uIndex)
{
    // The child cannot be equal the current object.
    if ( NULL == pChild || pChild == this )
    {
        return FALSE;
    }

    SdkViewElement *child = const_cast<SdkViewElement*>(pChild);

    // The child is already existed in this view layout.
    if ( GetIndexOfChild(child) >= 0 )
    {
        return FALSE;
    }

    // The child already has parent layout, then remove it from its parent.
    SdkViewLayout *pParentLayout = child->GetParent();
    if ( NULL != pParentLayout )
    {
        // Remove the child and does not push it to cache vector, because this removed child
        // view will be added to another layout, which manages the memory of the removed view.
        int nIndex = pParentLayout->GetIndexOfChild(child);
        if ( nIndex >= 0 && nIndex < (int)pParentLayout->GetChildCount() )
        {
            pParentLayout->ClearEventViews(child);
            pParentLayout->m_vctChildren.erase(pParentLayout->m_vctChildren.begin() + nIndex);
            pParentLayout->UpdateChildrenState();
        }
    }

    INT32 nCount = (INT32)m_vctChildren.size();
    uIndex = (uIndex > (UINT)nCount) ? (UINT)nCount : uIndex;
    child->SetParent(this);
    child->SetWindow(m_pWindow);

    if ( (UINT)nCount == uIndex )
    {
        m_vctChildren.push_back(child);
    }
    else
    {
        m_vctChildren.insert(m_vctChildren.begin() + uIndex, child);
    }

    // Update children's state, such as front state.
    UpdateChildrenState();

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewLayout::BringViewToFront(SdkViewElement *pChild)
{
    // Bring the child to front.
    if ( NULL == pChild || pChild == this )
    {
        return FALSE;
    }

    // The view is already front, so do nothing.
    BOOL isFront = (VIEW_STATE_FRONT == (pChild->GetState() & VIEW_STATE_FRONT));
    if ( isFront )
    {
        return FALSE;
    }

    INT32 findIndex = GetIndexOfChild(pChild);
    if ( findIndex >= 0 && (findIndex < ((INT32)m_vctChildren.size() - 1)) )
    {
        // First erase the item in vector at specified position.
        m_vctChildren.erase(m_vctChildren.begin() + findIndex);
        // Add current child to end of vector.
        m_vctChildren.push_back(pChild);

        UpdateChildrenState();
        Invalidate();
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

int SdkViewLayout::GetChildCount()
{
    return (int)m_vctChildren.size();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewLayout::GetChildAt(UINT index, SdkViewElement **ppChild)
{
    int childCount = GetChildCount();

    if ( index >= 0 && index < (UINT)childCount && NULL != ppChild )
    {
        (*ppChild) = m_vctChildren.at(index);
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewLayout::GetChildById(int id, SdkViewElement **ppChild)
{
    for (vector<SdkViewElement*>::iterator itor = m_vctChildren.begin();
         itor != m_vctChildren.end();
         ++itor)
    {
        SdkViewElement *pChild = static_cast<SdkViewElement*>(*itor);
        if ( id == pChild->GetId() )
        {
            (*ppChild) = pChild;
            return TRUE;
        }
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

INT32 SdkViewLayout::GetIndexOfChild(IN SdkViewElement * pChild)
{
    INT32 nSize = (int)m_vctChildren.size();
    INT32 nIndex = -1;

    for (INT32 i = 0; i < nSize; ++i)
    {
        if ( pChild == m_vctChildren[i] )
        {
            nIndex = i;
            break;
        }
    }

    return nIndex;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewLayout::RemoveAllChildren(BOOL isClearCache)
{
    if ( isClearCache )
    {
        for each (SdkViewElement *pChild in m_vctChildren)
        {
            ClearEventViews(pChild);
            SAFE_DELETE(pChild);
        }

        for each (SdkViewElement *pChild in m_vctRemovedChildren)
        {
            ClearEventViews(pChild);
            SAFE_DELETE(pChild);
        }

        m_vctChildren.clear();
        m_vctRemovedChildren.clear();
    }
    else
    {
        for each (SdkViewElement *pChild in m_vctChildren)
        {
            ClearEventViews(pChild);
            m_vctRemovedChildren.push_back(pChild);
        }
        m_vctChildren.clear();
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewLayout::RemoveChild(SdkViewElement * pChild, BOOL fToCache)
{
    // We should not delete the memory pointed by child to be deleted, because this
    // method may be called in child's OnClick event handler. so we always push deleted
    // child into temporary vector, which will be deleted at destructor function.
    int nIndex = GetIndexOfChild(pChild);
    if ( nIndex >= 0 && nIndex < (int)m_vctChildren.size() )
    {
        ClearEventViews(pChild);

        if ( fToCache )
        {
            m_vctRemovedChildren.push_back(m_vctChildren[nIndex]);
            m_vctChildren.erase(m_vctChildren.begin() + nIndex);
        }
        else
        {
            SAFE_DELETE(m_vctChildren[nIndex]);
            m_vctChildren.erase(m_vctChildren.begin() + nIndex);
        }

        // Update children's state, such as front state.
        UpdateChildrenState();

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewLayout::RemoveChildAt(UINT index)
{
    SdkViewElement *pChild = NULL;
    BOOL retVal = GetChildAt(index, &pChild);

    if ( retVal )
    {
        retVal = RemoveChild(pChild);
    }

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

SdkViewElement* SdkViewLayout::FindEventSource(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( !IsEnable() || !IsVisible() )
    {
        return NULL;
    }

    SdkViewElement *pSource = NULL;
    FLOAT xPos = (FLOAT)GET_X_LPARAM(lParam);
    FLOAT yPos = (FLOAT)GET_Y_LPARAM(lParam);
    BOOL isTouchOn = IsPtInRect(xPos, yPos);

    if ( isTouchOn )
    {
        // Find the event source from end to begin in the vector.
        for (vector<SdkViewElement*>::reverse_iterator itor = m_vctChildren.rbegin();
             itor != m_vctChildren.rend();
             ++itor)
        {
            SdkViewElement *pChild = (SdkViewElement*)(*itor);
            pSource = pChild->FindEventSource(message, wParam, lParam);
            if ( NULL != pSource )
            {
                break;
            }
        }
    }

    if ( NULL == pSource )
    {
        pSource = SdkViewElement::FindEventSource(message, wParam, lParam);
    }

    return pSource;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkViewLayout::PreDispatchMouseEvent(const LPMSG lpMsg, SdkViewElement *pSource)
{
    BOOL handled =  PerformOnMouseEvent(lpMsg, pSource);

    if ( !handled )
    {
        for (vector<SdkViewElement*>::reverse_iterator itor = m_vctChildren.rbegin();
             itor != m_vctChildren.rend(); ++itor)
        {
            SdkViewElement *pChild = (SdkViewElement*)(*itor);
            handled = pChild->PreDispatchMouseEvent(lpMsg, pSource);
            if ( handled )
            {
                break;
            }
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

void SdkViewLayout::ClearCacheView()
{
    int size = (int)m_vctRemovedChildren.size();
    for (int i = 0; i < size; ++i)
    {
        SAFE_DELETE(m_vctRemovedChildren[i]);
    }

    m_vctRemovedChildren.clear();
}

//////////////////////////////////////////////////////////////////////////

void SdkViewLayout::ClearAssocData()
{
    for each (SdkViewElement *pChild in m_vctChildren)
    {
        pChild->ClearAssocData();
    }

    for each (SdkViewElement *pChild in m_vctRemovedChildren)
    {
        pChild->ClearAssocData();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewLayout::UpdateChildrenState()
{
    int nSize = (int)m_vctChildren.size();

    for (int i = 0; i < nSize; ++i)
    {
        // The last child.
        if ( (i + 1) == nSize )
        {
            m_vctChildren.at(i)->AddFlag(VIEW_STATE_FRONT);
        }
        else
        {
            m_vctChildren.at(i)->RemoveFlag(VIEW_STATE_FRONT);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewLayout::ClearEventViews(SdkViewElement *pView)
{
    SdkWindow *pWindow = GetWindow();
    if ( NULL != pWindow )
    {
        SdkViewElement *pTargetView = pWindow->WindowViews.EventTargetView;
        SdkViewElement *pSourceView = pWindow->WindowViews.EventSourceView;

        if ( pTargetView == pView )
        {
            pWindow->WindowViews.EventTargetView = NULL;
        }

        if ( pSourceView == pView )
        {
            pWindow->WindowViews.EventSourceView = NULL;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewLayout::SetWindow(SdkWindow *pWindow)
{
    SdkViewElement::SetWindow(pWindow);

    for (vector<SdkViewElement*>::iterator itor = m_vctChildren.begin();
         itor != m_vctChildren.end();
         ++itor)
    {
        (*itor)->SetWindow(pWindow);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewLayout::OnWindowDestroy(SdkWindow *pWindow)
{
    SdkViewElement::OnWindowDestroy(pWindow);

    int nSize = (int)m_vctChildren.size();
    for (int i = 0; i < nSize; ++i)
    {
        m_vctChildren[i]->OnWindowDestroy(pWindow);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkViewLayout::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    UNREFERENCED_PARAMETER(left);
    UNREFERENCED_PARAMETER(top);
    UNREFERENCED_PARAMETER(width);
    UNREFERENCED_PARAMETER(height);

    for each (SdkViewElement *pChild in m_vctChildren)
    {
        pChild->OnLayout(
            fChanged,
            pChild->GetLeft(),
            pChild->GetTop(),
            pChild->GetWidth(),
            pChild->GetHeight());
    }
}