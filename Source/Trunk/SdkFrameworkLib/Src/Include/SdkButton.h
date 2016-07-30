/*!
* @file SdkButton.h
* 
* @brief This file defines the SdkButton class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#ifdef __cplusplus
#ifndef _SDKBUTTON_H_
#define _SDKBUTTON_H_

#include "SdkTextView.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The SdkButton class.
*/
class CLASS_DECLSPEC SdkButton : public SdkTextView
{
public:

    /*!
    * @brief The constructor function.
    *
    * @param isShowDefBk    [I/ ] Indicates show default button background image or not.
    */
    SdkButton(BOOL isShowDefBk = TRUE);

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkButton();

    /*!
    * @brief Set the button is toggle style or cancel toggle style.
    *
    * @param isToggle   [I/ ] Indicates the button is toggle or not.
    */
    void SetToggle(BOOL isToggle);

    /*!
    * @brief Show or hide UAC shield icon.
    *
    * @param fShow      [I/ ] TRUE if show, FALSE if hide.
    */
    void ShowUACIcon(BOOL fShow);

    /*!
    * @brief Indicate the button is toggle or not.
    *
    * @return TRUE if button is toggle, otherwise FALSE.
    */
    BOOL IsToggle() const;

    /*!
    * @brief Set the focused image.
    *
    * @param lpFile     [I/ ] The path of the image.
    * @param style      [I/ ] The style of this image.
    * @param uWidth     [I/ ] The width destination image.
    * @param uHeight    [I/ ] The height destination image.
    *
    * @return TRUE if success, otherwise return FALSE.
    */
    BOOL SetImage(IN LPCWSTR lpFile, VIEW_STYLE style, UINT32 uWidth = 0, UINT32 uHeight = 0);

    /*!
    * @brief Set the focused image.
    *
    * @param hBitmap    [I/ ] Handle to HBITMAP.
    * @param style      [I/ ] The style of this image.
    * @param uWidth     [I/ ] The width destination image.
    * @param uHeight    [I/ ] The height destination image.

    * @return TRUE if success, otherwise return FALSE.
    */
    BOOL SetImage(HBITMAP hBitmap, VIEW_STYLE style, UINT32 uWidth = 0, UINT32 uHeight = 0);

    /*!
    * @brief Set the focused image.
    *
    * @param uResId     [I/ ] Resource id.
    * @param style      [I/ ] The style of this image.
    * @param hModule    [I/ ] Handle to HMODULE, default value is NULL.
    * @param uWidth     [I/ ] The width destination image.
    * @param uHeight    [I/ ] The height destination image.
    *
    * @return TRUE if success, otherwise return FALSE.
    */
    BOOL SetImage(UINT uResId, VIEW_STYLE style, HMODULE hModule = NULL, UINT32 uWidth = 0, UINT32 uHeight = 0);

    /*!
    * @brief Set the overlay size and position.
    *
    * @param x          [I/ ] The x coordinate of left.
    * @param y          [I/ ] The y coordinate of left.
    * @param width      [I/ ] The width coordinate of left.
    * @param height     [I/ ] The height coordinate of left.
    */
    void SetOverlayInfo(FLOAT x, FLOAT y, FLOAT width, FLOAT height);

protected:

    /*!
    * @brief Calculate the UAC icon's rectangle.
    */
    void CalcUacIconRect();

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
    * @brief Get the D2DBitmap instance corresponding the specified style.
    *
    * @return The pointer to D2DBitmap, may be NULL.
    */
    virtual D2DBitmap* GetStyleBitmap(VIEW_STYLE style);

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
    * @brief The internal data of button.
    */
    struct _BUTTON_INTERNALDATA;

    _BUTTON_INTERNALDATA    *m_pButtonData;         // The button data.
};

END_NAMESPACE_VIEWS

#endif // _SDKBUTTON_H_
#endif // __cplusplus

