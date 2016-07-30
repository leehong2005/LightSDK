/*!
* @file RatingView.h 
* 
* @brief This file defines methods to show rating for media files.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/08
*/

#ifdef __cplusplus
#ifndef _RATINGVIEW_H_
#define _RATINGVIEW_H_

#include "ViewElement.h"

/*!
* @brief The RatingView is used to show rating for media files.
*/
class CLASS_DECLSPEC RatingView : public ViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    RatingView();

    /*!
    * @brief The destructor function.
    */
    virtual ~RatingView();

    /*!
    * @brief Call this method to set star numbers.
    * 
    * @param numStars       [I/ ] number of stars.
    */
    void SetStarsNumber(IN UINT32 numStars);

    /*!
    * @brief Call this method to set ratingBar rating.
    * 
    * @param rating         [I/ ] RatingBar rating.
    */
    void SetRating(IN UINT32 rating);

    /*!
    * @brief Call this method to get ratingBar rating.
    * 
    * @return The rating numbers.
    */
    UINT32 GetRating() const;

    /*!
    * @brief Set the layout information of the view.
    *
    * @param pLayoutInfo    [I/ ] The pointer to LAYOUTINFO structure.
    */
    virtual void SetLayoutInfo(IN const LAYOUTINFO* pLayoutInfo);

    /*!
    * @brief Set the layout information of the view.
    *
    * @param x              [I/ ] The x coordinate of left.
    * @param y              [I/ ] The y coordinate of left.
    * @param width          [I/ ] The width coordinate of left.
    * @param height         [I/ ] The height coordinate of left.
    */
    virtual void SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height);

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

    /*!
    * @brief Call this method to set pentacle's size.
    * 
    * @param radius         [I/ ] radius:the distance from center point to every angle.
    */
    void SetPentacleSize(IN UINT32 radius = 10);

protected:

    /*!
    * @brief Call this method to create a pentacle path geometry.
    * 
    * @param center         [I/ ] The pentacle's center point.
    * @param nRadius        [I/ ] size:the distance from center point to every angle.
    */
    void CreatePentacleGeometry(const D2D1_POINT_2F& center, UINT32 nRadius);

    /*!
    * @brief Create points of pentacle geometry.
    *
    * @param center         [I/ ] The center point of pentacle.
    * @param offsetAngle    [I/ ] The offset angle of top point relative to Y axis.
    * @param nRadius        [I/ ] The radius of pentacle outer circle.
    */
    void CreatePentaclePoints(IN const D2D1_POINT_2F& center, UINT32 offsetAngle, UINT32 nRadius);

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

private:

    UINT32                          m_nHMargin;                 // The margin on horizontal direction.
    UINT32                          m_nVMargin;                 // The margin on vertical direction.
    UINT32                          m_nStarsNumber;             // The ratingBar stars number.
    UINT32                          m_nRating;                  // The ratingBar rate value.
    UINT32                          m_nRadius;                  // The distance of center point to every angle.
    BOOL                            m_isLBDown;                 // Indicates whether left button is down or not.
    D2DBrush                       *m_pD2DBrush;                // The D2DBrush.
    D2D1_POINT_2F                   m_szPentaclePoints[5];      // The points of pentacle geometry.
    D2D1_POINT_2F                   m_centerPoint;              // The view center point.
    ID2D1PathGeometry              *m_pPathGeometry;            // The path of created pentacle.
    ID2D1BitmapRenderTarget        *m_pBitmapRenderTarget;      // The bitmap compatible renderTarget.Use it send bitmap to renderTarget.
};

#endif // _RATINGVIEW_H_
#endif // __cplusplus
