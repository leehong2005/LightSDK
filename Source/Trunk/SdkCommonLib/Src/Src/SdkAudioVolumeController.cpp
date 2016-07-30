/*!
* @file SdkAudioVolumeController.cpp
* 
* @brief This file defines class SdkAudioVolumeController, which can control audio volume
*        and get notification from system when audio volume is changed.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/08
*/

#include "stdafx.h"
#include <strmif.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "SdkAudioVolumeController.h"
#include "SdkAudioEndpointVolumeCallback.h"

USING_NAMESPACE_UTILITIES

#define MAX_VOLUME      100

SdkAudioVolumeController::SdkAudioVolumeController() : m_pMMDevice(NULL),
                                                       m_pMMDeviceEnumerator(NULL),
                                                       m_pAudioEndpointVolume(NULL),
                                                       m_pVolumeChangeHandler(NULL),
                                                       m_pAudioEndpointVolumeCallback(NULL),
                                                       m_guidDeviceContext(GUID_NULL)
{
    Initialize();
}

//////////////////////////////////////////////////////////////////////////

SdkAudioVolumeController::~SdkAudioVolumeController()
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

void SdkAudioVolumeController::SetVolume(FLOAT fVolume)
{
    if (NULL != m_pAudioEndpointVolume)
    {
        fVolume = fVolume / (FLOAT)MAX_VOLUME;
        m_pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume, &m_guidDeviceContext);
    }
}

//////////////////////////////////////////////////////////////////////////

FLOAT SdkAudioVolumeController::GetVolume()
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

void SdkAudioVolumeController::SetMute(BOOL isMute)
{
    if (NULL != m_pAudioEndpointVolume)
    {
        m_pAudioEndpointVolume->SetMute(isMute, &m_guidDeviceContext);
    }
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkAudioVolumeController::IsMute()
{
    BOOL isMute = FALSE;
    if (NULL != m_pAudioEndpointVolume)
    {
        m_pAudioEndpointVolume->GetMute(&isMute);
    }

    return isMute;
}

//////////////////////////////////////////////////////////////////////////

void SdkAudioVolumeController::SetOnVolumeChangeHandler(IAudioVolumeNotify *pVolumeChangeHandler)
{
    m_pVolumeChangeHandler = pVolumeChangeHandler;
}

//////////////////////////////////////////////////////////////////////////

BOOL SdkAudioVolumeController::Initialize()
{
    HRESULT hr = E_FAIL;

    hr = CoCreateGuid(&m_guidDeviceContext);
    if (SUCCEEDED(hr))
    {
        hr = CoCreateInstance(
            __uuidof(MMDeviceEnumerator),
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
        m_pAudioEndpointVolumeCallback = new SdkAudioEndpointVolumeCallback();
        m_pAudioEndpointVolumeCallback->SetNotifyReceiver(this);
        hr = m_pAudioEndpointVolume->RegisterControlChangeNotify(m_pAudioEndpointVolumeCallback);
    }

    return SUCCEEDED(hr) ? TRUE : FALSE;
}

//////////////////////////////////////////////////////////////////////////

void SdkAudioVolumeController::OnNotifyReceiver(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
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
