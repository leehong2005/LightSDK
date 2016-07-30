/*!
* @file IProgressBarEventHandler.h
* 
* @brief This file defines the event for SdkProgressBar.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/07/13
*/

#ifdef __cplusplus
#ifndef _IPROGRESSBAREVENTHANDLER_H_
#define _IPROGRESSBAREVENTHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief IProgressBarEventHandler class.
*/
class IProgressBarEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IProgressBarEventHandler() {}

    /*!
    * @brief Called when the progress is changed.
    *
    * @param pView      [I/ ] The event view.
    * @param fPercent   [I/ ] The percent of the progress bar.
    */
    virtual void OnPercentChanged(SdkProgressBar *pView, FLOAT fPercent) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _IPROGRESSBAREVENTHANDLER_H_
#endif // __cplusplus
