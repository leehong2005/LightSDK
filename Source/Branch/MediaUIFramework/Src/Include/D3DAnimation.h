/*!
* @file D3DBasicAnimation.h 
* 
* @brief This file defines the base functions of D3DBasicAnimation.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _D3DANIMATION_H_
#define _D3DANIMATION_H_

#include "Common.h"
#include "Animation.h"

class CLASS_DECLSPEC D3DBasicAnimation : public Animation
{
public:

    /*!
    * @brief construction function.
    *
    */
    D3DBasicAnimation(DOUBLE fFormValue,
        DOUBLE fToValue,
        DOUBLE dDuration = 1.0,
        DOUBLE accelerationRatio = 0.5,
        DOUBLE decelerationRatio = 0.5);

    /*!
    * @brief destruction function.
    *
    */
    virtual ~D3DBasicAnimation();

    /*!
    * @brief Get the value of the variable.
    *
    * @param pValue             [I/O] The variable value.
    * @param bUpdate            [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT GetValue(OUT DOUBLE *pValue, BOOL bUpdate = TRUE);

   /*!
    * @brief Get the animation type.
    *
    * @return The animation type.
    */
    INT GetAnimationType();

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
    virtual HRESULT GetTransform(OUT LPTRANSFORMINFO pTransform,
                                 IN  BOOL bUpdate = TRUE)
    {
        UNREFERENCED_PARAMETER(pTransform);
        UNREFERENCED_PARAMETER(bUpdate);
        return E_NOTIMPL;
    };

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
    virtual HRESULT GetAnimationInfo(OUT  IUIAnimationVariable ***pppVarible,
                                     OUT  IUIAnimationTransition ***pppTransition,
                                     OUT  UINT *pTransitionCount,
                                     BOOL bUpdate = TRUE);

protected:

    /*!
    * @brief Initialize the storyboard of the alpha animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT InitStoryboard();

private:

    LPTRANSITIONINFO                m_pValueInfo;       // The information of the alpha animation.
    IUIAnimationVariable            *m_pVariable;        // The variable of the alpha animation.
    IUIAnimationTransition          *m_pTransition;     // The transition of the alpha animation.

};

#endif //_D3DANIMATION_H_
#endif //__cplusplus