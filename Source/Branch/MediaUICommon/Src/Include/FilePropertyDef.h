/*!
* @file FilePropertyDef.h
* 
* @brief.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/27
*/

#ifdef __cplusplus
#ifndef _FILEPROPERTY_H_
#define _FILEPROPERTY_H_

#include "Common.h"
#include "CommonMacro.h"
#include "QueryCondition.h"

BEGIN_NAMESPACE_COMMON

#define  INTERNET_MAX_URL  2048

/*!
* @brief The file property type.
*/
typedef enum _FILEPROPTYPE
{
    FILEPROPTYPE_UNKNOWN  = 0,
    FILEPROPTYPE_MUSIC    = 1,
    FILEPROPTYPE_VIDEO    = 2,
    FILEPROPTYPE_PICTURE  = 3,
    FILEPROPTYPE_LINK     = 4,

} FILEPROPTYPE;

/*!
* @brief The basic file property.
*/
struct FilePropInfo 
{
    FILEPROPTYPE    eFilePropType;              // The file property type.
    TCHAR           szExternalPath[MAX_PATH];   // The external path, may be empty is media is on local disk.
    TCHAR           szComment[MAX_PATH];        // Comment.
    TCHAR           szCompany[MAX_PATH];        // Company.
    TCHAR           szFilePath[MAX_PATH];       // File full path.
    TCHAR           szFileName[MAX_PATH];       // File name.
    TCHAR           szTitle[MAX_PATH];          // Title property.
    TCHAR           szTags[MAX_PATH];           // Tags property.
    TCHAR           szDescription[MAX_PATH];    // Description property
    TCHAR           szItemType[MAX_PATH];       // Item type 
    TCHAR           szRatingText[MAX_PATH];     // Rating text.
    TCHAR           szFileSize[MAX_PATH];       // File Size for display.
    TCHAR           szDuration[MAX_PATH];       // Duration for display.
    FILETIME        ftDateCreated;              // Create date.
    FILETIME        ftDateModified;             // Modified date.
    FILETIME        ftDateAccessed;             // Accessed date.
    UINT64          uDuration;                  // Duration.
    UINT64          uFileSize;                  // File size.
    UINT32          uItemRating;                // Rating.
};

/*!
* @brief The music file property.
*/
struct MusicFilePropInfo : public FilePropInfo
{
    TCHAR     szAlbum[MAX_PATH];                // Album information.
    TCHAR     szAlbumAartist[MAX_PATH];         // Album artist.
    TCHAR     szContributeArtist[MAX_PATH];     // Contribute Artist.
    TCHAR     szGenre[MAX_PATH];                // Genre.
    TCHAR     szYear[MAX_PATH];                 // Year.
};

/*!
* @brief The picture file property.
*/
struct PictureFilePropInfo : public FilePropInfo
{
    UINT32      uHorizontalSize;                // Horizontal size.
    UINT32      uVerticalSize;                  // Vertical size.
    UINT16      uOrientation;                   // Orientation
    UINT16      uISOSpeed;                      // ISO speed.
    FILETIME    ftDateTaken;                    // Data taken.
    TCHAR       szOrientationText[MAX_PATH];    // Orientation text.
    TCHAR       szDimensions[MAX_PATH];         // Dimension text.
};

/*!
* @brief The video file property.
*/
struct VideoFilePropInfo : public FilePropInfo
{
    UINT64      uLength;                        // Video length.
    UINT32      uFrameWidth;                    // Frame width.
    UINT32      uFrameHeight;                   // Frame height.
    UINT32      uFrameRatio;                    // Frame ratio.
    UINT32      uHorizontalAspectRatio;         // Aspect ratio
    UINT32      uVerticalAspectRatio;           // Aspect ratio
};

/*!
* @brief The link file property.
*/
struct LinkFilePropInfo : public FilePropInfo
{
    TCHAR       szItemUrl[INTERNET_MAX_URL];    // Item Url.
    TCHAR       szThumbnailPath[MAX_PATH];      // Thumbnail path.
    TCHAR       szTitleName[MAX_PATH];          // Title name.
    BOOL        isDeleted;                      // whether deleted.
};

END_NAMESPACE

#endif // _FILEPROPERTY_H_
#endif // __cplusplus
