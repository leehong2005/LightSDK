/*!
* @file SdkAnimationCom.h 
* 
* @brief This file defines the common functions for animation.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _SDKANIMATIONCOMMON_H_
#define _SDKANIMATIONCOMMON_H_

#include "SdkCommon.h"
#include "SdkUICommon.h"

BEGIN_NAMESPACE_ANIMATION

/*!
* @brief This class defines the common functions for animation.
*/
class CLASS_DECLSPEC SdkAnimationCom
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkAnimationCom();

    /*
    * @brief The default destructor function.
    */
    ~SdkAnimationCom();

    /*!
    * @brief Initialize the common instance.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    *
    * @remark The instance must be initialized before first used.
    */
    HRESULT Initialize();

    /*!
    * @brief Get the finish flag.
    *
    * @param pIsFinish      [ /O]   The flag whether all the animation had finished or not.

    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT IsFinish(OUT BOOL *pIsFinish);

    /*!
    * @brief Get current time.
    *
    * @param pSeconds       [ /O]   The current time.

    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT GetTime(OUT UI_ANIMATION_SECONDS* pSeconds);

    /*!
    * @brief Add time Listener.
    *
    * @param pListener      [I/ ]   The listener.
    * @param pAnimation     [I/ ]   The animation.

    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT AddTimerListener(IAnimationTimerListener *pListener, SdkAnimation *pAnimation);

    /*!
    * @brief Remove time Listener.
    *
    * @param pListener  [I/ ]   The listener.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT RemoveTimerListener(IAnimationTimerListener *pListener);

    /*!
    * @brief Create the story board.
    *
    * @param ppStoryBoard  [ /O]  The pointer to the story board.

    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT CreateStoryboard(OUT IUIAnimationStoryboard **ppStoryBoard);

    /*!
    * @brief Update the animation data.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT Update();

    /*!
    * @brief Update the animation data according to the specified time.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT Update(UI_ANIMATION_SECONDS timeNow);

    /*!
    * @brief Create the Variable.
    *
    * @param dInitValue  [I/ ]  The initial value.
    * @param ppVariable  [ /O]  The pointer to the Variable.

    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT CreateVariable(DOUBLE dInitValue,
                           OUT IUIAnimationVariable **ppVariable);

    /*!
    * @brief Create the Variable.
    *
    * @param dInitValue  [I/ ]  The initial value.
    * @param dLowerBound [I/ ]  The lower bound.
    * @param dUpperBound [I/ ]  The upper bound.
    * @param ppVariable  [ /O]  The pointer to the Variable.

    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT CreateVariable(
        DOUBLE dInitValue,
        DOUBLE dLowerBound,
        DOUBLE dUpperBound,
        OUT IUIAnimationVariable **ppVariable);

    /*!
    * @brief Create the Accelerate-Decelerate-Transition.
    *
    * @param duration           [I/ ]  The duration.
    * @param finalValue         [I/ ]  The final value.
    * @param accelerationRatio  [I/ ]  The acceleration ratio.
    * @param decelerationRatio  [I/ ]  The deceleration ratio.
    * @param ppTransition       [ /O]  The pointer to the transition.

    * @return S_OK if success, otherwise return E_FAIL.
    */
   HRESULT CreateAccelerateDecelerateTransition(
       UI_ANIMATION_SECONDS        duration,
       DOUBLE                      finalValue,
       DOUBLE                      accelerationRatio,
       DOUBLE                      decelerationRatio,
       OUT IUIAnimationTransition  **ppTransition);

private:

    IUIAnimationManager             *m_pAnimationManager;       // The animation manager.
    IUIAnimationTimer               *m_pAnimationTimer;         // The animation timer.
    IUIAnimationTransitionLibrary   *m_pAnimationTransitionLib; // The animation transition lib.
    SdkAnimationTimerEventHandler      *m_pTimerEventHandler;      // The animation timer event handler.
};

END_NAMESPACE_ANIMATION

#endif // _SDKANIMATIONCOMMON_H_
#endif // __cplusplus
