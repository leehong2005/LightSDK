/*!
* @file SdkCommonRunTime.cpp
* 
* @brief The SdkCommonRunTime class implements the run-time of this library.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#include "stdafx.h"
#include "SdkCommonRunTime.h"
#include "SdkImagesManager.h"
#include "SdkLogger.h"
#include "SdkWICAnimatedGif.h"
#include "SdkWICImageHelper.h"

USING_NAMESPACE_COMMON
USING_NAMESPACE_UTILITIES

BOOL SdkCommonRunTime::InitializeRunTime()
{
    CoInitialize(NULL);
    SdkLogger::Initialize();
    SdkWICImageHelper::WICInitialize();
    SdkWICAnimatedGif::WICInitialize();
    SdkImagesManager::GdiplusInitialize();

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SdkCommonRunTime::UninitializeRunTime()
{
    SdkLogger::Uninitialize();
    SdkWICImageHelper::WICUninitialize();
    SdkWICAnimatedGif::WICUninitialize();
    SdkImagesManager::GdiplusInitialize();
    CoUninitialize();
}
