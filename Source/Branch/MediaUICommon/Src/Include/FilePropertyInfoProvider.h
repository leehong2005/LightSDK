/*!
* @file FilePropertyInfoProvider.h
* 
* @brief This file defines functions to help to get media infos from specified path.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/27
*/

#ifdef __cplusplus
#ifndef _MEDIAINFOPROVIDER_H_
#define _MEDIAINFOPROVIDER_H_

#include "Common.h"
#include "CommonMacro.h"
#include "FilePropertyHelper.h"
#include "FilePropertyDef.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief Media Infos providers.
*/
class CLASS_DECLSPEC FilePropertyInfoProvider
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
    static FilePropInfo* GetFilePropInfo(IN QUERYCONDITIONKIND kind, IN LPCTSTR lpFilePath);

protected:

    /*!
    * @brief Get the music info.
    *
    * @param lpFilePath [I/ ] The file path of media file.
    *
    * @return Pointer to MusicInfo.
    */
    static MusicFilePropInfo* GetMusicFilePropInfo(IN LPCTSTR lpFilePath);

    /*!
    * @brief Get the video info.
    *
    * @param lpFilePath [I/ ] The file path of media file.
    *
    * @return Pointer to VideoInfo.
    */
    static VideoFilePropInfo* GetVideoFilePropInfo(IN LPCTSTR lpFilePath);
    
    /*!
    * @brief Get the picture info.
    *
    * @param lpFilePath [I/ ] The file path of media file.
    *
    * @return Pointer to PictureInfo.
    */
    static PictureFilePropInfo* GetPictureFilePropInfo(IN LPCTSTR lpFilePath);
    
    /*!
    * @brief Get the link info.
    *
    * @param lpFilePath [I/ ] The file path of media file.
    *
    * @return Pointer to LinkInfo.
    */
    static LinkFilePropInfo* GetLinkFilePropInfo(IN LPCTSTR lpFilePath);
};

END_NAMESPACE

#endif // _MEDIAINFOPROVIDER_H_
#endif // __cplusplus
