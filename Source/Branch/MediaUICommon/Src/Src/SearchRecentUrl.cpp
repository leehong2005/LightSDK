/* 
* @file SearchRecentlyUrl.cpp
* 
* @brief This file defines class SearchRecentlyUrl using to search file from registry.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Zhou Yuan Qi.
* @date 2011/02/24.
*/

#include "SearchRecentUrl.h"

USING_NAMESPACE_COMMON

#define MAX_KEY_LENGTH      255
#define MAX_VALUE_NAME      16383
#define URLREGPATH          L"SOFTWARE\\Microsoft\\Internet Explorer\\TypedUrls"

SearchRecentUrl::SearchRecentUrl()
{
}

////////////////////////////////////////////////////////////////////////

SearchRecentUrl::~SearchRecentUrl()
{
}

/////////////////////////////////////////////////////////////////////////

void SearchRecentUrl::EnumRecentUrls()
{
    HKEY hKey = NULL;

    if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, URLREGPATH,
                                       0,
                                       KEY_READ,
                                       &hKey) )
    {
        QueryInfoKey(hKey);
        RegCloseKey(hKey);
    }
}

/////////////////////////////////////////////////////////////////////////

const vector<wstring>* SearchRecentUrl::GetRecentlyUrls() const
{
    return &m_searchUrls;
}

///////////////////////////////////////////////////////////////////////////

void SearchRecentUrl::EnumUrlValues(IN DWORD number,IN HKEY hKey)
{
    DWORD cchValue = MAX_VALUE_NAME; 
    TCHAR achValue[MAX_VALUE_NAME];
    DWORD i,retCode; 
    DWORD dwCount = 0;
    DWORD dwType  = REG_SZ;
    wstring szDataName;

    if(number > 0)
    {
        for(i = 0, retCode = ERROR_SUCCESS; i < number; i++)
        {
            cchValue = MAX_VALUE_NAME; 
            achValue[0] = '\0';
            retCode= RegEnumValue(hKey, i, achValue, &cchValue, 
                                  NULL, NULL,NULL,NULL);
            if (retCode == ERROR_SUCCESS ) 
            {
                retCode = RegQueryValueEx(hKey,achValue,NULL,&dwType,NULL,&dwCount);
                if (retCode== ERROR_SUCCESS)
                {
                    szDataName.resize(dwCount);
                    retCode = RegQueryValueEx(hKey,achValue,
                                              NULL,&dwType,
                                             (LPBYTE)szDataName.data(),
                                             &dwCount);

                    if(ERROR_SUCCESS == retCode)
                    {
                        // Add result to the  vector.
                        if (szDataName.length() > 0)
                        {
                            m_searchUrls.push_back(szDataName);
                        }
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////

void SearchRecentUrl::QueryInfoKey(IN HKEY hKey)
{
    TCHAR    achClass[MAX_PATH] = L"";
    DWORD    cchClassName = MAX_PATH;
    DWORD    cSubKeys=0;
    DWORD    cbMaxSubKey;
    DWORD    cchMaxClass;
    DWORD    kNumber;
    DWORD    cchMaxValue;
    DWORD    cbMaxValueData;
    DWORD    cbSecurityDescriptor; 
    FILETIME ftLastWriteTime;
    DWORD    retCode; 

    retCode = RegQueryInfoKey(hKey, achClass,&cchClassName,NULL,
                              &cSubKeys,&cbMaxSubKey,&cchMaxClass,
                              &kNumber, &cchMaxValue,&cbMaxValueData,
                              &cbSecurityDescriptor,&ftLastWriteTime);

    if (retCode == ERROR_SUCCESS)
    {
        EnumUrlValues(kNumber,hKey);
    }
}
