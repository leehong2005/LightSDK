#pragma once

#include "SampleDef.h"
#include "SdkUICommonInclude.h"

class SampleListView
{
public:

    SampleListView();
    ~SampleListView();

    HWND WINAPI CreateListView(HWND hWndParent,UINT uId);
    BOOL AddListViewColumns(HWND hWndList);
    void SetEditable(BOOL isEditable);
    void GetParamsInfo(FUNCTIONINFO *pFunInfo);
    void UpdateRetValues(FUNCTIONINFO *pFunInfo);
    void OnInsertParams(HWND hWndList, FUNCTIONINFO *pFunInfo);
    BOOL OnListViewNotify(WPARAM wParam, LPARAM lParam);
    void OnDestroy(HWND hWndList);

private:

    static LRESULT APIENTRY EditSubclassProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
    static LRESULT APIENTRY ListViewSubclassProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

private:

    int          m_nEditItem;
    int          m_nEditSubItem;
    BOOL         m_isEditing;
    BOOL         m_isEditable;
    RECT         m_rcEditSubItem;
    HWND         m_hWndList;
    WNDPROC      m_wpOrigEditProc;        // Original Edit Window procedure.
    WNDPROC      m_wpOrigListViewProc;    // Original Edit Window procedure.
};
