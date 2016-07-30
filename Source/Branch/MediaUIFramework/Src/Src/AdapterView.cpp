/*!
* @file AdapterView.cpp
* 
* @brief This file defines class AdapterView, An AdapterView is a view whose children are determined by an Adapter.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/15
*/

#include "AdapterView.h"
#include "WICAnimatedGif.h"
#include "WICImageHelper.h"

AdapterView::AdapterView() : m_pBaseAdapter(NULL),
                             m_isFirstGetView(TRUE),
                             m_hasCancelGetViewData(TRUE),
                             m_nStartIndex(0),
                             m_nEndIndex(0),
                             m_uThreadHandle(NULL)
{
}

//////////////////////////////////////////////////////////////////////////

AdapterView::~AdapterView()
{
}

//////////////////////////////////////////////////////////////////////////

void AdapterView::SetAdapter(BaseAdapter *pAdapter)
{
    m_pBaseAdapter = pAdapter;
    if (NULL != m_pBaseAdapter)
    {
        m_pBaseAdapter->SetDataSetObserver(this);
    }
}

//////////////////////////////////////////////////////////////////////////

BaseAdapter* AdapterView::GetAdapter() const
{
    return m_pBaseAdapter;
}

//////////////////////////////////////////////////////////////////////////

void AdapterView::OnPaint()
{
    if (m_isFirstGetView)
    {
        CreateViewFromAdapter();
        GetViewDataFromAdapter();
        m_isFirstGetView = FALSE;
    }

    ViewLayout::OnPaint();
}

//////////////////////////////////////////////////////////////////////////

void AdapterView::CreateViewFromAdapter()
{
    BaseAdapter *pAdapter = GetAdapter();
    if ((NULL == pAdapter) || (0 == pAdapter->GetCount()))
    {
        return;
    }

    INT32 nStartIndex = 0, nEndIndex = 0;
    CalcChildViewIndex(&nStartIndex, &nEndIndex);

    if ((nEndIndex - nStartIndex) > GetChildCount())
    {
        VirtualizeChildView(nStartIndex, nEndIndex);
    }
    else if ( (m_nStartIndex != nStartIndex) || (m_nEndIndex != nEndIndex) )
    {
        VirtualizeChildView(nStartIndex, nEndIndex);
    }

    m_nStartIndex = nStartIndex;
    m_nEndIndex   = nEndIndex;
}

//////////////////////////////////////////////////////////////////////////

void AdapterView::GetViewDataFromAdapter()
{
    m_hasCancelGetViewData = TRUE;
    WaitForSingleObject(m_uThreadHandle, 5000);
    unsigned int dwThreadId = 0;
    m_uThreadHandle = (HANDLE)_beginthreadex(NULL, 0, AdapterView::GetViewDataThreadProc, (LPVOID)this, 0, &dwThreadId);
}

//////////////////////////////////////////////////////////////////////////

void AdapterView::CancelGetViewDataFromAdapter()
{
    m_hasCancelGetViewData = TRUE;
    WaitForSingleObject(m_uThreadHandle, 5000);
}

//////////////////////////////////////////////////////////////////////////

BOOL AdapterView::HasCancelGetViewData()
{
    return m_hasCancelGetViewData;
}

//////////////////////////////////////////////////////////////////////////

void AdapterView::CalcChildViewIndex(INT32 *pStartIndex, INT32 *pEndIndex)
{
    UNREFERENCED_PARAMETER(pStartIndex);
    UNREFERENCED_PARAMETER(pEndIndex);
}

//////////////////////////////////////////////////////////////////////////

void AdapterView::VirtualizeChildView(INT32 nStartIndex, INT32 nEndIndex)
{
    BaseAdapter *pAdapter = GetAdapter();
    if ((NULL == pAdapter) || pAdapter->IsEmpty())
    {
        return;
    }

    BOOL isNeedLayout = FALSE;
    int nCount = pAdapter->GetCount();

    // Get views from adapter from specified start index and end index.
    for (int i = nStartIndex; i < nEndIndex && i < nCount; ++i)
    {
        ViewElement *pConvertView = NULL;
        GetChildAt(i, &pConvertView);
        ViewElement *pChild = pAdapter->GetView(i, pConvertView, NULL);
        if (NULL != pChild)
        {
            pChild->SetVisible(TRUE);
            if (AddView(pChild))
            {
                isNeedLayout = TRUE;
            }
        }
    }

    m_nStartIndex = nStartIndex;
    m_nEndIndex = nEndIndex;

    if (isNeedLayout)
    {
        RequestLayout();
    }

    ClearViewAssocData();
}

//////////////////////////////////////////////////////////////////////////

BOOL AdapterView::SetViewAssocData(INT32 nStartIndex, INT32 nEndIndex)
{
    BaseAdapter *pAdapter = GetAdapter();
    if ((NULL == pAdapter) || (0 == pAdapter->GetCount()))
    {
        return TRUE;
    }

    m_hasCancelGetViewData = FALSE;
    int nCount = pAdapter->GetCount();

    // Get views' associated data from adapter from specified start index and end index.
    for (int i = nStartIndex; i < nEndIndex && i < nCount; ++i)
    {
        if (HasCancelGetViewData())
        {
            return FALSE;
        }

        ViewElement *pChild = NULL;
        if (GetChildAt(i, &pChild) && (NULL != pChild))
        {
            if (pChild->IsClearAssocData())
            {
                Sleep(100);

                pAdapter->GetViewData(i, pChild);
                pChild->SetVisible(TRUE);
                pChild->SetClearAssocData(FALSE);
                ForceInvalidate();
            }
        }
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void AdapterView::ClearViewAssocData()
{
    ViewElement *pChild = NULL;

    for (int i = 0; i < m_nStartIndex - 5; ++i)
    {
        if (GetChildAt(i, &pChild) && (NULL != pChild))
        {
            pChild->ClearAssocData();
            pChild->SetClearAssocData(TRUE);
            pChild->SetVisible(FALSE);
        }
    }

    int nChildCount = GetChildCount();
    for (int i = m_nEndIndex + 5; i < nChildCount; ++i)
    {
        if (GetChildAt(i, &pChild) && (NULL != pChild))
        {
            pChild->ClearAssocData();
            pChild->SetClearAssocData(TRUE);
            pChild->SetVisible(FALSE);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void AdapterView::OnDataChanged()
{
    m_nStartIndex = 0;
    m_nEndIndex   = 0;
    CreateViewFromAdapter();
    GetViewDataFromAdapter();
}

//////////////////////////////////////////////////////////////////////////

unsigned int WINAPI AdapterView::GetViewDataThreadProc(LPVOID lpParameter)
{
    AdapterView *pThis = static_cast<AdapterView*>(lpParameter);
    if (NULL != pThis)
    {
        CoInitialize(NULL);
        WICImageHelper::WICInitialize();
        WICAnimatedGif::WICInitialize();

        INT32 nStartIndex = pThis->m_nStartIndex;
        INT32 nEndIndex = pThis->m_nEndIndex;
        pThis->SetViewAssocData(nStartIndex, nEndIndex);
        pThis->m_uThreadHandle = NULL;

        WICAnimatedGif::WICUninitialize();
        WICImageHelper::WICUninitialize();
        CoUninitialize();
    }

    return 0;
}
