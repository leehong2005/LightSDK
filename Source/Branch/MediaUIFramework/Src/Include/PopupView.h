/*!
* @file PopupView.h
* 
* @brief This file defines PopupView class which is popup view.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/18
*/

#ifdef __cplusplus
#ifndef _POPUPVIEW_H_
#define _POPUPVIEW_H_

#include "CommonInclude.h"
#include "ViewLayout.h"

class CLASS_DECLSPEC PopupView : public ViewLayout
{
public:

    /*!
    * @brief The constructor function.
    */
    PopupView();

    /*!
    * @brief The destructor function.
    */
    virtual ~PopupView();

    /*!
    * @brief Show or hide the view.
    *
    * @param isShow     [I/ ] The flag to indicate show or hide the view.
    */
    virtual void ShowView(BOOL isShow);

    /*!
    * @brief Enable or disable pop-up property.
    *
    * @param isPopupEnable  [I/ ] TRUE if enable, otherwise return FALSE.
    */
    virtual void SetPopupEnable(BOOL isPopupEnable);
};

#endif // _POPUPVIEW_H_
#endif // __cplusplus