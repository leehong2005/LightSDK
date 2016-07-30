/*!
* @file SdkSlideLayout.cpp
* 
* @brief This file defines class SdkSlideLayout, which supports drag to slide and sliding animation after dragging.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/28
*/

#include "stdafx.h"
#include "SdkSlideLayout.h"

USING_NAMESPACE_VIEWS

SdkSlideLayout::SdkSlideLayout() : m_uSlidingMaxWidth(200),
                                   m_uSlidingMaxHeight(200),
                                   m_pScrollBar(new SdkScrollBar()),
                                   m_pSlideBase(new SdkSlideBase())
{
    m_pSlideBase->SetOffsetChangedHandler(this);
    SdkViewLayout::AddView(m_pSlideBase);
    SdkViewLayout::AddView(m_pScrollBar);

    SetSlideDirection(SLIDEDIRECTIOIN_VERTICAL);
    AddFlag(VIEW_STATE_ALWAYSPAINTVIEW);
}

//////////////////////////////////////////////////////////////////////////

SdkSlideLayout::~SdkSlideLayout()
{
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkSlideLayout::AddView(IN const SdkViewElement *pChild)
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->AddView(pChild);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkSlideLayout::AddView(const SdkViewElement *pChild, UINT uIndex)
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->AddView(pChild, uIndex);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkSlideLayout::GetChildAt(UINT index, OUT SdkViewElement **ppChild)
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->GetChildAt(index, ppChild);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

int SdkSlideLayout::GetChildCount()
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->GetChildCount();
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkSlideLayout::RemoveChildAt(UINT index)
{
    if (NULL != m_pSlideBase)
    {
        CancelGetViewDataFromAdapter();
        SdkBaseAdapter *pAdapter = SdkAdapterView::GetAdapter();
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

BOOL SdkSlideLayout::RemoveAllChildren(BOOL isClearCache)
{
    if (NULL != m_pSlideBase)
    {
        // Cancel to get view data from adapter.
        CancelGetViewDataFromAdapter();
        SdkBaseAdapter *pAdapter = SdkAdapterView::GetAdapter();
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

void SdkSlideLayout::SetSlideDirection(SLIDEDIRECTIOIN slideDirection)
{
    m_pSlideBase->SetSlideDirection(slideDirection);

    switch (slideDirection)
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        //m_pScrollBar->SetOrientation(SCROLLBAR_ORIENTATION_HORIZONTAL);
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        //m_pScrollBar->SetOrientation(SCROLLBAR_ORIENTATION_VERTICAL);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

SLIDEDIRECTIOIN SdkSlideLayout::GetSlideDirection()
{
    return m_pSlideBase->GetSlideDirection();
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::SetSlideEnable(BOOL isSlideEnable)
{
    if (NULL != m_pSlideBase)
    {
        m_pSlideBase->SetSlideEnable(isSlideEnable);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkSlideLayout::IsSlideEnable()
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->IsSlideEnable();
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::SetSlideAnimationEnable(BOOL isEnable)
{
    if (NULL != m_pSlideBase)
    {
        m_pSlideBase->SetSlideAnimationEnable(isEnable);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkSlideLayout::IsSlideAnimationEnable()
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->IsSlideAnimationEnable();
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkSlideLayout::GetSlideOffset()
{
    if (NULL != m_pSlideBase)
    {
        return m_pSlideBase->GetSlideOffset();
    }

    return 0.0f;
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::SetSlideStep(FLOAT fStep)
{
    if (NULL != m_pSlideBase)
    {
        m_pSlideBase->SetSlideStep(fStep);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::SetHScrollBarVisible(BOOL isVisible)
{
    if ( NULL != m_pScrollBar )
    {
        m_pScrollBar->ShowView(isVisible);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::SetVScrollBarVisible(BOOL isVisible)
{
    if ( NULL != m_pScrollBar )
    {
        m_pScrollBar->ShowView(isVisible);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::ResetOffset(FLOAT offset, BOOL isCreateView)
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

SdkSlideBase* SdkSlideLayout::GetSlideBase() const
{
    return m_pSlideBase;
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::SetSlideRange(UINT uMaxWidth, UINT uMaxHeight)
{
    m_uSlidingMaxWidth  = uMaxWidth;
    m_uSlidingMaxHeight = uMaxHeight;
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::OnOffsetChanged(SdkSlideBase *pView, FLOAT offsetX, FLOAT offsetY)
{
    UNREFERENCED_PARAMETER(pView);

    switch (GetSlideDirection())
    {
    case SLIDEDIRECTIOIN_HORIZONTAL:
        CreateViewFromAdapter();
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        CreateViewFromAdapter();
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::OnFinishMoving(SdkSlideBase *pView)
{
    UNREFERENCED_PARAMETER(pView);

    // Get the data associated with created view.
    GetViewDataFromAdapter();
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::OnBeginMoving(SdkSlideBase *pView)
{
    UNREFERENCED_PARAMETER(pView);

    // Cancel to get data associated with created views.
    CancelGetViewDataFromAdapter();
}

//////////////////////////////////////////////////////////////////////////

void SdkSlideLayout::OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
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
        break;

    case SLIDEDIRECTIOIN_VERTICAL:
        break;
    }
}
