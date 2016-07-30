/*!
* @file D2DRadialGradientBrush.h
* 
* @brief This file define a RadialGradientBrush
*        which use for Painting an area with a radial gradient 
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Zhou Yuan Qi
* @date 2010/02/10
*/
#ifdef __cplusplus
#ifndef _D2DRADIALGRADIENTBRUSH_
#define _D2DRADIALGRADIENTBRUSH_

#include "D2DBrush.h"

/*!
* @brief The D2DRadialGradientBrush class.
*/

class CLASS_DECLSPEC D2DRadialGradientBrush : public D2DBrush
{
public:

    /*!
    * @brief The constructor function.
    */
    D2DRadialGradientBrush(void);

    /*!
    * @brief The constructor function with a Param.
    */
    D2DRadialGradientBrush(D2D1_GRADIENT_STOP *pGradientStops, UINT gradientStopsCount);

    /*!
    * @brief The default destructor function.
    */
    virtual ~D2DRadialGradientBrush(void);

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
    * @brief Call this function to set the center of brush.
    *
    * @param centerX  [I/ ] The centerX
    *
    * @param centerY  [I/ ] The centerY.
    */
    void SetCenter(IN FLOAT centerX,IN FLOAT centerY);

    /*!
    * @brief Call this function to set the Offset of brush.
    *
    * @param Offset of x [I/ ] The Offset x.
    *
    * @param Offset of y [I/ ] The Offset y
    */
    void SetGradientOriginOffset(IN FLOAT dx,IN FLOAT dy);

    /*!
    * @brief Call this function to Set RadiuX and RadiuY.
    *
    * @param radiuX [I/ ] The RadiuX.
    *
    * @param radiuY [I/ ] The RadiuY
    */
    void SetRadiuXandRadiuY(IN FLOAT radiuX,IN FLOAT radiuY);

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

    UINT                         m_nGradientStopsCount;     // 
    D2D1_POINT_2F                m_gradientCenterPT;        // The gradient start point.
    D2D1_POINT_2F                m_gradientOffsetPT;        // The gradient end point.
    FLOAT                        m_gradientRadiusX;         // The Radius x axis.
    FLOAT                        m_gradientRadiusY;         // The Radius y axis.
    ID2D1RadialGradientBrush     *m_pRadialGradientBrush;   // The pointer of radial brush.
    ID2D1GradientStopCollection  *m_pGradientStops;         // The pointer of stop collection.
    D2D1_GRADIENT_STOP           *m_pSzGradientStops;       // The point to color array.
};

#endif // _D2DRADIALGRADIENTBRUSH_
#endif // __cplusplus
