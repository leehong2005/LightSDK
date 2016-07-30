/*!
* @file CommandLineParser.h
* 
* @brief This module privodes functions to parse command line and output some states.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/24
*/

#ifdef __cplusplus
#ifndef _COMMANDLINEPARSER_H_
#define _COMMANDLINEPARSER_H_

#include "Common.h"
#include "CommonHelper.h"
#include "CommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The enumeration for application startup command.
*/
typedef enum _APPSTARTUPCOMMAND
{
    CMD_NONE            = 0x0000,           // No command
    CMD_SHOW            = 0x0001,           // Show application.
    CMD_HIDE            = 0x0002,           // Hide application.
    CMD_HOME            = 0x0004,           // Return home.
    CMD_EXIT            = 0x0008,           // Close application.
    CMD_RELOAD          = 0x0010,           // Reload data.
    CMD_ALTERNATE       = 0x0020,           // Alternate.

} APPSTARTUPCOMMAND;

/*!
* @brief The strut for command string and its corresponding command value.
*/
typedef struct _APPSTARTUPCOMMANDPAIR
{
    TCHAR               lpCmd[100];         // Command string.
    APPSTARTUPCOMMAND   cmdValue;           // The command value corresponding command string.

} APPSTARTUPCOMMANDPAIR, *LPAPPSTARTUPCOMMANDPAIR;


/*!
* @brief The CommandLineParser class can parse command line of application.
*/
class CLASS_DECLSPEC CommandLineParser
{
public:

    static void SetInitialCommandValues(LPAPPSTARTUPCOMMANDPAIR lpszCmdPairs, UINT32 uArraySize);

    /*!
    * @brief Parser command line and get result. This function will get command from system automatically.
    *
    * @return Parser result. This value may be one or combined of APPSTARTUPCOMMAND enumeration.
    */
    static DWORD Parser();

    /*!
    * @brief Get the command state value.
    *
    * @return Parser result. This value may be one or combined of APPSTARTUPCOMMAND enumeration.
    */
    static DWORD GetCommand();

protected:

    /*!
    * @brief Get command from specified string.
    *
    * @return One of APPSTARTUPCOMMAND value.
    */
    static APPSTARTUPCOMMAND GetCommandFromString(IN LPCTSTR lpCmd);

private:

    static DWORD        s_dwCmdPairsCount;  // The command pairs count.
    static DWORD        s_dwCommand;        // The command state.
};

END_NAMESPACE

#endif // _COMMANDLINEPARSER_H_
#endif // __cplusplus
