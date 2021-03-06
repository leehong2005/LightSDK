/*!
* @file IAnimationTimerListener.h 
* 
* @brief This file defines the event interface of animation timer.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Qiao LiYun
* @date 2011/02/15
*/

#ifdef __cplusplus
#ifndef _IANIMATIONTIMERLISTENER_H_
#define _IANIMATIONTIMERLISTENER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief The interface defines the common event handler for animation timer.
*/
class IAnimationTimerListener
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IAnimationTimerListener(){}

    /*!
    * @brief This method can be called when the timer update.
    */
    virtual void OnAnimationTimerUpdate(SdkAnimation *pAnimation) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _IANIMATIONTIMERLISTENER_H_
#endif //__cplusplus
