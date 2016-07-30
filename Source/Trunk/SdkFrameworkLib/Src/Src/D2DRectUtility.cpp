/*!
* @file D2DRectUtility.cpp
* 
* @brief The file defines the utility class D2DRectUtility to operate D2D rect.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/15
*/

#include "stdafx.h"
#include "D2DRectUtility.h"

USING_NAMESPACE_D2D

//////////////////////////////////////////////////////////////////////////

BOOL D2DRectUtility::IsEmpty(IN const D2D1_RECT_F &srcRc)
{
    RECT rc = { (LONG)srcRc.left, (LONG)srcRc.top, (LONG)srcRc.right, (LONG)srcRc.bottom };
    return IsRectEmpty(&rc);
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DRectUtility::PtInD2DRect(IN const D2D1_RECT_F &srcRc, IN const D2D1_POINT_2F &pt)
{
    if ( pt.x > srcRc.left &&
         pt.x < srcRc.right &&
         pt.y > srcRc.top &&
         pt.y < srcRc.bottom )
    {
        return TRUE;
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void D2DRectUtility::InflateD2DRectF(IN OUT D2D1_RECT_F &destRc, FLOAT dx, FLOAT dy)
{
    destRc.left     -= dx;
    destRc.top      -= dy;
    destRc.right    += dx;
    destRc.bottom   += dy;
}

//////////////////////////////////////////////////////////////////////////

void D2DRectUtility::InflateD2DRectF(IN OUT D2D1_RECT_F &destRc, FLOAT dl, FLOAT dt, FLOAT dr, FLOAT db)
{
    destRc.left     -= dl;
    destRc.top      -= dt;
    destRc.right    += dr;
    destRc.bottom   += db;
}

//////////////////////////////////////////////////////////////////////////

void D2DRectUtility::OffsetD2DRectF(IN OUT D2D1_RECT_F &destRc, FLOAT dx, FLOAT dy)
{
    destRc.left     += dx;
    destRc.top      += dy;
    destRc.right    += dx;
    destRc.bottom   += dy;
}

//////////////////////////////////////////////////////////////////////////

void D2DRectUtility::MoveD2DRectTo(IN OUT D2D1_RECT_F &destRc, FLOAT x, FLOAT y)
{
    FLOAT width  = destRc.right - destRc.left;
    FLOAT height = destRc.bottom - destRc.top;

    destRc.left     = x;
    destRc.top      = y;
    destRc.right    = destRc.left + width;
    destRc.bottom   = destRc.top + height;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DRectUtility::EqualD2DRectF(IN const D2D1_RECT_F &rc1, IN const D2D_RECT_F &rc2)
{
    BOOL isEqual = (
        ((LONG)rc1.left == (LONG)rc2.left) &&
        ((LONG)rc1.top == (LONG)rc2.top) &&
        ((LONG)rc1.right == (LONG)rc2.right) &&
        ((LONG)rc1.bottom == (LONG)rc2.bottom)
        );

    return isEqual;
}

//////////////////////////////////////////////////////////////////////////

BOOL D2DRectUtility::IntersectD2DRectF(OUT D2D1_RECT_F &destRc, IN const D2D1_RECT_F &srcRc1, IN const D2D_RECT_F &srcRc2)
{
    BOOL isIntersected = FALSE;

    RECT gdiDestRc = { 0 };
    RECT gdiSrcRc1 = { (LONG)srcRc1.left, (LONG)srcRc1.top, (LONG)srcRc1.right, (LONG)srcRc1.bottom };
    RECT gdiSrcRc2 = { (LONG)srcRc2.left, (LONG)srcRc2.top, (LONG)srcRc2.right, (LONG)srcRc2.bottom };

    isIntersected = ::IntersectRect(&gdiDestRc, &gdiSrcRc1, &gdiSrcRc2);
    destRc.left     = (FLOAT)gdiDestRc.left;
    destRc.top      = (FLOAT)gdiDestRc.top;
    destRc.right    = (FLOAT)gdiDestRc.right;
    destRc.bottom   = (FLOAT)gdiDestRc.bottom;

    return isIntersected;
}