/*!
* @file PreviewHandler.cpp
* 
* @brief Implementation of CPreviewHandlerManager class.
*
* Copyright (C) 2009, Toshiba Corporation.
* 
* @author Li Hong
* @date 2009/12/03 12:00
* @version 1.0.0
*/

#include "PreviewHandler.h"

USING_NAMESPACE_COMMON

// The GUID of IPreviewHandler interface.
static const WCHAR* g_pPreviewHandlerQueryGUID = _T("{8895b1c6-b41f-4c1c-a562-0d564250836f}");

PreviewHandler::PreviewHandler() : m_pHandler(NULL),
                                   m_pInitWithFile(NULL),
                                   m_pInitWithItem(NULL),
                                   m_pInitWithStream(NULL),
                                   m_hGlobalMemory(NULL)
{
    ZeroMemory(m_szFileName, MAX_PATH * sizeof(WCHAR));
}

//////////////////////////////////////////////////////////////////////////

PreviewHandler::~PreviewHandler()
{
    DetachPreview();

    SAFE_RELEASE(m_pInitWithFile);
    SAFE_RELEASE(m_pInitWithItem);
    SAFE_RELEASE(m_pInitWithStream);

    ClearInstalledHandlerList();
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::AttachPreview(IN HWND hWnd, IN const RECT* lpRect, IN LPCWSTR lpfileName)
{
    if ( !IsWindow(hWnd) || !PathFileExists(lpfileName) )
    {
        return FALSE;
    }

    HRESULT hr = E_FAIL;
    TCHAR szClsid[MAX_PATH] = { 0 };
    // Get the CLSID from the file's extension.
    BOOL bSuccess = GetClsidFromExtension(PathFindExtension(lpfileName), szClsid, MAX_PATH);

    if ( bSuccess )
    {
        // Before preview, first detach preview previous.
        DetachPreview();

        // Create the instance of IPreviewHandler.
        if ( CreatePreviewHandlerFromClsid(&m_pHandler, szClsid) )
        {
            // Initialize the IPreviewHandler.
            if ( InitializeHandler(m_pHandler, lpfileName) )
            {
                RECT rc = { 0 };
                if (NULL == lpRect)
                {
                    GetClientRect(hWnd, &rc);
                }
                else
                {
                    rc = (*lpRect);
                }

                hr = m_pHandler->SetWindow(hWnd, &rc);
                hr = m_pHandler->SetRect(&rc);
                hr = m_pHandler->DoPreview();
            }
        }
    }

    return (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

void PreviewHandler::DetachPreview()
{
    if ( NULL != m_pHandler )
    {
        m_pHandler->Unload();
    }

    SAFE_RELEASE(m_pHandler);

    if ( NULL != m_hGlobalMemory )
    {
        GlobalFree(m_hGlobalMemory);
    }
}

//////////////////////////////////////////////////////////////////////////

void PreviewHandler::EnumInstalledHandler()
{
    // Enumerate installed handler by reading registry.
    LONG lResult = -1;
    DWORD dwHandlersCount = 0;
    HKEY hInstalledHandlerKey = NULL;
    const WCHAR *pSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\PreviewHandlers";

    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, pSubKey, 0, KEY_QUERY_VALUE, &hInstalledHandlerKey);
    if ( ERROR_SUCCESS == lResult )
    {
        lResult = RegQueryInfoKey(hInstalledHandlerKey, NULL, NULL, NULL, NULL, NULL, NULL,
                                  &dwHandlersCount, NULL, NULL, NULL, NULL);
        if ( ERROR_SUCCESS == lResult )
        {
            // First clear previous found handler from vector.
            ClearInstalledHandlerList();
            // Add found handle to vector.
            PREVIEWHANDLERINFO *handlerInfo = NULL;
            for (int i = 0; i < (int)dwHandlersCount; ++i)
            {
                DWORD dwSize = 100;
                handlerInfo = new PREVIEWHANDLERINFO();
                lResult = RegEnumValue(hInstalledHandlerKey, i, handlerInfo->szClsid, &dwSize,
                                       NULL, NULL, (LPBYTE)handlerInfo->szName, &dwSize);
                if ( ERROR_SUCCESS == lResult )
                {
                    m_vtHandlerList.push_back(handlerInfo);
                }
                else
                {
                    SAFE_DELETE(handlerInfo);
                }
            }
        }
    }

    RegCloseKey(hInstalledHandlerKey);
}

//////////////////////////////////////////////////////////////////////////

DWORD PreviewHandler::GetInstalledHandlerCount()
{
    return (DWORD)m_vtHandlerList.size();
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::GetInstalledHandlerItem(IN DWORD nIndex, OUT LPPREVIEWHANDLERINFO previewHandleInfo)
{
    DWORD dwSize = (DWORD)m_vtHandlerList.size();
    if ( (nIndex < 0) || (nIndex >= dwSize) || (NULL == previewHandleInfo) )
    {
        return FALSE;
    }

    LPPREVIEWHANDLERINFO item = m_vtHandlerList.at(nIndex);
    if ( NULL != item )
    {
        StringCchCopy(previewHandleInfo->szClsid, 100, item->szClsid);
        StringCchCopy(previewHandleInfo->szName, 100, item->szName);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::GetClsidFromExtension(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize)
{
    // First get CLSID from map.
    if ( GetClsidFromMap(lpfileExt, pszClsid, dwSize) )
    {
        return TRUE;
    }

    BOOL bSuccess = TRUE;

    if ( !GetClsidFromDefaultType(lpfileExt, pszClsid, dwSize) )
    {
        if ( !GetClsidFromExtShellEx(lpfileExt, pszClsid, dwSize) )
        {
            if ( !GetClsidFromSystemFileAssoc(lpfileExt, pszClsid, dwSize) )
            {
                if ( !GetClsidFromSystemFileAssocExt(lpfileExt, pszClsid, dwSize) )
                {
                    bSuccess = GetClsidFromOpenWithProgIds(lpfileExt, pszClsid, dwSize);
                }
            }
        }
    }

    // If succeed to find CLSID, insert the CLSID and name pair to map.
    if ( bSuccess )
    {
        m_mapExtensionToCLSID.insert(pair<wstring, wstring>(CommonHelper::StringToLowerCase(lpfileExt), wstring(pszClsid)));
    }

    return bSuccess;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::CreatePreviewHandlerFromClsid(OUT IPreviewHandler** ppHandler, IN LPCTSTR pClsid)
{
    if ( (NULL == ppHandler) || (NULL == pClsid) )
    {
        return FALSE;
    }

    CLSID clsid = { 0 };
    HRESULT hr = CLSIDFromString(const_cast<LPOLESTR>(pClsid), &clsid);
    if ( SUCCEEDED(hr) )
    {
        (*ppHandler) = NULL;
        hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_PPV_ARGS(ppHandler));
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::InitializeHandler(IN IPreviewHandler* pHandler, IN LPCWSTR lpfileName)
{
    if (NULL != lpfileName)
    {
        wcscpy_s(m_szFileName, MAX_PATH, lpfileName);
    }

    if ( !InitializeHandlerWithFile(pHandler) )
    {
        if ( !InitializeHandlerWithStream(pHandler) )
        {
            if ( !InitializeHandlerWithItem(pHandler) )
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}


//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::InitializeHandlerWithFile(IN IPreviewHandler* pHandler)
{
    if ( NULL == pHandler )
    {
        return FALSE;
    }

    SAFE_RELEASE(m_pInitWithFile);

    HRESULT hr = pHandler->QueryInterface(IID_IInitializeWithFile, (LPVOID*)&m_pInitWithFile);
    if ( SUCCEEDED(hr) )
    {
        hr = m_pInitWithFile->Initialize(m_szFileName, STGM_READ);
    }

    return (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::InitializeHandlerWithStream(IN IPreviewHandler* pHandler)
{
    if ( NULL == pHandler )
    {
        return FALSE;
    }

    SAFE_RELEASE(m_pInitWithStream);

    HRESULT hr = pHandler->QueryInterface(IID_IInitializeWithStream, (LPVOID*)&m_pInitWithStream);
    if ( FAILED(hr) )
    {
        return FALSE;
    }

    HANDLE hFile = CreateFile(m_szFileName,
                              FILE_READ_DATA,
                              FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    if ( INVALID_HANDLE_VALUE != hFile )
    {
        DWORD dwFileSize = GetFileSize(hFile, NULL);
        if ( INVALID_FILE_SIZE != dwFileSize )
        {
            m_hGlobalMemory = GlobalAlloc(GMEM_FIXED, dwFileSize);
            BYTE *pData = (BYTE*)GlobalLock(m_hGlobalMemory);
            if ( NULL != pData )
            {
                DWORD dwRead = 0;
                ReadFile(hFile, pData, dwFileSize, &dwRead, NULL);
                IStream *pStream = NULL;
                if ( (S_OK == CreateStreamOnHGlobal(m_hGlobalMemory, TRUE, &pStream)) && (NULL != pStream) )
                {
                    hr = m_pInitWithStream->Initialize(pStream, STGM_READ);
                }
                SAFE_RELEASE(pStream);
            }
            GlobalUnlock(m_hGlobalMemory);
        }
    }

    CloseHandle(hFile);

    return (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::InitializeHandlerWithItem(IN IPreviewHandler* pHandler)
{
    if ( NULL == pHandler )
    {
        return FALSE;
    }

    SAFE_RELEASE(m_pInitWithItem);
    HRESULT hr = pHandler->QueryInterface(IID_IInitializeWithItem, (LPVOID*)&m_pInitWithItem);
    if ( SUCCEEDED(hr) )
    {
        IShellItem *pItem = NULL;
        hr = SHCreateItemFromParsingName(m_szFileName, NULL, IID_IShellItem, (LPVOID*)&pItem);
        if ( SUCCEEDED(hr) )
        {
            hr = m_pInitWithItem->Initialize(pItem, STGM_READ);
        }

        SAFE_RELEASE(pItem);
    }

    return (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::GetClsidFromMap(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize)
{
    if ( (NULL != pszClsid) && (dwSize > 0) )
    {
        map<wstring, wstring>::iterator item = m_mapExtensionToCLSID.find(CommonHelper::StringToLowerCase(lpfileExt));
        if ( item != m_mapExtensionToCLSID.end() )
        {
            StringCchCopy(pszClsid, dwSize, ((wstring)item->second).c_str());
            return TRUE;
        }
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::GetClsidFromDefaultType(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize)
{
    if ( (NULL == lpfileExt) || (NULL == pszClsid) || (dwSize <= 0) )
    {
        return FALSE;
    }

    HKEY hFileExtKey = NULL;
    if ( ERROR_SUCCESS != RegOpenKeyEx(HKEY_CLASSES_ROOT, lpfileExt, 0, KEY_QUERY_VALUE, &hFileExtKey) )
    {
        return FALSE;
    }

    LONG lResult = -1;
    WCHAR szDefText[MAX_PATH] = { 0 };
    DWORD dwLength = MAX_PATH;
    if ( ERROR_SUCCESS == RegQueryValueEx(hFileExtKey, L"", NULL, 0, (LPBYTE)szDefText, &dwLength) )
    {
        WCHAR szSubKey[MAX_PATH] = { 0 };
        StringCchPrintf(szSubKey, MAX_PATH, L"%s\\ShellEx\\%s", szDefText, g_pPreviewHandlerQueryGUID);
        HKEY hProgKey = NULL;
        if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_CLASSES_ROOT, szSubKey, 0, KEY_QUERY_VALUE, &hProgKey) )
        {
            lResult = RegQueryValueEx(hProgKey, L"", NULL, 0, (LPBYTE)pszClsid, &dwSize);
        }
        RegCloseKey(hProgKey);
    }

    RegCloseKey(hFileExtKey);

    return (ERROR_SUCCESS == lResult) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::GetClsidFromExtShellEx(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize)
{
    if ( (NULL == lpfileExt) || (NULL == pszClsid) || (dwSize <= 0) )
    {
        return FALSE;
    }

    LONG lResult = -1;
    WCHAR szSubKey[MAX_PATH] = { 0 };
    StringCchPrintf(szSubKey, MAX_PATH, L"%s\\ShellEx\\%s", lpfileExt, g_pPreviewHandlerQueryGUID);

    HKEY hFileExtKey = NULL;
    if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_CLASSES_ROOT, szSubKey, 0, KEY_QUERY_VALUE, &hFileExtKey) )
    {
        lResult = RegQueryValueEx(hFileExtKey, L"", NULL, 0, (LPBYTE)pszClsid, &dwSize);
    }

    RegCloseKey(hFileExtKey);

    return (ERROR_SUCCESS == lResult) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::GetClsidFromSystemFileAssoc(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize)
{
    if ( (NULL == lpfileExt) || (NULL == pszClsid) || (dwSize <= 0) )
    {
        return FALSE;
    }

    LONG lResult = -1;
    HKEY hFileExtKey = NULL;
    if ( ERROR_SUCCESS != RegOpenKeyEx(HKEY_CLASSES_ROOT, lpfileExt, 0, KEY_QUERY_VALUE, &hFileExtKey) )
    {
        return FALSE;
    }

    WCHAR szPerceivedText[MAX_PATH] = { 0 };
    DWORD dwLength = MAX_PATH;
    if ( ERROR_SUCCESS == RegQueryValueEx(hFileExtKey, L"PerceivedType", NULL, 0, (LPBYTE)szPerceivedText, &dwLength) )
    {
        WCHAR szSubKeyText[MAX_PATH] = { 0 };
        StringCchPrintf(szSubKeyText, MAX_PATH, L"SystemFileAssociations\\%s\\ShellEx\\%s", szPerceivedText, g_pPreviewHandlerQueryGUID);
        HKEY hPreviewHandlerKey = NULL;
        if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_CLASSES_ROOT, szSubKeyText, 0, KEY_QUERY_VALUE, &hPreviewHandlerKey) )
        {
            lResult = RegQueryValueEx(hPreviewHandlerKey, L"", NULL, 0, (LPBYTE)pszClsid, &dwSize);
        }
        RegCloseKey(hPreviewHandlerKey);
    }

    RegCloseKey(hFileExtKey);

    return (ERROR_SUCCESS == lResult) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::GetClsidFromSystemFileAssocExt(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize)
{
    if ( (NULL == lpfileExt) || (NULL == pszClsid) || (dwSize <= 0) )
    {
        return FALSE;
    }

    LONG lResult = -1;
    WCHAR szSubKey[MAX_PATH] = { 0 };
    StringCchPrintf(szSubKey, MAX_PATH, L"SystemFileAssociations\\%s\\ShellEx\\%s", lpfileExt, g_pPreviewHandlerQueryGUID);
    HKEY hPreviewHandlerKey = NULL;
    if ( ERROR_SUCCESS == RegOpenKeyEx(HKEY_CLASSES_ROOT, szSubKey, 0, KEY_QUERY_VALUE, &hPreviewHandlerKey) )
    {
        lResult = RegQueryValueEx(hPreviewHandlerKey, L"", NULL, 0, (LPBYTE)pszClsid, &dwSize);
    }

    RegCloseKey(hPreviewHandlerKey);

    return (ERROR_SUCCESS == lResult) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL PreviewHandler::GetClsidFromOpenWithProgIds(IN LPCWSTR lpfileExt, OUT LPTSTR pszClsid, IN DWORD dwSize)
{
    if ( (NULL == lpfileExt) || (NULL == pszClsid) || (dwSize <= 0) )
    {
        return FALSE;
    }

    LONG lResult = -1;
    HKEY hFileExtOpenWithProgIdsKey = NULL;
    WCHAR szSubKeyBuf[MAX_PATH] = { 0 };
    StringCchPrintf(szSubKeyBuf, MAX_PATH, L"%s\\OpenWithProgIds", lpfileExt);
    if ( ERROR_SUCCESS != RegOpenKeyEx(HKEY_CLASSES_ROOT, szSubKeyBuf, 0, KEY_QUERY_VALUE, &hFileExtOpenWithProgIdsKey) )
    {
        return FALSE;
    }

    DWORD dwValues = 0;
    lResult = RegQueryInfoKey(hFileExtOpenWithProgIdsKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValues, NULL, NULL, NULL, NULL);
    if ( ERROR_SUCCESS != lResult )
    {
        RegCloseKey(hFileExtOpenWithProgIdsKey);
        return FALSE;
    }

    WCHAR szValueName[MAX_PATH] = { 0 };
    WCHAR szTempValueName[MAX_PATH] = { 0 };
    DWORD dwValueNameSize = MAX_PATH;
    DWORD dwType = 0;
    for (int i = 0; i < (int)dwValues; ++i)
    {
        lResult = RegEnumValue(hFileExtOpenWithProgIdsKey, i, szTempValueName, &dwValueNameSize, NULL, &dwType, NULL, NULL);
        if ( (ERROR_SUCCESS == lResult) && (REG_NONE == dwType) )
        {
            StringCchCopy(szValueName, MAX_PATH, szTempValueName);
            break;
        }
    }

    if ( wcslen(szValueName) > 0 )
    {
        ZeroMemory(szSubKeyBuf, MAX_PATH * sizeof(WCHAR));
        StringCchPrintf(szSubKeyBuf, MAX_PATH, L"%s\\ShellEx\\%s", szValueName, g_pPreviewHandlerQueryGUID);
        HKEY hPreviewHandlerKey = NULL;
        lResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, szSubKeyBuf, 0, KEY_QUERY_VALUE, &hPreviewHandlerKey);
        if ( ERROR_SUCCESS == lResult )
        {
            lResult = RegQueryValueEx(hPreviewHandlerKey, L"", NULL, 0, (LPBYTE)pszClsid, &dwSize);
        }
        RegCloseKey(hPreviewHandlerKey);
    }

    return (ERROR_SUCCESS == lResult) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

void PreviewHandler::ClearInstalledHandlerList()
{
    for (vector<LPPREVIEWHANDLERINFO>::iterator itor = m_vtHandlerList.begin();
         itor != m_vtHandlerList.end(); ++itor)
    {
        SAFE_DELETE(*itor);
    }

    m_vtHandlerList.clear();
}
