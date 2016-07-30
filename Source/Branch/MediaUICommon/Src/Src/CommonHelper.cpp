/*!
* @file CommonHelper.cpp 
* 
* @brief The implementation of common functions defined in CommonHelper.h file.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#include "CommonHelper.h"
#include "DriversManager.h"
#include <math.h>

USING_NAMESPACE_COMMON

HBITMAP CommonHelper::ExtractFileIcon(IN LPCTSTR pszPath, UINT nWidth, UINT nHeight)
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

HBITMAP CommonHelper::ExtractFileThumbnail(IN LPCTSTR pszPath, UINT nWidth, UINT nHeight)
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

HBITMAP CommonHelper::GetWindowSnapshot(IN HWND hWnd, IN const RECT* lpRect)
{
    RECT rect = { 0 };

    if (NULL == lpRect)
    {
        GetClientRect(hWnd, &rect);
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
    ReleaseDC(NULL, hdcSrc);

    return hbmpMem;
}

//////////////////////////////////////////////////////////////////////////

BOOL CommonHelper::SetWindowCenter(HWND hwnd, BOOL isCenterScreen)
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

void CommonHelper::PrintDebugString(IN const WCHAR* pcsFormat, ...)
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

void CommonHelper::TranslateSysTimeToChar(double time, TCHAR *cBuffer, INT32 len)
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

HMODULE CommonHelper::GetCurrentModuleHandle()
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

//////////////////////////////////////////////////////////////////////////

BOOL CommonHelper::GetCurrentDateTime(OUT LPSYSTEMTIME psystemTime)
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

WCHAR CommonHelper::ToUpperInvariant(WCHAR input)
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

INT32 CommonHelper::OrdinalIgnoreCaseCompareStrings(LPCWSTR sz1, LPCWSTR sz2)
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

FLOAT CommonHelper::GetScale()
{
    HDC hdc = ::CreateDC(L"DISPLAY", NULL, NULL, NULL);
    FLOAT fScale = ::GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
    ::DeleteDC(hdc);

    return fScale;
}

//////////////////////////////////////////////////////////////////////////

void CommonHelper::SplitString(OUT vector<wstring>& splitStrings, IN LPCWSTR lpSrc, IN INT32 nBufSize, IN LPCWSTR delimiters)
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

BOOL CommonHelper::GetKnownFolderPath(OUT LPWSTR pszFolderPath, IN UINT nCsidl)
{
    return SHGetSpecialFolderPath(NULL, pszFolderPath, nCsidl, FALSE);
}

//////////////////////////////////////////////////////////////////////////

BOOL CommonHelper::CreateFolder(IN LPCWSTR lpPathName)
{
    if ( NULL == lpPathName )
    {
        return FALSE;
    }

    SHCreateDirectory(NULL, lpPathName);

    return (TRUE == PathFileExists(lpPathName));
}

//////////////////////////////////////////////////////////////////////////

BOOL CommonHelper::DeleteFile(IN LPCWSTR lpFilePath, IN BOOL bToRecycleBin)
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

BOOL CommonHelper::DeleteFilesByPrefix(IN LPCTSTR lpFolderPath, LPCTSTR lpPrefix)
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

void CommonHelper::ConverFileSizeToText(ULONGLONG ullSize, LPTSTR lpszSizeText, UINT32 uBufSize)
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

BOOL CommonHelper::InitCurrentLanguage(LANGID &langId)
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

BOOL CommonHelper::FileTimeToLocalTime(IN const FILETIME& fileTime, OUT LPWSTR pszSystemTime)
{
    SYSTEMTIME systemTime;
    SYSTEMTIME localTime;
    ::FileTimeToSystemTime(&fileTime, &systemTime);
    TIME_ZONE_INFORMATION tzinfo;
    GetTimeZoneInformation(&tzinfo);

    BOOL bSuccess = ::SystemTimeToTzSpecificLocalTime(&tzinfo, &systemTime, &localTime);
    if (bSuccess)
    {
        StringCchPrintf(pszSystemTime, STRSAFE_MAX_CCH, _T("%04d/%02d/%02d  %02d:%02d:%02d"),
            localTime.wYear, localTime.wMonth, localTime.wDay, localTime.wHour, localTime.wMinute, localTime.wSecond);
    }

    return bSuccess;
}

//////////////////////////////////////////////////////////////////////////

wstring CommonHelper::StringToLowerCase(IN LPCWSTR lpSrcString)
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
