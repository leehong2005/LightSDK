/*!
* @file SdkFileSearcher.cpp
* 
* @brief This file defines class SdkFileSearcher using to search file from disk.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#include "stdafx.h"
#include "SdkFileSearcher.h"
#include "SdkCommonHelper.h"
#include "SdkLogger.h"

USING_NAMESPACE_COMMON

SdkFileSearcher::SdkFileSearcher() : m_pSearchFolderItemFactory(NULL),
                                     m_pTempQueryScope(NULL),
                                     m_pTempQueryCondition(NULL),
                                     m_pSearcherNotify(NULL),
                                     m_hasCancel(FALSE),
                                     m_hasStopped(FALSE),
                                     m_uThreadHandle(NULL),
                                     m_nQueryConditionKind(QUERY_KIND_NONE),
                                     m_pQueryScope(new SdkQueryScope()),
                                     m_pQueryCondition(new SdkQueryCondition())
{
}

////////////////////////////////////////////////////////////////////////

SdkFileSearcher::~SdkFileSearcher()
{
    SAFE_RELEASE(m_pSearchFolderItemFactory);
    SAFE_DELETE(m_pQueryScope);
    SAFE_DELETE(m_pQueryCondition);
}

////////////////////////////////////////////////////////////////////////

HRESULT SdkFileSearcher::SetCondition(INT32 queryKind)
{
    HRESULT hr = E_FAIL;

    m_nQueryConditionKind = queryKind;

    if (NULL != m_pQueryCondition)
    {
        ClearSearchCondition();
        hr = m_pQueryCondition->SetCondition(queryKind);
    }

    return hr;
}

////////////////////////////////////////////////////////////////////////

HRESULT SdkFileSearcher::SetCondition(LPCWSTR *ppszQuerys, UINT32 uCount)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pQueryCondition)
    {
        ClearSearchCondition();
        hr = m_pQueryCondition->SetCondition(ppszQuerys, uCount);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkFileSearcher::SetCondition(const SdkQueryCondition *pQueryCondition)
{
    ClearSearchCondition();
    m_pTempQueryCondition = const_cast<SdkQueryCondition*>(pQueryCondition);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

INT32 SdkFileSearcher::GetCodition() const
{
    return m_nQueryConditionKind;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkFileSearcher::SetScope(INT32 scopeType)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pQueryScope)
    {
        ClearSearchScope();
        hr = m_pQueryScope->SetScope(scopeType);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkFileSearcher::SetScope(PCWSTR *pszScopePaths, UINT32 uCount)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pQueryScope)
    {
        ClearSearchScope();
        hr = m_pQueryScope->SetScope(pszScopePaths, uCount);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkFileSearcher::SetScope(const SdkQueryScope *pQueryScope)
{
    ClearSearchScope();
    m_pTempQueryScope = const_cast<SdkQueryScope*>(pQueryScope);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkFileSearcher::Search()
{
    SAFE_RELEASE(m_pSearchFolderItemFactory);
    CoCreateInstance(
        CLSID_SearchFolderItemFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&m_pSearchFolderItemFactory));

    if (NULL == m_pSearchFolderItemFactory)
    {
        return E_FAIL;
    }

    m_hasCancel  = FALSE;
    m_hasStopped = FALSE;
    ClearSearchResult();

    HRESULT hr = E_FAIL;
    SdkQueryScope *pQueryScope = 
        (NULL != m_pTempQueryScope) ? m_pTempQueryScope : m_pQueryScope;

    SdkQueryCondition *pQueryCondition = 
        (NULL != m_pTempQueryCondition) ? m_pTempQueryCondition : m_pQueryCondition;

    IShellItemArray *psiaScope = NULL;
    // +1
    hr = pQueryScope->GetScope(&psiaScope);
    if (SUCCEEDED(hr))
    {
        hr = m_pSearchFolderItemFactory->SetScope(psiaScope);
    }

    ICondition *pCondition = NULL;
    if (SUCCEEDED(hr))
    {
        // +2
        hr = pQueryCondition->GetCondition(&pCondition);
        if (SUCCEEDED(hr))
        {
            hr = m_pSearchFolderItemFactory->SetCondition(pCondition);
        }
    }

    if (SUCCEEDED(hr))
    {
        // +3
        PIDLIST_ABSOLUTE pSearchIDL = NULL;
        hr = m_pSearchFolderItemFactory->GetIDList(&pSearchIDL);
        if (SUCCEEDED(hr))
        {
            EnumSearchResult(pSearchIDL);
            // -3
            CoTaskMemFree(pSearchIDL);
        }
    }

    // -2
    SAFE_RELEASE(pCondition);
    // -1
    SAFE_RELEASE(psiaScope);

    // If user stops searching, do not call finish searching result callback.
    if (!m_hasStopped)
    {
        // Notify caller that the searching operation is finished.
        if (NULL != m_pSearcherNotify)
        {
            m_pSearcherNotify->OnSearchFinish(this);
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkFileSearcher::SearchAsync()
{
    // Stop search thread first.
    StopSearch();

    m_hasCancel = FALSE;
    unsigned int dwThreadId = 0;
    m_uThreadHandle = (HANDLE)_beginthreadex(
        NULL,
        0,
        SdkFileSearcher::SearchThreadProc,
        (LPVOID)this,
        0,
        &dwThreadId);

    //m_uThreadHandle = chBEGINTHREADEX(
    //    NULL,
    //    0,
    //    SdkFileSearcher::SearchThreadProc,
    //    (LPVOID)this,
    //    0,
    //    &dwThreadId);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

void SdkFileSearcher::CancelSearch()
{
    m_hasCancel = TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SdkFileSearcher::StopSearch()
{
    m_hasStopped = TRUE;
    WaitForSingleObject(m_uThreadHandle, 5000);
}

//////////////////////////////////////////////////////////////////////////

void SdkFileSearcher::ClearSearchResult()
{
    m_vctSearchResults.clear();
}

//////////////////////////////////////////////////////////////////////////

void SdkFileSearcher::ClearSearchCondition()
{
    if (NULL != m_pQueryCondition)
    {
        m_pQueryCondition->ClearCondtions();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkFileSearcher::ClearSearchScope()
{
    if (NULL != m_pQueryScope)
    {
        m_pQueryScope->ClearScope();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkFileSearcher::SetSearchNotify(IFileSearcherNotify *pSearcherNotify)
{
    m_pSearcherNotify = pSearcherNotify;
}

//////////////////////////////////////////////////////////////////////////

const vector<wstring>& SdkFileSearcher::GetSearchResult()
{
    return m_vctSearchResults;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFileSearcher::HasCancelled()
{
    return (m_hasCancel || m_hasStopped);
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkFileSearcher::EnumSearchResult(IN LPITEMIDLIST pidl)
{
    IShellFolder *psfDesktop = NULL;
    IShellFolder *psfSearch = NULL;
    IEnumIDList *penumIDList = NULL;

    // +1
    HRESULT hr = SHGetDesktopFolder(&psfDesktop);
    if (SUCCEEDED(hr) && !HasCancelled())
    {
        // +2
        hr = psfDesktop->BindToObject(pidl, NULL, IID_IShellFolder, (LPVOID*)&psfSearch);
    }

    if (SUCCEEDED(hr) && !HasCancelled())
    {
        // +3
        hr = psfSearch->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &penumIDList);
    }

    if (SUCCEEDED(hr))
    {
        ULONG celtFetched = 0;
        PITEMID_CHILD pChild = NULL;
        while ( !HasCancelled() &&
                SUCCEEDED(penumIDList->Next(1, &pChild, &celtFetched)) &&
                (1 == celtFetched) )
        {
            GetShellItemInfo(psfSearch, pChild);
            CoTaskMemFree(pChild);
        }
    }

    // -3
    SAFE_RELEASE(penumIDList);
    // -2
    SAFE_RELEASE(psfSearch);
    // -1
    SAFE_RELEASE(psfDesktop);

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkFileSearcher::GetShellItemInfo(IN IShellFolder *psf, IN LPCITEMIDLIST pidl)
{
    LPITEMIDLIST pRealIDL = NULL;
    HRESULT hr = SHGetRealIDL(psf, pidl, &pRealIDL);
    if (SUCCEEDED(hr))
    {
        STRRET strName;
        hr = psf->GetDisplayNameOf(pRealIDL, SHGDN_FORPARSING, &strName);
        if (SUCCEEDED(hr))
        {
            WCHAR szName[MAX_PATH] = { 0 };
            hr = StrRetToBuf(&strName, pRealIDL, szName, MAX_PATH);
            if (SUCCEEDED(hr))
            {
                // Add to vector to use subsequently.
                m_vctSearchResults.push_back(wstring(szName));

                // Once find one result, notify caller.
                if (NULL != m_pSearcherNotify)
                {
                    m_pSearcherNotify->OnSearchOneResult(this, szName);
                }
            }
        }
        CoTaskMemFree(pRealIDL);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

unsigned int WINAPI SdkFileSearcher::SearchThreadProc(LPVOID lpParameter)
{
    CoInitialize(NULL);

    SdkFileSearcher *pThis = static_cast<SdkFileSearcher*>(lpParameter);
    if (NULL != pThis)
    {
        pThis->Search();
        pThis->m_uThreadHandle = NULL;
    }

    CoUninitialize();

    return 0;
}
