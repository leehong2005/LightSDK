/*!
* @file SdkQueryScope.cpp
* 
* @brief This file defines class SdkQueryScope using to create search scope.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#include "stdafx.h"
#include "SdkQueryScope.h"

USING_NAMESPACE_COMMON

#define MAKE_PAIR(key, value)       TYPETOFOLDERIDPAIR(key, value)
#define MAKE_STRUCT(key, value)     { key, value }

SdkQueryScope::TYPETOFOLDERIDMAP SdkQueryScope::s_mapTypeToFolderID;

void SdkQueryScope::InitializeMap()
{
    static BOOL isFirst = TRUE;
    if (!isFirst)
    {
        return;
    }
    isFirst = FALSE;

    struct
    {
        QUERYSCOPETYPE  type;
        KNOWNFOLDERID   folderId;
    }

    static const g_szKeyValuePairs[] = 
    {
        MAKE_STRUCT(QUERY_SCOPE_PICTURES,   FOLDERID_PicturesLibrary),
        MAKE_STRUCT(QUERY_SCOPE_MUSIC,      FOLDERID_MusicLibrary),
        MAKE_STRUCT(QUERY_SCOPE_VIDEO,      FOLDERID_VideosLibrary),
        MAKE_STRUCT(QUERY_SCOPE_DECUMENT,   FOLDERID_DocumentsLibrary),
        MAKE_STRUCT(QUERY_SCOPE_FAVORITES,  FOLDERID_Favorites),
        MAKE_STRUCT(QUERY_SCOPE_COMPUTER,   FOLDERID_ComputerFolder),
        MAKE_STRUCT(QUERY_SCOPE_PUBLIC,     FOLDERID_Public),
        MAKE_STRUCT(QUERY_SCOPE_DESKTOP,    FOLDERID_Desktop),
    };

    s_mapTypeToFolderID.clear();

    int size = ARRAYSIZE(g_szKeyValuePairs);
    for (int i = 0; i < size; ++i)
    {
        s_mapTypeToFolderID.insert(
            MAKE_PAIR(g_szKeyValuePairs[i].type, g_szKeyValuePairs[i].folderId)
            );
    }
}

//////////////////////////////////////////////////////////////////////////

SdkQueryScope::SdkQueryScope() : m_nScopeType(0),
                                 m_uScopePathsCount(0),
                                 m_pszScopePaths(NULL)
{
    InitializeMap();
}

//////////////////////////////////////////////////////////////////////////

SdkQueryScope::~SdkQueryScope()
{
    ClearScope();
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryScope::SetScope(INT32 scopeType)
{
    m_nScopeType = scopeType;

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryScope::SetScope(PCWSTR *pszScopePaths, UINT32 uCount)
{
    if ( (NULL == pszScopePaths) || (0 == uCount) )
    {
        return E_INVALIDARG;
    }

    ClearScope();

    m_uScopePathsCount = uCount;
    m_pszScopePaths = new PCWSTR[uCount];
    memset(m_pszScopePaths, 0, uCount * sizeof(PCWSTR));

    for (UINT32 i = 0; i < uCount; ++i)
    {
        int length  = wcslen(pszScopePaths[i]);
        int ccbDest = sizeof(WCHAR) * (length + 1);
        int ccbSrc  = sizeof(WCHAR) * length;

        WCHAR *pDest = new WCHAR[length + 1];
        memset(pDest, 0, ccbDest);
        memcpy_s(pDest, ccbDest, pszScopePaths[i], ccbSrc);
        m_pszScopePaths[i] = pDest;
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryScope::GetScope(OUT IShellItemArray **ppsia)
{
    if (NULL == ppsia)
    {
        return E_INVALIDARG;
    }

    vector<PIDLIST_ABSOLUTE> vctIDLists;
    vector<QUERYSCOPETYPE>   vctScopeTypes;

    HRESULT hr = GetScopeType(vctScopeTypes);

    for each (QUERYSCOPETYPE scope in vctScopeTypes)
    {
        map<QUERYSCOPETYPE, KNOWNFOLDERID>::const_iterator itor = s_mapTypeToFolderID.find(scope);
        if (itor == s_mapTypeToFolderID.end())
        {
            continue;
        }

        PIDLIST_ABSOLUTE pItemID = NULL;
        hr = SHGetKnownFolderIDList(itor->second, KF_FLAG_DEFAULT, NULL, &pItemID);
        if (SUCCEEDED(hr))
        {
            vctIDLists.push_back(pItemID);
        }
    }

    for (UINT32 i = 0; i < m_uScopePathsCount; ++i)
    {
        IShellItem2 *pShellItem2 = NULL;
        hr = SHCreateItemFromParsingName(
            m_pszScopePaths[i], NULL, IID_PPV_ARGS(&pShellItem2));

        if ( SUCCEEDED(hr) )
        {
            PIDLIST_ABSOLUTE pItemID = NULL;
            hr = SHGetIDListFromObject(pShellItem2, &pItemID);
            if (SUCCEEDED(hr))
            {
                vctIDLists.push_back(pItemID);
            }
        }

        SAFE_RELEASE(pShellItem2);
    }

    int idlSize = vctIDLists.size();
    if (idlSize > 0)
    {
        PIDLIST_ABSOLUTE *rgItemIDs = new PIDLIST_ABSOLUTE[idlSize];
        for (int i = 0; i < idlSize; ++i)
        {
            rgItemIDs[i] = vctIDLists[i];
        }

        IShellItemArray *psia = NULL;
        hr = SHCreateShellItemArrayFromIDLists(idlSize, (LPCITEMIDLIST*)rgItemIDs, &psia);
        if (SUCCEEDED(hr))
        {
            hr = psia->QueryInterface(IID_PPV_ARGS(ppsia));
        }
        SAFE_RELEASE(psia);

        for (int i = 0; i < idlSize; ++i)
        {
            CoTaskMemFree(rgItemIDs[i]);
        }
        SAFE_DELETE_ARRAY(rgItemIDs);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryScope::ClearScope()
{
    if (NULL != m_pszScopePaths)
    {
        for (UINT32 i = 0; i < m_uScopePathsCount; ++i)
        {
            SAFE_DELETE_ARRAY(m_pszScopePaths[i]);
        }
        SAFE_DELETE_ARRAY(m_pszScopePaths);
    }

    m_uScopePathsCount = 0;

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryScope::GetScopeType(OUT vector<QUERYSCOPETYPE> &vctScopeTypes)
{
    FILTER_BITVALUE(m_nScopeType, QUERY_SCOPE_PICTURES,     vctScopeTypes);
    FILTER_BITVALUE(m_nScopeType, QUERY_SCOPE_MUSIC,        vctScopeTypes);
    FILTER_BITVALUE(m_nScopeType, QUERY_SCOPE_VIDEO,        vctScopeTypes);
    FILTER_BITVALUE(m_nScopeType, QUERY_SCOPE_DECUMENT,     vctScopeTypes);
    FILTER_BITVALUE(m_nScopeType, QUERY_SCOPE_PUBLIC,       vctScopeTypes);
    FILTER_BITVALUE(m_nScopeType, QUERY_SCOPE_SAMPLEMUSIC,  vctScopeTypes);
    FILTER_BITVALUE(m_nScopeType, QUERY_SCOPE_DESKTOP,      vctScopeTypes);
    FILTER_BITVALUE(m_nScopeType, QUERY_SCOPE_STARTMENU,    vctScopeTypes);
    FILTER_BITVALUE(m_nScopeType, QUERY_SCOPE_FAVORITES,    vctScopeTypes);
    FILTER_BITVALUE(m_nScopeType, QUERY_SCOPE_COMPUTER,     vctScopeTypes);

    return S_OK;
}
