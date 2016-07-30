/*!
* @file SlideLayout.cpp
* 
* @brief This file defines class SlideLayout, which supports drag to slide and sliding animation after dragging.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/28
*/

#include "SlideLayout.h"

SlideLayout::SlideLayout() : m_uSlidingMaxWidth(200),
                             m_uSlidingMaxHeight(200),
                             m_pScrollBar(new ScrollBar()),
                             m_pSlideBase(new SlideBase())
{
    m_pSlideBase->SetOffsetChangedHandler(this);
    ViewLayout::AddView(m_pSlideBase);
    ViewLayout::AddView(m_pScrollBar);

    m_pScrollBar->SetViewStyle(SCROLLBAR_VIEWSTYLE_ROUNDED);
    SetSlideDirection(SLIDEDIRECTIOIN_VERTICAL);
    AddFlag(VIEW_STATE_ALWAYSPAINTVIEW);
}

//////////////////////////////////////////////////////////////////////////

SlideLayout::~SlideLayout()
{
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideLayout::AddView(IN const ViewElement *pChild)
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->AddView(pChild);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideLayout::AddView(const ViewElement *pChild, UINT uIndex)
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->AddView(pChild, uIndex);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideLayout::GetChildAt(UINT index, OUT ViewElement **ppChild)
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->GetChildAt(index, ppChild);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

int SlideLayout::GetChildCount()
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->GetChildCount();
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideLayout::RemoveChildAt(UINT index)
{
    if (NULL != m_pSlideBase)
    {
        CancelGetViewDataFromAdapter();
        BaseAdapter *pAdapter = AdapterView::GetAdapter();
        if (NULL != pAdapter)
        {
            pAdapter->DeleteItem(index);
        }

        BOOL isSucceed = m_pSlideBase->RemoveChildAt(index);
        if (isSucceed)
        {
            CreateViewFromAdapter();
            GetViewDataFromAdapter();
        }
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideLayout::RemoveAllChildren(BOOL isClearCache)
{
    if (NULL != m_pSlideBase)
    {
        // Cancel to get view data from adapter.
        CancelGetViewDataFromAdapter();
        BaseAdapter *pAdapter = AdapterView::GetAdapter();
        if (NULL != pAdapter)
        {
            pAdapter->DeleteAll();
        }

        BOOL isSucceed =  m_pSlideBase->RemoveAllChildren(isClearCache);
        if (isSucceed)
        {
            ResetOffset(0, FALSE);
        }

        return isSucceed;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::SetSlideDirection(SLIDEDIRECTIOIN slideDirection)
{
    m_pSlideBase->SetSlideDirection(slideDirection);

    switch (slideDirection)
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        m_pScrollBar->SetOrientation(SCROLLBAR_ORIENTATION_HORIZONTAL);
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        m_pScrollBar->SetOrientation(SCROLLBAR_ORIENTATION_VERTICAL);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

SLIDEDIRECTIOIN SlideLayout::GetSlideDirection()
{
    return m_pSlideBase->GetSlideDirection();
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::SetSlideEnable(BOOL isSlideEnable)
{
    if (NULL != m_pSlideBase)
    {
        m_pSlideBase->SetSlideEnable(isSlideEnable);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideLayout::IsSlideEnable()
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->IsSlideEnable();
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::SetSlideAnimationEnable(BOOL isEnable)
{
    if (NULL != m_pSlideBase)
    {
        m_pSlideBase->SetSlideAnimationEnable(isEnable);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SlideLayout::IsSlideAnimationEnable()
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->IsSlideAnimationEnable();
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

FLOAT SlideLayout::GetSlideOffset()
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->GetSlideOffset();
    }

    return 0.0f;
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::SetSlideStep(FLOAT fStep)
{
    if (NULL != m_pSlideBase)
    {
        m_pSlideBase->SetSlideStep(fStep);
    }
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::SetHScrollBarVisible(BOOL isVisible)
{
    ViewLayout::SetHScrollBarVisible(isVisible);

    if (NULL != m_pScrollBar)
    {
        m_pScrollBar->ShowView(isVisible);
    }
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::SetVScrollBarVisible(BOOL isVisible)
{
    ViewLayout::SetVScrollBarVisible(isVisible);

    if (NULL != m_pScrollBar)
    {
        m_pScrollBar->ShowView(isVisible);
    }
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::ResetOffset(FLOAT offset, BOOL isCreateView)
{
    if (NULL != m_pSlideBase)
    {
        m_pSlideBase->SetOffsetChangedHandler(NULL);
        m_pSlideBase->OffsetViewLayout(offset, offset);
        m_pSlideBase->SetOffsetChangedHandler(this);
        if (isCreateView)
        {
            CreateViewFromAdapter();
        }
    }
}

//////////////////////////////////////////////////////////////////////////

SlideBase* SlideLayout::GetSlideBase() const
{
    return m_pSlideBase;
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::SetSlideRange(UINT uMaxWidth, UINT uMaxHeight)
{
    m_uSlidingMaxWidth  = uMaxWidth;
    m_uSlidingMaxHeight = uMaxHeight;
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::OnOffsetChanged(SlideBase *pView, FLOAT offsetX, FLOAT offsetY)
{
    UNREFERENCED_PARAMETER(pView);

    switch (GetSlideDirection())
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        m_pScrollBar->SetThumbPos(-offsetX);
        CreateViewFromAdapter();
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        m_pScrollBar->SetThumbPos(-offsetY);
        CreateViewFromAdapter();
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::OnFinishMoving(SlideBase *pView)
{
    UNREFERENCED_PARAMETER(pView);

    // Get the data associated with created view.
    GetViewDataFromAdapter();
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::OnBeginMoving(SlideBase *pView)
{
    UNREFERENCED_PARAMETER(pView);

    // Cancel to get data associated with created views.
    CancelGetViewDataFromAdapter();
}

//////////////////////////////////////////////////////////////////////////

void SlideLayout::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
    UNREFERENCED_PARAMETER(fChanged);
    UNREFERENCED_PARAMETER(left);
    UNREFERENCED_PARAMETER(top);

    FLOAT slideWidth  = ((FLOAT)m_uSlidingMaxWidth < width) ? width : (FLOAT)m_uSlidingMaxWidth;
    FLOAT slideHeight = ((FLOAT)m_uSlidingMaxHeight < height) ? height : (FLOAT)m_uSlidingMaxHeight;

    m_pSlideBase->SetLayoutInfo(m_pSlideBase->GetLeft(), m_pSlideBase->GetTop(), slideWidth, slideHeight);

    switch (GetSlideDirection())
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        m_pScrollBar->SetLayoutInfo(1, height - m_pScrollBar->GetHeight(), width - 2, 10);
        m_pScrollBar->SetScrollPage(slideWidth);
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        m_pScrollBar->SetLayoutInfo(width - m_pScrollBar->GetWidth(), 1, 10, height - 2);
        m_pScrollBar->SetScrollPage(slideHeight);
        break;
    }
}
