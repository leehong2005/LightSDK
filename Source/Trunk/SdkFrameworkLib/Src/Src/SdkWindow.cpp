/*!
* @file SdkWindow.cpp
* 
* @brief This file defines the class SdkWindow, represents a visual window.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#include "stdafx.h"
#include "SdkWindow.h"
#include "SdkViewElement.h"
#include "SdkViewLayout.h"
#include "D2DDevice.h"
#include "D3DDevice.h"
#include "SdkCommonInclude.h"

USING_NAMESPACE_D2D
USING_NAMESPACE_D3D
USING_NAMESPACE_WINDOW

vector<SdkWindow*> SdkWindow::s_vctWindows;

SdkWindow* SdkWindow::FromHwnd(HWND hWnd)
{
    SdkWindow *pWindow = NULL;

    int nSize = (int)s_vctWindows.size();
    for (int i = 0; i < nSize; ++i)
    {
        pWindow = s_vctWindows[i];
        if ( (NULL != pWindow) && (hWnd == pWindow->GetHwnd()) )
        {
            break;
        }
    }

    return pWindow;
}

//////////////////////////////////////////////////////////////////////////

SdkWindow::SdkWindow() : m_hWnd(NULL),
                         m_hWndParent(NULL),
                         m_pRootView(NULL),
                         m_pD2DDevice(NULL),
                         m_pD3DDevice(NULL),
                         m_fOpacity(1.0f),
                         m_dwBkColor(RGB(255, 255, 255)),
                         m_nWindowState(WINDOW_STATE_NONE)
{
    s_vctWindows.push_back(this);
    ZeroMemory(&WindowViews, sizeof(WINDOWVIEWS));

    m_pRootView = new SdkViewLayout();
    m_pRootView->SetParent(NULL);
    m_pRootView->SetWindow(this);
    m_pRootView->SetBkColor(D2D1::ColorF(m_dwBkColor));
}

//////////////////////////////////////////////////////////////////////////

SdkWindow::~SdkWindow()
{
    if ( NULL != m_pRootView )
    {
        m_pRootView->OnWindowDestroy(this);
    }

    SAFE_DELETE(m_pRootView);
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

    // Destroy window, this must be called after all views on this window are deleted.
    // Otherwise it may lead to crash sometime.
    ::DestroyWindow(m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

HWND SdkWindow::GetHwnd() const
{
    return m_hWnd;
}

//////////////////////////////////////////////////////////////////////////

SdkWindow::operator HWND() const
{
    return m_hWnd;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::SetBkColor(DWORD dwBkColor)
{
    m_dwBkColor = dwBkColor;

    if ( NULL != m_pRootView )
    {
        m_pRootView->SetBkColor(D2D1::ColorF(dwBkColor));
        // If the handle of window is NOT null or window is valid, means the 
        // window has been created, so we should redraw the window.
        if (NULL != m_hWnd)
        {
            Invalidate(TRUE, NULL);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::SetRoundCornerRadius(FLOAT fRadiusX, FLOAT fRadiusY)
{
    if ( NULL != m_pRootView )
    {
        m_pRootView->SetRoundCornerEnable(TRUE);
        m_pRootView->SetRoundCornerRadius(fRadiusX, fRadiusY);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::GetClassName(LPTSTR lpszClassName, UINT32 uSize)
{
    UNREFERENCED_PARAMETER(lpszClassName);
    UNREFERENCED_PARAMETER(uSize);

    // Should be implemented by derived class.
}

//////////////////////////////////////////////////////////////////////////

LONG SdkWindow::GetWidth() const
{
    WINDOWPLACEMENT wpl = { 0 };
    wpl.length = sizeof(WINDOWPLACEMENT);
    ::GetWindowPlacement(m_hWnd, &wpl);

    return (wpl.rcNormalPosition.right - wpl.rcNormalPosition.left);
}

//////////////////////////////////////////////////////////////////////////

LONG SdkWindow::GetHeight() const
{
    WINDOWPLACEMENT wpl = { 0 };
    wpl.length = sizeof(WINDOWPLACEMENT);
    ::GetWindowPlacement(m_hWnd, &wpl);

    return (wpl.rcNormalPosition.bottom - wpl.rcNormalPosition.top);
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::SetWindowPos(INT32 x, INT32 y)
{
    FLOAT dpiX = 1.0f;
    FLOAT dpiY = 1.0f;
    GetDesktopDpi(&dpiX, &dpiY);

    INT32 left = (INT32)((FLOAT)x * dpiX);
    INT32 top  = (INT32)((FLOAT)y * dpiY);

    ::SetWindowPos(m_hWnd, NULL, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::SetWindowSize(UINT32 w, UINT32 h)
{
    FLOAT dpiX = 1.0f;
    FLOAT dpiY = 1.0f;
    GetDesktopDpi(&dpiX, &dpiY);

    INT32 width  = (INT32)((FLOAT)w * dpiX);
    INT32 height = (INT32)((FLOAT)h * dpiY);

    ::SetWindowPos(m_hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindow::AddView(IN SdkViewElement *pView)
{
    if ( NULL != m_pRootView && NULL != pView )
    {
        SdkViewLayout *pRootView = dynamic_cast<SdkViewLayout*>(m_pRootView);
        if ( NULL != pRootView )
        {
            return pRootView->AddView(pView);
        }
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::GetDesktopDpi(FLOAT *pdpiX, FLOAT *pdpiY)
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

void SdkWindow::Invalidate(BOOL isErase, const LPRECT lprcPaint)
{
    if (isErase)
    {
        ::InvalidateRect(m_hWnd, NULL, TRUE);
        // Here can not call UpdateWindow function, it may lead some issue when play animation.
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

void SdkWindow::CenterWindow(BOOL fInScreen)
{
    if (::IsWindow(m_hWnd))
    {
        SdkCommonHelper::SetWindowCenter(m_hWnd, fInScreen);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::CloseWindow()
{
    if (::IsWindow(m_hWnd))
    {
        ::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::SetEnabled(BOOL isEnable)
{
    ::EnableWindow(m_hWnd, isEnable);
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::SetVisible(BOOL isShow)
{
    ::ShowWindow(m_hWnd, isShow ? SW_SHOW : SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindow::IsEnabled() const
{
    return ::IsWindowEnabled(m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindow::IsVisible() const
{
    return ::IsWindowVisible(m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindow::IsLayeredWindow() const
{
    if (::IsWindow(m_hWnd))
    {
        LONG lExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
        return (WS_EX_LAYERED == (lExStyle & WS_EX_LAYERED));
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

D2DDevice* SdkWindow::GetD2DDevices() const
{
    return m_pD2DDevice;
}

//////////////////////////////////////////////////////////////////////////

D3DDevice* SdkWindow::GetD3DDevice() const
{
    return m_pD3DDevice;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::PerformViewOnClick(SdkViewElement *pView)
{
    if ( NULL != pView )
    {
        pView->PerformClick();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::SetDeviceTargetType(DEVICE_TARGET_TYPE paintMode)
{
    if ( NULL != m_pD2DDevice )
    {
        m_pD2DDevice->SetPaintTargetType(paintMode);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::SetOpacity(FLOAT fOpacity)
{
    m_fOpacity = fOpacity;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::AddWindowState(WINDOW_STATE state)
{
    m_nWindowState = m_nWindowState | state;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::RemoveWindowState(WINDOW_STATE state)
{
    m_nWindowState = m_nWindowState & ~state;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindow::AddEventViews(SdkViewElement *pView)
{
    UNREFERENCED_PARAMETER(pView);

    // Not implements
}
