/*! 
* @file GridViewV.h
* 
* @brief This file defines the GridViewV layout.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author SunNing
* @date 2011/01/19
*/

#ifdef __cplusplus
#ifndef _GRIDVIEWV_H_
#define _GRIDVIEWV_H_

#include "ViewLayout.h"

class ScrollBar;

/*!
* @brief This class defines the GridViewV for Layout.
*/
class CLASS_DECLSPEC GridViewV : public ViewLayout
{
public:

    /*!
    * @brief The constructor function.
    */
    GridViewV();

    /*!
    * @brief The destructor function.
    */
    virtual ~GridViewV();

    /*!
    * @brief Set the layout scale.
    *
    * @param  [I/ ]FLOAT fScale            The layout scale.
    */
    void SetScale(IN FLOAT fScale);

    /*!
    * @brief Set the children's margin.
    *
    * @param  [I/ ]FLOAT fMarginX          The child margin in horizontal orientation.
    *         [I/ ]FLOAT fMarginY          The child margin in vertical orientation.
    */
    void SetChildMargin(IN FLOAT fMarginX, IN FLOAT fMarginY);

    /*!
    * @brief Set the second line's indent.
    *
    * @param [I/ ]FLOAT fIndent             The second line's indent.
    */
    void SetIndent(IN FLOAT fIndent);

    /*!
    * @brief Remove all children view
    *
    * @return True: remove success.
    *               remove failure
    */
    virtual BOOL RemoveAllChildren(BOOL isClearCache = TRUE);

    /*!
    * @brief Auto size the grid by children count.
    */
    void AutoSize(BOOL isAutoSize);

    /*!
    * @brief layout the grid again.
    */
    void SetOnlayout();

    void SetColumnAndRowCount(INT32 nColumnCount, INT32 nRowCount);

    void SetColumnAndRowCountInArea(INT32 nColumnCount, INT32 nRowCount, FLOAT fHeight);
    virtual BOOL RemoveChild(ViewElement *pChild);
    /*!
    * @brief Remove the child view that specified child index.
    *
    * @param index    [I/ ] the child view.
    *
    * @return True: remove success.
    *               remove failure.
    */
    virtual BOOL RemoveChildAt(UINT index);


	FLOAT GetScrollLenRatio();

	FLOAT GetScrollPosRatio();


	virtual void SetScrollBarY( ScrollBar* pScrollBar );
	virtual void SetScrollBarX( ScrollBar* pScrollBar );
	virtual	ScrollBar* GetScrollBarY();
	virtual ScrollBar* GetScrollBarX();
    virtual void CancelScroll();


   /*!
    * @brief Call this method when need to draw some stuffs in render target.
    */
    virtual void OnPaint();

	virtual void SetOffsetPos(FLOAT offset);
protected:

    /*!
    * @brief Judge arrangement types.
    *
    * @param  [I/O]FLOAT pLayoutInfo          The child's layout information.
    *         [I/ ]INT32 nIndex               The child's index number.
    *         [I/ ]INT32 colmunCount          The grid column count.
    *
    * @return TRUE if succeed handled, FALSE: if otherwise.
    */
    BOOL MeasureChildren(IN OUT LAYOUTINFO *pLayoutInfo, IN INT32 nIndex);

    /*!
    * @brief Mouse event.
    *
    * @param message        [I/ ] The mouse message.
    * @param wParam         [I/ ] Indicates whether various virtual keys are down.
    * @param lParam         [I/ ] The mouse position.
    *
    * @return TRUE if processed, FALSE: if otherwise.
    */
    BOOL OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

     /*!
    * @brief Called when the layout of view is changed.
    *
    * @param bChanged        [I/ ] Indicates whether is changed.
    * @param fLeft           [I/ ] The left value.
    * @param fTop            [I/ ] The top value.
    * @param fWidth          [I/ ] The width of view.
    * @param fHeight         [I/ ] The height of view.
    */
    virtual void OnLayout(BOOL bChanged, FLOAT fLeft, FLOAT fTop, FLOAT fWidth, FLOAT fHeight);
    /*!
    * @brief Calculate child width and height.
    */
    void CalculateChildSize();
    /*!
    * @brief When mouse moved, calculate the layout offset.
    */
    void CalculateLayoutOffset();
protected:

    FLOAT m_fOffsetY;           // The offset in vertical orientation.
    FLOAT m_fElementHeight;     // The element height.
    FLOAT m_fChildMarginY;      // The child margin in vertical orientation.
    FLOAT m_fChildMarginX;      // The child margin in horizontal orientation.
    BOOL  m_bIsMouseDown;       // To record if mouse is down.
    FLOAT m_fPointx;            // cursor x.
    FLOAT m_fPointy;            // cursor y.
    FLOAT m_fScale;             // The scale.
    FLOAT m_fElementWidth;      // The element width.
    FLOAT m_fOriginalIndent;    // The second line original indent.
    FLOAT m_usedAreaY;          // When dispatch child's layout, record the dispatched area.
    BOOL  m_bAutoSize;          // Auto size flag.
    FLOAT m_marginX;            // The child interval.
    FLOAT m_parentHeight;       // The parent height.
    INT32 m_nRowCount;
    FLOAT m_fHeight;
    INT32 m_nColumnCount;
    INT32 m_beginY;
	ScrollBar* m_pScrollBarY;
	ScrollBar* m_pScrollBarX;
private:
    BOOL  m_isExternalDraw;
    BOOL  m_cancelMove;
};

#endif //_GRIDVIEWV_H_
#endif //__cplusplus
