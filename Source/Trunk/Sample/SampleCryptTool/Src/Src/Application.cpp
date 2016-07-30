
#include "stdafx.h"
#include "SampleCryptTool.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    CoInitialize(NULL);

    {
        SdkCryptToolWindow toolWin;
        if (toolWin.CreateToolWindow(hInstance, NULL))
        {
            MSG msg;
            while (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    CoUninitialize();

    return 0;
}