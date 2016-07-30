/*!
* @file AudioVolumeController.cpp
* 
* @brief This file defines class AudioVolumeController, which can control audio volume
*        and get notification from system when audio volume is changed.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/08
*/

#include <strmif.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "AudioVolumeController.h"
#include "AudioEndpointVolumeCallback.h"

USING_NAMESPACE_COMMON

#define MAX_VOLUME      100

AudioVolumeController::AudioVolumeController() : m_pMMDevice(NULL),
                                                 m_pMMDeviceEnumerator(NULL),
                                                 m_pAudioEndpointVolume(NULL),
                                                 m_pVolumeChangeHandler(NULL),
                                                 m_pAudioEndpointVolumeCallback(NULL),
                                                 m_guidDeviceContext(GUID_NULL)
{
    Initialize();
}

//////////////////////////////////////////////////////////////////////////

AudioVolumeController::~AudioVolumeController()
{
    if (NULL != m_pAudioEndpointVolume)
    {
        m_pAudioEndpointVolume->UnregisterControlChangeNotify(m_pAudioEndpointVolumeCallback);
    }

    SAFE_RELEASE(m_pAudioEndpointVolumeCallback);
    SAFE_RELEASE(m_pMMDevice);
    SAFE_RELEASE(m_pMMDeviceEnumerator);
    SAFE_RELEASE(m_pAudioEndpointVolume);
}

//////////////////////////////////////////////////////////////////////////

void AudioVolumeController::SetVolume(FLOAT fVolume)
{
    if (NULL != m_pAudioEndpointVolume)
    {
        fVolume = fVolume / (FLOAT)MAX_VOLUME;
        m_pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume, &m_guidDeviceContext);
    }
}

//////////////////////////////////////////////////////////////////////////

FLOAT AudioVolumeController::GetVolume()
{
    FLOAT fVolume = 0.0f;
    if (NULL != m_pAudioEndpointVolume)
    {
        m_pAudioEndpointVolume->GetMasterVolumeLevelScalar(&fVolume);
        fVolume = (FLOAT)(INT32)(fVolume * (FLOAT)MAX_VOLUME + 0.5);
    }

    return fVolume;
}

//////////////////////////////////////////////////////////////////////////

void AudioVolumeController::SetMute(BOOL isMute)
{
    if (NULL != m_pAudioEndpointVolume)
    {
        m_pAudioEndpointVolume->SetMute(isMute, &m_guidDeviceContext);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL AudioVolumeController::IsMute()
{
    BOOL isMute = FALSE;
    if (NULL != m_pAudioEndpointVolume)
    {
        m_pAudioEndpointVolume->GetMute(&isMute);
    }

    return isMute;
}

//////////////////////////////////////////////////////////////////////////

void AudioVolumeController::SetOnVolumeChangeHandler(AudioVolumeChangeHandler *pVolumeChangeHandler)
{
    m_pVolumeChangeHandler = pVolumeChangeHandler;
}

//////////////////////////////////////////////////////////////////////////

BOOL AudioVolumeController::Initialize()
{
    HRESULT hr = E_FAIL;

    hr = CoCreateGuid(&m_guidDeviceContext);
    if (SUCCEEDED(hr))
    {
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
                              NULL,
                              CLSCTX_INPROC_SERVER,
                              IID_PPV_ARGS(&m_pMMDeviceEnumerator));
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pMMDevice);
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pMMDevice->Activate(__uuidof(IAudioEndpointVolume),
                                   CLSCTX_ALL,
                                   NULL,
                                   (void**)&m_pAudioEndpointVolume);
    }

    if (SUCCEEDED(hr))
    {
        m_pAudioEndpointVolumeCallback = new AudioEndpointVolumeCallback();
        m_pAudioEndpointVolumeCallback->SetNotifyReceiver(this);
        hr = m_pAudioEndpointVolume->RegisterControlChangeNotify(m_pAudioEndpointVolumeCallback);
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

void AudioVolumeController::OnNotifyReceiver(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
    if (NULL != pNotify)
    {
        if (pNotify->guidEventContext != m_guidDeviceContext)
        {
            if (NULL != m_pVolumeChangeHandler)
            {
                m_pVolumeChangeHandler->OnAudioVolumeChange(this);
            }
        }
    }
}
