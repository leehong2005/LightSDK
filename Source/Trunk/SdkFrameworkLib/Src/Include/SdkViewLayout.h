/*!
* @file IViewEvent.h 
* 
* @brief This file defines events interfaces for SdkViewElement class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _SDKVIEWLAYOUT_H_
#define _SDKVIEWLAYOUT_H_

#include "SdkViewElement.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief SdkViewLayout is a container of various View elements.
*/
class CLASS_DECLSPEC SdkViewLayout : public SdkViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkViewLayout();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkViewLayout();

    /*!
    * @brief Call this method when need to draw some stuffs in render target.
    */
    virtual void OnPaint();

    /*!
    * @brief Called when to draw children.
    */
    virtual void OnDrawChildren();

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
    virtual BOOL AddView(IN const SdkViewElement *pChild);

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
    virtual BOOL AddView(const SdkViewElement *pChild, UINT uIndex);

    /*!
    * @brief Bring a specified view to front.
    *
    * @param pChild     [I/ ] The child view.
    *
    * @return TRUE if succeeds, otherwise FALSE.
    */
    virtual BOOL BringViewToFront(IN SdkViewElement *pChild);

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
    * @param ppChild    [ /O] The output SdkViewElement instance.
    *
    * @return TRUE if succeed to add, otherwise return FALSE.
    */
    virtual BOOL GetChildAt(UINT index, OUT SdkViewElement **ppChild);

    /*!
    * @brief Get the child by id in the view layout.
    *
    * @param id         [I/ ] The id of the child.
    * @param ppChild    [ /O] The output SdkViewElement instance.
    *
    * @return TRUE if succeed to add, otherwise return FALSE.
    */
    virtual BOOL GetChildById(int id, SdkViewElement **ppChild);

    /*!
    * @brief Get the index of child view.
    *
    * @param pChild    [I/ ] The output SdkViewElement instance.
    *
    * @return child view index. -1 if does not exist.
    */
    virtual INT32 GetIndexOfChild(SdkViewElement *pChild);

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
    * @brief Remove specified child view. This function will not delete the
    *        memory of pChild parameter determined by the parameter fToCache.
    *        if fToCache is TRUE, do not delete the memory and push it to cache vector,
    *        if fToCache is FLASE, delete the memory.
    *
    * @param pChild         [I/ ] The child view to be removed.
    * @param fToCache       [I/ ] The flag to indicate push the removed view to cache or not.
    *
    * @return True: remove success. remove failure.
    */
    virtual BOOL RemoveChild(SdkViewElement *pChild, BOOL fToCache = TRUE);

    /*!
    * @brief Remove the child view that specified child index.
    *
    * @param index      [I/ ] the child view.
    *
    * @return True: remove success.
    *               remove failure.
    */
    virtual BOOL RemoveChildAt(UINT index);

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
    * @brief Called this function to dispatch mouse event. . This tunneling routing strategy
    *
    * @param lpMsg      [I/ ] The event argument.
    * @param pSource    [I/ ] The event's source.
    *
    * @return TRUE if the event is handled, otherwise return FALSE.
    */
    virtual BOOL PreDispatchMouseEvent(const LPMSG lpMsg, SdkViewElement *pSource);

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
    * @brief Update the children's state
    */
    virtual void UpdateChildrenState();

    /*!
    * @brief Reset event target and source view.
    *
    * @param pView  [I/ ] The current view.
    */
    virtual void ClearEventViews(SdkViewElement *pView);

    /*!
    * @brief Set the SdkWindow object to the view. In this method, we should set
    *        this object to each child.
    */
    virtual void SetWindow(SdkWindow *pWindow);

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

protected:

    vector<SdkViewElement*>     m_vctChildren;              // The children list.
    vector<SdkViewElement*>     m_vctRemovedChildren;       // The removed list.
};

END_NAMESPACE_VIEWS

#endif // _SDKVIEWLAYOUT_H_
#endif // __cplusplus
