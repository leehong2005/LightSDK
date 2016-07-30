#pragma once

#include "resource.h"
#include "SdkCommonInclude.h"
#include "SampleFolderItem.h"

class SampleKnownFolderWin
{
public:

    SampleKnownFolderWin();
    ~SampleKnownFolderWin();

    BOOL CreateSampleKnownFolderWin(HINSTANCE hInst, HWND hWndParent);

private:

    void ClearFolerItemList();
    void SetScrollBarInfo(int nTotalSize);
    void SetScrollBarPos(int nPos);
    void OnPaint(HDC hDC, LPRECT lpRect);
    void OnApplyBtnClick(HWND hWndBtn);
    void OnClearBtnClick(HWND hWndBtn);
    void OnScroll(WPARAM wParam, LPARAM lParam);
    void OnSize(int nNewX, int nNewY);
    void OnCreate(HWND hwndDlg);
    INT_PTR OnDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

    int         m_nCurScrollPos;
    int         m_nCurOffset;
    int         m_nMaxScroll;
    int         m_nCurSel;
    HWND        m_hWnd;
    HINSTANCE   m_hInst;
    RECT        m_rcIcons;
    vector<SampleFolderItem*> m_vctFolderItems;
};
