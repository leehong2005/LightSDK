/*!
* @file SdkImageView.h
* 
* @brief This file defines the header file of the SdkImageView module.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _SDKIMAGEVIEW_H_
#define _SDKIMAGEVIEW_H_

#include "SdkViewElement.h"
#include "SdkUICommon.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief SdkImageView class.
*/
class CLASS_DECLSPEC SdkImageView : public virtual SdkViewElement
{
public:

    /*!
    * @brief The constructor.
    */
    SdkImageView();

    /*!
    * @brief The destructor.
    */
    virtual ~SdkImageView();

    /*!
    * @brief Called to get the image from files, which will be set as the background.
    *
    * @param lpFile     [I/ ] The path of the image.
    *
    * @return TRUE if success, otherwise return FALSE.
    */
    virtual BOOL SetSrcImage(IN LPCWSTR lpFile, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Called to get the image from files, which will be set as the background.
    *
    * @param pD2DBitmap [I/ ] The path of the image.
    *
    * @return TRUE if success, otherwise return FALSE.
    */
    virtual BOOL SetSrcImage(IN D2DBitmap *pD2DBitmap);

    /*!
    * @brief Set the view's background image, from a handle to HBITMAP.
    *
    * @param hBitmap    [I/ ] Handler to HBITMAP.
    *
    * @return TRUE if success, otherwise return FALSE.
    */
    virtual BOOL SetSrcImage(IN HBITMAP hBitmap, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Called to get the image from resource, which will be set as the background.
    *
    * @param resId      [I/ ] The id of image.
    * @param hModule    [I/ ] The HINSTANCE, maybe NULL.
    *
    * @return TRUE if success, otherwise return FALSE.
    */
    virtual BOOL SetSrcImage(IN UINT resId, IN HMODULE hModule = NULL, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Called to set stretch or not.
    *
    * @param stretchMode    [I/ ] IMAGE_STRETCH_MODE mode value.
    */
    virtual void SetStretchMode(IN IMAGE_STRETCH_MODE stretchMode = IMAGE_STRETCH_MODE_CENTER);

    /*!
    * @brief Called to set the margin of fore image and background image.
    * 
    * @param margin         [I/ ] The around margin of fore image and background.
    */
    virtual void SetImageMargin(IN FLOAT margin);

    /*!
    * @brief Called to set the margin of fore image and background image.
    * 
    * @param left           [I/ ] The left margin.
    * @param top            [I/ ] The top margin.
    * @param right          [I/ ] The right margin.
    * @param bottom         [I/ ] The bottom margin.
    */
    virtual void SetImageMargin(IN FLOAT left, IN FLOAT top, IN FLOAT right, IN FLOAT bottom);

    /*!
    * @brief Clear the associated data with the view.
    */
    virtual void ClearAssocData();

protected:

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Get the rectangle of drawing bitmap.
    *
    * @param viewRc         [I/ ] The view rectangle.
    * @param pBitmap        [I/ ] The bitmap to be drawing.
    *
    * @return D2D1_RECT_F object.
    */
    D2D1_RECT_F GetDrawRect(const D2D1_RECT_F& viewRc, D2DBitmap *pBitmap);

    /*!
    * @brief Call this method to keep source image fit in destination rectangle.
    * 
    * @param destWidth      [I/ ] destination rectangle's width.
    * @param destHeight     [I/ ] destination rectangle's height.
    * @param srcWidth       [I/O] source image's width.
    * @param srcHeight      [I/O] source image's height.
    */
    virtual void ConvertToFitMode(IN FLOAT destWidth,IN FLOAT destHeight, IN OUT FLOAT& srcWidth, IN OUT FLOAT& srcHeight);

private:

    /*!
    * @brief The internal data of image view.
    */
    struct _IMAGEVIEW_INTERNALDATA;

    _IMAGEVIEW_INTERNALDATA *m_lpImageViewData;     // The image view's data.
};

END_NAMESPACE_VIEWS

#endif // _SDKIMAGEVIEW_H_
#endif // __cplusplus

