/*!
* @file Animation.h 
* 
* @brief This file defines the base functions of animation.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "CommonInclude.h"
#include "AnimationDef.h"
#include "AnimationCommon.h"
#include "IAnimationListener.h"
#include "IAnimationTimerListener.h"

class StoryboardEventHandler;

/*!
* @brief This class defines the base functions for animation.
*/
class CLASS_DECLSPEC Animation
{
public:

    /*!
    * @brief The constructor.
    */
    Animation();

    /*
    * @brief The default destructor.
    */
    virtual ~Animation();

    /*!
    * @brief Get the animation type.
    *
    * @return The animation type.
    */
    INT32 GetAnimationType();

    /*!
    * @brief Start the animation now.
    *
    * @param dDelaySeconds  [I/ ] The delayed time.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT Start(DOUBLE dDelaySeconds = 0);

    /*!
    * @brief Stop the animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT Stop();

    /*!
    * @brief Finish the animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT Finish();

    /*!
    * @brief Gets the time that has elapsed since the storyboard started playing.
    *
    * @param dElapsedTime   [ /O] The time that has elapsed since the storyboard started playing. 
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT GetElapsedTime(OUT DOUBLE &dElapsedTime);

    /*!
    * @brief Set the animation listener.
    *
    * @param pListener       [I/ ]   The animation listener.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT AddAnimationListener(IAnimationListener *pListener);

    /*!
    * @brief Remove the animation listener.
    *
    * @param pListener       [I/ ]   The animation listener.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT RemoveAnimationListener(IAnimationListener *pListener);

    /*!
    * @brief Set the animation timer listener.
    *
    * @param pListener       [I/ ]   The animation timer listener.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT AddAnimationTimerListener(IAnimationTimerListener *pListener);

    /*!
    * @brief Remove the animation timer listener.
    *
    * @param pListener       [I/ ]   The animation timer listener.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT RemoveAnimationTimerListener(IAnimationTimerListener *pListener);

    /*!
    * @brief Get the animation state which indicates the animation finished or not.
    *
    * @param bFinish        [ /O] The finish state.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT IsFinish(OUT BOOL *bFinish);

    /*!
    * @brief Get the animation state which indicates the animation start or not.
    *
    * @return The start flag.
    */
    BOOL IsStart();

    /*!
    * @brief Get the duration of animation.
    *
    * @param pDuration      [ /O] The duration.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetDuration(OUT DOUBLE *pDuration) = 0;

    /*!
    * @brief Get the transform of animation.
    *
    * @param pTransform     [ /O] The transform information.
    * @param bUpdateFrame   [I/ ] The flag whether update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetTransform(OUT LPTRANSFORMINFO pTransform, IN BOOL bUpdateValue = TRUE) = 0;

    /*!
    * @brief Get the animation info.
    *
    * @param pppVarible         [ /O] The variable information.
    * @param pppTransition      [ /O] The transition information.
    * @param pTransitionCount   [ /O] The transition count.
    * @param bUpdate            [I/ ] The flag whether update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetAnimationInfo(OUT  IUIAnimationVariable ***pppVarible,
                                     OUT  IUIAnimationTransition ***pppTransition,
                                     OUT  UINT32 *pTransitionCount,
                                     BOOL bUpdate = TRUE) = 0;

protected:

    /*!
    * @brief Start the animation at specified time.
    *
    * @param time           [I/ ] The specified time.
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT StartAnimation(IN UI_ANIMATION_SECONDS time);

    /*!
    * @brief Initialize the storyboard of animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT InitStoryboard() = 0;

    /*!
    * @brief Update the values of animation variables.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT Update();

    /*!
    * @brief Set the storyboard event listener.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT SetStoryBoardListener(IAnimationListener *pListener);

    /*!
    * @brief Create the animation variable.
    *
    * @param  LPTRANSITIONINFO       [I/ ] The transition information.
    * @param  IUIAnimationVariable   [ /O] The animation variable.
    * @param  bUpdate                [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT CreateVariable(IN LPTRANSITIONINFO      pTransitionInfo,
                           IN IUIAnimationVariable  **pTransitionVarible,
                           IN BOOL                  bUpdate = TRUE);

    /*!
    * @brief Create the animation transition.
    *
    * @param  LPTRANSITIONINFO         [I/ ] The transition information.
    * @param  IUIAnimationTransition   [ /O] The animation transition.
    * @param  bUpdate                  [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT CreateTransition(IN LPTRANSITIONINFO          pTransitionInfo,
                             IN IUIAnimationTransition    **pTransition,
                             IN BOOL                      bUpdate = TRUE);

     /*!
    * @brief Get the value of the variable.
    *
    * @param pVariableValue         [ /O] The variable value.
    * @param pTransitionVarible     [I/ ] The variable.
    * @param bUpdate                [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT GetVariableValue(OUT DOUBLE             *pVariableValue,
                             IUIAnimationVariable   *pTransitionVarible,
                             BOOL                   bUpdate = TRUE);

protected:

    BOOL                                m_bStart;                   // The flag indicated whether animation start or not.
    int                                 m_nAnimationType;           // The animation type.
    IUIAnimationStoryboard              *m_pStoryBoard;             // The storyboard to play animation.
    StoryboardEventHandler              *m_pStoryboardEventHandler; // The storyboard event handler.
    AnimationCommon                     *m_pAnimationCom;           // The animation com.
};

#endif // _ANIMATION_H_
#endif // __cplusplus
