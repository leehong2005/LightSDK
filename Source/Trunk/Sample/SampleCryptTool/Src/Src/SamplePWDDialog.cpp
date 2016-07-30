
#include "stdafx.h"
#include "SamplePWDDialog.h"
#include "Resource.h"

SamplePWDDialog::SamplePWDDialog() : m_strPWD(_T(""))
{
}

//////////////////////////////////////////////////////////////////////////

SamplePWDDialog::~SamplePWDDialog()
{
}

//////////////////////////////////////////////////////////////////////////

CryptString SamplePWDDialog::GetPassword() const
{
    return m_strPWD;
}

//////////////////////////////////////////////////////////////////////////

BOOL SamplePWDDialog::ShowPWDDialog(HINSTANCE hInst, HWND hWndParent)
{
    INT_PTR nRetVal = ::DialogBoxParam(
        hInst,
        MAKEINTRESOURCE(IDD_DIALOG_PASSWORD),
        hWndParent,
        SamplePWDDialog::DialogProc,
        (LPARAM)this);

    return (IDOK == nRetVal) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SamplePWDDialog::OnCheckPWN(HWND hwndDlg)
{
    TCHAR szPWD1[MAX_PATH] = { 0 };
    TCHAR szPWD2[MAX_PATH] = { 0 };
    ::GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_PWD1), szPWD1, MAX_PATH);
    ::GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_PWD2), szPWD2, MAX_PATH);

    BOOL isValid = (_tcslen(szPWD1) > 0) && (_tcslen(szPWD2) > 0);
    BOOL isEqual = (0 == _tcscmp(szPWD1, szPWD2));
    if ( !isValid )
    {
        ::SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_ERRORPWD), _T("Invalid password"));
        return FALSE;
    }

    if ( !isEqual )
    {
        ::SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_ERRORPWD), _T("Passwords do not match"));
        return FALSE;
    }

    m_strPWD = szPWD1;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SamplePWDDialog::OnDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case IDOK:
                {
                    if (OnCheckPWN(hwndDlg))
                    {
                        EndDialog(hwndDlg, IDOK);
                    }
                }
                break;

            case IDCANCEL:
                EndDialog(hwndDlg, IDCANCEL);
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

INT_PTR CALLBACK SamplePWDDialog::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_INITDIALOG)
    {
        SamplePWDDialog *pThis = static_cast<SamplePWDDialog*>((LPVOID)lParam);
        if (NULL != pThis)
        {
            SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG_PTR)pThis);
        }

        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_SAMPLECRYPTTOOL));
        ::SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
        ::DestroyIcon(hIcon);

        SdkCommonHelper::SetWindowCenter(hwndDlg);

        return TRUE;
    }

    SamplePWDDialog *pThis = static_cast<SamplePWDDialog*>((LPVOID)GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
    if (NULL != pThis)
    {
        return pThis->OnDialogProc(hwndDlg, uMsg, wParam, lParam);
    }

    return FALSE;
}
