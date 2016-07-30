/*!
* @file SdkWindowDialog.cpp
* 
* @brief This file defines class to show modal dialog.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/28
*/

#include "stdafx.h"
#include "SdkWindowDialog.h"

USING_NAMESPACE_WINDOW

SdkWindowDialog::SdkWindowDialog() : m_isEnableParent(FALSE),
                                     m_isEndDialog(FALSE)
{
}

//////////////////////////////////////////////////////////////////////////

SdkWindowDialog::~SdkWindowDialog()
{
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SdkWindowDialog::DoModal(HWND hWndParent)
{
    SaveRestoreParentState(hWndParent, TRUE);

    // About the implement of DoModal, please refer to MFC source dlgcore.cpp, line 523.
    BOOL isSucceed = CreateWindowForm(GetModuleHandle(NULL),  hWndParent, SW_HIDE);

    if ( !isSucceed )
    {
        return 0;
    }

    // Set the window center to is parent window.
    CenterWindow(FALSE);
    ::ShowWindow(m_hWnd, SW_SHOWNORMAL);

    BOOL isLayeredWindow = SdkWindowForm::IsLayeredWindow();
    if ( isLayeredWindow )
    {
        SetOpacity(0.01f);
        OnDrawUI(FALSE, NULL);
    }
    else
    {
        ::UpdateWindow(m_hWnd);
    }

    // The message loop.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // If the flag is true, break the message loop.
        if (m_isEndDialog)
        {
            break;
        }

        if ( !IsWindow(m_hWnd) || !IsDialogMessage(m_hWnd, &msg) )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    SaveRestoreParentState(hWndParent, FALSE);

    // Destroy the modal window.
    ::DestroyWindow(m_hWnd);

    return (INT_PTR)m_nExitCode;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowDialog::EndDialog(INT32 nResult)
{
    m_nExitCode   = (INT_PTR)nResult;

    // Set the flag to TRUE so that window will exit message loop.
    m_isEndDialog = TRUE;

    // Call this callback when dialog is being destroy.
    OnDestroy();

    // Send the NULL message to the message loop.
    ::PostMessage(m_hWnd, WM_NULL, 0, 0);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindowDialog::CreateWindowForm(HINSTANCE hInstance, HWND hWndParent, INT32 nCmdShow)
{
    return SdkWindowForm::CreateWindowForm(hInstance, hWndParent, nCmdShow);
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowDialog::OnPreCreate(LPCREATEWINDOWPARAMS lpCreateWinParams)
{
    lpCreateWinParams->cx        = 400;         // Default width.
    lpCreateWinParams->cy        = 300;         // Default height.
    lpCreateWinParams->dwStyle   = WS_DLGFRAME; // The dialog style.
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowDialog::SaveRestoreParentState(HWND hWndParent, BOOL isSaved)
{
    HWND hWndAncestor = GetAncestor(hWndParent, GA_ROOTOWNER);
    if ( isSaved )
    {
        if ( IsWindow(hWndParent) &&
             (hWndParent != GetDesktopWindow()) && 
             IsWindowEnabled(hWndParent) )
        {
            // Disable direct parent window.
            EnableWindow(hWndParent, FALSE);
            // Disable the ancestor window.
            EnableWindow(hWndAncestor, FALSE);

            m_isEnableParent = TRUE;
        }
    }
    else
    {
        // Hide the window before enabling the parent, etc.
        ::SetWindowPos(
            m_hWnd,
            NULL,
            0,
            0,
            0,
            0,
            SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

        if ( m_isEnableParent )
        {
            EnableWindow(hWndParent, TRUE);
            EnableWindow(hWndAncestor, TRUE);
        }

        m_isEnableParent = FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindowDialog::OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DISPLAYCHANGE:
        {
            CenterWindow(TRUE);
        }
        break;

    case WM_KEYDOWN:
        {
            if ( VK_ESCAPE == wParam )
            {
                m_nExitCode = IDCANCEL;
                // Post a quit message to quit message loop, after the message loop, destructor
                // function will be call, in the destructor function, we destroy the window.
                PostQuitMessage(m_nExitCode);
            }
            else
            {
                return SdkWindowForm::OnWndProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    default:
        return SdkWindowForm::OnWndProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
