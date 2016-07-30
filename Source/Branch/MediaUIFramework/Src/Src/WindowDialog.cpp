/*!
* @file WindowDialog.cpp
* 
* @brief This file defines class to show modal dialog.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/28
*/

#include "WindowDialog.h"

WindowDialog::WindowDialog() : m_isEnableParent(FALSE)
{
}

//////////////////////////////////////////////////////////////////////////

WindowDialog::~WindowDialog()
{
}

//////////////////////////////////////////////////////////////////////////

INT_PTR WindowDialog::DoModal(HWND hWndParent)
{
    SaveRestoreParentState(hWndParent, TRUE);

    // About the implement of DoModal, please refer to MFC source dlgcore.cpp, line 523.
    BOOL isSucceed = CreateWindowForm(GetModuleHandle(NULL),  hWndParent, SW_HIDE);
    if (!isSucceed)
    {
        return 0;
    }

    CommonHelper::SetWindowCenter(m_hWnd);
    ShowWindow(m_hWnd, SW_SHOWNORMAL);

    BOOL isLayeredWindow = WindowForm::IsLayeredWindow();
    if (isLayeredWindow)
    {
        SetOpacity(0.01f);
        OnDrawUI();
    }
    else
    {
        UpdateWindow(m_hWnd);
    }

    // The message loop.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if ( !IsWindow(m_hWnd) || !IsDialogMessage(m_hWnd, &msg) )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    SaveRestoreParentState(hWndParent, FALSE);

    return (INT_PTR)msg.wParam;
}

//////////////////////////////////////////////////////////////////////////

void WindowDialog::EndDialog(INT32 nResult)
{
    m_nExitCode = (INT_PTR)nResult;
    OnDestroy();
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowDialog::CreateWindowForm(HINSTANCE hInstance, HWND hWndParent, INT32 nCmdShow)
{
    return WindowForm::CreateWindowForm(hInstance, hWndParent, nCmdShow);
}

//////////////////////////////////////////////////////////////////////////

void WindowDialog::OnPreCreate(LPCREATEWINDOWPARAMS lpCreateWinParams)
{
    lpCreateWinParams->cx        = 400;
    lpCreateWinParams->cy        = 300;
    lpCreateWinParams->dwStyle   = WS_DLGFRAME;
}

//////////////////////////////////////////////////////////////////////////

void WindowDialog::SaveRestoreParentState(HWND hWndParent, BOOL isSaved)
{
    HWND hWndAncestor = GetAncestor(hWndParent, GA_ROOTOWNER);
    if (isSaved)
    {
        if ( IsWindow(hWndParent) && (hWndParent != GetDesktopWindow()) && 
             IsWindowEnabled(hWndParent) )
        {
            EnableWindow(hWndParent, FALSE);
            EnableWindow(hWndAncestor, FALSE);
            m_isEnableParent = TRUE;
        }
    }
    else
    {
        // Hide the window before enabling the parent, etc.
        ::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
        if (m_isEnableParent)
        {
            EnableWindow(hWndParent, TRUE);
            EnableWindow(hWndAncestor, TRUE);
        }

        m_isEnableParent = FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowDialog::OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DISPLAYCHANGE:
        CommonHelper::SetWindowCenter(hWnd, TRUE);
        break;

    case WM_KEYDOWN:
        if(VK_ESCAPE == (INT32)wParam)
        {
            m_nExitCode = IDCANCEL;
            // Post a quit message to quit message loop, after the message loop, destructor
            // function will be call, in the destructor function, we destroy the window.
            PostQuitMessage(m_nExitCode);
        }
        break;

    case WM_POWERBROADCAST:
        {
            if (PBT_APMRESUMESUSPEND == wParam)
            {
                Invalidate(TRUE);
            }
        }
        break;

    default:
        return WindowForm::OnWndProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
