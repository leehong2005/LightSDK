/*!
* @file SdkDragSourceHelper.cpp
* 
* @brief The file define the class.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#include "stdafx.h"
#include "SdkDragSourceHelper.h"

USING_NAMESPACE_COMMON

SdkDragSourceHelper::SdkDragSourceHelper() : m_pDragSourceHelper2(NULL)
{
}

//////////////////////////////////////////////////////////////////////////

SdkDragSourceHelper::~SdkDragSourceHelper()
{
    SAFE_RELEASE(m_pDragSourceHelper2);
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkDragSourceHelper::SetDropDescEnable()
{
    HRESULT hr = S_OK;

    if ( NULL == m_pDragSourceHelper2 )
    {
        hr = CoCreateInstance(
            CLSID_DragDropHelper,
            NULL, CLSCTX_INPROC,
            IID_PPV_ARGS(&m_pDragSourceHelper2));
    }

    if ( SUCCEEDED(hr) )
    {
        hr = m_pDragSourceHelper2->SetFlags(DSH_ALLOWDROPDESCRIPTIONTEXT);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkDragSourceHelper::InitializeFromBitmap(HWND hWnd, HBITMAP hBitmap, IDataObject *pDataObject)
{
    HRESULT hr = (NULL != pDataObject) ? S_OK : E_INVALIDARG;

    if ( SUCCEEDED(hr) && (NULL == m_pDragSourceHelper2) )
    {
        hr = CoCreateInstance(
            CLSID_DragDropHelper,
            NULL, CLSCTX_INPROC,
            IID_PPV_ARGS(&m_pDragSourceHelper2));
    }

    if ( SUCCEEDED(hr) )
    {
        SHDRAGIMAGE di = { 0 };
        BITMAP bm = { 0 };
        if ( GetObject(hBitmap, sizeof(BITMAP), &bm) )
        {
            di.sizeDragImage.cx = bm.bmWidth;
            di.sizeDragImage.cy = bm.bmHeight;
            di.hbmpDragImage = (HBITMAP)CopyImage(hBitmap, IMAGE_BITMAP, 0, 0, LR_COPYFROMRESOURCE);
            di.crColorKey = RGB(128, 128, 128);//GetSysColor(COLOR_WINDOW);

            RECT rc = { 0 };
            POINT ptDrag = { 0 };
            if ( ::GetWindowRect(hWnd, &rc) && ::GetCursorPos(&ptDrag) )
            {
                di.ptOffset.x = ptDrag.x - rc.left;
                di.ptOffset.y = ptDrag.y - rc.top;
            }

            // Initialize the data object from bitmap.
            hr = m_pDragSourceHelper2->InitializeFromBitmap(&di, pDataObject);
        }

        SAFE_DELETE_OBJECT(di.hbmpDragImage);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkDragSourceHelper::InitializeFromWindow(HWND hWnd, POINT& pt, IDataObject *pDataObject)
{
    HRESULT hr = (NULL == pDataObject && IsWindow(hWnd)) ? S_OK : E_INVALIDARG;

    if ( SUCCEEDED(hr) && (NULL == m_pDragSourceHelper2) )
    {
        hr = CoCreateInstance(
            CLSID_DragDropHelper,
            NULL, CLSCTX_INPROC,
            IID_PPV_ARGS(&m_pDragSourceHelper2));
    }

    if ( SUCCEEDED(hr) )
    {
        POINT ppt = { pt.x, pt.y };
        hr = m_pDragSourceHelper2->InitializeFromWindow(hWnd, &ppt, pDataObject);
    }

    return hr;
}