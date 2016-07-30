/*!
* @file D2DBitmap.h 
* 
* @brief This file defines the class D2DBitmap, is used to operate image from file or resource.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#ifdef __cplusplus
#ifndef _D2DBITMAP_H_
#define _D2DBITMAP_H_

#include "SdkCommonInclude.h"
#include "SdkUICommon.h"
#include "D2DResource.h"
#include "ID2DDeviceStateChange.h"

BEGIN_NAMESPACE_D2D

class CLASS_DECLSPEC D2DBitmap : public D2DResource, public ID2DDeviceStateChange
{
public:

    /*!
    * @brief The constructor function.
    */
    D2DBitmap();

    /*!
    * @brief The destructor function.
    */
    ~D2DBitmap();

    /*!
    * @brief Initialize current object, before using, you should call this method.
    *
    * @param pD2DDevice     [I/ ] The pointer to D2DDevice object.
    *
    * @return TRUE is success, FALSE is failure.
    */
    BOOL Initialize(D2DDevice *pD2DDevice);

    /*!
    * @brief Initialize current object, before using, you should call this method.
    *
    * @param pRenderTarget  [I/ ] The pointer to ID2D1RenderTarget object.
    *
    * @return TRUE is success, FALSE is failure.
    */
    BOOL Initialize(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Indicates an object of D2DBitmap whether has been initialized.
    *
    * @return TRUE if initialized, otherwise is FALSE.
    */
    BOOL HasInitialized();

    /*!
    * @brief Load a image from a specified file name.
    *
    * @param lpfile         [I/ ] The pointer to file name.
    * @param uDestHeight    [I/ ] The destination height.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    *
    * @remark The uDestWidth and uDestHeight default value is 0,
    *         that means destination bound is original bound.
    */
    BOOL LoadFromFile(LPCWSTR lpfile, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Load a image from HBITMAP.
    *
    * @param hBitmap        [I/ ] The handle to HBITMAP.
    * @param uDestHeight    [I/ ] The destination height.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    *
    * @remark The uDestWidth and uDestHeight default value is 0,
    *         that means destination bound is original bound.
    */
    BOOL LoadFromHBITMAP(HBITMAP hBitmap, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Load a image from a resource.
    *
    * @param uResId         [I/ ] The identifier of resource.
    * @param hModule        [I/ ] Indicates thats load image from.which module, default value is NULL.
    * @param uDestHeight    [I/ ] The destination height.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    *
    * @remark The uDestWidth and uDestHeight default value is 0,
    *         that means destination bound is original bound.
    */
    BOOL LoadFromResource(UINT uResId, HMODULE hModule = NULL, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Get an instance of ID2D1Bitmap, which can be used to draw with render target.
    *
    * @param ppD2DBitmap [ /O] The pointer to pointer to ID2D1Bitmap.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    */
    BOOL GetD2DBitmap(OUT ID2D1Bitmap **ppD2DBitmap);

    /*!
    * @brief Get the width of image.
    *
    * @return The width of image.
    */
    UINT GetWidth();

    /*!
    * @brief Get the height of image.
    *
    * @return The height of image.
    */
    UINT GetHeight();

protected:

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

    ID2D1Bitmap             *m_pD2DBitmap;          // The ID2D1Bitmap instance, represents a bitmap load from file or resource.
    ID2D1RenderTarget       *m_pRenderTarget;       // The Render target, used to create ID2D1Bitmap instance.
    SdkWICImageHelper       *m_pWICImageHelper;     // The pointer to SdkWICImageHelper, used to process image file.
};

END_NAMESPACE_D2D

#endif // _D2DBITMAP_H_
#endif // __cplusplus
