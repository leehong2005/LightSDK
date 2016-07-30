// SamplePreviewHandler.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SampleDragDrop.h"

SampleDragDropWin::SampleDragDropWin()
{
    m_hInst = NULL;
    m_hWnd  = NULL;
    m_szDispName[0] = 0;
    m_psiaDrop = NULL;
    m_curDropEffect = DROPEFFECT_COPY;
}

//////////////////////////////////////////////////////////////////////////

SampleDragDropWin::~SampleDragDropWin()
{
    SAFE_RELEASE(m_psiaDrop);
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SampleDragDropWin::DoModal( HINSTANCE hInst, HWND hWndParent )
{
    m_hInst = hInst;

    INT_PTR nRet = DialogBoxParam(
        hInst,
        MAKEINTRESOURCE(IDD_DIALOG),
        hWndParent,
        SampleDragDropWin::DialogProc,
        (LPARAM)this);

    return nRet;
}

//////////////////////////////////////////////////////////////////////////

DROPEFFECT SampleDragDropWin::OnDragEnter(HWND hWnd, IDataObject *pDataObj, DWORD dwKeyState, POINT pt)
{
    HRESULT hr = pDataObj->QueryInterface(IID_IDataObject, (void**)&m_pDataObject);
    if ( SUCCEEDED(hr) )
    {
        IShellItem *psi = NULL;
        hr = CreateItemFromObject(pDataObj, IID_PPV_ARGS(&psi));
        if ( SUCCEEDED(hr) )
        {
            PWSTR pszName = NULL;
            hr = psi->GetDisplayName(SIGDN_NORMALDISPLAY, &pszName);
            if ( SUCCEEDED(hr) )
            {
                wcscpy_s(m_szDispName, MAX_PATH, pszName);
                WCHAR szBuf[MAX_PATH] = { 0 };
                swprintf_s(szBuf, L"%s to SampleDragDropWindow", m_szDispName);

                DROPDESCRIPTION dd = { DROPIMAGE_COPY };
                dd.type = DROPIMAGE_COPY;
                wcscpy_s(dd.szMessage, MAX_PATH, L"Move %1");
                wcscpy_s(dd.szInsert, MAX_PATH, szBuf);
                SdkDataObjectHelper::SetDropTip(pDataObj, &dd);

                CoTaskMemFree(pszName);
                pszName = NULL;
            }
        }

        SAFE_RELEASE(psi);
    }

    return DROPEFFECT_COPY;
}

//////////////////////////////////////////////////////////////////////////

DROPEFFECT SampleDragDropWin::OnDragOver(HWND hWnd, DWORD dwKeyState, POINT pt)
{
    DROPEFFECT dropEffect = FilterDropEffect(dwKeyState, DROPEFFECT_COPY);

    if ( m_curDropEffect != dropEffect )
    {
        m_curDropEffect = dropEffect;

        DROPDESCRIPTION dd = { DROPIMAGE_COPY };
        if ( DROPEFFECT_MOVE == dropEffect )
        {
            //SdkDataObjectHelper::GetDropTip(m_pDataObject, &dd);
            WCHAR szBuf[MAX_PATH] = { 0 };
            swprintf_s(szBuf, L"Delete %s", m_szDispName);

            dd.type = DROPIMAGE_WARNING;
            wcscpy_s(dd.szMessage, MAX_PATH, szBuf);
        }
        else
        {
            WCHAR szBuf[MAX_PATH] = { 0 };
            swprintf_s(szBuf, L"%s to SampleDragDropWindow", m_szDispName);

            dd.type = DROPIMAGE_COPY;
            wcscpy_s(dd.szMessage, MAX_PATH, L"Move %1");
            wcscpy_s(dd.szInsert, MAX_PATH, szBuf);
        }

        SdkDataObjectHelper::SetDropTip(m_pDataObject, &dd);
    }

    return dropEffect;
}

//////////////////////////////////////////////////////////////////////////

void SampleDragDropWin::OnDragLeave(HWND hWnd)
{
    SAFE_RELEASE(m_pDataObject);
    m_szDispName[0] = 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleDragDropWin::OnDrop(HWND hWnd, IDataObject *pDataObj, DWORD dwKeyState, POINT pt)
{
    IShellItemArray *psia = NULL;
    HRESULT hr = SHCreateShellItemArrayFromDataObject(m_pDataObject, IID_PPV_ARGS(&psia));
    if ( SUCCEEDED(hr) )
    {
        SAFE_RELEASE(m_psiaDrop);
        hr = CopyShellItemArray(psia, &m_psiaDrop);
        if ( SUCCEEDED(hr) )
        {
            OnBindUI();
        }
    }

    m_curDropEffect = DROPIMAGE_COPY;
    SAFE_RELEASE(m_pDataObject);

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SampleDragDropWin::OnBindUI()
{
    HRESULT hr = (NULL != m_psiaDrop) ? S_OK : E_FAIL;

    if ( SUCCEEDED(hr) )
    {
        IShellItem2 *psi = NULL;
        IShellItemArray *psia = NULL;

        HRESULT hr = m_psiaDrop->QueryInterface(&psia);
        if ( SUCCEEDED(hr) )
        {
            hr = GetItemAt(psia, 0, IID_IShellItem2, (void**)&psi);
        }

        if ( SUCCEEDED(hr) )
        {
            SetDlgItemText(m_hWnd, IDC_STATIC, L"Start Drag Drop by clicking on icon");
            SetItemImageImageInStaticControl(GetDlgItem(m_hWnd, IDC_IMAGE), psi);
        }

        // Get display name.
        PWSTR psz = NULL;
        hr = psi->GetDisplayName(SIGDN_NORMALDISPLAY, &psz);
        if ( SUCCEEDED(hr) )
        {
            wcscpy_s(m_szDispName, MAX_PATH, psz);
            SetDlgItemText(m_hWnd, IDC_NAME, psz);
            CoTaskMemFree(psz);
            psz = NULL;
        }

        SFGAOF sfgaof;
        hr = psi->GetAttributes(SFGAO_CANCOPY| SFGAO_CANLINK | SFGAO_CANMOVE, &sfgaof);
        if (SUCCEEDED(hr))
        {
            hr = ShellAttributesToString(sfgaof, &psz);
            if (SUCCEEDED(hr))
            {
                SetDlgItemText(m_hWnd, IDC_ATTRIBUTES, psz);
                CoTaskMemFree(psz);
            }
        }

        SAFE_RELEASE(psi);
        SAFE_RELEASE(psia);
    }
    else
    {
        SetItemImageImageInStaticControl(GetDlgItem(m_hWnd, IDC_IMAGE), NULL);
        SetDlgItemText(m_hWnd, IDC_STATIC, L"Drop An Item Here");
        SetDlgItemText(m_hWnd, IDC_NAME, L"");
        SetDlgItemText(m_hWnd, IDC_ATTRIBUTES, L"");
    }

    ::EnableWindow(GetDlgItem(m_hWnd, IDC_CUSTOM_DATAOBJECT), SUCCEEDED(hr) ? TRUE : FALSE);
    ::EnableWindow(GetDlgItem(m_hWnd, IDC_OPEN), SUCCEEDED(hr) ? TRUE : FALSE);
    ::EnableWindow(GetDlgItem(m_hWnd, IDC_CLEAR), SUCCEEDED(hr) ? TRUE : FALSE);
}

//////////////////////////////////////////////////////////////////////////

void SampleDragDropWin::OnBeginDrag(HWND hwndDragBegin)
{
    POINT pt;
    GetCursorPos(&pt);
    MapWindowPoints(HWND_DESKTOP, hwndDragBegin, &pt, 1);   // screen -> client

    if ( SdkDragDropHelper::CheckForBeginDrag(hwndDragBegin, pt.x, pt.y) )
    {
        IDataObject *pdtobj = NULL;
        HRESULT hr = GetDataObject(hwndDragBegin, &pdtobj);
        if ( SUCCEEDED(hr) )
        {
            DWORD dwEffectResult = DROPEFFECT_NONE;
            hr = SHDoDragDrop(hwndDragBegin, pdtobj, NULL, GetDropEffects(), &dwEffectResult);
        }
        SAFE_RELEASE(pdtobj);
    }
}

//////////////////////////////////////////////////////////////////////////

HRESULT SampleDragDropWin::GetDataObject(HWND hwndDragBegin, IDataObject **ppdtobj)
{
    BOOL isUserData = IsDlgButtonChecked(m_hWnd, IDC_CUSTOM_DATAOBJECT) ? TRUE : FALSE;

    if ( isUserData )
    {
        HRESULT hr = SdkDataObjectHelper::CreateDataObject(IID_PPV_ARGS(ppdtobj));
        if ( SUCCEEDED(hr) )
        {
            // Create data object from shell item array.
            IDataObject *pdt = NULL;
            IShellItemArray *psia = NULL;
            if ( NULL != m_psiaDrop )
            {
                HRESULT hr = m_psiaDrop->QueryInterface(IID_PPV_ARGS(&psia));
                if ( SUCCEEDED(hr) )
                {
                    hr = psia->BindToHandler(NULL, BHID_DataObject, IID_PPV_ARGS(&pdt));
                }
            }
            SAFE_RELEASE(psia);

            // Copy the created data object from that one from shell item array.
            if ( SUCCEEDED(hr) )
            {
                SdkDataObjectHelper::CopyDataObject(pdt, (*ppdtobj));
            }

            SdkDragSourceHelper dragSrcHelper;

            // Enable drop tips.
            dragSrcHelper.SetDropDescEnable();

            // We need to make a copy of the HBITMAP held by the static control
            // as InitializeFromBitmap() takes owership of this.
            const HBITMAP hbmp = (HBITMAP)CopyImage(
                (HBITMAP)SendMessage(hwndDragBegin, STM_GETIMAGE, IMAGE_BITMAP, 0),
                IMAGE_BITMAP, 0, 0, 0);

            // alternate, load the bitmap from a resource
            // HBITMAP hbmp = (HBITMAP)LoadImage(NULL,
            //    MAKEINTRESOURCE(OBM_CLOSE), IMAGE_BITMAP, 128, 128, 0);
            dragSrcHelper.InitializeFromBitmap(hwndDragBegin, hbmp, (*ppdtobj));
        }
    }
    else
    {
        (*ppdtobj) = NULL;
        IShellItemArray *psia = NULL;
        if ( NULL != m_psiaDrop )
        {
            HRESULT hr = m_psiaDrop->QueryInterface(IID_PPV_ARGS(&psia));
            if ( SUCCEEDED(hr) )
            {
                hr = psia->BindToHandler(NULL, BHID_DataObject, IID_PPV_ARGS(ppdtobj));
            }
        }
        SAFE_RELEASE(psia);
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

DWORD SampleDragDropWin::GetDropEffects()
{
    DWORD dwEffect;

    BOOL isUserData = IsDlgButtonChecked(m_hWnd, IDC_CUSTOM_DATAOBJECT) ? TRUE : FALSE;

    if ( isUserData )
    {
        dwEffect = DROPEFFECT_MOVE | DROPEFFECT_COPY | DROPEFFECT_LINK;
    }
    else
    {
        dwEffect = DROPEFFECT_NONE;
        IShellItemArray *psiaItems = NULL;
        if ( NULL != m_psiaDrop )
        {
            HRESULT hr = m_psiaDrop->QueryInterface(IID_PPV_ARGS(&psiaItems));
            if ( SUCCEEDED(hr) )
            {
                psiaItems->GetAttributes(
                    SIATTRIBFLAGS_AND,
                    DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK,
                    &dwEffect);
            }
        }

        SAFE_RELEASE(psiaItems);
    }

    return dwEffect;
}

//////////////////////////////////////////////////////////////////////////

void SampleDragDropWin::OnOpen()
{
    IShellItem2 *psi = NULL;
    IShellItemArray *psia = NULL;

    if ( NULL != m_psiaDrop )
    {
        HRESULT hr = m_psiaDrop->QueryInterface(&psia);
        if ( SUCCEEDED(hr) )
        {
            hr = GetItemAt(psia, 0, IID_IShellItem2, (void**)&psi);
        }

        if ( SUCCEEDED(hr) )
        {
            hr = ShellExecuteItem(m_hWnd, L"open", psi);
        }
    }

    SAFE_RELEASE(psi);
    SAFE_RELEASE(psia);
}

//////////////////////////////////////////////////////////////////////////

void SampleDragDropWin::OnInitDlg(HWND hwndDlg)
{
    m_hWnd = hwndDlg;

    HICON hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_SAMPLEDRAGDROP));
    ::SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
    ::SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
    ::DestroyIcon(hIcon);

    SdkDropTarget::RegisterDropTarget(hwndDlg, NULL);

    OnBindUI();
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SampleDragDropWin::OnDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        OnInitDlg(hwndDlg);
        break;

    case WM_CLOSE:
        DestroyWindow(hwndDlg);
        break;

    case WM_COMMAND:
        {
            int idCmd = LOWORD(wParam);
            switch (idCmd)
            {
            case IDOK:
            case IDCANCEL:
                return EndDialog(hwndDlg, 0);

            case IDC_OPEN:
                OnOpen();
                break;

            case IDC_CLEAR:
                SAFE_RELEASE(m_psiaDrop);
                OnBindUI();
                break;

            case IDC_IMAGE:
                switch (HIWORD(wParam))
                {
                case STN_CLICKED:
                    OnBeginDrag(GetDlgItem(hwndDlg, IDC_IMAGE));
                    break;

                case STN_DBLCLK:
                    OnOpen();
                    break;
                }
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return FALSE;
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK SampleDragDropWin::DialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    SampleDragDropWin *pThis = static_cast<SampleDragDropWin*>(
        (LPVOID)GetWindowLongPtr(hwndDlg, GWLP_USERDATA));

    if (WM_INITDIALOG == uMsg)
    {
        pThis = static_cast<SampleDragDropWin*>((LPVOID)lParam);
        if (NULL != lParam)
        {
            SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (INT_PTR)pThis);
        }
    }

    return (NULL != pThis) ? pThis->OnDialogProc(hwndDlg, uMsg, wParam, lParam) : FALSE;
}
