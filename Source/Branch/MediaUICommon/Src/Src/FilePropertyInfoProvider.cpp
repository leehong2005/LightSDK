/*!
* @file FilePropertyInfoProvider.cpp
* 
* @brief This file defines functions to help to get media infos from specified path.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/27
*/

#include "FilePropertyInfoProvider.h"

USING_NAMESPACE_COMMON

FilePropInfo* FilePropertyInfoProvider::GetFilePropInfo(IN QUERYCONDITIONKIND kind, IN LPCTSTR lpFilePath)
{
    FilePropInfo *pMeidaInfo = NULL;

    switch (kind)
    {
    case QUERY_KIND_MUSIC:
        pMeidaInfo = GetMusicFilePropInfo(lpFilePath);
        break;

    case QUERY_KIND_PICTURE:
        pMeidaInfo = GetPictureFilePropInfo(lpFilePath);
        break;

    case QUERY_KIND_VIDEO:
        pMeidaInfo = GetVideoFilePropInfo(lpFilePath);
        break;

    case QUERY_KIND_LINK:
        pMeidaInfo = GetLinkFilePropInfo(lpFilePath);
        break;
    }

    return pMeidaInfo;
}

//////////////////////////////////////////////////////////////////////////

MusicFilePropInfo* FilePropertyInfoProvider::GetMusicFilePropInfo(IN LPCTSTR lpFilePath)
{
    FilePropertyHelper fileProp(lpFilePath);

    MusicFilePropInfo *pInfo = new MusicFilePropInfo();
    ZeroMemory(pInfo, sizeof(MusicFilePropInfo));

    pInfo->eFilePropType = FILEPROPTYPE_MUSIC;
    fileProp.GetString(SYSTEM_CORE_COMMENT,         pInfo->szComment,         MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_COMPANY,         pInfo->szCompany,         MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_TITLE,           pInfo->szTitle,           MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_PARSINGPATH,     pInfo->szFilePath,        MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_FILENAME,        pInfo->szFileName,        MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_ITEMTYPETEXT,    pInfo->szItemType,        MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_RATINGTEXT,      pInfo->szRatingText,      MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_KEYWORDS,        pInfo->szTags,            MAX_PATH);
    fileProp.FormatForDisplay(SYSTEM_CORE_SIZE,     pInfo->szFileSize,        MAX_PATH);
    fileProp.FormatForDisplay(SYSTEM_MEDIA_DURATION,pInfo->szDuration,        MAX_PATH);
    fileProp.GetDateTime(SYSTEM_CORE_DATEACCESSED, &pInfo->ftDateAccessed);
    fileProp.GetDateTime(SYSTEM_CORE_DATECREATED,  &pInfo->ftDateCreated);
    fileProp.GetDateTime(SYSTEM_CORE_DATEMODIFIED, &pInfo->ftDateModified);
    fileProp.GetUInt64(SYSTEM_CORE_SIZE,           &pInfo->uFileSize);
    fileProp.GetUInt64(SYSTEM_MEDIA_DURATION,      (ULONGLONG*)&pInfo->uDuration);
    fileProp.GetUInt32(SYSTEM_CORE_RATING,         (ULONG*)&pInfo->uItemRating);
    // Get music information.
    fileProp.GetString(SYSTEM_MEDIA_YEAR,           pInfo->szYear,            MAX_PATH);
    fileProp.GetString(SYSTEM_MUSIC_ALBUMTITLE,     pInfo->szAlbum,           MAX_PATH);
    fileProp.GetString(SYSTEM_MUSIC_ALBUMARTIST,    pInfo->szAlbumAartist,    MAX_PATH);
    fileProp.GetString(SYSTEM_MUSIC_ARTIST,         pInfo->szContributeArtist,MAX_PATH);
    fileProp.GetString(SYSTEM_MUSIC_GENRE,          pInfo->szGenre,           MAX_PATH);

    return pInfo;
}

//////////////////////////////////////////////////////////////////////////

VideoFilePropInfo* FilePropertyInfoProvider::GetVideoFilePropInfo(IN LPCTSTR lpFilePath)
{
    FilePropertyHelper fileProp(lpFilePath);

    VideoFilePropInfo *pInfo = new VideoFilePropInfo();
    ZeroMemory(pInfo, sizeof(VideoFilePropInfo));

    pInfo->eFilePropType = FILEPROPTYPE_VIDEO;
    // Get Basic information.
    fileProp.GetString(SYSTEM_CORE_COMMENT,             pInfo->szComment,      MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_COMPANY,             pInfo->szCompany,      MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_TITLE,               pInfo->szTitle,        MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_PARSINGPATH,         pInfo->szFilePath,     MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_FILENAME,            pInfo->szFileName,     MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_ITEMTYPETEXT,        pInfo->szItemType,     MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_RATINGTEXT,          pInfo->szRatingText,   MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_KEYWORDS,            pInfo->szTags,         MAX_PATH);
    fileProp.FormatForDisplay(SYSTEM_CORE_SIZE,         pInfo->szFileSize,     MAX_PATH);
    fileProp.FormatForDisplay(SYSTEM_MEDIA_DURATION,    pInfo->szDuration,     MAX_PATH);
    fileProp.GetDateTime(SYSTEM_CORE_DATEACCESSED,     &pInfo->ftDateAccessed);
    fileProp.GetDateTime(SYSTEM_CORE_DATECREATED,      &pInfo->ftDateCreated);
    fileProp.GetDateTime(SYSTEM_CORE_DATEMODIFIED,     &pInfo->ftDateModified);
    fileProp.GetUInt64(SYSTEM_CORE_SIZE,               &pInfo->uFileSize);
    fileProp.GetUInt32(SYSTEM_CORE_RATING,              (ULONG*)&pInfo->uItemRating);
    // Get video information.
    fileProp.GetUInt64(SYSTEM_MEDIA_DURATION,           (ULONGLONG*)&pInfo->uLength);
    fileProp.GetUInt32(SYSTEM_VIDEO_FRAMEWIDTH,         (ULONG*)&pInfo->uFrameWidth);
    fileProp.GetUInt32(SYSTEM_VIDEO_FRAMEHEIGHT,        (ULONG*)&pInfo->uFrameHeight);
    fileProp.GetUInt32(SYSTEM_VIDEO_FRAMERATE,          (ULONG*)&pInfo->uFrameRatio);
    fileProp.GetUInt32(SYSTEM_VIDEO_HORIZONTALASPECTRATIO,(ULONG*)&pInfo->uHorizontalAspectRatio);
    fileProp.GetUInt32(SYSTEM_VIDEO_VERTICALASPECTRATIO, (ULONG*)&pInfo->uVerticalAspectRatio);

    return pInfo;
}

//////////////////////////////////////////////////////////////////////////

PictureFilePropInfo* FilePropertyInfoProvider::GetPictureFilePropInfo(IN LPCTSTR lpFilePath)
{
    FilePropertyHelper fileProp(lpFilePath);

    PictureFilePropInfo *pInfo = new PictureFilePropInfo();
    ZeroMemory(pInfo, sizeof(PictureFilePropInfo));

    pInfo->eFilePropType = FILEPROPTYPE_PICTURE;
    // Get Basic information.
    fileProp.GetString(SYSTEM_CORE_COMMENT,          pInfo->szComment,      MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_COMPANY,          pInfo->szCompany,      MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_TITLE,            pInfo->szTitle,        MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_PARSINGPATH,      pInfo->szFilePath,     MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_FILENAME,         pInfo->szFileName,     MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_ITEMTYPETEXT,     pInfo->szItemType,     MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_RATINGTEXT,       pInfo->szRatingText,   MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_KEYWORDS,         pInfo->szTags,         MAX_PATH);
    fileProp.FormatForDisplay(SYSTEM_CORE_SIZE,      pInfo->szFileSize,     MAX_PATH);
    fileProp.GetDateTime(SYSTEM_CORE_DATEACCESSED,  &pInfo->ftDateAccessed);
    fileProp.GetDateTime(SYSTEM_CORE_DATECREATED,   &pInfo->ftDateCreated);
    fileProp.GetDateTime(SYSTEM_CORE_DATEMODIFIED,  &pInfo->ftDateModified);
    fileProp.GetUInt64(SYSTEM_CORE_SIZE,            &pInfo->uFileSize);
    fileProp.GetUInt32(SYSTEM_CORE_RATING,          (ULONG*)&pInfo->uItemRating);
    //Get picture information.
    fileProp.GetUInt32(SYSTEM_IMAGE_HORIZONTALSIZE,  (ULONG*)&pInfo->uHorizontalSize);
    fileProp.GetUInt32(SYSTEM_IMAGE_VERTICALSIZE,    (ULONG*)&pInfo->uVerticalSize);
    fileProp.GetUInt32(SYSTEM_PHOTO_ORIENTATION,     (ULONG*)&pInfo->uOrientation);//UInt16
    fileProp.GetUInt32(SYSTEM_PHOTO_ISOSPEED,        (ULONG*)&pInfo->uISOSpeed);   //UINT16.
    fileProp.GetDateTime(SYSTEM_PHOTO_DATETAKEN,    &pInfo->ftDateTaken);
    fileProp.GetString(SYSTEM_PHOTO_ORIENTATIONTEXT, pInfo->szOrientationText,MAX_PATH);
    fileProp.GetString(SYSTEM_IMAGE_DIMENSIONS,      pInfo->szDimensions,     MAX_PATH);

    return pInfo;
}

//////////////////////////////////////////////////////////////////////////

LinkFilePropInfo* FilePropertyInfoProvider::GetLinkFilePropInfo(IN LPCTSTR lpFilePath)
{
    FilePropertyHelper fileProp(lpFilePath);

    LinkFilePropInfo *pInfo = new LinkFilePropInfo();
    ZeroMemory(pInfo, sizeof(LinkFilePropInfo));

    pInfo->eFilePropType = FILEPROPTYPE_LINK;
    // Get Basic information.
    fileProp.GetString(SYSTEM_CORE_COMMENT,         pInfo->szComment,      MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_COMPANY,         pInfo->szCompany,      MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_TITLE,           pInfo->szTitle,        MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_PARSINGPATH,     pInfo->szFilePath,     MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_FILENAME,        pInfo->szFileName,     MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_ITEMTYPETEXT,    pInfo->szItemType,     MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_RATINGTEXT,      pInfo->szRatingText,   MAX_PATH);
    fileProp.GetString(SYSTEM_CORE_KEYWORDS,        pInfo->szTags,         MAX_PATH);
    fileProp.FormatForDisplay(SYSTEM_CORE_SIZE,     pInfo->szFileSize,     MAX_PATH);
    fileProp.GetDateTime(SYSTEM_CORE_DATEACCESSED, &pInfo->ftDateAccessed);
    fileProp.GetDateTime(SYSTEM_CORE_DATECREATED,  &pInfo->ftDateCreated);
    fileProp.GetDateTime(SYSTEM_CORE_DATEMODIFIED, &pInfo->ftDateModified);
    fileProp.GetUInt64(SYSTEM_CORE_SIZE,           &pInfo->uFileSize);
    fileProp.GetUInt32(SYSTEM_CORE_RATING,         (ULONG*)&pInfo->uItemRating);

    // Get Link information.
    fileProp.GetString(SYSTEM_LINK_ITEMURL,         pInfo->szItemUrl,      INTERNET_MAX_URL);

    return pInfo;
}
