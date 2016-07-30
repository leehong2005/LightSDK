/*!
* @file IConfigUtil.h 
* 
* @brief This file defines the common behaviors to get data from config files.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/11
*/

#ifdef __cplusplus
#ifndef _ICONFIGUTIL_H_
#define _ICONFIGUTIL_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief IConfigUtil class to defines behaviors to manage config files.
*/
class CLASS_DECLSPEC IConfigUtil
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IConfigUtil() {}

    /*!
    * @brief Get integer value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param nRetValue      [ /O] The value.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL GetIntValue(IN LPCTSTR lpKeyName, OUT INT32& nRetValue) = 0;

    /*!
    * @brief Get boolean value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param bRetValue      [ /O] The value.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL GetBooleanValue(IN LPCTSTR lpKeyName, OUT BOOL& bRetValue) = 0;

    /*!
    * @brief Get string value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param lpRetValue     [ /O] The value.
    * @param lpRetValue     [I/ ] Size of lpRetValue buffer.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL GetStringValue(IN LPCTSTR lpKeyName, OUT LPTSTR lpRetValue, IN DWORD dwSize) = 0;

    /*!
    * @brief Set integer value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param nValue         [I/ ] The value.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL SetIntValue(IN LPCTSTR lpKeyName, IN INT32 nValue) = 0;

    /*!
    * @brief Set boolean value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param bValue         [I/ ] The value.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL SetBooleanValue(IN LPCTSTR lpKeyName, IN BOOL bValue) = 0;

    /*!
    * @brief Set string value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param lpValue        [I/ ] The value.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL SetStringValue(IN LPCTSTR lpKeyName, IN LPCTSTR lpValue) = 0;

    /*!
    * @brief Read data from file which given at constructor function.
    *
    * @return TRUE if succeeds, FALSE otherwise.
    */
    virtual BOOL ReadFromFile() = 0;

    /*!
    * @brief Save data to config file.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL SaveToFile() = 0;

    /*!
    * @brief Indicates the config is modified or not.
    *
    * @return TURE if modified, otherwise return FALSE.
    */
    virtual BOOL IsModified() = 0;

    /*!
    * @brief Indicates the key name whether exist or not.
    *
    * @param lpKeyName      [I/ ] The key name.
    *
    * @return TURE if exists, otherwise return FALSE.
    */
    virtual BOOL IsKeyNameExist(IN LPCTSTR lpKeyName) = 0;

    /*!
    * @brief Clear data stored in memory and reload data from config file.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL ReLoadData() = 0;

    /*!
    * @brief Get config file path.
    *
    * @param lpFilePath     [I/ ] The file path.
    * @param dwSize         [I/ ] The buffer size.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL GetFilePath(OUT LPTSTR lpFilePath, IN DWORD dwSize) = 0;

    /*!
    * @brief Get all file name of _ini.

    * @return a vector with all records.
    */
    virtual const vector<wstring>* GetRecords() = 0;
};

END_NAMESPACE_COMMON

#endif // _ICONFIGUTIL_H_
#endif // __cplusplus