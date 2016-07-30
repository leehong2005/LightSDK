
#include "stdafx.h"
#include "SampleScrollBitmap.h"
#include "SdkUICommonInclude.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    SdkUIRunTime::InitializeUIRunTime();

    {
        SampleScrollBitmapWin win;
        if ( win.CreateMyWindow(hInstance, NULL) )
        {
            MSG msg;
            while ( GetMessage(&msg, NULL, 0, 0) )
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    SdkUIRunTime::UninitializeUIRunTime();

    return 0;
}