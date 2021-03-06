/*!
* @file SdkScaleAnimation.cpp 
* 
* @brief This file defines the functions of scale animation.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#include "stdafx.h"
#include "SdkScaleAnimation.h"
#include "SdkAnimationCom.h"

USING_NAMESPACE_ANIMATION

//////////////////////////////////////////////////////////////////////////

SdkScaleAnimation::SdkScaleAnimation(const D2D1_POINT_2F *pointCenter) :
    m_pScaleInfoX(NULL),
    m_pScaleInfoY(NULL),
    m_pVariableX(NULL),
    m_pTransitionX(NULL),
    m_pVariableY(NULL),
    m_pTransitionY(NULL)
{
    m_nAnimationType = TRANSFORM_TYPE_SCALE;

    if (NULL != pointCenter)
    {
        m_centerPoint = Point2F(pointCenter->x, pointCenter->y);
    }
    else
    {
        m_centerPoint = Point2F(0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////

SdkScaleAnimation::~SdkScaleAnimation()
{
    SAFE_DELETE(m_pScaleInfoX);
    SAFE_DELETE(m_pScaleInfoY);

    SAFE_RELEASE(m_pVariableX);
    SAFE_RELEASE(m_pTransitionX);
    SAFE_RELEASE(m_pVariableY);
    SAFE_RELEASE(m_pTransitionY);
}

//////////////////////////////////////////////////////////////////////////

void SdkScaleAnimation::SetScaleXInfo(
    DOUBLE dFromX, 
    DOUBLE dToX,
    DOUBLE dDuration,
    DOUBLE dAccelerationRatio,
    DOUBLE dDecelerationRatio)
{
    if (NULL == m_pScaleInfoX)
    {
        m_pScaleInfoX = new TRANSITIONINFO();
        ZeroMemory(m_pScaleInfoX, sizeof(TRANSITIONINFO));
    }

    m_pScaleInfoX->dFrom = dFromX;
    m_pScaleInfoX->dTo = dToX;
    m_pScaleInfoX->dDuration = dDuration;
    m_pScaleInfoX->dAccelerationRatio = dAccelerationRatio;
    m_pScaleInfoX->dDecelerationRatio = dDecelerationRatio;
}

//////////////////////////////////////////////////////////////////////////

void SdkScaleAnimation::SetScaleYInfo(
    DOUBLE dFromY, 
    DOUBLE dToY,
    DOUBLE dDuration,
    DOUBLE dAccelerationRatio,
    DOUBLE dDecelerationRatio)
{
    if (NULL == m_pScaleInfoY)
    {
        m_pScaleInfoY = new TRANSITIONINFO();
    }

    m_pScaleInfoY->dFrom = dFromY;
    m_pScaleInfoY->dTo = dToY;
    m_pScaleInfoY->dDuration = dDuration;
    m_pScaleInfoY->dAccelerationRatio = dAccelerationRatio;
    m_pScaleInfoY->dDecelerationRatio = dDecelerationRatio;
}

//////////////////////////////////////////////////////////////////////////

void SdkScaleAnimation::SetPivotPoint(D2D1_POINT_2F *pointCenter)
{
    if (NULL != pointCenter)
    {
        m_centerPoint = Point2F(pointCenter->x, pointCenter->y);
    }
    else
    {
        m_centerPoint = Point2F(0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkScaleAnimation::GetValue(OUT DOUBLE *pValueX, OUT DOUBLE *pValueY, BOOL bUpdate)
{
    HRESULT hr = (NULL != m_pVariableX || NULL != m_pVariableY) ? S_OK : E_FAIL;

    if ( SUCCEEDED(hr) )
    {
        if (NULL != m_pVariableX)
        {
            hr = GetVariableValue(pValueX, m_pVariableX, bUpdate);
        }
        else
        {
            if (NULL != pValueX)
            {
                *pValueX = 1.0;
            }
        }

        if (NULL != m_pVariableY)
        {
            hr = GetVariableValue(pValueY, m_pVariableY, bUpdate);
        }
        else
        {
            if (NULL != pValueY)
            {
                *pValueY = 1.0;
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkScaleAnimation::GetDuration(OUT DOUBLE *pDuration)
{
    HRESULT hr = (NULL != pDuration) ? S_OK : E_FAIL;

    if ( SUCCEEDED(hr) )
    {
        if ((NULL != m_pTransitionX) || (NULL != m_pTransitionY))
        {
            DOUBLE dDurationX = 0.0f;
            DOUBLE dDurationY = 0.0f;

            if (NULL != m_pTransitionX)
            {
                hr = m_pTransitionX->GetDuration(&dDurationX);
            }

            if (NULL != m_pTransitionY)
            {
                hr = m_pTransitionY->GetDuration(&dDurationY);
            }

            if (SUCCEEDED(hr))
            {
                *pDuration = (dDurationX > dDurationY) ? dDurationX : dDurationY;
            }
        }
    }
    
    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkScaleAnimation::GetTransform(OUT LPTRANSFORMINFO pTransform, IN BOOL bUpdateValue)
{
    if ( NULL == pTransform )
    {
        return E_INVALIDARG;
    }

    HRESULT hr = bUpdateValue ? this->Update() : S_OK;
    if ( SUCCEEDED(hr) )
    {
        hr = ((NULL != m_pScaleInfoX) || (NULL != m_pScaleInfoY)) ? S_OK : E_FAIL;
        if ( SUCCEEDED(hr) )
        {
            DOUBLE dValueX = 1.0;
            DOUBLE dValueY = 1.0;

            if (NULL != m_pScaleInfoX)
            {
                hr = (NULL != m_pVariableX) ? m_pVariableX->GetValue(&dValueX) : E_FAIL;
            }

            if (NULL != m_pScaleInfoY)
            {
                hr = (NULL != m_pVariableY) ? m_pVariableY->GetValue(&dValueY) : E_FAIL;
            }

            if (SUCCEEDED(hr))
            {
                pTransform->typeTransform = m_nAnimationType;
                pTransform->matrixTransform = D2D1::Matrix3x2F::Scale(
                    (FLOAT)dValueX,
                    (FLOAT)dValueY, m_centerPoint);
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkScaleAnimation::GetAnimationInfo(
    OUT IUIAnimationVariable ***pppVarible,
    OUT IUIAnimationTransition ***pppTransition,
    OUT UINT32 *pTransitionCount,
    BOOL bUpdate)
{
    HRESULT hr = E_FAIL;

    if ( (NULL != pppVarible) &&
         (NULL != pppTransition) &&
         (NULL != pTransitionCount) )
    {
        if (NULL != m_pScaleInfoX || NULL != m_pScaleInfoY)
        {
            UINT32 nVariableCount = 0;

            if (NULL != m_pScaleInfoX)
            {
                nVariableCount ++;
            }

            if (NULL != m_pScaleInfoY)
            {
                nVariableCount ++;
            }

            *pTransitionCount = nVariableCount;
            *pppVarible = new IUIAnimationVariable*[nVariableCount];
            *pppTransition = new IUIAnimationTransition*[nVariableCount];

            UINT32 nIndex = 0;

            if (NULL != m_pScaleInfoX)
            {
                hr = CreateVariable(m_pScaleInfoX, &m_pVariableX, bUpdate);
                if (SUCCEEDED(hr))
                {
                    hr = CreateTransition(m_pScaleInfoX, &m_pTransitionX, bUpdate);
                    if (SUCCEEDED(hr))
                    {
                        (*pppVarible)[nIndex] = m_pVariableX;
                        (*pppTransition)[nIndex] = m_pTransitionX;

                        SAFE_ADDREF(m_pVariableX);
                        SAFE_ADDREF(m_pTransitionX);
                        nIndex ++;
                    }
                }
            }

            if (NULL != m_pScaleInfoY)
            {
                hr = CreateVariable(m_pScaleInfoY, &m_pVariableY, bUpdate);
                if (SUCCEEDED(hr))
                {
                    hr = CreateTransition(m_pScaleInfoY, &m_pTransitionY, bUpdate);
                    if (SUCCEEDED(hr))
                    {
                        (*pppVarible)[nIndex] = m_pVariableY;
                        (*pppTransition)[nIndex] = m_pTransitionY;

                        SAFE_ADDREF(m_pVariableY);
                        SAFE_ADDREF(m_pTransitionY);
                    }
                }
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkScaleAnimation::InitStoryboard()
{
    HRESULT hr = E_FAIL;

    if ((NULL != m_pScaleInfoX) || (NULL != m_pScaleInfoY))
    {
        SAFE_RELEASE(m_pStoryBoard);
        hr = m_pAnimationCom->CreateStoryboard(&m_pStoryBoard);

        if (SUCCEEDED(hr))
        {
            if (NULL != m_pScaleInfoX)
            {
                hr = CreateVariable(m_pScaleInfoX, &m_pVariableX, TRUE);
                if (SUCCEEDED(hr))
                {
                    hr = CreateTransition(m_pScaleInfoX, &m_pTransitionX, TRUE);
                    if (SUCCEEDED(hr))
                    {
                        hr = m_pStoryBoard->AddTransition(m_pVariableX, m_pTransitionX);
                    }
                }
            }

            if (NULL != m_pScaleInfoY)
            {
                hr = CreateVariable(m_pScaleInfoY, &m_pVariableY, TRUE);
                if (SUCCEEDED(hr))
                {
                    hr = CreateTransition(m_pScaleInfoY, &m_pTransitionY, TRUE);
                    if (SUCCEEDED(hr))
                    {
                        hr = m_pStoryBoard->AddTransition(m_pVariableY, m_pTransitionY);
                    }
                }
            }
        }
    }

    return hr;
}
