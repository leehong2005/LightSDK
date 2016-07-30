/*!
* @file SdkAdapterView.cpp
* 
* @brief This file defines class SdkAdapterView, An SdkAdapterView is a view whose children are determined by an Adapter.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/15
*/

#include "stdafx.h"
#include "SdkAdapterView.h"
#include "SdkWICAnimatedGif.h"
#include "SdkWICImageHelper.h"

USING_NAMESPACE_VIEWS

SdkAdapterView::SdkAdapterView() : m_pBaseAdapter(NULL),
                             m_isFirstGetView(TRUE),
                             m_hasCancelGetViewData(TRUE),
                             m_nStartIndex(0),
                             m_nEndIndex(0),
                             m_uThreadHandle(NULL)
{
}

//////////////////////////////////////////////////////////////////////////

SdkAdapterView::~SdkAdapterView()
{
}

//////////////////////////////////////////////////////////////////////////

void SdkAdapterView::SetAdapter(SdkBaseAdapter *pAdapter)
{
    m_pBaseAdapter = pAdapter;
    if (NULL != m_pBaseAdapter)
    {
        m_pBaseAdapter->SetDataSetObserver(this);
    }
}

//////////////////////////////////////////////////////////////////////////

SdkBaseAdapter* SdkAdapterView::GetAdapter() const
{
    return m_pBaseAdapter;
}

//////////////////////////////////////////////////////////////////////////

void SdkAdapterView::OnPaint()
{
    if (m_isFirstGetView)
    {
        CreateViewFromAdapter();
        GetViewDataFromAdapter();
        m_isFirstGetView = FALSE;
    }

    SdkViewLayout::OnPaint();
}

//////////////////////////////////////////////////////////////////////////

void SdkAdapterView::CreateViewFromAdapter()
{
    SdkBaseAdapter *pAdapter = GetAdapter();
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

void SdkAdapterView::GetViewDataFromAdapter()
{
    m_hasCancelGetViewData = TRUE;
    WaitForSingleObject(m_uThreadHandle, 5000);
    unsigned int dwThreadId = 0;
    m_uThreadHandle = (HANDLE)_beginthreadex(NULL, 0, SdkAdapterView::GetViewDataThreadProc, (LPVOID)this, 0, &dwThreadId);
}

//////////////////////////////////////////////////////////////////////////

void SdkAdapterView::CancelGetViewDataFromAdapter()
{
    m_hasCancelGetViewData = TRUE;
    WaitForSingleObject(m_uThreadHandle, 5000);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkAdapterView::HasCancelGetViewData()
{
    return m_hasCancelGetViewData;
}

//////////////////////////////////////////////////////////////////////////

void SdkAdapterView::CalcChildViewIndex(INT32 *pStartIndex, INT32 *pEndIndex)
{
    UNREFERENCED_PARAMETER(pStartIndex);
    UNREFERENCED_PARAMETER(pEndIndex);
}

//////////////////////////////////////////////////////////////////////////

void SdkAdapterView::VirtualizeChildView(INT32 nStartIndex, INT32 nEndIndex)
{
    SdkBaseAdapter *pAdapter = GetAdapter();
    if ((NULL == pAdapter) || pAdapter->IsEmpty())
    {
        return;
    }

    BOOL isNeedLayout = FALSE;
    int nCount = pAdapter->GetCount();

    // Get views from adapter from specified start index and end index.
    for (int i = nStartIndex; i < nEndIndex && i < nCount; ++i)
    {
        SdkViewElement *pConvertView = NULL;
        GetChildAt(i, &pConvertView);
        SdkViewElement *pChild = pAdapter->GetView(i, pConvertView, NULL);
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

BOOL SdkAdapterView::SetViewAssocData(INT32 nStartIndex, INT32 nEndIndex)
{
    SdkBaseAdapter *pAdapter = GetAdapter();
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

        SdkViewElement *pChild = NULL;
        if (GetChildAt(i, &pChild) && (NULL != pChild))
        {
            //if (pChild->IsClearAssocData())
            {
                Sleep(100);

                pAdapter->GetViewData(i, pChild);
                pChild->SetVisible(TRUE);
                //pChild->SetClearAssocData(FALSE);
                ForceInvalidate();
            }
        }
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SdkAdapterView::ClearViewAssocData()
{
    SdkViewElement *pChild = NULL;

    for (int i = 0; i < m_nStartIndex - 5; ++i)
    {
        if (GetChildAt(i, &pChild) && (NULL != pChild))
        {
            pChild->ClearAssocData();
            //pChild->SetClearAssocData(TRUE);
            pChild->SetVisible(FALSE);
        }
    }

    int nChildCount = GetChildCount();
    for (int i = m_nEndIndex + 5; i < nChildCount; ++i)
    {
        if (GetChildAt(i, &pChild) && (NULL != pChild))
        {
            pChild->ClearAssocData();
            //pChild->SetClearAssocData(TRUE);
            pChild->SetVisible(FALSE);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkAdapterView::OnDataChanged()
{
    m_nStartIndex = 0;
    m_nEndIndex   = 0;
    CreateViewFromAdapter();
    GetViewDataFromAdapter();
}

//////////////////////////////////////////////////////////////////////////

unsigned int WINAPI SdkAdapterView::GetViewDataThreadProc(LPVOID lpParameter)
{
    SdkAdapterView *pThis = static_cast<SdkAdapterView*>(lpParameter);
    if (NULL != pThis)
    {
        CoInitialize(NULL);
        SdkWICImageHelper::WICInitialize();
        SdkWICAnimatedGif::WICInitialize();

        INT32 nStartIndex = pThis->m_nStartIndex;
        INT32 nEndIndex = pThis->m_nEndIndex;
        pThis->SetViewAssocData(nStartIndex, nEndIndex);
        pThis->m_uThreadHandle = NULL;

        SdkWICAnimatedGif::WICUninitialize();
        SdkWICImageHelper::WICUninitialize();
        CoUninitialize();
    }

    return 0;
}
