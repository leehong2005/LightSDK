/*!
* @file SdkAnimationCom.cpp 
* 
* @brief This file defines the common functions for animation.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#include "stdafx.h"
#include "SdkAnimationCom.h"
#include "SdkAnimation.h"
#include "SdkAnimationTimerEventHandler.h"
#include "IAnimationTimerListener.h"

USING_NAMESPACE_ANIMATION

//////////////////////////////////////////////////////////////////////////

SdkAnimationCom::SdkAnimationCom(): m_pAnimationManager(NULL),
                              m_pAnimationTimer(NULL),
                              m_pAnimationTransitionLib(NULL),
                              m_pTimerEventHandler(NULL)
{
    this->Initialize();
}

//////////////////////////////////////////////////////////////////////////

SdkAnimationCom::~SdkAnimationCom()
{
    SAFE_RELEASE(m_pAnimationManager);
    SAFE_RELEASE(m_pTimerEventHandler);
    SAFE_RELEASE(m_pAnimationTimer);
    SAFE_RELEASE(m_pAnimationTransitionLib);
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::Initialize()
{
    HRESULT hr = S_OK;

    if (NULL == m_pAnimationManager)
    {
        hr = ::CoCreateInstance(
            CLSID_UIAnimationManager,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_pAnimationManager));
    }

    if (SUCCEEDED(hr))
    {
        if (NULL == m_pAnimationTimer)
        {
            hr = ::CoCreateInstance(
                CLSID_UIAnimationTimer,
                NULL,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&m_pAnimationTimer));
        }
    }

    if (SUCCEEDED(hr))
    {
        SAFE_RELEASE(m_pAnimationTransitionLib);

        hr = ::CoCreateInstance(
            CLSID_UIAnimationTransitionLibrary,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_pAnimationTransitionLib));
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::IsFinish(OUT BOOL *pIsFinish)
{
    HRESULT hr = E_FAIL;

    if (NULL != pIsFinish && NULL != m_pAnimationManager)
    {
        UI_ANIMATION_MANAGER_STATUS status;
        hr = m_pAnimationManager->GetStatus(&status);
        if (SUCCEEDED(hr))
        {
            (*pIsFinish) = (UI_ANIMATION_MANAGER_BUSY == status) ? FALSE : TRUE;
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::GetTime(OUT UI_ANIMATION_SECONDS* seconds)
{
    return (NULL != m_pAnimationTimer) ? m_pAnimationTimer->GetTime(seconds) : E_FAIL;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::AddTimerListener(IAnimationTimerListener *pListener, SdkAnimation *pAnimation)
{
    HRESULT hr = S_OK;

    if (NULL == m_pTimerEventHandler)
    {
        hr = this->Initialize();
        if ( SUCCEEDED(hr) )
        {
            IUIAnimationTimerUpdateHandler *pTimerUpdateHandler = NULL;
            hr = m_pAnimationManager->QueryInterface(IID_PPV_ARGS(&pTimerUpdateHandler));
            if ( SUCCEEDED(hr) )
            {
                hr = m_pAnimationTimer->SetTimerUpdateHandler(
                    pTimerUpdateHandler,
                    UI_ANIMATION_IDLE_BEHAVIOR_DISABLE);

                if (SUCCEEDED(hr))
                {
                    // Create and set the Timer Event Handler
                    m_pTimerEventHandler = new SdkAnimationTimerEventHandler();
                    m_pTimerEventHandler->AddListener(pListener, pAnimation);
                    hr = m_pAnimationTimer->SetTimerEventHandler(m_pTimerEventHandler);
                }
            }

            SAFE_RELEASE(pTimerUpdateHandler);
        }
    }
    else
    {
        m_pTimerEventHandler->AddListener(pListener, pAnimation);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::RemoveTimerListener(IAnimationTimerListener *pListener)
{
    if (NULL != m_pTimerEventHandler)
    {
        m_pTimerEventHandler->RemoveListener(pListener);
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::CreateStoryboard(IUIAnimationStoryboard ** ppStoryBoard)
{
    HRESULT hr = E_FAIL;

    if(NULL != m_pAnimationManager)
    {
        hr = m_pAnimationManager->CreateStoryboard(ppStoryBoard);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::Update()
{
    UI_ANIMATION_SECONDS timeNow;
    HRESULT hr = this->GetTime(&timeNow);
    if( SUCCEEDED(hr) )
    {
        hr = this->Update(timeNow);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::Update(UI_ANIMATION_SECONDS timeNow)
{
    if (NULL == m_pAnimationManager)
    {
        return E_FAIL;
    }

    return m_pAnimationManager->Update(timeNow, NULL);
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::CreateVariable(IN DOUBLE dInitValue, OUT IUIAnimationVariable **ppVariable)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pAnimationManager)
    {
        hr = m_pAnimationManager->CreateAnimationVariable(dInitValue, ppVariable);
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::CreateVariable(
    IN  DOUBLE dInitValue,
    IN  DOUBLE dLowerBound,
    IN  DOUBLE dUpperBound,
    OUT IUIAnimationVariable **ppVariable)
{
    HRESULT hr = E_FAIL;

    if(NULL != m_pAnimationManager)
    {
        hr = m_pAnimationManager->CreateAnimationVariable(dInitValue, ppVariable);
        if(SUCCEEDED(hr))
        {
            hr = (*ppVariable)->SetLowerBound(dLowerBound);
            if(SUCCEEDED(hr))
            {
                hr = (*ppVariable)->SetUpperBound(dUpperBound);
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkAnimationCom::CreateAccelerateDecelerateTransition(
    IN UI_ANIMATION_SECONDS duration,
    IN DOUBLE finalValue,
    IN DOUBLE accelerationRatio,
    IN DOUBLE decelerationRatio,
    OUT IUIAnimationTransition **ppTransition)
{
    HRESULT hr = E_FAIL;

    if (NULL != m_pAnimationTransitionLib)
    {
        hr = m_pAnimationTransitionLib->CreateAccelerateDecelerateTransition(
            duration,
            finalValue,
            accelerationRatio,
            decelerationRatio,
            ppTransition);
    }

    return hr;
}
