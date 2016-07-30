/*!
* @file AudioVolumeChangeHandler.h
* 
* @brief This file defines class AudioVolumeChangeHandler, which defines functions for notification when
*        volume is changed.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/08
*/

#ifdef __cplusplus
#ifndef _AUDIOVOLUMECALLBACK_H_
#define _AUDIOVOLUMECALLBACK_H_

#include "Common.h"
#include "CommonMacro.h"

BEGIN_NAMESPACE_COMMON

class AudioVolumeController;

/*!
* @brief The AudioVolumeChangeHandler class.
*/
class CLASS_DECLSPEC AudioVolumeChangeHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~AudioVolumeChangeHandler();

    /*!
    * @brief Called when audio volume is changed.
    *
    * @param pVolumeController  [I/ ] The AudioVolumeController instance, from which you can
    *                                 get information about the audio volume.
    */
    virtual void OnAudioVolumeChange(AudioVolumeController *pVolumeController);
};

END_NAMESPACE

#endif // _AUDIOVOLUMECALLBACK_H_
#endif // __cplusplus