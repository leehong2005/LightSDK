/*!
* @file SdkConfigFactory.cpp
* 
* @brief This file defines ConfigFactory class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/11
*/

#include "stdafx.h"
#include "SdkConfigFactory.h"
#include "SdkCommonMacro.h"

USING_NAMESPACE_COMMON

IConfigUtil* SdkConfigFactory::CreateIConfigUtil(IN LPCTSTR lpFileName, CONFIGTYPE configType)
{
    IConfigUtil *pConfig = NULL;

    switch (configType)
    {
    case CONFIG_TYPE_INI:
        pConfig = new SdkIniConfigUtil(lpFileName);
        break;

    case CONFIG_TYPE_XML:
        break;

    default:
        pConfig = new SdkIniConfigUtil(lpFileName);
        break;
    }

    return pConfig;
}

//////////////////////////////////////////////////////////////////////////

void SdkConfigFactory::DeleteIConfigUtil(IN OUT IConfigUtil **ppConfig)
{
    if (NULL != ppConfig)
    {
        SAFE_DELETE(*ppConfig);
    }
}
