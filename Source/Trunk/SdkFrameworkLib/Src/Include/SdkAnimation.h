/*!
* @file SdkAnimation.h 
* 
* @brief This file defines the base functions of animation.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "SdkCommonInclude.h"
#include "SdkUICommon.h"
#include "SdkAnimationDef.h"

BEGIN_NAMESPACE_ANIMATION

/*!
* @brief This class defines the base functions for animation.
*/
class CLASS_DECLSPEC SdkAnimation
{
public:

    /*!
    * @brief The constructor.
    */
    SdkAnimation();

    /*
    * @brief The default destructor.
    */
    virtual ~SdkAnimation();

    /*!
    * @brief Get the animation type.
    *
    * @return The animation type.
    */
    virtual INT32 GetAnimationType();

    /*!
    * @brief Start the animation now.
    *
    * @param dDelaySeconds  [I/ ] The delayed time.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT Start(DOUBLE dDelaySeconds = 0);

    /*!
    * @brief Stop the animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT Stop();

    /*!
    * @brief Finish the animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT Finish();

    /*!
    * @brief Gets the time that has elapsed since the storyboard started playing.
    *
    * @param dElapsedTime   [ /O] The time that has elapsed since the storyboard started playing. 
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetElapsedTime(OUT DOUBLE &dElapsedTime);

    /*!
    * @brief Get the animation state which indicates the animation finished or not.
    *
    * @param bFinish        [ /O] The finish state.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT IsFinish(OUT BOOL *bFinish);

    /*!
    * @brief Get the animation state which indicates the animation start or not.
    *
    * @return The start flag.
    */
    virtual BOOL IsStart();

    /*!
    * @brief Get the duration of animation.
    *
    * @param pDuration      [ /O] The duration.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetDuration(OUT DOUBLE *pDuration);

    /*!
    * @brief Get the transform of animation.
    *
    * @param pTransform     [ /O] The transform information.
    * @param bUpdateFrame   [I/ ] The flag whether update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetTransform(OUT LPTRANSFORMINFO pTransform, IN BOOL bUpdateValue = TRUE);

    /*!
    * @brief Set the animation listener.
    *
    * @param pListener       [I/ ]   The animation listener.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT AddAnimationListener(IAnimationListener *pListener);

    /*!
    * @brief Remove the animation listener.
    *
    * @param pListener       [I/ ]   The animation listener.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT RemoveAnimationListener(IAnimationListener *pListener);

    /*!
    * @brief Set the animation timer listener.
    *
    * @param pListener       [I/ ]   The animation timer listener.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT AddAnimationTimerListener(IAnimationTimerListener *pListener);

    /*!
    * @brief Remove the animation timer listener.
    *
    * @param pListener       [I/ ]   The animation timer listener.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT RemoveAnimationTimerListener(IAnimationTimerListener *pListener);

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
    virtual HRESULT GetAnimationInfo(OUT IUIAnimationVariable ***pppVarible,
                                     OUT IUIAnimationTransition ***pppTransition,
                                     OUT UINT32 *pTransitionCount,
                                     BOOL bUpdate = TRUE);

protected:

    /*!
    * @brief Initialize the storyboard of animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT InitStoryboard();

    /*!
    * @brief Start the animation at specified time.
    *
    * @param time           [I/ ] The specified time.
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT StartAnimation(IN UI_ANIMATION_SECONDS time);

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
    HRESULT CreateVariable(IN LPTRANSITIONINFO pTransitionInfo,
                           IN IUIAnimationVariable  **pTransitionVarible,
                           IN BOOL bUpdate = TRUE);

    /*!
    * @brief Create the animation transition.
    *
    * @param  LPTRANSITIONINFO         [I/ ] The transition information.
    * @param  IUIAnimationTransition   [ /O] The animation transition.
    * @param  bUpdate                  [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT CreateTransition(IN LPTRANSITIONINFO pTransitionInfo,
                             IN IUIAnimationTransition **pTransition,
                             IN BOOL bUpdate = TRUE);

     /*!
    * @brief Get the value of the variable.
    *
    * @param pVariableValue         [ /O] The variable value.
    * @param pTransitionVarible     [I/ ] The variable.
    * @param bUpdate                [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT GetVariableValue(OUT DOUBLE *pVariableValue,
                             IUIAnimationVariable *pTransitionVarible,
                             BOOL bUpdate = TRUE);

protected:

    BOOL                             m_bStart;                   // The flag indicated whether animation start or not.
    INT32                            m_nAnimationType;           // The animation type.
    IUIAnimationStoryboard          *m_pStoryBoard;              // The storyboard to play animation.
    SdkStoryboardEventHandler       *m_pStoryboardEventHandler;  // The storyboard event handler.
    SdkAnimationCom                 *m_pAnimationCom;            // The animation com.
};

END_NAMESPACE_ANIMATION

#endif // _ANIMATION_H_
#endif // __cplusplus
