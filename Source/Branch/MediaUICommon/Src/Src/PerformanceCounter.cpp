/*!
* @file PerformanceWatcher.cpp
* 
* @brief PerformanceWatcher.cpp
* The file defines functions to get the elapsed time which a section of code is performanced.
* 
* Copyright (C) 2009, Toshiba Corporation.
* 
* @author Li Hong
* @date 2009/11/16 16:00
*/

#include "PerformanceCounter.h"
#include "CommonHelper.h"

USING_NAMESPACE_COMMON

LARGE_INTEGER PerformanceCounter::m_nFrequency      = { 0 };
LARGE_INTEGER PerformanceCounter::m_nStartCounter   = { 0 };
LARGE_INTEGER PerformanceCounter::m_nEndCounter     = { 0 };

WCHAR g_szFPSMsg[128] = { 0 };
double g_nTime = 0;
double g_dFPS = 0;
INT64 g_nStartTime = 0;
INT64 g_nEndTime = 0;
INT64 g_nFrequency = 0;

//////////////////////////////////////////////////////////////////////////

BOOL PerformanceCounter::Start()
{
    return QueryPerformanceCounter(&m_nStartCounter);
}

//////////////////////////////////////////////////////////////////////////

BOOL PerformanceCounter::Stop()
{
    return QueryPerformanceCounter(&m_nEndCounter);
}

//////////////////////////////////////////////////////////////////////////

void PerformanceCounter::StartTiming()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&g_nFrequency);
    QueryPerformanceCounter((LARGE_INTEGER*)&g_nStartTime);
}

//////////////////////////////////////////////////////////////////////////

DOUBLE PerformanceCounter::EndTiming(OUT wstring *lpStrFPS)
{
    QueryPerformanceCounter((LARGE_INTEGER*)&g_nEndTime);
    g_nTime = (double)(g_nEndTime - g_nStartTime) / (double)(g_nFrequency);
    g_dFPS = (double)(g_nFrequency) / (double)(g_nEndTime-g_nStartTime);

    if (NULL != lpStrFPS)
    {
        swprintf_s(g_szFPSMsg, ARRAYSIZE(g_szFPSMsg), L"FPS: %f", g_dFPS);
        lpStrFPS->clear();
        lpStrFPS->append(g_szFPSMsg);
    }

    return g_dFPS;
}

//////////////////////////////////////////////////////////////////////////

DWORD PerformanceCounter::GetElapsedTime()
{
    // Not call Start function
    if ( 0 == m_nStartCounter.QuadPart )
    {
        return 0;
    }

    // Not call Stop function.
    if ( 0 == m_nEndCounter.QuadPart )
    {
        Stop();
    }
    
    LONGLONG nElapsedTime = 0;
    BOOL bSuccess = QueryPerformanceFrequency(&m_nFrequency);
    if ( TRUE == bSuccess )
    {
        nElapsedTime = ((m_nEndCounter.QuadPart - m_nStartCounter.QuadPart) * LONGLONG(1000)) / m_nFrequency.QuadPart;
    }

    ZeroLargeInteger(&m_nEndCounter);
    return (DWORD)nElapsedTime;
}

//////////////////////////////////////////////////////////////////////////

void PerformanceCounter::OutputPerformanceTime(IN LPCTSTR lpMessage)
{
    DWORD dwElapsedTime = GetElapsedTime();
    if ( NULL != lpMessage )
    {
        CommonHelper::PrintDebugString(_T("%s: %d ms\n"), lpMessage, dwElapsedTime);
    }
}

//////////////////////////////////////////////////////////////////////////

void PerformanceCounter::ZeroLargeInteger(OUT LARGE_INTEGER* pLargerInteger)
{
    if ( NULL != pLargerInteger )
    {
        pLargerInteger->HighPart = 0;
        pLargerInteger->LowPart = 0;
        pLargerInteger->QuadPart = 0;
        pLargerInteger->u.HighPart = 0;
        pLargerInteger->u.LowPart = 0;
    }
}