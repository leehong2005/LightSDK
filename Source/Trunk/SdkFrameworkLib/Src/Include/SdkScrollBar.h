/*!
* @file SdkScrollBar.h
* 
* @brief This file defines the header file of the scrollBar.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/12
*/

#ifdef __cplusplus
#ifndef _SDKSCROLLBAR_H_
#define _SDKSCROLLBAR_H_

#include "SdkViewElement.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The scrollBar class.
*/
class CLASS_DECLSPEC SdkScrollBar : public SdkViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkScrollBar();

    /*!
    * @brief The constructor function.
    */
    virtual ~SdkScrollBar();
};

END_NAMESPACE_VIEWS

#endif // _SDKSCROLLBAR_H_
#endif // __cplusplus
