/*!
* @file ID2DDeviceStateChange.h 
* 
* @brief This file define the common behavior when D2D device is changed.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/18
*/

#ifdef __cplusplus
#ifndef _ID2DDEVICESTATECHANGE_H_
#define _ID2DDEVICESTATECHANGE_H_

#include "SdkUICommon.h"
#include "D2DDevice.h"

BEGIN_NAMESPACE_CALLBACK

class ID2DDeviceStateChange
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ID2DDeviceStateChange() {};

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
    virtual void OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal) = 0;

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
    virtual void OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _ID2DDEVICESTATECHANGE_H_
#endif // __cplusplus
