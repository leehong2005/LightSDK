/*!
* @file SdkAnimationTimerEventHandler.cpp 
* 
* @brief This file defines the event handler of animation timer.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2011/02/15
*/

#include "stdafx.h"
#include <UIAnimation.h>
#include "SdkAnimationTimerEventHandler.h"
#include "IAnimationTimerListener.h"

USING_NAMESPACE_ANIMATION

//////////////////////////////////////////////////////////////////////////

SdkAnimationTimerEventHandler::SdkAnimationTimerEventHandler() :
    m_pAnimation(NULL),
    m_lRefCount(1)
{
}

//////////////////////////////////////////////////////////////////////////

SdkAnimationTimerEventHandler::~SdkAnimationTimerEventHandler()
{
}

//////////////////////////////////////////////////////////////////////////

void SdkAnimationTimerEventHandler::AddListener(IAnimationTimerListener *pListener, SdkAnimation *pAnimation)
{
    this->m_pAnimation = pAnimation;

    if (NULL != pListener)
    {
        BOOL bFind = FALSE;

        for ( vector<IAnimationTimerListener*>::iterator iter = m_vecAnimationTimerListener.begin();
              iter != m_vecAnimationTimerListener.end();
              ++iter )
        {
            if (*iter == pListener)
            {
                bFind = TRUE;
                break;
            }
        }

        if (!bFind)
        {
            m_vecAnimationTimerListener.push_back(pListener);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkAnimationTimerEventHandler::RemoveListener(IAnimationTimerListener *pListener)
{
    if (NULL != pListener)
    {
        for ( vector<IAnimationTimerListener*>::iterator iter = m_vecAnimationTimerListener.begin();
              iter != m_vecAnimationTimerListener.end();
              ++iter )
        {
            if (*iter == pListener)
            {
                m_vecAnimationTimerListener.erase(iter);
                break;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP SdkAnimationTimerEventHandler::OnPreUpdate()
{
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP SdkAnimationTimerEventHandler::OnPostUpdate()
{
    for ( vector<IAnimationTimerListener*>::iterator iter = m_vecAnimationTimerListener.begin();
          iter != m_vecAnimationTimerListener.end();
          ++iter )
    {
        if (*iter != NULL)
        {
            (*iter)->OnAnimationTimerUpdate(m_pAnimation);
            break;
        }
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP SdkAnimationTimerEventHandler::OnRenderingTooSlow(UINT32 fps)
{
    UNREFERENCED_PARAMETER(fps);

    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP SdkAnimationTimerEventHandler::QueryInterface(
    REFIID riid,
    __RPC__deref_out void __RPC_FAR *__RPC_FAR * ppvObject)
{
    static const QITAB qit[] = 
    {
        QITABENT(SdkAnimationTimerEventHandler, IUIAnimationTimerEventHandler),
        { 0 },
    };

    return QISearch(this, qit, riid, ppvObject);
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP_(ULONG) SdkAnimationTimerEventHandler::AddRef()
{
    return InterlockedIncrement(&m_lRefCount);
}

//////////////////////////////////////////////////////////////////////////

IFACEMETHODIMP_(ULONG) SdkAnimationTimerEventHandler::Release()
{
    LONG lRef = InterlockedDecrement(&m_lRefCount);
    if ( 0 == lRef )
    {
        delete this;
    }

    return lRef;
}
