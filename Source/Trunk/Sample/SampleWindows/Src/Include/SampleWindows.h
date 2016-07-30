#pragma once

#include "resource.h"
#include "SampleTabPages.h"
#include "SdkUICommonInclude.h"
#include "SampleFunWindow.h"

class SampleWindows : public SdkWindowForm,
                      public ITabViewEventHandler
{
public:

    SampleWindows();
    virtual ~SampleWindows();

    void SetFunWindow(SampleFunWindow *pFunWin);
    SampleTabPages* GetCurTabPage();

protected:

    virtual void OnCreated();
    virtual void OnCreating(LPCREATESTRUCT lpCreateStruct);
    virtual void OnPreCreate(LPCREATEWINDOWPARAMS lpCreateWinParams);
    virtual BOOL OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    virtual void OnSelectedChanged(SdkTabView *pTabView, int nOldIndex, int nCurIndex);
    virtual void OnInsertTab(SdkTabView *pTabView);
    virtual void OnDeleteTab(SdkTabView *pTabView, int nDelIndex);

private:

    SampleFunWindow         *m_pFunWin;
    SdkTabView              *m_pTabView;
    vector<SampleTabPages*>  m_vctTabPages;
};
