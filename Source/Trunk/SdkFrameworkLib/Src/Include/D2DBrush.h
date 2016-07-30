/*!
* @file D2DBrush.h 
* 
* @brief This file defines the class D2DBrush.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#ifdef __cplusplus
#ifndef _D2DBRUSH_H_
#define _D2DBRUSH_H_

#include "SdkCommonInclude.h"
#include "D2DDevice.h"
#include "D2DResource.h"
#include "ID2DDeviceStateChange.h"

BEGIN_NAMESPACE_D2D

/*!
* @brief The class should be used as base class of various D2D brush.
*/
class CLASS_DECLSPEC D2DBrush : public D2DResource, public ID2DDeviceStateChange
{
public:

    /*!
    * @brief The constructor function.
    */
    D2DBrush();

    /*!
    * @brief The destructor function.
    */
    virtual ~D2DBrush();

    /*!
    * @brief Create a brush.
    *
    * @param pD2DDevice     [I/ ] The pointer to D2DDevice object.
    *
    * @return TRUE is success, FALSE is failure.
    */
    virtual BOOL CreateBrush(D2DDevice *pD2DDevice);

    /*!
    * @brief Create a brush.
    *
    * @param pRenderTarget  [I/ ] The render target.
    *
    * @return TRUE is success, FALSE is failure.
    */
    virtual BOOL CreateBrush(IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Indicates has create brush or not.
    *
    * @return TRUE if has created brush, otherwise return FALSE.
    */
    virtual BOOL HasCreatedBrush();

    /*!
    * @brief Get an instance of ID2D1Brush.
    *
    * @param ppD2DBrush     [ /O] The pointer to pointer to ID2D1Brush.
    */
    virtual void GetD2DBrush(OUT ID2D1Brush **ppD2DBrush);

    /*!
    * @brief Set the color of the solid brush.
    *
    * @param color          [I/ ] Pointer to D2D1_COLOR_F.
    */
    virtual void SetColor(IN const D2D1_COLOR_F &color);

    /*!
    * @brief Get the color of the solid brush.
    *
    * @param color          [ /O] Pointer to D2D1_COLOR_F.
    */
    virtual void GetColor(OUT D2D1_COLOR_F *color);

    /*!
    * @brief Set opacity of the brush.
    *
    * @param opacity         [I/ ] The opacity of the brush.
    */
    virtual void SetOpacity(FLOAT opacity);

    /*!
    * @brief Get opacity of the brush.
    *
    * @return The opacity of brush.
    */
    virtual FLOAT GetOpacity();

    /*!
    * @brief Called when device state change, typically the render target is recreated.
    *
    * @param pDevice    [I/ ] The D2DDevice pointer.
    * @param stateVal   [I/ ] This value indicates which state change just occurred.
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
};

END_NAMESPACE_D2D

#endif // _D2DBRUSH_H_
#endif // __cplusplus
