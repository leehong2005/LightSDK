/*!
* @file SdkFilePropDescription.h
* 
* @brief This file define functions to get shell property description.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/29
*/

#ifdef __cplusplus
#ifndef _SDKFILEPROPDESCRIPTION_H_
#define _SDKFILEPROPDESCRIPTION_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"
#include "SdkFilePropKey.h"
#include "SdkFilePropKeyHelper.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The definition of property key to property description map.
*/
typedef map<PROPERTYKEYNAME, IPropertyDescription*> PROPKEYTOPROPDESCMAP;

/*!
* @brief The SdkFilePropDescription class is used to get property descriptions.
*/
class CLASS_DECLSPEC SdkFilePropDescription
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkFilePropDescription();

    /*!
    * @brief The destructor function.
    */
    ~SdkFilePropDescription();

    /*!
    * @brief Get the property description .
    *
    * @param lpszPropDesc       [ /O] The property description.
    * @param uDescSize          [I/ ] The buffer size.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetPropDescription(IN PROPERTYKEYNAME keyName, OUT LPWSTR lpszPropDesc, IN UINT32 uDescSize);

private:

    /*!
    * @brief Get the property description and property value.
    *
    * @param pPropDesc          [I/ ] The IPropertyDescription instance.
    * @param lpszPropDesc       [ /O] The property description.
    * @param uDescSize          [I/ ] The buffer size.
    *
    * @return TRUE if succeed to get value, otherwise return FALSE.
    */
    BOOL GetPropDesc(IN IPropertyDescription *pPropDesc, OUT LPWSTR lpszDesc, UINT32 uDescSize);

private:

    PROPKEYTOPROPDESCMAP m_mapPropKeyToPropDesc;        // The property key to property description map.
};

END_NAMESPACE_COMMON

#endif // _SDKFILEPROPDESCRIPTION_H_
#endif // __cplusplus
