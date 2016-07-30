/*!
* @file ITabViewEventHandler.h
* 
* @brief This file defines the event for SdkComboBox.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/19
*/

#ifdef __cplusplus
#ifndef _ITABVIEWEVENTHANDLER_H_
#define _ITABVIEWEVENTHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief ITabViewEventHandler interface.
*/
class ITabViewEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ITabViewEventHandler() {};

    /*!
    * @brief Called when selection of tab is changed.
    *
    * @param pTabView       [I/ ] The event source.
    * @param nOldIndex      [I/ ] The old selected index.
    * @param nCurIndex      [I/ ] The current selected index.
    */
    virtual void OnSelectedChanged(SdkTabView *pTabView, int nOldIndex, int nCurIndex) = 0;

    /*!
    * @brief Called when user click the add tab button, the adding operation should be
    *        performed by client side.
    *
    * @param pTabView      [I/ ] The event source.
    */
    virtual void OnInsertTab(SdkTabView *pTabView) = 0;

    /*!
    * @brief Called when user delete a tab by clicking the close button on the tab, before 
    *        invoking this method, the tab has been deleted. in the implement of this method,
    *        the client side should do the cleanup operation if there is associated data to 
    *        the deleted tab.
    *
    * @param pTabView       [I/ ] The event source.
    * @param nDelIndex      [I/ ] The index of the deleted tab.
    */
    virtual void OnDeleteTab(SdkTabView *pTabView, int nDelIndex) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _ITABVIEWEVENTHANDLER_H_
#endif // __cplusplus
