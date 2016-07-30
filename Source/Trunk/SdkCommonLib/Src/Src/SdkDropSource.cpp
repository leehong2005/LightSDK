/*!
* @file SdkDropSource.cpp
* 
* @brief The file define the class.
*
* Copyright (C) 2009, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/10
*/

#include "stdafx.h"
#include "SdkDropSource.h"

USING_NAMESPACE_COMMON

SdkDropSource::SdkDropSource()
{
    m_lRefCount = 1;
}

//////////////////////////////////////////////////////////////////////////

SdkDropSource::~SdkDropSource()
{
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDropSource::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(SdkDropSource, IDropSource),
        { 0 }
    };

    return QISearch(this, qit, riid, ppv);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) SdkDropSource::AddRef()
{
    return InterlockedIncrement(&m_lRefCount);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) SdkDropSource::Release()
{
    ULONG lRef = InterlockedDecrement(&m_lRefCount);
    if (0 == lRef)
    {
        delete this;
    }
    return m_lRefCount;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
    if( TRUE == fEscapePressed )
    {
        return DRAGDROP_S_CANCEL;
    }

    if( !(grfKeyState & (MK_LBUTTON|MK_RBUTTON)) )
    {
        return DRAGDROP_S_DROP;
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkDropSource::GiveFeedback(DWORD dwEffect)
{
    UNREFERENCED_PARAMETER(dwEffect);

    // Use default cursor.
    return DRAGDROP_S_USEDEFAULTCURSORS;
}