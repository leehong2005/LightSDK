/*!
* @file D2DAnimatedGif.h 
* 
* @brief This file defines methods for operating GIF.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/03
*/

#ifdef __cplusplus
#ifndef _D2DANIMATEDGIF_H_
#define _D2DANIMATEDGIF_H_

#include "CommonInclude.h"
#include "D2DDevice.h"
#include "D2DResource.h"
#include "ID2DDeviceStateChange.h"

/*!
* @brief This file is a wrapper of WICAnimatedGif class in order to make it associates with D2D.
*/
class CLASS_DECLSPEC D2DAnimatedGif : public D2DResource, public ID2DDeviceStateChange
{
public:

    /*!
    * @brief The default constructor function.
    */
    D2DAnimatedGif();

    /*!
    * @brief The default destructor function.
    */
    ~D2DAnimatedGif();

    /*!
    * @brief Initialize current object, before using, you should call this method.
    *
    * @param pD2DDevice     [I/ ] The pointer to D2DDevice object.
    *
    * @return TRUE is success, FALSE is failure.
    */
    BOOL Initialize(D2DDevice *pD2DDevice);

    /*!
    * @brief Initialize current object, before using, you should call this method.
    *
    * @param pRenderTarget  [I/ ] The pointer to ID2D1RenderTarget object.
    *
    * @return TRUE is success, FALSE is failure.
    */
    BOOL Initialize(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Indicates an object of D2DBitmap whether has been initialized.
    *
    * @return TRUE if initialized, otherwise is FALSE.
    */
    BOOL HasInitialized();

    /*!
    * @brief Load a image from a specified file name.
    *
    * @param lpfile      [I/ ] The pointer to file name.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL LoadFromFile(LPCWSTR lpfile);

    /*!
    * @brief Load a image from a resource.
    *
    * @param uResId      [I/ ] The identifier of resource.
    * @param hModule     [I/ ] Indicates thats load image from.which module, default value is NULL.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL LoadFromResource(UINT uResId, HMODULE hModule = NULL);

    /*!
    * @brief Set the image type, which indicates which image can be processed by current class.
    *
    * @param type           [I/ ] The image type.
    */
    void SetImageType(WIC_GIF_TYPE type);

    /*!
    * @brief Get the resource type.
    *
    * @return WIC_GIF_TYPE value.
    */
    WIC_GIF_TYPE GetImageType();

    /*!
    * @brief Paint next frame of GIF. The frame is drawn on compatible render target.
    *
    * @return TRUE if succeeds, FALSE if fail.
    */
    BOOL OnPaintNextFrame();

    /*!
    * @brief Get paint bitmap drawn on compatible render target.
    *
    * @param ppD2DBitmap    [I/ ] The output ID2D1Bitmap instance, you should release if you no longer use it.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetPaintBitmap(ID2D1Bitmap **ppD2DBitmap);

    /*!
    * @brief Get the delay of current frame.
    *
    * @return The value of delay time.
    */
    UINT GetFrameDelay();

    /*!
    * @brief Get the frame count.
    *
    * @return The frame count.
    */
    UINT GetFrameCount();

    /*!
    * @brief Get size of GIF.
    *
    * @param uWidth     [ /O] The width of GIF.
    * @param uHeight    [ /O] The height of GIF.
    */
    void GetSize(OUT UINT &uWidth, OUT UINT &uHeight);

    /*!
    * @brief Set the color for clearing background.
    *
    * @param color      [I/ ] The color.
    */
    void SetClearColor(const ColorF &color);

    /*!
    * @brief Set the next frame index.
    *
    * @param uNextFrameIndex    [I/ ] The next frame index.
    */
    void SetFrameIndex(UINT uNextFrameIndex);

    /*!
    * @brief Get the current frame index of GIF.
    *
    * @return The frame index.
    */
    UINT GetFrameIndex();

    /*!
    * @brief Indicates whether end animation or not.
    *
    * @return TRUE if should end animation, otherwise return FALSE.
    */
    BOOL IsEndAnimation();

protected:

    /*!
    * @brief Determine clear background or not.
    *
    * @param uFrameDisposal         [I/ ] The disposal value, got from GIF frame.
    */
    BOOL DisposeCurrentFrame(UINT uFrameDisposal);

    /*!
    * @brief Clear current frame area with GIF's background.
    *
    * @param isUsingCustomColor     [I/ ] Indicates whether using custom color.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL ClearCurrentFrameArea(BOOL isUsingCustomColor = TRUE);

    /*!
    * @brief Get an instance of ID2D1Bitmap, which can be used to draw with render target.
    *        you should loop to call this method to make the GIF animates.
    *
    * @param ppD2DBitmap    [ /O] The pointer to pointer to ID2D1Bitmap.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetNextFrame(OUT ID2D1Bitmap **ppD2DBitmap);

    /*!
    * @brief Get an instance of ID2D1Bitmap, which can be used to draw with render target.
    *        you should loop to call this method to make the GIF animates.
    *
    * @param ppD2DBitmap    [ /O] The pointer to pointer to ID2D1Bitmap.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetFrameBitmap(OUT ID2D1Bitmap **ppD2DBitmap);

    /*!
    * @brief Get a bitmap at specified frame index.
    *
    * @param uFrameIndex    [I/ ] The frame index.
    * @param ppD2DBitmap    [ /O] The pointer to pointer to ID2D1Bitmap.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetFrameAt(UINT uFrameIndex, ID2D1Bitmap **ppD2DBitmap);

    /*!
    * @brief Get bitmap bound of current frame.
    *
    * @param rc             [ /O] Reference to D2D1_RECT_F.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetFrameRect(OUT D2D1_RECT_F& rc);

    /*!
    * @brief Get the current background color of GIF.
    *
    * @return The D2D1_COLOR_F object.
    */
    D2D1_COLOR_F GetBackgroundColor();

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

private:

    D2D1::ColorF             m_bkColor;                 // The background color.
    ID2D1Bitmap             *m_pD2DBitmap;              // Current frame bitmap.
    ID2D1RenderTarget       *m_pRenderTarget;           // The render target.
    WICAnimatedGif          *m_pWicAnimatedGif;         // WICAnimatedGif instance to process GIF with WIC.
    ID2D1BitmapRenderTarget *m_pBitmapRenderTarget;     // The compatible render target.
};

#endif // _D2DANIMATEDGIF_H_
#endif // __cplusplus

