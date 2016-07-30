/*!
* @file  SdkOverlapView.h
* 
* @brief This file defines the header file of the overlapView module.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Guo Jianbin
* @date 2011/01/04
*/

#ifdef __cplusplus
#ifndef _SDKOVERLAPVIEW_H_
#define _SDKOVERLAPVIEW_H_

#include "SdkViewElement.h"
#include "SdkCommonInclude.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The SdkOverlapView is used to show overlapped images.
*/
class CLASS_DECLSPEC SdkOverlapView : public SdkViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkOverlapView();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkOverlapView();

    /*!
    * @brief Clear the image collection.
    */
    virtual void ClearBitmap();

    /*!
    * @brief Set style for overlap view.
    *
    * @param style          [I/ ] The style for overlay view. The style include 
    * OVERLAP_STYLE_NONE, OVERLAP_STYLE_FIRST, OVERLAP_STYLE_SECOND, OVERLAP_STYLE_TRAPEZIA.
    */
    virtual void SetStyle(OVERLAP_STYLE style);

    /*!
    * @brief Clear the image from images list at specified index.
    *
    * @param index          {I/ ] The index of image to be clear.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    virtual BOOL RemoveImage(UINT32 index);

    /*!
    * @ brief Set the overlap image, from specified file.
    *
    * @param lpfileName     [I/ ] The path of image file.
    * @param uDestWidth     [I/ ] The destination width.
    * @param uDestHeight    [I/ ] The destination height.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    *
    * @remark The uDestWidth and uDestHeight default value is 0,
    *         that means destination bound is original bound.
    */
    virtual BOOL SetImage(IN LPCWSTR lpfileName, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @ brief Set the overlap image, from handler to HBITMAP.
    *
    * @param hBitmap        [I/ ] The handler to HBITMAP.
    * @param uDestWidth     [I/ ] The destination width.
    * @param uDestHeight    [I/ ] The destination height.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    *
    * @remark The uDestWidth and uDestHeight default value is 0,
    *         that means destination bound is original bound.
    */
    virtual BOOL SetImage(IN HBITMAP hBitmap, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @ brief Set the overlap image, from resource.
    *
    * @param uResId         [I/ ] The resource id.
    * @param hModule        [I/ ] The address of where the resource is loaded from.
    * @param uDestWidth     [I/ ] The destination width.
    * @param uDestHeight    [I/ ] The destination height.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    *
    * @remark The uDestWidth and uDestHeight default value is 0,
    *         that means destination bound is original bound.
    */
    virtual BOOL SetImage(IN UINT32 uResId, IN HMODULE hModule = NULL, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Set the margin of the view.
    *
    * @param left       [I/ ] The left margin.
    * @param top        [I/ ] The top margin.
    * @param right      [I/ ] The right margin.
    * @param bottom     [I/ ] The bottom margin.
    */
    virtual void SetImageMargin(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

    /*!
    * @brief Set the margin of the view.
    *
    * @param margin     [I/ ] The margin for four direction.
    */
    virtual void SetImageMargin(FLOAT margin);

protected:

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Called when the layout of view is changed.
    *
    * @param fChanged       [I/ ] Indicates whether is changed.
    * @param left           [I/ ] The left value.
    * @param top            [I/ ] The top value.
    * @param width          [I/ ] The width of view.
    * @param height         [I/ ] The height of view.
    */
    virtual void OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

    /*!
    * @brief Called when device state change, typically the render target is recreated.
    *
    * @param pDevice    [I/ ] The D2DDevice pointer.
    * @param stateVal   [I/ ] This value indicates which state change just occurred.
    *
    * @remark These classes which are dependent with render target must be override this method,
    *         because some render resource are created with specified render target, once the
    *         render target used to create render resource is invalid or recreated, these all render resource
    *         must be recreated or re-initialize. Render resource includes Brush, Bitmap, Layer, Mesh.
    */
    virtual void OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal);

    /*!
    * @brief Called when device state change, typically the render target is resize.
    *
    * @param pDevice    [I/ ] The D2DDevice pointer.
    * @param stateVal   [I/ ] This value indicates which state change just occurred.
    *
    * @remark These classes which are dependent with render target must be override this method,
    *         because some render resource are created with specified render target, once the
    *         render target used to create render resource is invalid or recreated, these all render resource
    *         must be recreated or re-initialize. Render resource includes Brush, Bitmap, Layer, Mesh.
    */
    virtual void OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal);

    /*!
    * @brief Draw overlap stuffs on compatible render target..
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    * @param rect           [I/ ] The drawing bound.
    */
    virtual void OnDrawOverlap(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F &rect);

    /*!
    * @brief Combine rotate, translate, scale, skew matrices to a new matrix.
    *
    * @param transform      [I/ ] The transform for rotate, translate, scale and skew.
    *
    * @return Combined matrix.
    */
    Matrix3x2F CombineMatrix(IN const OVERLAP_TRANSFORMINFO& transform);

    /*!
    * @brief Set style to none.
    */
    void SetupNoneStyle();

    /*!
    * @brief Set default first style.
    */
    void SetupFirstStyle();

    /*!
    * @brief Set default second style.
    */
    void SetupSecondStyle();

    /*!
    * @brief Set default third style.
    */
    void SetupThirdStyle();

    /*!
    * @brief Create a compatible target.
    *
    * @param pRenderTarget  [I/ ] The render target.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    virtual BOOL CreateCompatibleTarget(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Set the overlap view max or min layout information.
    */
    void SetupOverlapLayout();

    /*!
    * @brief Set the bitmap rect.
    * 
    * @param rect           [I/ ] The bitmap rect.
    */
    D2D1_RECT_F SetBitmapRect(const D2D1_RECT_F &rect);

protected:

    FLOAT                       m_leftMargin;             // The left margin value.
    FLOAT                       m_topMaragin;             // The top margin value.
    FLOAT                       m_rightMargin;            // The right margin value.
    FLOAT                       m_bottomMargin;           // The bottom margin value.
    D2D1_SIZE_F                 m_preSize;                // The previous view size.
    OVERLAP_STYLE               m_overlayStyle;           // The overlap view style.
    vector<D2DBitmap*>          m_vctBitmaps;             // D2DBitmap array collection.
    OVERLAP_TRANSFORMINFO       m_szOverlapTransforms[3]; // The transform information.
    ID2D1BitmapRenderTarget    *m_pBitmapRenderTarget;    // The bitmap render target.
};

END_NAMESPACE_VIEWS

#endif // _SDKOVERLAPVIEW_H_
#endif // __cplusplus
