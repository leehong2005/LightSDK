#pragma once

#include "SdkCommonInclude.h"
#include "SampleWindows.h"
#include "SampleFunWindow.h"

class SampleMainWindow
{
public:

    SampleMainWindow();
    ~SampleMainWindow();

    BOOL InitializeWindow(HINSTANCE hInst, HWND hWndParent);

protected:

    virtual BOOL OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

    SampleWindows       *m_pSampleWindows;
    SampleFunWindow     *m_pSampelFunWin;
};
