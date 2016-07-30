/*!
* @file D2DTextLayout.h 
* 
* @brief This file defines the class D2DTextLayout, is used to operate text.
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/01/04
*/

#ifdef __cplusplus
#ifndef _D2DTEXTLAYOUT_H_
#define _D2DTEXTLAYOUT_H_

#include "CommonInclude.h"
#include "D2DDevice.h"
#include "D2DResource.h"
#include "D2DSolidColorBrush.h"
#include "ID2DDeviceStateChange.h"

/*!
* @brief This class implements the DWriteTextLayout.
*/
class CLASS_DECLSPEC D2DTextLayout : public D2DResource, 
                                     public ID2DDeviceStateChange
{
public:

    /*!
    * @brief The constructor function.
    */
    D2DTextLayout();

    /*!
    * @brief The destructor function.
    */
    virtual ~D2DTextLayout();

    /*!
    * @brief Initialize current object, before using, you should call this method.
    */
    void Initialize();

    /*!
    * @brief Indicates an object of D2DBitmap whether has been initialized.
    *
    * @return TRUE if initialized, otherwise is FALSE.
    */
    BOOL HasInitialized();

    /*!
    * @brief Set D2DDevice pointer.
    *
    * @param pD2DDevice     [I/ ] The pointer to D2DDevice object.
    */
    void SetD2DDevice(IN D2DDevice *pD2DDevice);

    /*!
    * @brief Set string to this object to draw.
    * 
    * @param lpText             [I/ ] The display text.
    */
    void SetText(IN LPCWSTR lpText);

    /*!
    * @brief Get the text of this view.
    *
    * @param lpszText   [I/ ] The buffer to contains text.
    * @param uSize      [I/ ] The size of the buffer pointed by lpszText.
    */
    void GetText(IN LPWSTR lpszText, UINT32 uSize);

    /*!
    * @brief Get the length of the text string.
    *
    * @return The value of text length.
    */
    UINT32 GetTextLength();

    /*!
    * @brief Get the pointer to IDWriteTextLayout.
    *
    * @param ppDWriteTextLayout     [ /O] The IDWriteTextLayout pointer.
    */
    void GetIDWriteTextLayout(OUT IDWriteTextLayout **ppDWriteTextLayout);

    /*!
    * @brief Set the start point for drawing text.
    *
    * @param startPoint [I/ ] The reference to D2D1_POINT_2F.
    */
    void SetTextStartPoint(IN const D2D1_POINT_2F& startPoint);

    /*!
    * @brief Call this method to draw text.
    * 
    * @param pD2DDevice [I/ ] Device object.
    */
    void DrawText(IN D2DDevice *pD2DDevice);

    /*!
    * @brief Draw the text on specified render target.
    *
    * @param pRenderTarget  [I/ ] The render target.
    */
    void DrawText(IN ID2D1RenderTarget *pRenderTarget);

    /*!
    * @brief Call this method to set font family name.
    * 
    * @param familyName     [I/ ] The familyName.
    */
    void SetFontFamilyName(IN LPCWSTR familyName);

    /*!
    * @brief Call this method to set font size.
    * 
    * @param size           [I/ ] Text size.
    */
    void SetTextFontSize(IN FLOAT size);

    /*!
    * @brief Call this method to set text font stretch.
    * 
    * @param fontStretch.   [I/ ] The font stretch status.
    *                             Such as DWRITE_FONT_STRETCH_CONDENSED,
    *                                     DWRITE_FONT_STRETCH_NORMAL,
    *                                     DWRITE_FONT_STRETCH_EXPANDED.
    */
    void SetFontStretch(IN DWRITE_FONT_STRETCH fontStretch);

    /*!
    * @brief Call this method to set font style.
    * 
    * @param fontStyle.     [I/ ] The font style.
    */
    void SetFontStyle(IN DWRITE_FONT_STYLE fontStyle);

    /*!
    * @brief Call this method to set font weight.
    * 
    * @param fontWeight.    [I/ ] The font weight.
    */
    void SetFontWeight(IN DWRITE_FONT_WEIGHT fontWeight);

    /*!
    * @brief Call this method to set font local name.
    * 
    * @param pLocalName     [I/ ] The local name.
    */
    void SetLocaleName(IN LPCWSTR pLocalName);

    /*!
    * @brief Call this method to set textLayout max height.
    * 
    * @param maxHeight      [I/ ] The textLayout max height.
    */
    void SetMaxHeight(IN FLOAT maxHeight);

    /*!
    * @brief Call this method to set textLayout max width.
    *
    * @param maxWidth       [I/ ] The textLayout max width.
    */
    void SetMaxWidth(IN FLOAT maxWidth);

    /*!
    * @brief Call this method to make text strike through.
    * 
    * @param hasStrikethrough   [I/ ] Has or not strike through text.
    */
    void SetStrikethrough(IN BOOL hasStrikethrough);

    /*!
    * @brief Call this method to set text has or not underline. 
    * 
    * @param hasUnderLine   [I/ ] The text has or not underline.
    */
    void SetTextUnderline(IN BOOL hasUnderLine);

    /*!
    * @brief Call this method to set text color.
    * 
    * @param color          [I/ ] the color of text display.
    */
    void SetTextColor(IN const D2D1_COLOR_F& color);

    /*!
    * @brief Call this method to set text align style.
    * 
    * @param textAlign      [I/ ] The text align style.
    */
    void SetTextAlignment(IN DWRITE_TEXT_ALIGNMENT textAlign);

    /*!
    * @brief Call this method to set text align style.
    * 
    * @param textParagraph  [I/ ] The text align style.
    */
    void SetTextParagraphAlign(DWRITE_PARAGRAPH_ALIGNMENT textParagraph);

    /*!
    * @brief Call this method to set text can or not wrap.
    * 
    * @param textWrap       [I/ ] The text wrap style.
    */
    void SetTextWrap(IN DWRITE_WORD_WRAPPING textWrap);

protected:

    /*!
    * @brief Create textLayout.
    */
    void CreateTextLayout();

    /*!
    * @brief Called when device state change, typically the render target is recreated.
    *
    * @param pDevice    [I/ ] The D2DDevice pointer.
    * @param stateVal   [I/ ] This value indicates which state change just occurred.
    */
    virtual void OnD2DDeviceStateChange(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal);

    /*!
    * @brief Called when device state change, typically the render target is resize.
    *
    * @param pDevice    [I/ ] The D2DDevice pointer.
    * @param stateVal   [I/ ] This value indicates which state change just occurred.
    *
    * @remark These classes which are dependent with render target must be override this method,
    *         because some render resource are created with specified render target, once the
    *         render target used to create render resource is invalid or recreated, these all render resource
    *         must be recreated or re-initialize. Render resource includes Brush, Bitmap, Layer, Mesh.
    */
    virtual void OnD2DDeviceResize(D2DDevice *pDevice, DEVICE_STATECHANGE_VALUE stateVal);

protected:

    BOOL                     m_isReCreateLayout;    // Indicates recreate text layout or not.
    BOOL                     m_hasInitialized;      // Indicates has initialized or not.
    TCHAR                   *m_pText;               // The text of display.
    LPWSTR                   m_textFontFamily;      // The text fontFamily while draw text.
    FLOAT                    m_fDefaultFontSize;    // The default textFormat font size.
    D2D1_POINT_2F            m_textStartPoint;      // The text start point.
    D2D1_COLOR_F             m_curTextColor;        // The text color;
    IDWriteFactory          *m_pDWriteFactory;      // The DwriteFactory.
    IDWriteTextLayout       *m_pDWriteTextLayout;   // The TextLayout.
    IDWriteTextFormat       *m_pDWriteTextFormat;   // The TextFormat.
    DWRITE_TEXT_RANGE        m_textRangle;          // The text region.
    D2DSolidColorBrush      *m_pD2DSolidBrush;      // The D2DSolidBrush object.
    IDWriteTextRenderer     *m_pTextRender;         // The custom text renderer.
    DWRITE_FONT_STYLE        m_fontStyle;           // The text font style while draw text.
    DWRITE_FONT_WEIGHT       m_fontWeight;          // The text font weight.Like thin,normal,bold.
    DWRITE_FONT_STRETCH      m_fontStretch;         // The text font stretch status.
                                                    // Like condensed,normal,medium,expanded.
};

#endif // _D2DTEXTLAYOUT_H_
#endif // __cplusplus