/*!
* @file IViewEvent.h 
* 
* @brief This file defines events interfaces for ViewElement class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _VIEWLAYOUT_H_
#define _VIEWLAYOUT_H_

#include "CommonInclude.h"
#include "D2DRectUtility.h"
#include "ViewElement.h"

/*!
* @brief ViewLayout is a container of various View elements.
*/
class CLASS_DECLSPEC ViewLayout : public ViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    ViewLayout();

    /*!
    * @brief The destructor function.
    */
    virtual ~ViewLayout();

    /*!
    * @brief Call this method when need to draw some stuffs in render target.
    */
    virtual void OnPaint();

    /*!
    * @brief Call this method to paint views in specified bound on the specified device context.
    *
    * @param hDC        [I/ ] The device context to draw.
    * @param lpRect     [I/ ] The bound to draw.
    *
    * @remark User can call this function to paint views on specified device context, in this function,
    *         the type of render target is DC type.
    */
    virtual void OnPaintEx(HDC hDC, const LPRECT lpRect);

    /*!
    * @brief Called when to draw children.
    */
    virtual void OnDrawChildren();

    /*!
    * @brief Called when to draw children.
    *
    * @param lpRect     [I/ ] The bound to draw.
    */
    virtual void OnDrawChildrenEx(const LPRECT lpRect);

    /*!
    * @brief Add a view to the layout, the layout is the parent view of the added view.
    *
    * @param pChild      [I/ ] The child view.
    *
    * @return TRUE if succeed to add, otherwise return FALSE.
    *
    * @remark You need not release the memory of the child view, the layout will
    *         release them when the layout view destroy.
    */
    virtual BOOL AddView(IN const ViewElement *pChild);

    /*!
    * @brief Add a view to the layout, the layout is the parent view of the added view.
    *
    * @param pChild      [I/ ] The child view.
    * @param uIndex      [I/ ] the child view index.
    *
    * @return TRUE if succeed to add, otherwise return FALSE.
    *
    * @remark You need not release the memory of the child view, the layout will
    *         release them when the layout view destroy.
    */
    virtual BOOL AddView(const ViewElement *pChild, UINT uIndex);

    /*!
    * @brief Bring a specified view to front.
    *
    * @param pChild     [I/ ] The child view.
    *
    * @return TRUE if succeeds, otherwise FALSE.
    */
    virtual BOOL BringViewToFront(IN ViewElement *pChild);

    /*!
    * @brief Get the children count in the view layout.
    *
    * @return The children count.
    */
    virtual int  GetChildCount();

    /*!
    * @brief Get the children at specified index in the view layout.
    *
    * @param index      [I/ ] The index of child in the view layout.
    * @param ppChild    [ /O] The output ViewElement instance.
    *
    * @return TRUE if succeed to add, otherwise return FALSE.
    */
    virtual BOOL GetChildAt(UINT index, OUT ViewElement **ppChild);

    /*!
    * @brief Get the child by id in the view layout.
    *
    * @param id         [I/ ] The id of the child.
    * @param ppChild    [ /O] The output ViewElement instance.
    *
    * @return TRUE if succeed to add, otherwise return FALSE.
    */
    virtual BOOL GetChildById(int id, ViewElement **ppChild);

    /*!
    * @brief Get the index of child view.
    *
    * @param pChild    [I/ ] The output ViewElement instance.
    *
    * @return child view index. -1 if does not exist.
    */
    virtual INT32 GetIndexOfChild(ViewElement *pChild);

    /*!
    * @brief Remove all children view.
    *
    * @param isClearCache   [I/ ] Flag to indicate clear cache views or not.
    *
    * @return True: remove success.
    *               remove failure
    */
    virtual BOOL RemoveAllChildren(BOOL isClearCache = TRUE);

    /*!
    * @param start    [I/ ] The start position .
    * @param count    [I/ ] The count of deleted children.
    *
    * @return True: remove success. FALSE, remove failure
    */
    virtual BOOL RemoveChildren(UINT start, UINT count);

    /*!
    * @brief Remove specified child view. This function will not delete the memory of pChild parameter.
    *
    * @param pChild    [I/ ] the child view.
    *
    * @return True: remove success. remove failure.
    */
    virtual BOOL RemoveChild(ViewElement *pChild);
    
    /*!
    * @brief Remove the child view that specified child index.
    *
    * @param index    [I/ ] the child view.
    *
    * @return True: remove success.
    *               remove failure.
    */
    virtual BOOL RemoveChildAt(UINT index);
    
    /*!
    * @brief Remove the child view that specified child id.
    *
    * @param id    [I/ ] the child id.
    *
    * @return True: remove success.
    *               remove failure.
    */
    virtual BOOL RemoveChildById(UINT id);

    /*!
    * @brief Set the Padding of the layout.
    *
    * @param left     [I/ ] the left padding.
    * @param top      [I/ ] the top padding.
    * @param right    [I/ ] the right padding.
    * @param bottom   [I/ ] the bottom padding.
    */
    virtual void SetPadding(FLOAT left, FLOAT top, FLOAT right,FLOAT bottom);

    /*!
    * @brief Set the Padding of the layout.
    *
    * @param paddingValue       [I/] All padding's value.
    */
    virtual void SetPadding(FLOAT paddingValue);

    /*!
    * @brief Find the event source.
    *
    * @param message        [I/ ] message.
    * @param wParam         [I/ ] parameter.
    * @param lParam         [I/ ] parameter.
    *
    * @return Event source, should NOT delete this returned pointer, maybe NULL.
    */
    virtual ViewElement* FindEventSource(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Called this function to dispatch message event. such as WM_COPYDATA.
    *
    * @param lpMsg      [I/ ] The event argument.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL DispatchMessageEvent(const LPMSG lpMsg);

    /*!
    * @brief Called this function to dispatch mouse event. . This tunneling routing strategy
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL PreDispathMouseEvent(const LPMSG lpMsg, ViewElement *pSource);

    /*!
    * @brief Called this function to dispatch touch event. . This tunneling routing strategy
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL PreDispathTouchEvent(const LPMSG lpMsg, ViewElement *pSource);

    /*!
    * @brief Show horizontal scroll bar.
    *
    * @param isVisible  [I/ ] Indicates scrollbar is show or hide.
    */
    virtual void SetHScrollBarVisible(BOOL isVisible);

    /*!
    * @brief Show vertical scroll bar.
    *
    * @param isVisible  [I/ ] Indicates scrollbar is show or hide.
    */
    virtual void SetVScrollBarVisible(BOOL isVisible);

    /*!
    * @brief Indicates horizontal scroll bar is visible or invisible.
    *
    * @return TRUE if visible, FALSE otherwise.
    */
    virtual BOOL IsHScrollBarVisible();

    /*!
    * @brief Indicates vertical scroll bar is visible or invisible.
    *
    * @return TRUE if visible, FALSE otherwise.
    */
    virtual BOOL IsVScrollBarVisible();

    /*!
    * @brief Clear cache views.
    */
    virtual void ClearCacheView();

    /*!
    * @brief Clear the associated data with the view.
    */
    virtual void ClearAssocData();

protected:

    /*!
    * @brief Reset event target and source view.
    *
    * @param pView  [I/ ] The current view.
    */
    virtual void ClearEventTargetAndSource(ViewElement *pView);

    /*!
    * @brief Call this method to reset children's index.
    */
    virtual void ResetChildrenIndex();

    /*!
    * @brief Set the Window object to the view. In this method, we should set
    *        this object to each child.
    */
    virtual void SetWindow(Window *pWindow);

    /*!
    * @brief Called when window is destroy.
    *
    * @param pWindow    [I/ ] The window to be destroy.
    */
    virtual void OnWindowDestroy(Window *pWindow);

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

protected:

    BOOL                    m_isHScrollbarVisible;      // Horizontal scrollbar visible
    BOOL                    m_isVScrollbarVisible;      // Vertical scrollbar visible
    vector<ViewElement*>    m_vctChildren;              // The children list.
    vector<ViewElement*>    m_vctRemovedChildren;       // The removed list.
};

#endif // _VIEWLAYOUT_H_
#endif // __cplusplus
