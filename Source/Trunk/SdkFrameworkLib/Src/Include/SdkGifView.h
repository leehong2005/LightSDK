/*!
* @file SdkGifView.h 
* 
* @brief This file SdkGifView class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _SDKANIMATEDGIFVIEW_H_
#define _SDKANIMATEDGIFVIEW_H_

#include "SdkViewElement.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The image file type, gif, or image.
*/
typedef enum _IMAGEFILE_TYPE
{
    IMAGEFILE_TYPE_UNKNOWN  = 0x0000,       // Unknown type.
    IMAGEFILE_TYPE_IMAGE    = 0x0001,       // Image type.
    IMAGEFILE_TYPE_GIF      = 0x0002,       // Gif type.

} IMAGEFILE_TYPE;

/*!
* @brief The SdkGifView class is used to display GIF file.
*/
class CLASS_DECLSPEC SdkGifView : public SdkViewElement
{
public:

    /*
    * @brief The constructor function.
    */
    SdkGifView();

    /*
    * @brief The destructor function.
    */
    virtual ~SdkGifView();

    /*!
    * @brief Start animation of GIF.
    */
    virtual void Start();

    /*!
    * @brief Pause animation of GIF.
    */
    virtual void Pause();

    /*!
    * @brief Stop animation of GIF.
    */
    virtual void Stop();

    /*!
    * @brief Check whether the GIF is playing.
    *
    * @return TRUE if playing, otherwise return FALSE.
    */
    virtual BOOL IsPlaying();

    /*!
    * @brief Set the flag to indicates whether start animation automatically.
    *
    * @param fAutoStart  [I/ ] Flag to indicates whether start animation automatically.
    */
    virtual void SetAutoStart(BOOL fAutoStart = TRUE);

    /*!
    * @brief Load a image from a specified file name.
    *
    * @param lpfile      [I/ ] The pointer to file name.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    virtual BOOL LoadFromFile(LPCWSTR lpfile);

    /*!
    * @brief Load a image from a resource.
    *
    * @param uResId      [I/ ] The identifier of resource.
    * @param hModule     [I/ ] Indicates thats load image from.which module, default value is NULL.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    virtual BOOL LoadFromResource(UINT uResId, HMODULE hModule = NULL);

    /*!
    * @brief Set the image type, which indicates which image can be processed by current class.
    *
    * @param type           [I/ ] The image type.
    */
    virtual void SetImageType(WIC_GIF_TYPE type);

    /*!
    * @brief Get the resource type.
    *
    * @return WIC_GIF_TYPE value.
    */
    virtual WIC_GIF_TYPE GetImageType();

    /*!
    * @brief Get the type of image file or image resource.
    *
    * @return One of IMAGEFILE_TYPE enumerate.
    */
    virtual IMAGEFILE_TYPE GetImageFileType();

    /*!
    * @brief Auto adjust the size according to the GIF file.
    *
    * @param isAutoSize  [I/ ] TRUE is auto adjust size, default is FALSE.
    */
    virtual void AutoAdjustSize(BOOL isAutoSize);

    /*!
    * @brief Set the layout information of the view.
    *
    * @param x          [I/ ] The x coordinate of left.
    * @param y          [I/ ] The y coordinate of left.
    * @param width      [I/ ] The width coordinate of left.
    * @param height     [I/ ] The height coordinate of left.
    */
    virtual void SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height);

    /*!
    * @brief Called to set stretch or not.
    *
    * @param stretchMode    [I/ ] IMAGE_STRETCH_MODE mode value.
    */
    virtual void SetStretchMode(IN IMAGE_STRETCH_MODE stretchMode = IMAGE_STRETCH_MODE_CENTER);

    /*!
    * @brief Clear the associated data with the view.
    */
    virtual void ClearAssocData();

protected:

    /*!
    * @brief Get the rectangle of drawing bitmap.
    *
    * @param viewRc         [I/ ] The view rectangle.
    * @param pID2D1Bitmap   [I/ ] The bitmap to be drawing.
    *
    * @return D2D1_RECT_F object.
    */
    virtual D2D1_RECT_F GetDrawRect(const D2D1_RECT_F& viewRc, ID2D1Bitmap *pID2D1Bitmap);

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
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Paint the GIF frame.
    *
    * @param fStartTimer    [I/ ] Start timer or not.
    */
    virtual void OnPaintFrame(BOOL fStartTimer = TRUE);

    /*!
    * @brief Timer call back procedure.
    *
    * @param hwnd       [I/ ] Handle to the window associated with the timer.
    * @param uMsg       [I/ ] Specifies the WM_TIMER message.
    * @param idEvent    [I/ ] Specifies the timer's identifier.
    * @param dwTime     [I/ ] Specifies the number of milliseconds that have elapsed since the system was started.
    */
    static void CALLBACK OnTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

protected:

    /*
    * @brief The UINT_PTE to SdkGifView* map.
    */
    typedef map<UINT_PTR, SdkGifView*>     TimerIDGifViewMap;

    /*!
    * @brief The internal data of seek bar.
    */
    struct _GIFVIEW_INTERNALDATA;

    _GIFVIEW_INTERNALDATA      *m_pGifViewData;         // The Gif view internal data.
    static UINT_PTR             s_uTimerID;             // The static timer id.
    static TimerIDGifViewMap    s_timerIdGifViewMap;    // The timer id to SdkGifView pointer map.
};

END_NAMESPACE_VIEWS

#endif // _SDKANIMATEDGIFVIEW_H_
#endif // __cplusplus

