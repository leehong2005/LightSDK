// SamplePreviewHandler.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SampleWindows.h"

CONTROLTYPE szCtrlTypes[] = 
{
    TYPE_BUTTON         ,
    TYPE_CHECKBOX       ,
    TYPE_COMBOBOX       ,
    TYPE_RADIOBUTTON    ,
    TYPE_LISTBOX        ,
    TYPE_EDITBOX        ,
    TYPE_SEEKBAR        ,
    TYPE_PROGRESS       ,
    TYPE_TEXTVIEW       ,
    TYPE_RATINGVIEW     ,
    TYPE_TABHEADER      ,
    TYPE_TABVIEW        ,
    TYPE_GROUPBOX       ,
    TYPE_IMAGEVIEW      ,
    TYPE_OVERLAP        ,
    TYPE_GIFVIEW        ,
    TYPE_SCROLLBAR      ,
};


SampleWindows::SampleWindows()
{
    // Do not delete, it is added to window, which will manage its memory.
    m_pTabView  = NULL;
    m_pFunWin   = NULL;
}

//////////////////////////////////////////////////////////////////////////

SampleWindows::~SampleWindows()
{
    int nSize = (int)m_vctTabPages.size();

    for (int i = 0; i < nSize; ++i)
    {
        SAFE_DELETE(m_vctTabPages[i]);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleWindows::SetFunWindow(SampleFunWindow *pFunWin)
{
    m_pFunWin = pFunWin;
}

//////////////////////////////////////////////////////////////////////////

SampleTabPages* SampleWindows::GetCurTabPage()
{
    if ( NULL != m_pTabView )
    {
        int nSelIndex = m_pTabView->GetSelTab();
        int nSize = (int)m_vctTabPages.size();
        if ( nSelIndex >= 0 && nSelIndex < nSize )
        {
            return m_vctTabPages[nSelIndex];
        }
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////

/** 
* @brief SampleWindows::OnCreating 
* 
* Detailed description.
* @param[in] lpCreateStruct 
*/
void SampleWindows::OnCreating( LPCREATESTRUCT lpCreateStruct )
{
    //CenterWindow(FALSE);
    //HICON hIcon = LoadIcon(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDI_SAMPLEWINDOWS));
    //SendMessage(m_hWnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
    //SAFE_DELETE_ICON(hIcon);
    //SdkWindowForm::SetRoundCornerRadius(10, 10);
    //SdkWindowForm::SetBkColor(RGB(255, 128, 128));
}

//////////////////////////////////////////////////////////////////////////

/**
* @class OnPreCreate
* @brief 
* 
* Detailed description.
*/
void SampleWindows::OnPreCreate( LPCREATEWINDOWPARAMS lpCreateWinParams )
{
    lpCreateWinParams->x = 0;
    lpCreateWinParams->y = 0;
    lpCreateWinParams->cx = 1000;
    lpCreateWinParams->cy = 570;

    lpCreateWinParams->dwStyle = WS_CHILD;
}

//////////////////////////////////////////////////////////////////////////

/**
* @class OnCreated
* @brief 
* 
* Detailed description.
*/
void SampleWindows::OnCreated()
{
    SdkWindowForm::OnCreated();

    m_pTabView = new SdkTabView();
    m_pTabView->SetViewPos(10, 10);
    m_pTabView->SetTabWidth(152);
    m_pTabView->SetViewSize(990, 550);
    m_pTabView->SetTabViewEventHandler(this);
    //m_pTabView->SetDockDir(TAB_DOCK_RIGHT);

    int nCtrlNum = ARRAYSIZE(szCtrlTypes);
    for (int i = 0; i < nCtrlNum; ++i)
    {
        SampleTabPages *pTabPages = new SampleTabPages(szCtrlTypes[i], m_pTabView);
        m_vctTabPages.push_back(pTabPages);
    }

    int nTabPages = (int)m_vctTabPages.size();
    for (int i = 0; i < nTabPages; ++i)
    {
        SampleTabPages *pTabPage = m_vctTabPages[i];

        if ( 0 == i )
        {
            m_pTabView->SetTabText(i, pTabPage->GetCtrlName());
        }
        else
        {
            m_pTabView->InsertTab(m_pTabView->GetTabCount(), pTabPage->GetCtrlName());
        }

        SdkViewElement *pTabPageView = pTabPage->GetView();
        m_pTabView->AddTabView(i, pTabPageView);
    }

    this->AddView(m_pTabView);

    if ( NULL != m_pFunWin )
    {
        TCHAR szBuffer[MAX_PATH] = { 0 };
        m_pTabView->GetTabText(m_pTabView->GetSelTab(), szBuffer);
        m_pFunWin->SetCurCtrlName(szBuffer);
    }}

//////////////////////////////////////////////////////////////////////////

BOOL SampleWindows::OnWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    return SdkWindowForm::OnWndProc(hWnd, message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

void SampleWindows::OnSelectedChanged( SdkTabView *pTabView, int nOldIndex, int nCurIndex )
{
    if ( NULL != m_pFunWin )
    {
        TCHAR szBuffer[MAX_PATH] = { 0 };
        pTabView->GetTabText(nCurIndex, szBuffer);
        m_pFunWin->SetCurCtrlName(szBuffer);
    }
}

//////////////////////////////////////////////////////////////////////////

void SampleWindows::OnInsertTab( SdkTabView *pTabView )
{
}

//////////////////////////////////////////////////////////////////////////

void SampleWindows::OnDeleteTab( SdkTabView *pTabView, int nDelIndex )
{
}
