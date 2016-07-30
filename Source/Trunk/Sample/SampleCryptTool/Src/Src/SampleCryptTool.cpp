// SampleCryptTool.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SampleCryptTool.h"
#include <Commdlg.h>

#pragma comment(lib, "comctl32.lib")


#define KEY_FILENAME        _T("crypt_tool_key")


SdkCryptToolWindow::SdkCryptToolWindow() : m_strPWD(_T(""))
{
    m_hInst         = NULL;
    m_hWnd          = NULL;
    m_pMsgDlg       = new SampleShowMessage();

    ZeroMemory(m_szKeyPath, sizeof(TCHAR) * MAX_PATH);
    SdkCommonHelper::GetKnownFolderPath(m_szKeyPath, CSIDL_APPDATA);
    ::PathCombine(m_szKeyPath, m_szKeyPath, _T("LightSDK\\Crypt"));

    if ( !::PathFileExists(m_szKeyPath) )
    {
        SdkCommonHelper::CreateFolder(m_szKeyPath);
    }
}

//////////////////////////////////////////////////////////////////////////

SdkCryptToolWindow::~SdkCryptToolWindow()
{
    SAFE_DELETE(m_pMsgDlg);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkCryptToolWindow::CreateToolWindow(HINSTANCE hInst, HWND hWndParent)
{
    UNREFERENCED_PARAMETER(hWndParent);

    m_hInst = hInst;

    m_hWnd = CreateDialogParam(
        hInst,
        MAKEINTRESOURCE(IDD_DIALOG_CRYPT),
        hWndParent,
        SdkCryptToolWindow::DialogProc,
        (WPARAM)this);

    if (NULL != m_hWnd)
    {
        m_pMsgDlg->ShowErroDialog(m_hInst, m_hWnd);
        ::ShowWindow(m_pMsgDlg->GetHWnd(), SW_HIDE);

        ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
        ::UpdateWindow(m_hWnd);
    }

    return (NULL != m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnCryptBegin()
{

}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnCryptFinish()
{

}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnCryptNext(IN DWORD dwCurIndex, IN LPCTSTR lpCurFilePath)
{

}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnCryptError(IN LPCTSTR lpErrorMsg)
{
    m_pMsgDlg->SetErrorMessage(lpErrorMsg);
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnProgressChanged(IN DWORD dwCompleted, IN DWORD dwTotal)
{
    SdkCommonHelper::PrintDebugString(L"Percent: %d / %d\n", dwCompleted, dwTotal);
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnReplaceFiles(OUT CRYPT_INQUIRE_RESULT *pResult, IN LPCTSTR lpFileName)
{
}

//////////////////////////////////////////////////////////////////////////

CryptString SdkCryptToolWindow::CryptKeyFileName(IN const CryptString& strKeyFileName)
{
    CryptString strKeyName = SdkCryptFile::EncryptFileName(strKeyFileName);
    return strKeyName;
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnDoCrypt(BOOL isEncrypt)
{
    BOOL isDelOriFiles = ::SendDlgItemMessage(
        m_hWnd,
        IDC_CHECK_DELORIFILES, BM_GETCHECK, 0, 0);

    BOOL isReplaceFile = ::SendDlgItemMessage(
        m_hWnd,
        IDC_CHECK_REPLAACEFILES, BM_GETCHECK, 0, 0);

    UNREFERENCED_PARAMETER(isReplaceFile);

    HWND hWndEdit = ::GetDlgItem(m_hWnd, IDC_EDIT_FILES);
    TCHAR szBuf[MAX_PATH] = { 0 };
    GetWindowText(hWndEdit, szBuf, MAX_PATH);
    vector<CryptString> vctFiles;
    vctFiles.push_back(szBuf);
    SdkCryptFile *pCryptFile    = new SdkCryptFile();

    TCHAR szKeyFullPath[MAX_PATH] = { 0 };
    CryptString strKeyFileName = CryptKeyFileName(KEY_FILENAME);
    ::PathCombine(szKeyFullPath, m_szKeyPath, strKeyFileName.c_str());

    pCryptFile->SetCryptKeyPath(szKeyFullPath);
    pCryptFile->SetCryptFileSink(this);
    pCryptFile->SetCryptPassword(m_strPWD.c_str());
    pCryptFile->SetCryptFiles(vctFiles, isDelOriFiles);

    ShowWindow(m_hWnd, SW_HIDE);

    pCryptFile->BeginCrypt(isEncrypt ? CRYPT_OP_ENCRYPT : CRYPT_OP_DECRYPT);
    ShowWindow(m_hWnd, SW_SHOW);

    SAFE_DELETE(pCryptFile);
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnBrowserClick(HWND hWndCtrl)
{
    OPENFILENAME ofn;

    TCHAR szBuf[MAX_PATH] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFile = szBuf;

    //
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    //
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    WCHAR szDesktopPath[MAX_PATH] = {0};
    SHGetSpecialFolderPath(NULL, szDesktopPath, CSIDL_DESKTOP, FALSE);
    ofn.lpstrInitialDir = szDesktopPath;

    if (GetOpenFileName(&ofn))
    {
        SetWindowText(::GetDlgItem(m_hWnd, IDC_EDIT_FILES), szBuf);
    }
}

//////////////////////////////////////////////////////////////////////////

void OnReplaceFiles(OUT CRYPT_INQUIRE_RESULT *pResult, IN LPCTSTR lpFileName)
{

}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnEncryptClick(HWND hWndCtrl)
{
    OnDoCrypt(TRUE);
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnDecryptClick(HWND hWndCtrl)
{
    OnDoCrypt(FALSE);
}

//////////////////////////////////////////////////////////////////////////

void SdkCryptToolWindow::OnSetPWD(HWND hWndCtrl)
{
    SamplePWDDialog pwdDialog;
    if ( pwdDialog.ShowPWDDialog(m_hInst, m_hWnd) )
    {
        m_strPWD = pwdDialog.GetPassword();
    }
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SdkCryptToolWindow::OnDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwndDlg);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_COMMAND:
        {
            int nCtrlId = (int)LOWORD(wParam);
            switch (nCtrlId)
            {
            case IDC_BTN_MAINDLG_CANCEL:
                SendMessage(hwndDlg, WM_CLOSE, 0, 0);
                break;

            case IDC_BTN_MAINDLG_DECRYPT:
                OnDecryptClick((HWND)lParam);
                break;

            case IDC_BTN_MAINDLG_ENCRYPT:
                OnEncryptClick((HWND)lParam);
                break;

            case IDC_BTN_MAINDLG_BROWSER:
                OnBrowserClick((HWND)lParam);
                break;

            case IDC_BTN_SETPWD:
                OnSetPWD((HWND)lParam);
                break;
            }
        }
        break;

    default:
        return FALSE;
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK SdkCryptToolWindow::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (WM_INITDIALOG == uMsg)
    {
        SdkCryptToolWindow *pThis = static_cast<SdkCryptToolWindow*>((LPVOID)(LONG)lParam);
        if (NULL != pThis)
        {
            SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG)pThis);
        }

        SdkCommonHelper::SetWindowCenter(hwndDlg);
        HICON hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_SAMPLECRYPTTOOL));
        ::SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
        ::SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
        ::DestroyIcon(hIcon);

        CheckDlgButton(hwndDlg, IDC_CHECK_DELORIFILES, BST_CHECKED);
        CheckDlgButton(hwndDlg, IDC_CHECK_REPLAACEFILES, BST_CHECKED);

        // The control to receive the default keyboard focus is always the first 
        // control in the dialog box that is visible, not disabled, and that has the 
        // WS_TABSTOP style. When the dialog box procedure returns TRUE, the system
        // checks the control to ensure that the procedure has not disabled it. If it
        // been disabled, the system sets the keyboard focus to the next control that is
        // visible, not disabled, and has the WS_TABSTOP style.
        // An application can return FALSE only if it has set the keyboard focus to 
        // the next control focus to one of the controls of the dialog box.

        // Return TRUE will make the first control get focus, and return FALSE to avoid.
        return FALSE;
    }

    SdkCryptToolWindow *pThis = static_cast<SdkCryptToolWindow*>((LPVOID)GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
    if (NULL != pThis)
    {
        return pThis->OnDialogProc(hwndDlg, uMsg, wParam, lParam);
    }

    return FALSE;
}

