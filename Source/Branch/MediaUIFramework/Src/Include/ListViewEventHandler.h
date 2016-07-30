/*!
* @file ListViewEventHandler.h 
* 
* @brief This file defines the event for List view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/18
*/

#ifdef __cplusplus
#ifndef _LISTVIEWEVENTHANDLER_H_
#define _LISTVIEWEVENTHANDLER_H_

#include "CommonInclude.h"

class ListBox;

/*!
* @brief ListViewEventHandler class.
*/
class CLASS_DECLSPEC ListViewEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ListViewEventHandler() {};

    /*!
    * @brief Called when selected item is changed.
    *
    * @param pView  [I/ ] The event source.
    */
    virtual void OnSelectedChanged(ListBox *pView);

    /*!
    * @brief Called when visibility is changed.
    *
    * @param pView  [I/ ] The event source.
    */
    virtual void OnVisibleChanged(ListBox *pView);
};

#endif // _LISTVIEWEVENTHANDLER_H_
#endif // __cplusplus
