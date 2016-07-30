/* 
* @file SdkSearchRecentUrl.h
* 
* @brief This file defines class SdkSearchRecentUrl using to search file from registry.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Zhou Yuan Qi.
* @date 2011/02/24.
*/

#ifdef __cplusplus
#ifndef _SDKSEARCHRECENTURL_H_
#define _SDKSEARCHRECENTURL_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

/*!
* @brief The SdkSearchRecentUrl class is used to search files from Registry.
*/
class CLASS_DECLSPEC SdkSearchRecentUrl
{
public:

    /*!
    * @brief The constructor functions.
    */
    SdkSearchRecentUrl();

    /*!
    * @brief The destructor functions.
    */
    ~SdkSearchRecentUrl();

    /*!
    * @brief Begin to read the url from registry.
    */
    void EnumRecentUrls();

    /*!
    * @brief Get search result.
    *
    * @return The result list.
    */
    const vector<wstring>* GetRecentlyUrls() const;

private:

    /*!
    * @brief Query information about the specified registry key.
    *
    * @param hKey       [I/ ] A handle to an open registry key.
    */
    void QueryInfoKey(IN HKEY hKey);

    /*!
    * @brief Enumerate and store search result from registry.
    *
    * @param number      [I/ ] The result count.
    * @param hKey        [I/ ] A handle to an open registry key.
    */
    void EnumUrlValues(IN DWORD number,IN HKEY hKey);

private:

    vector<wstring>  m_searchUrls;              // The vector to store the results.
};

END_NAMESPACE_UTILITIES

#endif // _SDKSEARCHRECENTURL_H_
#endif // __cplusplus
