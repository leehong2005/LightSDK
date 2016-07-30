/*!
* @file SdkQueryUrlHistory.h 
* 
* @brief This file defines SdkQueryUrlHistory class to query url history of internet explorer.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/09
*/

#ifdef __cplusplus
#ifndef _SDKQUERYURLHISTORY_H_
#define _SDKQUERYURLHISTORY_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"
#include <urlhist.h>

BEGIN_NAMESPACE_COMMON

/*!
* @brief The SdkQueryUrlHistory class to query url history.
*/
class CLASS_DECLSPEC SdkQueryUrlHistory
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkQueryUrlHistory();

    /*!
    * @brief The destructor function.
    */
    ~SdkQueryUrlHistory();

    /*!
    * @brief Add a url to internet explorer history.
    *
    * @param pocsUrl    [I/ ] The url to add.
    * @param pocsTitle  [I/ ] The title of url.
    *
    * @return Returns TRUE if successful, or an error value otherwise.
    */
    BOOL AddUrl(LPCOLESTR pocsUrl, LPCOLESTR pocsTitle);

    /*!
    * @brief Deletes all instances of the specified URL from the Microsoft Internet Explorer history.
    *
    * @param pocsUrl    [I/ ] The URL to delete.
    *
    * @return Returns TRUE if successful, or an error value otherwise.
    */
    BOOL DeleteUrl(LPCOLESTR pocsUrl);

    /*!
    * @brief Get the next url history.
    *
    * @param statUrl    [ /O] The reference to STATURL to receive url information.
    *
    * @return TRUE if has next item, otherwise false.
    */
    BOOL Next(OUT STATURL &statUrl);

    /*!
    * @brief Get the urls count.
    *
    * @return The count of urls.
    */
    UINT GetCount();

    /*!
    * @brief Reset the cursor of url index.
    */
    void Reset();

    /*!
    * @brief Enumerate the url history.
    *
    * @brief uUrlsNum       [I/ ] The number of urls to enumerate, default value is 100.
    */
    void EnumUrlHistory(UINT uUrlsNum = 100);

private:

    /*!
    * @brief Initialize the COM interfaces.
    *
    * @return TRUE if succeeds, FALSE otherwise.
    */
    BOOL Initialize();

    /*!
    * @brief Clear the search urls and release allocated memory.
    */
    void ClearSearchUrls();

    /*!
    * @brief Find the index of earliest url item in urls list.
    *
    * @return The index of earliest urls.
    */
    UINT FindEarliestUrl();

    /*!
    * @brief Check the STATURL is valid or invalid, The title and url of a valid STATURL must not be empty.
    *
    * @param pstatUrl       [I/ ] The pointer to STATURL structure.
    *
    * @return TRUE if valid, FALSE if invalid.
    */
    BOOL IsValidSTATURL(const STATURL *pstatUrl);

    /*!
    * @brief Compares two url last visited time.
    *
    * @param firstItem      [I/ ] The reference to STATURL structure.
    * @param secondItem     [I/ ] The reference to STATURL structure.
    *
    * @return TRUE if first is lesser that second, otherwise FALSE.
    */
    static BOOL STATURLLesser(const STATURL &firstItem, const STATURL &secondItem);

    /*!
    * @brief Compares two url last visited time.
    *
    * @param firstItem      [I/ ] The reference to STATURL structure.
    * @param secondItem     [I/ ] The reference to STATURL structure.
    *
    * @return TRUE if first is greater that second, otherwise FALSE.
    */
    static BOOL STATURLGreater(const STATURL &firstItem, const STATURL &secondItem);

private:

    UINT32               m_uCursorIndex;        // The cursor index of urls list.
    IUrlHistoryStg2     *m_pUrlHistoryStg2;     // The IUrlHistoryStg2 instance.
    IEnumSTATURL        *m_pEnumStatUrl;        // The IEnumSTATURL instance.
    vector<STATURL>      m_vctSearchUrls;       // The search urls.
};

END_NAMESPACE_COMMON

#endif // _SDKQUERYURLHISTORY_H_
#endif // __cplusplus
