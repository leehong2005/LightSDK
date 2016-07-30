/*!
* @file SdkFilePropKeyHelper.h 
* 
* @brief This file define property key helper functions.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#ifdef __cplusplus
#ifndef _SDKFILEPROPKEYHELPER_H_
#define _SDKFILEPROPKEYHELPER_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"
#include "SdkFilePropKey.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The property key information.
*/
typedef struct _PROPERTYKEYINFO
{
    PROPERTYKEYNAME     keyName;        // The key name
    PROPERTYKEYTYPE     keyType;        // The key type.
    PROPERTYKEY         keyValue;       // The key value

} PROPERTYKEYINFO, *LPPROPERTYKEYINFO;

/*!
* @brief The MediaKeyHelper class is used to get key value by specified key name.
*/
class CLASS_DECLSPEC SdkFilePropKeyHelper
{
public:

    /*!
    * @brief Get the property key value by specified key name.
    *
    * @param keyName    [I/ ] The key name.
    * @param keyInfo    [ /O] The output key info.
    *
    * @return TRUE if succeed to get key info, otherwise return FALSE.
    */
    static BOOL GetPropertyKey(IN PROPERTYKEYNAME keyName, OUT PROPERTYKEYINFO &keyInfo);

private:

    /*!
    * @brief Initialize the property keys.
    */
    static void InitPropertyKey();

private:

    /*!
    * @brief Key name to key info map.
    */
    typedef map <PROPERTYKEYNAME, PROPERTYKEYINFO> KeyInfoMap;

    /*!
    * @brief Key name to key info pair.
    */
    typedef pair<PROPERTYKEYNAME, PROPERTYKEYINFO> KeyInfoPair;

    static BOOL         s_hasInitKeys;      // Indicates whether has initialized keys or not.
    static KeyInfoMap   s_keyInfoMap;       // The key name to key info map.
};

END_NAMESPACE_COMMON

#endif // _SDKFILEPROPKEYHELPER_H_
#endif // __cplusplus
