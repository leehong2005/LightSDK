/*!
* @file ComboBoxEventhandler.h 
* 
* @brief This file defines the event for ComboBox.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang
* @date 2011/02/19
*/

#ifdef __cplusplus
#ifndef _COMBOBOXEVENTHANDLER_H_
#define _COMBOBOXEVENTHANDLER_H_

#include "CommonInclude.h"

class ComboBox;

/*!
* @brief ComboBoxEventhandler class.
*/
class CLASS_DECLSPEC ComboBoxEventHandler
{
public:

    /*!
    * @brief The destructor function.
    */
    virtual ~ComboBoxEventHandler() {};

    /*!
    * @brief Called when visibility is changed.
    *
    * @param pView  [I/ ] The event source.
    */
    virtual void OnSelectedChanged(ComboBox *pView);
};

#endif // _COMBOBOXEVENTHANDLER_H_
#endif // __cplusplus
