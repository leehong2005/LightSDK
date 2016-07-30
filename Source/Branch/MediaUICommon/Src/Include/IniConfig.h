/*!
* @file IniConfig.h 
* 
* @brief This file defines IniConfig class and implements IConfig interface.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/11
*/

#ifdef __cplusplus
#ifndef _INICONFIG_H_
#define _INICONFIG_H_

#include "IConfig.h"
#include "CommonHelper.h"
#include "CommonMacro.h"

BEGIN_NAMESPACE_COMMON

class ConfigFactory;

/*!
* @brief IniConfig class implements IConfig class.
*/
class CLASS_DECLSPEC IniConfig : public IConfig
{
public:

    /*!
    * @brief The constructor function.
    */
    IniConfig(IN LPCTSTR lpFileName);

    /*!
    * @brief Get integer value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name, format is SectionName\KeyName
    * @param nRetValue      [ /O] The value.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL GetIntValue(IN LPCTSTR lpKeyName, OUT INT32& nRetValue);

    /*!
    * @brief Get boolean value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param bRetValue      [ /O] The value.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL GetBooleanValue(IN LPCTSTR lpKeyName, OUT BOOL& bRetValue);

    /*!
    * @brief Get string value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param lpRetValue     [ /O] The value.
    * @param lpRetValue     [I/ ] Size of lpRetValue buffer.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL GetStringValue(IN LPCTSTR lpKeyName, OUT LPTSTR lpRetValue, IN DWORD dwSize);

    /*!
    * @brief Set integer value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param nValue         [I/ ] The value.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL SetIntValue(IN LPCTSTR lpKeyName, IN INT32 nValue);

    /*!
    * @brief Set boolean value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param bValue         [I/ ] The value.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL SetBooleanValue(IN LPCTSTR lpKeyName, IN BOOL bValue);

    /*!
    * @brief Set string value by specified key name.
    *
    * @param lpKeyName      [I/ ] The key name.
    * @param lpValue        [I/ ] The value.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL SetStringValue(IN LPCTSTR lpKeyName, IN LPCTSTR lpValue);

    /*!
    * @brief Read data from file which given at constructor function.
    *
    * @return TRUE if succeeds, FALSE otherwise.
    */
    virtual BOOL ReadFromFile();

    /*!
    * @brief Save data to config file.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL SaveToFile();

    /*!
    * @brief Indicates the config is modified or not.
    *
    * @return TURE if modified, otherwise return FALSE.
    */
    virtual BOOL IsModified();

    /*!
    * @brief Indicates the key name whether exist or not.
    *
    * @param lpKeyName      [I/ ] The key name.
    *
    * @return TURE if exists, otherwise return FALSE.
    */
    virtual BOOL IsKeyNameExist(IN LPCTSTR lpKeyName);

    /*!
    * @brief Clear data stored in memory and reload data from config file.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL ReLoadData();

    /*!
    * @brief Get config file path.
    *
    * @param lpFilePath     [I/ ] The file path.
    * @param dwSize         [I/ ] The buffer size.
    *
    * @return TURE if succeeds, otherwise return FALSE.
    */
    virtual BOOL GetFilePath(OUT LPTSTR lpFilePath, IN DWORD dwSize);
    
    /*!
    * @brief Get the section.
    *
    * @return vector<wstring>*.
    */
    virtual const vector<wstring>* GetRecords();

protected:

    /*!
    * @brief The destructor function.
    */
    ~IniConfig();

    /*!
    * @brief Enumerate all sections.
    *
    * @return TRUE if succeed, FALSE otherwise.
    */
    BOOL EnumSections();

    /*!
    * @brief Enumerate all key-value pairs.
    *
    * @param lpSectionName  [I/ ] The section name.
    *
    * @return TRUE if succeed, FALSE otherwise.
    */
    BOOL EnumKeyValues(IN LPCTSTR lpSectionName);

    /*!
    * @brief Get Created INI file size.
    */
    BOOL GetIniFileSize(DWORD &dwordSize);

private:

    BOOL                    m_isModified;                   // Indicates the data is modified.
    TCHAR                   m_szFileName[MAX_PATH];         // Config file name.
    map<wstring, wstring>   m_mapKeyValues;                 // Key value pairs.
    vector<wstring>         m_vctSectionNames;              // Sec of section names;
    DWORD                   m_dwAllocSize;                  // The byte size of created INI file.
    BOOL                    m_isFileExist;                  // Indicates the file is exist.
};

END_NAMESPACE

#endif // _INICONFIG_H_
#endif // __cplusplus
