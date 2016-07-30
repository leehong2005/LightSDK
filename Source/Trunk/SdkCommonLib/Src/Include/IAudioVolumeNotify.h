/*!
* @file IAudioVolumeNotify.h
* 
* @brief This file defines class IAudioVolumeNotify, which defines functions for notification when
*        volume is changed.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/08
*/

#ifdef __cplusplus
#ifndef _IAUDIOVOLUMECHANGE_H_
#define _IAUDIOVOLUMECHANGE_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_UTILITIES

class SdkAudioVolumeController;

/*!
* @brief The IAudioVolumeNotify class.
*/
class CLASS_DECLSPEC IAudioVolumeNotify
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IAudioVolumeNotify() {};

    /*!
    * @brief Called when audio volume is changed.
    *
    * @param pVolumeController  [I/ ] The SdkAudioVolumeController instance, from which you can
    *                                 get information about the audio volume.
    */
    virtual void OnAudioVolumeChange(SdkAudioVolumeController *pVolumeController) = 0;
};

END_NAMESPACE_UTILITIES

#endif // _IAUDIOVOLUMECHANGE_H_
#endif // __cplusplus