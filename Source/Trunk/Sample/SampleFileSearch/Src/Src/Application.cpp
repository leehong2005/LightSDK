
#include "stdafx.h"
#include "SampleFileSearch.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitialize(NULL)))
    {
        {
            SampleFileSearch app;
            if (SUCCEEDED(app.Initialize(hInstance)))
            {
                app.RunMessageLoop();
            }
        }
        CoUninitialize();
    }

    return 0;
}