/*!
* @file Logger.h
* 
* @brief This file defines Logger class to wirte log information to files.
* 
* Copyright (C) 2009, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/09 12:00
*/

#ifdef __cplusplus
#ifndef _SDKLOG_H_
#define _SDKLOG_H_

#include "Common.h"
#include "CommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The log level.
*/
typedef enum _LOGLEVEL
{
    LOGLEVEL_DEBUG      = 0x0000,       // Debug
    LOGLEVEL_ERROR      = 0x0001,       // Error
    LOGLEVEL_WARNING    = 0x0002,       // Warning
    LOGLEVEL_INFO       = 0x0003,       // Information
    LOGLEVEL_FATAL      = 0x0004,       // Fatal

} LOGLEVEL;

#define DEBUG_LOG(message)  Logger::WriteLog(LOGLEVEL_DEBUG, message, __LINE__, __FILEW__, __FUNCTIONW__)
#define ERROR_LOG(message)  Logger::WriteLog(LOGLEVEL_ERROR, message, __LINE__, __FILEW__, __FUNCTIONW__)
#define FATAL_LOG(message)  Logger::WriteLog(LOGLEVEL_FATAL, message, __LINE__, __FILEW__, __FUNCTIONW__)

/*!
* @brief The Logger class, provides functions to write log to files.
*/
class CLASS_DECLSPEC Logger
{
public:

    /*!
    * @brief Initialize the Logger information, all operation for writing log should be after
    *        the calling of this function.
    *
    * @lpLogFolderPath      [I/ ] The log folder path, default is NULL, means using default path.
    *                             The default folder path is AppData\Roaming\TOSHIBA\TosMediaBrowser\Log.
    *
    * @return TRUE if succeeds to initialize, otherwise FALSE.
    */
    static BOOL Initialize(IN LPCTSTR lpLogFolderPath = NULL);

    /*!
    * @brief Uninitialize the Logger information, the calling of Initialize and Uninitialize should be matching.
    */
    static void Uninitialize();

private:

    /*!
    * @brief Write log message to file, if fails to initialize, this function takes no effect.
    *
    * @param logLevel           [I/ ] The log level. such as LOGLEVEL_DEBUG.
    * @param lpMessage          [I/ ] The written message.
    * @param dwCodeLine         [I/ ] The code line where call this method.
    * @param lpFileFullPath     [I/ ] The file full path..
    * @param lpFunctionName     [I/ ] The function name where call this method.
    */
    static void WriteLog(IN LOGLEVEL logLevel,
                         IN LPCTSTR lpMessage,
                         IN DWORD dwCodeLine = -1,
                         IN LPCTSTR lpFileFullPath = NULL,
                         IN LPCTSTR lpFunctionName = NULL);

    /*!
    * @brief The constructor function.
    *
    * @param lpLogFolderPath    [I/ ] The folder path.
    */
    Logger(IN LPCTSTR lpLogFolderPath);

    /*!
    * @brief The destructor function.
    */
    ~Logger();

    /*!
    * @brief Call this function to create log file.
    *
    * @return TURE if succeeds, FALSE otherwise.
    */
    BOOL CreateLogFile();

private:

    BOOL         m_isInitSucceed;                    // Succeed to initialize.
    HANDLE       m_hLogMutex;                        // The handle to mutex.
    HANDLE       m_hLogFile;                         // The handle to log file.
    TCHAR        m_szLogFolderPath[MAX_PATH];        // The log folder path.
    static Logger *s_pLogger;                        // The instance of logger.
};

END_NAMESPACE

#endif // _SDKLOG_H_
#endif // __cplusplus
