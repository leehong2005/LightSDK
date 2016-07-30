/*!
* @file IComboBoxEventhandler.h
* 
* @brief This file defines the event for SdkComboBox.
* 
* Copyright (C) 2011, LZT Corporation.
* 
* @author Li Hong
* @date 2011/02/19
*/

#ifdef __cplusplus
#ifndef _ICOMBOBOXEVENTHANDLER_H_
#define _ICOMBOBOXEVENTHANDLER_H_

#include "SdkUICommon.h"

BEGIN_NAMESPACE_CALLBACK

/*!
* @brief IComboBoxEventHandler class.
*/
class IComboBoxEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~IComboBoxEventHandler() {};

    /*!
    * @brief Called when visibility is changed.
    *
    * @param pView  [I/ ] The event source.
    */
    virtual void OnSelectedChanged(SdkComboBox *pView) = 0;
};

END_NAMESPACE_CALLBACK

#endif // _ICOMBOBOXEVENTHANDLER_H_
#endif // __cplusplus
