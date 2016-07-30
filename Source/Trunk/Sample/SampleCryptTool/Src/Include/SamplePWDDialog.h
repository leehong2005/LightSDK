#ifdef __cplusplus
#ifndef _SAMPLEPWDDIALOG_H_
#define _SAMPLEPWDDIALOG_H_

#include "stdafx.h"
#include "SdkCommonInclude.h"

class SamplePWDDialog
{
public:

    SamplePWDDialog();
    ~SamplePWDDialog();

    CryptString GetPassword() const;
    BOOL ShowPWDDialog(HINSTANCE hInst, HWND hWndParent);

private:

    BOOL OnCheckPWN(HWND hwndDlg);
    INT_PTR OnDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

    CryptString m_strPWD;
};

#endif // _SAMPLEPWDDIALOG_H_
#endif // __cplusplus

