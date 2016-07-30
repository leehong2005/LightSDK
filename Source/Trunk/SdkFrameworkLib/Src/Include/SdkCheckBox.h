/*!
* @file SdkCheckBox.h
* 
* @brief This file defines the SdkCheckBox class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/26
*/

#ifdef __cplusplus
#ifndef _SDKCHECKBOX_H_
#define _SDKCHECKBOX_H_

#include "SdkTextView.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief The check box view.check box consists of a square box and n application-defined label,
*        icon, or bitmap that indicates a choice the user can make by selecting the button.
*/
class CLASS_DECLSPEC SdkCheckBox : public SdkTextView
{
public:

    /*!
    * @brief The default constructor function.
    */
    SdkCheckBox();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkCheckBox();

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
    * @brief Set the interface to get notification from the check box.
    *
    * @param pEventHandler      [I/ ] The pointer to ICheckBoxEventHandler.
    */
    virtual void SetCheckBoxEventHandler(ICheckBoxEventHandler *pEventHandler);

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
    * @brief The internal data of check box.
    */
    struct _CHECKBOX_INTERNALDATA;

    _CHECKBOX_INTERNALDATA  *m_pCheckBoxData;       // The internal data of check box.
};

END_NAMESPACE_VIEWS

#endif // _SDKCHECKBOX_H_
#endif // __cplusplus
