/*!
* @file AnimatedGifView.h 
* 
* @brief This file AnimatedGifView class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _ANIMATEDGIFVIEW_H_
#define _ANIMATEDGIFVIEW_H_

#include "CommonInclude.h"
#include "ViewElement.h"
#include "D2DAnimatedGif.h"
#include "ImageView.h"

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
* @brief The AnimatedGifView class is used to display GIF file.
*/
class CLASS_DECLSPEC AnimatedGifView : public ViewElement
{
public:

    /*
    * @brief The UINT_PTE to AnimatedGifView* map.
    */
    typedef map<UINT_PTR, AnimatedGifView*>     TimerIDGifViewMap;

    /*
    * @brief The UINT_PTE to AnimatedGifView* pair.
    */
    typedef pair<UINT_PTR, AnimatedGifView*>    TImerIDGifViewPair;

    /*
    * @brief The constructor function.
    */
    AnimatedGifView();

    /*
    * @brief The destructor function.
    */
    virtual ~AnimatedGifView();

    /*!
    * @brief Start animation of GIF.
    */
    void Start();

    /*!
    * @brief Pause animation of GIF.
    */
    void Pause();

    /*!
    * @brief Stop animation of GIF.
    */
    void Stop();

    /*!
    * @brief Check whether the GIF is playing.
    *
    * @return TRUE if playing, otherwise return FALSE.
    */
    BOOL IsPlaying();

    /*!
    * @brief Set the flag to indicates whether start animation automatically.
    *
    * @param fAutoStart  [I/ ] Flag to indicates whether start animation automatically.
    */
    void SetAutoStart(BOOL fAutoStart = TRUE);

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
    * @brief Get the type of image file or image resource.
    *
    * @return One of IMAGEFILE_TYPE enumerate.
    */
    IMAGEFILE_TYPE GetImageFileType();

    /*!
    * @brief Auto adjust the size according to the GIF file.
    *
    * @param isAutoSize  [I/ ] TRUE is auto adjust size, default is FALSE.
    */
    void AutoAdjustSize(BOOL isAutoSize);

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
    virtual void SetImageStretchMode(IN IMAGE_STRETCH_MODE stretchMode = IMAGE_STRETCH_MODE_CENTER);

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
    D2D1_RECT_F GetDrawRect(const D2D1_RECT_F& viewRc, ID2D1Bitmap *pID2D1Bitmap);

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

    BOOL                        m_isAutoStart;          // Tell object whether is first frame.
    BOOL                        m_isAutoSize;           // Tell whether auto adjust size or not.
    BOOL                        m_isPlaying;            // Indicates whether is playing.
    UINT                        m_uFrameDelay;          // Frame delay.
    UINT_PTR                    m_curTimerID;           // Current timer id.
    IMAGE_STRETCH_MODE          m_stretchMode;         // The flag whether to Stretch the bitmap to fill all view.
    D2DAnimatedGif             *m_pD2DAnimatedGif;      // The pointer to D2DAnimatedGif.
    ID2D1BitmapRenderTarget    *m_pBitmapRenderTarget;  // The compatible render target.
    static UINT_PTR             s_uTimerID;             // The static timer id.
    static TimerIDGifViewMap    s_timerIdGifViewMap;    // The timer id to AnimatedGifView pointer map.
};

#endif // _ANIMATEDGIFVIEW_H_
#endif // __cplusplus

