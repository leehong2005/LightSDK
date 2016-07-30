/*!
* @file Window.cpp
* 
* @brief This file defines the class Window, represents a visual window.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#include "Window.h"
#include "ViewElement.h"
#include "PerformanceCounter.h"

vector<Window*> Window::s_vctWindows;

Window* Window::FromHwnd(HWND hWnd)
{
    Window *pWindow = NULL;

    int nSize = (int)s_vctWindows.size();
    for (int i = 0; i < nSize; ++i)
    {
        pWindow = s_vctWindows[i];
        if ( (NULL != pWindow) && (hWnd == pWindow->GetHWND()) )
        {
            return pWindow;
        }
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////

Window::Window() : m_hWnd(NULL),
                   m_hWndParent(NULL),
                   m_pContentView(NULL),
                   m_pD2DDevice(NULL),
                   m_pD3DDevice(NULL),
                   m_fOpacity(1.0f),
                   m_nWindowState(WINDOW_STATE_NONE)
{
    s_vctWindows.push_back(this);

    ZeroMemory(&WindowViews, sizeof(WINDOWVIEWS));
}

//////////////////////////////////////////////////////////////////////////

Window::~Window()
{
    if (NULL != m_pContentView)
    {
        m_pContentView->OnWindowDestroy(this);
    }

    SAFE_DELETE(m_pContentView);
    SAFE_DELETE(m_pD2DDevice);
    SAFE_DELETE(m_pD3DDevice);

    int nSize = (int)s_vctWindows.size();
    for (int i = 0; i < nSize; ++i)
    {
        if (s_vctWindows[i] == this)
        {
            s_vctWindows[i] = NULL;
            break;
        }
    }

    // Destroy window, this must be called after all views on this window are deleted. Otherwise 
    // it may lead to crash sometime.
    DestroyWindow(m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

HWND Window::GetHWND() const
{
    return m_hWnd;
}

//////////////////////////////////////////////////////////////////////////

Window::operator HWND() const
{
    return m_hWnd;
}

//////////////////////////////////////////////////////////////////////////

void Window::GetClassName(LPTSTR lpszClassName, UINT32 uSize)
{
    UNREFERENCED_PARAMETER(lpszClassName);
    UNREFERENCED_PARAMETER(uSize);

    // Should be implemented by derived class.
}

//////////////////////////////////////////////////////////////////////////

LONG Window::GetWidth() const
{
    WINDOWPLACEMENT wpl = { 0 };
    wpl.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(m_hWnd, &wpl);

    return (wpl.rcNormalPosition.right - wpl.rcNormalPosition.left);
}

//////////////////////////////////////////////////////////////////////////

LONG Window::GetHeight() const
{
    WINDOWPLACEMENT wpl = { 0 };
    wpl.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(m_hWnd, &wpl);

    return (wpl.rcNormalPosition.bottom - wpl.rcNormalPosition.top);
}

//////////////////////////////////////////////////////////////////////////

void Window::SetWindowPos(INT32 x, INT32 y)
{
    FLOAT dpiX = 1.0f;
    FLOAT dpiY = 1.0f;
    GetDesktopDpiRatio(&dpiX, &dpiY);

    INT32 left = (INT32)((FLOAT)x * dpiX);
    INT32 top  = (INT32)((FLOAT)y * dpiY);

    ::SetWindowPos(m_hWnd, NULL, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////

void Window::SetWindowSize(UINT32 w, UINT32 h)
{
    FLOAT dpiX = 1.0f;
    FLOAT dpiY = 1.0f;
    GetDesktopDpiRatio(&dpiX, &dpiY);

    INT32 width  = (INT32)((FLOAT)w * dpiX);
    INT32 height = (INT32)((FLOAT)h * dpiY);

    ::SetWindowPos(m_hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////

void Window::GetDesktopDpiRatio(FLOAT *pdpiX, FLOAT *pdpiY)
{
    if (NULL != m_pD2DDevice)
    {
        FLOAT dpiX = 1.0f;
        FLOAT dpiY = 1.0f;

        m_pD2DDevice->GetDesktopDpi(&dpiX, &dpiY);

        // The DPI on X and Y always return 1.0, because D3D interfaces don't support DPI change.
        // and D2D interfaces has considered the DPI changed.
        if (NULL != pdpiX)
        {
            *pdpiX = 1.0f; //dpiX / 96.0f;
        }

        if (NULL != pdpiY)
        {
            *pdpiY = 1.0f; //dpiY / 96.0f;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void Window::Invalidate(BOOL isErase, const LPRECT lprcPaint)
{
    if (isErase)
    {
        ::InvalidateRect(m_hWnd, NULL, TRUE);
        // Here can not call UpdateWindow function, it may lead some issue when paly animation.
    }
    else
    {
        if (m_nWindowState & WINDOW_STATE_INVALIDATE)
        {
            ::InvalidateRect(m_hWnd, NULL, TRUE);
        }
    }

    RemoveWindowState(WINDOW_STATE_INVALIDATE);
}

//////////////////////////////////////////////////////////////////////////

void Window::SetContentView(IN ViewElement *pViewElement)
{
    if (NULL != pViewElement)
    {
        SAFE_DELETE(m_pContentView);

        FLOAT dpiX = 1.0f;
        FLOAT dpiY = 1.0f;
        GetDesktopDpiRatio(&dpiX, &dpiY);

        m_pContentView = pViewElement;
        m_pContentView->SetWindow(this);
        m_pContentView->SetLayoutInfo(0.0, 0.0, (FLOAT)GetWidth() * dpiX, (FLOAT)GetHeight() * dpiY);
        m_pContentView->SetParent(NULL);
    }
}

//////////////////////////////////////////////////////////////////////////

void Window::CenterWindow(BOOL fInScreen)
{
    if (::IsWindow(m_hWnd))
    {
        CommonHelper::SetWindowCenter(m_hWnd, fInScreen);
    }
}

//////////////////////////////////////////////////////////////////////////

void Window::CloseWindow()
{
    if (::IsWindow(m_hWnd))
    {
        ::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////

void Window::SetEnabled(BOOL isEnable)
{
    ::EnableWindow(m_hWnd, isEnable);
}

//////////////////////////////////////////////////////////////////////////

void Window::SetVisible(BOOL isShow)
{
    ::ShowWindow(m_hWnd, isShow ? SW_SHOW : SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////

BOOL Window::IsEnabled() const
{
    return ::IsWindowEnabled(m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

BOOL Window::IsVisible() const
{
    return ::IsWindowVisible(m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

BOOL Window::IsLayeredWindow() const
{
    if (::IsWindow(m_hWnd))
    {
        LONG lExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
        return (WS_EX_LAYERED == (lExStyle & WS_EX_LAYERED));
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

DOUBLE Window::GetSpeedX()
{
    // Not implement.
    return 0.0f;
}

//////////////////////////////////////////////////////////////////////////

DOUBLE Window::GetSpeedY()
{
    // Not implement.
    return 0.0f;
}

//////////////////////////////////////////////////////////////////////////

DWORD Window::GetSpeedSampleTime()
{
    // Not implement.
    return 0;
}

//////////////////////////////////////////////////////////////////////////

D2DDevice* Window::GetD2DDevices() const
{
    return m_pD2DDevice;
}

//////////////////////////////////////////////////////////////////////////

D3DDevice* Window::GetD3DDevice() const
{
    return m_pD3DDevice;
}

//////////////////////////////////////////////////////////////////////////

void Window::SetDeviceTargetType(DEVICE_TARGET_TYPE paintMode)
{
    if (NULL != m_pD2DDevice)
    {
        m_pD2DDevice->SetPaintTargetType(paintMode);
    }
}

//////////////////////////////////////////////////////////////////////////

void Window::SetOpacity(FLOAT fOpacity)
{
    m_fOpacity = fOpacity;
}

//////////////////////////////////////////////////////////////////////////

FLOAT Window::GetOpacity() const
{
    return m_fOpacity;
}

//////////////////////////////////////////////////////////////////////////

void Window::AddWindowState(WINDOW_STATE state)
{
    m_nWindowState = m_nWindowState | state;
}

//////////////////////////////////////////////////////////////////////////

void Window::RemoveWindowState(WINDOW_STATE state)
{
    m_nWindowState = m_nWindowState & ~state;
}

//////////////////////////////////////////////////////////////////////////

void Window::PushEventView(ViewElement *pView)
{
    UNREFERENCED_PARAMETER(pView);
    // Not implements
}
