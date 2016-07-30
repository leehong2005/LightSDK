/*!
* @file D2DUtility.cpp 
* 
* @brief This file defines the class D2DUtility, it provides some common static method.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang.
* @date 2011/03/23
*/

#include "D2DUtility.h"

#define MAX_SIZE    1600

void D2DUtility::CreateScalerRect(UINT& uWidth, UINT& uHeight)
{
    if (uWidth > MAX_SIZE || uHeight > MAX_SIZE)
    {
        if (uWidth >= uHeight)
        {
            FLOAT ratio = (FLOAT)uHeight / uWidth;
            uWidth = MAX_SIZE;
            uHeight = (UINT)(MAX_SIZE * ratio);
        }
        else
        {
            FLOAT ratio = (FLOAT)uWidth / uHeight;
            uHeight = MAX_SIZE;
            uWidth = (UINT)(MAX_SIZE * ratio);
        }
    }
}
