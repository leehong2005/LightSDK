/*!
* @file ConfigFactory.h 
* 
* @brief This file defines ConfigFactory class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/11
*/

#ifdef __cplusplus
#ifndef _CONFIGFACTORY_H_
#define _CONFIGFACTORY_H_

#include "Common.h"
#include "CommonMacro.h"
#include "IniConfig.h"

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
* @brief ConfigFactory class uses to create IConfig instance.
*/
class CLASS_DECLSPEC ConfigFactory
{
public:

    /*!
    * @brief Create Config instance.
    *
    * @param lpFileName     [I/ ] The config file name.
    * @param configType     [I/ ] The config type.
    *
    * @return The IConfig instance.
    */
    static IConfig* CreateConfig(IN LPCTSTR lpFileName, CONFIGTYPE configType = CONFIG_TYPE_INI);

    /*!
    * @brief Delete Config instance.
    *
    * @param ppConfig       [I/O] The config file name.
    */
    static void DeleteConfig(IN OUT IConfig **ppConfig);
};

END_NAMESPACE

#endif // _CONFIGFACTORY_H_
#endif // __cplusplus
