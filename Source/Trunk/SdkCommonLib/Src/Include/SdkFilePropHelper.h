/*!
* @file SdkFilePropHelper.h
* 
* @brief This file define media content functions to retrieve various properties from media file.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#ifdef __cplusplus
#ifndef _SDKFILEPROPHELPER_H_
#define _SDKFILEPROPHELPER_H_

#include "SdkCommon.h"
#include "SdkCommonHelper.h"
#include "SdkCommonMacro.h"
#include "SdkFilePropKey.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The MediaContent can retrieve properties from a media file, each object of
*        this class represent a media file.
*/
class CLASS_DECLSPEC SdkFilePropHelper
{
public:

    /*!
    * @brief The constructor function.
    *
    * @param lpFileName     [I/ ] The file path.
    */
    SdkFilePropHelper(IN LPCWSTR lpFileName);

    /*!
    * @brief The copy constructor function.
    */
    SdkFilePropHelper(const SdkFilePropHelper& src);

    /*!
    * @brief The equal operator function.
    */
    SdkFilePropHelper& operator = (const SdkFilePropHelper& src);

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkFilePropHelper();

    /*!
    * @brief Set the file name.
    *
    * @lpFileName       [I/ ] The file name.
    *
    * @return TRUE if succeeds, FALSE otherwise.
    */
    BOOL SetFileName(IN LPCWSTR lpFileName);

    /*!
    * @brief Get the file name which is passed in SetFileName or MediaContent constructor.
    *
    * @param lpszFileNaem   [I/ ] The buffer to receive the file name.
    * @param uSize          [I/ ] The size of the buffer.
    */
    BOOL GetFileName(OUT LPWSTR lpszFileName, UINT32 uSize);

    /*!
    * @brief Gets a formatted, Unicode string representation of a property value.
    *
    * @param lpszStr    [ /O] The retrieved value.
    * @param uSize      [I/ ] The retrieved value.
    * @param pdfFlags   [I/ ] The format of the property string.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL FormatForDisplay(IN PROPERTYKEYNAME keyName, OUT LPWSTR lpszStr, IN UINT32 uSize, PROPDESC_FORMAT_FLAGS pdfFlags = PDFF_DEFAULT);

    /*!
    * @brief Get string value from the media file by specified key name.
    *
    * @param str        [ /O] The retrieved value.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetString(IN PROPERTYKEYNAME keyName, OUT wstring &str);

    /*!
    * @ brief Get string value from the media file by specified key name.
    *
    * @param lpszStr    [ /O] The retrieved value.
    * @param uSize      [I/ ] The retrieved value.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetString(IN PROPERTYKEYNAME keyName, OUT LPWSTR lpszStr, IN UINT32 uSize = MAX_PATH);

    /*!
    * @brief Get BOOL value from the media file by specified key name.
    *
    * @param pf         [ /O] The retrieved value.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetBOOL(IN PROPERTYKEYNAME keyName, OUT BOOL *pf);

    /*!
    * @brief Get INT32 value from the media file by specified key name.
    *
    * @param pi         [ /O] The retrieved value.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetInt32(IN PROPERTYKEYNAME keyName, OUT INT32 *pi);

    /*!
    * @brief Get UINT32 value from the media file by specified key name.
    *
    * @param pui        [ /O] The retrieved value.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetUInt32(IN PROPERTYKEYNAME keyName, OUT ULONG *pui);

    /*!
    * @brief Get UINT64 value from the media file by specified key name.
    *
    * @param pull       [ /O] The retrieved value.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetUInt64(IN PROPERTYKEYNAME keyName, OUT ULONGLONG  *pull);

    /*!
    * @brief Get DateTime value from the media file by specified key name.
    *
    * @param pft        [ /O] The retrieved value.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetDateTime(IN PROPERTYKEYNAME keyName, OUT FILETIME *pft);

private:

    WCHAR                       m_szItemPath[MAX_PATH];     // The file path of item.
    IShellItem2                *m_pShellItem2;              // The pointer to IShellItem2 which represent a file item.
    IPropertySystem            *m_pPropSystem;              // The property system.
};

END_NAMESPACE_COMMON

#endif // _SDKFILEPROPHELPER_H_
#endif // __cplusplus
