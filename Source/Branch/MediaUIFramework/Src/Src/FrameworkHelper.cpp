/*!
* @file FrameworkHelper.cpp
* 
* @brief This file defines common utility class used for framework.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/22
*/

#include "FrameworkHelper.h"

HMODULE FrameworkHelper::GetCurrentModuleHandle()
{
    // s_somevar must be static variable, otherwise the returned HMODULE is not correct instance of current dll.
    static int s_somevar = 0;
    MEMORY_BASIC_INFORMATION mbi;
    if(!::VirtualQuery(&s_somevar, &mbi, sizeof(mbi)))
    {
        return NULL;
    }

    return static_cast<HMODULE>(mbi.AllocationBase);
}
