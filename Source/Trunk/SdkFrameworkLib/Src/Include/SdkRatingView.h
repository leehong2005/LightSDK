/*!
* @file SdkRatingView.h 
* 
* @brief This file defines methods to show rating for media files.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/08
*/

#ifdef __cplusplus
#ifndef _SDKRATINGVIEW_H_
#define _SDKRATINGVIEW_H_

#include "SdkViewElement.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The SdkRatingView is used to show rating for media files.
*/
class CLASS_DECLSPEC SdkRatingView : public SdkViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkRatingView();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkRatingView();

    /*!
    * @brief Call this method to set star numbers.
    * 
    * @param numStars       [I/ ] number of stars.
    */
    void SetStarsCount(IN UINT32 numStars = 5);

    /*!
    * @brief Call this method to set ratingBar rating.
    * 
    * @param rating         [I/ ] RatingBar rating.
    */
    void SetRating(IN UINT32 rating);

    /*!
    * @brief Set the flag to indicate the view auto size to the stats number.
    */
    void SetAutoSize(BOOL fAutoSize);

    /*!
    * @brief Set the stars size, minimum is 20, maximum 32.
    *
    * @param fStartSize     [I/ ] The star size.
    */
    void SetStarsSize(FLOAT fStarSize);

    /*!
    * @brief Call this method to get ratingBar rating.
    * 
    * @return The rating numbers.
    */
    UINT32 GetRating() const;

    /*!
    * @brief Set the rating view event handler to get notification when rating is changed.
    *
    * @param pEventHandler  [I/ ] The pointer to IRatingViewEventHandler.
    */
    void SetRatingEventHandler(IRatingViewEventHandler *pEventHandler);

    /*!
    * @brief Set the layout information of the view.
    *
    * @param x              [I/ ] The x coordinate of left.
    * @param y              [I/ ] The y coordinate of left.
    * @param width          [I/ ] The width coordinate of left.
    * @param height         [I/ ] The height coordinate of left.
    */
    virtual void SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height);

protected:

    /*!
    * @brief Call this method to reset information about layout.
    */
    void ResetLayoutInfo();

    /*!
    * @brief Get the star index according to th point.
    *
    * @param xPos           [I/ ] The x-coordinate.
    * @param yPos           [I/ ] The y-coordinate.
    *
    * @return The index of the star.
    */
    int GetStarIndexFromPoint(FLOAT xPos, FLOAT yPos);

    /*!
    * @brief Get the drawing bitmap according to the specified star index.
    *
    * @param nStarIndex     [I/ ] The index of star.
    *
    * @return The pointer to D2DBitmap.
    */
    D2DBitmap* GetDrawingBitmap(int nStarIndex);

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Mouse event.
    *
    * @param message        [I/ ] The pointer to file name.
    * @param wParam         [I/ ] The pointer to file name.
    * @param lParam         [I/ ] The pointer to file name.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Called when device state change, typically the render target is recreated.
    *
    * @param pDevice        [I/ ] The D2DDevice pointer.
    * @param stateVal       [I/ ] This value indicates which state change just occurred.
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

    /*!
    * @brief The internal data of rating view.
    */
    struct _RATINGVIEW_INTERNALDATA;

    _RATINGVIEW_INTERNALDATA    *m_pRatingViewData; // The internal data.
};

END_NAMESPACE_VIEWS

#endif // _SDKRATINGVIEW_H_
#endif // __cplusplus
