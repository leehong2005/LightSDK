/*!
* @file SdkUrlShortcutUtil.h
* 
* @brief This file includes some common header files.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/18
*/

#ifdef __cplusplus
#ifndef _SDKURLSHORTCUTMANAGER_H_
#define _SDKURLSHORTCUTMANAGER_H_

#include "SdkCommonHelper.h"
#include "SdkCommonMacro.h"
#include <intshcut.h>

BEGIN_NAMESPACE_UTILITIES

/*!
* @brief The SdkUrlShortcutUtil class is used to manager url shortcut.
*/
class CLASS_DECLSPEC SdkUrlShortcutUtil
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkUrlShortcutUtil();

    /*!
    * @brief The destructor function.
    */
    ~SdkUrlShortcutUtil();

    /*!
    * @brief Create internet shortcut according to the specified url and file name.
    *        The default shortcut path is Favorite.
    *
    * @param lpcsUrl        [I/ ] The url.
    * @param lpcsFileName   [I/ ] The shortcut file name, must end with .url.
    *
    * @return TRUE if succeeds, FALSE otherwise.
    */
    BOOL CreateInternetShortcut(IN LPCTSTR lpcsUrl, IN LPCTSTR lpcsFileName);

    /*!
    * @brief Create internet shortcut according to the specified url and file name.
    *        The default shortcut path is Favorite.
    *
    * @param lpcsUrl        [I/ ] The url.
    * @param lpcsFileDir    [I/ ] The folder path of the shortcut.
    * @param lpcsFileName   [I/ ] The shortcut file name, must end with .url.
    *
    * @return TRUE if succeeds, FALSE otherwise.
    */
    BOOL CreateInternetShortcut(IN LPCTSTR lpcsUrl, IN LPCTSTR lpcsFileDir, IN LPCTSTR lpcsFileName);

protected:

    /*!
    * @brief Call this method to initialize.
    *
    * @return TRUE if succeeds, FALSE otherwise.
    */
    BOOL Initialize();

protected:

    WCHAR   m_szFavoritePath[MAX_PATH];     // The favorite folder path.
};

END_NAMESPACE_UTILITIES

#endif // _SDKURLSHORTCUTMANAGER_H_
#endif // __cplusplus
