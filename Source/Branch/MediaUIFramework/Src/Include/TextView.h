/*!
* @file TextView.h 
* 
* @brief This file defines the class TextView, it's a control for display text.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Liu Weibang
* @date 2011/01/04
*/

#ifdef __cplusplus
#ifndef _TEXTVIEW_H_
#define _TEXTVIEW_H_

#include "D2DTextLayout.h"
#include "ViewElement.h"

/*!
* @brief The text dock direction.
*/
typedef enum _TEXT_DOCK_DIRECTION
{
    TEXT_DOCK_DIRECTION_DEFAULT = 0x0000,       // The default, no docking.
    TEXT_DOCK_DIRECTION_LEFT    = 0x0001,       // Dock at left
    TEXT_DOCK_DIRECTION_TOP     = 0x0002,       // Dock at top
    TEXT_DOCK_DIRECTION_RIGHT   = 0x0003,       // Dock at right
    TEXT_DOCK_DIRECTION_BOTTOM  = 0x0004,       // Dock at bottom

} TEXT_DOCK_DIRECTION;

/*!
* @brief TextView class.
*/
class CLASS_DECLSPEC TextView : public virtual ViewElement
{
public:

    /*!
    * @brief The constructor function.
    */
    TextView();

    /*!
    * @brief The destructor function.
    */
    virtual ~TextView();

    /*!
    * @brief Set the flag to make view size fit the length of text.
    *
    * @param isWrapContent  [I/ ] TRUE if wrap content, otherwise FALSE.
    */
    virtual void SetWrapContent(BOOL isWrapContent = FALSE);

    /*!
    * @brief Get the measured text width.
    *
    * @param return width of measure text width.
    */
    virtual FLOAT GetMeasureWidth();

    /*!
    * @brief Get the measured text height.
    *
    * @param return width of measure text height.
    */
    virtual FLOAT GetMeasureHeight();

    /*!
    * @brief Call this method to clip the text that overflows the text bounds.
    *
    * @param isClipText     [I/ ] TRUE if clip, otherwise FALSE.
    */
    virtual void SetClipOverflowText(BOOL isClipText);

    /*!
    * @brief Indicate whether is wrap content.
    *
    * @return TRUE if wrap content, otherwise FALSE.
    */
    virtual BOOL GetWrapContent();

    /*!  
    * @brief Set the text dock direction. This function will make
    *        SetTextParagraphAlign and SetTextAlign function invalidate.
    *
    * @param dockDir        [I/ ] One of TEXT_DOCK_DIRECTION enumeration.
    */
    virtual void SetTextDockDirection(TEXT_DOCK_DIRECTION dockDir = TEXT_DOCK_DIRECTION_DEFAULT);

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
    * @brief Call this method to set display text information.
    * 
    * @param *string        [I/ ] Display text. 
    */
    virtual void SetText(IN LPCWSTR lpText);

    /*!
    * @brief Call this method to set display text information.
    * 
    * @param resId      [I/ ] The id of text.
    * @param hModule    [I/ ] The HINSTANCE, maybe NULL.
    */
    virtual void SetText(UINT32 resId, HMODULE hModule = NULL);

    /*!
    * @brief Get the text of this view.
    *
    * @param lpszText   [I/ ] The buffer to contains text.
    * @param uSize      [I/ ] The size of the buffer pointed by lpszText.
    */
    virtual void GetText(IN LPWSTR lpszText, UINT32 uSize);

    /*!
    * @brief Get the length of the text string.
    *
    * @return The value of text length.
    */
    virtual UINT32 GetTextLength();

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
    virtual void SetTextColor(IN const D2D1_COLOR_F& color);

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
    *                             Such as DWRITE_TEXT_ALIGNMENT_LEADING(LEFT), 
    *                             DWRITE_TEXT_ALIGNMENT_CENTER(CENTER), 
    *                             and DWRITE_TEXT_ALIGNMENT_TRAILING(RIGHT).
    */
    virtual void SetTextAlign(IN DWRITE_TEXT_ALIGNMENT textAlign);

    /*!
    * @brief Call this method to set text align style.
    * 
    * @param textParagraph  [I/ ] The text align style.
    */
    virtual void SetTextParagraphAlign(DWRITE_PARAGRAPH_ALIGNMENT textParagraph);

    /*!
    * @brief Call this method to set text can or not wrap.
    * 
    * @param textWrap       [I/ ] The text wrap style.
    */
    virtual void SetTextWrap(IN DWRITE_WORD_WRAPPING textWrap);

    /*!
    * @brief Clear the associated data with the view.
    */
    virtual void ClearAssocData();

protected:

    /*!
    * @brief Set the information of D2DTextLayout.
    */
    virtual void SetD2DTextLayoutInfo();

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

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

    BOOL                    m_isClipOverflowText;       // Clip the overflow text.
    BOOL                    m_isWrapContent;            // The wrap content flag.
    BOOL                    m_isRequestLayout;          // Should request layout.
    D2D1_COLOR_F            m_textColor;                // The normal status text color.
    TEXT_DOCK_DIRECTION     m_textDockDir;              // The text dock.
    D2DTextLayout          *m_pD2DTextLayout;           // The D2DTextLayout.
};


#endif // _TEXTVIEW_H_
#endif // __cplusplus
