/*!
* @file IniConfig.cpp
* 
* @brief This file defines IniConfig class and implements IConfig interface.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/11
*/

#include "IniConfig.h"
#include "DataConvertor.h"

USING_NAMESPACE_COMMON

#define MINIMUM_FILE_SIZE   1024

IniConfig::IniConfig(IN LPCTSTR lpFileName) : m_isModified(FALSE),
                                              m_dwAllocSize(MINIMUM_FILE_SIZE),
                                              m_isFileExist(TRUE)
{
    ZeroMemory(m_szFileName, MAX_PATH);

    if (NULL != lpFileName)
    {
        wcscpy_s(m_szFileName, MAX_PATH, lpFileName);

        BOOL isSucceed = GetIniFileSize(m_dwAllocSize);
        if (isSucceed)
        {
            if (INVALID_FILE_SIZE == m_dwAllocSize)
            {
                m_dwAllocSize = MINIMUM_FILE_SIZE;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////

IniConfig::~IniConfig()
{
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::GetIntValue(IN LPCTSTR lpKeyName, OUT INT32& nRetValue)
{
    TCHAR szValue[MAX_PATH] = { 0 };
    BOOL isSuccess = GetStringValue(lpKeyName, szValue, MAX_PATH);
    if (isSuccess)
    {
        isSuccess = DataConvertor::ToInt32(szValue, nRetValue);
    }

    return isSuccess;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::GetBooleanValue(IN LPCTSTR lpKeyName, OUT BOOL& bRetValue)
{
    TCHAR szValue[MAX_PATH] = { 0 };

    BOOL isSuccess = GetStringValue(lpKeyName, szValue, MAX_PATH);
    if (isSuccess)
    {
        isSuccess = DataConvertor::ToBoolean(szValue, bRetValue);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::GetStringValue(IN LPCTSTR lpKeyName, OUT LPTSTR lpRetValue, IN DWORD dwSize)
{
    if ((NULL == lpKeyName) || (NULL == lpRetValue) || (dwSize <= 0))
    {
        return FALSE;
    }

    BOOL isSuccess = FALSE;

    if (m_mapKeyValues.size() > 0)
    {
        map<wstring, wstring>::iterator item = m_mapKeyValues.find(wstring(lpKeyName));
        if (item != m_mapKeyValues.end())
        {
            wcscpy_s(lpRetValue, dwSize, item->second.c_str());
            isSuccess = TRUE;
        }
    }

    return isSuccess;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::SetIntValue(IN LPCTSTR lpKeyName, IN INT32 nValue)
{
    wstring outStr;
    if (DataConvertor::ToString(nValue, outStr))
    {
        return SetStringValue(lpKeyName, outStr.c_str());
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::SetBooleanValue(IN LPCTSTR lpKeyName, IN BOOL bValue)
{
    wstring outStr;
    if (DataConvertor::ToString(bValue ? true : false, outStr))
    {
        return SetStringValue(lpKeyName, outStr.c_str());
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::SetStringValue(IN LPCTSTR lpKeyName, IN LPCTSTR lpValue)
{
    if ((NULL == lpKeyName) || (NULL == lpValue))
    {
        return FALSE;
    }

    BOOL isSuccess = FALSE;

    map<wstring, wstring>::iterator item = m_mapKeyValues.find(wstring(lpKeyName));
    if (item != m_mapKeyValues.end())
    {
        m_mapKeyValues[lpKeyName] = wstring(lpValue);
        m_isModified = TRUE;
        isSuccess = TRUE;
    }
    else
    {
        m_mapKeyValues.insert(make_pair(lpKeyName, lpValue));
        m_isModified = TRUE;
        isSuccess = TRUE;
    }

    return isSuccess;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::ReadFromFile()
{
    if (m_isFileExist)
    {
        // Load and read INI data.
        return EnumSections();
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::SaveToFile()
{
    if (!m_isModified)
    {
        return TRUE;
    }

    wstring strContent;
    wstring strSection;
    wstring strTempSection;
    wstring strValues;

    for (map<wstring, wstring>::iterator itor = m_mapKeyValues.begin();
         itor != m_mapKeyValues.end(); ++itor)
    {
        // Get section
        wstring::size_type nIndex = (itor->first).find_first_of(L'\\');
        if (nIndex != wstring::npos)
        {
            strTempSection = (itor->first).substr(0, nIndex);
        }
        else
        {
            strTempSection = (itor->first);
        }

        if (0 != strSection.compare(strTempSection))
        {
            strSection = strTempSection;
            // Write sections.
            strContent.append(L"[").append(strSection).append(L"]\r\n");
        }

        strValues = (itor->first).substr(nIndex + 1, (itor->first).length() - nIndex);
        strValues.append(L"=").append(itor->second);
        strContent.append(strValues).append(L"\r\n");
    }

    BOOL retVal = FALSE;

    // Create folder
    WCHAR szConfigPath[MAX_PATH] = { 0 };
    wcscpy_s(szConfigPath, MAX_PATH, m_szFileName);
    BOOL isSucceed = PathRemoveFileSpec(szConfigPath);
    if (isSucceed)
    {
        CommonHelper::CreateFolder(szConfigPath);
    }

    // Write to file.
  
    HANDLE hFile = CreateFile(m_szFileName,             // File name.
                              GENERIC_WRITE,            // Only open for writing.
                              0,                        // Do not share.
                              NULL,                     // No security.
                              CREATE_ALWAYS,              // Opens the file, if it exits, otherwise create a new one.
                              FILE_ATTRIBUTE_NORMAL |   // Normal attributes.
                              FILE_FLAG_WRITE_THROUGH,  // Instructs the system to write through any intermediate cache and go directly to disk.
                              NULL                      // No template.
                              );
    if (ISVALIDHANDLE(hFile))
    {
        DWORD dwWritten = 0;
        DWORD dwWrite = 0;
        WCHAR datas = { 0xFEFF };
        WriteFile(hFile, &datas, sizeof(WCHAR), &dwWritten, NULL);
        retVal = WriteFile(hFile,
                           (LPCVOID)(strContent.c_str()),
                           (strContent.length() + 1) * sizeof(WCHAR),
                           &dwWrite,
                           NULL);
    }

    m_isModified = !retVal;
    SAFE_CLOSE_HANDLE(hFile);

    return retVal;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::IsModified()
{
    return m_isModified;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::IsKeyNameExist(IN LPCTSTR lpKeyName)
{
    BOOL isSuccess = FALSE;

    if (NULL != lpKeyName)
    {
        map<wstring, wstring>::iterator item = m_mapKeyValues.find(wstring(lpKeyName));
        if (item != m_mapKeyValues.end())
        {
            isSuccess = TRUE;
        }
    }
    return isSuccess;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::ReLoadData()
{
    m_mapKeyValues.clear();
    m_isModified = FALSE;
    EnumSections();

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::GetFilePath(OUT LPTSTR lpFilePath, IN DWORD dwSize)
{
    if (NULL == lpFilePath || (dwSize <= 0))
    {
        return FALSE;
    }
    wcscpy_s(lpFilePath, dwSize, m_szFileName);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

const vector<wstring>* IniConfig::GetRecords()
{
    return &m_vctSectionNames;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::EnumSections()
{
    DWORD dwSize = m_dwAllocSize / 2;
    TCHAR *pBuffer = new TCHAR[dwSize];
    if (NULL == pBuffer)
    {
        return FALSE;
    }

    ZeroMemory(pBuffer, dwSize);
    BOOL isSucceed = FALSE;
    DWORD dwRetBufSize = ::GetPrivateProfileSectionNames(pBuffer, dwSize, m_szFileName);

    if (dwRetBufSize > 0)
    {
        m_vctSectionNames.clear();
        CommonHelper::SplitString(m_vctSectionNames, pBuffer, dwRetBufSize * sizeof(TCHAR), L"\0");
        for each(wstring str in m_vctSectionNames)
        {
            EnumKeyValues(str.c_str());
        }

        isSucceed = TRUE;
    }

    SAFE_DELETE_ARRAY(pBuffer);

    return isSucceed;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::EnumKeyValues(IN LPCTSTR lpSectionName)
{
    DWORD dwSize = m_dwAllocSize / 2;
    TCHAR *pRetString = new TCHAR[dwSize];
    if (NULL == pRetString)
    {
        return FALSE;
    }
    
    ZeroMemory(pRetString, dwSize);
    BOOL isSucceed = FALSE;
    DWORD dwRetBufSize = GetPrivateProfileSection(lpSectionName, pRetString, dwSize, m_szFileName);

    if (dwRetBufSize > 0)
    {
        vector<wstring> strs;
        CommonHelper::SplitString(strs, pRetString, dwRetBufSize * sizeof(TCHAR), L"\0");

        for each(wstring str in strs)
        {
            wstring::size_type nIndex = str.find_first_of(L'=');

            if (nIndex != wstring::npos)
            {
                wstring strKey(lpSectionName);
                strKey.append(L"\\").append(str.substr(0, nIndex));
                wstring strValue(str.substr(nIndex + 1, str.length() - nIndex));
                m_mapKeyValues.insert(pair<wstring, wstring>(strKey, strValue));
            }
        }

        isSucceed = TRUE;
    }

    SAFE_DELETE_ARRAY(pRetString);

    return isSucceed;
}

//////////////////////////////////////////////////////////////////////////

BOOL IniConfig::GetIniFileSize(DWORD &dwordSize)
{
    BOOL isSucceed = FALSE;
    HANDLE hFile = CreateFile(m_szFileName,             // File name.
                              GENERIC_READ,             // Only open for reading.
                              0,                        // Do not share.
                              NULL,                     // No security.
                              OPEN_EXISTING,            // Opens the file, if it exits, otherwise failed.
                              FILE_ATTRIBUTE_NORMAL |   // Normal attributes.
                              FILE_FLAG_WRITE_THROUGH,  // Instructs the system to write through any intermediate cache and go directly to disk.
                              NULL                      // No template.
                              );

    if (ISVALIDHANDLE(hFile))
    {
        dwordSize = GetFileSize(hFile, NULL);
        isSucceed =TRUE;
    }
    else
    {
        DWORD dwError = GetLastError();
        if (dwError == 0)
        {
            m_isFileExist = FALSE;
        }
    }

    SAFE_CLOSE_HANDLE(hFile);

    return isSucceed;
}
