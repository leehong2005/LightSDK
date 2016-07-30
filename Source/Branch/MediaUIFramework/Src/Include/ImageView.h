/*!
* @file ImageView.h
* 
* @brief This file defines the header file of the ImageView module.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Cui Shuyan
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _IMAGEVIEW_H_
#define _IMAGEVIEW_H_

#include <wincodec.h>
#include "ViewElement.h"
#include "D2DBitmap.h"
#include "D2DRectUtility.h"

/*!
* @brief Image stretch mode.
*/
typedef enum _IMAGE_STRETCH_MODE
{
    IMAGE_STRETCH_MODE_FILL         = 0,            // The image fill whole view.
    IMAGE_STRETCH_MODE_CENTER       = 1,            // The image is in center.
    IMAGE_STRETCH_MODE_MARGIN       = 2,            // The image can set margin with background.
    IMAGE_STRETCH_MODE_FIT          = 3,            // The image can keep origin aspect ratio.
    IMAGE_STRETCH_MODE_FIT_ONTOP    = 4,            // 
    IMAGE_STRETCH_MODE_FIT_ONRIGHT  = 5,            // 
    IMAGE_STRETCH_MODE_FIT_ONBOTTOM = 6,            // 
    IMAGE_STRETCH_MODE_FIT_ONLEFT   = 7,            // 

} IMAGE_STRETCH_MODE;

/*!
* @brief ImageView class.
*/
class CLASS_DECLSPEC ImageView : public virtual ViewElement
{
public:

    /*!
    * @brief The constructor.
    */
    ImageView();

    /*!
    * @brief The destructor.
    */
    virtual ~ImageView();

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
    virtual void SetImageStretchMode(IN IMAGE_STRETCH_MODE stretchMode = IMAGE_STRETCH_MODE_CENTER);

    /*!
    * @brief Called to set the margin of fore image and background image.
    * 
    * @param margin         [I/ ] The around margin of fore image and background.
    */
    virtual void SetSrcImageMargin(IN FLOAT margin);

    /*!
    * @brief Called to set the margin of fore image and background image.
    * 
    * @param left           [I/ ] The left margin.
    * @param top            [I/ ] The top margin.
    * @param right          [I/ ] The right margin.
    * @param bottom         [I/ ] The bottom margin.
    */
    virtual void SetSrcImageMargin(IN FLOAT left, IN FLOAT top, IN FLOAT right, IN FLOAT bottom);

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
    * @param pID2D1Bitmap   [I/ ] The bitmap to be drawing.
    *
    * @return D2D1_RECT_F object.
    */
    D2D1_RECT_F GetDrawRect(const D2D1_RECT_F& viewRc, ID2D1Bitmap *pID2D1Bitmap);

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

    D2DBitmap               *m_pSrcD2DBitmap;       // The pointer which points to the object of D2DBitmap.
    IMAGE_STRETCH_MODE       m_stretchMode;         // The flag whether to Stretch the bitmap to fill all view.
    FLOAT                    m_leftMargin;          // The left margin of src image to background.
    FLOAT                    m_topMargin;           // The top margin of src image to background.
    FLOAT                    m_rightMargin;         // The right margin of src image to background.
    FLOAT                    m_bottomMargin;        // The bottom margin of src image to background.
};


#endif // _IMAGEVIEW_H_
#endif // __cplusplus

