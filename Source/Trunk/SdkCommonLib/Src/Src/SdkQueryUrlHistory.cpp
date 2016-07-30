/*!
* @file SdkQueryUrlHistory.cpp
* 
* @brief This file defines SdkQueryUrlHistory class to query url history of internet explorer.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/09
*/

#include "stdafx.h"
#include "SdkQueryUrlHistory.h"
#include "SdkCommonHelper.h"
#include <algorithm>

USING_NAMESPACE_COMMON

SdkQueryUrlHistory::SdkQueryUrlHistory() : m_pUrlHistoryStg2(NULL),
                                           m_pEnumStatUrl(NULL),
                                           m_uCursorIndex(0)
{
    Initialize();
}

//////////////////////////////////////////////////////////////////////////

SdkQueryUrlHistory::~SdkQueryUrlHistory()
{
    ClearSearchUrls();

    SAFE_RELEASE(m_pEnumStatUrl);
    SAFE_RELEASE(m_pUrlHistoryStg2);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkQueryUrlHistory::AddUrl(LPCOLESTR pocsUrl, LPCOLESTR pocsTitle)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pUrlHistoryStg2)
    {
        hr = m_pUrlHistoryStg2->AddUrl(pocsUrl, pocsTitle, 0);
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkQueryUrlHistory::DeleteUrl(LPCOLESTR pocsUrl)
{
    HRESULT hr = E_FAIL;
    if (NULL != m_pUrlHistoryStg2)
    {
        hr = m_pUrlHistoryStg2->DeleteUrl(pocsUrl, 0);
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkQueryUrlHistory::Next(OUT STATURL &statUrl)
{
    if (m_uCursorIndex < (UINT)m_vctSearchUrls.size())
    {
        statUrl = m_vctSearchUrls[m_uCursorIndex];
        m_uCursorIndex++;
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

UINT SdkQueryUrlHistory::GetCount()
{
    return (UINT)(m_vctSearchUrls.size());
}

//////////////////////////////////////////////////////////////////////////

void SdkQueryUrlHistory::Reset()
{
    m_uCursorIndex = 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkQueryUrlHistory::Initialize()
{
    HRESULT hr = CoCreateInstance(
        CLSID_CUrlHistory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&m_pUrlHistoryStg2));

    if ( SUCCEEDED(hr) )
    {
        hr = m_pUrlHistoryStg2->EnumUrls(&m_pEnumStatUrl);
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkQueryUrlHistory::EnumUrlHistory(UINT uUrlsNum)
{
    if (NULL == m_pEnumStatUrl)
    {
        return;
    }

    ClearSearchUrls();

    ULONG celtFetched = 0;
    UINT uMinIndex = 0;
    STATURL statUrl = { 0 };
    statUrl.cbSize = sizeof(STATURL);
    statUrl.dwFlags = STATURL_QUERYFLAG_ISCACHED;

    // Set the enumeration filter.
    m_pEnumStatUrl->Reset();
    m_pEnumStatUrl->SetFilter(L"http://", STATURL_QUERYFLAG_TOPLEVEL);

    while (SUCCEEDED(m_pEnumStatUrl->Next(1, &statUrl, &celtFetched)) && (1 == celtFetched))
    {
        if (m_vctSearchUrls.size() < uUrlsNum)
        {
            if (IsValidSTATURL(&statUrl))
            {
                m_vctSearchUrls.push_back(statUrl);

                LONG lRes = CompareFileTime(
                    &(statUrl.ftLastVisited),
                    &(m_vctSearchUrls[uMinIndex].ftLastVisited));

                if (lRes < 0)
                {
                    uMinIndex = (UINT)(m_vctSearchUrls.size() - 1);
                }
            }
        }
        else
        {
            if (IsValidSTATURL(&statUrl))
            {
                m_vctSearchUrls[uMinIndex] = statUrl;
                uMinIndex = FindEarliestUrl();
            }
        }
    }

    sort(m_vctSearchUrls.begin(), m_vctSearchUrls.end(), STATURLGreater);
}

//////////////////////////////////////////////////////////////////////////

void SdkQueryUrlHistory::ClearSearchUrls()
{
    for each (STATURL statUrl in m_vctSearchUrls)
    {
        if (NULL != statUrl.pwcsUrl)
        {
            CoTaskMemFree(statUrl.pwcsUrl);
        }

        if (NULL != statUrl.pwcsTitle)
        {
            CoTaskMemFree(statUrl.pwcsTitle);
        }
    }

    m_vctSearchUrls.clear();
}

//////////////////////////////////////////////////////////////////////////

UINT SdkQueryUrlHistory::FindEarliestUrl()
{
    vector<STATURL>::iterator item = min_element(
        m_vctSearchUrls.begin(),
        m_vctSearchUrls.end(),
        STATURLLesser);

    UINT index = (UINT)(item - m_vctSearchUrls.begin());

    return index;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkQueryUrlHistory::IsValidSTATURL(const STATURL *pstatUrl)
{
    if (NULL == pstatUrl)
    {
        return FALSE;
    }

    if ( (NULL == pstatUrl->pwcsTitle) || (0 == wcslen(pstatUrl->pwcsTitle)) )
    {
        return FALSE;
    }

    if ( (NULL == pstatUrl->pwcsUrl) || (0 == wcslen(pstatUrl->pwcsUrl)) )
    {
        return FALSE;
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkQueryUrlHistory::STATURLLesser(const STATURL &firstItem, const STATURL &secondItem)
{
    LONG lRes = CompareFileTime(
        &(firstItem.ftLastVisited),
        &(secondItem.ftLastVisited)
        );

    return (lRes < 0) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkQueryUrlHistory::STATURLGreater(const STATURL &firstItem, const STATURL &secondItem)
{
    LONG lRes = CompareFileTime(
        &(firstItem.ftLastVisited),
        &(secondItem.ftLastVisited)
        );

    return (lRes > 0) ? TRUE : FALSE;
}
