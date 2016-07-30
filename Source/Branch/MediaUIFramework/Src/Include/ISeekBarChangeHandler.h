/*!
* @file ISeekBarChangeHandler.h 
* 
* @brief This file defines events interfaces for SeekBar class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Zhou_Yuan_Qi
* @date 2011/01/10
*/

#ifdef __cplusplus
#ifndef _ISEEKBARCHANGELISTENER_H_
#define _ISEEKBARCHANGELISTENER_H_

#include "SeekBar.h"

/*!
* @brief The ISeekBarChangeHandler abstract class,
* defines common behavior of seekbar_ view event.
*/
class SeekBar;

class CLASS_DECLSPEC ISeekBarChangeHandler
{
public: 

    /*!
    * @brief The destructor function.
    */
    virtual ~ISeekBarChangeHandler(){};

    /*!
    * @brief Notification that the progress level has changed.
    *
    * @param pSeekBar       [I/ ] The SeekBar whose progress has changed.
    * @param progress       [I/ ] The current progress level.
    */
    virtual void OnProgressChanged(SeekBar *pSeekBar,FLOAT progress) = 0;
};

#endif // _ISEEKBARCHANGELISTENER_H_
#endif //__cplusplus