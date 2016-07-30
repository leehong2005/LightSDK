// SamplePreviewHandler.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SampleFileSearch.h"
#include "Resource.h"
#include <shellapi.h>

struct ComboboxItem
{
    LPWSTR lpText;
    UINT uId;
    UINT scopeId;
};

static const ComboboxItem g_szComboboxItem[] = 
{
    { L""           ,QUERY_KIND_NONE     ,QUERY_SCOPE_NONE},
    { L"Musics"     ,QUERY_KIND_MUSIC    ,QUERY_SCOPE_MUSIC},
    { L"Pictures"   ,QUERY_KIND_PICTURE  ,QUERY_SCOPE_PICTURES},
    { L"Videos"     ,QUERY_KIND_VIDEO    ,QUERY_SCOPE_VIDEO},
    { L"Documents"  ,QUERY_KIND_DOCUMENT ,QUERY_SCOPE_DECUMENT},
    { L"Link"       ,QUERY_KIND_LINK     ,QUERY_SCOPE_FAVORITES},
};


SampleFileSearch::SampleFileSearch() : m_dialog(NULL),
                                       m_hInst(NULL),
                                       m_combobox(NULL),
                                       m_listview(NULL),
                                       m_pFileSearch(new SdkFileSearcher())
{
    m_szSelFilePath[0] = 0;
}

//////////////////////////////////////////////////////////////////////////

SampleFileSearch::~SampleFileSearch()
{
    SAFE_DELETE(m_pFileSearch);
}

//////////////////////////////////////////////////////////////////////////

HRESULT SampleFileSearch::Initialize(HINSTANCE hInstance)
{
    m_hInst = hInstance;
    HRESULT hr;
    m_dialog = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDC_DIALOG), NULL, (DLGPROC)DialogProc, (LPARAM)this);
    hr = m_dialog ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        SdkCommonHelper::SetWindowCenter(m_dialog);
        ShowWindow(m_dialog, SW_SHOWNORMAL);
        UpdateWindow(m_dialog);
    }
    return hr;
}

//////////////////////////////////////////////////////////////////////////

void SampleFileSearch::RunMessageLoop()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SampleFileSearch::OnDialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_NOTIFY:
        {
            NMHDR *nmhdr = (NMHDR*)lParam;
            if (NM_RCLICK == nmhdr->code)
            {
                NMITEMACTIVATE *pnmItem = (NMITEMACTIVATE*)lParam;
                if (NULL != pnmItem)
                {
                    TCHAR pszFilePath[MAX_PATH] = { 0 };
                    LVITEM lvItem = { 0 };
                    lvItem.iItem = pnmItem->iItem;
                    lvItem.iSubItem = 5;
                    lvItem.mask = LVIF_TEXT;
                    lvItem.cchTextMax = MAX_PATH;
                    lvItem.pszText = pszFilePath;

                    if ( ListView_GetItem(m_listview, &lvItem) )
                    {
                        wcscpy_s(m_szSelFilePath, MAX_PATH, pszFilePath);

                        //OpenFileFolder(pszFilePath);

                        HMENU hMenu = ::LoadMenu(m_hInst, MAKEINTRESOURCE(IDR_MENU_FILE));
                        if ( NULL != hMenu )
                        {
                            HMENU hMenuPopup = GetSubMenu(hMenu, 0);
                            if ( NULL != hMenuPopup )
                            {
                                POINT pt = pnmItem->ptAction;
                                ClientToScreen(m_listview, &pt);
                                TrackPopupMenuEx(
                                    hMenuPopup,
                                    TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                                    pt.x,
                                    pt.y,
                                    hWnd,
                                    NULL);

                                DestroyMenu(hMenu);
                            }
                        }
                    }
                }
            }
        }
        break;

    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case ID_OPEN_OPENFILELOCATION:
                OpenFileFolder(m_szSelFilePath);
                break;

            case ID_OPEN_OPENFILE:
                OpenFile(m_szSelFilePath);
                break;

            case IDC_SEARCH:
                {
                    ListView_DeleteAllItems(m_listview);

                    TCHAR SearchItemText[100];
                    GetDlgItemText(hWnd, IDC_EDIT, SearchItemText, 100);

                    LRESULT lResult = SendMessage(m_combobox, CB_GETCURSEL, 0, 0);
                    UINT uId = g_szComboboxItem[lResult].uId;
                    UINT scopeId = g_szComboboxItem[lResult].scopeId;

                    m_pFileSearch->SetScope(scopeId);
                    m_pFileSearch->SetCondition(uId);

                    if (0 != lstrlen(SearchItemText))
                    {
                        PCWSTR szSufix[] = { SearchItemText };
                        m_pFileSearch->SetCondition(szSufix, ARRAYSIZE(szSufix));
                    }
                    m_pFileSearch->SetSearchNotify(this);
                    m_pFileSearch->CancelSearch();
                    m_pFileSearch->SearchAsync();
                }
                break;

            case IDC_CLEAR:
                {
                    ListView_DeleteAllItems(m_listview);
                }
                break;
            }
        }

    default:
        return FALSE;
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SampleFileSearch::OnCreate(HWND hWndDlg)
{
    //Initialization combobox.
    m_combobox = GetDlgItem(hWndDlg, IDC_COMBOX);
    int size = ARRAYSIZE(g_szComboboxItem);
    for (int i = 0; i < size; i++)
    {
        SendMessage(m_combobox, CB_SETCURSEL, 0, 0);
        SendMessage(m_combobox, CB_ADDSTRING, 0, (LPARAM)(g_szComboboxItem[i].lpText));
    }

    //Initialization list view.
    m_listview = GetDlgItem(hWndDlg, IDC_LIST);
    RECT rect;
    GetClientRect(m_listview,&rect);
    int width = rect.right - rect.left;
    LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    lvc.fmt = LVCFMT_LEFT;

    lvc.pszText = L"Number";
    lvc.cx = (int)width / 15;
    SendMessage(m_listview, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);

    lvc.pszText =L"Name";
    lvc.cx = (int)width / 5;
    SendMessage(m_listview, LVM_INSERTCOLUMN, 1, (LPARAM)&lvc);

    lvc.pszText = L"Date modified";
    lvc.cx = (int)width / 6;
    SendMessage(m_listview, LVM_INSERTCOLUMN, 2, (LPARAM)&lvc);

    lvc.pszText = L"Type";
    lvc.cx = (int)width / 13;
    SendMessage(m_listview, LVM_INSERTCOLUMN, 3, (LPARAM)&lvc);

    lvc.pszText = L"Size";
    lvc.cx = (int)width / 12;
    SendMessage(m_listview, LVM_INSERTCOLUMN, 4, (LPARAM)&lvc);

    lvc.pszText = L"Folder path";
    lvc.cx = (int)width/ 3 + 15;
    SendMessage(m_listview, LVM_INSERTCOLUMN, 5, (LPARAM)&lvc);

    ListView_SetExtendedListViewStyle(m_listview, LVS_EX_FULLROWSELECT);

    //Add icon.
    HICON hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_SAMPLEPREVIEWHANDLER));
    SendMessage(hWndDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
    SendMessage(hWndDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
    DestroyIcon(hIcon);
}

//////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK SampleFileSearch::DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (WM_INITDIALOG == message)
    {
        SampleFileSearch *pThis = static_cast<SampleFileSearch*>((LPVOID)lParam);
        if (NULL != pThis)
        {
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
            pThis->OnCreate(hWnd);
        }

        return TRUE;
    }

    SampleFileSearch *pThis = static_cast<SampleFileSearch*>((LPVOID)GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (NULL != pThis)
    {
        return pThis->OnDialogProc(hWnd, message, wParam, lParam);
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SampleFileSearch::InserToList(FILEPROPERTY fileProperty)
{
    int nCount = ListView_GetItemCount(m_listview);
    TCHAR buffer[50] = { 0 };
    LV_ITEM lvi;
    lvi.mask = LVIF_TEXT;
    lvi.iItem = nCount;

    lvi.iSubItem = 0;
    swprintf_s(buffer, 50, L"%d", (nCount + 1));
    lvi.pszText = buffer;
    ListView_InsertItem(m_listview, &lvi);

    lvi.iSubItem++;
    lvi.pszText = &fileProperty.fileName[0];
    SendMessage(m_listview, LVM_SETITEM, 0, (LPARAM)&lvi);

    lvi.iSubItem++;
    lvi.pszText = &fileProperty.fileDate[0];
    SendMessage(m_listview, LVM_SETITEM, 0, (LPARAM)&lvi);

    lvi.iSubItem++;
    lvi.pszText = &fileProperty.fileType[0];
    SendMessage(m_listview, LVM_SETITEM, 0, (LPARAM)&lvi);

    lvi.iSubItem++;
    lvi.pszText =fileProperty.szBuffer;
    SendMessage(m_listview, LVM_SETITEM, 0, (LPARAM)&lvi);

    lvi.iSubItem++;
    lvi.pszText = &fileProperty.filePath[0];
    SendMessage(m_listview, LVM_SETITEM, 0, (LPARAM)&lvi);

    ListView_EnsureVisible(m_listview, nCount, FALSE);
}

//////////////////////////////////////////////////////////////////////////

void SampleFileSearch::OnSearchOneResult(IN SdkFileSearcher *pfileSearcher, IN LPCTSTR lpFilePath)
{
    m_vctFilePath.push_back(lpFilePath);
}

//////////////////////////////////////////////////////////////////////////

void SampleFileSearch::OnSearchFinish(IN SdkFileSearcher *pfileSearcher)
{
    pfileSearcher->ClearSearchScope();
    SetSearchData();
    m_vctFilePath.clear();
}

//////////////////////////////////////////////////////////////////////////

void SampleFileSearch::SetSearchData()
{
    const vector<wstring> searchResult = (0 != m_vctFilePath.size()) ? m_vctFilePath : m_pFileSearch->GetSearchResult();
    int size = searchResult.size();
    for (int i = 0; i < size; ++i)
    {
        wstring resutlt = searchResult[i];
        LPCTSTR filePath = resutlt.c_str();

        SdkFilePropHelper *pFilePropertyHelp = new SdkFilePropHelper(filePath);
        pFilePropertyHelp->GetString(SYSTEM_CORE_FILENAME, m_filePropert.fileName);
        pFilePropertyHelp->FormatForDisplay(SYSTEM_CORE_DATEMODIFIED, m_filePropert.szBuffer, 100);
        m_filePropert.fileDate = m_filePropert.szBuffer;
        pFilePropertyHelp->GetString(SYSTEM_CORE_ITEMTYPETEXT, m_filePropert.fileType);
        pFilePropertyHelp->FormatForDisplay(SYSTEM_CORE_SIZE, m_filePropert.szBuffer, 100);
        pFilePropertyHelp->GetString(SYSTEM_CORE_PARSINGPATH, m_filePropert.filePath);

        InserToList(m_filePropert);
        SAFE_DELETE(pFilePropertyHelp);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleFileSearch::OpenFileFolder(LPCTSTR lpFilePath)
{
    TCHAR szBuf[MAX_PATH] = { 0 };
    swprintf_s(szBuf, L"/select, %s", lpFilePath);

    ShellExecute(
        NULL,
        _T("open"),
        _T("explorer"),
        szBuf,
        NULL,
        SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////

void SampleFileSearch::OpenFile(LPCTSTR lpFilePath)
{
    ShellExecute(
        NULL,
        _T("open"),
        lpFilePath,
        NULL,
        NULL,
        SW_SHOW);
}