/*!
* @file TranslateAnimation.cpp 
* 
* @brief This file defines the functions of translate animation.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#include "TranslateAnimation.h"

//////////////////////////////////////////////////////////////////////////

TranslateAnimation::TranslateAnimation():
                    m_pTranslateInfoX(NULL),
                    m_pTranslateInfoY(NULL),
                    m_pVariableX(NULL),
                    m_pTransitionX(NULL),
                    m_pVariableY(NULL),
                    m_pTransitionY(NULL)
{
    m_nAnimationType = TRANSFORM_TYPE_TRANSLATE;
}

//////////////////////////////////////////////////////////////////////////

TranslateAnimation::~TranslateAnimation()
{
    SAFE_DELETE(m_pTranslateInfoX);
    SAFE_DELETE(m_pTranslateInfoY);

    SAFE_RELEASE(m_pVariableX);
    SAFE_RELEASE(m_pTransitionX);
    SAFE_RELEASE(m_pVariableY);
    SAFE_RELEASE(m_pTransitionY);
}

//////////////////////////////////////////////////////////////////////////

VOID TranslateAnimation::SetTranslateXInfo(DOUBLE dFromX,
                                           DOUBLE dToX,
                                           DOUBLE dDuration ,
                                           DOUBLE dAccelerationRatio,
                                           DOUBLE dDecelerationRatio)
{
    if(NULL == m_pTranslateInfoX)
    {
        m_pTranslateInfoX = new TRANSITIONINFO();
    }
    m_pTranslateInfoX->dFrom = dFromX;
    m_pTranslateInfoX->dTo = dToX;
    m_pTranslateInfoX->dDuration = dDuration;
    m_pTranslateInfoX->dAccelerationRatio = dAccelerationRatio;
    m_pTranslateInfoX->dDecelerationRatio = dDecelerationRatio;
}

//////////////////////////////////////////////////////////////////////////

VOID TranslateAnimation::SetTranslateYInfo(DOUBLE dFromY,
                                           DOUBLE dToY,
                                           DOUBLE dDuration,
                                           DOUBLE dAccelerationRatio,
                                           DOUBLE dDecelerationRatio)
{
    if(NULL == m_pTranslateInfoY)
    {
        m_pTranslateInfoY = new TRANSITIONINFO();
    }

    m_pTranslateInfoY->dFrom = dFromY;
    m_pTranslateInfoY->dTo = dToY;
    m_pTranslateInfoY->dDuration = dDuration;
    m_pTranslateInfoY->dAccelerationRatio = dAccelerationRatio;
    m_pTranslateInfoY->dDecelerationRatio = dDecelerationRatio;
}

//////////////////////////////////////////////////////////////////////////

HRESULT TranslateAnimation::GetValue(OUT DOUBLE  *pValueX,
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
                *pValueX = 0.0;
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
                    *pValueY = 0.0;
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

HRESULT TranslateAnimation::GetDuration( OUT DOUBLE *pDuration )
{
    HRESULT hr = E_FAIL;

    if(NULL != pDuration)
    {
        DOUBLE dDurationX = 0.0;

        if((NULL != m_pTransitionX) || (NULL != m_pTransitionY))
        {
            if(NULL != m_pTransitionX)
            {
                hr = m_pTransitionX->GetDuration(&dDurationX);
            }

            DOUBLE dDurationY = 0.0;
            if(NULL != m_pTransitionY)
            {
                hr = m_pTransitionY->GetDuration(&dDurationY);
            }

            if(SUCCEEDED(hr))
            {
                *pDuration = dDurationX > dDurationY ? dDurationX : dDurationY;
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT TranslateAnimation::GetTransform(OUT LPTRANSFORMINFO pTransform, 
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
            if(NULL != m_pTranslateInfoX || NULL != m_pTranslateInfoY)
            {
                DOUBLE dValueX = 0.0, dValueY = 0.0;

                if(NULL != m_pTranslateInfoX)
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

                if(SUCCEEDED(hr))
                {
                    if(NULL != m_pTranslateInfoY)
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
                        pTransform->matrixTransform = D2D1::Matrix3x2F::Translation((FLOAT)dValueX, (FLOAT)dValueY);
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

HRESULT TranslateAnimation::InitStoryboard()
{
    HRESULT hr = E_FAIL;

    if(NULL != m_pTranslateInfoX || NULL != m_pTranslateInfoY)
    {

        SAFE_RELEASE(m_pStoryBoard);
        hr = m_pAnimationCom->CreateStoryboard(&m_pStoryBoard);

        if(SUCCEEDED(hr))
        {
            if(NULL != m_pTranslateInfoX)
            {
                hr = CreateVariable(m_pTranslateInfoX, &m_pVariableX, TRUE);

                if(SUCCEEDED(hr))
                {
                    hr = CreateTransition(m_pTranslateInfoX, &m_pTransitionX, TRUE);

                    if(SUCCEEDED(hr))
                    {
                        hr = m_pStoryBoard->AddTransition(m_pVariableX, m_pTransitionX);
                    }
                }
            }

            if(SUCCEEDED(hr))
            {
                if(NULL != m_pTranslateInfoY)
                {
                    hr = CreateVariable(m_pTranslateInfoY, &m_pVariableY, TRUE);

                    if(SUCCEEDED(hr))
                    {
                         hr = CreateTransition(m_pTranslateInfoY, &m_pTransitionY, TRUE);

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

//////////////////////////////////////////////////////////////////////////

HRESULT TranslateAnimation::GetAnimationInfo(OUT  IUIAnimationVariable ***pppVarible,
                                             OUT  IUIAnimationTransition ***pppTransition,
                                             OUT  UINT32 *pTransitionCount,
                                             BOOL bUpdate)
{
    HRESULT hr = E_FAIL;

    if((NULL != pppVarible)
        && (NULL != pppTransition)
        && (NULL != pTransitionCount))
    {
        if((NULL != m_pTranslateInfoX) || (NULL != m_pTranslateInfoY))
        {
            UINT32 nCount  = 0;

            if(NULL != m_pTranslateInfoX)
            {
                ++nCount;
            }

            if(NULL != m_pTranslateInfoY)
            {
                ++nCount;
            }

            if(nCount > 0)
            {
                *pTransitionCount = nCount;
                *pppVarible = new IUIAnimationVariable *[nCount];
                *pppTransition = new IUIAnimationTransition *[nCount];

                hr = S_OK;
                UINT32 nIndex = 0;

                if(NULL != m_pTranslateInfoX)
                {
                    hr = CreateVariable(m_pTranslateInfoX, &m_pVariableX, bUpdate);

                    if(SUCCEEDED(hr))
                    {
                        hr = CreateTransition(m_pTranslateInfoX, &m_pTransitionX, bUpdate);

                        if(SUCCEEDED(hr))
                        {
                            (*pppVarible)[nIndex] = m_pVariableX;
                            m_pVariableX->AddRef();

                            (*pppTransition)[nIndex] = m_pTransitionX;
                            m_pTransitionX->AddRef();

                            ++nIndex;
                        }
                    }
                }

                if(SUCCEEDED(hr))
                {
                    if(NULL != m_pTranslateInfoY)
                    {
                        hr = CreateVariable(m_pTranslateInfoY, &m_pVariableY, bUpdate);

                        if(SUCCEEDED(hr))
                        {
                            hr = CreateTransition(m_pTranslateInfoY, &m_pTransitionY, bUpdate);

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
        }
        
    }

    return hr;
}
