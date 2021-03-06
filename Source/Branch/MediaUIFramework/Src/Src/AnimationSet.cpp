/*!
* @file AnimationSet.cpp 
* 
* @brief This file defines the functions of animation set.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2010/12/28
*/


#include "AnimationSet.h"

//////////////////////////////////////////////////////////////////////////

AnimationSet::AnimationSet()
{
}

//////////////////////////////////////////////////////////////////////////

AnimationSet::~AnimationSet()
{
    ClearAnimation();
    ClearAnimationInfo();
}

//////////////////////////////////////////////////////////////////////////

VOID AnimationSet::AddAnimation(Animation *pAnimation)
{
    if(NULL != pAnimation)
    {
        m_listAnimation.push_back(pAnimation);
        m_nAnimationType |= pAnimation->GetAnimationType();
    }
}

//////////////////////////////////////////////////////////////////////////

HRESULT AnimationSet::GetDuration( OUT DOUBLE * /*pDuration*/ )
{
    HRESULT hr = E_FAIL;

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT AnimationSet::GetTransform(OUT LPTRANSFORMINFO pTransform, IN BOOL bUpdate)
{
    HRESULT hr = E_FAIL;

    if(NULL != pTransform)
    {
        if(bUpdate)
        {
            hr = this->Update();
        }
        else
        {
            hr = S_OK;
        }

        if(SUCCEEDED(hr))
        {
            pTransform->typeTransform = m_nAnimationType;
            pTransform->matrixTransform = D2D1::IdentityMatrix();
            static TRANSFORMINFO TempInfo;
            for(list<Animation*>::iterator iter = m_listAnimation.begin();
                iter != m_listAnimation.end();
                ++iter)
            {
                if(NULL != *iter )
                {
                    if(SUCCEEDED((*iter)->GetTransform(&TempInfo, bUpdate)))
                    {
                        pTransform->matrixTransform = pTransform->matrixTransform * TempInfo.matrixTransform;
                        
                        if(TRANSFORM_TYPE_ALPHA == TempInfo.typeTransform)
                        {
                            pTransform->dAlpha = TempInfo.dAlpha;
                        }
                    }
                }
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT AnimationSet::InitStoryboard()
{

    SAFE_RELEASE(m_pStoryBoard);
    ClearAnimationInfo();
    HRESULT hr = m_pAnimationCom->CreateStoryboard(&m_pStoryBoard);
    if(SUCCEEDED(hr))
    {
        for(list<Animation*>::iterator iter = m_listAnimation.begin();
            iter != m_listAnimation.end();
            ++iter)
        {
            if(NULL != *iter)
            {
                LPANIMATIONINFO2 pInfo = new ANIMATIONINFO2();

                hr = (*iter)->GetAnimationInfo(&pInfo->ppVariable, 
                    &pInfo->ppTransition, &pInfo->nTransitionCount);
                if(SUCCEEDED(hr))
                {
                    hr = AddAnimation(pInfo);
                    if(SUCCEEDED(hr))
                    {
                        m_listAniamtionInfo.push_back(pInfo);
                    }
                }

                if(FAILED(hr))
                {
                    SAFE_DELETE(pInfo);
                    break;
                }
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

HRESULT AnimationSet::AddAnimation(const LPANIMATIONINFO2 pInfo)
{
    HRESULT hr = E_FAIL;

    if(NULL == pInfo || NULL == pInfo->ppTransition || NULL == pInfo->ppVariable)
    {
        hr = E_INVALIDARG;
    }
    else if (NULL != m_pStoryBoard)
    {
        for(UINT32 nIndex = 0; nIndex < pInfo->nTransitionCount; ++nIndex )
        {
            hr = m_pStoryBoard->AddTransition((pInfo->ppVariable)[nIndex],
                    (pInfo->ppTransition)[nIndex]);
            if(FAILED(hr))
            {
                break;
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////

void AnimationSet::ClearAnimation()
{
    for(list<Animation*>::iterator iter = m_listAnimation.begin();
        iter != m_listAnimation.end();
        ++iter)
    {
        SAFE_DELETE(*iter);
    }
    m_listAnimation.clear();
}

//////////////////////////////////////////////////////////////////////////

void AnimationSet::ClearAnimationInfo()
{
    for(list<LPANIMATIONINFO2>::iterator iter = m_listAniamtionInfo.begin();
        iter != m_listAniamtionInfo.end();
        ++iter)
    {
        SAFE_DELETE(*iter);
    }
    m_listAniamtionInfo.clear();
}

//////////////////////////////////////////////////////////////////////////

HRESULT AnimationSet::GetAnimationInfo(OUT IUIAnimationVariable *** /*pppVarible*/,
                         OUT IUIAnimationTransition *** /*pppTransition*/,
                         OUT UINT32 * /*pTransitionCount*/,
                         BOOL /*bUpdate = TRUE*/)
{
    HRESULT hr = E_FAIL;

    return hr;
}
