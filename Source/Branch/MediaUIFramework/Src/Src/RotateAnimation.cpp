/*!
* @file RotateAnimation.cpp 
* 
* @brief This file defines the functions of rotate animation.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun, Cui ShuYan
* @date 2010/12/28
*/

#include "RotateAnimation.h"

#include "D2D1.h"
using namespace D2D1;

//////////////////////////////////////////////////////////////////////////

RotateAnimation::RotateAnimation(const D2D1_POINT_2F *pointCenter,
                                 DOUBLE dFromAngle,
                                 DOUBLE dToAngle,
                                 DOUBLE dDuration,
                                 DOUBLE dAccelerationRatio,
                                 DOUBLE dDecelerationRatio):
                 m_pAngleVariable(NULL),
                 m_pTransition(NULL)
{
    m_nAnimationType = TRANSFORM_TYPE_ROTATE;

    m_pRotateInfo = new _TRANSITIONINFO();
    m_pRotateInfo->dFrom = dFromAngle;
    m_pRotateInfo->dTo = dToAngle;
    m_pRotateInfo->dDuration = dDuration;
    m_pRotateInfo->dAccelerationRatio = dAccelerationRatio;
    m_pRotateInfo->dDecelerationRatio = dDecelerationRatio;

    if(NULL != pointCenter)
    {
        m_centerPoint = D2D1::Point2F(pointCenter->x, pointCenter->y);
    }
    else
    {
        m_centerPoint = D2D1::Point2F(0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////

RotateAnimation::~RotateAnimation(void)
{
    SAFE_DELETE(m_pRotateInfo);
    SAFE_RELEASE(m_pAngleVariable);
    SAFE_RELEASE(m_pTransition);
}

//////////////////////////////////////////////////////////////////////////

HRESULT RotateAnimation::GetValue(OUT DOUBLE  *pValue, 
                                  BOOL bUpdate /*= TRUE*/)
{
    if(NULL != pValue)
    {
        *pValue = 0.0;
    }

    return GetVariableValue(pValue, m_pAngleVariable, bUpdate);
}

//////////////////////////////////////////////////////////////////////////

HRESULT RotateAnimation::InitStoryboard()
{
    HRESULT hr = CreateVariable(m_pRotateInfo, &m_pAngleVariable, TRUE);

    if(SUCCEEDED(hr))
    {
        hr = CreateTransition(m_pRotateInfo, &m_pTransition, TRUE);

        if(SUCCEEDED(hr))
        {
            SAFE_RELEASE(m_pStoryBoard);
            hr = m_pAnimationCom->CreateStoryboard(&m_pStoryBoard);

            if(SUCCEEDED(hr))
            {
                hr = m_pStoryBoard->AddTransition(m_pAngleVariable, m_pTransition);
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT RotateAnimation::GetDuration(OUT DOUBLE *pDuration)
{
    HRESULT hr = E_FAIL;

    if(NULL != m_pTransition)
    {
        hr = m_pTransition->GetDuration(pDuration);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT RotateAnimation::GetTransform(OUT LPTRANSFORMINFO pTransform,
                                      IN  BOOL bUpdate) 
{
    HRESULT hr = S_OK;

    if(NULL != pTransform)
    {
        if(bUpdate)
        {
            hr = this->Update();
        }

        if(SUCCEEDED(hr))
        {
            if(NULL != m_pAngleVariable && NULL != m_pRotateInfo)
            {
                DOUBLE dValue = 0.0; 
                hr = m_pAngleVariable->GetValue(&dValue);

                if(SUCCEEDED(hr))
                {
                    pTransform->typeTransform = m_nAnimationType;
                    pTransform->matrixTransform = D2D1::Matrix3x2F::Rotation((FLOAT)dValue,
                        m_centerPoint);
                }
            }
            else
            {
                hr = E_FAIL;
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT RotateAnimation::GetAnimationInfo(OUT  IUIAnimationVariable ***pppVarible,
                                          OUT  IUIAnimationTransition ***pppTransition,
                                          OUT  UINT32 *pTransitionCount,
                                          BOOL bUpdate)
{
    HRESULT hr = E_FAIL;

    if((NULL != m_pRotateInfo)
        && (NULL != pppVarible)
        && (NULL != pppTransition)
        && (NULL != pTransitionCount))
    {
        *pTransitionCount = 1;
        *pppVarible = new IUIAnimationVariable *[1];
        *pppTransition = new IUIAnimationTransition *[1];

        hr = CreateVariable(m_pRotateInfo, &m_pAngleVariable, bUpdate);

        if(SUCCEEDED(hr))
        {
            hr = CreateTransition(m_pRotateInfo, &m_pTransition, bUpdate);

            if(SUCCEEDED(hr))
            {
                (*pppVarible)[0] = m_pAngleVariable;
                m_pAngleVariable->AddRef();

                (*pppTransition)[0] = m_pTransition;
                m_pTransition->AddRef();
            }
        }
    }

    return hr;
}
