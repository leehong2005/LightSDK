
/*!
* @file IImagePreviewUpdateHandler.h 
* 
* @brief This file defines the event for Image preview layout.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Liu Qiao
* @date 2011/2/12
*/

#ifdef __cplusplus
#ifndef _IIMAGEPREVIEWUPDATEHANDLER_H_
#define _IIMAGEPREVIEWUPDATEHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief The IImagePreviewUpdateHandler defines the event for SdkImagePreviewLayout.
*/
class IImagePreviewUpdateHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IImagePreviewUpdateHandler() {};

    /*!
    * @brief Update image view.
    *
    * @param view   [I/ ] The pointer ViewElement, you should NOT delete the pointer.
    * @param bNext  [I/ ] Next or last
    */
    virtual BOOL UpdateImageView(SdkImagePreviewLayout* imagePreview, BOOL bNext) = 0;

    /*!
    * @brief Called when switching image.
    *
    * @param view   [I/ ] The pointer ViewElement, you should NOT delete the pointer.
    */
    virtual void OnImageSwitchEvent(SdkImagePreviewLayout* imagePreview) = 0;

    /*!
    * @brief Called when get surplus time.
    *
    * @param dSurplusTime   [I/ ] The time that has surplus since the storyboard started playing.
    */
    virtual void GetSurplusTime(DOUBLE &dSurplusTime) = 0;

    /*!
    * @brief Called when auto play.
    */
    virtual void OnAutoPlay() = 0;

    /*!
    * @brief Called when bottom animation in.
    *
    * @param nMoveIndex  [I/ ] The move index.
    * @param bBottomIn   [I/ ] The bottom in.
    */
    virtual void OnBottomAnimationIn(INT32 nMoveIndex, BOOL bBottomIn) = 0;

    /*!
    * @brief Request Image size.
    *
    * @param lpFilePath   [I/ ] The file path.
    * @param nWidth       [I/ ] The width.
    */
    virtual void RequestImageSize(LPCTSTR lpFilePath, OUT INT32 &nWidth, OUT INT32 &nHeight) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _IIMAGEPREVIEWUPDATEHANDLER_H_
#endif // __cplusplus
