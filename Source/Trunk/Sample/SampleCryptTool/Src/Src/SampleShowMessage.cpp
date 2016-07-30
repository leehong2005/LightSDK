/*!
* @file SampleShowMessage.cpp 
* 
* @brief the class will be show dialog for display error list
* 
* Copyright (C) 2008, Toshiba Corporation.
* 
* @author Li Hong
* @date 2009/01/06
*/

#include "StdAfx.h"
#include "SampleShowMessage.h"
#include "SdkCommonInclude.h"

#define ERRORMSG_DLG_LEFT           40                                // the left point of error message dialog
#define ERRORMSG_DLG_TOP            40                                // the top point of error message dialog
#define LISTVIEW_COLUMN_WIDTH       600                                 // the width of the list view column
#define MAX_BUFFER_SIZE             MAX_PATH * sizeof(TCHAR) * 2        // the max buffer size

CryptString g_strLabel;

//////////////////////////////////////////////////////////////////////////

SampleShowMessage::SampleShowMessage()
{
    m_hWnd = NULL;
    m_hWndHost = NULL;
}

//////////////////////////////////////////////////////////////////////////

SampleShowMessage::~SampleShowMessage()
{
    if (NULL != m_hWnd)
    {
        ::DestroyWindow(m_hWnd);
        m_hWnd = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////

HWND SampleShowMessage::ShowErroDialog(HINSTANCE hInst, HWND hWndParent)
{
    m_hWndHost = hWndParent;
    m_hWnd = CreateDialogParam(
        hInst,
        MAKEINTRESOURCE(IDD_DIALOG_MSG),
        NULL,
        SampleShowMessage::DlgProc,
        (WPARAM)this);

    return m_hWnd;
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleShowMessage::SetErrorMessage(LPCTSTR lpText)
{
    if (NULL == lpText)
    {
        return FALSE;
    }

    ::SendMessage(m_hWnd, WM_USER_ERROR, (WPARAM)lpText, 0);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

HWND SampleShowMessage::GetHWnd()
{
    return m_hWnd;
}

//////////////////////////////////////////////////////////////////////////

void SampleShowMessage::OnCommand(HWND hWnd, UINT id, HWND hWndCtl, UINT codeNotify)
{
    UNREFERENCED_PARAMETER(hWndCtl);
    UNREFERENCED_PARAMETER(codeNotify);

    switch( id )
    {
    case IDC_BTN_ERRORDLG_COPY:
        CopyErrorMessage( hWnd, GetStringFromListCtrl(::GetDlgItem(hWnd, IDC_LIST_ERRORDLG_MSG)) );
        break;

    case IDC_BTN_ERRORDLG_OK:
        ::ShowWindow(hWnd, SW_HIDE);
        ClearErrorMessage();
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleShowMessage::AddItem(IN HWND hWnd, IN HWND hListHwnd, IN const CryptString& strErrorMsg)
{
    LVITEM listItem;
    ZeroMemory(&listItem, sizeof(LVITEM));

    listItem.mask       = LVIF_TEXT;
    listItem.cchTextMax = (UINT32)strErrorMsg.length() * sizeof(TCHAR);
    listItem.pszText    = (LPWSTR)strErrorMsg.c_str();
    listItem.iItem      = ListView_GetItemCount(hListHwnd);
    listItem.iSubItem   = 0;

    ListView_InsertItem(hListHwnd, &listItem);

    UpdateErrorCount(hWnd, ::GetDlgItem(hWnd, IDC_STATIC_TOTALERROR));

    int nCount = ListView_GetItemCount(hListHwnd);
    ListView_EnsureVisible(hListHwnd, nCount - 1, FALSE);
}

//////////////////////////////////////////////////////////////////////////

void SampleShowMessage::InitialListCtrl(IN HWND hListHwnd)
{
    ListView_SetExtendedListViewStyle(hListHwnd,
        LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT |LVS_EX_LABELTIP);
    //EnableWindow( ListView_GetHeader(hListHwnd), FALSE );

    LV_COLUMN lvc;
    ZeroMemory(&lvc, sizeof(LV_COLUMN));

    CryptString strColumnName = _T("Message");

    lvc.mask        = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
    lvc.fmt         = LVCFMT_CENTER ;
    lvc.cx          = LISTVIEW_COLUMN_WIDTH;
    lvc.cchTextMax  = MAX_PATH * sizeof(TCHAR);
    lvc.iSubItem    = 0;
    lvc.pszText     = (LPWSTR)strColumnName.c_str();

    ListView_InsertColumn(hListHwnd, 0, &lvc);
}

//////////////////////////////////////////////////////////////////////////

void SampleShowMessage::UpdateErrorCount(IN HWND hWnd, IN HWND hStaticHwnd)
{
    CryptString strErrorMsg(g_strLabel);
    WCHAR chNum[10];
    ZeroMemory(chNum, 10 * sizeof(WCHAR));
    UINT32 nIndex = ListView_GetItemCount(::GetDlgItem(hWnd, IDC_LIST_ERRORDLG_MSG));

    wsprintf(chNum, L"%d", nIndex);
    strErrorMsg.append(chNum);

    SetWindowText(hStaticHwnd, (LPWSTR)strErrorMsg.c_str());
}

//////////////////////////////////////////////////////////////////////////

void SampleShowMessage::CopyErrorMessage(IN HWND hWnd, IN const CryptString& strCopy)
{
    if( OpenClipboard(hWnd) && EmptyClipboard() )
    {
        HGLOBAL hMem;
        LPTSTR pBuffer = NULL;
        DWORD dwLength = ((DWORD)strCopy.length() + 1) * sizeof(TCHAR);

        hMem = GlobalAlloc(GMEM_MOVEABLE, dwLength);
        if ( NULL == hMem )
        {
            CloseClipboard();
            return;
        }

        pBuffer = (LPTSTR)GlobalLock(hMem);
        if ( NULL == pBuffer )
        {
            CloseClipboard();
            return;
        }
        ZeroMemory(pBuffer, dwLength);

        memcpy(pBuffer, strCopy.c_str(), dwLength);
        GlobalUnlock(hMem);
        SetClipboardData(CF_UNICODETEXT, hMem);
        // close the clipboard
        CloseClipboard();
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleShowMessage::ClearErrorMessage()
{
    ListView_DeleteAllItems(::GetDlgItem(m_hWnd, IDC_LIST_ERRORDLG_MSG));
    UpdateErrorCount(m_hWnd, ::GetDlgItem(m_hWnd, IDC_STATIC_TOTALERROR));
}

//////////////////////////////////////////////////////////////////////////

CryptString SampleShowMessage::GetStringFromListCtrl(IN HWND hListHwnd)
{
    CryptString strErrorMsg(L"");

    UINT32 nCount = ListView_GetItemCount(hListHwnd);
    WCHAR strMsg[MAX_BUFFER_SIZE] = { 0 };

    for ( UINT32 i = 0; i < nCount; i++ )
    {
        ListView_GetItemText(hListHwnd, i, 0, strMsg, MAX_BUFFER_SIZE);
        strErrorMsg.append(strMsg).append(L"\n");
        ZeroMemory(strMsg, MAX_BUFFER_SIZE);
    }

    return strErrorMsg;
}

//////////////////////////////////////////////////////////////////////////

CryptString SampleShowMessage::LoadStr(UINT uID)
{
    static TCHAR strTest[1024] = { 0 };
    wmemset(strTest, 0, 1024);
    UINT nLength = ::LoadStringW(GetModuleHandle(NULL), uID, strTest, 1024);
    if(nLength <= 0)
    {
        wmemset(strTest, 0, 1024);
    }
    return strTest;
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SampleShowMessage::OnDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_COMMAND:
        OnCommand(hWnd, (UINT)LOWORD(wParam), (HWND)lParam, (UINT)HIWORD(wParam));
        break;

    case WM_USER_ERROR:
        {
            HWND hListHwnd = ::GetDlgItem(hWnd, IDC_LIST_ERRORDLG_MSG);
            AddItem(hWnd, hListHwnd, (TCHAR*)wParam);
            ::ShowWindow(hWnd, SW_SHOW);
        }
        break;

    case WM_CLOSE:
        ::ShowWindow(hWnd, SW_HIDE);
        ClearErrorMessage();
        break;

    default:
        return FALSE;
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK SampleShowMessage::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (WM_INITDIALOG == uMsg)
    {
        // Load icon
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ERROR_MSG));
        ::SendMessage( hWnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
        ::DestroyIcon(hIcon);

        RECT rect;
        GetWindowRect(hWnd, &rect);
        MoveWindow(hWnd, ERRORMSG_DLG_LEFT, ERRORMSG_DLG_TOP, rect.right - rect.left, rect.bottom - rect.top, TRUE);

        g_strLabel.clear();
        g_strLabel.append(_T("Total errors: "));

        SampleShowMessage* pThis = static_cast<SampleShowMessage*>((LPVOID)(LONG)lParam);
        if (NULL != pThis)
        {
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)pThis);
            pThis->InitialListCtrl(::GetDlgItem(hWnd, IDC_LIST_ERRORDLG_MSG));
            pThis->UpdateErrorCount(hWnd, ::GetDlgItem(hWnd, IDC_STATIC_TOTALERROR));
        }

        return FALSE;
    }

    SampleShowMessage* pThis = static_cast<SampleShowMessage*>((LPVOID)GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (NULL != pThis)
    {
        return pThis->OnDlgProc(hWnd, uMsg, wParam, lParam);
    }

    return FALSE;
}