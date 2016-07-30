// SamplePreviewHandler.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SampleKnownFolder.h"

#define ICON_WIDTH      128
#define ICON_HEIGHT     128
#define TOP_OFFSET      100

#define SCROLL_LINE_INC         5
#define SCROLL_PAGE_INC         50

struct KnownFolder
{
    UINT uCSIDL;
    LPCTSTR lpDesc;
};


KnownFolder szFolders[] = 
{
    { CSIDL_DESKTOP,  _T("Desktop") },
    { CSIDL_CONTROLS, _T("Control Panel") },
    { CSIDL_DRIVES,   _T("My Computer") },
    { CSIDL_MYMUSIC,  _T("My Music") },
    { CSIDL_MYVIDEO,  _T("My Video") },
};


SampleKnownFolderWin::SampleKnownFolderWin()
{
    m_hInst = NULL;
    m_hWnd  = NULL;
    m_nCurSel = -1;
    m_nCurScrollPos = 0;
    m_nCurOffset = 0;
    m_nMaxScroll = 0;
    ZeroMemory(&m_rcIcons, sizeof(RECT));
}

//////////////////////////////////////////////////////////////////////////

SampleKnownFolderWin::~SampleKnownFolderWin()
{
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleKnownFolderWin::CreateSampleKnownFolderWin( HINSTANCE hInst, HWND hWndParent )
{
    m_hInst = hInst;
    m_hWnd = CreateDialogParam(
        hInst,
        MAKEINTRESOURCE(IDD_DIALOG_KNOWNFOLDER),
        hWndParent,
        SampleKnownFolderWin::DialogProc,
        (LPARAM)this);

    ShowWindow(m_hWnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SampleKnownFolderWin::ClearFolerItemList()
{
    int nSize = (int)m_vctFolderItems.size();
    for (int i = 0; i < nSize; ++i)
    {
        SAFE_DELETE(m_vctFolderItems[i]);
    }

    m_vctFolderItems.clear();
}

//////////////////////////////////////////////////////////////////////////

void SampleKnownFolderWin::SetScrollBarInfo(int nTotalSize)
{
    HWND hWndScroll = GetDlgItem(m_hWnd, IDC_SCROLLBAR_V);
    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
    GetScrollInfo(hWndScroll, SB_CTL, &si);

    RECT rcClient = { 0 };
    GetClientRect(hWndScroll, &rcClient);

    si.nMin   = 0;
    si.nMax   = nTotalSize;
    si.nPage  = rcClient.bottom;

    SetScrollInfo(hWndScroll, SB_CTL, &si, TRUE);
}

//////////////////////////////////////////////////////////////////////////

void SampleKnownFolderWin::SetScrollBarPos(int nPos)
{

}

//////////////////////////////////////////////////////////////////////////

void SampleKnownFolderWin::OnPaint(HDC hDC, LPRECT lpRect)
{
    int nSize = (int)m_vctFolderItems.size();
    if (nSize > 0)
    {
        //RECT rc = { 100, 100, 356, 356 };

        //BLENDFUNCTION ftn = { 0 };
        //ftn.BlendOp = AC_SRC_OVER;
        //ftn.AlphaFormat = AC_SRC_ALPHA;
        //ftn.BlendFlags = 0;
        //ftn.SourceConstantAlpha = 255;

        //static HBITMAP m_hItemIcon = SdkCommonHelper::ExtractFileIcon(L"D:\\a.mp3", 256, 256);
        //HDC hMemDC = CreateCompatibleDC(hDC);
        //HGDIOBJ hOldObj = SelectObject(hMemDC, m_hItemIcon);

        //BitBlt(
        //    hDC,
        //    rc.left,
        //    rc.top,
        //    (rc.right - rc.left),
        //    (rc.bottom - rc.top),
        //    hMemDC,
        //    0,
        //    m_nCurScrollPos,
        //    SRCCOPY);

        //BITMAP bmp;
        //GetObject(m_hItemIcon, sizeof(bmp), (LPVOID)&bmp);

        //AlphaBlend(
        //    hDC,
        //    rc.left,
        //    rc.top - m_nCurScrollPos,
        //    (rc.right - rc.left),
        //    (rc.right - rc.left),
        //    hMemDC,
        //    0,
        //    0,
        //    bmp.bmWidth,
        //    bmp.bmHeight,
        //    ftn);

        //SelectObject(hMemDC, hOldObj);
        //DeleteDC(hMemDC);

        BOOL isBitmap = (1 == IsDlgButtonChecked(m_hWnd, IDC_RADIO_BITMAP));
        for (int i = 0; i < nSize; ++i)
        {
            SampleFolderItem *pItem = m_vctFolderItems[i];
            pItem->OnPaint(hDC, isBitmap, -m_nCurScrollPos);
        }

        OutputDebugString(L"OnPaint.........");

        //HDC hDCMem = CreateCompatibleDC(hDC);
        //HBITMAP hBitmapMem = CreateCompatibleBitmap(
        //    hDC,
        //    m_rcIcons.right - m_rcIcons.left,
        //    m_rcIcons.bottom - m_rcIcons.top);

        //HGDIOBJ hOldObj = SelectObject(hDCMem, hBitmapMem);
        //FillRect(hDCMem, &m_rcIcons, CreateSolidBrush(RGB(255, 255, 255)));

        //BOOL isBitmap = (1 == IsDlgButtonChecked(m_hWnd, IDC_RADIO_BITMAP));
        //for (int i = 0; i < nSize; ++i)
        //{
        //    SampleFolderItem *pItem = m_vctFolderItems[i];
        //    pItem->OnPaint(hDCMem, isBitmap, 0);
        //}

        //BitBlt(
        //    hDC,
        //    m_rcIcons.left,
        //    m_rcIcons.top,
        //    (m_rcIcons.right - m_rcIcons.left),
        //    (m_rcIcons.bottom - m_rcIcons.top),
        //    hDCMem,
        //    0,
        //    m_nCurScrollPos,
        //    SRCAND);

        //SelectObject(hDCMem, hOldObj);
        //DeleteObject(hBitmapMem);
        //DeleteDC(hDCMem);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleKnownFolderWin::OnApplyBtnClick(HWND hWndBtn)
{
    HWND hWndCombo = GetDlgItem(m_hWnd, IDC_COMBO_FOLDER);
    int nCurSel = SendMessage(hWndCombo, CB_GETCURSEL, 0, 0);
    if (m_nCurSel == nCurSel)
    {
        return;
    }

    m_nCurSel = nCurSel;
    int nTotalSize = 0;

    if (nCurSel >= 0)
    {
        GetClientRect(m_hWnd, &m_rcIcons);
        m_rcIcons.top = TOP_OFFSET;
        m_rcIcons.right -= 10;

        KnownFolder &folder = szFolders[nCurSel];
        SdkKnownFolderUtil folderMgr;
        if ( folderMgr.InitFolderInfo(folder.uCSIDL, ICON_WIDTH, ICON_HEIGHT) )
        {
            ClearFolerItemList();

            RECT rcClient = { 0 };
            GetClientRect(m_hWnd, &rcClient);
            int left = 10;
            int top = TOP_OFFSET;
            int width = ICON_WIDTH;
            int height = ICON_HEIGHT + 40;
            int margin = 10;

            int nCount = folderMgr.GetItemCount();
            for (int i = 0; i < nCount; ++i)
            {
                if ((left + width + margin) >= rcClient.right)
                {
                    left = 10;
                    top += (height + margin);
                }

                RECT rcPaint = { 0 };
                rcPaint.bottom = height;
                rcPaint.right = width;
                OffsetRect(&rcPaint, left, top);

                KNOWNITEM item = { 0 };
                folderMgr.GetItem(i, &item);

                SampleFolderItem *pFolderItem = new SampleFolderItem();
                pFolderItem->SetClipRect(&m_rcIcons);
                pFolderItem->Initialize(item.hBitmap, item.hIcon, item.szDisplayName, &rcPaint);
                m_vctFolderItems.push_back(pFolderItem);

                left += (width + margin);
            }

            nTotalSize = top + height;
        }

        if (nTotalSize <= (m_rcIcons.bottom - m_rcIcons.top))
        {
            ::ShowScrollBar(GetDlgItem(m_hWnd, IDC_SCROLLBAR_V), SB_CTL, FALSE);
        }
        else
        {
            m_nMaxScroll = nTotalSize - (m_rcIcons.bottom - m_rcIcons.top);
            ::ShowScrollBar(GetDlgItem(m_hWnd, IDC_SCROLLBAR_V), SB_CTL, TRUE);
            SetScrollBarInfo(nTotalSize);
        }

        TCHAR szBuffer[50] = { 0 };
        swprintf_s(szBuffer, L"Item count: %d", folderMgr.GetItemCount());
        ::SetDlgItemText(m_hWnd, IDC_STATIC_TEXT, szBuffer);

        InvalidateRect(m_hWnd, &m_rcIcons, TRUE);
        UpdateWindow(m_hWnd);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleKnownFolderWin::OnClearBtnClick(HWND hWndBtn)
{
    ClearFolerItemList();

    TCHAR szBuffer[50] = { 0 };
    swprintf_s(szBuffer, L"Item count: %d", 0);
    ::SetDlgItemText(m_hWnd, IDC_STATIC_TEXT, szBuffer);

    ::ShowScrollBar(GetDlgItem(m_hWnd, IDC_SCROLLBAR_V), SB_CTL, FALSE);

    m_nCurScrollPos = 0;
    m_nCurOffset = 0;
    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    SetScrollInfo(GetDlgItem(m_hWnd, IDC_SCROLLBAR_V), SB_CTL, &si, TRUE);

    InvalidateRect(m_hWnd, &m_rcIcons, TRUE);
    UpdateWindow(m_hWnd);
}

//////////////////////////////////////////////////////////////////////////

void SampleKnownFolderWin::OnScroll(WPARAM wParam, LPARAM lParam)
{
    int yNewPos = 0;

    switch (LOWORD(wParam))
    {
    case SB_PAGEUP:
        yNewPos = m_nCurScrollPos - SCROLL_PAGE_INC;
        break;

    case SB_PAGEDOWN:
        yNewPos = m_nCurScrollPos + SCROLL_PAGE_INC;
        break;

    case SB_LINEUP:
        yNewPos = m_nCurScrollPos - SCROLL_LINE_INC;
        break;

    case SB_LINEDOWN:
        yNewPos = m_nCurScrollPos + SCROLL_LINE_INC;
        break;

    case SB_THUMBPOSITION:
        yNewPos = HIWORD(wParam); 
        break;

    case SB_THUMBTRACK:
        yNewPos = HIWORD(wParam); 
        break;

    case SB_ENDSCROLL:
        m_nCurOffset = m_nCurScrollPos;
        yNewPos = m_nCurScrollPos;
        break;

    default:
        yNewPos = m_nCurScrollPos;
        break;
    }

    yNewPos = max(0, yNewPos); 
    yNewPos = min(m_nMaxScroll, yNewPos);

    if (yNewPos != m_nCurScrollPos)
    {
        int yDelta = yNewPos - m_nCurScrollPos; 
        m_nCurScrollPos = yNewPos;

        m_rcIcons.bottom += 20;
        RECT rcClip = m_rcIcons;
        rcClip.bottom += 20;

        ScrollWindowEx(
            m_hWnd,
            0,
            -yDelta,
            &m_rcIcons,
            &rcClip,
            (HRGN)NULL,
            (LPRECT)NULL, 
            SW_INVALIDATE);

        UpdateWindow(m_hWnd);

        SCROLLINFO si = { 0 };
        si.cbSize = sizeof(si);
        si.fMask = SIF_POS;
        si.nPos = m_nCurScrollPos;

        SdkCommonHelper::PrintDebugString(L"m_nCurScrollPos = %d, ", m_nCurScrollPos);

        SetScrollInfo(GetDlgItem(m_hWnd, IDC_SCROLLBAR_V), SB_CTL, &si, TRUE);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleKnownFolderWin::OnSize(int nNewX, int nNewY)
{
}

//////////////////////////////////////////////////////////////////////////

void SampleKnownFolderWin::OnCreate(HWND hwndDlg)
{
    m_hWnd = hwndDlg;
    SdkCommonHelper::SetWindowCenter(hwndDlg);

    // Set icon of dialog.
    HICON hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_SAMPLEKNOWNFOLDE));
    ::SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
    ::SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
    ::DestroyIcon(hIcon);

    HWND hWndCombo = GetDlgItem(hwndDlg, IDC_COMBO_FOLDER);
    int nSize = ARRAYSIZE(szFolders);
    for (int i = 0; i < nSize; ++i)
    {
        SendMessage(hWndCombo, CB_ADDSTRING, 0, (LPARAM)(szFolders[i].lpDesc));
    }

    SendMessage(hWndCombo, CB_SETCURSEL, (WPARAM)0, 0);
    SendMessage(GetDlgItem(m_hWnd, IDC_RADIO_BITMAP), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
    ShowScrollBar(GetDlgItem(m_hWnd, IDC_SCROLLBAR_V), SB_CTL, FALSE);

    RECT rcClient = { 0 };
    GetClientRect(hwndDlg, &rcClient);

    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
    si.nMax = 0;
    si.nMin = 0;
    si.nPage = rcClient.bottom - TOP_OFFSET;
    si.nPos = 0;

    SetScrollInfo(GetDlgItem(m_hWnd, IDC_SCROLLBAR_V), SB_CTL, &si, TRUE);
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SampleKnownFolderWin::OnDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwndDlg);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hwndDlg, &ps);
            OnPaint(hDC, NULL);
            EndPaint(hwndDlg, &ps);
        }
        break;

    case WM_VSCROLL:
        OnScroll(wParam, lParam);
        break;

    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            switch (LOWORD(wParam))
            {
            case IDC_BTN_APPLY:
                OnApplyBtnClick((HWND)lParam);
                break;

            case IDC_BTN_CLEAR:
                OnClearBtnClick((HWND)lParam);
                break;

            case IDC_RADIO_BITMAP:
            case IDC_RADIO_ICON:
                InvalidateRect(m_hWnd, &m_rcIcons, TRUE);
                break;
            }
        }
        break;

    default:
        return FALSE;
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK SampleKnownFolderWin::DialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if (WM_INITDIALOG == uMsg)
    {
        SampleKnownFolderWin *pThis = static_cast<SampleKnownFolderWin*>((LPVOID)lParam);
        if (NULL != pThis)
        {
            SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG_PTR)pThis);
            pThis->OnCreate(hwndDlg);
        }

        return TRUE;
    }

    SampleKnownFolderWin *pThis = static_cast<SampleKnownFolderWin*>((LPVOID)GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
    if (NULL != pThis)
    {
        return pThis->OnDialogProc(hwndDlg, uMsg, wParam, lParam);
    }

    return FALSE;
}
