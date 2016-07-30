
#include "stdafx.h"
#include "SampleKnownFolder.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    CoInitialize(NULL);
    SdkCommonRunTime::InitializeRunTime();

    {
        SampleKnownFolderWin folderWin;
        folderWin.CreateSampleKnownFolderWin(hInstance, NULL);
    }

    SdkCommonRunTime::UninitializeRunTime();
    CoUninitialize();
    return 0;
}