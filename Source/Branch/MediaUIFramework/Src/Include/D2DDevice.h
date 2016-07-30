/*!
* @file D2DDevice.h
* 
* @brief The file defines the class D2DDevice, which wraps the D2D device, such as Render Target, etc.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#ifdef __cplusplus
#ifndef _D2DDEVICE_H_
#define _D2DDEVICE_H_

#include "CommonInclude.h"

class ID2DDeviceStateChange;

/*!
* @brief The enumeration value of device state.
*/
typedef enum _DEVICE_STATECHANGE_VALUE
{
    DEVICE_STATECHANGE_VALUE_NONE       = 0,        // N/A.
    DEVICE_STATECHANGE_VALUE_RESIZE     = 1,        // Device resize.
    DEVICE_STATECHANGE_VALUE_RECREATE   = 2,        // Device recreate.
    DEVICE_STATECHANGE_VALUE_CHANGE     = 3,        // Device change.

} DEVICE_STATECHANGE_VALUE;


/*!
* @brief The enumeration value of device state.
*/
typedef enum _DEVICE_TARGET_TYPE
{
    DEVICE_TARGET_TYPE_NONE             = 0,        // Unknown target type.
    DEVICE_TARGET_TYPE_HWND             = 1,        // HWND target.
    DEVICE_TARGET_TYPE_MEMORY           = 2,        // Bitmap target.
    DEVICE_TARGET_TYPE_DC               = 3,        // DC target.

} DEVICE_TARGET_TYPE;


/*!
* @brief The class is a wrapper of D2D devices, such as Render Target, D2D factory, etc.
*/
class CLASS_DECLSPEC D2DDevice
{
public:

    /*!
    * @brief Get D2DDevice from specified Render Target.
    *
    * @param pRenderTarget      [I/ ] The Render Target.
    *
    * @return The found D2DDevice pointer, if failed, returns NULL.
    */
    static D2DDevice* FromD2DRenderTarget(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief The constructor function.
    */
    D2DDevice();

    /*!
    * @brief The destructor function.
    */
    ~D2DDevice();

    /*!
    * @brief Initialize the device, with specified handle to window.
    *
    * @param hWnd       [I/ ] The handle to window.
    *
    * @return S_OK is success, E_FALSE is failure.
    */
    HRESULT InitDevice(HWND hWnd);

    /*!
    * @brief Initialize the device.
    *
    * @param hdc        [I/ ] The device context.
    *
    * @return S_OK is success, E_FALSE is failure.
    */
    HRESULT InitMemDevice(HDC hdc);

    /*!
    * @brief Initialize DC render target.
    *
    * @return S_OK is success, E_FALSE is failure.
    */
    HRESULT InitDCDevice();

    /*!
    * @brief Create the factory.
    */
    HRESULT CreateFactory();

    /*!
    * @brief Get the desktop DPI.
    *
    * @param dpiX               [ /O] The DPI X value.
    * @param dpiY               [ /O] The DPI Y value.
    */
    void GetDesktopDpi(OUT FLOAT *dpiX, OUT FLOAT *dpiY);

    /*!
    * @brief Get the instance of ID2D1Factory interface.
    *
    * @param ppD2DFactory        [ /O] Output instance of ID2D1Factory.
    */
    void GetD2DFactory(OUT ID2D1Factory **ppD2DFactory);

    /*!
    * @brief Get the HBITMAP handle of drawing stuffs, it is valid when paint mode
    *        is DEVICE_PAINT_MODE_MEMORY and after calling to EndDraw function.
    *
    * @param pWidth     [ /O] The width of HBITMAP.
    * @param pHeight    [ /O] The height of HBITMAP.
    *
    * @return Handle of HBITMAP.
    */
    HBITMAP GetMemHBITMAP(OUT UINT32 *pWidth = NULL, OUT UINT32 *pHeight = NULL);

    /*!
    * @brief Get the instance of ID2D1RenderTarget interface, generally, when you
    *        want to draw something with D2D, you should call this method to get
    *        the ID2D1RenderTarget instance.
    *
    * @param ppTarget   [ /O] Output instance of ID2D1RenderTarget.
    */
    void GetRenderTarget(OUT ID2D1RenderTarget **ppTarget);

    /*!
    * @brief Set the opacity of window.
    *
    * @param fOpacity       [I/ ] The Opacity value. 0.0 to 1.0.
    */
    void SetOpacity(FLOAT fOpacity);

    /*!
    * @brief Get opacity value.
    *
    * @return Opacity value.
    */
    FLOAT GetOpacity() const;

    /*!
    * @brief Begin to prepare scene to draw with D2D, generally, you should NOT call
    *        this method in your drawing code.
    *
    * @param hDC        [I/ ] The handle to device context.
    * @param lpRect     [I/ ] The rectangle to draw.
    * @param isClearRT  [I/ ] Clear render target or not.
    */
    void BeginDraw(HDC hDC = NULL, const LPRECT lpRect = NULL, BOOL isClearRT = TRUE);

    /*!
    * @brief Call this method to flush the drawing to screen.
    */
    void EndDraw();

    /*!
    * Set the paint mode.
    *
    * @param paintMode  [I/ ] The paint mode, one value of DEVICE_PAINT_MODE.
    */
    void SetPaintTargetType(DEVICE_TARGET_TYPE paintMode);

    /*!
    * @brief Get the paint target type.
    *
    * @return One value of DEVICE_PAINT_TARGET enumeration.
    */
    DEVICE_TARGET_TYPE GetPaintTargetType();

    /*!
    * @brief Get the size of the render target.
    *
    * @return D2D1_SIZE_F structure object.
    */
    D2D1_SIZE_F GetSize();

    /*!
    * @brief Indicates window's state whether is occluded or not.
    *
    * @return TRUE if it is occluded, FALSE otherwise.
    */
    BOOL IsWindowOccluded();

    /*!
    * @brief Resize the render target, typically, it is called when window's size is changed.
    *
    * @param w          [I/ ] The new width of window.
    * @param h          [I/ ] The new height of window.
    */
    void Resize(UINT32 w, UINT32 h);

    /*!
    * @brief Register D2D device change notification.
    *
    * @param pNotification  [I/ ] The registered instance.
    */
    void RegisterD2DDeviceNotification(ID2DDeviceStateChange *pNotification);

    /*!
    * @brief Unregister D2D device change notification.
    *
    * @param pNotification  [I/ ] The registered instance.
    */
    void UnregisterD2DDeviceNotification(ID2DDeviceStateChange *pNotification);

protected:

    /*!
    * @brief Create the WIC bitmap render target, used to draw stuffs on WIC Bitmap.
    *
    * @param hDC                [I/ ] The device context.
    * @param hOutMemDC          [ /O] The memory device context.
    * @param ppTarget           [ /O] Output instance of ID2D1RenderTarget.
    * @param ppWICBitmap        [ /O] The IWICBitmap instance, held by WIC Bitmap render target.
    *
    * @param S_OK if succeeds, otherwise E_FAIL.
    *
    * @remark It is your responsibility to release ppTarget ad ppWICBitmap instances.
    */
    HRESULT CreateWICBitmapRenderTarget(IN HDC hDC, OUT HDC& hOutMemDC, OUT ID2D1RenderTarget **ppTarget, OUT IWICBitmap **ppWICBitmap);

    /*!
    * @brief Perform device change notify.
    *
    * @brief val        [I/ ] The device state change.
    */
    void PerformDeviceChangeNotify(DEVICE_STATECHANGE_VALUE val);

    /*!
    * @brief Add a D2DDevice pointer to D2DDevice list.
    *
    * @param pD2DDevice [I/ ] D2DDevice pointer.
    */
    void AddD2DDeviceToList(D2DDevice *pD2DDevice);

    /*!
    * @brief Remove a D2DDevice pointer from D2DDevice list.
    *
    * @param pD2DDevice [I/ ] D2DDevice pointer.
    */
    void RemoveD2DDeviceFromList(D2DDevice *pD2DDevice);

protected:

    FLOAT                            m_fOpacity;                    // Opacity value, 0.0 to 1.0.
    HDC                              m_hMemDC;                      // The memory device context.
    HWND                             m_hWnd;                        // The handle to window.
    BOOL                             m_isPaintModeChange;           // Indicates whether paint mode is changed.
    DEVICE_TARGET_TYPE              m_paintTargetType;             // The paint mode.
    IWICBitmap                      *m_pWICBitmap;                  // The WIC bitmap held by WIC Bitmap render target.
    ID2D1Factory                    *m_pD2DFactory;                 // The instance of ID2D1Factory, it is the entry of D2D.
    ID2D1HwndRenderTarget           *m_pRenderTarget;               // The Render target instance.
    ID2D1DCRenderTarget             *m_pDCRenderTarget;             // The GDI DC render target.
    ID2D1RenderTarget               *m_pWICBitmapRenderTarget;      // The memory render target.
    IDWriteBitmapRenderTarget       *m_pDWriteBitmapTarget;         // The Direct write bitmap target.
    vector<ID2DDeviceStateChange*>   m_vctDeviceChangeListeners;    // The device change listeners.

    static vector<D2DDevice*>        s_vctD2DDeviceList;            // The D2DDevice list.
};

#endif // _D2DDEVICE_H_
#endif // __cplusplus

