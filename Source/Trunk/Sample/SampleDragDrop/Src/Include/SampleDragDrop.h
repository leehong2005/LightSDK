#pragma once

#include "resource.h"
#include "SdkCommonInclude.h"
#include "SampleDragDropHelper.h"

class SampleDragDropWin : public SdkDropTarget
{
public:

    SampleDragDropWin();
    ~SampleDragDropWin();

    INT_PTR DoModal(HINSTANCE hInst, HWND hWndParent);

    virtual DROPEFFECT OnDragEnter(HWND hWnd, IDataObject *pDataObj, DWORD dwKeyState, POINT pt);
    virtual DROPEFFECT OnDragOver(HWND hWnd, DWORD dwKeyState, POINT pt);
    virtual void OnDragLeave(HWND hWnd);
    virtual BOOL OnDrop(HWND hWnd, IDataObject *pDataObj, DWORD dwKeyState, POINT pt);

private:

    HRESULT GetDataObject(HWND hwndDragBegin, IDataObject **ppdtobj);
    DWORD GetDropEffects();

    void OnOpen();
    void OnBindUI();
    void OnBeginDrag(HWND hwndDragBegin);
    void OnInitDlg(HWND hwndDlg);
    INT_PTR OnDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

    IShellItemArray *m_psiaDrop;
    TCHAR m_szDispName[MAX_PATH];
    HWND m_hWnd;
    HINSTANCE m_hInst;
    DROPEFFECT m_curDropEffect;
    IDataObject *m_pDataObject;
};