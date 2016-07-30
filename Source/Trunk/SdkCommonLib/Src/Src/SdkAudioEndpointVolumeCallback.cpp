/*!
* @file SdkAudioEndpointVolumeCallback.cpp
* 
* @brief This file defines class SdkAudioEndpointVolumeCallback, which defines functions for notification when
*        volume is changed.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/08
*/

#include "stdafx.h"
#include <Shlwapi.h>
#include "SdkAudioEndpointVolumeCallback.h"

USING_NAMESPACE_UTILITIES

SdkAudioEndpointVolumeCallback::SdkAudioEndpointVolumeCallback() : m_cRefCount(1)
{
    m_pCallbackReceiver = NULL;
}

//////////////////////////////////////////////////////////////////////////

SdkAudioEndpointVolumeCallback::~SdkAudioEndpointVolumeCallback()
{
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) SdkAudioEndpointVolumeCallback::AddRef()
{
    return InterlockedIncrement(&m_cRefCount);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) SdkAudioEndpointVolumeCallback::Release()
{
    ULONG ulRef = InterlockedDecrement(&m_cRefCount);
    if (0 == ulRef)
    {
        delete this;
    }

    return ulRef;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkAudioEndpointVolumeCallback::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(SdkAudioEndpointVolumeCallback, IUnknown),
        QITABENT(SdkAudioEndpointVolumeCallback, IAudioEndpointVolumeCallback),
        { 0 },
    };

    return QISearch(this, qit, riid, ppv);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkAudioEndpointVolumeCallback::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
    if (NULL == pNotify)
    {
        return E_INVALIDARG;
    }

    if (NULL != m_pCallbackReceiver)
    {
        m_pCallbackReceiver->OnNotifyReceiver(pNotify);
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP SdkAudioEndpointVolumeCallback::SetNotifyReceiver(
    SdkAudioEndpointVolumeCallbackReceiver *pCallbackReceiver)
{
    m_pCallbackReceiver = pCallbackReceiver;
    return S_OK;
}
