/*!
* @file ViewOnLongClickHandler.h 
* 
* @brief This file defines the long click event for view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/16
*/

#ifdef __cplusplus
#ifndef _VIEWONLONGCLICKHANDLER_H_
#define _VIEWONLONGCLICKHANDLER_H_

class ViewElement;

#include "CommonInclude.h"

/*!
* @brief Long click definition.
**/
class CLASS_DECLSPEC ViewOnLongClickHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ViewOnLongClickHandler() {};

    /*!
    * @brief Called when a view has been clicked and held.
    *
    * @param pView      [I/ ] The event source.
    *
    * @return TRUE if consumed the long click, FALSE otherwise.
    */
    virtual BOOL OnLongClick(ViewElement *pView);
};

#endif // _VIEWONLONGCLICKHANDLER_H_
#endif // __cplusplus