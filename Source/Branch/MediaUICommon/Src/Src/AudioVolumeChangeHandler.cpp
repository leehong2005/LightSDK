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

#include "AudioVolumeChangeHandler.h"

USING_NAMESPACE_COMMON

AudioVolumeChangeHandler::~AudioVolumeChangeHandler()
{
}

//////////////////////////////////////////////////////////////////////////

void AudioVolumeChangeHandler::OnAudioVolumeChange(AudioVolumeController *pVolumeController)
{
    UNREFERENCED_PARAMETER(pVolumeController);
}