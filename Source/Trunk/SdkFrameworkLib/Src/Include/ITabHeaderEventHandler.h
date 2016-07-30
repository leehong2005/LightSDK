/*!
* @file ITabHeaderEventHandler.h
* 
* @brief This file defines the event for SdkComboBox.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/19
*/

#ifdef __cplusplus
#ifndef _ITABHEADEREVENTHANDLER_H_
#define _ITABHEADEREVENTHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief ITabHeaderEventHandler interface.
*/
class ITabHeaderEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ITabHeaderEventHandler() {};

    /*!
    * @brief Called when selection of tab is changed.
    *
    * @param pTabHeader     [I/ ] The event source.
    * @param nOldIndex      [I/ ] The old selected index.
    * @param nCurIndex      [I/ ] The current selected index.
    */
    virtual void OnSelectedChanged(SdkTabHeader *pTabHeader, int nOldIndex, int nCurIndex) = 0;

    /*!
    * @brief Called when user click the add tab button, the adding operation should be
    *        performed by client side.
    *
    * @param pTabHeader     [I/ ] The event source.
    */
    virtual BOOL OnInsertTab(SdkTabHeader *pTabHeader) = 0;

    /*!
    * @brief Called when user delete a tab by clicking the close button on the tab, before 
    *        invoking this method, the tab has been deleted. in the implement of this method,
    *        the client side should do the cleanup operation if there is associated data to 
    *        the deleted tab.
    *
    * @param pTabHeader     [I/ ] The event source.
    * @param nDelIndex      [I/ ] The index of the deleted tab.
    */
    virtual BOOL OnDeleteTab(SdkTabHeader *pTabHeader, int nDelIndex) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _ITABHEADEREVENTHANDLER_H_
#endif // __cplusplus
