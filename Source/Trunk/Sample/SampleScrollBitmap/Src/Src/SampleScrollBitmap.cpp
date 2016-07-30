// SamplePreviewHandler.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SampleScrollBitmap.h"
#include "SdkUICommonInclude.h"

#define SCROLL_LINE_INC         5
#define SCROLL_PAGE_INC         50

#define BTN_CONNECT_ID          1000

SampleScrollBitmapWin::SampleScrollBitmapWin()
{
    m_hInst = NULL;
    m_hWnd = NULL;
    m_hBitmap = NULL;

    m_bmpSize.cx = 0;
    m_bmpSize.cy = 0;
    m_xCurScroll = 0;
    m_yCurScroll = 0;
    m_nTopOffset = 0;
}

//////////////////////////////////////////////////////////////////////////

SampleScrollBitmapWin::~SampleScrollBitmapWin()
{
    SAFE_DELETE_OBJECT(m_hBitmap);
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleScrollBitmapWin::CreateMyWindow( HINSTANCE hInst, HWND hWndParent )
{
    m_hInst = hInst;

    LPCWSTR lpClassName = L"SampleScrollBitmapWin";
    LPCWSTR lpTitle = L"SampleScrollBitmap";

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = SampleScrollBitmapWin::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInst;
    wcex.hIcon          = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SAMPLESCROLLBITMAP));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = lpClassName;
    wcex.hIconSm        = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassEx(&wcex);

    m_hWnd = CreateWindowEx(
        0,
        lpClassName,
        lpTitle,
        WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
        CW_USEDEFAULT,
        0,
        CW_USEDEFAULT,
        0,
        hWndParent,
        NULL,
        hInst,
        (LPVOID)this);

    if (NULL != m_hWnd)
    {
        SdkCommonHelper::SetWindowCenter(m_hWnd);

        m_hBitmap = SdkCommonHelper::GetWindowSnapshot(NULL);
        if (NULL != m_hBitmap)
        {
            BITMAP bmp;
            if ( GetObject(m_hBitmap, sizeof(BITMAP), (LPVOID)&bmp) )
            {
                m_bmpSize.cx = bmp.bmWidth;
                m_bmpSize.cy = bmp.bmHeight;
            }
        }
        ShowWindow(m_hWnd, SW_SHOW);
        UpdateWindow(m_hWnd);
    }

    return (NULL != m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

void SampleScrollBitmapWin::OnCreate(HWND hWnd)
{
    //HWND hWndBtn = CreateWindow(
    //    L"BUTTON",
    //    L"Button",
    //    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
    //    5,
    //    5,
    //    100,
    //    35,
    //    hWnd,
    //    (HMENU)BTN_CONNECT_ID,
    //    m_hInst,
    //    NULL);

    //UNREFERENCED_PARAMETER(hWndBtn);
}

//////////////////////////////////////////////////////////////////////////

void SampleScrollBitmapWin::OnVScroll(WPARAM wParam, LPARAM lParam)
{
    int xDelta = 0; 
    int yDelta = 0;     // yDelta = new_pos - current_pos 
    int yNewPos = 0;    // new position 

    RECT rc = { 0 };
    GetClientRect(m_hWnd, &rc);
    int yMaxScroll = max((m_bmpSize.cy - rc.bottom + m_nTopOffset), 0);

    switch (LOWORD(wParam)) 
    { 
        // User clicked the scroll bar shaft above the scroll box. 
    case SB_PAGEUP: 
        yNewPos = m_yCurScroll - SCROLL_PAGE_INC; 
        break; 

        // User clicked the scroll bar shaft below the scroll box. 
    case SB_PAGEDOWN: 
        yNewPos = m_yCurScroll + SCROLL_PAGE_INC; 
        break; 

        // User clicked the top arrow. 
    case SB_LINEUP: 
        yNewPos = m_yCurScroll - SCROLL_LINE_INC; 
        break; 

        // User clicked the bottom arrow. 
    case SB_LINEDOWN: 
        yNewPos = m_yCurScroll + SCROLL_LINE_INC;
        break; 

        // User dragged the scroll box. 
    case SB_THUMBPOSITION: 
        yNewPos = HIWORD(wParam);
        break; 

    case SB_THUMBTRACK:
        yNewPos = HIWORD(wParam);
        break;

    case SB_TOP:
        yNewPos = 0;
        break;

    case SB_BOTTOM:
        yNewPos = yMaxScroll;
        break;

    default: 
        yNewPos = m_yCurScroll; 
    } 

    // New position must be between 0 and the screen height. 
    yNewPos = max(0, yNewPos); 
    yNewPos = min(yMaxScroll, yNewPos);

    if (yNewPos != m_yCurScroll)
    {
        yDelta = yNewPos - m_yCurScroll;
        m_yCurScroll = yNewPos;

        rc.top += m_nTopOffset;
        ScrollWindowEx(
            m_hWnd,
            -xDelta,
            -yDelta,
            &rc,//NULL,
            &rc,//NULL,
            NULL,
            NULL,
            SW_INVALIDATE);

        UpdateWindow(m_hWnd);

        SCROLLINFO si = { 0 };
        si.cbSize = sizeof(si);
        si.fMask = SIF_POS;
        si.nPos = m_yCurScroll;
        SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleScrollBitmapWin::OnHScroll(WPARAM wParam, LPARAM lParam)
{
    int xDelta = 0;     // xDelta = new_pos - current_pos  
    int xNewPos = 0;    // new position 
    int yDelta = 0; 

    switch (LOWORD(wParam)) 
    { 
        // User clicked the scroll bar shaft left of the scroll box. 
    case SB_PAGEUP: 
        xNewPos = m_xCurScroll - SCROLL_PAGE_INC; 
        break; 

        // User clicked the scroll bar shaft right of the scroll box. 
    case SB_PAGEDOWN: 
        xNewPos = m_xCurScroll + SCROLL_PAGE_INC; 
        break; 

        // User clicked the left arrow. 
    case SB_LINELEFT: 
        xNewPos = m_xCurScroll - SCROLL_LINE_INC; 
        break; 

        // User clicked the right arrow. 
    case SB_LINERIGHT: 
        xNewPos = m_xCurScroll + SCROLL_LINE_INC; 
        break; 

        // User dragged the scroll box. 
    case SB_THUMBPOSITION: 
        xNewPos = HIWORD(wParam); 
        break;

    case SB_THUMBTRACK:
        xNewPos = HIWORD(wParam);
        break;

    default: 
        xNewPos = m_xCurScroll; 
    } 

    RECT rc = { 0 };
    GetClientRect(m_hWnd, &rc);

    int xMaxScroll = max((m_bmpSize.cx - rc.right), 0);
    xNewPos = max(0, xNewPos); 
    xNewPos = min(xMaxScroll, xNewPos);

    if (xNewPos != m_xCurScroll)
    {
        xDelta = xNewPos - m_xCurScroll;
        m_xCurScroll = xNewPos;

        rc.top += m_nTopOffset;

        ScrollWindowEx(
            m_hWnd,
            -xDelta,
            -yDelta,
            &rc,//NULL,
            &rc,//NULL,
            NULL,
            NULL,
            SW_INVALIDATE);
        UpdateWindow(m_hWnd);

        SCROLLINFO si = { 0 };
        si.cbSize = sizeof(si);
        si.fMask = SIF_POS;
        si.nPos = m_xCurScroll;
        SetScrollInfo(m_hWnd, SB_HORZ, &si, TRUE);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleScrollBitmapWin::OnKeyKown(WPARAM wParam, LPARAM lParam)
{
    WORD wScrollNotify = 0xFFFF;
    int nFlag = WM_VSCROLL;

    switch (wParam)
    {
    case VK_UP:
        wScrollNotify = SB_LINEUP;
        break;

    case VK_PRIOR:
        wScrollNotify = SB_PAGEUP;
        break;

    case VK_NEXT:
        wScrollNotify = SB_PAGEDOWN; 
        break; 

    case VK_DOWN:
        wScrollNotify = SB_LINEDOWN; 
        break;

    case VK_LEFT:
        wScrollNotify = SB_LINELEFT;
        nFlag = WM_HSCROLL;
        break;

    case VK_RIGHT:
        wScrollNotify = SB_LINERIGHT;
        nFlag = WM_HSCROLL;
        break;

    case VK_HOME:
        wScrollNotify = SB_TOP;
        break; 

    case VK_END:
        wScrollNotify = SB_BOTTOM;
        break; 
    }

    if (-1 != wScrollNotify)
    {
        SendMessage(m_hWnd, nFlag, (WPARAM)MAKELONG(wScrollNotify, 0), 0L);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleScrollBitmapWin::OnSize(int nNewX, int nNewY)
{
    // Set the scroll info for vertical and horizontal scroll bar.

    int xMaxScroll = max( (m_bmpSize.cx - nNewX), 0 );
    m_xCurScroll = min(m_xCurScroll, xMaxScroll);

    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMax = m_bmpSize.cx;
    si.nMin = 0;
    si.nPage = nNewX;
    si.nPos = m_xCurScroll;
    SetScrollInfo(m_hWnd, SB_HORZ, &si, TRUE);

    int yMaxScroll = max( (m_bmpSize.cy - nNewY), 0 );
    m_yCurScroll = min( m_yCurScroll, yMaxScroll );

    si.nMax = m_bmpSize.cy;
    si.nMin = 0;
    si.nPage = nNewY - m_nTopOffset;
    si.nPos = m_yCurScroll;
    SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
}

//////////////////////////////////////////////////////////////////////////

void SampleScrollBitmapWin::OnPaint( HDC hDC, LPRECT lprcPaint )
{
    if (NULL != m_hBitmap)
    {
        HDC hdcCompat = CreateCompatibleDC(hDC);
        HGDIOBJ hOldObj = SelectObject(hdcCompat, m_hBitmap);

        BitBlt(
            hDC,
            0,
            m_nTopOffset,
            m_bmpSize.cx,
            m_bmpSize.cy,
            hdcCompat,
            m_xCurScroll,
            m_yCurScroll,
            SRCCOPY);

        SelectObject(hdcCompat, hOldObj);
        DeleteDC(hdcCompat);
    }
}

//////////////////////////////////////////////////////////////////////////

LRESULT SampleScrollBitmapWin::OnWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch (message)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);
            OnPaint(hDC, &ps.rcPaint);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_SIZE:
        OnSize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_VSCROLL:
        OnVScroll(wParam, lParam);
        break;

    case WM_HSCROLL:
        OnHScroll(wParam, lParam);
        break;

    case WM_KEYDOWN:
        OnKeyKown(wParam, lParam);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK SampleScrollBitmapWin::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if (WM_CREATE == message)
    {
        SampleScrollBitmapWin *pThis = static_cast<SampleScrollBitmapWin*>(((CREATESTRUCT*)lParam)->lpCreateParams);
        if (NULL != pThis)
        {
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
            pThis->OnCreate(hWnd);
        }

        return 0;
    }

    SampleScrollBitmapWin *pThis = static_cast<SampleScrollBitmapWin*>((LPVOID)GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (NULL != pThis)
    {
        return pThis->OnWndProc(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
