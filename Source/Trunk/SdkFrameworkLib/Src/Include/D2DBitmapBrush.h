/*!
* @file D2DBitmapBrush.h
* 
* @brief The file defines D2DBitmapBrush class, inherited from D2DBrush.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/03/01
*/

#ifdef __cplusplus
#ifndef _D2DBITMAPBRUSH_H_
#define _D2DBITMAPBRUSH_H_

#include "SdkCommon.h"
#include "D2DBrush.h"
#include "D2DBitmap.h"

BEGIN_NAMESPACE_D2D

class CLASS_DECLSPEC D2DBitmapBrush : public D2DBrush
{
public:

    /*!
    * @brief The constructor function.
    */
    D2DBitmapBrush(UINT uResId, HMODULE hModule);

    /*!
    * @brief The constructor function.
    */
    D2DBitmapBrush(HBITMAP hBitmap);

    /*!
    * @brief The constructor function.
    */
    D2DBitmapBrush(LPCTSTR lpfilePath);

    /*!
    * @brief The destructor function.
    */
    virtual ~D2DBitmapBrush();

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

    D2DBitmap           *m_pD2DBitmap;          // The D2DBitmap instance.
    ID2D1BitmapBrush    *m_pBitmapBrush;        // The instance of ID2D1BitmapBrush.
};

END_NAMESPACE_D2D

#endif // _D2DBITMAPBRUSH_H_
#endif // __cplusplus
