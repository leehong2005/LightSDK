
#include "stdafx.h"
#include "SamplePreviewHandler.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(nCmdShow);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(hPrevInstance);

    CoInitialize(NULL);

    {
        SamplePreviewHandler app;
        if (SUCCEEDED(app.Initialize(hInstance)))
        {
            app.RunMessageLoop();
        }
    }

    CoUninitialize();

    return 0;
}