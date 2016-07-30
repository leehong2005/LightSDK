/*!
* @file SdkUIRunTime.cpp
* 
* @brief This file defines the class SdkUIRunTime, which is used to initialize UI runtime.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/07/12
*/

#include "StdAfx.h"
#include "SdkUIRunTime.h"
#include "SdkResManager.h"
#include "SdkD2DTheme.h"
#include "SdkCommonInclude.h"

USING_NAMESPACE_THEME
USING_NAMESPACE_UILIB

BOOL SdkUIRunTime::InitializeUIRunTime()
{
    // Initialize common run-time, such as WIC, COM, Logger, etc.
    BOOL retVal1 = SdkCommonRunTime::InitializeRunTime();

    // Load resource library.
    BOOL retVal2 = SdkResManager::LoadResLibrary();

    // Create instance of D2D Theme using to draw D2D graphics.
    BOOL retVal3 = SdkD2DTheme::CreateD2DThemeInstance();

    return (retVal1 && retVal2 && retVal3);
}

//////////////////////////////////////////////////////////////////////////

void SdkUIRunTime::UninitializeUIRunTime()
{
    SdkD2DTheme::DeleteD2DThemeInstance();
    SdkResManager::FreeResLibrary();
    SdkCommonRunTime::UninitializeRunTime();
}
