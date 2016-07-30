// SamplePreviewHandler.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SamplePreviewHandler.h"
#include "SdkCommonInclude.h"


#define  BUTTON_PREVIEW  1001


SamplePreviewHandler::SamplePreviewHandler():m_hinstance(NULL),
                                             m_hWnd(NULL)
{
    m_sdkPriveHandle = new SdkPreviewHandler();
}

//////////////////////////////////////////////////////////////////////////

SamplePreviewHandler::~SamplePreviewHandler()
{
    if (NULL != m_sdkPriveHandle)
    {
        m_sdkPriveHandle->DetachPreview();
    }

    SAFE_DELETE(m_sdkPriveHandle);
}

//////////////////////////////////////////////////////////////////////////

HRESULT SamplePreviewHandler::Initialize( HINSTANCE hInstance )
{
    m_hinstance = hInstance;
    HWND hwndDlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DLG_PREVIEWHANDLER), NULL, (DLGPROC)DialogProc, (LPARAM)this); 
    HRESULT hr = hwndDlg ? S_OK :  E_FAIL;
    m_hWnd = hwndDlg;

    if (SUCCEEDED(hr))
    {
        SdkCommonHelper::SetWindowCenter(hwndDlg);
        ShowWindow(hwndDlg, SW_SHOWNORMAL);
        UpdateWindow(hwndDlg);
    }

    return  hr;
}

//////////////////////////////////////////////////////////////////////////

void SamplePreviewHandler::RunMessageLoop()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

//////////////////////////////////////////////////////////////////////////

void SamplePreviewHandler::OnCommandHandle( HWND hwnd, WPARAM wParam)
{
    switch (LOWORD(wParam))
    {
    case IDC_BUTTON_BROWSER:
        {
            if (BN_CLICKED == HIWORD(wParam))
            {
                TCHAR filename[MAX_PATH];
                ZeroMemory(filename,MAX_PATH);
                if(ShowOpenFileDialog(hwnd, filename))
                {
                    RECT rect = { 0 };
                    HWND hWndAttach = GetDlgItem(hwnd, IDC_STATIC_ATTACH);
                    GetClientRect(hWndAttach, &rect);

                    m_sdkPriveHandle->DetachPreview();

                    vector<HWND> hWndChildren;
                    EnumChildren(hWndChildren, hWndAttach);
                    for (int i = 0; i < (int)hWndChildren.size(); ++i)
                    {
                        SendMessage(hWndChildren[i], WM_CLOSE, 0, 0);
                        DestroyWindow(hWndChildren[i]);
                    }

                    InflateRect(&rect, -2, -2);
                    BOOL isOK = m_sdkPriveHandle->AttachPreview(hWndAttach, &rect, filename);
                    if ( isOK )
                    {
                        SetDlgItemText(hwnd, IDC_STATIC_TIP, filename);
                    }
                    else
                    {
                        SetDlgItemText(hwnd, IDC_STATIC_TIP, L"No preview available.");
                    }

                    // Make its children window update.
                    hWndChildren.clear();
                    EnumChildren(hWndChildren, hWndAttach);
                    for (int i = 0; i < (int)hWndChildren.size(); ++i)
                    {
                        HWND hWndChild = hWndChildren[i];
                        InvalidateRect(hWndChild, NULL, TRUE);
                        UpdateWindow(hWndChild);
                    }

                    InvalidateRect(m_hWnd, NULL, TRUE);
                    UpdateWindow(m_hWnd);
                }
            }
        }

        break;
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SamplePreviewHandler::ShowOpenFileDialog( HWND hWnd, OUT LPWSTR pszselfile )
{
    OPENFILENAME ofn;
    TCHAR opfile[MAX_PATH];
    ZeroMemory(opfile,MAX_PATH);
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = hWnd;
    ofn.lpstrFile       = pszselfile;
    ofn.nMaxFile        = MAX_PATH;
    ofn.lpstrFilter     = L"All Files\0*.*\0";
    ofn.nFilterIndex    = 1;
    ofn.lpstrFileTitle  = NULL;
    ofn.nMaxFileTitle   = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    WCHAR szDesktopPath[MAX_PATH] = {0};
    ZeroMemory(szDesktopPath,sizeof(WCHAR));
    SHGetSpecialFolderPath(NULL, szDesktopPath, CSIDL_DESKTOP, FALSE);
    ofn.lpstrInitialDir = szDesktopPath;
    return GetOpenFileName(&ofn);
}

//////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK SamplePreviewHandler::DialogProc( HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    if (uMsg == WM_INITDIALOG)
    {
        SamplePreviewHandler *pThis = static_cast<SamplePreviewHandler*>((LPVOID)lParam);
        if (NULL != pThis)
        {
            SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG_PTR)pThis);

            HICON hIcon = LoadIcon(pThis->m_hinstance, MAKEINTRESOURCE(IDI_SAMPLEPREVIEWHANDLER));
            SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
            SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
            DestroyIcon(hIcon);

            HWND hWndCombo = GetDlgItem(hwndDlg, IDC_COMBO1);
            pThis->m_sdkPriveHandle->EnumInstalledHandler();
            DWORD dwCount  = pThis->m_sdkPriveHandle->GetInstalledHandlerCount();
            PREVIEWHANDLERINFO previewHandleInfos = { 0 };
            for (int i = 0; i < (int)dwCount; ++i)
            {
                BOOL isOK = pThis->m_sdkPriveHandle->GetInstalledHandlerItem(i , &previewHandleInfos);
                if ( isOK )
                {
                    SendMessage(hWndCombo, CB_ADDSTRING, 0, (LPARAM)previewHandleInfos.szName);
                }
            }

            SendMessage(hWndCombo, CB_SETCURSEL, (WPARAM)0, 0);
        }

        HWND hWndStatic = GetDlgItem(hwndDlg, IDC_STATIC_ATTACH);
        SetWindowSubclass(hWndStatic, SamplePreviewHandler::SubClassProc, 1000, (DWORD_PTR)pThis);

        return FALSE;
    }

    SamplePreviewHandler *pThis = (SamplePreviewHandler*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
    if (NULL != pThis)
    {
        return pThis->OnDialogProc(hwndDlg, uMsg, wParam, lParam);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

LRESULT SamplePreviewHandler::OnDialogProc( HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    switch (uMsg)
    {
    case WM_CLOSE:
        RemoveWindowSubclass(
            GetDlgItem(hwndDlg, IDC_STATIC_ATTACH),
            SamplePreviewHandler::SubClassProc,
            1000);
        DestroyWindow(hwndDlg);
        break;

    case WM_COMMAND:
        OnCommandHandle(hwndDlg, wParam);
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

LRESULT CALLBACK SamplePreviewHandler::SubClassProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    UINT_PTR uIdSubclass,
    DWORD_PTR dwRefData )
{
    //SamplePreviewHandler *pThis = static_cast<SamplePreviewHandler*>((LPVOID)dwRefData);

    switch (uMsg)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);
            static HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));
            RECT rcClient = { 0 };
            GetClientRect(hWnd, &rcClient);
            static HPEN hPen = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
            SelectObject(hDC, hPen);
            Rectangle(hDC, 1, 1, rcClient.right, rcClient.bottom);
            InflateRect(&rcClient, -2, -2);
            FillRect(hDC, &rcClient, hbr);
            EndPaint(hWnd, &ps);
        }
        break;

    default:
        return DefSubclassProc(hWnd, uMsg, wParam, lParam);
    }

    return (LRESULT)FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam) 
{
    vector<HWND> *pChildren = (vector<HWND>*)(LPVOID)(lParam);
    if (NULL != pChildren)
    {
        pChildren->push_back(hwndChild);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SamplePreviewHandler::EnumChildren( vector<HWND> &hWndChildren, HWND hWndParent )
{
    EnumChildWindows(hWndParent, EnumChildProc, (LPARAM)(LPVOID)&hWndChildren);
}
