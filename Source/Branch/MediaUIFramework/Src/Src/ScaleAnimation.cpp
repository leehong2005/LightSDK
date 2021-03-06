/*!
* @file ScaleAnimation.cpp 
* 
* @brief This file defines the functions of scale animation.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#include "ScaleAnimation.h"

//////////////////////////////////////////////////////////////////////////

ScaleAnimation::ScaleAnimation(const D2D1_POINT_2F *pointCenter):
                m_pScaleInfoX(NULL),
                m_pScaleInfoY(NULL),
                m_pVariableX(NULL),
                m_pTransitionX(NULL),
                m_pVariableY(NULL),
                m_pTransitionY(NULL)
{
    m_nAnimationType = TRANSFORM_TYPE_SCALE;

    if(NULL != pointCenter)
    {
        m_centerPoint = Point2F(pointCenter->x, pointCenter->y);
    }
    else
    {
        m_centerPoint = Point2F(0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////

ScaleAnimation::~ScaleAnimation()
{
    SAFE_DELETE(m_pScaleInfoX);
    SAFE_DELETE(m_pScaleInfoY);

    SAFE_RELEASE(m_pVariableX);
    SAFE_RELEASE(m_pTransitionX);
    SAFE_RELEASE(m_pVariableY);
    SAFE_RELEASE(m_pTransitionY);
}

//////////////////////////////////////////////////////////////////////////

VOID ScaleAnimation::SetScaleXInfo(DOUBLE dFromX, 
                                   DOUBLE dToX,
                                   DOUBLE dDuration,
                                   DOUBLE dAccelerationRatio,
                                   DOUBLE dDecelerationRatio)
{
    if(NULL == m_pScaleInfoX)
    {
        m_pScaleInfoX = new TRANSITIONINFO();
    }

    m_pScaleInfoX->dFrom = dFromX;
    m_pScaleInfoX->dTo = dToX;
    m_pScaleInfoX->dDuration = dDuration;
    m_pScaleInfoX->dAccelerationRatio = dAccelerationRatio;
    m_pScaleInfoX->dDecelerationRatio = dDecelerationRatio;
}

//////////////////////////////////////////////////////////////////////////

VOID ScaleAnimation::SetScaleYInfo(DOUBLE dFromY, 
                                   DOUBLE dToY,
                                   DOUBLE dDuration,
                                   DOUBLE dAccelerationRatio,
                                   DOUBLE dDecelerationRatio )
{
    if(NULL == m_pScaleInfoY)
    {
        m_pScaleInfoY = new TRANSITIONINFO();
    }

    m_pScaleInfoY->dFrom = dFromY;
    m_pScaleInfoY->dTo = dToY;
    m_pScaleInfoY->dDuration = dDuration;
    m_pScaleInfoY->dAccelerationRatio = dAccelerationRatio ;
    m_pScaleInfoY->dDecelerationRatio = dDecelerationRatio ;
}

//////////////////////////////////////////////////////////////////////////

VOID ScaleAnimation::SetPivotPoint(D2D1_POINT_2F *pointCenter)
{
    if(NULL != pointCenter)
    {
        m_centerPoint = Point2F(pointCenter->x, pointCenter->y);
    }
    else
    {
        m_centerPoint = Point2F(0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////

HRESULT ScaleAnimation::GetValue(OUT DOUBLE  *pValueX,
                                 OUT DOUBLE  *pValueY,
                                 BOOL        bUpdate)
{
    HRESULT hr = S_OK;

    if(NULL != m_pVariableX || NULL != m_pVariableY)
    {
        if(NULL != m_pVariableX)
        {
            hr = GetVariableValue(pValueX, m_pVariableX, bUpdate);
        }
        else
        {
            if(NULL != pValueX)
            {
                *pValueX = 1.0;
            }
        }

        if(SUCCEEDED(hr))
        {
            if(NULL != m_pVariableY)
            {
                hr = GetVariableValue(pValueY, m_pVariableY, bUpdate);
            }
            else
            {
                if(NULL != pValueY)
                {
                    *pValueY = 1.0;
                }
            }
        }
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT ScaleAnimation::GetDuration(OUT DOUBLE *pDuration)
{
    HRESULT hr = E_FAIL;

    if(NULL != pDuration)
    {
        DOUBLE dDurationX = 0.0f;

        if((NULL != m_pTransitionX) || (NULL != m_pTransitionY))
        {
            if(NULL != m_pTransitionX)
            {
                hr = m_pTransitionX->GetDuration(&dDurationX);
            }

            DOUBLE dDurationY = 0.0f;

            if(NULL != m_pTransitionY)
            {
                hr = m_pTransitionY->GetDuration(&dDurationY);
            }

            if(SUCCEEDED(hr))
            {
                *pDuration = (dDurationX > dDurationY) ? dDurationX : dDurationY;
            }
        }
    }
    
    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT ScaleAnimation::GetTransform(OUT LPTRANSFORMINFO pTransform,
                                     IN  BOOL bUpdateValue)
{
    HRESULT hr = S_OK;

    if(NULL != pTransform)
    {
        if(bUpdateValue)
        {
            hr = this->Update();
        }

        if(SUCCEEDED(hr))
        {
            if((NULL != m_pScaleInfoX) || (NULL != m_pScaleInfoY))
            {
                DOUBLE dValueX = 1.0;

                if(NULL != m_pScaleInfoX)
                {
                    if(NULL == m_pVariableX)
                    {
                        hr = E_FAIL;
                    }
                    else
                    {
                        hr = m_pVariableX->GetValue(&dValueX);
                    }
                }

                DOUBLE dValueY = 1.0;

                if(SUCCEEDED(hr))
                {
                    if(NULL != m_pScaleInfoY)
                    {
                        if(NULL == m_pVariableY)
                        {
                            hr = E_FAIL;
                        }
                        else
                        {
                            hr = m_pVariableY->GetValue(&dValueY);
                        }
                    }

                    if(SUCCEEDED(hr))
                    {
                        pTransform->typeTransform = m_nAnimationType;
                        pTransform->matrixTransform = D2D1::Matrix3x2F::Scale((FLOAT)dValueX, (FLOAT)dValueY,
                            m_centerPoint);
                    }
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

HRESULT ScaleAnimation::GetAnimationInfo(OUT  IUIAnimationVariable   ***pppVarible,
                                         OUT  IUIAnimationTransition ***pppTransition,
                                         OUT  UINT32 *pTransitionCount,
                                         BOOL bUpdate)
{
    HRESULT hr = E_FAIL;

    if((NULL != pppVarible)
        && (NULL != pppTransition)
        && (NULL != pTransitionCount))
    {
        if(NULL != m_pScaleInfoX || NULL != m_pScaleInfoY)
        {
            UINT32 nVariableCount = 0;

            if(NULL != m_pScaleInfoX)
            {
                nVariableCount ++;
            }

            if(NULL != m_pScaleInfoY)
            {
                nVariableCount ++;
            }

            *pTransitionCount = nVariableCount;
            *pppVarible = new IUIAnimationVariable *[nVariableCount];
            *pppTransition = new IUIAnimationTransition *[nVariableCount];

            UINT32 nIndex = 0;

            if(NULL != m_pScaleInfoX)
            {
                hr = CreateVariable(m_pScaleInfoX, &m_pVariableX, bUpdate);
                if(SUCCEEDED(hr))
                {
                    hr = CreateTransition(m_pScaleInfoX, &m_pTransitionX, bUpdate);
                    if(SUCCEEDED(hr))
                    {
                        (*pppVarible)[nIndex] = m_pVariableX;
                        m_pVariableX->AddRef();

                        (*pppTransition)[nIndex] = m_pTransitionX;
                        m_pTransitionX->AddRef();

                        nIndex ++;
                    }
                }
            }

            if(NULL != m_pScaleInfoY)
            {
                hr = CreateVariable(m_pScaleInfoY, &m_pVariableY, bUpdate);
                if(SUCCEEDED(hr))
                {
                    hr = CreateTransition(m_pScaleInfoY, &m_pTransitionY, bUpdate);
                    if(SUCCEEDED(hr))
                    {
                        (*pppVarible)[nIndex] = m_pVariableY;
                        m_pVariableY->AddRef();

                        (*pppTransition)[nIndex] = m_pTransitionY;
                        m_pTransitionY->AddRef();
                    }
                }
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT ScaleAnimation::InitStoryboard()
{
    HRESULT hr = E_FAIL;

    if((NULL != m_pScaleInfoX) || (NULL != m_pScaleInfoY))
    {
        SAFE_RELEASE(m_pStoryBoard);
        hr = m_pAnimationCom->CreateStoryboard(&m_pStoryBoard);

        if(SUCCEEDED(hr))
        {
            if(NULL != m_pScaleInfoX)
            {
                hr = CreateVariable(m_pScaleInfoX, &m_pVariableX, TRUE);

                if(SUCCEEDED(hr))
                {
                    hr = CreateTransition(m_pScaleInfoX, &m_pTransitionX, TRUE);

                    if(SUCCEEDED(hr))
                    {
                        hr = m_pStoryBoard->AddTransition(m_pVariableX, m_pTransitionX);
                    }
                }
            }

            if(SUCCEEDED(hr))
            {
                if(NULL != m_pScaleInfoY)
                {
                    hr = CreateVariable(m_pScaleInfoY, &m_pVariableY, TRUE);

                    if(SUCCEEDED(hr))
                    {
                        hr = CreateTransition(m_pScaleInfoY, &m_pTransitionY, TRUE);

                        if(SUCCEEDED(hr))
                        {
                            hr = m_pStoryBoard->AddTransition(m_pVariableY, m_pTransitionY);
                        }
                    }
                }
            }
        }
    }

    return hr;
}
