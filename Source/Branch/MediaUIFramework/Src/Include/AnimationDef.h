/*! 
* @file AnimationDef.h 
* 
* @brief This file defines the struct about animation.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Qiao LiYun, Cui ShuYan
* @date 2010/01/06
*/

#ifdef __cplusplus
#ifndef _ANIMATIONDEF_H_
#define _ANIMATIONDEF_H_

#include "CommonInclude.h"

/*!
* @brief The type of the animation.
*/
typedef enum _TRANSFORM_TYPE
{
    TRANSFORM_TYPE_UNKNOWN      = 0,        // The unknown type.
    TRANSFORM_TYPE_TRANSLATE    = 1,        // The translate type.
    TRANSFORM_TYPE_SCALE        = 2,        // The scale type.
    TRANSFORM_TYPE_ROTATE       = 4,        // The rotate type.
    TRANSFORM_TYPE_ALPHA        = 8         // The alpha type.

} TRANSFORM_TYPE;

/*!
* @brief The transform information.
*/
typedef struct _TRANSFORMINFO
{
    UINT32              typeTransform;      // The transform type.
    D2D1_MATRIX_3X2_F   matrixTransform;    // The transformed matrix.
    double              dAlpha;             // The alpha value.

} TRANSFORMINFO, *LPTRANSFORMINFO;

/*!
* @brief The information of the animation.
*/
typedef struct _TRANSITIONINFO
{
    DOUBLE dFrom;                // The position of the start point.
    DOUBLE dTo;                  // The position of the end point.
    DOUBLE dDuration;            // The duration of the animation.
    DOUBLE dAccelerationRatio;   // The accelerate ratio of the animation.
    DOUBLE dDecelerationRatio;   // The decelerate ratio of the animation.

} TRANSITIONINFO, *LPTRANSITIONINFO;

/*!
* @brief ANIMATIONINFO2.
*/
typedef struct _ANIMATIONINFO2
{
    IUIAnimationTransition ** ppTransition;
    IUIAnimationVariable   ** ppVariable;
    UINT32                 nTransitionCount;

    _ANIMATIONINFO2()
    {
        ppTransition = NULL;
        ppVariable = NULL;
        nTransitionCount = 0;
    }

    ~_ANIMATIONINFO2()
    {
        if(NULL != ppTransition)
        {
            for(UINT32 nIndex = 0; nIndex < nTransitionCount; ++nIndex)
            {
                SAFE_RELEASE(ppTransition[nIndex]);
            }
            delete []ppTransition;
        }

        if(NULL != ppVariable)
        {
            for(UINT32 nIndex = 0; nIndex < nTransitionCount; ++nIndex)
            {
                SAFE_RELEASE(ppVariable[nIndex]);
            }
            delete []ppVariable;
        }
    }

} ANIMATIONINFO2, *LPANIMATIONINFO2;

#endif // _ANIMATIONDEF_H_
#endif // __cplusplus
