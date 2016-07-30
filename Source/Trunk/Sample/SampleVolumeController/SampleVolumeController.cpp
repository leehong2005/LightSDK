// Epvolume.cpp -- WinMain and dialog box functions

#include "stdafx.h"
#include <commctrl.h>
#include "resource.h"

#include "SdkCommonInclude.h"
#pragma comment(lib, "comctl32.lib")

#define MAX_VOL 100

HWND  g_hDlg = NULL;
HINSTANCE  g_hInst = NULL;
SdkAudioVolumeController *pAudioVolumeController = NULL;

static BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

class MyAudioVolumeChangeHandler : public IAudioVolumeNotify
{
public:

    void OnAudioVolumeChange(SdkAudioVolumeController *pVolumeController)
    {
        PostMessage(GetDlgItem(g_hDlg, IDC_CHECK_MUTE), BM_SETCHECK,
            (pVolumeController->IsMute()) ? BST_CHECKED : BST_UNCHECKED, 0);

        PostMessage(GetDlgItem(g_hDlg, IDC_SLIDER_VOLUME),
            TBM_SETPOS, TRUE,
            LPARAM((UINT32)(pVolumeController->GetVolume())));
    }
};


//-----------------------------------------------------------
// WinMain -- Registers an IAudioEndpointVolumeCallback
//   interface to monitor endpoint volume level, and opens
//   a dialog box that displays a volume control that will
//   mirror the endpoint volume control in SndVol.
//-----------------------------------------------------------
int APIENTRY WinMain(HINSTANCE  hInstance,
                     HINSTANCE  hPrevInstance,
                     LPSTR  lpCmdLine,
                     int  nCmdShow)
{
    HRESULT  hr = S_OK;

    hr = CoInitialize(NULL);

    LANGID nLANGID = 2025;
    SdkCommonHelper::InitCurrentLanguage(nLANGID);

    MyAudioVolumeChangeHandler myVolumeChangeHandler;
    pAudioVolumeController = new SdkAudioVolumeController();
    pAudioVolumeController->SetOnVolumeChangeHandler(&myVolumeChangeHandler);

    g_hInst = hInstance;
    InitCommonControls();
    DialogBox(hInstance, TEXT("VOLUMECONTROL"), NULL, (DLGPROC)DlgProc);

    delete pAudioVolumeController;
    pAudioVolumeController = NULL;

    CoUninitialize();
    return 0;
}

//-----------------------------------------------------------
// DlgProc -- Dialog box procedure
//-----------------------------------------------------------

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    BOOL  bMute;
    int  nVolume;
    int  nChecked;

    switch (message)
    {
    case WM_INITDIALOG:
        {
            g_hDlg = hDlg;
            {
                WCHAR szBuf[100] = { 0 };
                LoadString(g_hInst, IDS_APP_TITLE, szBuf, 100);
                SetWindowText(g_hDlg, szBuf);
                HICON hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_SMALL));
                SendMessage(g_hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
                DestroyIcon(hIcon);
            }
            SendDlgItemMessage(hDlg, IDC_SLIDER_VOLUME, TBM_SETRANGEMIN, FALSE, 0);
            SendDlgItemMessage(hDlg, IDC_SLIDER_VOLUME, TBM_SETRANGEMAX, FALSE, MAX_VOL);
            bMute = pAudioVolumeController->IsMute();
            SendDlgItemMessage(hDlg, IDC_CHECK_MUTE, BM_SETCHECK,
            bMute ? BST_CHECKED : BST_UNCHECKED, 0);
            nVolume = (INT32)pAudioVolumeController->GetVolume();
            SendDlgItemMessage(hDlg, IDC_SLIDER_VOLUME, TBM_SETPOS, TRUE, nVolume);

            SdkCommonHelper::SetWindowCenter(hDlg);

            TCHAR szText[50] = { 0 };
            _stprintf_s(szText, _T("%d"), nVolume);
        }
        return TRUE;

    case WM_HSCROLL:
        switch (LOWORD(wParam))
        {
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
        case SB_LINERIGHT:
        case SB_LINELEFT:
        case SB_PAGERIGHT:
        case SB_PAGELEFT:
        case SB_RIGHT:
        case SB_LEFT:
            // User moved volume slider in dialog box.
            SendDlgItemMessage(hDlg, IDC_CHECK_MUTE, BM_SETCHECK, BST_UNCHECKED, 0);
            pAudioVolumeController->SetMute(FALSE);
            nVolume = SendDlgItemMessage(hDlg, IDC_SLIDER_VOLUME, TBM_GETPOS, 0, 0);
            pAudioVolumeController->SetVolume((FLOAT)nVolume);

            return TRUE;
        }
        break;

    case WM_COMMAND:
        switch ((int)LOWORD(wParam))
        {
        case IDC_CHECK_MUTE:
            // The user selected the Mute check box in the dialog box.
            nChecked = SendDlgItemMessage(hDlg, IDC_CHECK_MUTE, BM_GETCHECK, 0, 0);
            bMute = (BST_CHECKED == nChecked);
            pAudioVolumeController->SetMute(bMute);
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, TRUE);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

