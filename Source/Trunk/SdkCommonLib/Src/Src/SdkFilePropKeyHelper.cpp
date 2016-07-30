/*!
* @file SdkFilePropKeyHelper.cpp 
* 
* @brief This file define property key helper functions.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#include "stdafx.h"
#include "SdkFilePropKeyHelper.h"
#include <Shlguid.h>
#include <Propkey.h>

USING_NAMESPACE_COMMON

BOOL SdkFilePropKeyHelper::s_hasInitKeys = FALSE;
SdkFilePropKeyHelper::KeyInfoMap SdkFilePropKeyHelper::s_keyInfoMap;

//////////////////////////////////////////////////////////////////////////

BOOL SdkFilePropKeyHelper::GetPropertyKey(IN PROPERTYKEYNAME keyName, OUT PROPERTYKEYINFO &keyInfo)
{
    if (!s_hasInitKeys)
    {
        InitPropertyKey();
        s_hasInitKeys = TRUE;
    }

    KeyInfoMap::iterator itor = s_keyInfoMap.find(keyName);
    if (itor != s_keyInfoMap.end())
    {
        keyInfo = itor->second;
    }

    return (itor != s_keyInfoMap.end());
}

//////////////////////////////////////////////////////////////////////////

void SdkFilePropKeyHelper::InitPropertyKey()
{
    static PROPERTYKEYINFO szKeyInfos[] = 
    {
        { SYSTEM_CORE_AUTHOR,                TYPE_STRING,    PKEY_Author },
        { SYSTEM_CORE_COMMENT,               TYPE_STRING,    PKEY_Comment },
        { SYSTEM_CORE_COMPANY,               TYPE_STRING,    PKEY_Company },
        { SYSTEM_CORE_COPYRIGHT,             TYPE_STRING,    PKEY_Copyright },
        { SYSTEM_CORE_DATECREATED,           TYPE_DATETIME,  PKEY_DateCreated },
        { SYSTEM_CORE_DATEMODIFIED,          TYPE_DATETIME,  PKEY_DateModified },
        { SYSTEM_CORE_DATEACCESSED,          TYPE_DATETIME,  PKEY_DateAccessed },
        { SYSTEM_CORE_ITEMTYPE,              TYPE_STRING,    PKEY_ItemType },
        { SYSTEM_CORE_ITEMTYPETEXT,          TYPE_STRING,    PKEY_ItemTypeText },
        { SYSTEM_CORE_RATING,                TYPE_UINT32,    PKEY_Rating },
        { SYSTEM_CORE_RATINGTEXT,            TYPE_STRING,    PKEY_RatingText },
        { SYSTEM_CORE_TITLE,                 TYPE_STRING,    PKEY_Title },
        { SYSTEM_CORE_TOTALFILESIZE,         TYPE_UINT64,    PKEY_TotalFileSize },
        { SYSTEM_CORE_FILEALLOCATIONSIZE,    TYPE_UINT64,    PKEY_FileAllocationSize },
        { SYSTEM_CORE_SIZE,                  TYPE_UINT64,    PKEY_Size },
        { SYSTEM_CORE_DISPLAYNAME,           TYPE_STRING,    PKEY_Identity_DisplayName },
        { SYSTEM_CORE_FILENAME,              TYPE_STRING,    PKEY_FileName },
        { SYSTEM_CORE_ITEMPATHDISPLAY,       TYPE_STRING,    PKEY_ItemPathDisplay },
        { SYSTEM_CORE_PARSINGPATH,           TYPE_STRING,    PKEY_ParsingPath },
        { SYSTEM_CORE_DESCRIPTION,           TYPE_STRING,    PKEY_FileDescription },
        { SYSTEM_CORE_KEYWORDS,              TYPE_STRING,    PKEY_Keywords },
        { SYSTEM_CORE_FOLDERPATH,            TYPE_STRING,    PKEY_ItemFolderPathDisplay },

        { SYSTEM_LINK_ITEMURL,               TYPE_STRING,    PKEY_Link_TargetUrl},

        { SYSTEM_MUSIC_ALBUMARTIST,          TYPE_STRING,    PKEY_Music_AlbumArtist },
        { SYSTEM_MUSIC_ALBUMTITLE,           TYPE_STRING,    PKEY_Music_AlbumTitle },
        { SYSTEM_MUSIC_ALBUMID,              TYPE_STRING,    PKEY_Music_AlbumID },
        { SYSTEM_MUSIC_ARTIST,               TYPE_STRING,    PKEY_Music_Artist },
        { SYSTEM_MUSIC_TRACKNUMBER,          TYPE_UINT32,    PKEY_Music_TrackNumber },
        { SYSTEM_MUSIC_GENRE,                TYPE_STRING,    PKEY_Music_Genre },
        { SYSTEM_MUSIC_MOOD,                 TYPE_STRING,    PKEY_Music_Mood  },
        { SYSTEM_MUSIC_LYRICS,               TYPE_STRING,    PKEY_Music_Lyrics },
        { SYSTEM_MUSIC_PERIOD,               TYPE_STRING,    PKEY_Music_Period },
        { SYSTEM_MUSIC_DISPLAYARTIST,        TYPE_STRING,    PKEY_Music_DisplayArtist },

        { SYSTEM_MEDIA_DURATION,             TYPE_UINT64,    PKEY_Media_Duration },
        { SYSTEM_MEDIA_WRITER,               TYPE_STRING,    PKEY_Media_Writer },
        { SYSTEM_MEDIA_YEAR,                 TYPE_UINT32,    PKEY_Media_Year },
        { SYSTEM_MEDIA_PUBLISHER,            TYPE_STRING,    PKEY_Media_Publisher },

        { SYSTEM_PHOTO_DATETAKEN,            TYPE_DATETIME,  PKEY_Photo_DateTaken },
        { SYSTEM_PHOTO_ISOSPEED,             TYPE_UINT16,    PKEY_Photo_ISOSpeed },
        { SYSTEM_PHOTO_ORIENTATION,          TYPE_UINT16,    PKEY_Photo_Orientation },
        { SYSTEM_PHOTO_TAGVIEWAGGREGATE,     TYPE_STRING,    PKEY_Photo_TagViewAggregate },

        { SYSTEM_IMAGE_BITDEPTH,             TYPE_UINT32,    PKEY_Image_BitDepth },
        { SYSTEM_IMAGE_HORIZONTALSIZE,       TYPE_UINT32,    PKEY_Image_HorizontalSize },
        { SYSTEM_IMAGE_VERTICALSIZE,         TYPE_UINT32,    PKEY_Image_VerticalSize },
        { SYSTEM_IMAGE_VERTICALRESOLUTION,   TYPE_DOUBLE,    PKEY_Image_VerticalResolution },
        { SYSTEM_IMAGE_DIMENSIONS,           TYPE_STRING,    PKEY_Image_Dimensions },
        { SYSTEM_IMAGE_COMPRESSION,          TYPE_UINT16,    PKEY_Image_Compression },

        { SYSTEM_VIDEO_COMPRESSION,          TYPE_STRING,    PKEY_Video_Compression },
        { SYSTEM_VIDEO_FRAMERATE,            TYPE_UINT32,    PKEY_Video_FrameRate },
        { SYSTEM_VIDEO_FRAMEWIDTH,           TYPE_UINT32,    PKEY_Video_FrameWidth },
        { SYSTEM_VIDEO_FRAMEHEIGHT,          TYPE_UINT32,    PKEY_Video_FrameHeight },
        { SYSTEM_VIDEO_HORIZONTALASPECTRATIO,TYPE_UINT32,    PKEY_Video_HorizontalAspectRatio},
    };

    int keyInfoSize = ARRAYSIZE(szKeyInfos);

    for (int i = 0; i < keyInfoSize; ++i)
    {
        PROPERTYKEYINFO &keyInfo = szKeyInfos[i];
        s_keyInfoMap.insert(KeyInfoPair(keyInfo.keyName, keyInfo));
    }
}