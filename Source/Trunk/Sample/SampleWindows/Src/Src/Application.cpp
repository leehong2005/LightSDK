
#include "stdafx.h"
#include "SampleMainWindow.h"
#include "SampleWindows.h"

#include <shellapi.h>
#pragma comment(lib, "shell32.lib")

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    SdkApplication app(hInstance, nCmdShow);

    {
        //SdkMessageBox box;
        //box.Show(NULL, L"SdkMessageBox", L"SdkMessageBox");

        //SampleWindows win;
        //if ( win.CreateWindowForm(hInstance, NULL) )
        SampleMainWindow win;
        if ( win.InitializeWindow(hInstance, NULL) )
        {
            app.Run();
        }
    }

    return 0;
}