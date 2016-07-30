// TestButtonCtrl.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestButtonCtrl.h"
#include "CommonInclude.h"
#include "FrameworkInclude.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    MSG msg = { 0 };

    CoInitialize(NULL);

    {
        WindowForm winForm;
        if ( winForm.CreateWindowForm(hInstance, NULL) )
        {
            while (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    CoUninitialize();

    return (int) msg.wParam;
}
