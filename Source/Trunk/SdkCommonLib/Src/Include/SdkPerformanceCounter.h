/*!
* @file PerformanceWatcher.h
* 
* @brief The file defines functions to get the elapsed time which a section of code is performanced.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/06
*/

#ifdef __cplusplus
#ifndef _SDKPERFORMANCECOUNTER_H_
#define _SDKPERFORMANCECOUNTER_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

class CLASS_DECLSPEC SdkPerformanceCounter
{
public:

    /*!
    * @brief This function should be called at the beginning of a section of code to be timed.
    *
    * @return return TRUE if succeed to start timer, otherwise return FALSE.
    */
    static BOOL Start();

    /*!
    * @brief This function should be called at the end of a section of code to be timed.
    *
    * @return return TRUE if succeed to start timer, otherwise return FALSE.
    */
    static BOOL Stop();

    /*!
    * @brief Start to time.
    */
    static void StartTiming();

    /*!
    * @brief End to time.
    *
    * @param strFPS         [I/O] The buffer to receive text.
    *
    * @return Returned value is FPS.
    */
    static DOUBLE EndTiming(OUT wstring *lpStrFPS = NULL);

    /*!
    * @brief Get the elapsed time which is took by timed the section code. This function calling will automatically call Stop function 
    *        to stop the timer.
    *
    * @return The value of elapsed time, the unit is millisecond(ms).
    */
    static DWORD GetElapsedTime();

    /*!
    * @brief This function will output the elapsed time on the debug mode. This function calling will automatically call Stop function 
    *        to stop the timer.
    *
    * @param lpMessage      [I/ ] The message to be displayed. The elapsed time will be appended at the end of string specified by lpMessage.
    *
    * @remark
    * The default string output format is "%s: %d ms".
    * If the application has no debugger, the system debugger displays the string.
    * If the application has no debugger and the system debugger is not active, OutputPerformanceTime does nothing.
    */
    static void OutputPerformanceTime(IN LPCTSTR lpMessage);

private:

    /*!
    * @brief This function can set all members of LARGE_INTEGER to zero.
    *
    * @param pLargerInteger [ /O] The pointer to LARGE_INTEGER which members will be set to zero.
    */
    static void ZeroLargeInteger(OUT LARGE_INTEGER* pLargerInteger);

private:

    static WCHAR g_szFPSMsg[128];
    static double g_nTime;
    static double g_dFPS;
    static INT64 g_nStartTime;
    static INT64 g_nEndTime;
    static INT64 g_nFrequency;

    static LARGE_INTEGER m_nFrequency;           // The frequency of high-resolution performance.
    static LARGE_INTEGER m_nStartCounter;        // The start counter of high-resolution performance.
    static LARGE_INTEGER m_nEndCounter;          // The end counter of high-resolution performance.
};

END_NAMESPACE_UTILITIES

#endif // _SDKPERFORMANCECOUNTER_H_
#endif // __cplusplus
