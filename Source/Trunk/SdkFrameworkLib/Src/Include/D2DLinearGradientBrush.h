/*!
* @file D2DLinearGradientBrush.h
* 
* @brief This file define a LinearGradientBrush
*        which use for Painting an area with a linear gradient 
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Zhou Yuan Qi
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _D2DLINEARGRADIENTBRUSH_
#define _D2DLINEARGRADIENTBRUSH_

#include "D2DBrush.h"

BEGIN_NAMESPACE_D2D

/*!
* @brief The D2DLinearGradientBrush class.
*/
class CLASS_DECLSPEC D2DLinearGradientBrush : public D2DBrush
{
public:

    /*!
    * @brief The constructor function.
    */
    D2DLinearGradientBrush(void);

    /*!
    * @brief The constructor function with a Param.
    */
    D2DLinearGradientBrush(D2D1_GRADIENT_STOP *pGradientStops, UINT gradientStopsCount);

    /*!
    * @brief The default destructor function.
    */
    virtual ~D2DLinearGradientBrush(void);

    /*!
    * @brief Call this function to create a Brush.
    *
    * @param pD2DDevice [I/ ] the Device of current application
    */
    BOOL CreateBrush(D2DDevice *pD2DDevice);

    /*!
    * @brief Create a brush.
    *
    * @param pRenderTarget  [I/ ] The render target.
    *
    * @return TRUE is success, FALSE is failure.
    */
    BOOL CreateBrush(IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Indicates has create brush or not.
    *
    * @return TRUE if has created brush, otherwise return FALSE.
    */
    BOOL HasCreatedBrush();

    /*!
    * @brief Call this function to get a Brush object from current application.
    *
    * @param pD2DDevice [ /O] the Device of current application
    */
    void GetD2DBrush(OUT ID2D1Brush **ppD2DBrush);

    /*!
    * @brief Call this function to set the starting and the ending coordinates of
    *                          the linear gradient in the brush's coordinate space.
    *
    * @param point [I/] the ending coordinates of the linear gradient.
    */
    void SetStartAndEndPoint(IN FLOAT startX,
                             IN FLOAT startY,
                             IN FLOAT   endX,
                             IN FLOAT   EndY);

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

    UINT                         m_nGradientStopsCount;     // The count of the GradientStops.
    D2D1_POINT_2F                m_gradientStartPT;         // The gradient start point.
    D2D1_POINT_2F                m_gradientEndPT;           // The gradient end point.
    ID2D1LinearGradientBrush    *m_pLinearGradientBrush;    // The LinearGradientBrush.
    ID2D1GradientStopCollection *m_pGradientStops;          // The Collection use to stop Gradient.
    D2D1_GRADIENT_STOP          *m_pSzGradientStops;        // The pointer to point a  D2D1_GRADIENT_STOP array.
};

END_NAMESPACE_D2D

#endif // _D2DLINEARGRADIENTBRUSH_
#endif // __cplusplus