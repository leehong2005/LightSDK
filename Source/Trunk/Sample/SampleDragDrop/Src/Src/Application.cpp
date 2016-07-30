
#include "stdafx.h"

#include "SampleDragDrop.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    OleInitialize(NULL);

    {
        SampleDragDropWin win;
        win.DoModal(hInstance, NULL);
    }

    OleUninitialize();

    return 0;
}