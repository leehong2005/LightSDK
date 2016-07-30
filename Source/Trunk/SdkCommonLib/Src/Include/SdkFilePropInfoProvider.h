/*!
* @file SdkFilePropInfoProvider.h
* 
* @brief This file defines functions to help to get media infos from specified path.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/27
*/

#ifdef __cplusplus
#ifndef _SDKFILEPROPINFOPROVIDER_H_
#define _SDKFILEPROPINFOPROVIDER_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"
#include "SdkFilePropHelper.h"
#include "SdkFilePropDef.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief Media Infos providers.
*/
class CLASS_DECLSPEC SdkFilePropInfoProvider
{
public:

    /*!
    * @brief Get media infos from specified file path.
    *
    * @param kind       [I/ ] One of QUERYCONDITIONKIND enumeration to specify which kind media to retrieve.
    * @param lpFilePath [I/ ] The file path of the media file.
    *
    * @return The pointer to MediaInfo structure, it is caller's responsibility to delete the memory if it no longer uses.
    */
    static FILEPROPINFO* GetFilePropInfo(IN QUERYCONDITIONKIND kind, IN LPCTSTR lpFilePath);

protected:

    /*!
    * @brief Get the music info.
    *
    * @param lpFilePath [I/ ] The file path of media file.
    *
    * @return Pointer to MusicInfo.
    */
    static MUSICFILEPROPINFO* GetMusicFilePropInfo(IN LPCTSTR lpFilePath);

    /*!
    * @brief Get the video info.
    *
    * @param lpFilePath [I/ ] The file path of media file.
    *
    * @return Pointer to VideoInfo.
    */
    static VIDEOFILEPROPINFO* GetVideoFilePropInfo(IN LPCTSTR lpFilePath);
    
    /*!
    * @brief Get the picture info.
    *
    * @param lpFilePath [I/ ] The file path of media file.
    *
    * @return Pointer to PictureInfo.
    */
    static PICTUREFILEPROPINFO* GetPictureFilePropInfo(IN LPCTSTR lpFilePath);
    
    /*!
    * @brief Get the link info.
    *
    * @param lpFilePath [I/ ] The file path of media file.
    *
    * @return Pointer to LinkInfo.
    */
    static LINKFILEPROPINFO* GetLinkFilePropInfo(IN LPCTSTR lpFilePath);
};

END_NAMESPACE_COMMON

#endif // _SDKFILEPROPINFOPROVIDER_H_
#endif // __cplusplus
