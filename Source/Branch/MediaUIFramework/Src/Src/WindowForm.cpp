/*!
* @file MainWindow.cpp
* 
* @brief The implementation of functions in MainWindow.h file.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/28
*/

#include "WindowForm.h"

BOOL WindowForm::s_hasRegClass = FALSE;

WindowForm::WindowForm() : m_hInstance(NULL),
                           m_isLBDown(FALSE),
                           m_isTracked(FALSE),
                           m_isSyncPerformEvents(FALSE),
                           m_isEraseBK(TRUE),
                           m_nExitCode(0),
                           m_blendAlpha(255),
                           m_hMemBitmap(NULL),
                           m_pMouseSpeedTracker(NULL)
{
    m_pMouseSpeedTracker = new MouseSpeedTracker();
}

//////////////////////////////////////////////////////////////////////////

WindowForm::~WindowForm()
{
    if (NULL != m_hMemBitmap)
    {
        DeleteObject(m_hMemBitmap);
    }

    SAFE_DELETE(m_pMouseSpeedTracker);

    SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)0);
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowForm::CreateWindowForm(HINSTANCE hInstance, HWND hWndParent, INT32 nCmdShow)
{
    if (!s_hasRegClass)
    {
        if (!RegisterWindowClass(hInstance, WINDOWFORMCLASSNAME))
        {
            return FALSE;
        }
        s_hasRegClass = TRUE;
    }

    CREATEWINDOWPARAMS createParams = { 0 };
    createParams.cx             = 800;
    createParams.cy             = 600;
    createParams.hInstance      = hInstance;
    createParams.hwndParent     = hWndParent;
    createParams.hMenu          = NULL;
    createParams.lpCreateParams = this;
    createParams.lpszClass      = WINDOWFORMCLASSNAME;
    createParams.lpszName       = L"WindowForm";
    createParams.dwStyle        = WS_OVERLAPPEDWINDOW;

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
                               this);

    if (NULL == hWnd)
    {
        return FALSE;
    }

    m_hWnd       = hWnd;
    m_hWndParent = hWndParent;
    m_hInstance  = hInstance;

    OnCreated();
    ShowWindow(hWnd, nCmdShow);

    BOOL isLayeredWindow = IsLayeredWindow();
    if (isLayeredWindow)
    {
        SetOpacity(0.01f);
        OnDrawUI();
    }
    else
    {
        UpdateWindow(hWnd);
    }

    return (NULL != hWnd) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::GetClassName(LPTSTR lpszClassName, UINT32 uSize)
{
    // Not implement current.
    if ( NULL != lpszClassName )
    {
        _tcscpy_s(lpszClassName, uSize, WINDOWFORMCLASSNAME);
    }
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::Invalidate(BOOL isErase, const LPRECT lprcPaint)
{
    BOOL hasDraw = FALSE;
    BOOL isLayeredWindow = IsLayeredWindow();
    if (isLayeredWindow)
    {
        if (isErase)
        {
            hasDraw = OnDrawUI(isErase, lprcPaint);
        }
        else
        {
            if (m_nWindowState & WINDOW_STATE_INVALIDATE)
            {
                hasDraw = OnDrawUI(FALSE, lprcPaint);
            }
        }
    }
    else
    {
        Window::Invalidate(isErase, lprcPaint);
    }

    if (hasDraw)
    {
        RemoveWindowState(WINDOW_STATE_INVALIDATE);
    }
}

//////////////////////////////////////////////////////////////////////////

DOUBLE WindowForm::GetSpeedX()
{
    return m_pMouseSpeedTracker->GetSpeedX();
}

//////////////////////////////////////////////////////////////////////////

DOUBLE WindowForm::GetSpeedY()
{
    return m_pMouseSpeedTracker->GetSpeedY();
}

//////////////////////////////////////////////////////////////////////////

DWORD WindowForm::GetSpeedSampleTime()
{
    return m_pMouseSpeedTracker->GetSpeedSampleTime();
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowForm::InitializeDevice()
{
    if (NULL == m_pD2DDevice)
    {
        m_pD2DDevice = new D2DDevice();
    }

    HRESULT hr1 = m_pD2DDevice->InitDevice(m_hWnd);
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

BOOL WindowForm::RegisterWindowClass(HINSTANCE hInstance, LPCTSTR lpClassName)
{
    WNDCLASSEX wcex;

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WindowForm::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = (NULL == lpClassName) ? WINDOWFORMCLASSNAME : lpClassName;
    wcex.hIconSm        = NULL;

    return RegisterClassEx(&wcex);
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::PushEventView(ViewElement *pView)
{
    if (NULL != pView && !m_isSyncPerformEvents)
    {
        m_vctEventViews.push_back(pView);
    }
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::CancelTargetViewEvent()
{
    for (map<UINT, ViewElement*>::iterator itor = m_mapTargetViews.begin();
         itor != m_mapTargetViews.end(); ++itor)
    {
        if (NULL != (itor->second))
        {
            (itor->second)->CancelViewEvent(TRUE);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowForm::HideCurPopupView(LONG x, LONG y)
{
    ViewElement *pCurPopupView = Window::WindowViews.CurrentPopupView;

    if (NULL != pCurPopupView)
    {
        BOOL isPopup   = (VIEW_STATE_POPUP == (pCurPopupView->GetState() & VIEW_STATE_POPUP));
        BOOL isTouchOn = pCurPopupView->IsPtInRect((FLOAT)x, (FLOAT)y);
        if (isPopup && !isTouchOn)
        {
            pCurPopupView->ShowView(FALSE);
            pCurPopupView = NULL;
        }

        return (isPopup && !isTouchOn);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowForm::DispatchMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ( (NULL == m_pContentView) || !m_pContentView->IsEnable() ||
         (!IsWindow(m_hWnd)))
    {
        return FALSE;
    }

    BOOL handled = FALSE;
    MSG msg = { 0 };
    msg.message = message;
    msg.lParam  = lParam;
    msg.wParam  = wParam;

    ViewElement *pSource = m_pContentView->FindEventSource(message, wParam, lParam);
    if (NULL != pSource)
    {
        // Hide current visible pop-up view.
        if ( (WM_LBUTTONDOWN == message) || (WM_RBUTTONDOWN == message) )
        {
            handled = HideCurPopupView(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }

        if (!handled)
        {
            // Get the event target and dispatch mouse event from this target to root view.
            ViewElement *pTarget = NULL;
            if ( (WM_LBUTTONUP == message) || (WM_MOUSEMOVE == message) )
            {
                pTarget = Window::WindowViews.EventTargetView;
            }

            if (NULL != pTarget)
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
        if ( (WM_MOUSEMOVE == message) || (WM_MOUSELEAVE == message) ||
             (WM_LBUTTONUP == message) )
        {
            ViewElement *pTarget = Window::WindowViews.EventTargetView;
            if (NULL != pTarget)
            {
                handled = pTarget->DispatchMouseEvent(&msg);
            }
            else
            {
                handled = m_pContentView->PreDispathMouseEvent(&msg, m_pContentView);
            }
        }
    }

    return handled;
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowForm::DispatchTouchEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (NULL != m_pContentView && m_pContentView->IsEnable())
    {
        MSG msg = { 0 };
        msg.message = message;
        msg.lParam  = lParam;
        msg.wParam  = wParam;

        m_pContentView->PreDispathTouchEvent(&msg, m_pContentView);
        {
            m_pContentView->DispatchTouchEvent(&msg);
        }
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::PerformViewEvent(UINT message)
{
    if (WM_LBUTTONUP == message)
    {
        if (!m_isSyncPerformEvents)
        {
            for each (ViewElement *pChild in m_vctEventViews)
            {
                m_isSyncPerformEvents = TRUE;
                pChild->PerformAssocEvent();
            }

            m_vctEventViews.clear();

            m_isSyncPerformEvents = FALSE;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::OnPaint(HDC hdc, const RECT& rcPaint)
{
    if (NULL != m_pD2DDevice)
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

void WindowForm::OnHwndPaint(HDC hdc, const RECT& rcPaint)
{
    if (NULL != m_pD2DDevice && !m_pD2DDevice->IsWindowOccluded())
    {
        m_pD2DDevice->SetPaintTargetType(DEVICE_TARGET_TYPE_HWND);
        m_pD2DDevice->SetOpacity(m_fOpacity);
        m_pD2DDevice->BeginDraw(hdc, NULL);
        if (NULL != m_pContentView)
        {
            m_pContentView->OnPaint();
        }
        m_pD2DDevice->EndDraw();
    }
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::OnDCPaint(HDC hdc, const RECT& rcPaint)
{
    if (NULL != m_pD2DDevice)
    {
        m_pD2DDevice->SetPaintTargetType(DEVICE_TARGET_TYPE_DC);
        m_pD2DDevice->SetOpacity(m_fOpacity);
        m_pD2DDevice->BeginDraw(hdc, NULL);
        if (NULL != m_pContentView)
        {
            m_pContentView->OnPaint();
        } 
        m_pD2DDevice->EndDraw();
    }
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::OnMemPaint(HDC hdc, const RECT& rcPaint)
{
    if (NULL != m_pD2DDevice)
    {
        m_pD2DDevice->SetPaintTargetType(DEVICE_TARGET_TYPE_MEMORY);
        m_pD2DDevice->SetOpacity(m_fOpacity);
        m_pD2DDevice->BeginDraw(hdc, NULL);
        if (NULL != m_pContentView)
        {
            m_pContentView->OnPaint();
        }
        m_pD2DDevice->EndDraw();

        // Get HBITMAP and draw to DC using GDI.
        HBITMAP hBitmap = m_pD2DDevice->GetMemHBITMAP();
        if (NULL != hBitmap)
        {
            OnDrawBitmap(hdc, hBitmap);
            DeleteObject(hBitmap);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowForm::OnDrawBitmap(HDC hDC, IN const LPRECT lpRect)
{
    if ((NULL == hDC) || (NULL == lpRect))
    {
        return FALSE;
    }

    HDC hMemDC = CreateCompatibleDC(hDC);
    HGDIOBJ hOldObj = SelectObject(hMemDC, m_hMemBitmap);

    BitBlt(hDC,
        lpRect->left,
        lpRect->top,
        (lpRect->right - lpRect->left),
        (lpRect->bottom - lpRect->top),
        hMemDC,
        0,
        0,
        SRCCOPY);

    SelectObject(hMemDC, hOldObj);
    DeleteDC(hMemDC);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::OnDrawBitmap(HDC hDC, HBITMAP hBitmap, IN const RECT *paintRC)
{
    if (NULL != hBitmap)
    {
        RECT rc = { 0 };
        if (NULL == paintRC)
        {
            BITMAP bmp = { 0 };
            if ( 0 == GetObject(hBitmap, sizeof(BITMAP), &bmp) )
            {
                return;
            }

            rc.right  = bmp.bmWidth;
            rc.bottom = bmp.bmHeight;
        }
        else
        {
            rc = *paintRC;
        }

        HDC hMemDC = CreateCompatibleDC(hDC);
        HGDIOBJ hOldObj = SelectObject(hMemDC, hBitmap);
        BitBlt(hDC, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), hMemDC, 0, 0, SRCCOPY);
        SelectObject(hMemDC, hOldObj);
        DeleteDC(hMemDC);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowForm::OnDrawUI(BOOL fAlwaysDraw, const LPRECT lprcPaint)
{
    if (!fAlwaysDraw)
    {
        BOOL isLayeredWindow = IsLayeredWindow();
        BOOL isWindowVisible = IsWindowVisible(m_hWnd);
        if (!isLayeredWindow || !isWindowVisible)
        {
            return FALSE;
        }
    }

    HDC hdc = GetWindowDC(m_hWnd);
    RECT clientRect = { 0 };
    GetClientRect(m_hWnd, &clientRect);

    LONG nWidth = RECT_WIDTH(clientRect);
    LONG nHeight = RECT_HEIGHT(clientRect);

    if (NULL == m_hMemBitmap)
    {
        m_hMemBitmap = CreateCompatibleBitmap(hdc, nWidth, nHeight);
    }

    HDC hMemDC = CreateCompatibleDC(hdc);
    HGDIOBJ hOldObj = SelectObject(hMemDC, m_hMemBitmap);

    OnDCPaint(hMemDC, clientRect);

    BLENDFUNCTION blend = { AC_SRC_OVER, 0, m_blendAlpha, AC_SRC_ALPHA };
    SIZE size = { nWidth, nHeight };
    POINT pointSrc  = { 0, 0 };
    UpdateLayeredWindow(m_hWnd, hdc, NULL, &size, hMemDC, &pointSrc, 0, &blend, ULW_ALPHA);

    SelectObject(hMemDC, hOldObj);
    DeleteObject(hMemDC);
    ReleaseDC(m_hWnd, hdc);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::OnCreated()
{
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::OnCreating(LPCREATESTRUCT lpCreateStruct)
{
    UNREFERENCED_PARAMETER(lpCreateStruct);

    // Not implements.
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::OnPreCreate(LPCREATEWINDOWPARAMS lpCreateWinParams)
{
    UNREFERENCED_PARAMETER(lpCreateWinParams);

    // Not implements.
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowForm::OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    m_pMouseSpeedTracker->OnMouseEvent(m_hWnd, message, wParam, lParam);

    BOOL handled = FALSE;

    switch(message)
    {
    case WM_MOUSEMOVE:
        {
            if (!m_isTracked)
            {
                TRACKMOUSEEVENT EventTrack;
                EventTrack.cbSize = sizeof(TRACKMOUSEEVENT);
                EventTrack.dwFlags = TME_HOVER | TME_LEAVE;
                EventTrack.hwndTrack = m_hWnd;
                EventTrack.dwHoverTime = 1;
                m_isTracked = TrackMouseEvent(&EventTrack);
            }

            if (m_isLBDown)
            {
                handled = DispatchMouseEvent(message, wParam, lParam);
            }
        }
        break;

    case WM_MOUSELEAVE:
        {
            m_isTracked = FALSE;
            m_isLBDown  = FALSE;
            handled = DispatchMouseEvent(message, wParam, lParam);
            Window::WindowViews.EventTargetView = NULL;
        }
        break;

    case WM_RBUTTONUP:
        {
            m_isLBDown  = FALSE;
            handled = DispatchMouseEvent(message, wParam, lParam);

            // If the left button is NOT hold down.
            if (MK_LBUTTON != (MK_LBUTTON & (LONG)wParam))
            {
                ReleaseCapture();
            }
        }
        break;

    case WM_LBUTTONUP:
        {
            m_isLBDown  = FALSE;
            handled = DispatchMouseEvent(message, wParam, lParam);
            Window::WindowViews.EventTargetView = NULL;

            // If the right button is NOT hold down.
            if (MK_RBUTTON != (MK_RBUTTON & (LONG)wParam))
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

    case WM_LBUTTONDOWN:
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

BOOL WindowForm::OnTouchEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(message);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    return DispatchTouchEvent(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::OnDestroy()
{
    if (NULL == m_hWndParent)
    {
        PostQuitMessage(m_nExitCode);
    }
}

//////////////////////////////////////////////////////////////////////////

void WindowForm::OnSize(int w, int h)
{
    if (NULL != m_hMemBitmap)
    {
        DeleteObject(m_hMemBitmap);
        m_hMemBitmap = NULL;
    }

    if (NULL != m_pD2DDevice)
    {
        FLOAT fScale = CommonHelper::GetScale();
        m_pD2DDevice->Resize((UINT32)(w * fScale), (UINT32)(h * fScale));
    }

    // When application is iconic, system will send WM_SIZE message, and got width and
    // height is zero, so in this case, we should not reset content view's size, because
    // if do so, it will lead unnecessary layout.
    if (NULL != m_pContentView)
    {
        if (w > 0 && h > 0)
        {
            if ( (FLOAT)w != m_pContentView->GetWidth() ||
                (FLOAT)h != m_pContentView->GetHeight() )
            {
                m_pContentView->SetViewSize((FLOAT)(w) , (FLOAT)(h));
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL WindowForm::OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            OnPaint(ps.hdc, ps.rcPaint);
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

    case WM_SIZE:
        OnSize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_DESTROY:
        OnDestroy();
        break;

    case WM_DISPLAYCHANGE:
        {
            // Hide pop-up view when display change.
            HideCurPopupView();
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowForm::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (WM_CREATE == message)
    {
        WindowForm *pMainWin = (WindowForm*)(((CREATESTRUCT*)lParam)->lpCreateParams);
        if (NULL != pMainWin)
        {
            pMainWin->m_hWnd = hWnd;
            pMainWin->OnCreating((CREATESTRUCT*)lParam);
            ShowWindow(hWnd, SW_HIDE);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pMainWin);
        }

        return 0;
    }

    WindowForm *pThis = (WindowForm*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (NULL != pThis)
    {
        return pThis->OnWndProc(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
