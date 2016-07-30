/*!
* @file LinearLayout.h 
* 
* @brief This file defines the linear layout 
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Sun Ning, Cao Chen, Liu Qiao
* @date 2011/01/06
*/

#ifdef __cplusplus
#ifndef _LINEARLAYOUT_H_
#define _LINEARLAYOUT_H_

#include "CommonInclude.h"
#include "ViewLayout.h"

/*!
* @brief The orientation of view element.
*/
typedef enum _ORIENTATION
{
    ORIENTATION_HORIZONTAL          = 0,                // This type is horizontal layout.
    ORIENTATION_VERTICAL            = 1,                // This type is vertical layout.

} ORIENTATION;

/*!
* @brief The layout_style of view element.
*/
typedef enum _LAYOUT_STYLE
{
    LAYOUT_STYLE_CENTER              = 1,               // This type is the linear-layout center.
    LAYOUT_STYLE_NOTCENTER           = 2,               // This type is the linear-layout not-center.
    LAYOUT_STYLE_FORWARD             = 4,               // This type is the linear-layout forward.
    LAYOUT_STYLE_BACKWORD            = 8,               // This type is the linear-layout backward.

} LAYOUT_STYLE;

/*!
* @brief The padding of view element.
*/
typedef struct _PADDING
{
    FLOAT paddingLeft;                                  // The left padding.
    FLOAT paddingTop;                                   // The top padding.
    FLOAT paddingRight;                                 // The right padding.
    FLOAT paddingBottom;                                // The bottom padding.

} PADDING, *LPPADDING;

/*!
* @brief This class defines the LinearLayout for Layout.
*/
class CLASS_DECLSPEC LinearLayout: public ViewLayout
{

public:

    /*!
    * @brief The constructor function.
    */
    LinearLayout();

    /*!
    * @brief The destructor function.
    */
    virtual ~LinearLayout();

    /*!
    * @brief Set the orientation property of the linear layout.
    *
    * @param orientation     [I/ ] orientation property [ORIENTATION_HORIZONTAL, ORIENTATION_VERTICAL].
    */
    virtual void SetOrientation(ORIENTATION orientation);

    /*!
    * @brief Set the Style property of the linear layout.
    *
    * @param nStyle          [I/ ] Style property [LAYOUT_STYLE_CENTER, LAYOUT_STYLE_NOTCENTER, 
    *                                             LAYOUT_STYLE_FORWARD, LAYOUT_STYLE_BACKWORD].
    */
    virtual void SetStyle(INT32 nStyle);

    /*!
    * @brief Set the Padding of the linear layout.
    *
    * @param fLeft                  [I/ ] The left padding value.
    * @param fTop                   [I/ ] The top padding value.
    * @param fRight                 [I/ ] The right padding value.
    * @param fBottom                [I/ ] The bottom padding value.
    */
    virtual void SetPadding(FLOAT fLeft, FLOAT fTop, FLOAT fRight,FLOAT fBottom);

    /*!
    * @brief Set the Padding of the linear layout.
    *
    * @param paddingValue          [I/ ] All padding's value.
    */
    void SetPadding(FLOAT paddingValue);

    /*!
    * @brief paint all controls.
    */
    virtual void OnPaint();

protected:

    /*!
    * @brief Called when the layout of view is changed.
    *
    * @param fChanged        [I/ ] Indicates whether is changed.
    * @param fLeft           [I/ ] The left value.
    * @param fTop            [I/ ] The top value.
    * @param fWidth          [I/ ] The width of view.
    * @param fHeight         [I/ ] The height of view.
    */
    virtual void OnLayout(BOOL bChanged, FLOAT fLeft, FLOAT fTop, FLOAT fWidth, FLOAT fHeight);

    /*!
    * @brief Setting vertical layout.
    *
    * @param pLayoutInfo                   [I/O] The child's layout information.
    *
    * @return TRUE: if succeed, FALSE: if otherwise.
    */
    virtual BOOL MeasureVertical(IN OUT LAYOUTINFO *pLayoutInfo);

    /*!
    * @brief Setting horizontal layout.
    *
    * @param pLayoutInfo                    [I/O] The child's layout information.
    *
    * @return TRUE: if succeed process, FALSE: if otherwise.
    */
    virtual BOOL MeasureHorizontal(IN OUT LAYOUTINFO *pLayoutInfo);

    /*!
    * @brief Judge arrangement types.
    *
    * @param pLayoutInfo                    [I/O] The child's layout information.
    *
    * @return TRUE if succeed handled, FALSE: if otherwise.
    */
    virtual BOOL MeasureChildren(LAYOUTINFO *pLayoutInfo);

protected:

    PADDING             m_Padding;               // Padding property.
    FLOAT               m_fOffsetX;              // The offset in horizontal orientation.
    FLOAT               m_fOffsetY;              // The offset in vertical orientation.
    ORIENTATION         m_nOrientation;          // Orientation property.
    INT32               m_nStyle;                // LinearLayout style.
    FLOAT               m_fTempParentWidth;      // Temp parent width
    FLOAT               m_fTempParentHeight;     // Temp parent height
};

#endif // #ifndef _LINEARLAYOUT_H_
#endif // #ifdef __cplusplus
