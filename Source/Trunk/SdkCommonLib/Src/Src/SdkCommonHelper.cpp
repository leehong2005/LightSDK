/*!
* @file CommonHelper.cpp 
* 
* @brief The implementation of common functions defined in CommonHelper.h file.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#include "stdafx.h"
#include "SdkCommonHelper.h"
#include "SdkDriversManager.h"
#include <math.h>

#pragma comment(lib, "comctl32.lib")

USING_NAMESPACE_COMMON

HBITMAP SdkCommonHelper::ExtractFileIcon(IN LPCTSTR pszPath, UINT nWidth, UINT nHeight)
{
    HBITMAP hBitmpa = NULL;
    if ( (NULL != pszPath) && (nWidth > 0.0) && (nHeight > 0.0) )
    {
        IShellItemImageFactory *psif = NULL;
        SIZE size = { nWidth, nHeight };
        HRESULT hr = ::SHCreateItemFromParsingName(pszPath, NULL, IID_PPV_ARGS(&psif));
        if ( SUCCEEDED(hr) && (NULL != psif) )
        {
            psif->GetImage(size, SIIGBF_ICONONLY, &hBitmpa);
        }
        SAFE_RELEASE(psif);
    }

    return hBitmpa;
}

//////////////////////////////////////////////////////////////////////////

HBITMAP SdkCommonHelper::ExtractFileThumbnail(IN LPCTSTR pszPath, UINT nWidth, UINT nHeight)
{
    HBITMAP hBitmpa = NULL;
    if ( (NULL != pszPath) && (nWidth > 0.0) && (nHeight > 0.0) )
    {
        IShellItemImageFactory *psif = NULL;
        SIZE size = { nWidth, nHeight };
        HRESULT hr = ::SHCreateItemFromParsingName(pszPath, NULL, IID_PPV_ARGS(&psif));
        if ( SUCCEEDED(hr) && (NULL != psif) )
        {
            psif->GetImage(size, SIIGBF_THUMBNAILONLY, &hBitmpa);
        }
        SAFE_RELEASE(psif);
    }

    return hBitmpa;
}

//////////////////////////////////////////////////////////////////////////

HBITMAP SdkCommonHelper::GetWindowSnapshot(IN HWND hWnd, IN const RECT* lpRect)
{
    RECT rect = { 0 };

    if (NULL == lpRect)
    {
        // If hWnd is NULL, get the entire screen rectangle.
        if (NULL != hWnd)
        {
            GetClientRect(hWnd, &rect);
        }
        else
        {
            HDC hdcScreen = CreateDC(L"DISPLAY", NULL, NULL, NULL);
            rect.right = GetDeviceCaps(hdcScreen, HORZRES);
            rect.bottom = GetDeviceCaps(hdcScreen, VERTRES);
            DeleteDC(hdcScreen);
        }
    }
    else
    {
        CopyRect(&rect, lpRect);
    }

    INT32 uRcWidth  = rect.right - rect.left;
    INT32 uRcHeight = rect.bottom - rect.top;
    if ( (uRcHeight <= 0) || (uRcHeight <= 0) )
    {
        return NULL;
    }

    HDC hdcSrc = GetDC(hWnd);
    HDC hdcMem = CreateCompatibleDC(hdcSrc);
    HBITMAP hbmpMem = CreateCompatibleBitmap(hdcSrc, uRcWidth, uRcHeight);
    HGDIOBJ holdBitmap = SelectObject(hdcMem, hbmpMem);

    BOOL bSuccess = BitBlt(hdcMem, 0, 0, uRcWidth, uRcHeight, hdcSrc, 0, 0, SRCCOPY);
    if ( !bSuccess )
    {
        DeleteObject(hbmpMem);
        hbmpMem = NULL;
    }

    SelectObject(hdcMem, holdBitmap);
    DeleteObject(hdcMem);
    ReleaseDC(hWnd, hdcSrc);

    return hbmpMem;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::SetWindowCenter(HWND hwnd, BOOL isCenterScreen)
{
    // The handle to window is invalid.
    if ( FALSE == IsWindow(hwnd) )
    {
        return FALSE;
    }

    // Get window width and height
    RECT rect;
    GetWindowRect(hwnd, &rect);
    LONG lWndWidth = rect.right - rect.left;
    LONG lWndHeight = rect.bottom - rect.top;

    // Get monitor information
    MONITORINFO mi;
    ZeroMemory(&mi, sizeof(MONITORINFO));
    mi.cbSize = sizeof(MONITORINFO);

    HWND hParent = ::GetParent(hwnd);
    if ( !isCenterScreen && (hParent != NULL) && IsWindow(hParent) && IsWindowVisible(hParent) )
    {
        // Get parent rect
        RECT rectParent;
        GetWindowRect(hParent, &rectParent);
        mi.rcWork.top = rectParent.top;
        mi.rcWork.bottom = rectParent.bottom;
        mi.rcWork.left = rectParent.left;
        mi.rcWork.right = rectParent.right;
    }
    else
    {
        GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi);
    }

    LONG lLeft = mi.rcWork.left + ( mi.rcWork.right - mi.rcWork.left ) / 2 - lWndWidth / 2;
    LONG lTop = mi.rcWork.top + ( mi.rcWork.bottom - mi.rcWork.top ) / 2 - lWndHeight / 2;

    return SetWindowPos(hwnd, HWND_TOP, lLeft, lTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////

void SdkCommonHelper::PrintDebugString(IN const WCHAR* pcsFormat, ...)
{
#ifdef _DEBUG

    if ( NULL == pcsFormat )
    {
        return;
    }

    size_t sizeOfTemplate = ::wcslen(pcsFormat);
    if ( sizeOfTemplate > 0 )
    {
        va_list argsList;
        va_start(argsList, pcsFormat);
        TCHAR szBuffer[1024] = { 0 };
        _vswprintf_s_l(szBuffer, 1024, pcsFormat, NULL, argsList);
        va_end(argsList);
        ::OutputDebugStringW(szBuffer);
    }
#else
    UNREFERENCED_PARAMETER(pcsFormat);
#endif // _DEBUG
}

////////////////////////////////////////////////////////////////////////////

void SdkCommonHelper::TranslateSysTimeToChar(double time, TCHAR *cBuffer, INT32 len)
{
    INT32  h = 0;
    INT32  m = 0;
    INT32  s = 0;
    double i = 0;

    i = time - INT32(time);
    h = INT32(time / 10000000) / 3600;
    m = INT32(time / 10000000) / 60 % 60;
    s = INT32(time / 10000000) % 60;
    if(i >= 0.5)
    {
        s = s + 1;
    }

    s = (s >= 60) ? 59 : s;
    m = (m >= 60) ? 59 : m;

    if(NULL != cBuffer)
    {
        if(0 == h)
        {
            swprintf_s(cBuffer, len, TEXT("%.2d:%.2d"), m, s);
        }
        else
        {
            swprintf_s(cBuffer, len, TEXT("%.2d:%.2d:%.2d"), h, m, s);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

HMODULE SdkCommonHelper::GetCurrentModuleHandle()
{
    // s_somevar must be static variable, otherwise the returned
    // HMODULE is not correct instance of current dll.
    static int s_somevar = 0;
    MEMORY_BASIC_INFORMATION mbi;
    if(!::VirtualQuery(&s_somevar, &mbi, sizeof(mbi)))
    {
        return NULL;
    }

    return static_cast<HMODULE>(mbi.AllocationBase);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::GetCurrentDateTime(OUT LPSYSTEMTIME psystemTime)
{
    if ( NULL == psystemTime )
    {
        return FALSE;
    }

    FILETIME fileTime;
    SYSTEMTIME systemTime;
    TIME_ZONE_INFORMATION tzinfo;
    GetSystemTimeAsFileTime(&fileTime);
    FileTimeToSystemTime(&fileTime, &systemTime);
    GetTimeZoneInformation(&tzinfo);

    return SystemTimeToTzSpecificLocalTime(&tzinfo, &systemTime, psystemTime);
}

//////////////////////////////////////////////////////////////////////////

WCHAR SdkCommonHelper::ToUpperInvariant(WCHAR input)
{
    wchar_t result;
    LONG lres = LCMapStringW(
        LOCALE_INVARIANT,
        LCMAP_UPPERCASE,
        &input,
        1,
        &result,
        1
        );
    if (lres == 0)
    {
        result = input;
    }
    return result;
}

//////////////////////////////////////////////////////////////////////////

INT32 SdkCommonHelper::OrdinalIgnoreCaseCompareStrings(LPCWSTR sz1, LPCWSTR sz2)
{
    if (sz1 == sz2)
    {
        return 0;
    }
    if (sz1 == NULL)
    {
        sz1 = L"";
    }

    if (sz2 == NULL)
    {
        sz2 = L"";
    }

    for (;; sz1++, sz2++)
    {
        const WCHAR c1 = *sz1;
        const WCHAR c2 = *sz2;
        // check for binary equality first
        if (c1 == c2)
        {
            if (c1 == 0)
            {
                return 0; // We have reached the end of both strings. No difference found.
            }
        }

        else
        {
            if (c1 == 0 || c2 == 0)
            {
                return (c1-c2); // We have reached the end of one string
            }

            const WCHAR u1 = ToUpperInvariant(c1);
            const WCHAR u2 = ToUpperInvariant(c2);
            if (u1 != u2)
            {
                return (u1-u2); // strings are different
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkCommonHelper::GetScale()
{
    HDC hdc = ::CreateDC(L"DISPLAY", NULL, NULL, NULL);
    FLOAT fScale = ::GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
    ::DeleteDC(hdc);

    return fScale;
}

//////////////////////////////////////////////////////////////////////////

void SdkCommonHelper::SplitString(OUT vector<wstring>& splitStrings, IN LPCWSTR lpSrc, IN INT32 nBufSize, IN LPCWSTR delimiters)
{
    if (NULL != lpSrc && nBufSize > 0)
    {
        INT32 nOffset = 0;
        INT32 nLength = nBufSize;
        INT32 nTempBufSize = nBufSize / 2;  // The characters number.
        LPWSTR lpTempSrc = new WCHAR[nLength];
        ZeroMemory(lpTempSrc, nLength);
        memcpy_s(lpTempSrc, nLength, lpSrc, nLength);

        WCHAR *pNextToken = NULL;
        WCHAR *pToken = wcstok_s(lpTempSrc, delimiters, &pNextToken);
        while (NULL != pToken)
        {
            splitStrings.push_back(wstring(pToken));
            nOffset += (wcslen(pToken) + 1);
            if (nOffset < nTempBufSize)
            {
                pNextToken = lpTempSrc + nOffset;
            }

            pToken = wcstok_s(NULL, delimiters, &pNextToken);
        }

        SAFE_DELETE_ARRAY(lpTempSrc);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::GetKnownFolderPath(OUT LPWSTR pszFolderPath, IN UINT nCsidl)
{
    return SHGetSpecialFolderPath(NULL, pszFolderPath, nCsidl, FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::CreateFolder(IN LPCWSTR lpPathName)
{
    if ( NULL == lpPathName )
    {
        return FALSE;
    }

    SHCreateDirectory(NULL, lpPathName);

    return (TRUE == PathFileExists(lpPathName));
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::DeleteFile(IN LPCWSTR lpFilePath, IN BOOL bToRecycleBin)
{
    if (NULL == lpFilePath || !::PathFileExists(lpFilePath))
    {
        return TRUE;
    }

    TCHAR tc[MAX_PATH] = {0};
    swprintf_s(tc, MAX_PATH, L"%s", lpFilePath);
    INT32 nLength = wcslen(tc);
    if(nLength < MAX_PATH)
    {
        tc[nLength] = L'\0';
        tc[nLength + 1] = L'\0';
    }

    SHFILEOPSTRUCT FileOp;
    ::ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
    FileOp.pFrom = tc;
    FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;
    if(bToRecycleBin)
    {
        FileOp.fFlags |= FOF_ALLOWUNDO;
    }
    FileOp.wFunc = FO_DELETE;

    return (0 == ::SHFileOperation(&FileOp));
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::DeleteFilesByPrefix(IN LPCTSTR lpFolderPath, LPCTSTR lpPrefix)
{
    if ( !PathFileExists(lpFolderPath) || (NULL == lpPrefix) || (0 == _tcslen(lpPrefix)) )
    {
        return FALSE;
    }

    vector<wstring> vctFiles;
    WCHAR szFolderPath[MAX_PATH] = { 0 };
    IShellFolder *psfDesktop = NULL;
    IShellFolder *psfWorkDir = NULL;
    IEnumIDList  *penumIDList = NULL;
    LPITEMIDLIST  pidworkDir = NULL;
    wcscpy_s(szFolderPath, MAX_PATH, lpFolderPath);

    // +1
    HRESULT hr = SHGetDesktopFolder(&psfDesktop);
    if (SUCCEEDED(hr))
    {
        // +2
        hr = psfDesktop->ParseDisplayName(NULL, NULL, szFolderPath, NULL, &pidworkDir, NULL);
    }

    if (SUCCEEDED(hr))
    {
        // +3
        hr = psfDesktop->BindToObject(pidworkDir, NULL, IID_PPV_ARGS(&psfWorkDir));
    }

    if (SUCCEEDED(hr))
    {
        // +4
        hr = psfWorkDir->EnumObjects(NULL, SHCONTF_NONFOLDERS, &penumIDList);
    }

    if (SUCCEEDED(hr))
    {
        ULONG celtFetched = 0;
        LPITEMIDLIST pidChild = NULL;
        while (SUCCEEDED(penumIDList->Next(1, &pidChild, &celtFetched)) && (1 == celtFetched))
        {
            LPITEMIDLIST pRealIDL = NULL;
            HRESULT hr = SHGetRealIDL(psfWorkDir, pidChild, &pRealIDL);
            if (SUCCEEDED(hr))
            {
                STRRET strName;
                hr = psfWorkDir->GetDisplayNameOf(pRealIDL, SHGDN_FORPARSING, &strName);
                if (SUCCEEDED(hr))
                {
                    WCHAR szName[MAX_PATH] = { 0 };
                    hr = StrRetToBuf(&strName, pRealIDL, szName, MAX_PATH);
                    if (SUCCEEDED(hr))
                    {
                        vctFiles.push_back(szName);
                    }
                }
                CoTaskMemFree(pRealIDL);
            }

            CoTaskMemFree(pidChild);
        }
    }

    for each(wstring str in vctFiles)
    {
        LPTSTR lpFileName = PathFindFileName(str.c_str());
        if (0 == _tcsnicmp(lpFileName, lpPrefix, _tcslen(lpPrefix)))
        {
            // Delete this file.
            DeleteFile(str.c_str(), FALSE);
        }
    }

    // -4
    SAFE_RELEASE(penumIDList);
    // -3
    SAFE_RELEASE(psfWorkDir);
    // -2
    CoTaskMemFree(pidworkDir);
    // -1
    SAFE_RELEASE(psfDesktop);

    return (vctFiles.size() > 0);
}

//////////////////////////////////////////////////////////////////////////

void SdkCommonHelper::ConverFileSizeToText(ULONGLONG ullSize, LPTSTR lpszSizeText, UINT32 uBufSize)
{
    if ( (NULL == lpszSizeText) || (0 == uBufSize) )
    {
        return;
    }

    const ULONGLONG B  = 1;           // Bytes.
    const ULONGLONG KB = 1024 * B;    // Kilobyte.
    const ULONGLONG MB = 1024 * KB;   // Megabyte.
    const ULONGLONG GB = 1024 * MB;   // Gigabyte.

    TCHAR szBuffer[MAX_PATH] = { 0 };

    if (ullSize >= GB)
    {
        _stprintf_s(szBuffer, MAX_PATH, _T("%3.2f GB"), (FLOAT)ullSize / (FLOAT)GB);
    }
    else if (ullSize >= MB)
    {
        _stprintf_s(szBuffer, MAX_PATH, _T("%3.2f MB"), (FLOAT)ullSize / (FLOAT)MB);
    }
    else if (ullSize >= KB)
    {
        _stprintf_s(szBuffer, MAX_PATH, _T("%3.2f KB"), (FLOAT)ullSize / (FLOAT)KB);
    }
    else if (ullSize >= B)
    {
        _stprintf_s(szBuffer, MAX_PATH, _T("%3.2f B"), (FLOAT)ullSize / (FLOAT)B);
    }
    else
    {
        _stprintf_s(szBuffer, MAX_PATH, _T("%d bytes"), ullSize);
    }

    _tcscpy_s(lpszSizeText, uBufSize, szBuffer);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::InitCurrentLanguage(LANGID &langId)
{
    BOOL bSuccess = FALSE;
    LANGID tempLandId = (0 == langId) ? GetUserDefaultUILanguage() : langId;
    switch(tempLandId)
    {
    case 3076: // Hong-kong ZH-HK
        tempLandId = 1028; // ZH-TW (Traditional)
        break;

    case 1027: // ca-es Catalan
    case 1069: // eu-es Basque
    case 1110: // gl-es Galician
        tempLandId = 3082; // es
        break;

    default:
        break;
    }

    langId = tempLandId;

    WCHAR szLocaleName[100] = { 0 };

    if ( 0 != LCIDToLocaleName(tempLandId, szLocaleName, 100, 0) )
    {
        ULONG numLnaguages = 1;
        bSuccess = SetThreadPreferredUILanguages(MUI_LANGUAGE_NAME, szLocaleName, &numLnaguages);
    }

    return bSuccess;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::FileTimeToLocalTime(IN const FILETIME& fileTime, OUT LPWSTR pszSystemTime)
{
    SYSTEMTIME systemTime;
    SYSTEMTIME localTime;
    ::FileTimeToSystemTime(&fileTime, &systemTime);
    TIME_ZONE_INFORMATION tzinfo;
    GetTimeZoneInformation(&tzinfo);

    BOOL bSuccess = ::SystemTimeToTzSpecificLocalTime(&tzinfo, &systemTime, &localTime);
    if (bSuccess)
    {
        StringCchPrintf(
            pszSystemTime,
            STRSAFE_MAX_CCH,
            _T("%04d/%02d/%02d  %02d:%02d:%02d"),
            localTime.wYear,
            localTime.wMonth,
            localTime.wDay,
            localTime.wHour,
            localTime.wMinute,
            localTime.wSecond);
    }

    return bSuccess;
}

//////////////////////////////////////////////////////////////////////////

CryptString SdkCommonHelper::StringToLowerCase(IN LPCWSTR lpSrcString)
{
    int nLength = wcslen(lpSrcString);
    if ( nLength > 0 )
    {
        nLength += 1;
        WCHAR *pTemp = new WCHAR[nLength];
        ZeroMemory(pTemp, nLength * sizeof(WCHAR));
        wcscpy_s(pTemp, nLength, lpSrcString);
        _wcslwr_s(pTemp, nLength);
        wstring strRet(pTemp);
        SAFE_DELETE_ARRAY(pTemp);
        return strRet;
    }

    return wstring(L"");
}

//////////////////////////////////////////////////////////////////////////

CryptString SdkCommonHelper::CreateNewGuid()
{
    static TCHAR chGuid[50] = {0};
    chGuid[0] = 0;
    GUID guid;

    if (S_OK == ::CoCreateGuid(&guid))
    {
        _stprintf_s(
            chGuid,
            50,
            L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
            guid.Data1,
            guid.Data2,
            guid.Data3,
            guid.Data4[0], guid.Data4[1],
            guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]);
    }

    return CryptString(chGuid);
}

//////////////////////////////////////////////////////////////////////////

wstring SdkCommonHelper::AnsiToUnicode(IN const CHAR* szSrc)
{
    if (NULL == szSrc)
    {
        return NULL;
    }

    UINT nACP = GetACP();
    int nLen = MultiByteToWideChar(nACP, 0, szSrc, -1, NULL, 0);
    if (0 == nLen)
    {
        return NULL;
    }

    WCHAR *pRet = new WCHAR[nLen];
    MultiByteToWideChar(nACP, 0, szSrc, -1, pRet, nLen);
    wstring strRet(pRet);
    SAFE_DELETE_ARRAY(pRet);
    return strRet;
}

//////////////////////////////////////////////////////////////////////////

string SdkCommonHelper::UnicodeToAnsi(IN const WCHAR* szSrc)
{
    if (NULL == szSrc)
    {
        return NULL;
    }

    UINT nACP = GetACP();
    int nLen = WideCharToMultiByte(nACP, 0, szSrc, -1, NULL, 0, NULL, NULL);
    if (0 == nLen)
    {
        return NULL;
    }

    CHAR *pRet = new CHAR[nLen];
    WideCharToMultiByte(nACP, 0, szSrc, -1, pRet, nLen, NULL, NULL);
    string strRet(pRet);
    SAFE_DELETE_ARRAY(pRet);
    return strRet;
}

//////////////////////////////////////////////////////////////////////////

CryptString SdkCommonHelper::LoadResString(HINSTANCE hInst, UINT32 uResId)
{
    static TCHAR szBuffer[1024] = { 0 };
    ZeroMemory(szBuffer, 1024 * sizeof(TCHAR));

    UINT nLength = ::LoadStringW(hInst, uResId, szBuffer, 1024);
    if(nLength <= 0)
    {
        ZeroMemory(szBuffer, 1024 * sizeof(TCHAR));
    }

    return szBuffer;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkCommonHelper::GetDefaultAssocApp(IN LPCWSTR pszextension, OUT LPWSTR pszappPath)
{
    HRESULT hr = E_FAIL;
    if ( (pszextension != NULL) && (pszappPath != NULL) )
    {
        DWORD bufferSize = MAX_PATH + 1;

        hr = AssocQueryString(
            ASSOCF_INIT_DEFAULTTOSTAR,
            ASSOCSTR_EXECUTABLE,
            pszextension,
            NULL,
            pszappPath,
            &bufferSize);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::AddToolTip(HINSTANCE hInst, IN HWND htoolWnd, IN LPCTSTR pszToolTipText, OUT HWND& htooltipWnd)
{
    if ( NULL == pszToolTipText )
    {
        return FALSE;
    }

    INITCOMMONCONTROLSEX initCtrls;
    initCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    initCtrls.dwICC = ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_WIN95_CLASSES;
    if ( !InitCommonControlsEx(&initCtrls) )
    {
        return FALSE;
    }

    HWND hParentWnd = GetParent(htoolWnd);
    htooltipWnd = CreateWindowEx(
        WS_EX_TOPMOST,      // Topmost.
        TOOLTIPS_CLASS,     // ToolTip
        NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        hParentWnd,
        NULL,
        hInst, NULL);

    if ( NULL == htooltipWnd )
    {
        return FALSE;
    }

    SendMessage(htooltipWnd, TTM_ACTIVATE, TRUE, 0);

    TOOLINFO toolInfo = { sizeof(TOOLINFO) };
    toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    toolInfo.uId = (UINT_PTR)htoolWnd;
    toolInfo.hwnd = hParentWnd;
    toolInfo.hinst = hInst;
    toolInfo.lpszText = const_cast<LPWSTR>(pszToolTipText);
    toolInfo.rect.left = toolInfo.rect.right = toolInfo.rect.bottom = toolInfo.rect.top = 0;
    SendMessage(htooltipWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&toolInfo);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::IsUserAdmin()
{
    BOOL isAdmin = FALSE;
    PSID psid = NULL;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

    BOOL bSuccess = AllocateAndInitializeSid(
        &NtAuthority,                  // A handle to an access token
        2,                             // Specifies the number of subauthorities to place in the SID
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &psid);

    if ( bSuccess )
    {
        bSuccess = CheckTokenMembership(NULL, psid, &isAdmin);
        isAdmin = (TRUE == bSuccess) ? isAdmin : FALSE;
    }

    if ( NULL != psid )
    {
        FreeSid(psid);
    }

    return isAdmin;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::IsEnableUAC()
{
    BOOL isEnableUAC = FALSE;
    OSVERSIONINFO osversioninfo;
    ZeroMemory(&osversioninfo, sizeof(osversioninfo));
    osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);
    BOOL bSuccess = GetVersionEx(&osversioninfo);
    if ( FALSE == bSuccess )
    {
        return FALSE;
    }

    // window vista or windows server 2008 or later operating system
    if ( osversioninfo.dwMajorVersion > 5 )
    {
        HKEY hKEY = NULL;
        DWORD dwType = REG_DWORD;
        DWORD dwEnableLUA = 0;
        DWORD dwSize = sizeof(DWORD);
        LPTSTR lpSubKey = 
            TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\");

        LONG status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKEY);

        if ( ERROR_SUCCESS == status )
        {
            status = RegQueryValueEx(
                hKEY,
                TEXT("EnableLUA"),
                NULL,
                &dwType,
                (BYTE*)&dwEnableLUA,
                &dwSize);

            if ( ERROR_SUCCESS == status )
            {
                isEnableUAC = (1 == dwEnableLUA) ? TRUE : FALSE;
            }

            RegCloseKey(hKEY);
        }
    }

    return isEnableUAC;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCommonHelper::IsWindows7()
{
    // Prepare the OSVERSIONINFOEX structure to indicate Windows 7
    OSVERSIONINFOEX osver = { 0 };
    osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osver.dwMajorVersion = 6;
    osver.dwMinorVersion = 1;
    osver.dwPlatformId = VER_PLATFORM_WIN32_NT;

    // Prepare the condition mask.
    DWORDLONG dwlConditionMask = 0;
    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_PLATFORMID, VER_EQUAL);

    BOOL isWin7 = VerifyVersionInfo(
        &osver,
        VER_MAJORVERSION | VER_MINORVERSION | VER_PLATFORMID,
        dwlConditionMask);

    return isWin7;
}

//////////////////////////////////////////////////////////////////////////

HICON SdkCommonHelper::GetIcon(IN LPCTSTR lpszfilePath)
{
    if ( (NULL == lpszfilePath) ||
         (0 == wcslen(lpszfilePath)) ||
         (FALSE == PathFileExists(lpszfilePath)) )
    {
        return NULL;
    }

    HICON hIcon = NULL;
    TCHAR szFolderPath[MAX_PATH] = { 0 };        // The folder path
    TCHAR *szFilePath = NULL;                    // The file name
    IShellFolder *pshdesktop = NULL;             // The pointer to desktop shell folder
    IShellFolder *pshworkdir = NULL;             // The pointer to work directory shell folder
    LPITEMIDLIST pidworkDir = NULL;              // The pidl of work directory
    LPITEMIDLIST pidworkfile = NULL;             // The pidl of file
    HRESULT hr = E_FAIL;

    _tcscpy_s(szFolderPath, MAX_PATH, lpszfilePath);
    PathRemoveFileSpec(szFolderPath);
    szFilePath = PathFindFileName(lpszfilePath);

    // +1 pshdesktop Retrieves the IShellFolder interface for the desktop folder.
    hr = SHGetDesktopFolder(&pshdesktop);
    if ( FAILED(hr) )
    {
        return NULL;
    }

    // +2 pidworkDir Translates the display name of a file object
    // or a folder into an item identifier list
    hr = pshdesktop->ParseDisplayName(NULL, NULL, szFolderPath, NULL, &pidworkDir, NULL);
    if ( FAILED(hr) )
    {
        goto Clearup;
    }

    // +3 pshworkdir Get work directory shell folder pointer by work directory pidl
    hr = pshdesktop->BindToObject(pidworkDir, NULL, IID_IShellFolder, (LPVOID*)&pshworkdir);
    if ( FAILED(hr) )
    {
        goto Clearup;
    }

    // +4 pidworkfile Get the work directory file pidl pointed by the pFile.
    hr = pshworkdir->ParseDisplayName(NULL, NULL, szFilePath, NULL, &pidworkfile, NULL);
    if ( FAILED(hr) )
    {
        goto Clearup;
    }

    hIcon = GetIcon(pshworkdir, pidworkfile);

Clearup:

    // -4 pidworkDir
    CoTaskMemFree(pidworkfile);
    // -3 pshworkdir
    SAFE_RELEASE(pshworkdir);
    // -2 pidworkDir
    CoTaskMemFree(pidworkDir);
    // -1 pshdesktop
    SAFE_RELEASE(pshdesktop);

    return hIcon;
}

//////////////////////////////////////////////////////////////////////////

HICON SdkCommonHelper::GetIcon(IN IShellFolder* pshWorkDir, IN LPCITEMIDLIST pidlWorkDir)
{
    if ( (NULL == pshWorkDir) || (NULL == pidlWorkDir) )
    {
        return NULL;
    }

    HICON hIcon = NULL;
    IExtractIcon* pExtractIcon = NULL;

    HRESULT hr = pshWorkDir->GetUIObjectOf(
        NULL,
        1,
        (LPCITEMIDLIST*)&pidlWorkDir,
        IID_IExtractIcon,
        NULL,
        (LPVOID*)&pExtractIcon);

    if ( SUCCEEDED(hr) )
    {
        int uIndex = 0;
        UINT dwFlags = IEIFLAG_ASPECT;
        TCHAR szIconLocation[MAX_PATH] = { 0 };
        hr = pExtractIcon->GetIconLocation(
            GIL_FORSHELL,
            szIconLocation,
            MAX_PATH,
            &uIndex,
            &dwFlags);

        if ( SUCCEEDED(hr) )
        {
            // Get the size of system icon.
            int nIconCx = ::GetSystemMetrics(SM_CXICON);
            //int nIconCy = ::GetSystemMetrics(SM_CYICON);
            UINT nIconSize = nIconCx;

            HICON hSmallIcon = NULL;
            HICON hLargeIcon = NULL;

            // Get the large icon. Low word is the size of large icon.
            hr = pExtractIcon->Extract(
                szIconLocation,
                uIndex,
                &hLargeIcon,
                NULL,
                nIconSize);

            if (NULL == hLargeIcon)
            {
                // The high word is the size of small icon.
                nIconSize = (UINT)(nIconCx << 16);
                hr = pExtractIcon->Extract(
                    szIconLocation,
                    uIndex,
                    NULL,
                    &hSmallIcon,
                    nIconSize);

                hIcon = hSmallIcon;
            }
            else
            {
                hIcon = hLargeIcon;
            }
        }
    }

    SAFE_RELEASE(pExtractIcon);

    return hIcon;
}

//////////////////////////////////////////////////////////////////////////

HBITMAP SdkCommonHelper::GetItemBitmap(IN LPCITEMIDLIST pidlItem, UINT nWidth, UINT nHeight)
{
    HBITMAP hBitmap = NULL;

    if ( NULL != pidlItem )
    {
        IShellItemImageFactory *psif = NULL;
        SIZE size = { nWidth, nHeight };
        HRESULT hr = SHCreateItemFromIDList(pidlItem, IID_PPV_ARGS(&psif));
        if (SUCCEEDED(hr))
        {
            psif->GetImage(size, SIIGBF_THUMBNAILONLY, &hBitmap);
            if (NULL == hBitmap)
            {
                psif->GetImage(size, SIIGBF_ICONONLY, &hBitmap);
            }
        }
    }

    return hBitmap;
}
