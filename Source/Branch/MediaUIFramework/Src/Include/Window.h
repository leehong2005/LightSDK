/*!
* @file Window.h
* 
* @brief This file defines the class Window, represents a visual window.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "CommonInclude.h"
#include "D2DDevice.h"
#include "D3DDevice.h"

class ViewElement;

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
    ViewElement     *EventSourceView;       // Event source view.
    ViewElement     *EventTargetView;       // Event target view.
    ViewElement     *CurrentPopupView;      // Current pop-up view.

} WINDOWVIEWS, *LPWINDOWVIEWS;


/*!
* @brief The window class is a abstract present of a visual window.
*/
class CLASS_DECLSPEC Window
{
public:

    /*!
    * @brief The public property.
    */
    WINDOWVIEWS WindowViews;

public:

    friend ViewElement;

    /*!
    * @brief Get Window instance by window handler.
    *
    * @param hWnd       [I/ ] The passed handle to window.
    *
    * @return Window instance if hWnd is valid, otherwise return NULL.
    */
    static Window* FromHwnd(HWND hWnd);

    /*!
    * @brief The constructor function.
    */
    Window();

    /*!
    * @brief The destructor function.
    */
    virtual ~Window();

    /*!
    * @brief Get the window to handle.
    *
    * @return The handler to window.
    */
    virtual HWND GetHWND() const;

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

public:

    /*!
    * @brief Get the desktop DPI ratio, typically value is 1.0, 1.25.
    *
    * @param pdpiX      [I/ ] The x DPI ratio.
    * @param pdpiY      [I/ ] The y DPI ratio.
    */
    virtual void GetDesktopDpiRatio(FLOAT *pdpiX, FLOAT *pdpiY);

    /*!
    * @brief Update the window.
    *
    * @param isErase    [I/ ] Indicates force to redraw, default value is FALSE.
    * @param lprcPaint  [I/ ] Pointer to a RECT structure that to be paint.
    */
    virtual void Invalidate(BOOL isErase = FALSE, const LPRECT lprcPaint = NULL);

    /*!
    * @brief Set the content view to the window, a window only can hold one content view.
    *
    * @param pViewElement   [I/ ] The content view to be added to window.
    */
    virtual void SetContentView(IN ViewElement *pViewElement);

    /*!
    * @brief Make the window display in center in screen or its parent window.
    *
    * @param fInScreen  [I/ ] Center window in screen or its parent window (if has).
    */
    virtual void CenterWindow(BOOL fInScreen = TRUE);

    /*!
    * @brief Close the window.
    */
    virtual void CloseWindow();

public:

    /*!
    * @brief Set the window enable or disable.
    *
    * @param isEnable   [I/ ] Indicates the window is enable or disable.
    */
    virtual void SetEnabled(BOOL isEnable = TRUE);

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
    * @brief Indicates whether the window is layered.
    *
    * @return TRUE if it is layered, otherwise FALSE.
    */
    virtual BOOL IsLayeredWindow() const;

    /*!
    * @brief Get the X-speed.
    */
    virtual DOUBLE GetSpeedX();

    /*!
    * @brief Get the Y-speed.
    */
    virtual DOUBLE GetSpeedY();

    /*!
    * @brief Get the Sample time.
    */
    virtual DWORD GetSpeedSampleTime();

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
    * @brief Get opacity value.
    *
    * @return Opacity value.
    */
    virtual FLOAT GetOpacity() const;

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
    * @param pView      [I/.] The pointer to ViewElement.
    */
    virtual void PushEventView(ViewElement *pView);

protected:

    HWND             m_hWnd;                // The handle to window.
    HWND             m_hWndParent;          // The handle to parent window.
    FLOAT            m_fOpacity;            // Opacity value, 0.0 to 1.0.
    INT32            m_nWindowState;        // The window state.
    D2DDevice       *m_pD2DDevice;          // The D2DDevice instance.
    D3DDevice       *m_pD3DDevice;          // D3DDevice instance.
    ViewElement     *m_pContentView;        // The content view instance.

    static vector<Window*> s_vctWindows;    // The window list.
};

#endif // _WINDOW_H_
#endif //__cplusplus
