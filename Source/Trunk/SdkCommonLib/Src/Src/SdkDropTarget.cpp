/*!
* @file SdkDropTarget.cpp
* 
* @brief The file define the class.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#include "stdafx.h"
#include "SdkDropTarget.h"

USING_NAMESPACE_COMMON

SdkDropTarget::SdkDropTarget() : m_lRefCount(1),    // The reference count must be 1.
                                 m_isDataAvailable(TRUE),
                                 m_hTargetWnd(NULL),
                                 m_pDropTargetNotify(NULL),
                                 m_pDropTargetHelper(NULL)
{
    CoCreateInstance(
        CLSID_DragDropHelper,
        NULL,
        CLSCTX_INPROC,
        IID_PPV_ARGS(&m_pDropTargetHelper));
}

//////////////////////////////////////////////////////////////////////////

SdkDropTarget::~SdkDropTarget()
{
    SAFE_RELEASE(m_pDropTargetHelper);

    m_hTargetWnd = NULL;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkDropTarget::RegisterDropTarget(HWND hTargetWnd, IDropTargetNotify *pDropTargetNotify)
{
    HRESULT hr = E_FAIL;

    if ( IsWindow(hTargetWnd) && (m_hTargetWnd != hTargetWnd) )
    {
        hr = ::RegisterDragDrop(hTargetWnd, this);
        if ( SUCCEEDED(hr) )
        {
            m_pDropTargetNotify = pDropTargetNotify;
            m_hTargetWnd = hTargetWnd;
        }
        else
        {
            m_hTargetWnd = NULL;
        }
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkDropTarget::RevokeDropTarget()
{
    HRESULT hr = E_FAIL;

    if (NULL != m_hTargetWnd)
    {
        hr = ::RevokeDragDrop(m_hTargetWnd);
        m_hTargetWnd = NULL;
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

DROPEFFECT SdkDropTarget::OnDragEnter(HWND hWnd, IDataObject *pDataObj, DWORD dwKeyState, POINT pt)
{
    UNREFERENCED_PARAMETER(hWnd);
    UNREFERENCED_PARAMETER(pDataObj);
    UNREFERENCED_PARAMETER(dwKeyState);
    UNREFERENCED_PARAMETER(pt);

    DROPEFFECT dwDropEffect = DROPEFFECT_NONE;

    if ( NULL != m_pDropTargetNotify )
    {
        dwDropEffect = m_pDropTargetNotify->OnDragEnter(hWnd, pDataObj, dwKeyState, pt);
    }

    // Not implement
    return dwDropEffect;
}

//////////////////////////////////////////////////////////////////////////

DROPEFFECT SdkDropTarget::OnDragOver(HWND hWnd, DWORD dwKeyState, POINT pt)
{
    UNREFERENCED_PARAMETER(hWnd);
    UNREFERENCED_PARAMETER(dwKeyState);
    UNREFERENCED_PARAMETER(pt);

    DROPEFFECT dwDropEffect = DROPEFFECT_NONE;

    if ( NULL != m_pDropTargetNotify )
    {
        dwDropEffect = m_pDropTargetNotify->OnDragOver(hWnd, dwKeyState, pt);
    }

    // Not implement
    return dwDropEffect;
}

//////////////////////////////////////////////////////////////////////////

void SdkDropTarget::OnDragLeave(HWND hWnd)
{
    UNREFERENCED_PARAMETER(hWnd);
    // Not implement

    if ( NULL != m_pDropTargetNotify )
    {
        m_pDropTargetNotify->OnDragLeave(hWnd);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkDropTarget::OnDrop(HWND hWnd, IDataObject *pDataObj, DWORD dwKeyState, POINT pt)
{
    UNREFERENCED_PARAMETER(hWnd);
    UNREFERENCED_PARAMETER(pDataObj);
    UNREFERENCED_PARAMETER(dwKeyState);
    UNREFERENCED_PARAMETER(pt);

    BOOL bRet = TRUE;

    if ( NULL != m_pDropTargetNotify )
    {
        bRet = m_pDropTargetNotify->OnDrop(hWnd, pDataObj, dwKeyState, pt);
    }

    // Not implement.
    return bRet;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDropTarget::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(SdkDropTarget, IDropTarget),
        { 0 },
    };

    return QISearch(this, qit, riid, ppv);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) SdkDropTarget::AddRef()
{
    return InterlockedIncrement(&m_lRefCount);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) SdkDropTarget::Release()
{
    ULONG lRef = InterlockedDecrement(&m_lRefCount);
    if (0 == lRef)
    {
        delete this;
    }
    return m_lRefCount;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDropTarget::DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    if ( NULL == pDataObj )
    {
        return E_INVALIDARG;
    }

    POINT ppt = { pt.x, pt.y };
    DROPEFFECT dwEffect = OnDragEnter(m_hTargetWnd, pDataObj, grfKeyState, ppt);
    *pdwEffect = dwEffect;
    m_isDataAvailable = (DROPEFFECT_NONE == dwEffect) ? FALSE : TRUE;

    HRESULT hr = S_OK;
    if ( NULL != m_pDropTargetHelper )
    {
        hr = m_pDropTargetHelper->DragEnter(m_hTargetWnd, pDataObj, &ppt, *pdwEffect);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    POINT ppt = { pt.x, pt.y };
    DWORD dwEffect = OnDragOver(m_hTargetWnd, grfKeyState, ppt);
    *pdwEffect = (FALSE == m_isDataAvailable) ? DROPEFFECT_NONE : dwEffect;

    if ( NULL != m_pDropTargetHelper )
    {
        m_pDropTargetHelper->DragOver(&ppt, *pdwEffect);
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDropTarget::DragLeave(void)
{
    OnDragLeave(m_hTargetWnd);

    m_isDataAvailable = TRUE;
    if ( NULL != m_pDropTargetHelper )
    {
        m_pDropTargetHelper->DragLeave();
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDropTarget::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    if ( NULL == pDataObj )
    {
        return E_INVALIDARG;
    }

    POINT ppt = { pt.x, pt.y };
    if ( NULL != m_pDropTargetHelper )
    {
        m_pDropTargetHelper->Drop(pDataObj, &ppt, *pdwEffect);
    }

    BOOL isOK = OnDrop(m_hTargetWnd, pDataObj, grfKeyState, ppt);

    return (isOK) ? S_OK : E_FAIL;
}

//////////////////////////////////////////////////////////////////////////

DROPEFFECT SdkDropTarget::FilterDropEffect(DWORD grfKeyState, DROPEFFECT dwEffectSrc)
{
    DROPEFFECT dwEffect = dwEffectSrc;

    //CTRL+SHIFT  -- DROPEFFECT_LINK
    //CTRL        -- DROPEFFECT_COPY
    //SHIFT       -- DROPEFFECT_MOVE
    //no modifier -- DROPEFFECT_MOVE or whatever is allowed by src

    if ( MK_SHIFT & grfKeyState )
    {
        dwEffect = DROPEFFECT_MOVE;
    }

    if ( MK_CONTROL & grfKeyState )
    {
        dwEffect = DROPEFFECT_COPY;
    }

    if ( (MK_CONTROL & grfKeyState) && (MK_SHIFT & grfKeyState) )
    {
        dwEffect = DROPEFFECT_LINK;
    }

    return dwEffect;
}
