/*!
* @file SdkRadioButton.h
* 
* @brief This file defines the SdkRadioButton class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#ifdef __cplusplus
#ifndef _SDKRADIOBUTTON_H_
#define _SDKRADIOBUTTON_H_

#include "SdkTextView.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief A radio button (also called option button) consists of a round button
*        and an application-defined label, icon, or bitmap that indicates a 
*        choice the user can make by selecting the button.
*/
class CLASS_DECLSPEC SdkRadioButton : public SdkTextView
{
public:

    /*!
    * @brief The default constructor function.
    */
    SdkRadioButton();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkRadioButton();

    /*!
    * @brief Set the radio button checked or unchecked.
    *
    * @param isChecked  [I/ ] TRUE if checked, FALSE if unchecked.
    */
    virtual void SetChecked(BOOL isChecked);

    /*!
    * @brief Indicate the radio button is checked or unchecked.
    *
    * @return TRUE if checked, FALSE if unchecked.
    */
    virtual BOOL IsChecked();

    /*!
    * @brief Set view enable or disable.
    *
    * @param isEnable       [I/ ] TRUE if enable, otherwise FALSE.
    */
    virtual void SetEnable(BOOL isEnable);

    /*!
    * @brief Set the event handler to this radio button.
    *
    * @param pEventHandler  [I/ ] The pointer to IRadioButtonEventHandler.
    */
    void SetRadioButtonEventHandler(IRadioButtonEventHandler *pEventHandler);

private:

    /*!
    * @brief Get the D2DBitmap instance corresponding the specified style.
    *
    * @return The pointer to D2DBitmap, may be NULL.
    */
    virtual D2DBitmap* GetStyleBitmap(VIEW_STYLE style) const;

    /*!
    * @brief Called when the view's selection has changed.
    *
    * @param pView          [I/ ] The pointer to the view that lost or gets the focus, may be NULL.
    * @param fSelected      [I/ ] Indicated this view is selected or not.
    */
    virtual void OnSelectChenaged(SdkViewElement *pView, BOOL fSelected);

    /*!
    * @brief Call this method to draw item. In this method, you need not concern the
    *        transform of view.
    *
    * @param pRenderTarget  [I/ ] The render target, should not release the pointer.
    */
    virtual void OnDrawItem(ID2D1RenderTarget *pRenderTarget);

private:

    /*!
    * @brief The internal data of radio button.
    */
    struct _RADIOBUTTON_INTERNALDATA;

    _RADIOBUTTON_INTERNALDATA   *m_pRadioData;    // The data of radio button.
};

END_NAMESPACE_VIEWS

#endif // _SDKRADIOBUTTON_H_
#endif // __cplusplus
