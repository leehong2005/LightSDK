/*!
* @file SdkFilePropDescription.cpp
* 
* @brief This file define functions to get shell property description.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/29
*/

#include "stdafx.h"
#include "SdkFilePropDescription.h"

USING_NAMESPACE_COMMON

SdkFilePropDescription::SdkFilePropDescription()
{
}

//////////////////////////////////////////////////////////////////////////

SdkFilePropDescription::~SdkFilePropDescription()
{
    for (PROPKEYTOPROPDESCMAP::iterator itor = m_mapPropKeyToPropDesc.begin();
         itor != m_mapPropKeyToPropDesc.end(); ++itor)
    {
        SAFE_RELEASE(itor->second);
    }

    m_mapPropKeyToPropDesc.clear();
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropDescription::GetPropDescription(IN PROPERTYKEYNAME keyName, OUT LPWSTR lpszPropDesc, IN UINT32 uDescSize)
{
    IPropertyDescription *pPropDesc = NULL;
    PROPKEYTOPROPDESCMAP::iterator itor = m_mapPropKeyToPropDesc.find(keyName);
    if (itor != m_mapPropKeyToPropDesc.end())
    {
        pPropDesc = (itor->second);
    }
    else
    {
        PROPERTYKEYINFO keyInfo;
        BOOL isSucceed = SdkFilePropKeyHelper::GetPropertyKey(keyName, keyInfo);
        if (isSucceed)
        {
            HRESULT hr = PSGetPropertyDescription(keyInfo.keyValue, IID_PPV_ARGS(&pPropDesc));
            if (SUCCEEDED(hr))
            {
                m_mapPropKeyToPropDesc[keyName] = pPropDesc;
            }
        }
    }

    return GetPropDesc(pPropDesc, lpszPropDesc, uDescSize);;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropDescription::GetPropDesc(IN IPropertyDescription *pPropDesc, OUT LPWSTR lpszDesc, UINT32 uDescSize)
{
    if ( (NULL == pPropDesc) || (NULL == lpszDesc) || (0 == uDescSize) )
    {
        return FALSE;
    }

    LPWSTR lpDesc = NULL;
    HRESULT hr = pPropDesc->GetDisplayName(&lpDesc);
    if (SUCCEEDED(hr))
    {
        wcscpy_s(lpszDesc, uDescSize, lpDesc);
        CoTaskMemFree(lpDesc);
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}
