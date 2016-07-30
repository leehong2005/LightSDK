/*!
* @file SdkWindow.h
* 
* @brief This file defines the class SdkWindow, represents a visual window.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _SDKWINDOW_H_
#define _SDKWINDOW_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_WINDOW

/*!
* @brief The window state values.
*/
typedef enum _WINDOW_STATE
{
    WINDOW_STATE_NONE           = 0,
    WINDOW_STATE_INVALIDATE     = 1,

} WINDOW_STATE;


/*
* @brief The window's views.
*/
typedef struct _WINDOWVIEWS
{
    SdkViewElement     *EventSourceView;        // Event source view.
    SdkViewElement     *EventTargetView;        // Event target view.
    SdkViewElement     *CurrentPopupView;       // Current pop-up view.
    SdkViewElement     *CurrentFocusView;       // Current focus view.

} WINDOWVIEWS, *LPWINDOWVIEWS;


/*!
* @brief The window class is a abstract present of a visual window.
*/
class CLASS_DECLSPEC SdkWindow
{
public:

    friend class SdkViewElement;

    /*!
    * @brief The public property.
    */
    WINDOWVIEWS WindowViews;

public:

    /*!
    * @brief Get SdkWindow instance by window handler.
    *
    * @param hWnd       [I/ ] The passed handle to window.
    *
    * @return SdkWindow instance if hWnd is valid, otherwise return NULL.
    */
    static SdkWindow* FromHwnd(HWND hWnd);

    /*!
    * @brief The constructor function.
    */
    SdkWindow();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkWindow();

    /*!
    * @brief Get the window to handle.
    *
    * @return The handler to window.
    */
    virtual HWND GetHwnd() const;

    /*!
    * @brief The (HWND) operator.
    *
    * @return return the handle to window.
    */
    virtual operator HWND() const;

    /*!
    * @brief Retrieve the class name of the window.
    *
    * @param lpszClassName  [I/ ] The buffer to receive class name.
    * @param uSize          [I/ ] The buffer size.
    */
    virtual void GetClassName(LPTSTR lpszClassName, UINT32 uSize);

    /*!
    * @brief Get width of window.
    *
    * @return The width of window.
    */
    virtual LONG GetWidth() const;

    /*!
    * @brief Get height of window.
    *
    * @return The height of window.
    */
    virtual LONG GetHeight() const;

    /*!
    * @brief Set window position.
    *
    * @param left   [I/ ] The left coordinate of window.
    * @param top    [I/ ] The top coordinate of window.
    */
    virtual void SetWindowPos(INT32 left, INT32 top);

    /*!
    * @brief Set window position.
    *
    * @param w      [I/ ] The width of window.
    * @param h      [I/ ] The height of window.
    */
    virtual void SetWindowSize(UINT32 w, UINT32 h);

    /*!
    * @brief Set the color of background.
    *
    * @param dwBkColor  [I/ ] The background color value.
    */
    virtual void SetBkColor(DWORD dwBkColor);

    /*!
    * @brief Set round corner radius of the window.
    *
    * @param fRadiusX       [I/ ] The radius of x.
    * @param fRadiusY       [I/ ] The radius of y.
    */
    virtual void SetRoundCornerRadius(FLOAT fRadiusX, FLOAT fRadiusY);

    /*!
    * @brief Indicates whether the window is layered.
    *
    * @return TRUE if it is layered, otherwise FALSE.
    */
    virtual BOOL IsLayeredWindow() const;

    /*!
    * @brief Get the desktop DPI ratio, typically value is 1.0, 1.25.
    *
    * @param pdpiX      [I/ ] The x DPI ratio.
    * @param pdpiY      [I/ ] The y DPI ratio.
    */
    virtual void GetDesktopDpi(FLOAT *pdpiX, FLOAT *pdpiY);

public:

    /*
    * @brief Call this method to add a visual view to the window. Once you add a view
    *        to the window, the window will manage memory of the view.
    *
    * @param pView      [I/ ] The pointer to SdkViewElement class.
    *
    * @return TRUE if succeed to add view, otherwise FALSE.
    */
    virtual BOOL AddView(IN SdkViewElement *pView);

    /*!
    * @brief Update the window.
    *
    * @param isErase    [I/ ] Indicates force to redraw, default value is FALSE.
    * @param lprcPaint  [I/ ] Pointer to a RECT structure that to be paint.
    */
    virtual void Invalidate(BOOL isErase = FALSE, const LPRECT lprcPaint = NULL);

    /*!
    * @brief Make the window display in center in screen or its parent window.
    *
    * @param fInScreen  [I/ ] Center window in screen or its parent window (if has).
    */
    virtual void CenterWindow(BOOL fInScreen);

    /*!
    * @brief Close the window.
    */
    virtual void CloseWindow();

    /*!
    * @brief Set the window enable or disable.
    *
    * @param isEnable   [I/ ] Indicates the window is enable or disable.
    */
    virtual void SetEnabled(BOOL isEnable);

    /*!
    * @brief Set the visibility of view.
    *
    * @param isShow     [I/ ] TRUE if visible, FALSE if invisible.
    */
    virtual void SetVisible(BOOL isShow);

    /*!
    * @brief Indicates the window is enable or disable.
    *
    * @return TRUE if enable, otherwise FALSE.
    */
    virtual BOOL IsEnabled() const;

    /*!
    * @brief Get the visibility of window.
    *
    * @return TRUE if visible, otherwise FALSE.
    */
    virtual BOOL IsVisible() const;

    /*!
    * @brief Get device instance held by window.
    *
    * @param ppDevices      [ /O] The pointer to pointer to D2DDevice class.
    *
    * @return The pointer to D2DDevice.
    */
    virtual D2DDevice* GetD2DDevices() const;

    /*!
    * @brief Get 3d device instance held by window.
    *
    * @return The pointer to D3DDevice.
    */
    virtual D3DDevice* GetD3DDevice() const;

protected:

    /*!
    * @brief Perform the view's on client.
    *
    * @param pView      [I/ ] The pointer to the SdkViewElement.
    */
    virtual void PerformViewOnClick(SdkViewElement *pView);

    /*!
    * Set the paint mode.
    *
    * @param paintMode  [I/ ] The paint mode, one value of DEVICE_PAINT_MODE.
    */
    virtual void SetDeviceTargetType(DEVICE_TARGET_TYPE paintMode);

    /*!
    * @brief Set the opacity of window.
    *
    * @param fOpacity       [I/ ] The Opacity value. 0.0 to 1.0.
    */
    virtual void SetOpacity(FLOAT fOpacity);

    /*!
    * @brief Add a window state.
    *
    * @param state      [I/ ] One value of WINDOW_STATE enumeration.
    */
    virtual void AddWindowState(WINDOW_STATE state);

    /*!
    * @brief Remove a window state.
    *
    * @param state      [I/ ] One value of WINDOW_STATE enumeration.
    */
    virtual void RemoveWindowState(WINDOW_STATE state);

    /*!
    * @brief Push the view to event chain to perform some event procedure. This function is 
    *        useful when perform OnClick or OnLongClick callback, we want to perform all callbacks
    *        after all event are processed.
    *
    * @param pView      [I/.] The pointer to SdkViewElement.
    */
    virtual void AddEventViews(SdkViewElement *pView);

protected:

    HWND             m_hWnd;                // The handle to window.
    HWND             m_hWndParent;          // The handle to parent window.
    FLOAT            m_fOpacity;            // Opacity value, 0.0 to 1.0.
    DWORD            m_dwBkColor;           // The background color.
    INT32            m_nWindowState;        // The window state.
    D2DDevice       *m_pD2DDevice;          // The D2DDevice instance.
    D3DDevice       *m_pD3DDevice;          // D3DDevice instance.
    SdkViewElement  *m_pRootView;           // The content view instance.

    static vector<SdkWindow*> s_vctWindows; // The window list.
};

END_NAMESPACE_WINDOW

#endif // _SDKWINDOW_H_
#endif //__cplusplus
