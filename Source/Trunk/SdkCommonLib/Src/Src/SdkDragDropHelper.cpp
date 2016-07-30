/*!
* @file SdkDragDropHelper.cpp
* 
* @brief The file define the class.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#include "stdafx.h"
#include "SdkDragDropHelper.h"

USING_NAMESPACE_COMMON

//////////////////////////////////////////////////////////////////////////

CLIPFORMAT SdkDragDropHelper::RegisterClipFormat(IN LPCTSTR lpszFormat)
{
    return (CLIPFORMAT)RegisterClipboardFormat(lpszFormat);
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkDragDropHelper::CheckForBeginDrag(IN HWND hWndDrag, IN int x, IN int y)
{
    int const cxDrag = GetSystemMetrics(SM_CXDRAG);
    int const cyDrag = GetSystemMetrics(SM_CYDRAG);

    // See if the user moves a certain number of pixels in any direction.
    RECT rcDragRadius = { 0 };
    ::SetRect(&rcDragRadius, x - cxDrag, y - cyDrag, x + cxDrag, y + cyDrag);

    // Client -> Screen
    ::MapWindowPoints(hWndDrag, NULL, (POINT*)&rcDragRadius, 2);
    ::SetCapture(hWndDrag);

    do 
    {
        // Sleep the thread waiting for mouse input, Prevents pegging the CPU in 
        // a PeekMessage loop.
        MSG msg = { 0 };
        DWORD dwWaitCode = MsgWaitForMultipleObjectsEx(
            0,
            NULL,
            INFINITE,
            QS_MOUSE,
            MWMO_INPUTAVAILABLE);

        switch (dwWaitCode)
        {
        case WAIT_OBJECT_0:
            {
                if ( PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE) )
                {
                    // See if the application wants to process the message...
                    if ( 0 == CallMsgFilter(&msg, MSGF_COMMCTRL_BEGINDRAG) )
                    {
                        switch (msg.message)
                        {
                        case WM_LBUTTONUP:
                        case WM_RBUTTONUP:
                        case WM_LBUTTONDOWN:
                        case WM_RBUTTONDOWN:
                            // Released the mouse without moving outside the drag radius
                            // not beginning a drag.
                            ReleaseCapture();
                            return FALSE;

                        case WM_MOUSEMOVE:
                            if ( !PtInRect(&rcDragRadius, msg.pt) )
                            {
                                // Moved outside the drag radius, beginning a drag.
                                ReleaseCapture();
                                return TRUE;
                            }
                            break;

                        default:
                            TranslateMessage(&msg);
                            DispatchMessage(&msg);
                            break;
                        }
                    }
                }
            }
            break;
        }

    } while (::GetCapture() == hWndDrag);

    return FALSE;
}