/*!
* @file D3DBasicAnimation.h 
* 
* @brief This file defines the base functions of _D3DKeyFrameAnimation_H_.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Guo Kai
* @date 2010/12/28
*/

#ifdef __cplusplus
#ifndef _D3DKEYFRAMEANIMATION_H_
#define _D3DKEYFRAMEANIMATION_H_

#include "SdkAnimation.h"
#include <d3dx9.h>
#include "SdkCommonHelper.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_D3D

template<typename T1>
class CLASS_DECLSPEC D3DKeyFrameAnimation : public SdkAnimation
{
public:

    typedef struct _D3DKEYFRAME
    {
        DOUBLE time;
        T1 val;
    } D3DKEYFRAME, LPD3DKEYFRAME ;

    /*!
    * @brief construction function.
    *
    */
    D3DKeyFrameAnimation();

    /*!
    * @brief destruction function.
    *
    */
    virtual ~D3DKeyFrameAnimation();


    void initKeyFrames( const vector<D3DKEYFRAME>& keyframes );

    void AddKeyFrame( const D3DKEYFRAME& keyframes );

    void ClearKeyFrames();

    /*!
    * @brief Get the value of the variable.
    *
    * @param pValue             [I/O] The variable value.
    * @param bUpdate            [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    HRESULT GetValue(OUT T1 *pValue, BOOL bUpdate = TRUE);

    HRESULT GetKeyFrameValue( OUT T1 *pValue, IN  DOUBLE  currentTime, BOOL bUpdate = TRUE);
    
   /*!
    * @brief Get the animation type.
    *
    * @return The animation type.
    */
    INT GetAnimationType();

   /*!
    * @brief Get the duration of the alpha animation.
    *
    * @param pDuration      [ /O] The duration.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetDuration(OUT DOUBLE *pDuration);

    /*!
    * @brief Get the transform of the alpha animation.
    *
    * @param pTransform     [ /O] The transform information.
    * @param bUpdate        [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetTransform(OUT LPTRANSFORMINFO pTransform,
                                 IN  BOOL bUpdate = TRUE)
    {
        UNREFERENCED_PARAMETER(pTransform);
        UNREFERENCED_PARAMETER(bUpdate);
        return E_NOTIMPL;
    };

    /*!
    * @brief Get the information of the alpha animation.
    *
    * @param pppVarible             [ /O] The variable information of the alpha animation.
    * @param pppTransition          [ /O] The transition information of the alpha animation.
    * @param pTransitionCount       [ /O] The count of the variable.
    * @param bUpdate                [I/ ] The flag which indicates to update the animation value or not.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT GetAnimationInfo(OUT  IUIAnimationVariable ***pppVarible,
                                     OUT  IUIAnimationTransition ***pppTransition,
                                     OUT  UINT *pTransitionCount,
                                     BOOL bUpdate = TRUE);

protected:

    /*!
    * @brief Initialize the storyboard of the alpha animation.
    *
    * @return S_OK if success, otherwise return E_FAIL.
    */
    virtual HRESULT InitStoryboard();

private:
    vector<D3DKEYFRAME>             m_keyframes;

    LPTRANSITIONINFO                m_pValueInfo;       // The information of the alpha animation.
    IUIAnimationVariable            *m_pVariable;        // The variable of the alpha animation.
    IUIAnimationTransition          *m_pTransition;     // The transition of the alpha animation.

};


////////////////////////////////////////////////////////////////////////////
template<typename T1>
D3DKeyFrameAnimation<T1>::D3DKeyFrameAnimation()
{
    m_nAnimationType = TRANSFORM_TYPE_UNKNOWN;


    //m_pValueInfo = new TRANSITIONINFO();
    //m_pValueInfo->dFrom = dFromValue;
    //m_pValueInfo->dTo = dToValue;
    //m_pValueInfo->dDuration = dDuration;
    //m_pValueInfo->dAccelerationRatio = dAccelerationRatio;
    //m_pValueInfo->dDecelerationRatio = dDecelerationRatio;
    m_pValueInfo = NULL;
    m_pVariable = NULL;
    m_pTransition = NULL;
}

//////////////////////////////////////////////////////////////////////////
template<typename T1>
D3DKeyFrameAnimation<T1>::~D3DKeyFrameAnimation(void)
{
    SAFE_DELETE(m_pValueInfo);
    SAFE_RELEASE(m_pVariable);
    SAFE_RELEASE(m_pTransition);
}

//////////////////////////////////////////////////////////////////////////
template<typename T1>
void D3DKeyFrameAnimation<T1>::initKeyFrames( const vector<D3DKEYFRAME>& keyframes )
{
    for ( UINT32 i = 0; i < keyframes.size(); i++ )
    {
        m_keyframes.push_back( keyframes[i] );
    }
    m_nAnimationType = TRANSFORM_TYPE_UNKNOWN;
    m_pValueInfo = new TRANSITIONINFO();
    m_pValueInfo->dFrom = m_keyframes[0].time;
    m_pValueInfo->dTo = m_keyframes[m_keyframes.size() - 1].time;
    m_pValueInfo->dDuration = m_pValueInfo->dTo - m_pValueInfo->dFrom;
    m_pValueInfo->dAccelerationRatio = 0.0;
    m_pValueInfo->dDecelerationRatio = 0.0;
}

//////////////////////////////////////////////////////////////////////////////
template<typename T1>
HRESULT D3DKeyFrameAnimation<T1>::GetValue(OUT T1  *pValue, 
                                              BOOL        bUpdate)
{
    DOUBLE curTime = 0.0f;
    HRESULT hr = GetVariableValue(&curTime, m_pVariable, bUpdate);
    GetKeyFrameValue( pValue, curTime );
    //CommonHelper::PrintDebugString(L"Anim curtime: %f value %f\n", curTime, *pValue);
    return hr;
}

//////////////////////////////////////////////////////////////////////////
template<typename T1>
HRESULT D3DKeyFrameAnimation<T1>::GetKeyFrameValue(
    OUT T1  *pValue,
    IN  DOUBLE  currentTime,
    BOOL        /*bUpdate*/)
{
    HRESULT hr = S_OK;
    DOUBLE curTime = currentTime;

    // get the key frame length
    UINT32 frameLength = m_keyframes.size();
    // get the time different
    DOUBLE Time = ( curTime > 0 )? curTime:0;
    INT32 keyFrameLength = frameLength;
    T1 dataChanged;
    BOOL endAnim = FALSE;

    DOUBLE currentPersent = 0.0f;

    // check current time is larger than last key frame time
    if ( Time >= m_keyframes[keyFrameLength - 1].time )
    {
        currentPersent = 1.0f;
        // animation is end
        dataChanged = m_keyframes[keyFrameLength - 1].val;
        endAnim = TRUE;
    }
    else
    {
        // check current time is smaller than first key frame time
        if ( Time <= m_keyframes[0].time )
        {
            // set the data to be the first key frame's data
            dataChanged = m_keyframes[0].val;
            currentPersent = 0.0f;
        }
        else
        {
            currentPersent = Time / m_keyframes[keyFrameLength - 1].time;
            // get the percent of current time in all key frame times
            //Time /= (m_keyframes[keyFrameLength - 1].time /*+ 1*/);
            DWORD Keyframe = 0;

            // get the closer key frame's index whose time is smaller than current
            for (int i = 0; i < keyFrameLength; i++)
            {
                if (Time >= m_keyframes[i].time)
                {
                    Keyframe = i;
                }
            }

            // get the closer key frame's index whose time is larger than current
            DWORD Keyframe2 = (Keyframe == (DWORD)(keyFrameLength - 1)) ? Keyframe : Keyframe + 1;

            // get time different
            DOUBLE TimeDiff = m_keyframes[Keyframe2].time - 
                m_keyframes[Keyframe].time;
            if(!TimeDiff) 
                TimeDiff=1;
            //CommonHelper::PrintDebugString(L"Timediff: %f\n", TimeDiff);
            // scale time
            DOUBLE Scalar = (Time - m_keyframes[Keyframe].time) / TimeDiff;

            // get the scaled data
            dataChanged = m_keyframes[Keyframe2].val 
                - m_keyframes[Keyframe].val;
#pragma warning(disable : 4244)
            dataChanged *= Scalar;
            dataChanged += m_keyframes[Keyframe].val;
#pragma warning(default : 4244)
            //CommonHelper::PrintDebugString(L"Scalar: %f, keyframe diff: %f base data: %f\n", Scalar,
            //m_keyframes[Keyframe2].val - m_keyframes[Keyframe].val,
            //m_keyframes[Keyframe].val);
        }
    }

    if ( pValue != NULL )
    {
        *pValue = dataChanged;
    }
    //if ( isEndAnim != NULL)
    //{
    //    *isEndAnim = endAnim;
    //}
    //if ( persent != NULL )
    //{
    //    *persent = currentPersent;
    //}

    return hr;
}


//////////////////////////////////////////////////////////////////////////
template<typename T1>
INT D3DKeyFrameAnimation<T1>::GetAnimationType()
{
    return m_nAnimationType;
}

//////////////////////////////////////////////////////////////////////////
template<typename T1>
HRESULT D3DKeyFrameAnimation<T1>::GetDuration(OUT DOUBLE *pDuration)
{
    HRESULT hr = E_FAIL;

    if(NULL != m_pTransition)
    {
        hr = m_pTransition->GetDuration(pDuration);
    }

    return hr;
}
//////////////////////////////////////////////////////////////////////////
template<typename T1>
HRESULT D3DKeyFrameAnimation<T1>::GetAnimationInfo(OUT  IUIAnimationVariable ***pppVarible,
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
template<typename T1>
HRESULT D3DKeyFrameAnimation<T1>::InitStoryboard()
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

template<typename T1>
void D3DKeyFrameAnimation<T1>::AddKeyFrame( const D3DKEYFRAME& keyframes )
{
    this->m_keyframes.push_back(keyframes);
    if ( m_keyframes.size() > 1 )
    {
        m_nAnimationType = TRANSFORM_TYPE_UNKNOWN;
        if ( NULL == m_pValueInfo )
        {
            m_pValueInfo = new TRANSITIONINFO();
        }
        
        m_pValueInfo->dFrom = m_keyframes[0].time;
        m_pValueInfo->dTo = m_keyframes[m_keyframes.size() - 1].time;
        m_pValueInfo->dDuration = m_pValueInfo->dTo - m_pValueInfo->dFrom;
        m_pValueInfo->dAccelerationRatio = 0.0;
        m_pValueInfo->dDecelerationRatio = 0.0;
    }
}

template<typename T1>
void D3DKeyFrameAnimation<T1>::ClearKeyFrames()
{
    this->m_keyframes.clear();
}

END_NAMESPACE_D3D

#endif //_D3DANIMATION_H_
#endif //__cplusplus