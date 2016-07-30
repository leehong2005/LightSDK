/*!
* @file ViewOnClickHandler.h 
* 
* @brief This file defines the click event for view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/16
*/

#ifdef __cplusplus
#ifndef _VIEWONCLICKHANDLER_H_
#define _VIEWONCLICKHANDLER_H_

#include "CommonInclude.h"

class ViewElement;

/*!
* @brief The click event definition.
*/
class CLASS_DECLSPEC ViewOnClickHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ViewOnClickHandler() {};

    /*!
    * @brief The destructor function.
    *
    * @param view       [I/ ] The pointer ViewElement, you should NOT delete the pointer.
    */
    virtual void OnClick(ViewElement *pView);
};

#endif // _VIEWONCLICKHANDLER_H_
#endif // __cplusplus