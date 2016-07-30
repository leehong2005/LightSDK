/*!
* @file AudioEndpointVolumeCallback.cpp
* 
* @brief This file defines class AudioEndpointVolumeCallback, which defines functions for notification when
*        volume is changed.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/08
*/

#include <Shlwapi.h>
#include "AudioEndpointVolumeCallback.h"

USING_NAMESPACE_COMMON

AudioEndpointVolumeCallback::AudioEndpointVolumeCallback() : m_cRefCount(1)
{
    m_pCallbackReceiver = NULL;
}

//////////////////////////////////////////////////////////////////////////

AudioEndpointVolumeCallback::~AudioEndpointVolumeCallback()
{
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) AudioEndpointVolumeCallback::AddRef()
{
    return InterlockedIncrement(&m_cRefCount);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP_(ULONG) AudioEndpointVolumeCallback::Release()
{
    ULONG ulRef = InterlockedDecrement(&m_cRefCount);
    if (0 == ulRef)
    {
        delete this;
    }

    return ulRef;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP AudioEndpointVolumeCallback::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(AudioEndpointVolumeCallback, IUnknown),
        QITABENT(AudioEndpointVolumeCallback, IAudioEndpointVolumeCallback),
        { 0 },
    };

    return QISearch(this, qit, riid, ppv);
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP AudioEndpointVolumeCallback::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
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

STDMETHODIMP AudioEndpointVolumeCallback::SetNotifyReceiver(AudioEndpointVolumeCallbackReceiver *pCallbackReceiver)
{
    m_pCallbackReceiver = pCallbackReceiver;
    return S_OK;
}
