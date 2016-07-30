#include "StdAfx.h"
#include "SampleFunWindow.h"
#include "SdkUICommonInclude.h"
#include "SampleTabPages.h"
#include "SampleWindows.h"


SampleFunWindow::SampleFunWindow(void)
{
    m_hWnd          = NULL;
    m_hWndList      = NULL;
    m_pSampleWin    = NULL;
    m_pListView     = new SampleListView();
}

//////////////////////////////////////////////////////////////////////////

SampleFunWindow::~SampleFunWindow(void)
{
    SAFE_DELETE(m_pListView);
}

//////////////////////////////////////////////////////////////////////////

BOOL SampleFunWindow::InitializeWindow(HINSTANCE hInst, HWND hWndParent)
{
    HWND hWnd = CreateDialogParam(
        hInst,
        MAKEINTRESOURCE(IDD_DLG_FUNS),
        hWndParent,
        SampleFunWindow::DialogProc,
        (LPARAM)this);

    m_hWnd = hWnd;

    ShowWindow(hWnd, SW_SHOW);

    return (NULL != hWnd);
}

//////////////////////////////////////////////////////////////////////////

void SampleFunWindow::SetWindowSize(int width, int height)
{
    ::SetWindowPos(m_hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////

void SampleFunWindow::SetWindowPos(int left, int top)
{
    ::SetWindowPos(m_hWnd, NULL, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////

void SampleFunWindow::SetCurCtrlName(LPCTSTR lpCtrlName)
{
    HWND hWndCheck = GetDlgItem(m_hWnd, IDC_CHECK_DISABLECTRL);
    if ( NULL != hWndCheck )
    {
        LPCTSTR lpText = (NULL == lpCtrlName) ? _T("") : lpCtrlName;
        wstring str(_T("Disable "));
        str.append(lpText);

        CheckDlgButton(m_hWnd, IDC_CHECK_DISABLECTRL, BST_UNCHECKED);
        SetWindowText(hWndCheck, str.c_str());
        EnableTagPageCtrl();
        LoadFunsAndParams();
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleFunWindow::SetTabPageWindow( SampleWindows *pSampleWin )
{
    m_pSampleWin = pSampleWin;
}

//////////////////////////////////////////////////////////////////////////

void SampleFunWindow::EnableTagPageCtrl()
{
    BOOL isChecked = IsDlgButtonChecked(m_hWnd, IDC_CHECK_DISABLECTRL);
    if ( NULL != m_pSampleWin )
    {
        SampleTabPages *pTabPage = m_pSampleWin->GetCurTabPage();
        if ( NULL != pTabPage )
        {
            SdkViewElement *pView = pTabPage->GetView();
            if ( NULL != pView )
            {
                pView->SetEnable(!isChecked);
                pView->Invalidate();
                m_pSampleWin->Invalidate();
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleFunWindow::LoadFunsAndParams()
{
    if ( NULL != m_pSampleWin )
    {
        SampleTabPages *pTabPage = m_pSampleWin->GetCurTabPage();
        if ( NULL != pTabPage )
        {
            vector<FUNCTIONINFO*> vctFuns;
            pTabPage->GetFunAndParams(vctFuns);

            HWND hComboBox = GetDlgItem(m_hWnd, IDC_COMBO_FUNS);
            SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);

            for (int i = 0; i < (int)vctFuns.size(); ++i)
            {
                FUNCTIONINFO *funInfo = vctFuns[i];
                SendMessage(hComboBox, CB_ADDSTRING,   (WPARAM)0, (LPARAM)funInfo->strFunName);
                SendMessage(hComboBox, CB_SETITEMDATA, (WPARAM)i, (LPARAM)(DWORD)funInfo);
            }

            int nSelIndex = 0;
            SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)nSelIndex, 0);
            OnComboBoxSelChanged(nSelIndex);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleFunWindow::OnBtnApplyClick(HWND hWndCtl)
{
    HWND hWndCombo = GetDlgItem(m_hWnd, IDC_COMBO_FUNS);
    int nCurIndex = SendMessage(hWndCombo, CB_GETCURSEL, 0, 0);
    LRESULT val = SendMessage(hWndCombo, CB_GETITEMDATA, (WPARAM)nCurIndex, 0);
    if ( CB_ERR != val )
    {
        FUNCTIONINFO *pFunInfo = static_cast<FUNCTIONINFO*>((LPVOID)val);
        //FUNCTIONINFO funInfo = *pFunInfo;
        m_pListView->GetParamsInfo(pFunInfo);

        SampleTabPages *pTabPage = m_pSampleWin->GetCurTabPage();
        if ( NULL != pTabPage )
        {
            pTabPage->PerformFuns(pFunInfo);
            m_pSampleWin->Invalidate();
        }

        // Set return value to list view.
        m_pListView->UpdateRetValues(pFunInfo);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleFunWindow::OnCreate(HWND hWndDlg)
{
    m_hWndList = GetDlgItem(hWndDlg, IDC_LIST_FUNS);
    m_pListView->AddListViewColumns(m_hWndList);
}

//////////////////////////////////////////////////////////////////////////

void SampleFunWindow::OnComboBoxSelChanged(int nCurIndex)
{
    HWND hWndList = GetDlgItem(m_hWnd, IDC_LIST_FUNS);
    ListView_DeleteAllItems(hWndList);

    HWND hWndCombo = GetDlgItem(m_hWnd, IDC_COMBO_FUNS);
    LRESULT val = SendMessage(hWndCombo, CB_GETITEMDATA, (WPARAM)nCurIndex, 0);
    if ( CB_ERR != val )
    {
        FUNCTIONINFO *pFunInfo = static_cast<FUNCTIONINFO*>((LPVOID)val);
        if ( NULL != pFunInfo )
        {
            m_pListView->SetEditable(pFunInfo->isSet);
            m_pListView->OnInsertParams(hWndList, pFunInfo);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleFunWindow::OnCommand(HWND hWnd, UINT id, HWND hWndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDC_CHECK_DISABLECTRL:
        {
            EnableTagPageCtrl();
        }
        break;

    case IDC_COMBO_FUNS:
        {
            if ( CBN_SELCHANGE == codeNotify )
            {
                int nSelIndex = SendMessage(hWndCtl, CB_GETCURSEL, 0, 0);
                OnComboBoxSelChanged(nSelIndex);
            }
        }
        break;

    case IDC_BTN_APPLY:
        {
            OnBtnApplyClick(hWndCtl);
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

INT_PTR SampleFunWindow::OnDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_COMMAND:
        OnCommand(hwndDlg, (UINT)LOWORD(wParam), (HWND)lParam, (UINT)HIWORD(wParam));
        break;

    case WM_CLOSE:
        m_pListView->OnDestroy(GetDlgItem(hwndDlg, IDC_LIST_FUNS));
        DestroyWindow(hwndDlg);
        break;

    case WM_SIZE:
        {
            int width  = LOWORD(lParam);
            int height = HIWORD(lParam);
            int margin = 11;
            int top    = margin;
            HWND hWndList = GetDlgItem(hwndDlg, IDC_LIST_FUNS);
            HWND hWndBox = GetDlgItem(hwndDlg, IDC_STATIC_GROUPBOX);
            MoveWindow(hWndBox,  margin, top, width - margin * 2, 70, TRUE);
            top = 90;
            MoveWindow(hWndList, margin, top, width - margin * 2, height - top - margin, TRUE);
        }
        break;

    case WM_NOTIFY:
        {
            LPNMHDR lpnmHdr = (LPNMHDR)lParam;
            switch(lpnmHdr->idFrom)
            {
            case IDC_LIST_FUNS:
                {
                    BOOL retVal = m_pListView->OnListViewNotify(wParam, lParam);
                    return retVal;
                }
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

INT_PTR CALLBACK SampleFunWindow::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (WM_INITDIALOG == uMsg)
    {
        SampleFunWindow* pThis = static_cast<SampleFunWindow*>((LPVOID)(LONG)lParam);
        if (NULL != pThis)
        {
            SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG)pThis);
            pThis->OnCreate(hwndDlg);
        }

        return FALSE;
    }

    SampleFunWindow* pThis = static_cast<SampleFunWindow*>(
        (LPVOID)GetWindowLongPtr(hwndDlg, GWLP_USERDATA));

    if (NULL != pThis)
    {
        return pThis->OnDialogProc(hwndDlg, uMsg, wParam, lParam);
    }

    return FALSE;
}
