/*!
* @file SdkKnownFolderUtil.cpp
* 
* @brief This file defines the special folder manager.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/16 12:00
*/

#include "stdafx.h"
#include "SdkKnownFolderUtil.h"
#include "SdkImagesManager.h"
#include "SdkCommonHelper.h"

USING_NAMESPACE_COMMON
USING_NAMESPACE_UTILITIES

//////////////////////////////////////////////////////////////////////////

LPITEMIDLIST SdkKnownFolderUtil::BrowseSpecialFolder(HWND hWnd, UINT nCSIDL, LPWSTR pszDisplayName)
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    LPITEMIDLIST pidlRoot = NULL;
    LPITEMIDLIST pidlSelected = NULL;

    // +1 pidlRoot
    SHGetSpecialFolderLocation(hWnd, nCSIDL, &pidlRoot);

    BROWSEINFO bi = { 0 };
    WCHAR szDisplayName[MAX_PATH] = { 0 };
    bi.pidlRoot = pidlRoot;
    bi.hwndOwner = hWnd;
    bi.lpszTitle = pszDisplayName;
    bi.pszDisplayName = szDisplayName;
    bi.ulFlags = BIF_USENEWUI | BIF_UAHINT | BIF_RETURNONLYFSDIRS;
    bi.lpfn = NULL;
    bi.lParam = 0;

    pidlSelected = SHBrowseForFolder(&bi);

    // -1 pidlRoot
    CoTaskMemFree(pidlRoot);
    CoUninitialize();

    return pidlSelected;
}

//////////////////////////////////////////////////////////////////////////

SdkKnownFolderUtil::SdkKnownFolderUtil()
{
    m_isFolder    = FALSE;
    m_pshDesktop  = NULL;
    m_pKnownItemList = new KNOWNITEMLIST();

    m_bmpSize.cx = 256;
    m_bmpSize.cy = 256;
    ZeroMemory(m_szFolderPath, MAX_PATH * sizeof(TCHAR));
}

//////////////////////////////////////////////////////////////////////////

SdkKnownFolderUtil::~SdkKnownFolderUtil()
{
    ClearAllItems();

    SAFE_DELETE(m_pKnownItemList);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkKnownFolderUtil::InitFolderInfo(IN UINT ncsidlFolder, UINT nBmpWidth, UINT nBmpHeight)
{
    IShellFolder *pshWorkDir = NULL;
    ITEMIDLIST   *pidlWorkDir = NULL;

    m_bmpSize.cx = nBmpWidth;
    m_bmpSize.cy = nBmpHeight;

    // +1
    HRESULT hr = SHGetSpecialFolderLocation(NULL, ncsidlFolder, &pidlWorkDir);
    if (SUCCEEDED(hr))
    {
        // +2
        hr = SHGetDesktopFolder(&m_pshDesktop);
        if (SUCCEEDED(hr))
        {
            // +3
            hr = m_pshDesktop->BindToObject(pidlWorkDir, NULL, IID_PPV_ARGS(&pshWorkDir));
            // Enumerate the shell item information
            hr = EnumShellItemInfo(SUCCEEDED(hr) ? pshWorkDir : m_pshDesktop);
            // Get the current work directory path
            SHGetPathFromIDList(pidlWorkDir, m_szFolderPath);
        }
    }

    // -3
    SAFE_RELEASE(pshWorkDir);
    // -2
    SAFE_RELEASE(m_pshDesktop);
    // -1
    CoTaskMemFree(pidlWorkDir);

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

UINT SdkKnownFolderUtil::GetItemCount()
{
    return (NULL != m_pKnownItemList) ? (UINT)m_pKnownItemList->size() : 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkKnownFolderUtil::GetItem(UINT nIndex, OUT KNOWNITEM* pKnownItem)
{
    if ( (NULL == this->m_pKnownItemList) || (NULL == pKnownItem) )
    {
        return FALSE;
    }

    if ( nIndex < (UINT)m_pKnownItemList->size() )
    {
        KNOWNITEM *pTempItem = m_pKnownItemList->at(nIndex);

        pKnownItem->hIcon = ::CopyIcon(pTempItem->hIcon);
        pKnownItem->hBitmap = (HBITMAP)::CopyImage(
            pTempItem->hBitmap,
            IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
        _tcscpy_s(pKnownItem->szDisplayName, MAX_PATH, pTempItem->szDisplayName);
        _tcscpy_s(pKnownItem->szFilePath, MAX_PATH, pTempItem->szFilePath);

        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkKnownFolderUtil::SaveAllIcons(IN LPCTSTR pszFolderPath)
{
    if ( NULL == pszFolderPath )
    {
        return FALSE;
    }

    // Create the directory.
    if ( !PathFileExists(pszFolderPath) )
    {
        if ( !SdkCommonHelper::CreateFolder(pszFolderPath) )
        {
            return FALSE;
        }
    }

    TCHAR szFilePath[MAX_PATH] = { 0 };
    wstring strFileName;
    SdkImagesManager imageManager;
    for (KNOWNITEMITER iter = m_pKnownItemList->begin();
         iter != m_pKnownItemList->end(); iter++)
    {
        strFileName.clear();
        strFileName.append((*iter)->szDisplayName).append(L".png");
        ::PathCombine(szFilePath, pszFolderPath, strFileName.c_str());

        // Save icons to specified path
        ICONINFO iconInfo;
        ::GetIconInfo((*iter)->hIcon, &iconInfo);
        imageManager.LoadImageFromHBITMAP(iconInfo.hbmColor);
        imageManager.SaveImageToFile(szFilePath);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SdkKnownFolderUtil::GetFolderPath(OUT LPTSTR pszFolderPath, IN DWORD dwSize)
{
    if ( (NULL != pszFolderPath) && (dwSize > 0) )
    {
        StringCchCopy(pszFolderPath, dwSize, m_szFolderPath);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkKnownFolderUtil::ClearAllItems()
{
    int nSize = (int)m_pKnownItemList->size();
    for (int i = 0; i < nSize; ++i)
    {
        LPKNOWNITEM pItem = m_pKnownItemList->at(i);

        SAFE_DELETE_OBJECT(pItem->hBitmap);
        SAFE_DELETE_ICON(pItem->hIcon);
        SAFE_DELETE(pItem);
    }

    m_pKnownItemList->clear();
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkKnownFolderUtil::EnumShellItemInfo(IN IShellFolder* pshWorkDir)
{
    if ( NULL == pshWorkDir )
    {
        return E_FAIL;
    }

    IEnumIDList* penumIdlist = NULL;
    HRESULT hr = pshWorkDir->EnumObjects(
        NULL,
        SHCONTF_FOLDERS | SHCONTF_NONFOLDERS,
        &penumIdlist);

    if ( SUCCEEDED(hr) && (NULL != penumIdlist) )
    {
        ULONG pceltFetched = 0;
        ITEMIDLIST* rgelt = NULL;
        while ( SUCCEEDED(penumIdlist->Next(1, &rgelt, &pceltFetched)) &&
                (1 == pceltFetched) )
        {
            KNOWNITEM *pknownItem = NULL;
            if ( GetShellItemInfo(pshWorkDir, rgelt, &pknownItem) )
            {
                m_pKnownItemList->push_back(pknownItem);
            }

            CoTaskMemFree(rgelt);
        }
    }

    SAFE_RELEASE(penumIdlist);

    return hr;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkKnownFolderUtil::GetShellItemInfo(IN IShellFolder* pshWorkDir, IN LPCITEMIDLIST pidl, OUT KNOWNITEM** pknownItem)
{
    if ( (NULL == pshWorkDir) || (NULL == pidl) || (NULL == pknownItem) )
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;

    *pknownItem = new KNOWNITEM();
    SHGetPathFromIDList(pidl, (*pknownItem)->szFilePath);
    STRRET pName;

    // Get file path.
    if ( 0 == wcslen((*pknownItem)->szFilePath) )
    {
        LPITEMIDLIST pidlreal = NULL;
        SHGetRealIDL(m_pshDesktop, pidl, &pidlreal);
        if ( NULL != pidlreal )
        {
            SHGetPathFromIDList(pidlreal, (*pknownItem)->szFilePath);
            hr = pshWorkDir->GetDisplayNameOf(pidlreal, SHGDN_FORPARSING, &pName);
            hr = StrRetToBuf(&pName, pidlreal, (*pknownItem)->szFilePath, MAX_PATH);
        }
        CoTaskMemFree(pidlreal);
    }

    // Get display name
    hr = pshWorkDir->GetDisplayNameOf(pidl, SHGDN_INFOLDER | SHGDN_FOREDITING, &pName);
    hr = StrRetToBuf(&pName, pidl, (*pknownItem)->szDisplayName, MAX_PATH);
    // Get items' icon
    (*pknownItem)->hIcon = SdkCommonHelper::GetIcon(pshWorkDir, pidl);
    // This may can not load hbitmap of an item, such as items in Control Panel.
    (*pknownItem)->hBitmap = SdkCommonHelper::GetItemBitmap(pidl, m_bmpSize.cx, m_bmpSize.cy);

    //ICONINFO iconInfo;
    //::GetIconInfo((*pknownItem)->hIcon, &iconInfo);
    //SdkImagesManager imageMgr;
    //imageMgr.LoadImageFromHBITMAP(iconInfo.hbmColor);
    //(*pknownItem)->hBitmap = imageMgr.GetHBITMAP();

    if ( FAILED(hr) )
    {
        SAFE_DELETE(*pknownItem);
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}
