/*!
* @file SdkEditBox.h
* 
* @brief This file defines the SdkEditBox class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#ifdef __cplusplus
#ifndef _SDKEDITBOX_H_
#define _SDKEDITBOX_H_

#include "SdkTextView.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The edit box can input text.
*/
class CLASS_DECLSPEC SdkEditBox : public SdkTextView
{
public:

    /*!
    * @brief The constructor method.
    */
    SdkEditBox();

    /*!
    * @brief The destructor method.
    */
    virtual ~SdkEditBox();

protected:

    /*!
    * @brief Get the D2DBitmap instance corresponding the specified style.
    *
    * @return The pointer to D2DBitmap, may be NULL.
    */
    virtual D2DBitmap* GetStyleBitmap(VIEW_STYLE style);

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

    /*!
    * @brief Called when the view's focus has changed.
    *
    * @param pView          [I/ ] The pointer to the view that lost or gets the focus, may be NULL.
    * @param fGetFocus      [I/ ] Indicates if the view has gained or lost the focus.
    *                             TRUE if view has gained focus, FALSE otherwise.
    */
    virtual void OnFocusChanged(SdkViewElement *pView, BOOL fGetFocus);

private:

    D2DBitmap           *m_pNormalBitmap;           // The normal bitmap.
    D2DBitmap           *m_pHoverBitmap;            // The hover bitmap.
    D2DBitmap           *m_pDisableBitmap;          // The disable bitmap.
};

END_NAMESPACE_VIEWS

#endif // _SDKEDITBOX_H_
#endif // __cplusplus
