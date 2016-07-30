/*!
* @file AudioEndpointVolumeCallback.h
* 
* @brief This file defines class AudioEndpointVolumeCallback, which defines functions for notification when
*        volume is changed.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/08
*/

#ifdef __cplusplus
#ifndef _AUDIOENDPOINTVOLUMECALLBACK_H_
#define _AUDIOENDPOINTVOLUMECALLBACK_H_

// Add this header file before endpointvolume.h is to solve definition conflict.
#include "Common.h"
#include "CommonMacro.h"
#include <DShow.h> 
#include <endpointvolume.h>

BEGIN_NAMESPACE_COMMON

class AudioEndpointVolumeCallbackReceiver;

/*!
* @brief AudioEndpointVolumeCallback class.
*/
class AudioEndpointVolumeCallback : public IAudioEndpointVolumeCallback
{
public:

    /*!
    * @brief The construct function.
    */
    AudioEndpointVolumeCallback();

    /*!
    * @brief The destructor function.
    */
    virtual ~AudioEndpointVolumeCallback();

    /*!
    * @brief Add reference count.
    */
    STDMETHODIMP_(ULONG) AddRef();

    /*!
    * @brief Decrement reference count, if the reference count is equal zero, this function
    *        will release the memory allocated by this class.
    */
    STDMETHODIMP_(ULONG) Release();

    /*!
    * @brief Returns a pointer to a specified interface on an object to which a client currently holds an interface pointer.
    *
    * @param riid       [I/ ] Identifier of the interface being requested.
    * @param ppv        [ /O] Address of pointer variable that receives the interface pointer requested in riid 
    *
    * @return S_OK if the interface is supported, E_NOINTERFACE if not.
    */
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);

    /*!
    * @brief The OnNotify method notifies the client that the volume level or muting state of the audio endpoint device has changed.
    *
    * @param pNotify    [I/ ] Pointer to the volume-notification data.
    *
    * @return S_OK if the interface is supported, E_NOINTERFACE if not.
    */
    STDMETHODIMP OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);

    /*!
    * @brief Set the notification receiver.
    *
    * @param pCallbackReceiver  [I/ ] Pointer to the AudioEndpointVolumeReceiver.
    *
    * @return S_OK if the interface is supported, E_NOINTERFACE if not.
    */
    STDMETHODIMP SetNotifyReceiver(AudioEndpointVolumeCallbackReceiver *pCallbackReceiver);

private:

    LONG                                    m_cRefCount;            // The reference of this instance.
    AudioEndpointVolumeCallbackReceiver    *m_pCallbackReceiver;    // Notification receiver.
};

/*!
* @brief AudioEndpointVolumeCallback class.
*/
class CLASS_DECLSPEC AudioEndpointVolumeCallbackReceiver
{
public:

    /*!
    * @brief The construct function.
    */
    virtual ~AudioEndpointVolumeCallbackReceiver() {};

    /*!
    * @brief The OnNotify method notifies the client that the volume level or muting state of the audio endpoint device has changed.
    *
    * @param pNotify    [I/ ] Pointer to the volume-notification data.
    */
    virtual void OnNotifyReceiver(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify) 
    {
        UNREFERENCED_PARAMETER(pNotify);
    };
};

END_NAMESPACE

#endif // _AUDIOENDPOINTVOLUMECALLBACK_H_
#endif // __cplusplus
