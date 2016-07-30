/*!
* @file SdkSlideLayout.h 
* 
* @brief This file defines class SdkSlideLayout, which supports drag to slide and sliding animation after dragging.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/28
*/

#ifdef __cplusplus
#ifndef _SDKSLIDELAYOUT_H_
#define _SDKSLIDELAYOUT_H_

#include "SdkCommonInclude.h"
#include "SdkSlideBase.h"
#include "SdkAdapterView.h"
#include "SdkScrollBar.h"
#include "ISlideBaseEventHandler.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief SdkSlideLayout class supports drag to slide and sliding animation.
*/
class CLASS_DECLSPEC SdkSlideLayout : public SdkAdapterView, public ISlideBaseEventHandler
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkSlideLayout();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkSlideLayout();

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
    * @brief Get the children at specified index in the view layout.
    *
    * @param index      [I/ ] The index of child in the view layout.
    * @param ppChild    [ /O] The output SdkViewElement instance.
    *
    * @return TRUE if succeed to add, otherwise return FALSE.
    */
    virtual BOOL GetChildAt(UINT index, OUT SdkViewElement **ppChild);

    /*!
    * @brief Get the children count in the view layout.
    *
    * @return The children count.
    */
    virtual int GetChildCount();

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
    * @brief Remove all children view.
    *
    * @param isClearCache   [I/ ] Flag to indicate clear cache views or not.
    *
    * @return True: remove success.
    *               remove failure
    */
    virtual BOOL RemoveAllChildren(BOOL isClearCache = TRUE);

    /*!
    * @brief Set the sliding direction.
    *
    * @param slideDirection     [I/ ] One value of SLIDEDIRECTIOIN enumeration.
    */
    void SetSlideDirection(SLIDEDIRECTIOIN slideDirection);

    /*!
    * @brief Get the sliding direction.
    *
    * @return Slide direction.
    */
    SLIDEDIRECTIOIN GetSlideDirection();

    /*!
    * @brief Set sliding enable or disable.
    *
    * @param isSlidingEnable    [I/ ] TRUE if enable, FALSE is disable.
    */
    void SetSlideEnable(BOOL isSlideEnable);

    /*!
    * @brief Check sliding is enable or disable.
    *
    * @return TRUE if enable, FALSE if disable.
    */
    BOOL IsSlideEnable();

    /*!
    * @brief Enable or disable sliding animation.
    *
    * @param isEnable   [I/ ] TRUE if enable, FALSE otherwise.
    */
    void SetSlideAnimationEnable(BOOL isEnable = TRUE);

    /*!
    * @brief Indicates sliding animation enable or disable.
    *
    * @return TRUE if enable, FALSE otherwise.
    */
    BOOL IsSlideAnimationEnable();

    /*!
    * @brief Get slide offset.
    *
    * @return Offset value, on x-coordinate or y-coordinate.
    */
    FLOAT GetSlideOffset();

    /*!
    * @brief Set the slide step.
    *
    * @param fStep      [I/ ] The step for slide, -1.0 indicates not use this step.
    */
    void SetSlideStep(FLOAT fStep = -1.0f);

    /*!
    * @brief Show horizontal scroll bar.
    *
    * @param isVisible  [I/ ] Indicates scrollbar is show or hide.
    */
    void SetHScrollBarVisible(BOOL isVisible);

    /*!
    * @brief Show vertical scroll bar.
    *
    * @param isVisible  [I/ ] Indicates scrollbar is show or hide.
    */
    void SetVScrollBarVisible(BOOL isVisible);

    /*!
    * @brief Reset the offset for sliding.
    *
    * @param offset [I/ ] The offset value.
    */
    void ResetOffset(FLOAT offset = 0, BOOL isCreateView = TRUE);

    /*!
    * @brief Get the slide base class object.
    *
    * @return Pointer to SdkSlideBase class.
    */
    SdkSlideBase* GetSlideBase() const;

protected:

    /*!
    * @brief Set the slide range, in other word, set the maximum size of sliding layout.
    *
    * @param uMaxWidth      [I/ ] The maximum width of sliding layout.
    * @param uMaxHeight     [I/ ] The maximum height of sliding layout.
    */
    void SetSlideRange(UINT uMaxWidth, UINT uMaxHeight);

    /*!
    * @brief Called when view's offset is changed.
    *
    * @param pView      [I/ ] The view calls this function.
    * @param offstX     [I/ ] The offset on x-coordinate.
    * @param offstY     [I/ ] The offset on y-coordinate.
    */
    virtual void OnOffsetChanged(SdkSlideBase *pView, FLOAT offsetX, FLOAT offsetY);

    /*!
    * @brief Called when view finishes moving.
    *
    * @param pView      [I/ ] The view calls this function.
    */
    virtual void OnFinishMoving(SdkSlideBase *pView);

    /*!
    * @brief Called when view begin moving.
    *
    * @param pView      [I/ ] The view calls this function.
    */
    virtual void OnBeginMoving(SdkSlideBase *pView);

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

    UINT                  m_uSlidingMaxWidth;       // Maximum sliding width.
    UINT                  m_uSlidingMaxHeight;      // Maximum sliding height.
    SdkSlideBase         *m_pSlideBase;             // The SdkSlideBase instance.
    SdkScrollBar         *m_pScrollBar;             // The scroll bar.
};

END_NAMESPACE_VIEWS

#endif // _SDKSLIDELAYOUT_H_
#endif // __cplusplus
