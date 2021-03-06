/*!
* @file SdkStoryboardEventHandler.cpp 
* 
* @brief This file defines the event handler of storyboard.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/

#include "stdafx.h"
#include "SdkStoryboardEventHandler.h"
#include <UIAnimation.h>
#include "IAnimationListener.h"
#include "SdkAnimation.h"

USING_NAMESPACE_ANIMATION

//////////////////////////////////////////////////////////////////////////

SdkStoryboardEventHandler::SdkStoryboardEventHandler() :
    m_pAnimation(NULL),
    m_lRefCount(1)
{
}

//////////////////////////////////////////////////////////////////////////

SdkStoryboardEventHandler::~SdkStoryboardEventHandler()
{
}

//////////////////////////////////////////////////////////////////////////

void SdkStoryboardEventHandler::AddAnimationListener(IAnimationListener *pListener, SdkAnimation *pAnimation)
{
    m_pAnimation = pAnimation;
    if (NULL != pListener)
    {
        BOOL bFind = FALSE;
        for ( vector<IAnimationListener*>::iterator iter = m_vecListener.begin();
              iter != m_vecListener.end();
              ++iter )
        {
            if(*iter == pListener)
            {
                bFind = TRUE;
                break;
            }
        }

        if(!bFind)
        {
            m_vecListener.push_back(pListener);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void SdkStoryboardEventHandler::RemoveAnimationListener(IAnimationListener *pListener)
{
    if(NULL != pListener)
    {
        for ( vector<IAnimationListener*>::iterator iter = m_vecListener.begin();
              iter != m_vecListener.end();
              ++iter )
        {
            if (*iter == pListener)
            {
                m_vecListener.erase(iter);
                break;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkStoryboardEventHandler::OnStoryboardStatusChanged(
    IUIAnimationStoryboard *storyboard,
    UI_ANIMATION_STORYBOARD_STATUS newStatus,
    UI_ANIMATION_STORYBOARD_STATUS previousStatus)
{
    UNREFERENCED_PARAMETER(storyboard);

    IAnimationListener *pListener = NULL;
    for ( vector<IAnimationListener*>::iterator iter = m_vecListener.begin();
          iter != m_vecListener.end();
          ++iter )
    {
        pListener = *iter;
        if (NULL != pListener)
        {
            if ( previousStatus == UI_ANIMATION_STORYBOARD_BUILDING &&
                 newStatus == UI_ANIMATION_STORYBOARD_SCHEDULED )
            {
                pListener->OnAnimationStart(m_pAnimation);
            }
            else if ( previousStatus == UI_ANIMATION_STORYBOARD_SCHEDULED &&
                      newStatus == UI_ANIMATION_STORYBOARD_PLAYING )
            {
                pListener->OnAnimationPlaying(m_pAnimation);
            }
            else if ( (previousStatus == UI_ANIMATION_STORYBOARD_PLAYING) &&
                      (newStatus == UI_ANIMATION_STORYBOARD_FINISHED) )
            {
                pListener->OnAnimationEnd(m_pAnimation);
            }
        }
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkStoryboardEventHandler::OnStoryboardUpdated(IUIAnimationStoryboard *storyboard)
{
    UNREFERENCED_PARAMETER(storyboard);

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

HRESULT SdkStoryboardEventHandler::QueryInterface(
    REFIID riid,
    __RPC__deref_out void __RPC_FAR *__RPC_FAR * ppvObject)
{
    static const QITAB qit[] = 
    {
        QITABENT(SdkStoryboardEventHandler, IUIAnimationStoryboardEventHandler),
        { 0 },
    };

    return QISearch(this, qit, riid, ppvObject);
}

//////////////////////////////////////////////////////////////////////////

ULONG SdkStoryboardEventHandler::AddRef()
{
    return InterlockedIncrement(&m_lRefCount);
}

//////////////////////////////////////////////////////////////////////////

ULONG SdkStoryboardEventHandler::Release()
{
    LONG lRef = InterlockedDecrement(&m_lRefCount);
    if ( 0 == lRef )
    {
        delete this;
    }

    return lRef;
}
