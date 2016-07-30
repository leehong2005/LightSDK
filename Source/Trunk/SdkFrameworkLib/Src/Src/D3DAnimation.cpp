/*!
* @file D3DBasicAnimation.cpp
* 
* @brief This file defines the base functions of D3DBasicAnimation.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#include "stdafx.h"
#include "D3DAnimation.h"
#include "SdkAnimationCom.h"

USING_NAMESPACE_D3D

//////////////////////////////////////////////////////////////////////////

D3DBasicAnimation::D3DBasicAnimation(DOUBLE dFromValue,
                               DOUBLE dToValue,
                               DOUBLE dDuration,
                               DOUBLE dAccelerationRatio,
                               DOUBLE dDecelerationRatio)
{
    m_nAnimationType = TRANSFORM_TYPE_UNKNOWN;


    m_pValueInfo = new TRANSITIONINFO();
    m_pValueInfo->dFrom = dFromValue;
    m_pValueInfo->dTo = dToValue;
    m_pValueInfo->dDuration = dDuration;
    m_pValueInfo->dAccelerationRatio = dAccelerationRatio;
    m_pValueInfo->dDecelerationRatio = dDecelerationRatio;

    m_pVariable = NULL;
    m_pTransition = NULL;
}

//////////////////////////////////////////////////////////////////////////

D3DBasicAnimation::~D3DBasicAnimation(void)
{
    SAFE_DELETE(m_pValueInfo);
    SAFE_RELEASE(m_pVariable);
    SAFE_RELEASE(m_pTransition);
}

//////////////////////////////////////////////////////////////////////////

HRESULT D3DBasicAnimation::GetValue(OUT DOUBLE  *pValue, 
                                 BOOL        bUpdate)
{
    HRESULT hr = GetVariableValue(pValue, m_pVariable, bUpdate);

    return hr;
}

//////////////////////////////////////////////////////////////////////////

INT D3DBasicAnimation::GetAnimationType()
{
    return m_nAnimationType;
}

//////////////////////////////////////////////////////////////////////////

HRESULT D3DBasicAnimation::GetDuration(OUT DOUBLE *pDuration)
{
    HRESULT hr = E_FAIL;

    if(NULL != m_pTransition)
    {
        hr = m_pTransition->GetDuration(pDuration);
    }

    return hr;
}
//////////////////////////////////////////////////////////////////////////

HRESULT D3DBasicAnimation::GetAnimationInfo(OUT  IUIAnimationVariable ***pppVarible,
                                         OUT  IUIAnimationTransition ***pppTransition,
                                         OUT  UINT *pTransitionCount,
                                         BOOL bUpdate)
{
    HRESULT hr = E_FAIL;

    if((NULL != m_pValueInfo)
        && (NULL != pppVarible)
        && (NULL != pppTransition)
        && (NULL != pTransitionCount))
    {
        *pTransitionCount = 1;
        *pppVarible = new IUIAnimationVariable *[1];
        *pppTransition = new IUIAnimationTransition *[1];

        hr = CreateVariable(m_pValueInfo, &m_pVariable, bUpdate);

        if(SUCCEEDED(hr))
        {
            hr = CreateTransition(m_pValueInfo, &m_pTransition, bUpdate);

            if(SUCCEEDED(hr))
            {
                (*pppVarible)[0] = m_pVariable;
                m_pVariable->AddRef();

                (*pppTransition)[0] = m_pTransition;
                m_pTransition->AddRef();
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT D3DBasicAnimation::InitStoryboard()
{
    HRESULT hr = CreateVariable(m_pValueInfo, &m_pVariable, TRUE);

    if(SUCCEEDED(hr))
    {
        hr = CreateTransition(m_pValueInfo, &m_pTransition, TRUE);

        if(SUCCEEDED(hr))
        {
            SAFE_RELEASE(m_pStoryBoard);

            hr = m_pAnimationCom->CreateStoryboard(&m_pStoryBoard);

            if(SUCCEEDED(hr))
            {
                hr = m_pStoryBoard->AddTransition(m_pVariable, m_pTransition);
            }
        }
    }

    return hr;
}
