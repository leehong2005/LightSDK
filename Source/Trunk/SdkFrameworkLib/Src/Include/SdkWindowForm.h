/*!
* @file SdkWindowForm.h 
* 
* @brief This file defines functions to create, show and process messages for window.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _SDKWINDOWFORM_H_
#define _SDKWINDOWFORM_H_

#include "SdkWindow.h"

BEGIN_NAMESPACE_WINDOW

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
    LPCTSTR     lpszClass;              // SdkWindow's class name.

} CREATEWINDOWPARAMS, *LPCREATEWINDOWPARAMS;

/*!
* @brief The main window of application.
*/
class CLASS_DECLSPEC SdkWindowForm : public SdkWindow
{
public:

    /*
    * @brief The default constructor function.
    */
    SdkWindowForm();

    /*
    * @brief The default destructor function.
    */
    virtual ~SdkWindowForm();

    /*!
    * @brief Call this function to show main window.
    *
    * @param hInstance      [I/ ] The instance of current process to run main window.
    * @param hWndParent     [I/ ] The parent of the window, usually set NULL.
    * @param nCmdShow       [I/ ] Specifies the window show state.
    *
    * @return TRUE if success to initialize and show window, otherwise return FALSE.
    */
    virtual BOOL CreateWindowForm(HINSTANCE hInst, HWND hWndParent, INT32 nCmdShow = SW_SHOW);

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

protected:

    /*!
    * @brief Update the window.
    *
    * @return TRUE if succeed, FALSE otherwise.
    */
    virtual BOOL InitializeDevice();

    /*
    * @brief Dispatch mouse events to view element.
    *
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    *
    * @return Handled return TRUE, otherwise FALSE.
    */
    virtual BOOL DispatchMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*
    * @brief Dispatch keyboard events to view element.
    *
    * @param message    [I/ ] The message.
    * @param lParam     [I/ ] The high bit of parameter.
    * @param wParam     [I/ ] The low bit of parameter.
    *
    * @return Handled return TRUE, otherwise FALSE.
    */
    virtual BOOL DispatchKeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Push the view to event chain to perform some event procedure. This function is 
    *        useful when perform OnClick or OnLongClick callback, we want to perform all callbacks
    *        after all event are processed.
    *
    * @param pView      [I/.] The pointer to SdkViewElement.
    */
    virtual void AddEventViews(SdkViewElement *pView);

    /*!
    * @brief Call this method to perform view event.
    *
    * @param message    [I/ ] The message.
    */
    virtual void PerformViewEvent(UINT message);

    /*!
    * @brief Show or hide the current pop-up view.
    *
    * @param x          [I/ ] The x of mouse point.
    * @param y          [I/ ] The t of mouse point.
    *
    * @return TRUE if succeeds to perform the operation, FALSE otherwise.
    */
    virtual BOOL HideCurPopupView(LONG x, LONG y);

protected:

    /*!
    * @brief Call this function when it is to be drawn.
    *
    * @param hdc        [I/ ] The device context to be drawn.
    * @param rcpaint    [I/ ] The const reference to RECT structure.
    */
    virtual void OnPaint(HDC hdc, const LPRECT rcPaint);

    /*!
    * @brief Call this function when it is to be drawn with HWND render target.
    *
    * @param hdc        [I/ ] The device context to be drawn.
    * @param rcpaint    [I/ ] The const reference to RECT structure.
    */
    virtual void OnHwndPaint(HDC hdc, const LPRECT rcPaint);

    /*!
    * @brief Paint on DC render target.
    *
    * @param hdc        [I/ ] The device context to be drawn.
    * @param rcpaint    [I/ ] The const reference to RECT structure.
    */
    virtual void OnDCPaint(HDC hdc, const LPRECT rcPaint);

    /*!
    * @brief Paint in memory, not paint on window.
    *
    * @param hdc        [I/ ] The device context to be drawn.
    * @param rcpaint    [I/ ] The const reference to RECT structure.
    */
    virtual void OnMemPaint(HDC hdc, const LPRECT rcPaint);

    /*!
    * @brief Framework calls this function to draw layered window UI. This function is used
    *        only for layered window.
    *
    * @param fAlwaysDraw [I/ ] Indicate whether always to draw or not.
    * @param lprcPaint   [I/ ] Pointer to a RECT structure that to be paint.
    *
    * @return TRUE if succeeds to draw, otherwise FALSE.
    */
    virtual void OnDrawUI(BOOL fAlwaysDraw, const LPRECT lprcPaint);

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
    * @param lpCreateStruct    [I/ ] Points to a CREATESTRUCT structure that contains information 
    *                                about the WindowForm object being created.
    *
    * @remark This function is called when window receives WM_CREATE message. Client application
    *         can set window's icon, title, etc.
    */
    virtual void OnCreating(LPCREATESTRUCT lpCreateStruct);

    /*!
    * @brief The framework calls this function when an application requests that the
    *        the Windows window be created by calling the CreateWindowForm() member function.
    *
    * @param lpCreateWinParams  [I/ ] The pointer to CREATEWINDOWPARAMS structure.
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
    * @brief The keyboard event.
    *
    * @param message        [I/ ] The pointer to file name.
    * @param wParam         [I/ ] The pointer to file name.
    * @param lParam         [I/ ] The pointer to file name.
    *
    * @return TRUE if handled, FALSE otherwise.
    */
    virtual BOOL OnKeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Call this function when window size is changed.
    *
    * @param w          [I/ ] The device context to be drawn.
    * @param h          [I/ ] The const reference to RECT structure.
    */
    virtual void OnSize(int w, int h);

    /*!
    * @brief The framework calls this member function to inform the Window 
    *        object that it is being destroyed.
    */
    virtual void OnDestroy();

    /*!
    * @brief The framework calls this member function to inform the window
    *        object that it is being closed.
    */
    virtual void OnClose();

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

    HBITMAP                     m_hMemBitmap;               // The handle to a compatible memory bitmap.
    INT_PTR                     m_nExitCode;                // The exit code, such as IDYES, IDOK, etc.
    BOOL                        m_isLBDown;                 // Indicate whether left button presses down.
    BOOL                        m_isTracked;                // Indicates system tracks the mouse or not.
    BOOL                        m_isSyncPerformEvents;      // Synchronism perform events.
    HINSTANCE                   m_hInst;                    // The instance of current process.
    vector<SdkViewElement*>     m_vctEventViews;            // The event views collection.
    map<UINT, SdkViewElement*>  m_mapTargetViews;           // The target views collection.
    static BOOL                 s_hasRegClass;              // Indicate whether register class.
};

END_NAMESPACE_WINDOW

#endif // _SDKWINDOWFORM_H_
#endif // __cplusplus
