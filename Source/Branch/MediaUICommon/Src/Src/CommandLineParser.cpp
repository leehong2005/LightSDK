/*!
* @file CommandLineParser.cpp 
* 
* @brief This module provides functions to parse command line and output some states.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/24
*/

#include "CommandLineParser.h"

USING_NAMESPACE_COMMON

#define CMDPAIRS_MAXCOUNT      20           // The max count of command pairs.

APPSTARTUPCOMMANDPAIR g_szCmdPairs[CMDPAIRS_MAXCOUNT] =
{
    { _T("/hide"),      CMD_HIDE },
    { _T("/show"),      CMD_SHOW },
    { _T("/exit"),      CMD_EXIT },
    { _T("/home"),      CMD_HOME },
    { _T("/alternate"), CMD_ALTERNATE },
};

DWORD CommandLineParser::s_dwCmdPairsCount  = 5;
DWORD CommandLineParser::s_dwCommand        = CMD_NONE;

//////////////////////////////////////////////////////////////////////////

void CommandLineParser::SetInitialCommandValues(LPAPPSTARTUPCOMMANDPAIR lpszCmdPairs, UINT32 uArraySize)
{
    if (NULL != lpszCmdPairs && uArraySize > 0 && uArraySize <= CMDPAIRS_MAXCOUNT)
    {
        s_dwCmdPairsCount = (DWORD)uArraySize;
        int arrSize = ARRAYSIZE(g_szCmdPairs) * sizeof(APPSTARTUPCOMMANDPAIR);
        ZeroMemory(g_szCmdPairs, arrSize);
        memcpy_s(g_szCmdPairs, arrSize, lpszCmdPairs, uArraySize * sizeof(APPSTARTUPCOMMANDPAIR));
    }
}

//////////////////////////////////////////////////////////////////////////

DWORD CommandLineParser::Parser()
{
    int args = 0;
    s_dwCommand = CMD_NONE;
    LPTSTR lpCmdLine = GetCommandLine();
    LPWSTR *pArgv = CommandLineToArgvW(lpCmdLine, &args);
    if ( (NULL != pArgv) && args > 1 )
    {
        for (int i = 1; i < args; ++i)
        {
            APPSTARTUPCOMMAND cmdValue = GetCommandFromString(pArgv[i]);
            if (CMD_NONE != cmdValue)
            {
                s_dwCommand |= cmdValue;
            }
        }
        LocalFree(pArgv);
    }

    return s_dwCommand;
}

//////////////////////////////////////////////////////////////////////////

DWORD CommandLineParser::GetCommand()
{
    return s_dwCommand;
}

//////////////////////////////////////////////////////////////////////////

APPSTARTUPCOMMAND CommandLineParser::GetCommandFromString(IN LPCTSTR lpCmd)
{
    for (int i = 0; i < (int)s_dwCmdPairsCount; ++i)
    {
        if (0 == CommonHelper::OrdinalIgnoreCaseCompareStrings(g_szCmdPairs[i].lpCmd, lpCmd))
        {
            return g_szCmdPairs[i].cmdValue;
        }
    }

    return CMD_NONE;
}
