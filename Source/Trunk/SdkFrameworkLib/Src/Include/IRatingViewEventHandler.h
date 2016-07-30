/*!
* @file IRatingViewEventHandler.h 
* 
* @brief This file defines interface IRatingViewEventHandler for SdkRatingView.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/07/12
*/

#ifdef __cplusplus
#ifndef _IRATINGVIEWEVENTHANDLER_H_
#define _IRATINGVIEWEVENTHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief The rating view event handler
*/
class IRatingViewEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IRatingViewEventHandler() {};

    /*!
    * @brief Called when the rating is changed.
    *
    * @param pView      [I/ ] The event source, rating view.
    * @param nOldRating [I/ ] The old rating.
    * @param nCurRating [I/ ] The current rating.
    */
    virtual void OnRatingChanged(SdkRatingView *pView, int nOldRating, int nCurRating) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _IRATINGVIEWEVENTHANDLER_H_
#endif // __cplusplus
