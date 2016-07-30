/*!
* @file MediaProperty.h 
* 
* @brief This file defines property key names and key types.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/11
*/

#ifdef __cplusplus
#ifndef _MEDIAPROPERTY_H_
#define _MEDIAPROPERTY_H_

#include "CommonMacro.h"

BEGIN_NAMESPACE_COMMON

/*!
* @brief The friendly key type.
*/
typedef enum _PROPERTYKEYTYPE
{
    TYPE_STRING                  = 1,
    TYPE_UINT16                  = 2,
    TYPE_DOUBLE                  = 3,
    TYPE_INT32                   = 4,
    TYPE_UINT32                  = 5,
    TYPE_UINT64                  = 6,
    TYPE_DATETIME                = 7,

} PROPERTYKEYTYPE;

END_NAMESPACE


/*!
* @brief The friendly key name.
*/
typedef enum _PROPERTYKEYNAME
{
    SYSTEM_CORE_AUTHOR                      = 1,        // String Author
    SYSTEM_CORE_COMMENT                     = 2,        // String Comment
    SYSTEM_CORE_COMPANY,                                // String Company
    SYSTEM_CORE_COPYRIGHT,                              // String Copyright
    SYSTEM_CORE_DATECREATED,                            // DateTime
    SYSTEM_CORE_DATEMODIFIED,                           // DateTime
    SYSTEM_CORE_DATEACCESSED,                           // DateTime
    SYSTEM_CORE_ITEMTYPE,                               // String
    SYSTEM_CORE_ITEMTYPETEXT,                           // String
    SYSTEM_CORE_RATING,                                 // UInt32
    SYSTEM_CORE_RATINGTEXT,                             // String
    SYSTEM_CORE_TITLE,                                  // String
    SYSTEM_CORE_TOTALFILESIZE,                          // UInt64
    SYSTEM_CORE_FILEALLOCATIONSIZE,                     // UInt64
    SYSTEM_CORE_SIZE,                                   // UInt64
    SYSTEM_CORE_DISPLAYNAME,                            // String
    SYSTEM_CORE_FILENAME,                               // String
    SYSTEM_CORE_ITEMPATHDISPLAY,                        // String
    SYSTEM_CORE_PARSINGPATH,                            // String
    SYSTEM_CORE_DESCRIPTION,                            // String
    SYSTEM_CORE_KEYWORDS,                               // String Tags
    SYSTEM_CORE_FOLDERPATH,                             // String Folder Path.

    SYSTEM_LINK_ITEMURL,                                // String

    SYSTEM_MUSIC_ALBUMARTIST,                           // String Album artist
    SYSTEM_MUSIC_ALBUMTITLE,                            // String Album
    SYSTEM_MUSIC_ALBUMID,                               // String Contributing artists
    SYSTEM_MUSIC_ARTIST,                                // String
    SYSTEM_MUSIC_TRACKNUMBER,                           // UInt32
    SYSTEM_MUSIC_GENRE,                                 // String
    SYSTEM_MUSIC_MOOD,                                  // String
    SYSTEM_MUSIC_LYRICS,                                // String
    SYSTEM_MUSIC_PERIOD,                                // String
    SYSTEM_MUSIC_DISPLAYARTIST,                         // String

    SYSTEM_MEDIA_DURATION,                              // UInt64
    SYSTEM_MEDIA_WRITER,                                // String
    SYSTEM_MEDIA_YEAR,                                  // UInt32
    SYSTEM_MEDIA_PUBLISHER,                             // String

    SYSTEM_PHOTO_DATETAKEN,                             // DatetTime
    SYSTEM_PHOTO_ISOSPEED,                              // UInt16
    SYSTEM_PHOTO_ORIENTATION,                           // UInt16
    SYSTEM_PHOTO_ORIENTATIONTEXT,                       // String
    SYSTEM_PHOTO_TAGVIEWAGGREGATE,                      // String

    SYSTEM_IMAGE_BITDEPTH,                              // UInt32
    SYSTEM_IMAGE_HORIZONTALSIZE,                        // UInt32
    SYSTEM_IMAGE_VERTICALSIZE,                          // UInt32
    SYSTEM_IMAGE_VERTICALRESOLUTION,                    // Double
    SYSTEM_IMAGE_DIMENSIONS,                            // String
    SYSTEM_IMAGE_COMPRESSION,                           // UInt16

    SYSTEM_VIDEO_COMPRESSION,                           // String
    SYSTEM_VIDEO_FRAMERATE,                             // UInt32
    SYSTEM_VIDEO_FRAMEWIDTH,                            // UInt32
    SYSTEM_VIDEO_FRAMEHEIGHT,                           // UInt32
    SYSTEM_VIDEO_HORIZONTALASPECTRATIO,                 // UInt32
    SYSTEM_VIDEO_VERTICALASPECTRATIO,                   // UInt32

} PROPERTYKEYNAME;

#endif // _MEDIAPROPERTY_H_
#endif // __cplusplus