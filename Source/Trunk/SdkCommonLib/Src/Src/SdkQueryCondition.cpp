/*!
* @file SdkQueryCondition.cpp
* 
* @brief This file defines class QueryScope using to create search condition.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#include "stdafx.h"
#include "SdkQueryCondition.h"

BEGIN_NAMESPACE_COMMON
typedef struct _QUERYPARSEROPTIONS
{
    PCWSTR pszPropertyName;
    PCWSTR pszSemanticType;

} QUERYPARSEROPTIONS, *LPQUERYPARSEROPTIONS;
END_NAMESPACE_COMMON

USING_NAMESPACE_COMMON

const QUERYPARSEROPTIONS g_rgGenericProperties[] =
{
    { L"System.Generic.String",          L"System.StructuredQueryType.String" },
    { L"System.Generic.Integer",         L"System.StructuredQueryType.Integer" },
    { L"System.Generic.DateTime",        L"System.StructuredQueryType.DateTime" },
    { L"System.Generic.Boolean",         L"System.StructuredQueryType.Boolean" },
    { L"System.Generic.FloatingPoint",   L"System.StructuredQueryType.FloatingPoint" }
};

//////////////////////////////////////////////////////////////////////////

SdkQueryCondition::SdkQueryCondition() : m_pQueryParser(NULL),
                                         m_pszQuerys(NULL),
                                         m_nQueryKind(0),
                                         m_uQueryCount(0)
{
}

//////////////////////////////////////////////////////////////////////////

SdkQueryCondition::~SdkQueryCondition()
{
    SAFE_RELEASE(m_pQueryParser);
    ClearCondtions();
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryCondition::GetCondition(ICondition **ppc)
{
    if (NULL == ppc)
    {
        return E_INVALIDARG;
    }

    *ppc = NULL;
    IConditionFactory2 *pConditionFactory = NULL;
    HRESULT hr = CoCreateInstance(
        CLSID_ConditionFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pConditionFactory));

    if ( FAILED(hr) )
    {
        return hr;
    }

    vector<ICondition*> vctConditions;
    vector<QUERYCONDITIONKIND> vctKinds;
    hr = CreateQueryParser();
    hr = GetQueryKindList(vctKinds);

    for (UINT32 i = 0; i < m_uQueryCount; ++i)
    {
        ICondition *pCondition = NULL;
        hr = ParseStructuredQuery(m_pszQuerys[i], &pCondition);
        if (SUCCEEDED(hr))
        {
            vctConditions.push_back(pCondition);
        }
    }

    for each (QUERYCONDITIONKIND kind in vctKinds)
    {
        ICondition *pCondition = NULL;
        hr = ParseStructuredQuery(kind, &pCondition);
        if (SUCCEEDED(hr))
        {
            vctConditions.push_back(pCondition);
        }
    }

    int conditionCount = vctConditions.size();
    hr = (conditionCount > 0) ? S_OK : E_FAIL;

    if (conditionCount > 0)
    {
        ICondition **pszConditions = new ICondition*[conditionCount];
        for (int i = 0; i < conditionCount; ++i)
        {
            pszConditions[i] = vctConditions[i];
        }

        hr = pConditionFactory->CreateCompoundFromArray(
            CT_OR_CONDITION,
            pszConditions,
            conditionCount,
            CONDITION_CREATION_DEFAULT,
            IID_PPV_ARGS(ppc));

        // Release the memory.
        for (int i = 0; i < conditionCount; ++i)
        {
            SAFE_RELEASE(pszConditions[i]);
        }

        SAFE_DELETE_ARRAY(pszConditions);
    }

    SAFE_RELEASE(pConditionFactory);

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryCondition::SetCondition(LPCWSTR *ppszQuerys, UINT32 uCount)
{
    if ( (NULL == ppszQuerys) || (0 == uCount) )
    {
        return E_INVALIDARG;
    }

    ClearCondtions();
    m_uQueryCount = uCount;
    m_pszQuerys = new PCWSTR[uCount];
    memset(m_pszQuerys, 0, uCount * sizeof(PCWSTR));

    for (UINT32 i = 0; i < uCount; ++i)
    {
        int length  = wcslen(ppszQuerys[i]);
        int ccbDest = sizeof(WCHAR) * (length + 1);
        int ccbSrc  = sizeof(WCHAR) * length;

        WCHAR *pDest = new WCHAR[length + 1];
        memset(pDest, 0, ccbDest);
        memcpy_s(pDest, ccbDest, ppszQuerys[i], ccbSrc);
        m_pszQuerys[i] = pDest;
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryCondition::SetCondition(INT32 queryKind)
{
    m_nQueryKind = queryKind;

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryCondition::ClearCondtions()
{
    if (NULL != m_pszQuerys)
    {
        for (UINT32 i = 0; i < m_uQueryCount; ++i)
        {
            SAFE_DELETE_ARRAY(m_pszQuerys[i]);
        }

        SAFE_DELETE_ARRAY(m_pszQuerys);
    }

    m_uQueryCount = 0;

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryCondition::CreateQueryParser()
{
    SAFE_RELEASE(m_pQueryParser);
    IQueryParserManager *pQueryParserManager = NULL;

    HRESULT hr = CoCreateInstance(
        CLSID_QueryParserManager,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pQueryParserManager));

    if ( SUCCEEDED(hr) )
    {
        IQueryParser *pQueryParser = NULL;
        hr = pQueryParserManager->CreateLoadedParser(
            L"SystemIndex",
            LOCALE_USER_DEFAULT,
            IID_PPV_ARGS(&pQueryParser));

        if (SUCCEEDED(hr))
        {
            hr = pQueryParserManager->InitializeOptions(FALSE, TRUE, pQueryParser);
            for (int i = 0; i < ARRAYSIZE(g_rgGenericProperties) && SUCCEEDED(hr); ++i)
            {
                PCWSTR pszPropertyName = g_rgGenericProperties[i].pszPropertyName;
                PCWSTR pszSemanticType = g_rgGenericProperties[i].pszSemanticType;

                PROPVARIANT propVar;
                hr = InitPropVariantFromString(pszPropertyName, &propVar);

                hr = pQueryParser->SetMultiOption(
                    SQMO_DEFAULT_PROPERTY,
                    pszSemanticType,
                    &propVar);

                PropVariantClear(&propVar);
            }

            if (SUCCEEDED(hr))
            {
                pQueryParser->QueryInterface(IID_PPV_ARGS(&m_pQueryParser));
            }
        }

        SAFE_RELEASE(pQueryParser);
    }

    SAFE_RELEASE(pQueryParserManager);

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryCondition::ParseStructuredQuery(QUERYCONDITIONKIND kind, OUT ICondition **ppc)
{
    LPCWSTR strKind = NULL;

    switch (kind)
    {
    case QUERY_KIND_MUSIC:
        strKind = KIND_MUSIC;
        break;

    case QUERY_KIND_VIDEO:
        strKind = KIND_VIDEO;
        break;

    case QUERY_KIND_PICTURE:
        strKind = KIND_PICTURE;
        break;

    case QUERY_KIND_DOCUMENT:
        strKind = KIND_DOCUMENT;
        break;

    case QUERY_KIND_LINK:
        strKind = KIND_LINK;
        break;
    }

    HRESULT hr = (NULL != strKind) ? S_OK : E_INVALIDARG;

    if ( SUCCEEDED(hr) )
    {
        IConditionFactory2 *pConditionFactory = NULL;
        hr = CoCreateInstance(
            CLSID_ConditionFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pConditionFactory));

        if ( SUCCEEDED(hr) )
        {
            hr = pConditionFactory->CreateStringLeaf(
                PKEY_Kind, 
                COP_EQUAL,
                strKind,
                NULL,
                CONDITION_CREATION_DEFAULT,
                IID_PPV_ARGS(ppc));
        }

        SAFE_RELEASE(pConditionFactory);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryCondition::ParseStructuredQuery(PCWSTR pszString, OUT ICondition **ppc)
{
    HRESULT hr = ( (NULL == ppc) || (NULL == m_pQueryParser) ) ? S_OK : E_INVALIDARG;

    if ( SUCCEEDED(hr) )
    {
        IQuerySolution *pQuerySolution = NULL;
        hr = m_pQueryParser->Parse(pszString, NULL, &pQuerySolution);

        if (SUCCEEDED(hr))
        {
            ICondition *pCondition = NULL;
            hr = pQuerySolution->GetQuery(&pCondition, NULL);
            if (SUCCEEDED(hr))
            {
                SYSTEMTIME sysTime;
                GetLocalTime(&sysTime);
                hr = pQuerySolution->Resolve(pCondition, SQRO_DONT_SPLIT_WORDS, &sysTime, ppc);
            }

            SAFE_RELEASE(pCondition);
        }

        SAFE_RELEASE(pQuerySolution);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkQueryCondition::GetQueryKindList(OUT vector<QUERYCONDITIONKIND> &vctKinds)
{
    FILTER_BITVALUE(m_nQueryKind, QUERY_KIND_MUSIC,     vctKinds);
    FILTER_BITVALUE(m_nQueryKind, QUERY_KIND_VIDEO,     vctKinds);
    FILTER_BITVALUE(m_nQueryKind, QUERY_KIND_PICTURE,   vctKinds);
    FILTER_BITVALUE(m_nQueryKind, QUERY_KIND_DOCUMENT,  vctKinds);
    FILTER_BITVALUE(m_nQueryKind, QUERY_KIND_LINK,      vctKinds);

    return S_OK;
}