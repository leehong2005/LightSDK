#pragma once

#include "Resource.h"
#include "SdkCommon.h"
#include "SampleDef.h"
#include "SampleListView.h"

class SampleWindows;

class SampleFunWindow
{
public:
    SampleFunWindow();
    ~SampleFunWindow();

    BOOL InitializeWindow(HINSTANCE hInst, HWND hWndParent);
    void SetWindowSize(int width, int height);
    void SetWindowPos(int left, int top);
    void SetCurCtrlName(LPCTSTR lpCtrlName);
    void SetTabPageWindow(SampleWindows *pSampleWin);

private:

    void EnableTagPageCtrl();
    void LoadFunsAndParams();

    void OnBtnApplyClick(HWND hWndCtl);
    void OnCreate(HWND hWndDlg);
    void OnComboBoxSelChanged(int nCurIndex);
    void OnCommand(HWND hWnd, UINT id, HWND hWndCtl, UINT codeNotify);

    virtual INT_PTR OnDialogProc(
        HWND hwndDlg,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam);


    static INT_PTR CALLBACK DialogProc(
        HWND hwndDlg,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam);

private:

    HWND            m_hWnd;
    HWND            m_hWndList;
    SampleListView *m_pListView;
    SampleWindows  *m_pSampleWin;
};
