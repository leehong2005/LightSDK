/*!
* @file AudioVolumeController.h
* 
* @brief This file defines class AudioVolumeController, which can control audio volume
*        and get notification from system when audio volume is changed.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/08
*/

#ifdef __cplusplus
#ifndef _VOLUMENCONTROLLER_H_
#define _VOLUMENCONTROLLER_H_

#include "Common.h"
#include "CommonMacro.h"
#include "AudioVolumeChangeHandler.h"
#include "AudioEndpointVolumeCallback.h"

struct IMMDevice;
struct IMMDeviceEnumerator;
struct IAudioEndpointVolume;

BEGIN_NAMESPACE_COMMON

/*!
* @brief AudioVolumeController, the controller for audio volume.
*        it implements IAudioEndpointVolumeCallback to get notification when audio volume is changed.
*/
class CLASS_DECLSPEC AudioVolumeController : public AudioEndpointVolumeCallbackReceiver
{
public:

    /*!
    * @brief The construct function.
    */
    AudioVolumeController();

    /*!
    * @brief The destructor function.
    */
    virtual ~AudioVolumeController();

    /*!
    * @brief Set the volume of audio. The value is from 0 to 100.
    *
    * @param fVolume    [I/ ] The value of volume, 0 is minimum, 100 is maximum.
    */
    virtual void SetVolume(FLOAT fVolume);

    /*!
    * @brief Get current volume of audio.
    *
    * @return The value of volume.
    */
    virtual FLOAT GetVolume();

    /*!
    * @brief Set the audio mute or not, decided by the isMute parameter.
    *
    * @param isMute     [I/ ] Indicates audio mute or not, TRUE if mute, otherwise FALSE.
    */
    virtual void SetMute(BOOL isMute);

    /*!
    * @brief Indicates the audio device is mute or not.
    *
    * @return TRUE if it is mute, otherwise return FALSE.
    */
    virtual BOOL IsMute();

    /*!
    * @brief Set the volume change handler to get notification when volume is changed.
    *
    * @param pVolumeChangeHandler   [I/ ] The pointer to AudioVolumeChangeHandler class.
    */
    virtual void SetOnVolumeChangeHandler(AudioVolumeChangeHandler *pVolumeChangeHandler);

protected:

    /*!
    * @brief Call this method to initialize COM interfaces.
    *
    * @return TRUE if succeeds, FALSE if fails.
    */
    virtual BOOL Initialize();

    /*!
    * @brief The OnNotify method notifies the client that the volume level or muting state of the audio endpoint device has changed.
    *
    * @param pNotify    [I/ ] Pointer to the volume-notification data.
    */
    virtual void OnNotifyReceiver(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);

private:

    GUID                         m_guidDeviceContext;               // The GUID to identify device context.
    IMMDevice                   *m_pMMDevice;                       // The instance of IMMDevice.
    IMMDeviceEnumerator         *m_pMMDeviceEnumerator;             // The instance of IMMDeviceEnumerator.
    IAudioEndpointVolume        *m_pAudioEndpointVolume;            // The instance of IAudioEndpointVolume.
    AudioVolumeChangeHandler    *m_pVolumeChangeHandler;            // The instance of AudioVolumeChangeHandler.
    AudioEndpointVolumeCallback *m_pAudioEndpointVolumeCallback;    // The instance of AudioEndpointVolumeCallback.
};

END_NAMESPACE

#endif // _VOLUMENCONTROLLER_H_
#endif // __cplusplus
