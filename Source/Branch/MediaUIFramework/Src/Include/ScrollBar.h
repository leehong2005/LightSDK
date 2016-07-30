/*!
* @file ScrollBar.h
* 
* @brief This file defines the header file of the scrollBar.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Guo Jianbin
* @date 2011/02/12
*/

#ifdef __cplusplus
#ifndef _SCROLLBAR_H_
#define _SCROLLBAR_H_

#include "D2DSolidColorBrush.h"
#include "ViewElement.h"
#include "D2DRectUtility.h"

/*!
* @brief The style of scrollBar, vertical or horizontal.
*/
typedef enum _SCROLLBAR_ORIENTATION
{
    SCROLLBAR_ORIENTATION_UNKONWN       = 0,       // Unknown orientation.
    SCROLLBAR_ORIENTATION_VERTICAL      = 1,       // The scrollBar is vertical.
    SCROLLBAR_ORIENTATION_HORIZONTAL    = 2,       // The scrollBar is horizontal.

} SCROLLBAR_ORIENTATION;

/*!
* @brief The style of scrollBar view, square corners or rounded.
*/
typedef enum _SCROLLBAR_VIEWSTYLE
{
    SCROLLBAR_VIEWSTYLE_SQUARE          = 1,        // The scrollBar view is square corners.
    SCROLLBAR_VIEWSTYLE_ROUNDED         = 2,        // The scrollBar view is rounded.

} SCROLLBAR_VIEWSTYLE;

/*!
* @brief The scrollBar class.
*/
class CLASS_DECLSPEC ScrollBar : public ViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    ScrollBar();

    /*!
    * @brief The constructor function.
    */
    virtual ~ScrollBar();

    /*!
    * @brief Call this method to set scrollBar style.
    * 
    * @param style            [I/ ] The scrollBar style.
    */
    virtual void SetOrientation(SCROLLBAR_ORIENTATION style);

    /*!
    * @brief Call this method to set scrollBarView style.
    * 
    * @param viewStyle        [I/ ] The scrollBarView style.
    */
    virtual void SetViewStyle(SCROLLBAR_VIEWSTYLE viewStyle);

    /*!
    * @brief Call this method to set thumb position.
    * 
    * @param fThumbPos        [I/ ] The thumb position.
    */
    virtual void SetThumbPos(FLOAT fThumbPos);

    /*!
    * @brief Call this method to set the thumb size ratio.
    * 
    * @param fSizeRatio       [I/ ] The thumb size ratio.
    *                               It size is from 0 to 1.
    */
    virtual void SetThumbSizeRatio(FLOAT fSizeRatio);

    /*!
    * @brief Call this method to set the thumb position ratio.
    * 
    * @param fPosRatio        [I/ ] The thumb position ratio.
    *                               It size is from 0 to 1.
    */
    virtual void SetThumbPosRatio(FLOAT fPosRatio);

    /*!
    * @brief Call this method to set scroll page size.
    * 
    * @param fScrollPage       [I/ ] The scroll page size.
    */
    virtual void SetScrollPage(FLOAT fScrollPage);

    /*!
    * @brief Call this method to get scrollBar style.
    */
    virtual SCROLLBAR_ORIENTATION GetScrollBarStyle();

    /*!
    * @param Call this method to get thumb position.
    */
    virtual FLOAT GetThumbPos();

    /*!
    * @brief Call this method to get
    */
    virtual FLOAT GetScrollPage();

    /*!
    * @brief Call this method to set thumb color.
    * 
    * @param color             [I/ ] The set color.
    */
    virtual void SetThumbColor(const ColorF &color);

    /*!
    * @brief Call this method to set track color.
    * 
    * @param color             [I/ ] The set color.
    */
    virtual void SetTrackColor(const ColorF &color);

    /*!
    * @brief Set the layout information of the view.
    *
    * @param x                 [I/ ] The x coordinate of left.
    * @param y                 [I/ ] The y coordinate of left.
    * @param width             [I/ ] The width coordinate of left.
    * @param height            [I/ ] The height coordinate of left.
    */
    virtual void SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height);

protected:

    /*!
    * @brief Call this method to calculation thumb's size.
    */
    virtual void CalcThumbSize();

    /*!
    * @brief Call this method to calculation thumb position.
    */
    virtual void CalcThumbPos();

    /*!
    * @brief Call this method to convert thumb position.
    * 
    * @param fThumbPos         [I/ ] The thumb position.
    */
    virtual FLOAT ConvertThumbPos(FLOAT fThumbPos);

    /*!
    * @brief Call this method to convert thumb position.
    * 
    * @param fThumbPosRatio    [I/ ] The thumb position ratio.
    */
    virtual FLOAT ConvertThumbPos2(FLOAT fThumbPosRatio);

    /*!
    * @brief Call this method to calculation rect when draw thumb.
    * 
    * @param absRect            [I/ ] The absolute rect.
    */
    virtual D2D1_RECT_F CalcThumbRect(const D2D1_RECT_F &absRc);

    /*!
    * @brief Call this method when need to draw some stuffs in render target.
    */
    virtual void OnPaint();

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    * 
    * @param pRenderTarget     [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Draw a track of scroll bar view.
    * 
    * @param pRenderTarget     [I/ ] The pointer to RenderTarget.
    */
    virtual void OnDrawTrack(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Draw a thumb of scroll bar view.
    * 
    * @param pRenderTarget     [I/ ] The pointer to RenderTarget.
    */
    virtual void OnDrawThumb(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Call this method to draw thumb and track rectangle or round corner rectangle.
    *
    * @param pRenderTarget      [I/ ] The pointer to RenderTarget.
    * @param destRc             [I/ ] The rectangle rectangle.
    * @param pBrush             [I/ ] Brush to fill rectangle.
    */
    virtual void OnDrawRect(ID2D1RenderTarget *pRenderTarget, const D2D1_RECT_F &destRc, ID2D1Brush *pBrush);

	/*!
    * @brief Called when the layout of view is changed.
    *
    * @param fChanged       [I/ ] Indicates whether is changed.
    * @param left           [I/ ] The left value.
    * @param top            [I/ ] The top value.
    * @param width          [I/ ] The width of view.
    * @param height         [I/ ] The height of view.
    */
    virtual void OnLayout(BOOL fChanged, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

private:

    FLOAT                      m_thumbPos;              // The thumb position.
    FLOAT                      m_scrollPage;            // The scroll page size.
    D2D1_SIZE_F                m_thumbSize;             // The thumb size.
    D2DSolidColorBrush        *m_pTrackBrush;           // The brush use in track.
    D2DSolidColorBrush        *m_pThumbBrush;           // The brush use in thumb.
    SCROLLBAR_ORIENTATION      m_scrollOrientation;     // The scrollBar style.
    SCROLLBAR_VIEWSTYLE        m_scrollViewStyle;       // The scrollBarView style.
    D2D1_COLOR_F               m_thumbColor;            // The color of the thumb.
    D2D1_COLOR_F               m_trackColor;            // The track color.
    FLOAT                      m_thumbRatio;            // The thumb ratio.
    FLOAT                      m_thumbPosRatio;       // The convert thumb position.
};

#endif // _SCROLLBAR_H_
#endif // __cplusplus
