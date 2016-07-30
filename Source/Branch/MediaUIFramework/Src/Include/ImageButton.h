/*!
* @file ImageButton.h
* 
* @brief This file defines the ImageButton class.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2010/12/31
*/

#ifdef __cplusplus
#ifndef _IMAGEBUTTON_H_
#define _IMAGEBUTTON_H_

#include "ImageView.h"
#include "D2DTextLayout.h"
#include "TextView.h"

/*!
* @brief The ImageButton class.
*/
class CLASS_DECLSPEC ImageButton : public ImageView, public TextView
{
public:

    /*!
    * @brief The constructor function.
    *
    * @param isShowDefBk    [I/ ] Indicates show default button background image or not.
    */
    ImageButton(BOOL isShowDefBk = FALSE);

    /*!
    * @brief The destructor function.
    */
    virtual ~ImageButton();

    /*!
    * @brief Set the focused image.
    *
    * @param lpFile     [I/ ] The path of the image.
    *
    * @return TRUE if success, otherwise return FALSE.
    */
    BOOL SetFocusImage(IN LPCWSTR lpFile, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Set the focused image.
    *
    * @param hBitmap    [I/ ] Handle to HBITMAP.
    *
    * @return TRUE if success, otherwise return FALSE.
    */
    BOOL SetFocusImage(HBITMAP hBitmap, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Set the focused image.
    *
    * @param uResId     [I/ ] Resource id.
    * @param hModule    [I/ ] Handle to HMODULE, default value is NULL.
    *
    * @return TRUE if success, otherwise return FALSE.
    */
    BOOL SetFocusImage(UINT uResId, HMODULE hModule = NULL, UINT32 uDestWidth = 0, UINT32 uDestHeight = 0);

    /*!
    * @brief Set the text color for focused state.
    *
    * @param color      [I/ ] The reference to D2D1_COLOR_F.
    */
    void SetFocusTextColor(IN const D2D1_COLOR_F& color);

    /*!
    * @brief Call this method to set ImageButton's text size on focus state.
    * 
    * @param size               [I/ ] Text size.
    */
    void SetFocusFontSize(IN FLOAT size);

    /*!
    * @brief Call this method to set ImageButton's text size on normal state.
    * 
    * @param size               [I/ ] Text size.
    */
    void SetNormalFontSize(IN FLOAT size);

    /*!
    * @brief Call this method to set text color.
    * 
    * @param color          [I/ ] The color of text display.
    */
    void SetTextColor(IN const D2D1_COLOR_F& color);

    /*!
    * @brief Set the layout information of the view.
    *
    * @param x          [I/ ] The x coordinate of left.
    * @param y          [I/ ] The y coordinate of left.
    * @param width      [I/ ] The width coordinate of left.
    * @param height     [I/ ] The height coordinate of left.
    */
    void SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height);

    /*!
    * @brief Clear the associated data with the view.
    */
    virtual void ClearAssocData();

protected:

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Paint focus image.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    *
    * @return TRUE if draw focus image, FALSE if not, when return FALSE, we should draw normal background.
    */
    BOOL OnPaintFocusImage(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Draw focus text.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    void OnPaintFocusText(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Paint normal image.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    void OnPaintNormalImage(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Paint normal text.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    void OnPaintNormalText(ID2D1RenderTarget *pRenderTarget);

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

    D2D1_COLOR_F             m_normalTextColor;     // The normal status text color.
    D2D1_COLOR_F             m_focusTextColor;      // The focused status text color.
    FLOAT                    m_focusTextSize;       // The focused status text size
    FLOAT                    m_normalTextSize;      // The normal status text size.
    D2DBitmap               *m_pFocusedD2DBitmap;   // The pointer which points to the object of D2DBitmap.
};


#endif // _IMAGEBUTTON_H_
#endif // __cplusplus

