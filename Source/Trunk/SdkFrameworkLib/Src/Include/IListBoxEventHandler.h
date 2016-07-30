/*!
* @file IListBoxEventHandler.h 
* 
* @brief This file defines the event for List view.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/18
*/

#ifdef __cplusplus
#ifndef _ILISTVIEWEVENTHANDLER_H_
#define _ILISTVIEWEVENTHANDLER_H_

#include "SdkCommonInclude.h"
#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief IListBoxEventHandler class.
*/
class IListBoxEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IListBoxEventHandler() {};

    /*!
    * @brief Called when selected item is changed.
    *
    * @param pView  [I/ ] The event source.
    */
    virtual void OnSelectedChanged(SdkListBox *pView) = 0;

    /*!
    * @brief Called when visibility is changed.
    *
    * @param pView  [I/ ] The event source.
    */
    virtual void OnVisibleChanged(SdkListBox *pView) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _ILISTVIEWEVENTHANDLER_H_
#endif // __cplusplus
