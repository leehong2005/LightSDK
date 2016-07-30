/*!
* @file SlideLayout.h 
* 
* @brief This file defines class SdkSlideBase, which supports drag to slide and sliding animation after dragging.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/28
*/

#ifdef __cplusplus
#ifndef _SDKSLIDEBASE_H_
#define _SDKSLIDEBASE_H_

#include "SdkCommonInclude.h"
#include "SdkTranslateAnimation.h"
#include "SdkViewLayout.h"
#include "ISlideBaseEventHandler.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The enumeration for slide direction.
*/
typedef enum _SLIDEDIRECTIOIN
{
    SLIDEDIRECTIOIN_HORIZONTAL      = 1,
    SLIDEDIRECTIOIN_VERTICAL        = 2,

} SLIDEDIRECTIOIN;


/*!
* @brief SdkSlideBase class supports drag to slide and sliding animation.
*/
class CLASS_DECLSPEC SdkSlideBase : public SdkViewLayout
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkSlideBase();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkSlideBase();

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
    * @brief Set offset changing handler of this view.
    *
    * @param ISlideBaseEventHandler  [I/ ] The handler object.
    */
    void SetOffsetChangedHandler(ISlideBaseEventHandler *pEventHandler);

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
    * @brief Get slide step.
    *
    * @return step value, on x-coordinate or y-coordinate.
    */
    FLOAT GetSlideStep();

    /*!
    * @brief OffsetLayout current layout.
    *
    * @param offsetX    [I/ ] Offset on X direction.
    * @param offsetY    [I/ ] Offset on Y direction.
    */
    virtual void OffsetViewLayout(FLOAT offsetX, FLOAT offsetY);

    /*!
    * @brief Call this method when need to draw some stuffs in render target.
    */
    virtual void OnPaint();

protected:

    /*!
    * @brief Mouse event.
    *
    * @param message        [I/ ] The pointer to file name.
    * @param wParam         [I/ ] The pointer to file name.
    * @param lParam         [I/ ] The pointer to file name.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief This method can be called when the animation has ended.
    */
    virtual void OnAnimationEnd(SdkAnimation *pAnimation);

    /*!
    * @brief Called when view finishes moving.
    */
    virtual void OnFinishMoving();

    /*!
    * @brief Called when view begins moving.
    */
    virtual void OnBeginMoving();

    /*!
    * @brief Start revert animation.
    *
    * @return TRUE if succeeds to start animation, FALSE otherwise.
    */
    virtual BOOL StartRevertAnimation();

    /*!
    * @brief Start to play sliding animation.
    *
    * @param fromX      [I/ ] The start point on X direction.
    * @param toX        [I/ ] The end point on X direction.
    * @param fromY      [I/ ] The start point on Y direction.
    * @param toY        [I/ ] The end point on Y direction.
    *
    * @return TRUE if succeeds to start animation, FALSE otherwise.
    */
    virtual BOOL StartSlideAnimation(FLOAT fromX, FLOAT toX, FLOAT fromY, FLOAT toY);

private:

    BOOL                    m_hasMoved;                 // Indicates whether has moved.
    BOOL                    m_isSlideEnable;            // Indicates whether sliding enable or disable.
    BOOL                    m_isSlideAniEnable;         // Indicates whether sliding animation enable or disable.
    FLOAT                   m_fSlideStep;               // The sliding step.
    SLIDEDIRECTIOIN         m_slideDirection;           // Sliding direction.
    D2D1_POINT_2F           m_ptDown;                   // The touch point.
    SdkTranslateAnimation  *m_pTransAnim;               // The translate animation.
    ISlideBaseEventHandler *m_pEventHandler;            // The event handler.
};

END_NAMESPACE_VIEWS

#endif // _SDKSLIDEBASE_H_
#endif // __cplusplus
