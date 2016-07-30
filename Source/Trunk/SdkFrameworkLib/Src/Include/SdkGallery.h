/*!
* @file SdkGallery.h 
* 
* @brief This file defines class SdkGallery, which supports drag to slide and sliding animation after dragging.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/28
*/

#ifdef __cplusplus
#ifndef _SDKGALLERY_H_
#define _SDKGALLERY_H_

#include "SdkSlideLayout.h"

BEGIN_NAMESPACE_VIEWS

class CLASS_DECLSPEC SdkGallery : public SdkSlideLayout
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkGallery();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkGallery();

    /*!
    * @brief Set the margin between elements.
    *
    * @param fMargin    [I/ ] The margin value.
    */
    virtual void SetChildMargin(FLOAT fMargin);

    /*!
    * @brief Set the elements' size.
    *
    * @param fWidth     [I/ ] The width.
    * @param fHeight    [I/ ] The height.
    */
    virtual void SetChildSize(FLOAT fWidth, FLOAT fHeight);

protected:

    /*!
    * @brief Calculate child views' start and end index.
    *        Derived class should implement this function to tell rendering views' index.
    *
    * @param pStartIndex    [ /O] The start index of first rendering view.
    * @param pEndIndex      [ /O] The end index of last rendering view.
    */
    virtual void CalcChildViewIndex(OUT INT32 *pStartIndex, OUT INT32 *pEndIndex);

    /*!
    * @brief Called when the layout of view is changed.
    *
    * @param fChanged       [I/ ] Indicates whether is changed.
    * @param left           [I/ ] The left value.
    * @param top            [I/ ] The top value.
    * @param width          [I/ ] The width of view.
    * @param height         [I/ ] The height of view.
    */
    virtual void OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

protected:

    FLOAT       m_fMargin;          // The margin value between elements.
    FLOAT       m_fChildWidth;      // The width of children.
    FLOAT       m_fChildHeight;     // The height of children.
};

END_NAMESPACE_VIEWS

#endif // _SDKGALLERY_H_
#endif // __cplusplus
