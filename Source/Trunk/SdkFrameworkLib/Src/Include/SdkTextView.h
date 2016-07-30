/*!
* @file SdkTextView.h 
* 
* @brief This file defines the class SdkTextView, it's a control for display text.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/24
*/

#ifdef __cplusplus
#ifndef _SDKTEXTVIEW_H_
#define _SDKTEXTVIEW_H_

#include "SdkViewElement.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief SdkTextView class.
*/
class CLASS_DECLSPEC SdkTextView : public SdkViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkTextView();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkTextView();

    /*!
    * @brief Set the flag to make view size fit the length of text.
    *
    * @param isWrapContent  [I/ ] TRUE if wrap content, otherwise FALSE.
    */
    virtual void SetWrapContent(BOOL isWrapContent);

    /*!
    * @brief Call this method to clip the text that overflows the text bounds.
    *
    * @param isClipText     [I/ ] TRUE if clip, otherwise FALSE.
    */
    virtual void SetClipText(BOOL isClipText);

    /*!  
    * @brief Set the text dock direction. This function will make
    *        SetTextParagraphAlign and SetTextAlign function invalidate.
    *
    * @param dockDir    [I/ ] One of TEXT_DOCK_DIRECTION enumeration.
    */
    virtual void SetTextDockDir(TEXT_DOCK_DIRECTION dockDir);

    /*!
    * @brief Call this method to set display text information.
    * 
    * @param *string        [I/ ] Display text. 
    */
    virtual void SetText(IN LPCTSTR lpText);

    /*!
    * @brief Call this method to set display text information.
    * 
    * @param resId      [I/ ] The id of text.
    * @param hModule    [I/ ] The HINSTANCE, maybe NULL.
    */
    virtual void SetText(UINT32 resId, HMODULE hModule = NULL);

    /*!
    * @brief Call this method to set text size.
    * 
    * @param size           [I/ ] Text size.
    */
    virtual void SetTextSize(IN FLOAT size = 18.0f);

    /*!
    * @brief Call this method to set text color.
    * 
    * @param color          [I/ ] The color of text display.
    */
    virtual void SetTextColor(IN const ColorF& color);

    /*!
    * @brief Call this method to set text font familyName.
    * 
    * @param familyName.    [I/ ] The font familyName.
    */
    virtual void SetFontFamilyName(IN LPCWSTR familyName);

    /*!
    * @brief Call this method to set text font style.
    * 
    * @param fontStyle.     [I/ ] The font style.
    */
    virtual void SetFontStyle(IN DWRITE_FONT_STYLE fontStyle);

    /*!
    * @brief Call this method to set text font stretch.
    * 
    * @param fontStretch.   [I/ ] The font stretch status.
    *                             Such as DWRITE_FONT_STRETCH_CONDENSED,
    *                                     DWRITE_FONT_STRETCH_NORMAL,
    *                                     DWRITE_FONT_STRETCH_EXPANDED.
    */
    virtual void SetFontStretch(IN DWRITE_FONT_STRETCH fontStretch);

    /*!
    * @brief Call this method to set font weight.
    * 
    * @param fontWeight.    [I/ ] The font weight.
    *                             Such as DWRITE_FONT_WEIGHT_THIN,
    *                                     DWRITE_FONT_WEIGHT_BOLD,
    *                                     DWRITE_FONT_WEIGHT_HEAVY,
    *                                     DWRITE_FONT_WEIGHT_NORMAL.
    */
    virtual void SetFontWeight(IN DWRITE_FONT_WEIGHT fontWeight);

    /*!
    * @brief Call this method to set ImageButton's text align style.
    * 
    * @param textAlign      [I/ ] The text align style.
    *                             Such as DWRITE_TEXT_ALIGNMENT_LEADING(Left),
    *                             DWRITE_TEXT_ALIGNMENT_CENTER(Center),
    *                             and DWRITE_TEXT_ALIGNMENT_TRAILING(Right)
    */
    virtual void SetTextAlign(IN TEXT_ALIGNMENT textAlign);

    /*!
    * @brief Call this method to set text align style.
    * 
    * @param textParagraph  [I/ ] The text align style.
    *                             DWRITE_PARAGRAPH_ALIGNMENT_NEAR(Top),
    *                             DWRITE_PARAGRAPH_ALIGNMENT_CENTER(Center),
    *                             DWRITE_PARAGRAPH_ALIGNMENT_FAR(Bottom)
    */
    virtual void SetTextParagraphAlign(TEXT_PARAGRAPH_ALIGNMENT textParagraph);

    /*!
    * @brief Call this method to set text can or not wrap.
    * 
    * @param textWrap       [I/ ] The text wrap style.
    */
    virtual void SetTextWrap(IN DWRITE_WORD_WRAPPING textWrap);

    /*!
    * @brief Set the layout information of the view.
    *
    * @param x          [I/ ] The x coordinate of left.
    * @param y          [I/ ] The y coordinate of left.
    * @param width      [I/ ] The width coordinate of left.
    * @param height     [I/ ] The height coordinate of left.
    */
    virtual void SetLayoutInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height);

    /*!
    * @brief Get the text of this view.
    *
    * @param lpszText   [I/ ] The buffer to contains text.
    * @param uSize      [I/ ] The size of the buffer pointed by lpszText.
    */
    virtual void GetText(IN LPTSTR lpszText, UINT32 uSize);

    /*!
    * @brief Get the length of the text string.
    *
    * @return The value of text length.
    */
    virtual UINT32 GetTextLength();

    /*!
    * @brief Indicate whether is wrap content.
    *
    * @return TRUE if wrap content, otherwise FALSE.
    */
    virtual BOOL GetWrapContent();

    /*!
    * @brief Get the measured text size.
    *
    * @param return The size of text, width and height.
    */
    virtual SIZE GetMeasureSize();

    /*!
    * @brief Get the text metrics information.
    */
    virtual TEXT_METRICS GetTextMetrics();

protected:

    /*!
    * @brief Set the start offset point of text. This function is useful when user want to
    *        draw the text which has an offset from the beginning (most left).
    *
    * @param x      [I/ ] The x of the point.
    * @param y      [I/ ] The y of the point.
    */
    void SetTextStartPoint(FLOAT x, FLOAT y);

    /*!
    * @brief Set the information of D2DTextLayout. In this function, set the start
    *        point of the text, text color, maximum and minimum width, etc.
    */
    void CalcTextLayoutInfo();

    /*!
    * @brief Called when the properties of text layout are changed, for example, user calls
    *        APIs of this class to change text's alignment, text size, font ,etc, this
    *        function will be called, derived class can override this method to get notification
    *        from this class.
    */
    virtual void OnTextLayoutChanged();

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Call this method to draw text.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawText(ID2D1RenderTarget *pRenderTarget);

private:

    /*!
    * @brief The internal data of text view.
    */
    struct _TEXTVIEW_INTERNALDATA;

    _TEXTVIEW_INTERNALDATA  *m_pTextViewData;   // The text view data.
};

END_NAMESPACE_VIEWS

#endif // _SDKTEXTVIEW_H_
#endif // __cplusplus
