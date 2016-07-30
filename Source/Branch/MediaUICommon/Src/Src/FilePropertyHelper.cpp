/*!
* @file FilePropertyHelper.cpp
* 
* @brief This file define media content functions.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#include "FilePropertyHelper.h"
#include "FilePropertyKeyHelper.h"

USING_NAMESPACE_COMMON

FilePropertyHelper::FilePropertyHelper(IN LPCWSTR lpFileName) : m_pShellItem2(NULL),
                                                                m_pPropSystem(NULL)
{
    ZeroMemory(m_szItemPath, sizeof(WCHAR) * MAX_PATH);

    ::SHCreateItemFromParsingName(lpFileName, NULL, IID_PPV_ARGS(&m_pShellItem2));
    if (NULL != m_pShellItem2)
    {
        wcscpy_s(m_szItemPath, MAX_PATH, lpFileName);
    }

    ::PSGetPropertySystem(IID_PPV_ARGS(&m_pPropSystem));
}

//////////////////////////////////////////////////////////////////////////

FilePropertyHelper::FilePropertyHelper(const FilePropertyHelper& src)
{
    m_pShellItem2    = src.m_pShellItem2;
    m_pPropSystem    = src.m_pPropSystem;
    wcscpy_s(m_szItemPath, MAX_PATH, src.m_szItemPath);

    SAFE_ADDREF(m_pShellItem2);
    SAFE_ADDREF(m_pPropSystem);
}

//////////////////////////////////////////////////////////////////////////

FilePropertyHelper& FilePropertyHelper::operator = (const FilePropertyHelper& src)
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

FilePropertyHelper::~FilePropertyHelper()
{
    SAFE_RELEASE(m_pShellItem2);
    SAFE_RELEASE(m_pPropSystem);
}

//////////////////////////////////////////////////////////////////////////

BOOL FilePropertyHelper::SetFileName(IN LPCWSTR lpFileName)
{
    if (NULL != lpFileName)
    {
        if (0 != CommonHelper::OrdinalIgnoreCaseCompareStrings(lpFileName, m_szItemPath))
        {
            FilePropertyHelper mediaContent(lpFileName);
            (*this) = mediaContent;
        }
    }

    return (NULL != lpFileName) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL FilePropertyHelper::GetFileName(OUT LPWSTR lpszFileName, UINT32 uSize)
{
    if (NULL != lpszFileName)
    {
        wcscpy_s(lpszFileName, uSize, m_szItemPath);
    }

    return (NULL != lpszFileName) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL FilePropertyHelper::FormatForDisplay(IN PROPERTYKEYNAME keyName, OUT LPWSTR lpszStr, IN UINT32 uSize, PROPDESC_FORMAT_FLAGS pdfFlags)
{
    HRESULT hr = E_FAIL;

    if ( (NULL != m_pShellItem2) && (NULL != m_pPropSystem) )
    {
        PROPERTYKEYINFO keyInfo;
        if (FilePropertyKeyHelper::GetPropertyKey(keyName, keyInfo))
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

BOOL FilePropertyHelper::GetString(IN PROPERTYKEYNAME keyName, OUT wstring &str)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (FilePropertyKeyHelper::GetPropertyKey(keyName, keyInfo))
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

BOOL FilePropertyHelper::GetString(IN PROPERTYKEYNAME keyName, OUT LPWSTR lpszStr, IN UINT32 uSize)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (FilePropertyKeyHelper::GetPropertyKey(keyName, keyInfo))
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

BOOL FilePropertyHelper::GetBOOL(IN PROPERTYKEYNAME keyName, OUT BOOL *pf)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (FilePropertyKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            hr = m_pShellItem2->GetBool(keyInfo.keyValue, pf);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL FilePropertyHelper::GetInt32(IN PROPERTYKEYNAME keyName, OUT INT32 *pi)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (FilePropertyKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            hr = m_pShellItem2->GetInt32(keyInfo.keyValue, pi);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL FilePropertyHelper::GetUInt32(IN PROPERTYKEYNAME keyName, OUT ULONG *pui)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (FilePropertyKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            hr = m_pShellItem2->GetUInt32(keyInfo.keyValue, pui);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL FilePropertyHelper::GetUInt64(IN PROPERTYKEYNAME keyName, OUT ULONGLONG  *pull)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (FilePropertyKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            hr = m_pShellItem2->GetUInt64(keyInfo.keyValue, pull);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL FilePropertyHelper::GetDateTime(IN PROPERTYKEYNAME keyName, OUT FILETIME *pft)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pShellItem2)
    {
        PROPERTYKEYINFO keyInfo;
        if (FilePropertyKeyHelper::GetPropertyKey(keyName, keyInfo))
        {
            hr = m_pShellItem2->GetFileTime(keyInfo.keyValue, pft);
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}
