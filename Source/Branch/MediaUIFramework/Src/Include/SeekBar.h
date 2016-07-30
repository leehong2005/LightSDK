/*!
* @file  SeekBar.h
* 
* @brief This file defines the header file of the seekBar.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Zhou_Yuan_Qi
* @date 2011/01/04
*/

#ifdef __cplusplus
#ifndef _SEEKBAR_H_
#define _SEEKBAR_H_

#include "D2DBrush.h"
#include "ViewElement.h"
#include "D2DRectUtility.h"
#include "D2DSolidColorBrush.h"
#include "ISeekBarChangeHandler.h"
#include "D2DLinearGradientBrush.h"

/*!
* @brief The Style of this View.
*/
typedef enum _SEEKBAR_VIEWSTYLE
{
    SEEKBAR_STYLE           = 0x000,        // Seek bar style.
    PROGRESSBAR_STYLE       = 0x001,        // Progress bar style.

} SEEKBAR_VIEWSTYLE;

/*!
* @brief The orientation of the SeekBar.
*/
typedef enum _SEEKBAR_ORIENT
{
    VERTICAL_ORIENTATION    = 0x001,
    HORIZONTAL_ORIENTATION  = 0x002,

} SEEKBAR_ORIENT;

/*!
* @brief The state of the SeekBar.
*/
typedef enum _SEEKBARCHAANGE_STATE
{
    SEEKBARCHANGE_BEGIN = 0x001,
    SEEKBARCHANGE       = 0x002,
    SEEKBARCHANGE_END   = 0x004,
    SEEKBARCHANGE_NONE  = 0x000,

} SEEKBARCHAANGE_STATE;
/*!
* @brief SeekBar is to show a progress can change by user.
*/
class CLASS_DECLSPEC SeekBar : public ViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    SeekBar();

    /*!
    * @brief The destructor function.
    */
    virtual ~SeekBar();

    /*!
    * @brief Call this method to set the current position of the seekBar.
    * 
    * @param fPos   [I/ ] The position you gave.
    */
    void SetCurPos(IN const FLOAT fPos);

    /*!
    * @brief Call this method to set the Progress color.
    *
    * @param color          [I/ ] The color set to the back.
    */
    void SetProgressColor(IN const ColorF &color);

    /*!
    * @brief Call this method to set the track color.
    *
    * @param color          [I/ ] The color set to the track.
    */
    void SetTrackColor(IN const ColorF &color);

    /*!
    * @brief Call this method to set the processed show or not
    *
    * @param isVisible      [I/ ] The color set to the Inner Circle.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    void SetProgressVisible(IN BOOL isVisible);

    /*!
    * @brief Call this method Set the layout information of the view.
    *
    * @param x              [I/ ] The x coordinate of left.
    * @param y              [I/ ] The y coordinate of left.
    * @param width          [I/ ] The width coordinate of left.
    * @param height         [I/ ] The height coordinate of left.
    */
    virtual void SetLayoutInfo(IN FLOAT x, IN FLOAT y, IN FLOAT width, IN FLOAT height);

    /*!
    * @brief perform the event ISeekBarChangeListener handle.
    *
    * @param progress           [I/ ] The current value of the track.
    */
    void OnProgressChange(IN FLOAT progress);
    
    /*!
    * @brief Set the view event handler.
    *
    * @param pSeekBarListener   [I/ ] The ISeekBarChangeListener instance.
    */
    void SetChangeHandler(IN ISeekBarChangeHandler *pSeekBarListener);

    /*!
    * @brief Set the style of this view to SeekBar or Progress.
    *
    * @param eStyle             [I/ ] The style view of this view.
    */
    void SetViewStyle(SEEKBAR_VIEWSTYLE eStyle);

    /*!
    * @brief Set the Orientation of this view 
    *  you have to use it after Call SetlayoutInfo
    *  method if you need a vertical orientation.
    * @param eStyle             [I/ ] The style view of this view.
    */
    void SetViewOrientation(SEEKBAR_ORIENT eOrient);

    /*!
    * @brief Set the step of the SeekBar.
    *
    * @param step               [I/ ] The step of this view.
    */
    void SetStep(IN FLOAT step);

    /*!
    * @brief Set the Image of the progress from resource.
    *
    * @param resLeftId           [I/ ] The id of left image.
    * @param resCenterId         [I/ ] The id of center image.
    * @param resRightId          [I/ ] The id of right image.
    */
    void SetProgressSrcImage(UINT resLeftId, UINT resCenterId, UINT resRightId);

    /*!
    * @brief Get the  State SeekBar.
    */
    SEEKBARCHAANGE_STATE GetSeekBarState();

protected:

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

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Mouse event.
    *
    * @param message        [I/ ] The pointer to file name.
    * @param wParam         [I/ ] The pointer to file name.
    * @param lParam         [I/ ] The pointer to file name.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

protected:

    /*!
    * @brief Draw a rectangle of this View.
    *
    * @param outRc                 [I/ ] The rectangle get from layout Information.
    * @param pD2DBrush             [I/ ] The pointer to D2DBrush.
    * @param pRenderTarget         [I/ ] The pointer to RenderTarget.
    */
    void OnDrawViewBk(IN const D2D1_RECT_F &outRc,
                      IN ID2D1Brush *pD2DBrush,
                      IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Draw a track in the Out rectangle.
    *
    * @param outRc                 [I/ ] The rectangle get from layout Information.
    * @param pD2DBrush             [I/ ] The pointer to D2DBrush.
    * @param pRenderTarget         [I/ ] The pointer to RenderTarget.
    */
    void OnDrawTrack(IN const D2D1_RECT_F &outRc,
                     IN ID2D1Brush *pD2DBrush,
                     IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Draw a active rectangle as progress.
    *
    * @param outRc                 [I/ ] The rectangle get from layout Information.
    * @param pD2DBrush             [I/ ] The pointer to D2DBrush.
    * @param pRenderTarget         [I/ ] The pointer to RenderTarget.
    */
    void OnDrawProgress(IN const D2D1_RECT_F &outRc, 
                        IN ID2D1Brush *pD2DBrush,
                        IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Draw a slider.
    *
    * @param outRc                 [I/ ] The rectangle get from layout Information.
    * @param pD2DBrush             [I/ ] The pointer to D2DBrush.
    * @param pRenderTarget         [I/ ] The pointer to RenderTarget.
    */
    void OnDrawSlider(IN const D2D1_RECT_F &outRc, 
                      IN ID2D1Brush *pD2DBrush,
                      IN ID2D1RenderTarget *pRenderTarget);
    /*!
    * @brief draw the track with bitmap.
    *
    * @param pD2DBitmap           [I/ ] The pointer to the bitmap
    * @param pRenderTarget        [I/ ] The pointer to RenderTarget.
    * @param outRc                [I/ ] The rectangle to draw the bitmap.
    */
    void OnDrawTrackBitmap(IN D2DBitmap *pD2DBitmap,
                           IN ID2D1RenderTarget *pRenderTarget,
                           IN const D2D1_RECT_F &outRc);

    /*!
    * @brief draw the background of the track for horizontal.
    *
    * @param pRenderTarget        [I/ ] The pointer to RenderTarget.
    */
    void OnDrawHorTrackBk(IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief draw the background of the progress.
    *
    * @param pRenderTarget        [I/ ] The pointer to RenderTarget.
    */
    void OnDrawProgressBk(IN const D2D1_RECT_F &outRc, IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Set the position of the progress depend on a value from user.
    */
    void SetProgressByCurPos();

    /*!
    * @brief Handle mouse move event.
    *
    * @param xPos                 [I/ ] The current coordinate of x.
    * @param maxSize              [I/ ] The max coordinate.
    * @param minSize              [I/ ] The min coordinate.
    * @param OutRc                [I/ ] The min coordinate.
    */
    void OnMouseDownHandler(IN FLOAT xPos,IN FLOAT yPos, IN FLOAT maxSize,
                            IN const D2D1_RECT_F &OutRc);

    /*!
    * @brief Handle mouse move event.
    *
    * @param xPos                 [I/ ] The pointer to file name.
    * @param maxSize              [I/ ] The max coordinate.
    * @param OutRc                [I/ ] The min coordinate.
    */
    void OnMouseMoveHandler(IN FLOAT xPos,IN FLOAT yPos, IN FLOAT maxSize,
                            IN const D2D1_RECT_F &OutRc);

    /*!
    * @brief get a rectangle.
    *
    * @param xPos                 [I/ ] The x coordinate
    * @param yPos                 [I/ ] The y coordinate.
    * @param OutRc                [I/ ] The rectangle get from the layout.
    */
    void GetTrackRect(IN FLOAT xPos, IN FLOAT yPos, IN D2D1_RECT_F OutRc);

    /*!
    * @brief get Point after Converted.
    *
    * @param outRc                [I/ ] The x coordinate
    * @param xPos                 [I/ ] The y coordinate.
    * @param yPos                 [I/ ] The rectangle get from the layout.
    */
    void GetConvertPoint(IN D2D1_RECT_F outRc, IN FLOAT &xPos,FLOAT &yPos);

    /*!
    * @brief Initialize x coordinate of centerPt.
    */
    void InitializeCenterPy();

    /*!
    * @brief Initialize y coordinate of centerPt.
    */
    void InitializeCenterPx();

    /*!
    * @brief Call this method calculate x coordinate when mouse down.
    *
    * @param OutRc                [I/ ] The rectangle get from the out.
    * @param xPos                 [I/ ] The x coordinate.
    * @param maxSize              [I/ ] A size to control the side of view.
    */
    void MouseDownXCal(IN FLOAT xPos, IN FLOAT maxSize, IN const D2D1_RECT_F &OutRc);

    /*!
    * @brief Call this method calculate y coordinate when mouse down.
    *
    * @param OutRc                [I/ ] The rectangle get from the out.
    * @param xPos                 [I/ ] The x coordinate.
    * @param maxSize              [I/ ] A size to control the side of view.
    */
    void MouseDownYCal(IN FLOAT yPos, IN FLOAT maxSize, IN const D2D1_RECT_F &OutRc);

    /*!
    * @brief Call this method calculate y coordinate when mouse move.
    *
    * @param OutRc                [I/ ] The rectangle get from the out.
    * @param yPos                 [I/ ] The y coordinate.
    * @param maxSize              [I/ ] A size to control the side of view.
    */
    void MouseMoveXCal(IN FLOAT xPos, IN FLOAT maxSize, IN const D2D1_RECT_F &OutRc);

    /*!
    * @brief Call this method calculate y coordinate when mouse move.
    *
    * @param OutRc                [I/ ] The rectangle get from the out.
    * @param yPos                 [I/ ] The y coordinate.
    * @param maxSize              [I/ ] A size to control the side of view.
    */
    void MouseMoveYCal(IN FLOAT yPos, IN FLOAT maxSize, IN const D2D1_RECT_F &OutRc);

    /*!
    * @brief Call this method calculate y coordinate when mouse move.
    *
    * @param outRc                [I/ ] The rectangle get from the out.
    * @param size                 [I/ ] A size to control the side of view.
    */
    void GetMaxSize(IN const D2D1_RECT_F &outRc, FLOAT &size);

protected:

    D2DBrush              *m_pSolidBrush;               // The brush use in this view
    ISeekBarChangeHandler *m_psChangeHandle;            // The event handle.
    D2DBitmap             *m_pSrcTrackLeft;             // The pointer to a bitmap.for track.
    D2DBitmap             *m_pSrcTrackCenter;           // The pointer to a bitmap.for track.
    D2DBitmap             *m_pSrcTrackRight;            // The pointer to a bitmap.for track.
    D2DBitmap             *m_pSrcProgressLeft;          // The Pointer to a bitmap for progress.
    D2DBitmap             *m_pSrcProgressRight;         // The Pointer to a bitmap for progress.
    D2DBitmap             *m_pSrcProgressCenter;        // The Pointer to a bitmap for progress.
    D2D1_COLOR_F          m_brushColor;                 // The color of the Brush.
    D2D1_COLOR_F          m_trackColor;                 // The color of the Brush.
    D2D1_COLOR_F          m_progressColor;              // The color of progress.
    D2D1_POINT_2F         m_trackCenterPt;              // The center of the track.
    D2D1_RECT_F           m_trackRc;                    // The rectangle of the track.
    SEEKBARCHAANGE_STATE  m_seekBarState;               // The state of the SeekBar.
    SEEKBAR_VIEWSTYLE     m_viewStyle;                  // The style of the view 
    SEEKBAR_ORIENT        m_viewOrient;                 // The orientation of the view.
    FLOAT                 m_sPointX;                    // The start Point X of the track.
    FLOAT                 m_sPointY;                    // The start Point Y of the track.
    FLOAT                 m_radiusX;                    // The radius of the ellipse.
    FLOAT                 m_radiusY;                    // The radius of the ellipse.
    FLOAT                 m_offsetX;                    // The offset X.
    FLOAT                 m_offsetY;                    // The offset Y.
    FLOAT                 m_topMargin;                  // The top margin.
    FLOAT                 m_curPos;                     // The current position of the slip.
    FLOAT                 m_fStep;                      // The setp of the SeekBar.
    BOOL                  m_isTouchOnSlider;            // The bool value to remember the touch 
    BOOL                  m_isTouchOnTrack;             // The bool value to remember the touch.
    BOOL                  m_isTouchDown;                // The bool value use when touchdown.
    BOOL                  m_isProgressVisible;          // The value whether to show progress bar.
};

#endif // _SEEKBAR_H_
#endif // _cplusplus_
