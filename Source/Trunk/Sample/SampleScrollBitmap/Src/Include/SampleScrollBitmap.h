#pragma once

#include "stdafx.h"
#include "resource.h"
#include "SdkCommonInclude.h"

class SampleScrollBitmapWin
{
public:

    SampleScrollBitmapWin();
    ~SampleScrollBitmapWin();

    BOOL CreateMyWindow(HINSTANCE hInst, HWND hWndParent);

private:

    void OnVScroll(WPARAM wParam, LPARAM lParam);
    void OnHScroll(WPARAM wParam, LPARAM lParam);
    void OnKeyKown(WPARAM wParam, LPARAM lParam);
    void OnCreate(HWND hWnd);
    void OnSize(int nNewX, int nNewY);
    void OnPaint(HDC hDC, LPRECT lprcPaint);
    LRESULT OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

    int         m_nTopOffset;
    int         m_xCurScroll;
    int         m_yCurScroll;
    SIZE        m_bmpSize;
    HBITMAP     m_hBitmap;
    HINSTANCE   m_hInst;
    HWND        m_hWnd;
};