/*!
* @file IViewOnClickHandler.h 
* 
* @brief This file defines the click event for view.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/16
*/

#ifdef __cplusplus
#ifndef _IVIEWONCLICKHANDLER_H_
#define _IVIEWONCLICKHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief The click event definition.
*/
class IViewOnClickHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IViewOnClickHandler() {};

    /*!
    * @brief The destructor function.
    *
    * @param view       [I/ ] The pointer SdkViewElement, you should NOT delete the pointer.
    */
    virtual void OnClick(SdkViewElement *pView) = 0;

    /*!
    * @brief Called when a view has been clicked and held.
    *
    * @param pView      [I/ ] The event source.
    *
    * @return TRUE if consumed the long click, FALSE otherwise.
    */
    virtual BOOL OnLongClick(SdkViewElement *pView) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _IVIEWONCLICKHANDLER_H_
#endif // __cplusplus