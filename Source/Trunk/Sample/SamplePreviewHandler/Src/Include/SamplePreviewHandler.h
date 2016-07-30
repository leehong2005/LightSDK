#pragma once
#ifdef __cplusplus
#ifndef _SAMPLEPREVIEWHANDLER_H_
#define _SAMPLEPREVIEWHANDLER_H_

#include "resource.h"
#include "SdkCommonInclude.h"

class SamplePreviewHandler
{
public:

    SamplePreviewHandler();
    ~SamplePreviewHandler();
    HRESULT Initialize(HINSTANCE hInstance);
    void RunMessageLoop();

protected:

    void EnumChildren(vector<HWND> &hWndChildren, HWND hWndParent);
    LRESULT OnDialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
    void OnCommandHandle(HWND hwnd, WPARAM wParam);
    BOOL ShowOpenFileDialog(HWND hWnd, OUT LPWSTR pszselfile);
    static INT_PTR CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);
    static LRESULT CALLBACK SubClassProc(
        HWND hWnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam,
        UINT_PTR uIdSubclass,
        DWORD_PTR dwRefData);

private:

    HINSTANCE               m_hinstance;
    HWND                    m_hWnd;
    SdkPreviewHandler       *m_sdkPriveHandle;
};

#endif // _SAMPLEPREVIEWHANDLER_H_
#endif // __cplusplus

