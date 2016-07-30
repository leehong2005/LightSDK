/*!
* @file SdkSeekBar.h
* 
* @brief This file defines the header file of the SdkSeekBar.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/07/04
*/

#ifdef __cplusplus
#ifndef _SDKSEEKBAR_H_
#define _SDKSEEKBAR_H_

#include "SdkProgressBar.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief SdkSeekBar is to show a progress can change by user.
*/
class CLASS_DECLSPEC SdkSeekBar : public SdkProgressBar
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkSeekBar();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkSeekBar();

    /*!
    * @brief Set the slide range.
    *
    * @param fMin       [I/ ] The minimum value of range.
    * @param fMax       [I/ ] The maximum value of range.
    */
    void SetRange(FLOAT fMin, FLOAT fMax);

    /*!
    * @brief Get the slide range of the seek bar.
    *
    * @param pMin       [ /O] The pointer to FLOAT to receive minimum range value.
    * @param pMax       [ /O] The pointer to FLOAT to receive maximum range value.
    */
    void GetRange(FLOAT *pMin, FLOAT *pMax) const;

protected:

    /*!
    * @brief Get the size of the progress thumb
    *
    * @return The SIZE structure.
    */
    virtual SIZE GetThumbSize() const;

    /*!
    * @brief Set the thumb position.
    *
    * @param xPos       [I/ ] The X-coordinate value.
    * @param yPos       [I/ ] The Y-coordinate value.
    */
    virtual void SetThumbPos(FLOAT xPos, FLOAT yPos);

    /*!
    * @brief Check the specified point whether locates in the rectangle of thumb.
    *
    * @param xPos       [I/ ] The x-coordinate value.
    * @param yPos       [I/ ] The y-coordinate value.
    *
    * @return TRUE if locates in, FALSE otherwise.
    */
    virtual BOOL IsPtInThumbRect(FLOAT xPos, FLOAT yPos);

    /*!
    * @brief Calculate the thumb rectangle.
    *
    * @return D2D1_RECT_F.
    */
    virtual D2D1_RECT_F CalcThumbRect();

    /*!
    * @brief Called when get the absolute rectangle of progress bar track.
    */
    virtual D2D1_RECT_F OnCalcTrackRect();

    /*!
    * @brief Called when get the width of progress bar progress.
    *
    * @return The width of the progress.
    */
    virtual FLOAT OnGetProgressWidth();

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
    * @param message        [I/ ] message.
    * @param wParam         [I/ ] parameter.
    * @param lParam         [I/ ] parameter.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

    /*!
    * @brief Keyboard event.
    *
    * @param message        [I/ ] message.
    * @param wParam         [I/ ] parameter.
    * @param lParam         [I/ ] parameter.
    *
    * @return TRUE if processed, otherwise return FALSE.
    */
    virtual BOOL OnKeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam);

protected:

    /*!
    * @brief The internal data of seek bar.
    */
    struct _SEEKBAR_INTERNALDATA;

    _SEEKBAR_INTERNALDATA   *m_pSeekBarData;        // The seek bar data.
};

END_NAMESPACE_VIEWS

#endif // _SDKSEEKBAR_H_
#endif // _cplusplus_
