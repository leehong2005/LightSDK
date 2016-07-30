/*!
* @file SdkAssocHandler.cpp
* 
* @brief The SdkAssocHandler class is a wrapper of IAssocHandler interface.
* 
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2009/12/26 12:00
*/

#include "stdafx.h"
#include "SdkAssocHandler.h"
#include "SdkImagesManager.h"

USING_NAMESPACE_UTILITIES

//////////////////////////////////////////////////////////////////////////

BOOL SdkAssocHandler::GetAssocHandlers(
    IN OUT AssocHandlerList& assocHandlersList,
    IN LPCWSTR lpfileExt,
    BOOL isRecommended)
{
    if ( (NULL == lpfileExt) || (0 == wcslen(lpfileExt)) )
    {
        return FALSE;
    }

    IEnumAssocHandlers *penumAssocHandler = NULL;
    HRESULT hr = SHAssocEnumHandlers(
        lpfileExt,
        (TRUE == isRecommended) ? ASSOC_FILTER_RECOMMENDED : ASSOC_FILTER_NONE,
        &penumAssocHandler);

    if ( SUCCEEDED(hr) )
    {
        ULONG fetchedAmount = 0;
        IAssocHandler *assocHandler = NULL;

        while ( (SUCCEEDED(penumAssocHandler->Next(1, &assocHandler, &fetchedAmount))) &&
                (1 == fetchedAmount) )
        {
            assocHandlersList.push_back(new SdkAssocHandler(assocHandler));
        }

        SAFE_RELEASE(penumAssocHandler);
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

SdkAssocHandler::SdkAssocHandler()
{
    m_hAssocIcon    = NULL;
    m_hAssocBitmap  = NULL;
    m_pAssocHandler = NULL;
    m_nBmpWidth     = 0;
    m_nBmpHeight    = 0;
}

//////////////////////////////////////////////////////////////////////////

SdkAssocHandler::SdkAssocHandler(IAssocHandler *passocHandler)
{
    m_hAssocIcon    = NULL;
    m_hAssocBitmap  = NULL;
    m_nBmpWidth     = 0;
    m_nBmpHeight    = 0;
    m_pAssocHandler = passocHandler;

    SAFE_ADDREF(m_pAssocHandler);
}

//////////////////////////////////////////////////////////////////////////

SdkAssocHandler::~SdkAssocHandler()
{
    DestroyIcon(m_hAssocIcon);
    DeleteObject(m_hAssocBitmap);
    SAFE_RELEASE(m_pAssocHandler);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkAssocHandler::Attach(IAssocHandler *passocHandler)
{
    m_pAssocHandler = passocHandler;
    SAFE_RELEASE(m_pAssocHandler);

    return (NULL != m_pAssocHandler);
}

//////////////////////////////////////////////////////////////////////////

wstring SdkAssocHandler::GetUIName()
{
    wstring strRet;
    if ( NULL != m_pAssocHandler )
    {
        LPWSTR psz = NULL;
        if ( SUCCEEDED(m_pAssocHandler->GetUIName(&psz)) )
        {
            strRet.append(psz);
            CoTaskMemFree((void*)psz);
        }
    }
    return strRet;
}

//////////////////////////////////////////////////////////////////////////

wstring SdkAssocHandler::GetName()
{
    wstring strRet;
    if ( NULL != m_pAssocHandler )
    {
        LPWSTR psz = NULL;
        if ( SUCCEEDED(m_pAssocHandler->GetName(&psz)) )
        {
            strRet.append(psz);
            CoTaskMemFree((void*)psz);
        }
    }

    return strRet;
}

//////////////////////////////////////////////////////////////////////////

HICON SdkAssocHandler::GetIcon()
{
    if (NULL != m_hAssocIcon)
    {
        return m_hAssocIcon;
    }

    int nIndex = -1;
    LPWSTR psz = NULL;
    if ( (NULL != m_pAssocHandler) &&
         SUCCEEDED(m_pAssocHandler->GetIconLocation(&psz, &nIndex)) )
    {
        if ( 0 != _wcsicmp(PathFindExtension(psz), L".dll") )
        {
            SHFILEINFO info = { 0 };
            SHGetFileInfo(
                psz,
                (DWORD)-1,
                &info,
                sizeof(info),
                SHGFI_LARGEICON | SHGFI_OPENICON | SHGFI_ICON);

            m_hAssocIcon = info.hIcon;
        }
        else
        {
            INT32 nIconCount = 0;
            nIconCount = ::ExtractIconEx(psz, 0, NULL, NULL, 0);
            if (nIconCount >= 1)
            {
                ::ExtractIconEx(psz, 0, &m_hAssocIcon, NULL, 1);
                if (NULL == m_hAssocIcon)
                {
                    ::ExtractIconEx(psz, 0, NULL, &m_hAssocIcon, 1);
                }
            }
        }

        CoTaskMemFree(psz);
    }

    return m_hAssocIcon;
}

//////////////////////////////////////////////////////////////////////////

HBITMAP SdkAssocHandler::GetBitmap(int nWidth, int nHeight)
{
    if ( (nWidth > 0.0) && (nHeight > 0.0) &&
         (nWidth != m_nBmpWidth) && (nHeight != m_nBmpHeight) )
    {
        m_nBmpWidth = nWidth;
        m_nBmpHeight = nHeight;

        wstring strFilePath = GetName();
        IShellItemImageFactory *psif = NULL;
        SIZE size = { nWidth, nHeight };

        HRESULT hr = ::SHCreateItemFromParsingName(
            strFilePath.c_str(),
            NULL,
            IID_PPV_ARGS(&psif));

        if ( SUCCEEDED(hr) )
        {
            SAFE_DELETE_OBJECT(m_hAssocBitmap);
            psif->GetImage(size, SIIGBF_ICONONLY, &m_hAssocBitmap);
        }
        SAFE_RELEASE(psif);
    }

    return m_hAssocBitmap;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkAssocHandler::IsRecommended()
{
    return (NULL != m_pAssocHandler) ? m_pAssocHandler->IsRecommended() : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkAssocHandler::MakeDefault(IN LPCWSTR pszDescription)
{
    if (NULL != m_pAssocHandler)
    {
        return (SUCCEEDED(m_pAssocHandler->MakeDefault(pszDescription)));
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkAssocHandler::Invoke(IN LPCWSTR pszfilePath)
{
    HRESULT hr = E_FAIL;
    IShellFolder *psfDesktop = NULL;
    ITEMIDLIST *pidlCurFile  = NULL;
    ITEMIDLIST *pidlDesktop  = NULL;

    if ( SUCCEEDED(SHGetDesktopFolder(&psfDesktop)) )
    {
        WCHAR szfilePath[MAX_PATH] = { 0 };
        wcscpy_s(szfilePath, MAX_PATH, pszfilePath);

        hr = psfDesktop->ParseDisplayName(
            NULL,
            NULL,
            szfilePath,
            NULL,
            &pidlCurFile,
            NULL);

        if ( SUCCEEDED(hr) )
        {
            hr = SHGetIDListFromObject(psfDesktop, &pidlDesktop);
            if ( SUCCEEDED(hr) )
            {
                IDataObject *pdataobj = NULL;
                hr = SHCreateDataObject(
                    pidlDesktop,
                    1, 
                    (LPCITEMIDLIST*)&pidlCurFile,
                    NULL,
                    IID_PPV_ARGS(&pdataobj));

                if ( SUCCEEDED(hr) && (NULL != m_pAssocHandler) )
                {
                    hr = m_pAssocHandler->Invoke(pdataobj);
                }
            }
        }
    }

    CoTaskMemFree(pidlCurFile);
    CoTaskMemFree(pidlDesktop);
    SAFE_RELEASE(psfDesktop);

    return SUCCEEDED(hr);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkAssocHandler::SaveToFile(IN LPCWSTR pszfilePath)
{
    SdkImagesManager imageManager;
    imageManager.LoadImageFromHBITMAP(GetBitmap());
    return imageManager.SaveImageToFile(pszfilePath);
}