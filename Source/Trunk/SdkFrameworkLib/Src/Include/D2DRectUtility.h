/*!
* @file D2DRectUtility.h 
* 
* @brief The file defines the utility class D2DRectUtility to operate D2D rect.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/01/15
*/

#ifdef __cplusplus
#ifndef _D2DRECTUTILITY_H_
#define _D2DRECTUTILITY_H_

#include "SdkCommon.h"
#include "SdkCommonMacro.h"

BEGIN_NAMESPACE_D2D

class CLASS_DECLSPEC D2DRectUtility
{
public:

    /*!
    * @brief Indicates the rect is empty or not.
    *
    * @param srcRc          [I/ ] The source rectangle.
    *
    * @return TRUE if it is empty, otherwise FALSE.
    */
    static BOOL IsEmpty(IN const D2D1_RECT_F &srcRc);

    /*!
    * @brief Check the point is in rectangle or not.
    *
    * @param srcRc          [I/ ] The source rectangle.
    * @param pt             [I/ ] The source point.
    *
    * @return TRUE if point in rectangle, otherwise FALSE.
    */
    static BOOL PtInD2DRect(IN const D2D1_RECT_F &srcRc, IN const D2D1_POINT_2F &pt);

    /*!
    * @brief Increase or decrease the window and height of the specified rectangle.
    *
    * @param destRc         [I/O] The destination D2D rectangle.
    * @param dx             [I/ ] The offset on x axis.
    * @param dy             [I/ ] The offset on y axis.
    *
    * @remark positive values increase the width and height, and negative values decrease them.
    */
    static void InflateD2DRectF(IN OUT D2D1_RECT_F &destRc, FLOAT dx, FLOAT dy);

    /*!
    * @brief Increase or decrease the window and height of the specified rectangle.
    *
    * @param destRc         [I/O] The destination D2D rectangle.
    * @param dl             [I/ ] The offset on left.
    * @param dt             [I/ ] The offset on top.
    * @param dr             [I/ ] The offset on right.
    * @param db             [I/ ] The offset on bottom.
    *
    * @remark positive values increase the width and height, and negative values decrease them.
    */
    static void InflateD2DRectF(IN OUT D2D1_RECT_F &destRc, FLOAT dl, FLOAT dt, FLOAT dr, FLOAT db);

    /*!
    * @brief Offset the specified D2D rect with given offset on x axis and y axis.
    *
    * @param destRc         [I/O] The destination D2D rectangle.
    * @param dx             [I/ ] The offset on x axis.
    * @param dy             [I/ ] The offset on y axis.
    */
    static void OffsetD2DRectF(IN OUT D2D1_RECT_F &destRc, FLOAT dx, FLOAT dy);

    /*!
    * @brief Move the D2D rectangle to specified point.
    *
    * @param destRc         [I/O] The destination D2D rectangle.
    * @param x              [I/ ] The new x value.
    * @param y              [I/ ] The new y value.
    */
    static void MoveD2DRectTo(IN OUT D2D1_RECT_F &destRc, FLOAT x, FLOAT y);

    /*!
    * @brief Check specified two D2D rect whether is equal.
    *
    * @param rc1            [I/ ] The first source D2D rect.
    * @param rc2            [I/ ] The second source D2D rect.
    *
    * @return TRUE if the two rect is equal, otherwise return FALSE.
    */
    static BOOL EqualD2DRectF(IN const D2D1_RECT_F &rc1, IN const D2D_RECT_F &rc2);

    /*!
    * @brief Get the intersection of two source D2D rectangle.
    *
    * @param destRc          [ /O] The intersection destination rect.
    * @param srcRc1          [I/ ] The first source D2D rect.
    * @param srcRc2          [I/ ] The second source D2D rect.
    *
    * @return TRUE if the two rect is intersected, otherwise return FALSE.
    */
    static BOOL IntersectD2DRectF(OUT D2D1_RECT_F &destRc, IN const D2D1_RECT_F &srcRc1, IN const D2D_RECT_F &srcRc2);
};

END_NAMESPACE_D2D

#endif // _D2DRECTUTILITY_H_
#endif // __cplusplus
