/*!
* @file CommonHelper.h 
* 
* @brief This file defines the CommonHelper class which supplies some static common functions.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#ifdef __cplusplus
#ifndef _COMMONHELPER_H_
#define _COMMONHELPER_H_

#include "Common.h"
#include "CommonMacro.h"

BEGIN_NAMESPACE_COMMON

class CLASS_DECLSPEC CommonHelper
{
public:

    /*!
    * @brief Extract file icon
    * 
    * @param pszPath    [I/ ] Specified file path
    * @param nWidth     [I/ ] The width of icon wanted to extract
    * @param nHeight    [I/ ] The height of icon wanted to extract
    * 
    * @return The handler of icon
    */
    static HBITMAP ExtractFileIcon(IN LPCTSTR pszPath, UINT nWidth, UINT nHeight);

    /*!
    * @brief Extract file thumbnail
    * 
    * @param pszPath        [I/ ] Specified file path
    * @param nWidth         [I/ ] The width of thumbnail wanted to extract
    * @param nHeight        [I/ ] The height of thumbnail wanted to extract
    *
    * @return The handler of thumbnail
    */
    static HBITMAP ExtractFileThumbnail(IN LPCTSTR pszPath, UINT nWidth, UINT nHeight);

    /*!
    * @brief Create a snapshot of specified window, the size of the snapshot is specified by rect argument.
    *
    * @param hWnd           [I/ ] The handle to HBITMAP to be as source of Gdiplus::Bitmap object.
    * @param lpRect         [I/ ] The pointer to RECT structure, indicates the rectangle of the snapshot. If this argument is NULL(default value),
    *                             The size of snapshot is same as the client size of window.
    *
    * @return The handle to the HBITMAP of snapshot. If the function fails, the returned value is NULL.
    *
    * @remark It is your responsibility to call DeleteObject to delete GID object if the returned HBITMAP is no longer needed.
    */
    static HBITMAP GetWindowSnapshot(IN HWND hWnd, IN const RECT* lpRect = NULL);

    /*!
    * @brief Set window to the center of screen.
    *
    * @param hwnd           [I/ ] The handler of window.
    * @param isCenterScreen [I/ ] Center in screen.
    *
    * @return TRUE is success, FALSE is failure.
    */
    static BOOL SetWindowCenter(HWND hwnd, BOOL isCenterScreen = FALSE);

    /*!
    * @brief Output the debug string which format is specified by pcsFormat arguments.
    *
    * @param pcsFormat      [I/ ] The format of the output string.
    * @param ...            [I/ ] The optional arguments.
    *
    * @remark The maximum length of output string is 1024
    */
    static void PrintDebugString(IN const WCHAR* pcsFormat, ...);

    /*!
    * @brief Called to translate system time to chars.
    *
    * @param time          [I/ ] The system time.
    * @param cBuffer       [ /O] The pointer to the output chars.
    * @param len           [ /O] The length of chars which you want to output.
    */
    static void TranslateSysTimeToChar(double time, TCHAR *cBuffer, INT32 len);

    /*!
    * @brief Get instance of current process, if this function is in DLL project,
    *        returned HMODULE is the instance of DLL.
    *
    * @return The handle to current process if succeeds, otherwise return NULL.
    */
    static HMODULE GetCurrentModuleHandle();

    /*!
    * @brief Retrieve current date time.
    *
    * @param psystemTime    [ /O] The pointer to SYSTEMTIME structure.
    *
    * @return If the function succeeds, return TURE, otherwise return FALSE.
    */
    static BOOL GetCurrentDateTime(OUT LPSYSTEMTIME psystemTime);

    /*!
    * @brief Convert wide character to upper.
    *
    * @param input          [I/ ] The wide character to be converted.
    *
    * @return The converted wide character.
    */
    static WCHAR ToUpperInvariant(WCHAR input);

    /*!
    * @brief Compare two string.
    *
    * @param sz1            [I/ ] The pointer to the first null-terminated string to compare.
    * @param sz2            [I/ ] The pointer to the second null-terminated string to compare.
    *
    * @return return 0 if two string is equal.
    */
    static INT32 OrdinalIgnoreCaseCompareStrings(LPCWSTR sz1, LPCWSTR sz2);

    /*!
    * @brief 
    * Get the current display scale.
    *
    * @return FLOAT the current display scale
    */
    static FLOAT GetScale();

    /*!
    * @brief This function is used to split string which separated with \0.
    *
    * @param splitStrings   [ /O] The string after split.
    * @param lpSrc          [I/ ] The source buffer to be split.
    * @param nBufSize       [I/ ] The source buffer size.
    * @param delimiters     [I/ ] Set of delimiter characters.
    *
    * @return Split string array.
    */
    static void SplitString(OUT vector<wstring>& splitStrings, IN LPCWSTR lpSrc, 
                            IN INT32 nBufSize, IN LPCWSTR delimiters);

    /*!
    * @brief Get the desktop path.
    *
    * @param pszFolderPath  [ /O] The path of desktop.
    * @param nCsidl         [I/ ] A CSIDL that identifies the folder of interest. If a virtual folder is specified, this function will fail.
    *                             nCsidl has prefix CSIDL_, such CSIDL_DESKTOP, CSIDL_PROGRAMS, CSIDL_RECENT, CSIDL_MYDOCUMENTS .etc.
    *
    * @return If succeed, return value is TRUE, otherwise is FALSE.
    */
    static BOOL GetKnownFolderPath(OUT LPWSTR pszFolderPath, IN UINT nCsidl);

    /*!
    * @brief Creates a new directory.
    *
    * @param lpPathName     [I/ ] The path of the directory to be created.
    *                             There is a default string size limit for paths of 248 characters.
    *
    * @return If the function succeeds, the return value is nonzero.
    *         If the function fails, the return value is zero.
    */
    static BOOL CreateFolder(IN LPCWSTR lpPathName);

    /*!
    * @brief Remove file.
    *
    * @param lpFilePath     [I/ ] The file path.
    * @param bToRecycleBin  [I/ ] The flag whether remove to recycle bin or not.
    *
    * @return If succeed, return value is TRUE, otherwise is FALSE.
    */
    static BOOL DeleteFile(IN LPCWSTR lpFilePath, IN BOOL bToRecycleBin = TRUE);

    /*!
    * @brief Delete these files which file name matches the specified prefix in specified folder path. excludes subfolder.
    *
    * @param lpFolderPath   [I/ ] The folder path, in which files are deleted.
    * @param lpPrefix       [I/ ] The prefix of the file name.
    *
    * @return TRUE if succeeds to finish operation, otherwise FALSE.
    */
    static BOOL DeleteFilesByPrefix(IN LPCTSTR lpFolderPath, LPCTSTR lpPrefix);

    /*!
    * @brief Convert file size(bytes) ti friendly text, such as XXX MB, XXX KB.
    *
    * @param ullSize        [I/ ] The file size.
    * @param lpszSizeText   [I/ ] The buffer to receive friendly size text.
    * @param uBufSize       [I/ ] The size of buffer.
    */
    static void ConverFileSizeToText(ULONGLONG ullSize, LPTSTR lpszSizeText, UINT32 uBufSize);

    /*!
    * @brief Sets the thread preferred UI languages for the current thread.
    *
    * @param langId         [I/ ] The language id of current thread, default value is 0, presents get user
    *                             default language..
    *
    * @return TRUE if the function succeeds or FALSE otherwise.
    */
    static BOOL InitCurrentLanguage(LANGID &langId);

    /*!
    * @brief Convert File time to system time and retrieve the string of date.
    *
    * @param fileTime       [I/ ] The file time.
    * @param pszSystemTime  [I/ ] The system date string, which format is YYYY/MM/DD/HH/MM.
    *
    * @return If succeed, return value is TRUE, otherwise is FALSE.
    */
    static BOOL FileTimeToLocalTime(IN const FILETIME& fileTime, OUT LPWSTR pszSystemTime);

    /*!
    * @brief Convert the specified string to lower case.
    *
    * @param lpSrcString    [I/ ] The pointer wchar_t*.
    *
    * @return The lower case wstring.
    */
    static wstring StringToLowerCase(IN LPCWSTR lpSrcString);
};

END_NAMESPACE

#endif // _COMMONHELPER_H_
#endif // __cplusplus
