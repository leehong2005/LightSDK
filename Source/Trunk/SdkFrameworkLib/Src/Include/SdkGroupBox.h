/*!
* @file SdkGroupBox.h
* 
* @brief This file defines the SdkGroupBox class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#ifdef __cplusplus
#ifndef _SDKGROUPBOX_H_
#define _SDKGROUPBOX_H_

#include "SdkViewElement.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief A group box is a rectangle that surrounds a set of controls, such as check boxes
*        or radio buttons, with an application-defined text label in its upper left corner.
*/
class CLASS_DECLSPEC SdkGroupBox : public SdkViewElement
{
public:

    /*!
    * @brief The default constructor function.
    */
    SdkGroupBox();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkGroupBox();
};

END_NAMESPACE_VIEWS

#endif // _SDKGROUPBOX_H_
#endif // __cplusplus
