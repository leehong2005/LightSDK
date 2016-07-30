/*!
* @file ProcessBar.h 
* 
* @brief This file defines the class ProcessBar, it's a control for display process.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang
* @date 2011/01/04
*/

#ifdef  __cplusplus
#ifndef _PROGRESSBAR_H_
#define _PROGRESSBAR_H_

#include "ViewElement.h"
#include "D2DSolidColorBrush.h"

/*!
* @brief This file defines the class ProcessBar, it's a control for display process.
*/
class CLASS_DECLSPEC ProgressBar : public ViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    ProgressBar();

    /*!
    * @brief The destructor function.
    */
    virtual ~ProgressBar();

    /*!
    * @brief Set the layout information of the view.
    *
    * @param x              [I/ ] The x coordinate of left.
    * @param y              [I/ ] The y coordinate of left.
    * @param width          [I/ ] The width coordinate of left.
    * @param height         [I/ ] The height coordinate of left.
    */
    void SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height);

    /*!
    * @brief Call this method to set progressBar process.
    * 
    * @param value          [I/ ] The value to set to progressBar,From 0~100.
    */
    void SetProcess(IN FLOAT process);

    /*!
    * @brief Call this method to get progress's process.
    */
    FLOAT GetProcess();

    /*!
    * @brief Call this method to set process increase setp.
    * 
    * @param setp.          [I/ ] Increase step.
    */
    void SetStep(IN FLOAT step);

    /*!
    * @brief Call this method to set progressBar fill color.
    * 
    * @param color          [I/ ] Fill color.
    */
    void SetProcessColor(IN const D2D1_COLOR_F& color);

protected:

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

private:

    D2DSolidColorBrush *m_pD2DBrush;                // The background brush;
    FLOAT               m_fCurProgress;             // The progressBar's current process value.
    FLOAT               m_fPreProcess;              // The progressBar previous process value.
    D2D1_COLOR_F        m_processColor;             // The progressBar fill color.
};

#endif // _PROGRESSBAR_H_
#endif // __cplusplus
