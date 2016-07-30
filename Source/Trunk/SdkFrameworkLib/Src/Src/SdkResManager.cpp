/*!
* @file SdkResManager.cpp
* 
* @brief This file defines SdkResManager class for managing resources.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/22
*/

#include "stdafx.h"
#include "SdkResManager.h"

USING_NAMESPACE_THEME

HMODULE SdkResManager::s_hResModule = NULL;
vector<wstring> SdkResManager::s_vctStrings;
vector<LPIMAGEDATA> SdkResManager::s_vctImageInfos;
SdkWICImageHelper* SdkResManager::s_pImageHelper = NULL;


//////////////////////////////////////////////////////////////////////////

BOOL SdkResManager::LoadResLibrary(LPCTSTR lpLibFileName)
{
    if ( NULL == lpLibFileName )
    {
        lpLibFileName = L"SdkResourceLib.dll";
    }

    s_hResModule   = ::LoadLibrary(lpLibFileName);
    s_pImageHelper = new SdkWICImageHelper();

    return (NULL != s_hResModule);
}

//////////////////////////////////////////////////////////////////////////

void SdkResManager::FreeResLibrary()
{
    if ( NULL != s_hResModule )
    {
        ::FreeLibrary(s_hResModule);
        s_hResModule = NULL;
    }

    SAFE_DELETE(s_pImageHelper);

    // Delete the cache image data.
    int nSize = (int)s_vctImageInfos.size();
    for (int i = 0; i < nSize; ++i)
    {
        LPIMAGEDATA lpImageData = s_vctImageInfos[i];
        SAFE_DELETE_OBJECT(lpImageData->hBitmap);
        SAFE_DELETE(lpImageData);
    }
}

//////////////////////////////////////////////////////////////////////////

HMODULE SdkResManager::GetResModule()
{
    return s_hResModule;
}

//////////////////////////////////////////////////////////////////////////

HBITMAP SdkResManager::GetImage(int nResId)
{
    return GetImage(nResId, SdkResManager::GetResModule());
}

//////////////////////////////////////////////////////////////////////////

HBITMAP SdkResManager::GetImage(int nResId, HMODULE hModule)
{
    if ( NULL == hModule )
    {
        hModule = s_hResModule;
    }

    LPIMAGEDATA lpImageData = GetImageInfo(nResId, hModule);
    if ( NULL != lpImageData )
    {
        return lpImageData->hBitmap;
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////

LPIMAGEDATA SdkResManager::GetImageInfo(int nResId, HMODULE hModule)
{
    if ( NULL == hModule )
    {
        hModule = s_hResModule;
    }

    LPIMAGEDATA lpImageData = FindImageData(nResId, hModule);
    if ( NULL == lpImageData )
    {
        if ( NULL != s_pImageHelper )
        {
            BOOL isSucceed = s_pImageHelper->LoadFromResource(nResId, hModule);
            if ( isSucceed )
            {
                lpImageData = new IMAGEDATA();

                lpImageData->hModule = hModule;
                lpImageData->nResId  = nResId;
                lpImageData->hBitmap = s_pImageHelper->GetHBITMAP();
                lpImageData->nHeight = s_pImageHelper->GetHeight();
                lpImageData->nWidth  = s_pImageHelper->GetWidth();

                s_vctImageInfos.push_back(lpImageData);
            }
        }
    }

    return lpImageData;
}

//////////////////////////////////////////////////////////////////////////

LPCTSTR SdkResManager::GetString(int nResId, HMODULE hModule)
{
    if ( NULL == hModule )
    {
        hModule = s_hResModule;
    }

    CryptString str = SdkCommonHelper::LoadResString(hModule, nResId);

    return str.c_str();
}

//////////////////////////////////////////////////////////////////////////

LPIMAGEDATA SdkResManager::FindImageData(int nResId, HMODULE hModule)
{
    LPIMAGEDATA lpImageData = NULL;

    for each (LPIMAGEDATA imageData in s_vctImageInfos)
    {
        if ( nResId  == imageData->nResId &&
             hModule == imageData->hModule )
        {
            lpImageData = imageData;
            break;
        }
    }

    return lpImageData;
}
