/*!
* @file SdkConfigFactory.h 
* 
* @brief This file defines ConfigFactory class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/11
*/

#ifdef __cplusplus
#ifndef _SDKCONFIGFACTORY_H_
#define _SDKCONFIGFACTORY_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"
#include "SdkIniConfigUtil.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The config type enumeration.
*/
typedef enum _CONFIGTYPE
{
    CONFIG_TYPE_INI     = 1,
    CONFIG_TYPE_XML     = 2,

} CONFIGTYPE;

/*!
* @brief ConfigFactory class uses to create IConfigUtil instance.
*/
class CLASS_DECLSPEC SdkConfigFactory
{
public:

    /*!
    * @brief Create Config instance.
    *
    * @param lpFileName     [I/ ] The config file name.
    * @param configType     [I/ ] The config type.
    *
    * @return The IConfigUtil instance.
    */
    static IConfigUtil* CreateIConfigUtil(IN LPCTSTR lpFileName, CONFIGTYPE configType);

    /*!
    * @brief Delete Config instance.
    *
    * @param ppConfig       [I/O] The config file name.
    */
    static void DeleteIConfigUtil(IN OUT IConfigUtil **ppConfig);
};

END_NAMESPACE_COMMON

#endif // _SDKCONFIGFACTORY_H_
#endif // __cplusplus
