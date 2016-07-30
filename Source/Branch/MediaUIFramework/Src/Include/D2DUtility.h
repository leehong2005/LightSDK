/*!
* @file D2DUtility.h 
* 
* @brief This file defines the class D2DUtility, it provides some common static method.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang.
* @date 2011/03/23
*/

#ifdef __cplusplus
#ifndef _D2DUTILITY_H_
#define _D2DUTILITY_H_

#include "Common.h"

class CLASS_DECLSPEC D2DUtility
{
public:

    /*!
    * @brief Call this method to scale a rectangle's width and height based Algorithm.
    * 
    * @param uWidth         [I/O] input rectangle's width,and output computed width.
    * @param uHeight        [I/O] input rectangle's height,and output computed height.
    */
    static void CreateScalerRect(UINT& uWidth, UINT& uHeight);
};

#endif // _D2DUTILITY_H_
#endif // __cplusplus
