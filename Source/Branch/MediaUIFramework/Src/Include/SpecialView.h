/*!
* @file SpecialView.h
* 
* @brief This file defines SpecialView for special functions. This view would clear
*        the drawing rectangle without painting.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/03/31
*/

#ifdef __cplusplus
#ifndef _SPECIALVIEW_H_
#define _SPECIALVIEW_H_

#include "Common.h"
#include "ViewElement.h"

class CLASS_DECLSPEC SpecialView : public ViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    SpecialView();

    /*!
    * @brief The destructor function.
    */
    virtual ~SpecialView();

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);
};

#endif // _SPECIALVIEW_H_
#endif // __cplusplus
