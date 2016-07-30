/*!
* @file ConfigFactory.cpp
* 
* @brief This file defines ConfigFactory class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/11
*/

#include "ConfigFactory.h"
#include "CommonMacro.h"

USING_NAMESPACE_COMMON

IConfig* ConfigFactory::CreateConfig(IN LPCTSTR lpFileName, CONFIGTYPE configType)
{
    IConfig *pConfig = NULL;

    switch (configType)
    {
    case CONFIG_TYPE_INI:
        pConfig = new IniConfig(lpFileName);
        break;

    case CONFIG_TYPE_XML:
        break;

    default:
        pConfig = new IniConfig(lpFileName);
        break;
    }

    return pConfig;
}

//////////////////////////////////////////////////////////////////////////

void ConfigFactory::DeleteConfig(IN OUT IConfig **ppConfig)
{
    if (NULL != ppConfig)
    {
        SAFE_DELETE(*ppConfig);
    }
}
