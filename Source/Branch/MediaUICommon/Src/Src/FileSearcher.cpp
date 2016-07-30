/*!
* @file FileSearcher.cpp
* 
* @brief This file defines class FileSearcher using to search file from disk.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#include "FileSearcher.h"
#include "CommonHelper.h"
#include "Logger.h"

USING_NAMESPACE_COMMON

FileSearcher::FileSearcher() : m_pSearchFolderItemFactory(NULL),
                               m_pTempQueryScope(NULL),
                               m_pTempQueryCondition(NULL),
                               m_pSearcherNotify(NULL),
                               m_hasCancel(FALSE),
                               m_hasStopped(FALSE),
                               m_nTag(0),
                               m_uThreadHandle(NULL),
                               m_nQueryConditionKind(QUERY_KIND_NONE),
                               m_pQueryScope(new QueryScope()),
                               m_pQueryCondition(new QueryCondition())
{
}

////////////////////////////////////////////////////////////////////////

FileSearcher::~FileSearcher()
{
    SAFE_RELEASE(m_pSearchFolderItemFactory);
    SAFE_DELETE(m_pQueryScope);
    SAFE_DELETE(m_pQueryCondition);
}

////////////////////////////////////////////////////////////////////////

HRESULT FileSearcher::SetCondition(INT32 queryKind)
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

HRESULT FileSearcher::SetCondition(LPCWSTR *ppszQuerys, UINT32 uCount)
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

HRESULT FileSearcher::SetCondition(const QueryCondition *pQueryCondition)
{
    ClearSearchCondition();
    m_pTempQueryCondition = const_cast<QueryCondition*>(pQueryCondition);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

INT32 FileSearcher::GetCodition() const
{
    return m_nQueryConditionKind;
}

//////////////////////////////////////////////////////////////////////////

HRESULT FileSearcher::SetScope(INT32 scopeType)
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

HRESULT FileSearcher::SetScope(PCWSTR *pszScopePaths, UINT32 uCount)
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

HRESULT FileSearcher::SetScope(const QueryScope *pQueryScope)
{
    ClearSearchScope();
    m_pTempQueryScope = const_cast<QueryScope*>(pQueryScope);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT FileSearcher::Search()
{
    SAFE_RELEASE(m_pSearchFolderItemFactory);
    CoCreateInstance(CLSID_SearchFolderItemFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pSearchFolderItemFactory));

    if (NULL == m_pSearchFolderItemFactory)
    {
        return E_FAIL;
    }

    m_hasCancel  = FALSE;
    m_hasStopped = FALSE;
    ClearSearchResult();

    HRESULT hr = E_FAIL;
    QueryScope *pQueryScope = (NULL != m_pTempQueryScope) ? m_pTempQueryScope : m_pQueryScope;
    QueryCondition *pQueryCondition = (NULL != m_pTempQueryCondition) ? m_pTempQueryCondition : m_pQueryCondition;

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

HRESULT FileSearcher::SearchAsync()
{
    // Stop search thread first.
    StopSearch();

    m_hasCancel = FALSE;
    unsigned int dwThreadId = 0;
    m_uThreadHandle = (HANDLE)_beginthreadex(NULL, 0, FileSearcher::SearchThreadProc, (LPVOID)this, 0, &dwThreadId);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

void FileSearcher::CancelSearch()
{
    m_hasCancel = TRUE;
}

//////////////////////////////////////////////////////////////////////////

void FileSearcher::StopSearch()
{
    m_hasStopped = TRUE;
    WaitForSingleObject(m_uThreadHandle, 5000);
}

//////////////////////////////////////////////////////////////////////////

void FileSearcher::ClearSearchResult()
{
    m_vctSearchResults.clear();
}

//////////////////////////////////////////////////////////////////////////

void FileSearcher::ClearSearchCondition()
{
    if (NULL != m_pQueryCondition)
    {
        m_pQueryCondition->ClearCondtions();
    }
}

//////////////////////////////////////////////////////////////////////////

void FileSearcher::ClearSearchScope()
{
    if (NULL != m_pQueryScope)
    {
        m_pQueryScope->ClearScope();
    }
}

//////////////////////////////////////////////////////////////////////////

void FileSearcher::SetSearchNotify(FileSearcherNotify *pSearcherNotify)
{
    m_pSearcherNotify = pSearcherNotify;
}

//////////////////////////////////////////////////////////////////////////

const vector<wstring>& FileSearcher::GetSearchResult()
{
    return m_vctSearchResults;
}

//////////////////////////////////////////////////////////////////////////

void FileSearcher::SetTag(INT_PTR tag)
{
    m_nTag = tag;
}

//////////////////////////////////////////////////////////////////////////

INT_PTR FileSearcher::GetTag()
{
    return m_nTag;
}

//////////////////////////////////////////////////////////////////////////

BOOL FileSearcher::HasCancelled()
{
    return (m_hasCancel || m_hasStopped);
}

//////////////////////////////////////////////////////////////////////////

HRESULT FileSearcher::EnumSearchResult(IN LPITEMIDLIST pidl)
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
        while ( !HasCancelled() && SUCCEEDED(penumIDList->Next(1, &pChild, &celtFetched)) && (1 == celtFetched) )
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

HRESULT FileSearcher::GetShellItemInfo(IN IShellFolder *psf, IN LPCITEMIDLIST pidl)
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

unsigned int WINAPI FileSearcher::SearchThreadProc(LPVOID lpParameter)
{
    CoInitialize(NULL);

    FileSearcher *pThis = static_cast<FileSearcher*>(lpParameter);
    if (NULL != pThis)
    {
        pThis->Search();
        pThis->m_uThreadHandle = NULL;
    }

    CoUninitialize();

    return 0;
}
