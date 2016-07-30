// TestButtonCtrl.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestButtonCtrl.h"
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
    SdkUIRunTime::InitializeUIRunTime();
    SdkApplication app(hInstance, nCmdShow);

    // The GIF view.
    SdkGifView *gifView = new SdkGifView();
    gifView->LoadFromFile(L"D:\\loading.gif");
    gifView->SetViewPos(100, 100);
    gifView->AutoAdjustSize(TRUE);
    gifView->SetBorderWidth(2.0f);
    gifView->SetBorderColor(ColorF::Red);
    gifView->SetBkColor(ColorF::Blue);

    // The Window.
    SdkWindowForm winForm;
    winForm.AddView(gifView);
    winForm.CreateWindowForm(hInstance, NULL);

    //winForm.SetBkColor(RGB(255, 0, 0));
    SdkCommonHelper::SetWindowCenter(winForm.GetHwnd());
    app.Run();

    SdkUIRunTime::UninitializeUIRunTime();

    return (int)0;
}
