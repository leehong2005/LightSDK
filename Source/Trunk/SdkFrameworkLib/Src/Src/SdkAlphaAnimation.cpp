/*!
* @file SdkAlphaAnimation.cpp 
* 
* @brief This file defines the functions of alpha animation.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun, Cui ShuYan
* @date 2010/12/28
*/

#include "stdafx.h"
#include "SdkAlphaAnimation.h"
#include "SdkAnimationCom.h"

USING_NAMESPACE_ANIMATION

//////////////////////////////////////////////////////////////////////////

SdkAlphaAnimation::SdkAlphaAnimation(
    DOUBLE dFromAlpha,
    DOUBLE dToAlpha,
    DOUBLE dDuration,
    DOUBLE dAccelerationRatio,
    DOUBLE dDecelerationRatio)
{
    m_pTransition    = NULL;
    m_pAlphaVariable = NULL;
    m_nAnimationType = TRANSFORM_TYPE_ALPHA;
    dToAlpha = (dToAlpha < 0) ? 0 : ( (dToAlpha > 1.0) ? 1.0 : dToAlpha);

    m_pAlphaInfo = new TRANSITIONINFO();
    ZeroMemory(m_pAlphaInfo, sizeof(TRANSITIONINFO));

    m_pAlphaInfo->dFrom = dFromAlpha;
    m_pAlphaInfo->dTo = dToAlpha;
    m_pAlphaInfo->dDuration = dDuration;
    m_pAlphaInfo->dAccelerationRatio = dAccelerationRatio;
    m_pAlphaInfo->dDecelerationRatio = dDecelerationRatio;
}

//////////////////////////////////////////////////////////////////////////

SdkAlphaAnimation::~SdkAlphaAnimation(void)
{
    SAFE_DELETE(m_pAlphaInfo);

    SAFE_RELEASE(m_pAlphaVariable);
    SAFE_RELEASE(m_pTransition);
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAlphaAnimation::GetValue(OUT DOUBLE *pValue, BOOL bUpdate)
{
    HRESULT hr = GetVariableValue(pValue, m_pAlphaVariable, bUpdate);
    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAlphaAnimation::GetDuration(OUT DOUBLE *pDuration)
{
    return (NULL != m_pTransition) ? m_pTransition->GetDuration(pDuration) : E_FAIL;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAlphaAnimation::GetTransform(OUT LPTRANSFORMINFO pTransform, IN BOOL bUpdate)
{
    HRESULT hr = (NULL != pTransform) ? S_OK : S_FALSE;
    if ( SUCCEEDED(hr) )
    {
        if (bUpdate)
        {
            hr = this->Update();
        }

        if (SUCCEEDED(hr))
        {
            hr = (NULL != m_pAlphaVariable && NULL != m_pAlphaInfo) ? S_OK : E_FAIL;

            if ( SUCCEEDED(hr) )
            {
                DOUBLE dValue = 0.0; 
                hr = m_pAlphaVariable->GetValue(&dValue);

                if (SUCCEEDED(hr))
                {
                    pTransform->typeTransform = m_nAnimationType;
                    pTransform->matrixTransform = D2D1::Matrix3x2F::Identity();
                    pTransform->dAlpha = dValue;
                }
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAlphaAnimation::GetAnimationInfo(
    OUT IUIAnimationVariable ***pppVarible,
    OUT IUIAnimationTransition ***pppTransition,
    OUT UINT32 *pTransitionCount,
    BOOL bUpdate)
{
    HRESULT hr = E_FAIL;

    if ( (NULL != m_pAlphaInfo) &&
         (NULL != pppVarible) &&
         (NULL != pppTransition) &&
         (NULL != pTransitionCount) )
    {
        (*pTransitionCount) = 1;
        (*pppVarible) = new IUIAnimationVariable*[1];
        (*pppTransition) = new IUIAnimationTransition*[1];

        hr = CreateVariable(m_pAlphaInfo, &m_pAlphaVariable, bUpdate);
        if ( SUCCEEDED(hr) )
        {
            hr = CreateTransition(m_pAlphaInfo, &m_pTransition, bUpdate);

            if ( SUCCEEDED(hr) )
            {
                (*pppVarible)[0] = m_pAlphaVariable;
                (*pppTransition)[0] = m_pTransition;

                SAFE_ADDREF(m_pAlphaVariable);
                SAFE_ADDREF(m_pTransition);
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAlphaAnimation::InitStoryboard()
{
    HRESULT hr = CreateVariable(m_pAlphaInfo, &m_pAlphaVariable, TRUE);

    if (SUCCEEDED(hr))
    {
        hr = CreateTransition(m_pAlphaInfo, &m_pTransition, TRUE);
        if (SUCCEEDED(hr))
        {
            SAFE_RELEASE(m_pStoryBoard);
            hr = m_pAnimationCom->CreateStoryboard(&m_pStoryBoard);
            if (SUCCEEDED(hr))
            {
                hr = m_pStoryBoard->AddTransition(m_pAlphaVariable, m_pTransition);
            }
        }
    }

    return hr;
}
