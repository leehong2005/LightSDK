/*!
* @file SdkFilePropHelper.cpp
* 
* @brief This file define media content functions.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#include "stdafx.h"
#include "SdkFilePropHelper.h"
#include "SdkFilePropKeyHelper.h"

USING_NAMESPACE_COMMON

SdkFilePropHelper::SdkFilePropHelper(IN LPCWSTR lpFileName)
{
    m_pShellItem2 = NULL;
    m_pPropSystem = NULL;
    ZeroMemory(m_szItemPath, sizeof(WCHAR) * MAX_PATH);

    ::SHCreateItemFromParsingName(lpFileName, NULL, IID_PPV_ARGS(&m_pShellItem2));
    if (NULL != m_pShellItem2)
    {
        wcscpy_s(m_szItemPath, MAX_PATH, lpFileName);
    }

    ::PSGetPropertySystem(IID_PPV_ARGS(&m_pPropSystem));
}

//////////////////////////////////////////////////////////////////////////

SdkFilePropHelper::SdkFilePropHelper(const SdkFilePropHelper& src)
{
    m_pShellItem2    = src.m_pShellItem2;
    m_pPropSystem    = src.m_pPropSystem;
    wcscpy_s(m_szItemPath, MAX_PATH, src.m_szItemPath);

    SAFE_ADDREF(m_pShellItem2);
    SAFE_ADDREF(m_pPropSystem);
}

//////////////////////////////////////////////////////////////////////////

SdkFilePropHelper& SdkFilePropHelper::operator = (const SdkFilePropHelper& src)
{
    if (this == &src)
    {
        return *this;
    }

    SAFE_RELEASE(m_pShellItem2);
    SAFE_RELEASE(m_pPropSystem);

    m_pShellItem2    = src.m_pShellItem2;
    m_pPropSystem    = src.m_pPropSystem;
    wcscpy_s(m_szItemPath, MAX_PATH, src.m_szItemPath);

    SAFE_ADDREF(m_pShellItem2);
    SAFE_ADDREF(m_pPropSystem);

    return *this;
}

//////////////////////////////////////////////////////////////////////////

SdkFilePropHelper::~SdkFilePropHelper()
{
    SAFE_RELEASE(m_pShellItem2);
    SAFE_RELEASE(m_pPropSystem);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropHelper::SetFileName(IN LPCWSTR lpFileName)
{
    if (NULL != lpFileName)
    {
        if (0 != SdkCommonHelper::OrdinalIgnoreCaseCompareStrings(lpFileName, m_szItemPath))
        {
            SdkFilePropHelper mediaContent(lpFileName);
            (*this) = mediaContent;
        }
    }

    return (NULL != lpFileName) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropHelper::GetFileName(OUT LPWSTR lpszFileName, UINT32 uSize)
{
    if (NULL != lpszFileName)
    {
        wcscpy_s(lpszFileName, uSize, m_szItemPath);
    }

    return (NULL != lpszFileName) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropHelper::FormatForDisplay(IN PROPERTYKEYNAME keyName, OUT LPWSTR lpszStr, IN UINT32 uSize, PROPDESC_FORMAT_FLAGS pdfFlags)
{
    HRESULT hr = E_FAIL;

    if ( (NULL != m_pShellItem2) && (NULL != m_pPropSystem) )
    {
        PROPERTYKEYINFO keyInfo;
        if (SdkFilePropKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            PROPVARIANT propValue;
            PropVariantInit(&propValue);

            hr = m_pShellItem2->GetProperty(keyInfo.keyValue, &propValue);
            if (SUCCEEDED(hr))
            {
                hr = m_pPropSystem->FormatForDisplay(keyInfo.keyValue, propValue, pdfFlags, lpszStr, uSize);
            }

            PropVariantClear(&propValue);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropHelper::GetString(IN PROPERTYKEYNAME keyName, OUT wstring &str)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (SdkFilePropKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            LPWSTR pstr = NULL;
            hr = m_pShellItem2->GetString(keyInfo.keyValue, &pstr);

            if (SUCCEEDED(hr))
            {
                str.clear();
                str.append(pstr);
            }

            ::CoTaskMemFree(pstr);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropHelper::GetString(IN PROPERTYKEYNAME keyName, OUT LPWSTR lpszStr, IN UINT32 uSize)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (SdkFilePropKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            LPWSTR pstr = NULL;
            hr = m_pShellItem2->GetString(keyInfo.keyValue, &pstr);
            if (SUCCEEDED(hr))
            {
                wcscpy_s(lpszStr, uSize, pstr);
            }
            ::CoTaskMemFree(pstr);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropHelper::GetBOOL(IN PROPERTYKEYNAME keyName, OUT BOOL *pf)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (SdkFilePropKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            hr = m_pShellItem2->GetBool(keyInfo.keyValue, pf);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropHelper::GetInt32(IN PROPERTYKEYNAME keyName, OUT INT32 *pi)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (SdkFilePropKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            hr = m_pShellItem2->GetInt32(keyInfo.keyValue, pi);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropHelper::GetUInt32(IN PROPERTYKEYNAME keyName, OUT ULONG *pui)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (SdkFilePropKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            hr = m_pShellItem2->GetUInt32(keyInfo.keyValue, pui);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropHelper::GetUInt64(IN PROPERTYKEYNAME keyName, OUT ULONGLONG  *pull)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (SdkFilePropKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            hr = m_pShellItem2->GetUInt64(keyInfo.keyValue, pull);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropHelper::GetDateTime(IN PROPERTYKEYNAME keyName, OUT FILETIME *pft)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (SdkFilePropKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            hr = m_pShellItem2->GetFileTime(keyInfo.keyValue, pft);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}
