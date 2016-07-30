// TestWindowForm.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestWindowForm.h"
#include "SdkCommonInclude.h"
#include "SdkUICommonInclude.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    MSG msg = { 0 };

    SdkUIRunTime::InitializeUIRunTime();

    {
        SdkWindowForm winForm;
        if ( winForm.CreateWindowForm(hInstance, NULL) )
        {
            winForm.SetWindowSize(1300, 800);
            winForm.CenterWindow(TRUE);

            //SdkWindowDialog dlg;
            //dlg.DoModal((HWND)winForm);

            while (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    SdkUIRunTime::UninitializeUIRunTime();

    return (int) msg.wParam;
}
