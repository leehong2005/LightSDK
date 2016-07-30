/*!
* @file DataConvertor.cpp
* 
* @brief Defines some functions to convert one data type to another data type.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/21
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "DataConvertor.h"

USING_NAMESPACE_COMMON

BOOL DataConvertor::ToInt32(IN BOOL bValue, OUT INT32& outValue)
{
    outValue = (bValue) ? 1 : 0;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL DataConvertor::ToInt32(IN LPCWSTR lpValue, OUT INT32& outValue)
{
    outValue = _wtoi(lpValue);
    return ((errno != EINVAL) && (errno != ERANGE));
}

//////////////////////////////////////////////////////////////////////////

BOOL DataConvertor::ToBoolean(IN INT32 nValue, OUT BOOL& outValue)
{
    outValue = (nValue > 0) ? TRUE : FALSE;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL DataConvertor::ToBoolean(IN LPCWSTR lpValue, OUT BOOL& outValue)
{
    if (NULL != lpValue)
    {
        wstring str(lpValue);
        outValue = (0 == str.compare(L"TRUE")) ? TRUE : ((0 == str.compare(L"FALSE")) ? FALSE : outValue);
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL DataConvertor::ToString(IN INT32 nValue, OUT wstring& outValue)
{
    WCHAR szBuf[MAX_PATH] = { 0 };
    int retVal = _itow_s(nValue, szBuf, MAX_PATH, 10);
    if (0 == retVal)
    {
        outValue.clear();
        outValue.append(szBuf);
    }

    return (0 == retVal) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL DataConvertor::ToString(IN bool bValue, OUT wstring& outValue)
{
    outValue.clear();
    outValue.append(bValue ? L"TRUE" : L"FALSE");

    return TRUE;
}
