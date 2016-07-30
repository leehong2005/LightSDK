/*!
* @file SdkViewElement.h 
* 
* @brief This file defines the base view class for various visual elements.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _SDKVIEWELEMENT_H_
#define _SDKVIEWELEMENT_H_

#include "SdkCommonInclude.h"
#include "SdkUICommon.h"
#include "SdkAnimation.h"
#include "SdkWindow.h"
#include "D2DBrush.h"
#include "D2DBitmap.h"
#include "ID2DDeviceStateChange.h"
#include "IViewOnClickHandler.h"
#include "IViewOnKeyHandler.h"
#include "IViewOnMouseHandler.h"

USING_NAMESPACE_D2D
USING_NAMESPACE_ANIMATION
USING_NAMESPACE_WINDOW
USING_NAMESPACE_CALLBACK

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The internal data.
*/
struct _INTERNALDATA;
typedef _INTERNALDATA INTERNALDATA, *LPINTERNALDATA;

/*!
* @brief The style of the view, different values have different drawing effect.
*/
typedef enum _VIEW_STYLE
{
    VIEW_STYLE_NORMAL                       = 0x0001,           // The normal style.
    VIEW_STYLE_DISABLE                      = 0x0002,           // The disable style.
    VIEW_STYLE_MOUSEHOVER                   = 0x0003,           // The mouse over.style
    VIEW_STYLE_MOUSEDOWN                    = 0x0004,           // The mouse down style.
    VIEW_STYLE_SELECTED                     = 0x0005,           // The selected style.
    VIEW_STYLE_SELECTEDOVER                 = 0x0006,           // The selected mouse over style
    VIEW_STYLE_SELECTEDDOWN                 = 0x0007,           // The selected mouse down style.
    VIEW_STYLE_OVERLAY                      = 0x0008,           // The overlay style.

} VIEW_STYLE;

/*!
* @brief The state of view element.
*/
typedef enum _VIEW_STATE
{
    VIEW_STATE_FRONT                        = 0x00000001,       // The front state.
    VIEW_STATE_TOGGLE                       = 0x00000004,       // The toggle state.
    VIEW_STATE_SELECTED                     = 0x00000008,       // Selected state.
    VIEW_STATE_VISIBLE                      = 0x00000010,       // Visible state.
    VIEW_STATE_CLICKABLE                    = 0x00000040,       // Click able.
    VIEW_STATE_LONGCLICKABLE                = 0x00000080,       // Long-click able.
    VIEW_STATE_FOCUSABLE                    = 0x00000100,       // Indicates can focus or not.
    VIEW_STATE_ENABLE                       = 0x00000200,       // Enable state.
    VIEW_STATE_PRESSED                      = 0x00000400,       // Left button is pressed down.
    VIEW_STATE_RIGHTPRESSED                 = 0x00000800,       // Right button is pressed down.
    VIEW_STATE_DISABLEANIMMAT               = 0x00001000,       // Disable matrices of animation installed on view.
    VIEW_STATE_CLEARANIMATION               = 0x00002000,       // Clear animation when finished.
    VIEW_STATE_ALWAYSPAINTVIEW              = 0x00004000,       // Always paint view although view is out of parent's bound.
    VIEW_STATE_ROUNDCORNERENABLE            = 0x00008000,       // Show round corner.
    VIEW_STATE_CLIPVIEW                     = 0x00020000,       // Clip view, it is useful for clip background image with round corner rectangle.
    VIEW_STATE_NOTPAINTBK                   = 0x00040000,       // Not paint background image.
    VIEW_STATE_POPUP                        = 0x00080000,       // Popup property.
    VIEW_STATE_CANCELEVENT                  = 0x00100000,       // Cancel event.
    VIEW_STATE_DISABLECANCELEVENT           = 0x00200000,       // Disable cancel clicking.
    VIEW_STATE_PAINTALLVIEWS                = 0x00800000,       // Paint all children of a view layout.

} VIEW_STATE;

/*!
* @brief The layout information of view element.
*/
typedef struct _LAYOUTINFO
{
    FLOAT x;                                // x coordinate of left-top corner.
    FLOAT y;                                // y coordinate of left-top corner.
    FLOAT width;                            // Width of view.
    FLOAT height;                           // Height of view.
    FLOAT maxWidth;                         // Maximum width.
    FLOAT maxHeight;                        // Maximum height.
    FLOAT minWidth;                         // Minimum width.
    FLOAT minHeight;                        // Minimum height.
    FLOAT leftMargin;                       // The left margin.
    FLOAT topMargin;                        // The top margin.
    FLOAT rightMargin;                      // The right margin.
    FLOAT bottomMargin;                     // The bottom margin.

} LAYOUTINFO, *LPLAYOUTINFO;

/*!
* @brief The SdkViewElement is a base class, all visual element must inherit from it, such as ImageView, ImageButton ,etc.
*/
class CLASS_DECLSPEC SdkViewElement : public ID2DDeviceStateChange
{
public:

    friend class SdkD2DTheme;
    friend class SdkWindow;
    friend class SdkViewLayout;

    /*!
    * @brief The constructor function.
    */
    SdkViewElement();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkViewElement();

    /*!
    * @brief Call this method when need to draw some stuffs in render target.
    */
    virtual void OnPaint();

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Call this method to update view.
    *
    * @param isUpdateNow    [I/ ] Indicates whether update immediately. If TRUE it only updates
    *                             its owner bounds on the windows.
    */
    virtual void Invalidate(BOOL isUpdateNow = FALSE);

    /*!
    * @brief Set the layout information of the view.
    *
    * @param x          [I/ ] The x coordinate of left.
    * @param y          [I/ ] The y coordinate of left.
    * @param width      [I/ ] The width coordinate of left.
    * @param height     [I/ ] The height coordinate of left.
    */
    virtual void SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height);

    /*!
    * @brief Set maximum width of view.
    *
    * @param maxWidth   [I/ ] Maximum width of view.
    */
    virtual void SetMaxWidth(FLOAT maxWidth);

    /*!
    * @brief Set maximum height of view.
    *
    * @param maxHeight  [I/ ] Maximum height of view.
    */
    virtual void SetMaxHeight(FLOAT maxHeight);

    /*!
    * @brief Set minimum width of view.
    *
    * @param minWidth   [I/ ] Minimum width of view.
    */
    virtual void SetMinWidth(FLOAT minWidth);

    /*!
    * @brief Set minimum height of view.
    *
    * @param minHeight  [I/ ] Minimum width of view.
    */
    virtual void SetMinHeight(FLOAT minHeight);

    /*!
    * @brief Get the layout information of view.
    *
    * @param LAYOUTINFO [ /O] The width coordinate of left.
    */
    virtual void GetLayoutInfo(OUT LAYOUTINFO* pLayoutInfo) const;

    /*!
    * @brief Get the instance of SdkWindow.
    *
    * @return SdkWindow instance.
    */
    virtual SdkWindow* GetWindow();

    /*!
    * @brief Show or hide the view.
    *
    * @param isShow     [I/ ] The flag to indicate show or hide the view.
    */
    virtual void ShowView(BOOL isShow);

    /*!
    * @brief Specified the view' mouse event handler.
    *
    * @param pViewMouseHandler  [I/ ] The pointer to ViewEventHandler class.
    */
    virtual void SetOnMouseHandler(IN IViewOnMouseHandler *pViewMouseHandler);

    /*!
    * @brief Specified the view' key event handler.
    *
    * @param pViewKeyHandler    [I/ ] The pointer to ViewEventHandler class.
    */
    virtual void SetOnKeyHandler(IN IViewOnKeyHandler *pViewKeyHandler);

    /*!
    * @brief Set view clicking handler.
    *
    * @brief The instance of IViewOnClickHandler class or its derived class.
    */
    virtual void SetOnClickHandler(IN IViewOnClickHandler *pClickHandler);

    /*!
    * @brief Set background color of view.
    *
    * @param color          [I/ ] The view background color.
    */
    virtual void SetBkColor(IN const ColorF &color);

    /*!
    * @brief Get the background color of view.
    *
    * @param The background color.
    */
    virtual ColorF GetBkColor();

    /*
    * @brief Set the background image.
    *
    * @param uResId         [I/ ] The resource id.
    * @param hInstance      [I/ ] The address to process.
    */
    virtual BOOL SetBkImage(IN UINT32 uResId, HINSTANCE hInstance = NULL,  UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Set the background image.
    *
    * @param hBitmap        [I/ ] The handle to HBITMAP.
    */
    virtual BOOL SetBkImage(IN HBITMAP hBitmap, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Set the background image.
    *
    * @param lpfileName     [I/ ] The pointer to file name.
    * @param uDestHeight    [I/ ] The destination height.
    *
    * @return TRUE if succeeds, otherwise return FALSE.
    *
    * @remark The uDestWidth and uDestHeight default value is 0,
    *         that means destination bound is original bound.
    */
    virtual BOOL SetBkImage(IN LPCWSTR lpfileName, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Set the background brush. pBKBrush must not be pointed memory on stack.
    *
    * @param pBKBrush       [I/ ] The pointer to D2DBrush class. This pointer should point memory on heap.
    *                             SdkViewElement will delete the memory, caller do not delete again.
    */
    virtual void SetBkBrush(IN D2DBrush *pBKBrush);

    /*!
    * @brief Set animation to the view.
    *
    * @param pAnimation     [I/ ] The pointer to SdkAnimation class.
    *
    * @remark You are responsible to release the memory of animation object.
    *         And you should make sure the memory is valid until the view is
    *         destroyed.
    */
    virtual void SetAnimation(IN SdkAnimation *pAnimation);

    /*!
    * @brief Get the animation of this view.
    *
    * @return Pointer to SdkAnimation, may be NULL.
    */
    virtual SdkAnimation* GetAnimation() const;

    /*!
    * @brief Start the animation. If you did not set any animation, it is no effect.
    *
    * @param dDelaySecond     [I/ ] The delayed time.
    */
    virtual void StartAnimation(IN DOUBLE dDelaySecond = 0);

    /*!
    * @brief Clear the animation, it is the same with calling SetAnimation and pass NULL.
    */
    virtual void ClearAnimation();

    /*!
    * @brief Set the parent of the view, generally, it is called when added to a layout.
    *
    * @param pParentView    [I/ ] The parent view.
    */
    virtual void SetParent(SdkViewLayout *pParentView);

    /*!
    * @brief Get the parent view.
    *
    * @return The pointer to SdkViewLayout class.
    *
    * @remark You should NOT delete the memory pointed by the returned point.
    */
    virtual SdkViewLayout* GetParent() const;

    /*!
    * @brief Get the class name of the view.
    *
    * @return The name of this view, do not delete the pointer.
    */
    virtual LPCTSTR GetClassName();

    /*!
    * @brief Find the event source.
    *
    * @param message        [I/ ] message.
    * @param wParam         [I/ ] parameter.
    * @param lParam         [I/ ] parameter.
    *
    * @return Event source, should NOT delete this returned pointer, maybe NULL.
    */
    virtual SdkViewElement* FindEventSource(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Call this function to dispatch keyboard event.
    *
    * @param lpMsg      [I/ ] The event argument.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL DispatchKeyboardEvent(const LPMSG lpMsg);

    /*!
    * @brief Called this function to dispatch mouse event. This bubbling routing strategy
    *
    * @param lpMsg      [I/ ] The event argument.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL DispatchMouseEvent(const LPMSG lpMsg);

    /*!
    * @brief Called this function to dispatch mouse event. . This tunneling routing strategy
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL PreDispatchMouseEvent(const LPMSG lpMsg, SdkViewElement *pSource);

    /*!
    * @brief Clear the associated data with the view.
    */
    virtual void ClearAssocData();

public:

    /*!
    * @brief Check the view is click-able.
    *
    * @return TRUE if click-able, otherwise FALSE.
    */
    BOOL IsClickable();

    /*!
    * @brief Indicates whether this view reacts to long click events or not.
    *
    * @return TRUE if click-able, otherwise FALSE.
    */
    BOOL IsLongClickable();

    /*!
    * @brief Set view is click-able or not.
    *
    * @param isClickable        [I/ ] TRUE if click-able, otherwise non click-able.
    */
    void SetClickable(BOOL isClickable);

    /*!
    * @brief Set long click-able or not.
    *
    * @param isLongClickable    [I/ ] TRUE if long click-able, otherwise non click-able.
    */
    void SetLongClickable(BOOL isLongClickable);

    /*!
    * @brief Set whether this view can receive the focus.
    *
    * @param isFocusable        [I/ ] If true, this view can receive the focus.
    */
    void SetFocusable(BOOL isFocusable);

    /*!
    * @brief Select or dis-select the view.
    *
    * @param isSelected     [I/ ] TRUE if selected, otherwise FALSE.
    */
    virtual void SetSelected(BOOL isSelected);

    /*!
    * @brief Set view enable or disable.
    *
    * @param isEnable       [I/ ] TRUE if enable, otherwise FALSE.
    */
    virtual void SetEnable(BOOL isEnable);

    /*!
    * @brief Set the visibility of view.
    *
    * @param isVisible      [I/ ] TRUE if visible, FALSE is invisible.
    *
    * @remark This function is same as ShowView function, but this function will 
    *         not invalidate after calling.
    */
    virtual void SetVisible(BOOL isVisible);

    /*!
    * @brief This function set the keyboard focus to this view if the view is focusable.
    */
    void SetFocus();

    /*!
    * @brief Indicate the view is focused.
    *
    * @return TRUE if focused, FALSE otherwise.
    */
    BOOL IsFocused();

    /*!
    * @brief Check the view is selected or not.
    *
    * @return TRUE if selected, otherwise FALSE.
    */
    BOOL IsSelected();

    /*!
    * @brief Indicate this view is focus or not.
    *
    * @return TRUE if focus, otherwise return FALSE.
    */
    BOOL IsFocusable();

    /*!
    * @brief Check the view is enable or disable.
    *
    * @return TRUE if enable, otherwise is FALSE.
    */
    BOOL IsEnable();

    /*!
    * @brief Indicates whether the view is visible or invisible.
    */
    BOOL IsVisible();

    /*!
    * @brief Enable or disable round corner.
    *
    * @param fEnable        [I/ ] TRUE if enable, FALSE if disable
    */
    void SetRoundCornerEnable(BOOL fEnable);

    /*!
    * @brief Set Round corner radius.
    *
    * @param fRadiusX       [I/ ] The radius of x.
    * @param fRadiusY       [I/ ] The radius of y.
    */
    void SetRoundCornerRadius(FLOAT fRadiusX = 10.0f, FLOAT fRadiusY = 10.0f);

    /*!
    * @brief Set border width, maximum value is 4.0f.
    *
    * @param nBorderWidth   [I/ ] Border width.
    */
    void SetBorderWidth(FLOAT fBorderWidth = 1.0f);

    /*!
    * @brief Set border color.
    *
    * @param borderColor    [I/ ] Border color.
    */
    void SetBorderColor(const ColorF& borderColor);

    /*!
    * @brief Set the tag of view element.
    *
    * @param pTag       [I/ ] Set the tag of view element.
    */
    void SetTag(PVOID pTag);

    /*!
    * @brief Get the tag of the view.
    */
    PVOID GetTag() const;

    /*!
    * @brief Set the id of view.
    *
    * @param id         [I/ ] The id of the view.
    */
    void SetId(UINT id);

    /*!
    * @brief Get the id of view.
    *
    * @return The id of view.
    */
    INT32 GetId() const;

    /*!
    * @brief Translate the view by specified offset.
    *
    * @param dx     [I/ ] The offset on x axis.
    * @param dy     [I/ ] The offset on y axis.
    * @param dz     [I/ ] The offset on z axis. Reserved, not used.
    */
    void Translate(FLOAT dx, FLOAT dy, FLOAT dz = 0.0f);

    /*!
    * @brief Scale the view by specified ratio.
    *
    * @param sx         [I/ ] The ratio on x axis.
    * @param sy         [I/ ] The ratio on y axis.
    * @param centerPt   [I/ ] The scale center point.
    *
    * @remark The center point is relative to itself, not absolute coordinate.
    */
    void Scale(FLOAT sx, FLOAT sy, POINTF centerPt = POINTF());

    /*!
    * @brief Rotate the view by specified angle.
    *
    * @param angle      [I/ ] The rotate angle.
    * @param centerPt   [I/ ] The scale center point.
    *
    * @remark The center point is relative to itself, not absolute coordinate.
    */
    void Rotate(FLOAT angle, POINTF centerPt = POINTF());

    /*!
    * @brief Set the view position.
    *
    * @param x      [I/ ] position x.
    * @param y      [I/ ] position y.
    */
    void SetViewPos(FLOAT x, FLOAT y);

    /*!
    * @brief Set view size.
    *
    * @param w      [I/ ] view's width.
    * @param h      [I/ ] view's height.
    */
    void SetViewSize(FLOAT w, FLOAT h);

    /*!
    * @brief Get the left value of view.
    *
    * @return Left value, it is relative to its parent value.
    */
    FLOAT GetLeft() const;

    /*!
    * @brief Get the top value of view.
    *
    * @return Top value, it is relative to its parent value.
    */
    FLOAT GetTop() const;

    /*!
    * @brief Get the right value of view.
    *
    * @return Right value.
    */
    FLOAT GetRight() const;

    /*!
    * @brief Get the bottom value of view.
    *
    * @return Bottom value.
    */
    FLOAT GetBottom() const;

    /*!
    * @brief Get the width value of view.
    *
    * @return Width value.
    */
    FLOAT GetWidth() const;

    /*!
    * @brief Get the height value of view.
    *
    * @return Height value.
    */
    FLOAT GetHeight() const;

    /*!
    * @brief Check the specified point value whether locates in the view's bound.
    *
    * @param x      [I/ ] The point x value.
    * @param y      [I/ ] The point y value.
    *
    * @return TRUE if point is in bound, otherwise return FALSE.
    */
    BOOL IsPtInRect(FLOAT x, FLOAT y);

    /*!
    * @brief Request to layout the view, after calling of this function, layout will reset.
    */
    void RequestLayout();

    /*!
    * @brief Get the bound of view element.
    *
    * @return The object of D2D1_RECT_F, the view rectangle.
    */
    D2D1_RECT_F GetViewRect();

    /*!
    * @brief Get drawing rectangle of view.
    *
    * @return Drawing rectangle.
    *
    * @remark This drawing rectangle is relative to its parent view.
    */
    D2D1_RECT_F GetDrawingRect();

    /*!
    * @brief Change the view's z order in the tree, so it's on top of other sibling views.
    */
    void BringToFront();

    /*!
    * @brief Cancel the event of this view. This is useful when user drags the view or layt
    *
    * @param isCancel   [I/ ] TRUE if cancel, otherwise FALSE.
    */
    void CancelViewEvent(BOOL isCancel = TRUE);

    /*!
    * Get view state flag.
    *
    * @return state flag, combined with values of VIEW_STATE.
    */
    INT32 GetState() const;

protected:

    /*!
    * @brief Called when window is destroy.
    *
    * @param pWindow    [I/ ] The window to be destroy.
    */
    virtual void OnWindowDestroy(SdkWindow *pWindow);

    /*!
    * @brief Called when the layout of view is changed.
    *
    * @param fChanged       [I/ ] Indicates whether is changed.
    * @param left           [I/ ] The left value.
    * @param top            [I/ ] The top value.
    * @param width          [I/ ] The width of view.
    * @param height         [I/ ] The height of view.
    */
    virtual void OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

    /*!
    * @brief Call this method to draw view's border.
    *
    * @param pRenderTarget  [I/ ] Render target instance.
    * @param absRc          [I/ ] Absolute rectangle.
    */
    virtual void OnDrawBoarder(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F& absRc);

    /*!
    * @brief Call this method to draw view's background, background image or background color.
    *
    * @param pRenderTarget  [I/ ] Render target instance.
    * @param absRc          [I/ ] Absolute rectangle.
    */
    virtual void OnDrawBackground(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F& absRc);

    /*!
    * @brief Mouse event.
    *
    * @param message        [I/ ] message.
    * @param wParam         [I/ ] parameter.
    * @param lParam         [I/ ] parameter.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Keyboard event.
    *
    * @param message        [I/ ] message.
    * @param wParam         [I/ ] parameter.
    * @param lParam         [I/ ] parameter.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnKeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Called when the view's focus has changed.
    *
    * @param pView          [I/ ] The pointer to the view that lost or gets the focus, may be NULL.
    * @param fGetFocus      [I/ ] Indicates if the view has gained or lost the focus.
    *                             TRUE if view has gained focus, FALSE otherwise.
    */
    virtual void OnFocusChanged(SdkViewElement *pView, BOOL fGetFocus);

    /*!
    * @brief Called when the view' visibility is changed.
    *
    * @param pView          [I/ ] The pointer to the view that visibility is changed, may be this or its parent.
    * @param fVisible       [I/ ] Indicate the view is visible or invisible.
    */
    virtual void OnVisibilityChanged(SdkViewElement *pView, BOOL fVisible);

    /*!
    * @brief Called when the view's selection has changed.
    *
    * @param pView          [I/ ] The pointer to the view that lost or gets the focus, may be NULL.
    * @param fSelected      [I/ ] Indicated this view is selected or not.
    */
    virtual void OnSelectChenaged(SdkViewElement *pView, BOOL fSelected);

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

protected:

    /*!
    * @brief Set the window instance to the view.
    *
    * @param pWindow        [I/ ] The pointer to SdkWindow.
    */
    virtual void SetWindow(SdkWindow *pWindow);

    /*!
    * @brief Specifies a shape to which all subsequent drawing operations are clipped.
    *
    * @param pRenderTarget  [I/ ] The render target.
    */
    virtual void PushClip(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Removes the last clip shape from the render target. After this method is called, 
    *        the clip is no longer applied to subsequent drawing operations.
    *
    * @param pRenderTarget  [I/ ] The render target.
    */
    virtual void PopClip(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Perform the click event handler.
    *
    * @return TRUE there was an assigned OnClickListener that was called, FALSE
    *         otherwise is returned.
    */
    virtual BOOL PerformClick();

    /*!
    * @brief Perform the long click event handler.
    *
    * @return TRUE there was an assigned OnClickListener that was called, FALSE
    *         otherwise is returned.
    */
    virtual BOOL PerformLongClick();

    /*!
    * @brief Called this function to dispatch mouse event.
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL PerformOnMouseEvent(const LPMSG lpMsg, SdkViewElement *pSource);

    /*!
    * @brief Called this function to dispatch key event.
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL PerformOnKeyboardEvent(const LPMSG lpMsg, SdkViewElement *pSource);

protected:

    /*!
    * @brief Get current matrix, includes animation and transform matrices.
    *
    * @return The Matrix3x2F object.
    */
    Matrix3x2F GetCurMatrix();

    /*!
    * @brief Get the matrix of T, S and R.
    *
    * @return The multiply to T, S and R.
    */
    Matrix3x2F GetTSRMatrix();

    /*!
    * @brief Get the matrix of animation.
    */
    Matrix3x2F GetAnimationMatrix();

    /*!
    * @brief Check the animation matrix is disable or enable, sometimes, we use animation
    *        to change some value of view, such as width, left, .etc, in this case, we should
    *        not use animation matrix to change view's appearance.
    *
    * @return TRUE if animation matrix is enable, FALSE is disable.
    */
    BOOL IsAnimMatrixEnable();

    /*!
    * @brief Get the absolute bound of current view.
    *
    * @param outRc          [ /O] The reference to RECT.
    */
    void GetAbsoluteRect(OUT RECT& outRc);

    /*!
    * @brief Get the absolute bound of current view.
    *
    * @param outRc          [ /O] The reference to D2D1_RECT_F.
    */
    void GetAbsoluteRect(OUT D2D1_RECT_F& outRc);

    /*!
    * @brief Convert a input point to absolute point.
    *
    * @param outPt          [I/O] The reference to POINT.
    */
    void GetAbsolutePoint(IN OUT POINT& outPt);

    /*!
    * @brief Convert a input point to absolute point.
    *
    * @param outPt          [I/O] The reference to POINT.
    */
    void GetAbsolutePoint(IN OUT D2D1_POINT_2F& outPt);

    /*!
    * @brief Convert a input point to absolute point.
    *
    * @return.The matrix, multiplied by every parent view's matrix.
    */
    Matrix3x2F GetAbsoluteMatrix();

    /*!
    * @brief Convert specified point to another point, if no matrix takes on the view, 
    *        returned point is same with source point.
    *
    * @param srcPt      [I/ ] The source point, typically is mouse or touch point.
    * @param fRelative  [I/ ] Indicate the destination point is relative to the view or
    *                         the destination point is absolute, defalut value is TRUE.
    *
    * @return Converted point, it is relative to view's layout information.
    */
    D2D1_POINT_2F ConvertPoint(IN const D2D1_POINT_2F& srcPt, BOOL fRelative = TRUE);

    /*!
    * @brief Check should cancel perform click operation.
    *
    * @param xPos       [I/ ] The x position of mouse pointer.
    * @param yPos       [I/ ] The y position of mouse pointer.
    *
    * @return TRUE if should cancel to perform, otherwise FALSE.
    */
    BOOL IsCancelClick(LONG xPos, LONG yPos);

    /*!
    * @brief Set the view style.
    *
    * @param style      [I/ ] The view style value.
    */
    void SetStyle(VIEW_STYLE style);

    /*!
    * @brief Get the view's style.
    *
    * @return The value of VIEW_STYLE.
    */
    VIEW_STYLE GetStyle() const;

    /*!
    * @brief Set the flag of view state.
    *
    * @param state      [I/ ] The value of VIEW_STATE to bee added.
    */
    void AddFlag(VIEW_STATE state);

    /*!
    * @brief Remove the state of view.
    *
    * @param state      [I/ ] The value of VIEW_STATE to be removed.
    */
    void RemoveFlag(VIEW_STATE state);

    /*!
    * @brief Get the T, S, R matrix.
    *
    * @remark This function should be called in OnPaint function.
    */
    void CombineTSRMatrix();

    /*!
    * @brief Force to update view.
    */
    void ForceInvalidate();

    /*!
    * @brief Call this method to set the class name, typically, it is called in derived
    *        class and the memory pointed by lpClassName should not be in stack, instead existing
    *        in static storage memory.
    *
    * @param lpClassName    [I/ ] The class name, should not point a memory allocated at stack.
    *                             typically the parameter is macro NAME_XXXX defined in SdkUICommon.h.
    */
    void SetClassName(LPCTSTR lpClassName);

protected:

    LAYOUTINFO              m_layoutInfo;           // The layout information.
    SdkWindow              *m_pWindow;              // The pointer to SdkWindow, should NOT delete the memory pointed by this pointer.
    INTERNALDATA           *m_pInternalData;        // The internal data of this class.
};

END_NAMESPACE_VIEWS

#endif //_SDKVIEWELEMENT_H_
#endif // __cplusplus
