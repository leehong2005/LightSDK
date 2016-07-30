/*!
* @file WindowForm.h 
* 
* @brief This file defines functions to create, show and process messages for window.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _WINDOWFORM_H_
#define _WINDOWFORM_H_

#include "CommonInclude.h"
#include "FrameworkCommonDef.h"
#include "ViewElement.h"

/*!
* @brief The create window parameter.
*/
typedef struct _CREATEWINDOWPARAMS
{
    int         x;                      // Specifies the x coordinate.
    int         y;                      // Specifies the y coordinate.
    int         cx;                     // Specifies the width of new window.
    int         cy;                     // Specifies the height of new window.
    LPVOID      lpCreateParams;         // Pointer to data to be used to create window.
    HINSTANCE   hInstance;              // Identifies the module-instance handle.
    HMENU       hMenu;                  // Menu handle. If a child window, contains the ID.
    HWND        hwndParent;             // Parent window handle. NULL if new window is top-level window.
    DWORD       dwStyle;                // Specifies the style of new window.
    DWORD       dwExStyle;              // Specifies the new window's extended style.
    LPCTSTR     lpszName;               // Pointer to a null-terminated string that specifies window's name.
    LPCTSTR     lpszClass;              // Window's class name.

} CREATEWINDOWPARAMS, *LPCREATEWINDOWPARAMS;

/*!
* @brief The main window of Media UI application.
*/
class CLASS_DECLSPEC WindowForm : public Window
{
public:

    /*!
    * @brief The constructor function.
    *
    * @param isPostQuitMsg  [I/ ] Post quit message or not when receive WM_DESTROY.
    */
    WindowForm();

    /*
    * @brief The default destructor function
    */
    virtual ~WindowForm();

    /*!
    * @brief Call this function to show main window.
    *
    * @param hInstance      [I/ ] The instance of current process to run main window.
    * @param hWndParent     [I/ ] The parent of the window, usually set NULL.
    * @param nCmdShow       [I/ ] Specifies the window show state.
    *
    * @return TRUE if success to initialize and show window, otherwise return FALSE.
    */
    virtual BOOL CreateWindowForm(HINSTANCE hInstance, HWND hWndParent, INT32 nCmdShow = SW_SHOW);

    /*!
    * @brief Retrieve the class name of the window.
    *
    * @param lpszClassName  [I/ ] The buffer to receive class name.
    * @param uSize          [I/ ] The buffer size.
    */
    virtual void GetClassName(LPTSTR lpszClassName, UINT32 uSize);

    /*!
    * @brief Update the window.
    *
    * @param isErase    [I/ ] Indicates force to redraw, default value is FALSE.
    * @param lprcPaint  [I/ ] Pointer to a RECT structure that to be paint.
    */
    virtual void Invalidate(BOOL isErase = FALSE, const LPRECT lprcPaint = NULL);

    /*!
    * @brief Get the X-speed.
    */
    virtual DOUBLE GetSpeedX();

    /*!
    * @brief Get the Y-speed.
    */
    virtual DOUBLE GetSpeedY();

    /*!
    * @brief Get the speed sample time.
    */
    virtual DWORD GetSpeedSampleTime();

protected:

    /*!
    * @brief Update the window.
    *
    * @return TRUE if succeed, FALSE otherwise.
    */
    virtual BOOL InitializeDevice();

    /*!
    * @brief Register window class.
    *
    * @param hInstance   [I/ ] The instance of current process to run main window.
    * @param lpClassName [I/ ] The class name.
    *
    * @return TRUE if success to initialize and show window, otherwise return FALSE.
    */
    virtual BOOL RegisterWindowClass(HINSTANCE hInstance, LPCTSTR lpClassName);

    /*!
    * @brief Push the view to event chain to perform some event procedure. This function is 
    *        useful when perform OnClick or OnLongClick callback, we want to perform all callbacks
    *        after all event are processed.
    *
    * @param pView      [I/.] The pointer to ViewElement.
    */
    virtual void PushEventView(ViewElement *pView);

    /*!
    * @brief Cancel the target view's event.
    */
    virtual void CancelTargetViewEvent();

    /*!
    * @brief Show or hide the current pop-up view.
    *
    * @param x          [I/ ] The x of mouse point.
    * @param y          [I/ ] The t of mouse point.
    *
    * @return TRUE if succeeds to perform the operation, FALSE otherwise.
    */
    virtual BOOL HideCurPopupView(LONG x = 0, LONG y = 0);

    /*
    * @brief Dispatch events to view element.
    *
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    *
    * @return Handled return TRUE, otherwise FALSE.
    */
    virtual BOOL DispatchMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*
    * @brief Dispatch events to view element.
    *
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    *
    * @return Handled return TRUE, otherwise FALSE.
    */
    virtual BOOL DispatchTouchEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Call this method to perform view event.
    *
    * @param message    [I/ ] The message.
    */
    virtual void PerformViewEvent(UINT message);

protected:

    /*!
    * @brief Call this function when it is to be drawn.
    *
    * @param hdc        [I/ ] The device context to be drawn.
    * @param rcpaint    [I/ ] The const reference to RECT structure.
    */
    virtual void OnPaint(HDC hdc, const RECT& rcPaint);

    /*!
    * @brief Call this function when it is to be drawn with HWND render target.
    *
    * @param hdc        [I/ ] The device context to be drawn.
    * @param rcpaint    [I/ ] The const reference to RECT structure.
    */
    virtual void OnHwndPaint(HDC hdc, const RECT& rcPaint);

    /*!
    * @brief Paint on DC render target.
    *
    * @param hdc        [I/ ] The device context to be drawn.
    * @param rcpaint    [I/ ] The const reference to RECT structure.
    */
    virtual void OnDCPaint(HDC hdc, const RECT& rcPaint);

    /*!
    * @brief Paint in memory, not paint on window.
    *
    * @param hdc        [I/ ] The device context to be drawn.
    * @param rcpaint    [I/ ] The const reference to RECT structure.
    */
    virtual void OnMemPaint(HDC hdc, const RECT& rcPaint);

    /*!
    * @brief Draw bitmap on specified device.
    *
    * @param hDC        [I/ ] The handle to Device context.
    * @param paintRC    [I/ ] The bound to draw.
    *
    * @return TRUE if succeeds to draw bitmap, otherwise FALSE.
    */
    virtual BOOL OnDrawBitmap(HDC hDC, IN const LPRECT lpRect);

    /*!
    * @brief Draw bitmap on specified device.
    *
    * @param hDC        [I/ ] The handle to Device context.
    * @param hBitmap    [I/ ] The handle to bitmap.
    */
    virtual void OnDrawBitmap(HDC hDC, HBITMAP hBitmap, IN const RECT *paintRC = NULL);

    /*!
    * @brief Framework calls this function to draw layered window UI. This function is used
    *        only for layered window.
    *
    * @param fAlwaysDraw [I/ ] Indicate whether always to draw or not.
    * @param lprcPaint   [I/ ] Pointer to a RECT structure that to be paint.
    *
    * @return TRUE if succeeds to draw, otherwise FALSE.
    */
    virtual BOOL OnDrawUI(BOOL fAlwaysDraw = FALSE, const LPRECT lprcPaint = NULL);

protected:

    /*!
    * @brief Called when the window is created. In this method, derived class
    *        can initialize device, add child view, etc.
    *
    * @remark This function is called after calling of CreateWindow and before ShowWindow calling.
    */
    virtual void OnCreated();

    /*!
    * @brief The framework calls this member function when an application requests that the Windows window be
    *        created by calling CreateWindow or CreateWindowEx.
    *
    * @param lpCreateStruct    [I/ ] Points to a CREATESTRUCT structure that contains information about the WindowForm object being created.
    *
    * @remark This function is called when window receives WM_CREATE message.
    */
    virtual void OnCreating(LPCREATESTRUCT lpCreateStruct);

    /*!
    * @brief The framework calls this function when an application requests that the
    *        the Windows window be created by calling the CreateWindowForm() member function.
    *
    * @remark If derived class wants to create custom window, should override this function
    *         to specified custom parameters.
    */
    virtual void OnPreCreate(LPCREATEWINDOWPARAMS lpCreateWinParams);

    /*
    * @brief The message procedure for mouse event.
    *
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    *
    * @return TRUE if handled, FALSE otherwise.
    */
    virtual BOOL OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Touch event, such as windows 7 multi-touch.
    *
    * @param message        [I/ ] The pointer to file name.
    * @param wParam         [I/ ] The pointer to file name.
    * @param lParam         [I/ ] The pointer to file name.
    *
    * @return TRUE if handled, FALSE otherwise.
     */
    virtual BOOL OnTouchEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief The framework calls this member function to inform the CWnd object that it is being destroyed.
    */
    virtual void OnDestroy();

    /*!
    * @brief Call this function when window size is changed.
    *
    * @param w          [I/ ] The device context to be drawn.
    * @param h          [I/ ] The const reference to RECT structure.
    */
    virtual void OnSize(int w, int h);

    /*!
    * @brief The message procedure.
    *
    * @param hWnd       [I/ ] The handle to the window which occurs the message.
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    *
    * @return If you process the message, return 0, otherwise call DefWindowProc.
    */
    virtual BOOL OnWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

    /*
    * @brief The message procedure.
    *
    * @param hWnd       [I/ ] The handle to the window which occurs the message.
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    *
    * @return If you process the message, return 0, otherwise call DefWindowProc.
    */
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:

    HBITMAP                 m_hMemBitmap;               // The handle to a compatible memory bitmap.
    INT_PTR                 m_nExitCode;                // The exit code, such as IDYES, IDOK, etc.
    BYTE                    m_blendAlpha;               // The blend alpha value, 0 - 255.
    BOOL                    m_isEraseBK;                // Indicates erase background or not.
    BOOL                    m_isLBDown;                 // Indicate whether left button presses down.
    BOOL                    m_isTracked;                // Indicates system tracks the mouse or not.
    BOOL                    m_isSyncPerformEvents;      // Synchronism perform events.
    HINSTANCE               m_hInstance;                // The instance of current process.
    MouseSpeedTracker      *m_pMouseSpeedTracker;       // The instance to help to get mouse properties.
    vector<ViewElement*>    m_vctEventViews;            // The event views collection.
    map<UINT, ViewElement*> m_mapTargetViews;           // The target views collection.
    static BOOL             s_hasRegClass;              // Indicate whether register class.
};

#endif // _WINDOWFORM_H_
#endif // __cplusplus
