/*!
* @file Logger.cpp
* 
* @brief This file defines Logger class to wirte log information to files.
* 
* Copyright (C) 2009, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/09 12:00
*/

#include "Logger.h"
#include "CommonHelper.h"

USING_NAMESPACE_COMMON

#define LOGFILE_DIRECOTRY               L"\\TOSHIBA\\TosMediaBrowser\\Log"
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

Logger* Logger::s_pLogger = NULL;

//////////////////////////////////////////////////////////////////////////

BOOL Logger::Initialize(IN LPCTSTR lpLogFolderPath)
{
    BOOL isSuccess = FALSE;

    if (NULL == s_pLogger)
    {
        s_pLogger = new Logger(lpLogFolderPath);
        isSuccess = s_pLogger->m_isInitSucceed;
    }

    return isSuccess;
}

//////////////////////////////////////////////////////////////////////////

void Logger::Uninitialize()
{
    SAFE_DELETE(s_pLogger);
}

//////////////////////////////////////////////////////////////////////////

void Logger::WriteLog(IN LOGLEVEL logLevel,
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
    CommonHelper::GetCurrentDateTime(&systemTime);

    INT32 nTotalLen = (wcslen(lpMessage) + wcslen(lpFileFullPath) + wcslen(lpFunctionName) + 1024);
    TCHAR *pMessage = new TCHAR[nTotalLen];
    ZeroMemory(pMessage, nTotalLen * sizeof(TCHAR));

    swprintf_s(pMessage, nTotalLen, L"%04d/%02d/%02d [%02d:%02d:%02d] %s [%s %s %d] [%s] \r\n",
               systemTime.wYear, systemTime.wMonth, systemTime.wDay,
               systemTime.wHour, systemTime.wMinute, systemTime.wSecond,
               LogLevels[(INT32)logLevel],
               PathFindFileName(lpFileFullPath), lpFunctionName, dwCodeLine, lpMessage);

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

Logger::Logger(IN LPCTSTR lpLogFolderPath) : m_isInitSucceed(FALSE),
                                             m_hLogMutex(NULL),
                                             m_hLogFile(NULL)
{
    ZeroMemory(m_szLogFolderPath, MAX_PATH);

    if (NULL != lpLogFolderPath)
    {
        wcscpy_s(m_szLogFolderPath, MAX_PATH, lpLogFolderPath);
        m_isInitSucceed = CommonHelper::CreateFolder(lpLogFolderPath);
    }
    else
    {
        CommonHelper::GetKnownFolderPath(m_szLogFolderPath, CSIDL_APPDATA);
        PathAppend(m_szLogFolderPath, LOGFILE_DIRECOTRY);
        m_isInitSucceed = CommonHelper::CreateFolder(m_szLogFolderPath);
    }

    if (m_isInitSucceed)
    {
        m_hLogMutex = CreateMutex(NULL, FALSE, LOGMUTEX_NAME);
        //m_isInitSucceed = CreateLogFile();
    }
}

//////////////////////////////////////////////////////////////////////////

Logger::~Logger()
{
    ReleaseMutex(m_hLogMutex);

    SAFE_CLOSE_HANDLE(m_hLogMutex);
    SAFE_CLOSE_HANDLE(m_hLogFile);
}

//////////////////////////////////////////////////////////////////////////

BOOL Logger::CreateLogFile()
{
    // Get the log file name, which is formed by LOG_YYYYMMDD_HHMMSS.log.
    TCHAR szLogFileName[MAX_PATH] = { 0 };
    SYSTEMTIME systemTime = { 0 };
    CommonHelper::GetCurrentDateTime(&systemTime);
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
