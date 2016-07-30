/*!
* @file MainWindow.cpp
* 
* @brief The implementation of functions in MainWindow.h file.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/28
*/

#include "stdafx.h"
#include "SdkWindowForm.h"
#include "D2DDevice.h"
#include "SdkViewElement.h"

USING_NAMESPACE_WINDOW

#define SDKWINDOWFORM_PROP_NAME         _T("SdkWindowFormProp")

BOOL SdkWindowForm::s_hasRegClass = FALSE;

SdkWindowForm::SdkWindowForm() : m_hInst(NULL),
                                 m_hMemBitmap(NULL),
                                 m_isLBDown(FALSE),
                                 m_isTracked(FALSE),
                                 m_isSyncPerformEvents(FALSE),
                                 m_nExitCode(0)
{
}

//////////////////////////////////////////////////////////////////////////

SdkWindowForm::~SdkWindowForm()
{
    if ( NULL != m_hWnd )
    {
        SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)0);
    }

    SAFE_DELETE(m_pD2DDevice);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindowForm::CreateWindowForm(HINSTANCE hInst, HWND hWndParent, INT32 nCmdShow)
{
    if ( !s_hasRegClass )
    {
        WNDCLASSEX wcex;
        wcex.cbSize         = sizeof(WNDCLASSEX);
        wcex.style          = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc    = SdkWindowForm::WndProc;
        wcex.cbClsExtra     = 0;
        wcex.cbWndExtra     = 0;
        wcex.hInstance      = hInst;
        wcex.hIcon          = NULL;
        wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszMenuName   = NULL;
        wcex.lpszClassName  = WINDOWFORMCLASSNAME;
        wcex.hIconSm        = NULL;

        if ( !RegisterClassEx(&wcex) )
        {
            return FALSE;
        }

        s_hasRegClass = TRUE;
    }

    CREATEWINDOWPARAMS createParams = { 0 };
    createParams.cx             = 800;
    createParams.cy             = 600;
    createParams.hInstance      = hInst;
    createParams.hwndParent     = hWndParent;
    createParams.hMenu          = NULL;
    createParams.lpCreateParams = this;
    createParams.lpszClass      = WINDOWFORMCLASSNAME;
    createParams.lpszName       = L"SdkWindowForm";
    createParams.dwStyle        = WS_OVERLAPPEDWINDOW;

    // Call this method so that derived can customize window's style.
    OnPreCreate(&createParams);

    // If caller wants to create layered window, set window style WS_POPUP.
    if ((WS_EX_LAYERED ==  (createParams.dwExStyle & WS_EX_LAYERED)))
    {
        createParams.dwStyle = WS_POPUP;
    }

    HWND hWnd = CreateWindowEx(createParams.dwExStyle,
                               createParams.lpszClass,
                               createParams.lpszName,
                               createParams.dwStyle,
                               createParams.x,
                               createParams.y,
                               createParams.cx,
                               createParams.cy,
                               createParams.hwndParent,
                               createParams.hMenu,
                               createParams.hInstance,
                               (LPVOID)this);

    if ( NULL == hWnd )
    {
        return FALSE;
    }

    m_hWnd = hWnd;
    m_hInst = hInst;
    m_hWndParent = hWndParent;

    // Call the method before the window show.
    OnCreated();

    ::ShowWindow(hWnd, nCmdShow);

    BOOL isLayeredWindow = IsLayeredWindow();
    if ( isLayeredWindow )
    {
        m_fOpacity = 0.01f;
        OnDrawUI(FALSE, NULL);
    }
    else
    {
        ::UpdateWindow(hWnd);
    }

    return (NULL != hWnd) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::GetClassName(LPTSTR lpszClassName, UINT32 uSize)
{
    if ( NULL != lpszClassName )
    {
        _tcscpy_s(lpszClassName, uSize, WINDOWFORMCLASSNAME);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::Invalidate(BOOL isErase, const LPRECT lprcPaint)
{
    BOOL isLayeredWindow = IsLayeredWindow();

    if (isLayeredWindow)
    {
        if (isErase)
        {
            OnDrawUI(isErase, lprcPaint);
        }
        else
        {
            if (m_nWindowState & WINDOW_STATE_INVALIDATE)
            {
                OnDrawUI(FALSE, lprcPaint);
            }
        }
    }
    else
    {
        SdkWindow::Invalidate(isErase, lprcPaint);
    }

    RemoveWindowState(WINDOW_STATE_INVALIDATE);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindowForm::InitializeDevice()
{
    if (NULL == m_pD2DDevice)
    {
        m_pD2DDevice = new D2DDevice();
    }

    HRESULT hr1 = m_pD2DDevice->InitHwndDevice(m_hWnd);
    HRESULT hr2 = m_pD2DDevice->InitDCDevice();

    BOOL isLayeredWindow = IsLayeredWindow();
    if (isLayeredWindow)
    {
        m_pD2DDevice->SetPaintTargetType(DEVICE_TARGET_TYPE_DC);
    }
    else
    {
        m_pD2DDevice->SetPaintTargetType(DEVICE_TARGET_TYPE_HWND);
    }

    return (SUCCEEDED(hr1) && SUCCEEDED(hr2)) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindowForm::DispatchMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( (NULL == m_pRootView) || !m_pRootView->IsEnable() )
    {
        return FALSE;
    }

    BOOL handled = FALSE;
    MSG msg = { 0 };
    msg.message = message;
    msg.lParam  = lParam;
    msg.wParam  = wParam;

    SdkViewElement *pSource = m_pRootView->FindEventSource(message, wParam, lParam);

    // If the old event source is different from the new event source, we should also
    // dispatch messages to old event source, because the old source may want to receive
    // message which the mouse has been off its client rectangle.
    if ( pSource != WindowViews.EventSourceView )
    {
        // If the event target is not NULL, the all messages should be dispatched to the
        // event target view, otherwise the message should be dispatched to the current event 
        // source view.
        if ( (NULL != WindowViews.EventSourceView) && (NULL == WindowViews.EventTargetView) )
        {
            // If the current message is WM_MOUSEMOVE, we dispatch WM_MOUSELEAVE message
            // to the old event source, and treat the WM_MOUSEMOVE message as WM_MOUSELEAVE.
            if ( WM_MOUSEMOVE == message )
            {
                msg.message = WM_MOUSELEAVE;
                WindowViews.EventSourceView->DispatchMouseEvent(&msg);
                msg.message = message;
            }
        }

        // Save the new event source.
        WindowViews.EventSourceView = pSource;

        if ( (NULL != WindowViews.EventSourceView) && (NULL == WindowViews.EventTargetView) )
        {
            // If the message is WM_MOUSEMOVE, we dispatch WM_MOUSEHOVER to the new event source.
            // Treat the WM_MOUSEMOVE message as the WM_MOUSEHOVER message.
            if ( WM_MOUSEMOVE == message )
            {
                msg.message = WM_MOUSEHOVER;
                WindowViews.EventSourceView->DispatchMouseEvent(&msg);
                msg.message = message;
            }
        }
    }

    // If found a source view, dispatch messages to this source view.
    if ( NULL != pSource )
    {
        if ( (WM_LBUTTONDOWN == message) || (WM_RBUTTONDOWN == message) )
        {
            // Hide current visible pop-up view.
            handled = HideCurPopupView(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

            if ( !handled )
            {
                // Set current pressed view focus.
                pSource->SetFocus();
            }
        }

        // If not succeed to hide pop-up view.
        if ( !handled )
        {
            // Get the event target and dispatch mouse event from this target to root view.
            SdkViewElement *pTarget = NULL;
            if ( (WM_LBUTTONUP == message) || (WM_MOUSEMOVE == message) )
            {
                pTarget = SdkWindow::WindowViews.EventTargetView;
            }

            if ( NULL != pTarget )
            {
                handled = pTarget->DispatchMouseEvent(&msg);
            }
            else
            {
                handled = pSource->DispatchMouseEvent(&msg);
            }

            PerformViewEvent(message);
        }
    }
    else
    {
        // If the event source is NULL, in this situation, the mouse may be out of the window.
        if ( (WM_MOUSEMOVE == message) || (WM_MOUSELEAVE == message) ||
             (WM_LBUTTONUP == message) )
        {
            SdkViewElement *pTarget = SdkWindow::WindowViews.EventTargetView;

            if ( NULL != pTarget )
            {
                handled = pTarget->DispatchMouseEvent(&msg);
            }
            else
            {
                handled = m_pRootView->PreDispatchMouseEvent(&msg, m_pRootView);
            }
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindowForm::DispatchKeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    SdkViewElement *pCurrentFocusView = WindowViews.CurrentFocusView;

    BOOL handled = FALSE;

    if ( NULL != pCurrentFocusView &&
         pCurrentFocusView->IsEnable() &&
         pCurrentFocusView->IsVisible() )
    {
        MSG msg = { 0 };
        msg.message = message;
        msg.lParam  = lParam;
        msg.wParam  = wParam;

        // Dispatch the keyboard message to the current focus view.
        handled = pCurrentFocusView->DispatchKeyboardEvent(&msg);
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::AddEventViews(SdkViewElement *pView)
{
    // If the m_isSyncPerformEvents member is TRUE, the vector is being visited,
    // so at this time, should not push back item to the vector, otherwise leads
    // crash about the vector.

    if ( NULL != pView && !m_isSyncPerformEvents )
    {
        m_vctEventViews.push_back(pView);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::PerformViewEvent(UINT message)
{
    // If the message is left button up.
    if ( WM_LBUTTONUP == message )
    {
        if ( !m_isSyncPerformEvents )
        {
            for each (SdkViewElement *pView in m_vctEventViews)
            {
                m_isSyncPerformEvents = TRUE;
                PerformViewOnClick(pView);
            }

            m_vctEventViews.clear();

            m_isSyncPerformEvents = FALSE;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindowForm::HideCurPopupView(LONG x, LONG y)
{
    SdkViewElement *pCurPopupView = SdkWindow::WindowViews.CurrentPopupView;

    if ( NULL != pCurPopupView )
    {
        BOOL isPopup   = (VIEW_STATE_POPUP == (pCurPopupView->GetState() & VIEW_STATE_POPUP));
        BOOL isTouchOn = pCurPopupView->IsPtInRect((FLOAT)x, (FLOAT)y);
        // If the view is pop-up view and the mouse point is not in its rectangle, hide it.
        if ( isPopup && !isTouchOn )
        {
            pCurPopupView->ShowView(FALSE);
            // Set the current pop-up view to NULL.
            SdkWindow::WindowViews.CurrentPopupView = NULL;
        }

        return (isPopup && !isTouchOn);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnPaint(HDC hdc, const LPRECT rcPaint)
{
    if ( NULL != m_pD2DDevice )
    {
        switch (m_pD2DDevice->GetPaintTargetType())
        {
        case DEVICE_TARGET_TYPE_HWND:
            OnHwndPaint(hdc, rcPaint);
            break;

        case DEVICE_TARGET_TYPE_DC:
            OnDCPaint(hdc, rcPaint);
            break;

        case DEVICE_TARGET_TYPE_MEMORY:
            OnMemPaint(hdc, rcPaint);
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnHwndPaint(HDC hdc, const LPRECT rcPaint)
{
    if ( NULL != m_pD2DDevice && !m_pD2DDevice->IsWindowOccluded() )
    {
        m_pD2DDevice->SetPaintTargetType(DEVICE_TARGET_TYPE_HWND);
        m_pD2DDevice->SetOpacity(m_fOpacity);
        m_pD2DDevice->BeginDraw(hdc, NULL);

        if ( NULL != m_pRootView )
        {
            m_pRootView->OnPaint();
        }

        m_pD2DDevice->EndDraw();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnDCPaint(HDC hdc, const LPRECT rcPaint)
{
    if ( NULL != m_pD2DDevice )
    {
        m_pD2DDevice->SetPaintTargetType(DEVICE_TARGET_TYPE_DC);
        m_pD2DDevice->SetOpacity(m_fOpacity);
        m_pD2DDevice->BeginDraw(hdc, NULL);

        if ( NULL != m_pRootView )
        {
            m_pRootView->OnPaint();
        }

        m_pD2DDevice->EndDraw();
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnMemPaint(HDC hdc, const LPRECT rcPaint)
{
    if (NULL != m_pD2DDevice)
    {
        m_pD2DDevice->SetPaintTargetType(DEVICE_TARGET_TYPE_MEMORY);
        m_pD2DDevice->SetOpacity(m_fOpacity);
        m_pD2DDevice->BeginDraw(hdc, NULL);

        if ( NULL != m_pRootView )
        {
            m_pRootView->OnPaint();
        }

        m_pD2DDevice->EndDraw();

        // Get HBITMAP and draw to DC using GDI.
        HBITMAP hBitmap = m_pD2DDevice->GetMemHBITMAP();
        if (NULL != hBitmap)
        {
            RECT rc = { 0 };
            BITMAP bmp = { 0 };
            if ( 0 == GetObject(hBitmap, sizeof(BITMAP), &bmp) )
            {
                return;
            }

            rc.right  = bmp.bmWidth;
            rc.bottom = bmp.bmHeight;

            HDC hMemDC = CreateCompatibleDC(hdc);
            HGDIOBJ hOldObj = SelectObject(hMemDC, hBitmap);

            BitBlt(
                hdc,
                rc.left,
                rc.top,
                (rc.right - rc.left),
                (rc.bottom - rc.top),
                hMemDC,
                0,
                0,
                SRCCOPY);

            SelectObject(hMemDC, hOldObj);
            SAFE_DELETE_DC(hMemDC);
            SAFE_DELETE_OBJECT(hBitmap);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnDrawUI(BOOL fAlwaysDraw, const LPRECT lprcPaint)
{
    if ( !fAlwaysDraw )
    {
        BOOL isLayeredWindow = IsLayeredWindow();
        BOOL isWindowVisible = IsWindowVisible(m_hWnd);
        if (!isLayeredWindow || !isWindowVisible)
        {
            return;
        }
    }

    HDC hdc = GetWindowDC(m_hWnd);
    RECT clientRect = { 0 };
    GetClientRect(m_hWnd, &clientRect);

    LONG nWidth = RECT_WIDTH(clientRect);
    LONG nHeight = RECT_HEIGHT(clientRect);

    if ( NULL == m_hMemBitmap )
    {
        m_hMemBitmap = CreateCompatibleBitmap(hdc, nWidth, nHeight);
    }

    HDC hMemDC = CreateCompatibleDC(hdc);
    HGDIOBJ hOldObj = SelectObject(hMemDC, m_hMemBitmap);

    OnDCPaint(hMemDC, &clientRect);

    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    SIZE size = { nWidth, nHeight };
    POINT pointSrc  = { 0, 0 };
    UpdateLayeredWindow(m_hWnd, hdc, NULL, &size, hMemDC, &pointSrc, 0, &blend, ULW_ALPHA);

    SelectObject(hMemDC, hOldObj);
    DeleteObject(hMemDC);
    ReleaseDC(m_hWnd, hdc);
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnCreated()
{
    InitializeDevice();
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnCreating(LPCREATESTRUCT lpCreateStruct)
{
    // In this method, derived class can set window's icon, title, etc.
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnPreCreate(LPCREATEWINDOWPARAMS lpCreateWinParams)
{
    UNREFERENCED_PARAMETER(lpCreateWinParams);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindowForm::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL handled = FALSE;

    switch (message)
    {
    case WM_MOUSEMOVE:
        {
            if ( !m_isTracked )
            {
                TRACKMOUSEEVENT eventTrack;
                eventTrack.cbSize  = sizeof(TRACKMOUSEEVENT);
                eventTrack.dwFlags = TME_HOVER | TME_LEAVE;
                eventTrack.hwndTrack = m_hWnd;
                eventTrack.dwHoverTime = 1;
                m_isTracked = TrackMouseEvent(&eventTrack);
            }

            // We should also dispatch mouse move message to views.
            handled = DispatchMouseEvent(message, wParam, lParam);
        }
        break;

    case WM_MOUSELEAVE:
        {
            m_isTracked = FALSE;
            m_isLBDown  = FALSE;
            handled = DispatchMouseEvent(message, wParam, lParam);
            SdkWindow::WindowViews.EventTargetView = NULL;
        }
        break;

    case WM_LBUTTONUP:
        {
            m_isLBDown  = FALSE;
            handled = DispatchMouseEvent(message, wParam, lParam);
            SdkWindow::WindowViews.EventTargetView = NULL;

            // If the right button is NOT hold down.
            if ( MK_RBUTTON != (MK_RBUTTON & (LONG)wParam) )
            {
                ReleaseCapture();
            }
        }
        break;

    case WM_LBUTTONDOWN:
        {
            m_isLBDown  = TRUE;
            SetCapture(m_hWnd);
            handled = DispatchMouseEvent(message, wParam, lParam);
        }
        break;

    case WM_RBUTTONUP:
        {
            m_isLBDown  = FALSE;
            handled = DispatchMouseEvent(message, wParam, lParam);

            // If the left button is NOT hold down.
            if ( MK_LBUTTON != (MK_LBUTTON & (LONG)wParam) )
            {
                ReleaseCapture();
            }
        }
        break;

    case WM_RBUTTONDOWN:
        {
            m_isLBDown  = TRUE;
            SetCapture(m_hWnd);
            handled = DispatchMouseEvent(message, wParam, lParam);
        }
        break;

    case WM_MOUSEWHEEL:
        {
            handled = DispatchMouseEvent(message, wParam, lParam);
        }
        break;
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindowForm::OnKeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL handled = FALSE;

    handled = DispatchKeyboardEvent(message, wParam, lParam);

    if ( !handled )
    {
        // If the focus view do not process th key event and the key is TAB, make
        // next view gain keyboard focus.
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnDestroy()
{
    if ( NULL == m_hWndParent )
    {
        ::PostQuitMessage(m_nExitCode);
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnClose()
{
    ::DestroyWindow(m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

void SdkWindowForm::OnSize(int w, int h)
{
    SAFE_DELETE_OBJECT(m_hMemBitmap);

    if (NULL != m_pD2DDevice)
    {
        FLOAT fScale = SdkCommonHelper::GetScale();
        m_pD2DDevice->Resize((UINT32)(w * fScale), (UINT32)(h * fScale));
    }

    // When application is iconic, system will send WM_SIZE message, and got width and
    // height is zero, so in this case, we should not reset content view's size, because
    // if do so, it will lead unnecessary layout.
    if ( NULL != m_pRootView) 
    {
        if (w > 0 && h > 0)
        {
            if ( (FLOAT)w != m_pRootView->GetWidth() ||
                 (FLOAT)h != m_pRootView->GetHeight() )
            {
                m_pRootView->SetViewSize((FLOAT)(w) , (FLOAT)(h));
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkWindowForm::OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            OnPaint(ps.hdc, &ps.rcPaint);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_ERASEBKGND:
        break;

    case WM_LBUTTONUP:          // Left button is up.
    case WM_LBUTTONDOWN:        // Left button presses down.
    case WM_MOUSEMOVE:          // Mouse cursor is move on window.
    case WM_MOUSELEAVE:         // Mouse cursor leaves the client of window.
    case WM_MOUSEHOVER:         // Mouse cursor stays over client of window.
    case WM_MOUSEWHEEL:         // Mouse wheel.
    case WM_RBUTTONUP:          // Right button up.
    case WM_RBUTTONDOWN:        // Right button down.
        OnMouseEvent(message, wParam, lParam);
        Invalidate();
        break;

    case WM_KEYDOWN:            // The key down message.
    case WM_KEYUP:              // The key up message.
    case WM_CHAR:               // The character message.
        OnKeyboardEvent(message, wParam, lParam);
        Invalidate();
        break;

    case WM_SIZE:
        OnSize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_CLOSE:
        OnClose();
        break;

    case WM_DESTROY:
        OnDestroy();
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK SdkWindowForm::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (WM_CREATE == message)
    {
        SdkWindowForm *pMainWin = (SdkWindowForm*)(((CREATESTRUCT*)lParam)->lpCreateParams);
        if (NULL != pMainWin)
        {
            pMainWin->m_hWnd = hWnd;
            pMainWin->OnCreating((CREATESTRUCT*)lParam);

            ::ShowWindow(hWnd, SW_HIDE);
            //::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pMainWin);
            ::SetProp(hWnd, SDKWINDOWFORM_PROP_NAME, (HANDLE)pMainWin);
        }

        return 0;
    }

    //SdkWindowForm *pThis = (SdkWindowForm*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
    SdkWindowForm *pThis = (SdkWindowForm*)::GetProp(hWnd, SDKWINDOWFORM_PROP_NAME);
    if (NULL != pThis)
    {
        return pThis->OnWndProc(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
