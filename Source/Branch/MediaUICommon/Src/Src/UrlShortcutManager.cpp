/*!
* @file UrlShortcutManager.cpp
* 
* @brief This file includes some common header files.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/18
*/

#include "UrlShortcutManager.h"

USING_NAMESPACE_COMMON

UrlShortcutManager::UrlShortcutManager()
{
    m_szFavoritePath[0] = 0;
    Initialize();
}

//////////////////////////////////////////////////////////////////////////

UrlShortcutManager::~UrlShortcutManager()
{
}

//////////////////////////////////////////////////////////////////////////

BOOL UrlShortcutManager::CreateInternetShortcut(LPCTSTR lpcsUrl, LPCTSTR lpcsFileName)
{
    return CreateInternetShortcut(lpcsUrl, m_szFavoritePath, lpcsFileName);
}

//////////////////////////////////////////////////////////////////////////

BOOL UrlShortcutManager::CreateInternetShortcut(LPCTSTR lpcsUrl, LPCTSTR lpcsFileDir, LPCTSTR lpcsFileName)
{
    HRESULT hr = S_OK;
    IPersistFile *pPersistFile = NULL;
    IUniformResourceLocator *pUniformResLocator = NULL;

    TCHAR szFullPath[MAX_PATH] = { 0 };
    PathCombine(szFullPath, lpcsFileDir, lpcsFileName);

    hr = CoCreateInstance(CLSID_InternetShortcut,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_PPV_ARGS(&pUniformResLocator));
    if (SUCCEEDED(hr))
    {
        hr = pUniformResLocator->SetURL(lpcsUrl, IURL_SETURL_FL_GUESS_PROTOCOL);
    }

    if (SUCCEEDED(hr))
    {
        hr = pUniformResLocator->QueryInterface(IID_PPV_ARGS(&pPersistFile));
    }

    if (SUCCEEDED(hr))
    {
        hr = pPersistFile->Save(szFullPath, TRUE);
    }

    SAFE_RELEASE(pPersistFile);
    SAFE_RELEASE(pUniformResLocator);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL UrlShortcutManager::Initialize()
{
    return CommonHelper::GetKnownFolderPath(m_szFavoritePath, CSIDL_FAVORITES);
}
