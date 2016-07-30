// helper to convert a data object with HIDA format or folder into a shell item
// note: if the data object contains more than one item this function will fail
// if you want to operate on the full selection use SHCreateShellItemArrayFromDataObject

#include <strsafe.h>
#include <commoncontrols.h>
#include <objbase.h>
#include <shlobj.h>             // shell stuff
#include <shlwapi.h>            // QISearch, easy way to implement QI
#include <windows.h>
#include <windowsx.h>           // for WM_COMMAND handling macros
#include <shellapi.h>

#pragma comment(lib, "shlwapi.lib")     // link to this
#pragma comment(lib, "comctl32.lib")    // link to this
#pragma comment(lib, "propsys.lib")     // link to this

__inline CLIPFORMAT GetClipboardFormat(CLIPFORMAT *pcf, PCWSTR pszForamt)
{
    if (*pcf == 0)
    {
        *pcf = (CLIPFORMAT)RegisterClipboardFormat(pszForamt);
    }
    return *pcf;
}


__inline HRESULT CreateItemFromObject(IUnknown *punk, REFIID riid, void **ppv)
{
    *ppv = NULL;

    PIDLIST_ABSOLUTE pidl;
    HRESULT hr = SHGetIDListFromObject(punk, &pidl);
    if (SUCCEEDED(hr))
    {
        hr = SHCreateItemFromIDList(pidl, riid, ppv);
        ILFree(pidl);
    }
    else
    {
        // perhaps the input is from IE and if so we can construct an item from the URL
        IDataObject *pdo;
        hr = punk->QueryInterface(IID_PPV_ARGS(&pdo));
        if (SUCCEEDED(hr))
        {
            static CLIPFORMAT g_cfURL = 0;

            FORMATETC fmte = { GetClipboardFormat(&g_cfURL, CFSTR_SHELLURL), NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
            STGMEDIUM medium;
            hr = pdo->GetData(&fmte, &medium);
            if (SUCCEEDED(hr))
            {
                PCSTR pszURL = (PCSTR)GlobalLock(medium.hGlobal);
                if (pszURL)
                {
                    WCHAR szURL[2048];
                    SHAnsiToUnicode(pszURL, szURL, ARRAYSIZE(szURL));
                    hr = SHCreateItemFromParsingName(szURL, NULL, riid, ppv);
                    GlobalUnlock(medium.hGlobal);
                }
                ReleaseStgMedium(&medium);
            }
            pdo->Release();
        }
    }
    return hr;
}

// The IDataObject passed in by OLE through the CDragDropHelper::Drop function is only valid until the Drop function returns
// This means the the IShellItemArray we are receiving may go bad as well since it is based on the incoming IDataObject
// Here we will create a stream and marshal the IShellItemArray which will create a copied IShellItemArray which does not depend on the IDataObject
__inline HRESULT CopyShellItemArray(IShellItemArray *psia, IShellItemArray **ppsiaOut)
{
    *ppsiaOut = NULL;
    IStream *pstm;
    HRESULT hr = CoMarshalInterThreadInterfaceInStream(__uuidof(psia), psia, &pstm);
    if (SUCCEEDED(hr))
    {
        hr = CoGetInterfaceAndReleaseStream(pstm, IID_PPV_ARGS(ppsiaOut));
        pstm = NULL; // released by CoGetInterfaceAndReleaseStream
    }
    return hr;
}


__inline HRESULT GetItemAt(IShellItemArray *psia, DWORD i, REFIID riid, void **ppv)
{
    *ppv = NULL;
    IShellItem *psi = NULL;     // avoid error C4701
    HRESULT hr = psia ? psia->GetItemAt(i, &psi) : E_NOINTERFACE;
    if (SUCCEEDED(hr))
    {
        hr = psi->QueryInterface(riid, ppv);
        psi->Release();
    }
    return hr;
}

__inline HRESULT SetItemImageImageInStaticControl(HWND hwndStatic, IShellItem *psi)
{
    HBITMAP hbmp = NULL;
    HRESULT hr = S_OK;
    if (psi)
    {
        IShellItemImageFactory *psiif;
        hr = psi->QueryInterface(IID_PPV_ARGS(&psiif));
        if (SUCCEEDED(hr))
        {
            RECT rc;
            GetWindowRect(hwndStatic, &rc);
            const UINT dxdy = min(rc.right - rc.left, rc.bottom - rc.top);    // make it square
            const SIZE size = { dxdy, dxdy };

            hr = psiif->GetImage(size, SIIGBF_RESIZETOFIT, &hbmp);
            psiif->Release();
        }
    }

    if (SUCCEEDED(hr))
    {
        HGDIOBJ hgdiOld = (HGDIOBJ) SendMessage(hwndStatic, STM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) hbmp);
        if (hgdiOld)
        {
            DeleteObject(hgdiOld);  // if there was an old one clean it up
        }
    }

    return hr;
}

__inline HRESULT ShellAttributesToString(SFGAOF sfgaof, PWSTR *ppsz)
{

#define MAP_ENTRY(x) {L#x, x}

    *ppsz = NULL;

    static const struct { PCWSTR pszName; SFGAOF sfgaof; } c_rgItemAttributes[] =
    {
        // note, SFGAO_HASSUBFOLDER is too expesnive to compute
        // and has been excluded from this list
        MAP_ENTRY(SFGAO_STREAM),
        MAP_ENTRY(SFGAO_FOLDER),
        MAP_ENTRY(SFGAO_FILESYSTEM),
        MAP_ENTRY(SFGAO_FILESYSANCESTOR),
        MAP_ENTRY(SFGAO_STORAGE),
        MAP_ENTRY(SFGAO_STORAGEANCESTOR),
        MAP_ENTRY(SFGAO_LINK),
        MAP_ENTRY(SFGAO_CANCOPY),
        MAP_ENTRY(SFGAO_CANMOVE),
        MAP_ENTRY(SFGAO_CANLINK),
        MAP_ENTRY(SFGAO_CANRENAME),
        MAP_ENTRY(SFGAO_CANDELETE),
        MAP_ENTRY(SFGAO_HASPROPSHEET),
        MAP_ENTRY(SFGAO_DROPTARGET),
        MAP_ENTRY(SFGAO_ENCRYPTED),
        MAP_ENTRY(SFGAO_ISSLOW),
        MAP_ENTRY(SFGAO_GHOSTED),
        MAP_ENTRY(SFGAO_SHARE),
        MAP_ENTRY(SFGAO_READONLY),
        MAP_ENTRY(SFGAO_HIDDEN),
        MAP_ENTRY(SFGAO_REMOVABLE),
        MAP_ENTRY(SFGAO_COMPRESSED),
        MAP_ENTRY(SFGAO_BROWSABLE),
        MAP_ENTRY(SFGAO_NONENUMERATED),
        MAP_ENTRY(SFGAO_NEWCONTENT),
    };

    WCHAR sz[512] = {};
    PWSTR psz = sz;
    size_t cch = ARRAYSIZE(sz);

    StringCchPrintfEx(psz, cch, &psz, &cch, 0, L"0x%08X", sfgaof);

    for (int i = 0; i < ARRAYSIZE(c_rgItemAttributes); i++)
    {
        if (c_rgItemAttributes[i].sfgaof & sfgaof)
        {
            StringCchPrintfEx(psz, cch, &psz, &cch, 0, L", %s", c_rgItemAttributes[i].pszName);
        }
    }
    return SHStrDup(sz, ppsz);
}

__inline HRESULT ResultFromKnownLastError() { const DWORD err = GetLastError(); return err == ERROR_SUCCESS ? E_FAIL : HRESULT_FROM_WIN32(err); }

// map Win32 APIs that follow the return BOOL/set last error protocol
// into HRESULT
//
// example: MoveFileEx()

__inline HRESULT ResultFromWin32Bool(BOOL b)
{
    return b ? S_OK : ResultFromKnownLastError();
}

__inline HRESULT ShellExecuteItem(HWND hwnd, PCWSTR pszVerb, IShellItem *psi)
{
    // how to activate a shell item, use ShellExecute().
    PIDLIST_ABSOLUTE pidl;
    HRESULT hr = SHGetIDListFromObject(psi, &pidl);
    if (SUCCEEDED(hr))
    {
        SHELLEXECUTEINFO ei = { sizeof(ei) };
        ei.fMask = SEE_MASK_INVOKEIDLIST;
        ei.hwnd = hwnd;
        ei.nShow = SW_NORMAL;
        ei.lpIDList = pidl;
        ei.lpVerb = pszVerb;

        hr = ResultFromWin32Bool(ShellExecuteEx(&ei));

        CoTaskMemFree(pidl);
    }
    return hr;
}