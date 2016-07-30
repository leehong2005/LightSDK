#include "StdAfx.h"
#include "SampleMainWindow.h"

SampleMainWindow::SampleMainWindow(void)
{
    m_pSampleWindows = NULL;
    m_pSampelFunWin  = NULL;
}

//////////////////////////////////////////////////////////////////////////

SampleMainWindow::~SampleMainWindow(void)
{
    SAFE_DELETE(m_pSampleWindows);
    SAFE_DELETE(m_pSampelFunWin);
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleMainWindow::InitializeWindow( HINSTANCE hInst, HWND hWndParent )
{
    WNDCLASSEX wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = SampleMainWindow::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInst;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = L"SampleMainWindow";
    wcex.hIconSm        = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));

    if ( !RegisterClassEx(&wcex) )
    {
        return FALSE;
    }

    HWND hWnd = CreateWindowEx(
        0,
        L"SampleMainWindow",
        L"SampleMainWindow",
        WS_OVERLAPPED | WS_SYSMENU| WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        0,
        1024,
        900,
        hWndParent,
        NULL,
        hInst,
        (LPVOID)this);

    if ( NULL != hWnd )
    {
        m_pSampelFunWin  = new SampleFunWindow();
        m_pSampleWindows = new SampleWindows();

        m_pSampelFunWin->SetTabPageWindow(m_pSampleWindows);
        m_pSampleWindows->SetFunWindow(m_pSampelFunWin);
        m_pSampelFunWin->InitializeWindow(hInst, hWnd);
        m_pSampleWindows->CreateWindowForm(hInst, hWnd);

        SdkCommonHelper::SetWindowCenter(hWnd);
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }

    return (NULL != hWnd);
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleMainWindow::OnWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        {
            int width  = LOWORD(lParam);
            int height = HIWORD(lParam);
            int topWinH = 570;

            if ( NULL != m_pSampleWindows )
            {
                m_pSampleWindows->SetWindowSize(width, (UINT32)topWinH);
            }

            if ( NULL != m_pSampelFunWin )
            {
                m_pSampelFunWin->SetWindowPos(0, (UINT32)topWinH);
                m_pSampelFunWin->SetWindowSize(width, (UINT32)(height - topWinH));
            }
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK SampleMainWindow::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if (WM_CREATE == message)
    {
        SampleMainWindow *pThis = static_cast<SampleMainWindow*>(((CREATESTRUCT*)lParam)->lpCreateParams);
        if (NULL != pThis)
        {
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
        }

        return 0;
    }

    SampleMainWindow *pThis = static_cast<SampleMainWindow*>((LPVOID)GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (NULL != pThis)
    {
        return pThis->OnWndProc(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}