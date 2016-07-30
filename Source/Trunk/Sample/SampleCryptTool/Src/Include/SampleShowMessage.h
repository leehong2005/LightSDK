
#ifdef __cplusplus
#ifndef _SAMPLESHOWMESSAGE_H_
#define _SAMPLESHOWMESSAGE_H_


#include <windowsx.h>
#include <commctrl.h>
#include "Resource.h"
#include "SdkCommonInclude.h"


#define WM_USER_ERROR       WM_USER + 0x01

class SampleShowMessage
{
public:

    SampleShowMessage();
    ~SampleShowMessage();

    HWND ShowErroDialog(HINSTANCE hInst, HWND hWndParent);
    BOOL SetErrorMessage(LPCTSTR lpText);
    HWND GetHWnd();

private:

    CryptString LoadStr(UINT uID);

    void OnCommand(HWND hWnd, UINT id, HWND hWndCtl, UINT codeNotify);
    void AddItem(IN HWND hWnd, IN HWND hListHwnd, IN const CryptString& strErrorMsg);
    void InitialListCtrl(IN HWND hListHwnd);
    void UpdateErrorCount(IN HWND hWnd, IN HWND hStaticHwnd);
    void CopyErrorMessage(IN HWND hWnd, IN const CryptString& strCopy);
    void ClearErrorMessage();
    CryptString GetStringFromListCtrl(IN HWND hListHwnd);

    INT_PTR OnDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

    HWND        m_hWnd;
    HWND        m_hWndHost;
};

#endif // __cplusplus
#endif // _SAMPLESHOWMESSAGE_H_
