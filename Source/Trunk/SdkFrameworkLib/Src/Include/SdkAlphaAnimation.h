/*! 
* @file SdkAlphaAnimation.h 
* 
* @brief This file defines the functions of alpha animation.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun, Cui ShuYan
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _SDKALPHAANIMATION_H_
#define _SDKALPHAANIMATION_H_

#include "SdkAnimation.h"

BEGIN_NAMESPACE_ANIMATION

/*!
* @brief This class implements the function of the alpha animation.
*/
class CLASS_DECLSPEC SdkAlphaAnimation : public SdkAnimation
{
public:

    /*!
    * @brief The constructor.
    */
    SdkAlphaAnimation(
        DOUBLE fFormAlpha,
        DOUBLE fToAlpha,
        DOUBLE dDuration = 1.0,
        DOUBLE accelerationRatio = 0.5,
        DOUBLE decelerationRatio = 0.5);

    /*!
    * @brief The default destructor.
    */
    virtual ~SdkAlphaAnimation();

    /*!
    * @brief Get the value of the variable.
    *
    * @param pValue       [ /O] The variable value.
    * @param bUpdate      [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetValue(OUT DOUBLE *pValue, BOOL bUpdate = TRUE);

    /*!
    * @brief Get the duration of the alpha animation.
    *
    * @param pDuration      [ /O] The duration.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetDuration(OUT DOUBLE *pDuration);

    /*!
    * @brief Get the transform of the alpha animation.
    *
    * @param pTransform     [ /O] The transform information.
    * @param bUpdate        [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetTransform(OUT LPTRANSFORMINFO pTransform, IN  BOOL bUpdate = TRUE);

    /*!
    * @brief Get the information of the alpha animation.
    *
    * @param pppVarible             [ /O] The variable information of the alpha animation.
    * @param pppTransition          [ /O] The transition information of the alpha animation.
    * @param pTransitionCount       [ /O] The count of the variable.
    * @param bUpdate                [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetAnimationInfo(
        OUT IUIAnimationVariable ***pppVarible,
        OUT IUIAnimationTransition ***pppTransition,
        OUT UINT32 *pTransitionCount,
        BOOL bUpdate = TRUE);

protected:

    /*!
    * @brief Initialize the storyboard of the alpha animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT InitStoryboard();

private:

    LPTRANSITIONINFO                 m_pAlphaInfo;           // The information of the alpha animation.
    IUIAnimationVariable            *m_pAlphaVariable;       // The variable of the alpha animation.
    IUIAnimationTransition          *m_pTransition;          // The transition of the alpha animation.
};

END_NAMESPACE_ANIMATION

#endif // _SDKALPHAANIMATION_H_
#endif // __cplusplus
