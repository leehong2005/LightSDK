#include "StdAfx.h"
#include "SampleListView.h"

#define EDIT_SUBITEM        2

TCHAR* g_szListViewColNames[] =
{
    TEXT("Param name"),
    TEXT("Param type"),
    TEXT("Param value (Click to edit)"),
    TEXT("Return value (if have)"),
};


//////////////////////////////////////////////////////////////////////////

SampleListView::SampleListView()
{
    m_nEditItem          = -1;
    m_nEditSubItem       = 0;
    m_isEditing          = FALSE;
    m_isEditable         = FALSE;
    m_hWndList           = NULL;
    m_wpOrigEditProc     = NULL;
    m_wpOrigListViewProc = NULL;

    ZeroMemory(&m_rcEditSubItem, sizeof(RECT));
}

//////////////////////////////////////////////////////////////////////////

SampleListView::~SampleListView()
{
}

//////////////////////////////////////////////////////////////////////////

HWND WINAPI SampleListView::CreateListView(HWND hWndParent,UINT uId)
{
    // First create window through CreateWindowEx
    HWND hwndStrListView = CreateWindowEx(
        0L,
        WC_LISTVIEW,
        NULL,
        WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS,
        0,
        0,
        0,
        0,
        hWndParent,
        (HMENU)uId,
        GetModuleHandle(NULL),
        NULL);

    ShowWindow(hwndStrListView,SW_SHOWDEFAULT);

    return hwndStrListView;
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleListView::AddListViewColumns(HWND hWndList)
{
    m_hWndList = hWndList;

    LV_COLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    lvc.fmt = LVCFMT_LEFT;
    lvc.cx = 170;

    int nCount = ARRAYSIZE(g_szListViewColNames);
    for (int i = 0; i < nCount; ++i)
    {
        lvc.pszText = g_szListViewColNames[i];
        ListView_InsertColumn(hWndList, i, &lvc);
        lvc.cx += 30;
    }

    ListView_SetExtendedListViewStyle(hWndList, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    SetWindowLong(hWndList, GWL_USERDATA, (LONG)this);
    m_wpOrigListViewProc = (WNDPROC)SetWindowLong(
        hWndList,
        GWL_WNDPROC,
        (LONG)ListViewSubclassProc);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void SampleListView::SetEditable(BOOL isEditable)
{
    if ( isEditable )
    {
        DWORD dwStyle = (DWORD)GetWindowLong(m_hWndList, GWL_STYLE) | LVS_EDITLABELS;
        SetWindowLong(m_hWndList, GWL_STYLE, dwStyle);
    }
    else
    {
        DWORD dwStyle = (DWORD)GetWindowLong(m_hWndList, GWL_STYLE) & (~LVS_EDITLABELS);
        SetWindowLong(m_hWndList, GWL_STYLE, dwStyle);
    }

    m_isEditable = isEditable;
}

//////////////////////////////////////////////////////////////////////////

void SampleListView::GetParamsInfo(FUNCTIONINFO *pFunInfo)
{
    TCHAR szText[MAX_PATH] = { 0 };
    int nItemCount = ListView_GetItemCount(m_hWndList);

    for (int i = 0; i < nItemCount; ++i)
    {
        ListView_GetItemText(m_hWndList, i, EDIT_SUBITEM, szText, MAX_PATH);
        pFunInfo->szParams[i].strValue = szText;
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleListView::UpdateRetValues(FUNCTIONINFO *pFunInfo)
{
    int nItemCount = ListView_GetItemCount(m_hWndList);
    TCHAR szText[MAX_PATH] = { 0 };
    for (int i = 0; i < nItemCount && i < pFunInfo->nNumParam; ++i)
    {
        LPCTSTR lpText = pFunInfo->szParams[i].strValue.c_str();
        _tcscpy(szText, lpText);
        ListView_SetItemText(
            m_hWndList,
            i,
            EDIT_SUBITEM,
            szText);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleListView::OnInsertParams(HWND hWndList, FUNCTIONINFO *pFunInfo)
{
    ListView_DeleteAllItems(hWndList);

    for (int i = 0; i < pFunInfo->nNumParam; ++i)
    {
        PARAMINFO &paramInfo = pFunInfo->szParams[i];

        LV_ITEM lvi;
        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;
        lvi.iSubItem = 0;   // Param name.
        lvi.pszText = paramInfo.strName;
        ListView_InsertItem(hWndList, &lvi);

        // Param type.
        ListView_SetItemText(hWndList, i, 1, ParamTypeToString(paramInfo.nType));

        TCHAR szText[MAX_PATH] = { 0 };
        _tcscpy(szText, paramInfo.strValue.c_str());

        // Param value
        ListView_SetItemText(hWndList, i, 2, szText);
    }

    ListView_EditLabel(hWndList, 3);
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleListView::OnListViewNotify(WPARAM wParam, LPARAM lParam)
{
    NMHDR *nmhdr = (NMHDR*)lParam;
    HWND hWndList = nmhdr->hwndFrom;
    static HWND hWndEdit = NULL;

    switch (nmhdr->code)
    {
    case NM_DBLCLK:
    case NM_CLICK:
        {
             LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;

             if (lpnmitem->iItem != -1 && lpnmitem->iSubItem != -1)
             {
                 m_nEditItem    = lpnmitem->iItem;
                 m_nEditSubItem = lpnmitem->iSubItem;
             }
        }
        break;

    case LVN_BEGINLABELEDIT:    // Label edit begin
        {
            RECT rc = { 0 };

            if ( m_nEditSubItem == EDIT_SUBITEM && m_isEditable )
            {
                m_isEditing = TRUE;
                hWndEdit = ListView_GetEditControl(hWndList);

                // Get the rect of item and block its update.
                ListView_GetItemRect(hWndList, m_nEditItem, &rc, LVIR_LABEL);
                ValidateRect(hWndList, &rc);

                // Get the rect of sub item being edited.
                ListView_GetSubItemRect(
                    hWndList,
                    m_nEditItem,
                    m_nEditSubItem,
                    LVIR_LABEL,
                    &rc);

                SetWindowLong(hWndEdit, GWL_USERDATA, (LONG)this);
                // Subclass the edit control.
                m_wpOrigEditProc = (WNDPROC)SetWindowLong(
                    hWndEdit, 
                    GWL_WNDPROC,
                    (LONG)SampleListView::EditSubclassProc);

                // Get the text of editing item.
                TCHAR szText[MAX_PATH] = { 0 };

                ListView_GetItemText(
                    hWndList,
                    m_nEditItem,
                    m_nEditSubItem,
                    szText,
                    MAX_PATH);

                m_rcEditSubItem = rc;
                m_rcEditSubItem.right = ListView_GetColumnWidth(hWndList, m_nEditSubItem);

                SetWindowText(hWndEdit, szText);
            }

            // Finally return FALSE to allow item editing.
            return FALSE;
        }
        break;

    case LVN_ENDLABELEDIT:
        {
            NMLVDISPINFO *lvdisp = (NMLVDISPINFO*)lParam;
            LVITEM *lvItem = &lvdisp->item;
            size_t dwLen = 0; // strlen of inserted text
            StringCbLength(lvItem->pszText, 1024, &dwLen);

            if ( m_nEditSubItem == EDIT_SUBITEM && m_isEditable )
            {
                if ( NULL != lvItem->pszText )
                {
                    if ( dwLen > 0 )
                    {
                        ListView_SetItemText(
                            hWndList,
                            m_nEditItem,
                            m_nEditSubItem,
                            lvItem->pszText);
                    }
                }

                SetWindowLong(hWndEdit, GWL_WNDPROC, (LONG)m_wpOrigEditProc);

                m_nEditSubItem = 0;
                m_isEditing = FALSE;
                return FALSE; // Return FALSE to avoid text change on first item.
            }

            return TRUE; // Return TRUE to indicate that the text can be set
        }
        break;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SampleListView::OnDestroy(HWND hWndList)
{
    SetWindowLong(hWndList, GWL_USERDATA, (LONG)0);
    SetWindowLong(hWndList, GWL_WNDPROC, (LONG)m_wpOrigListViewProc);
}

//////////////////////////////////////////////////////////////////////////

LRESULT APIENTRY SampleListView::EditSubclassProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    SampleListView *pListView = (SampleListView*)(LPVOID)GetWindowLong(hwnd, GWL_USERDATA);

    switch (uMsg)
    {
    case WM_WINDOWPOSCHANGING:
        {
            if ( NULL != pListView )
            {
                WINDOWPOS *pos = (WINDOWPOS*)lParam;

                pos->x  = pListView->m_rcEditSubItem.left;
                pos->cx = pListView->m_rcEditSubItem.right;
            }
        }
        break;

    default:
        if ( NULL != pListView )
        {
            return CallWindowProc(
                pListView->m_wpOrigEditProc,
                hwnd,
                uMsg,
                wParam,
                lParam);
        }
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

LRESULT APIENTRY SampleListView::ListViewSubclassProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    SampleListView *pListView = (SampleListView*)(LPVOID)GetWindowLong(hwnd, GWL_USERDATA);

    switch (uMsg)
    {
    case WM_PAINT:  // Trap WM_PAINT for block update of first item on subitem editing.
        {
            RECT rcItem = { 0 };
            BOOL isOK = FALSE;

            if ( !pListView->m_isEditing )
            {
                break;
            }

            if ( pListView->m_nEditSubItem > 0 )
            {
                isOK = ListView_GetItemRect(hwnd, pListView->m_nEditItem, &rcItem, LVIR_LABEL);
                if ( isOK )
                {
                    ValidateRect(hwnd, &rcItem);
                }
            }
        }
        break;
    }

    return CallWindowProc(
        pListView->m_wpOrigListViewProc,
        hwnd,
        uMsg,
        wParam,
        lParam);
}
