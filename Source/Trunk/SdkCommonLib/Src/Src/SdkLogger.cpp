/*!
* @file SdkLogger.cpp
* 
* @brief This file defines SdkLogger class to wirte log information to files.
* 
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/09 12:00
*/

#include "stdafx.h"
#include "SdkLogger.h"
#include "SdkCommonHelper.h"

USING_NAMESPACE_COMMON
USING_NAMESPACE_UTILITIES

#define LOGFILE_DIRECOTRY               L"\\LightSDK\\Log"
#define LOGMUTEX_NAME                   L"{98979242-D46B-4a2c-B15C-C86AE4345AD9}"
#define WAIT_TIME                       2000

static const WCHAR* LogLevels[] = 
{
    L"DEBUG",
    L"ERROR",
    L"WARNING",
    L"INFO",
    L"FATAL",
};

SdkLogger* SdkLogger::s_pLogger = NULL;

//////////////////////////////////////////////////////////////////////////

BOOL SdkLogger::Initialize(IN LPCTSTR lpLogFolderPath)
{
    BOOL isSuccess = FALSE;

    if (NULL == s_pLogger)
    {
        s_pLogger = new SdkLogger(lpLogFolderPath);
        isSuccess = s_pLogger->m_isInitSucceed;
    }

    return isSuccess;
}

//////////////////////////////////////////////////////////////////////////

void SdkLogger::Uninitialize()
{
    SAFE_DELETE(s_pLogger);
}

//////////////////////////////////////////////////////////////////////////

void SdkLogger::WriteLog(
    IN LOGLEVEL logLevel,
    IN LPCTSTR lpMessage,
    IN DWORD dwCodeLine,
    IN LPCTSTR lpFileFullPath,
    IN LPCTSTR lpFunctionName)
{
    if (NULL == s_pLogger || !s_pLogger->m_isInitSucceed)
    {
        return;
    }

    if (NULL == s_pLogger->m_hLogFile)
    {
        if (!s_pLogger->CreateLogFile())
        {
            return;
        }
    }

    lpMessage      = (NULL == lpMessage) ? _T("") : lpMessage;
    lpFileFullPath = (NULL == lpFileFullPath) ? _T("") : lpFileFullPath;
    lpFunctionName = (NULL == lpFunctionName) ? _T("") : lpFunctionName;

    SYSTEMTIME systemTime = { 0 };
    SdkCommonHelper::GetCurrentDateTime(&systemTime);

    INT32 nTotalLen = (wcslen(lpMessage) + wcslen(lpFileFullPath) + wcslen(lpFunctionName) + 1024);
    TCHAR *pMessage = new TCHAR[nTotalLen];
    ZeroMemory(pMessage, nTotalLen * sizeof(TCHAR));

    swprintf_s(
        pMessage,
        nTotalLen,
        L"%04d/%02d/%02d [%02d:%02d:%02d] %s [%s %s %d] [%s] \r\n",
        systemTime.wYear,
        systemTime.wMonth,
        systemTime.wDay,
        systemTime.wHour,
        systemTime.wMinute,
        systemTime.wSecond,
        LogLevels[(INT32)logLevel],
        PathFindFileName(lpFileFullPath),
        lpFunctionName,
        dwCodeLine,
        lpMessage);

    nTotalLen = wcslen(pMessage);
    HANDLE hFile = s_pLogger->m_hLogFile;
    if ( ISVALIDHANDLE(hFile) )
    {
        WaitForSingleObject(s_pLogger->m_hLogMutex, WAIT_TIME);

        try
        {
            DWORD dwWritten = 0;
            WriteFile(hFile, pMessage, nTotalLen * sizeof(TCHAR), &dwWritten, NULL);
        }
        catch (...)
        {
        }

        ReleaseMutex(s_pLogger->m_hLogMutex);
    }

    SAFE_DELETE_ARRAY(pMessage);
}

//////////////////////////////////////////////////////////////////////////

void SdkLogger::WriteLog(
    IN LOGLEVEL logLevel,
    IN const LogString& strMessage,
    IN DWORD dwCodeLine,
    IN LPCTSTR lpFileFullPath,
    IN LPCTSTR lpFunctionName)
{
    WriteLog(logLevel, strMessage.c_str(), dwCodeLine, lpFileFullPath, lpFunctionName);
}

//////////////////////////////////////////////////////////////////////////

SdkLogger::SdkLogger(IN LPCTSTR lpLogFolderPath) : m_isInitSucceed(FALSE),
                                                   m_hLogMutex(NULL),
                                                   m_hLogFile(NULL)
{
    ZeroMemory(m_szLogFolderPath, MAX_PATH);

    if (NULL != lpLogFolderPath)
    {
        wcscpy_s(m_szLogFolderPath, MAX_PATH, lpLogFolderPath);
        m_isInitSucceed = SdkCommonHelper::CreateFolder(lpLogFolderPath);
    }
    else
    {
        SdkCommonHelper::GetKnownFolderPath(m_szLogFolderPath, CSIDL_APPDATA);
        PathAppend(m_szLogFolderPath, LOGFILE_DIRECOTRY);
        m_isInitSucceed = SdkCommonHelper::CreateFolder(m_szLogFolderPath);
    }

    if (m_isInitSucceed)
    {
        m_hLogMutex = CreateMutex(NULL, FALSE, LOGMUTEX_NAME);
        //m_isInitSucceed = CreateLogFile();
    }
}

//////////////////////////////////////////////////////////////////////////

SdkLogger::~SdkLogger()
{
    ReleaseMutex(m_hLogMutex);

    SAFE_CLOSE_HANDLE(m_hLogMutex);
    SAFE_CLOSE_HANDLE(m_hLogFile);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkLogger::CreateLogFile()
{
    // Get the log file name, which is formed by LOG_YYYYMMDD_HHMMSS.log.
    TCHAR szLogFileName[MAX_PATH] = { 0 };
    SYSTEMTIME systemTime = { 0 };
    SdkCommonHelper::GetCurrentDateTime(&systemTime);
    swprintf_s(szLogFileName, L"%s\\LOG_%04d%02d%02d_%02d%02d%02d.log",
               m_szLogFolderPath,
               systemTime.wYear, systemTime.wMonth, systemTime.wDay,
               systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

    // Create the log file.
    m_hLogFile = CreateFile(szLogFileName,
                            GENERIC_WRITE,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

    if ( ISVALIDHANDLE(m_hLogFile) )
    {
        // The encode of log file is unicode format.
        DWORD dwWritten = 0;
        WCHAR datas = { 0xFEFF };
        WriteFile(m_hLogFile, &datas, sizeof(WCHAR), &dwWritten, NULL);
    }

    return ISVALIDHANDLE(m_hLogFile);
}
