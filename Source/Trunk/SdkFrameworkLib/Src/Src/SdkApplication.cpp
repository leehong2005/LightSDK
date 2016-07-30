/*!
* @file SdkApplication.cpp
* 
* @brief This file defines functions to run an application, wrap message loop.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/07/19
*/

#include "StdAfx.h"
#include "SdkApplication.h"
#include "SdkWindowForm.h"
#include "SdkUIRunTime.h"

USING_NAMESPACE_UILIB

SdkApplication::SdkApplication() : m_hInstance(NULL),
                                   m_nCmdShow(SW_SHOW)
{
    // Initialize the UI runtime, such as COM, WIC, Resource library, etc.
    SdkUIRunTime::InitializeUIRunTime();
}

//////////////////////////////////////////////////////////////////////////

SdkApplication::SdkApplication(HINSTANCE hInst, int nCmdShow)
{
    m_hInstance = hInst;
    m_nCmdShow  = nCmdShow;

    // Initialize the UI runtime, such as COM, WIC, Resource library, etc.
    SdkUIRunTime::InitializeUIRunTime();
}

//////////////////////////////////////////////////////////////////////////

SdkApplication::~SdkApplication()
{
    SdkUIRunTime::UninitializeUIRunTime();
}

//////////////////////////////////////////////////////////////////////////

int SdkApplication::Run()
{
    MSG msg;
    while ( GetMessage(&msg, NULL, 0, 0) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

//////////////////////////////////////////////////////////////////////////

int SdkApplication::Run(SdkWindowForm *pWindowForm)
{
    if ( NULL == pWindowForm )
    {
        return -1;
    }

    if ( pWindowForm->CreateWindowForm(m_hInstance, NULL, m_nCmdShow) )
    {
        MSG msg;
        while ( GetMessage(&msg, NULL, 0, 0) )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}
