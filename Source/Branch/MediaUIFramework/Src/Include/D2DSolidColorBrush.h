/*!
* @file D2DSolidColorBrush.h
* 
* @brief The file defines D2DSolidColorBrush class, inherited from D2DBrush.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#ifdef __cplusplus
#ifndef _D2DSOLIDCOLORBRUSH_H_
#define _D2DSOLIDCOLORBRUSH_H_

#include "D2DBrush.h"

/*!
* @brief This class implements the solid color brush.
*/
class CLASS_DECLSPEC D2DSolidColorBrush : public D2DBrush
{
public:

    /*!
    * @brief The constructor function.
    */
    D2DSolidColorBrush();

    /*!
    * @brief The destructor function.
    */
    virtual ~D2DSolidColorBrush();

    /*!
    * @brief Create a brush.
    *
    * @param pD2DDevice     [I/ ] The pointer to D2DDevice object.
    *
    * @return TRUE is success, FALSE is failure.
    */
    BOOL CreateBrush(IN D2DDevice *pD2DDevice);

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
    * @brief Create a brush.
    *
    * @param pD2DDevice     [I/ ] The pointer to D2DDevice object.
    *
    * @return TRUE is success, FALSE is failure.
    */
    void GetD2DBrush(OUT ID2D1Brush **ppD2DBrush);

    /*!
    * @brief Set the color of the solid brush.
    *
    * @param color          [I/ ] Pointer to D2D1_COLOR_F.
    */
    void SetColor(IN const D2D1_COLOR_F &color);

    /*!
    * @brief Get the color of the solid brush.
    *
    * @param color          [ /O] Pointer to D2D1_COLOR_F.
    */
    void GetColor(OUT D2D1_COLOR_F *color);

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

    ID2D1SolidColorBrush    *m_pSolidColorBrush;        // The instance of ID2D1SolidColorBrush.
};

#endif // _D2DSOLIDCOLORBRUSH_H_
#endif // __cplusplus

