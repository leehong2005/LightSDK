/*!
* @file IAnimationTimerListener.h 
* 
* @brief This file defines the event interface of animation timer.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun
* @date 2011/02/15
*/

#ifdef __cplusplus
#ifndef _IANIMATIONTIMERLISTENER_H_
#define _IANIMATIONTIMERLISTENER_H_

#include "CommonInclude.h"

class Animation;

/*!
* @brief The interface defines the common event handler for animation timer.
*/
class CLASS_DECLSPEC IAnimationTimerListener
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IAnimationTimerListener(){}

    /*!
    * @brief This method can be called when the timer update.
    */
    virtual VOID OnAnimationTimerUpdate(Animation *pAnimation) = 0;
};

#endif // _IANIMATIONTIMERLISTENER_H_
#endif //__cplusplus
