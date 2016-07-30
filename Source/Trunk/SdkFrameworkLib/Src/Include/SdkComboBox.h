/*!
* @file SdkComboBox.h
* 
* @brief This file defines the SdkComboBox class.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2011/06/27
*/

#ifdef __cplusplus
#ifndef _SDKCOMBOBOX_H_
#define _SDKCOMBOBOX_H_

#include "SdkViewLayout.h"
#include "D2DTextLayout.h"
#include "IListBoxEventHandler.h"
#include "IComboBoxEventHandler.h"
#include "IViewOnClickHandler.h"

BEGIN_NAMESPACE_VIEWS

/*!
* @brief A combo box consists of a list and a selection field.
*/
class CLASS_DECLSPEC SdkComboBox : public SdkViewLayout, 
                                   public IViewOnClickHandler,
                                   public IListBoxEventHandler
{
public:

    /*!
    * @brief The constructor function.
    */
    SdkComboBox();

    /*!
    * @brief The destructor function.
    */
    virtual ~SdkComboBox();

    /*!
    * @brief Add an item to the menu.
    *
    * @param sItemText      [I/ ] The child text.
    */
    virtual void AddItem(IN LPCWSTR lpItemText);

    /*!
    * @brief Add an item to the menu.
    * 
    * @param uResId     [I/ ] The string resource identifier.
    * @param hInst      [I/ ] The handle to instance from which load string.
    */
    virtual void AddItem(UINT32 uResId, HMODULE hInst = NULL);

    /*!
    * @brief Add an item to the menu which has special index.
    * 
    * @param lpItemText      [I/ ] The child text.
    * @param uIndex          [I/ ] The index.
    */
    virtual void AddItem(IN LPCWSTR lpItemText, UINT32 uIndex);

    /*!
    * @brief Call this method delete special index item.
    * 
    * @param uIndex.        [I/ ] The item index.
    */
    virtual void DeleteItem(UINT32 uIndex);

    /*!
    * @brief Remove all menu view
    *
    * @return True: remove success.
    *               remove failure
    */
    virtual void RemoveAllItems();

    /*!
    * @brief Set the drop down list width.
    *
    * @param fDropDownWidth [I/ ] The drop down list width.
    */
    virtual void SetDropDownWidth(FLOAT fDropDownWidth);

    /*!
    * @brief Set the menu item text by index.
    *
    * @param menuItemIndex         [I/ ] The menu item index.
    * @param lpText                [I/ ] The text.
    */
    virtual void SetItemText(IN INT32 menuItemIndex, IN LPWSTR lpText);

    /*!
    * @brief Set the menu item text by index.
    *
    * @param menuItemIndex         [I/ ] The menu item index.
    * @param uResId     [I/ ] The string resource identifier.
    * @param hInst      [I/ ] The handle to instance from which load string.
    */
    virtual void SetItemText(IN INT32 menuItemIndex, IN UINT32 uResId, HMODULE hInst = NULL);

    /*!
    * @brief Call this method set index item to be selected.
    */
    virtual void SetCurSel(INT32 nIndex);

    /*!
    * @brief Call this method get selected index.
    * 
    * @return selected index.
    */
    virtual INT32 GetCurSel();

    /*!
    * @brief Get the selected item's text.
    *
    * @param lpText                 [I/O] The text.
    * @param uSize                  [I/O] Buffer size.
    * 
    * @return True:success/False:failure.
    */
    virtual BOOL GetSelText(OUT LPWSTR lpText, IN UINT32 uSize);

    /*!
    * @brief Get selected item text length.
    *
    * @return The text length of selected item.
    */
    virtual UINT32 GetSelItemLength();

    /*!
    * @brief Set the parent of the view, generally, it is called when added to a layout.
    *
    * @param pParentView    [I/ ] The parent view.
    */
    virtual void SetParent(SdkViewLayout *pParentView);

    /*!
    * @brief Show or hide the list box of a combo box.
    *
    * @param isShow     [I/ ] TRUE if show, FALSE if hide.
    */
    virtual void ShowDropDown(BOOL isShow);

    /*!
    * @brief Set view enable or disable.
    *
    * @param isEnable       [I/ ] TRUE if enable, otherwise FALSE.
    */
    virtual void SetEnable(BOOL isEnable);

    /*!
    * @brief Set the comboBox event handler.
    * 
    * @param pComboBoxEventHadler      [I/ ] The handler to set.
    */
    virtual void SetComboBoxEventHandler(IN IComboBoxEventHandler *pComboBoxEventHandler);

protected:

    /*!
    * @brief Called to set text to comboBox.
    * 
    * @param lpText     [I/ ] The text.
    */
    virtual void SetComboBoxText(IN LPCWSTR lpText);

    /*!
    * @brief InitializeViews comboBox. It must be called.
    */
    virtual void InitializeViews();

    /*!
    * @brief Get the D2DBitmap instance corresponding the specified style.
    *
    * @return The pointer to D2DBitmap, may be NULL.
    */
    virtual D2DBitmap* GetStyleBitmap(VIEW_STYLE style) const;

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

    /*!
    * @brief Called when view item is clicked.
    * 
    * @param view   [I/ ] The clicked SdkViewElement.
    */
    virtual void OnClick(SdkViewElement* view);

    /*!
    * @brief Called when a view has been clicked and held.
    *
    * @param pView      [I/ ] The event source.
    *
    * @return TRUE if consumed the long click, FALSE otherwise.
    */
    virtual BOOL OnLongClick(SdkViewElement *pView);

    /*!
    * @brief Called when selected item is changed.
    * 
    * @param pView  [I/ ] The event source.
    */
    virtual void OnSelectedChanged(SdkListBox *pView);

    /*!
    * @brief Called when visibility is changed.
    *
    * @param pView  [I/ ] The event source.
    */
    virtual void OnVisibleChanged(SdkListBox *pView);

private:

    /*!
    * @brief The combo box internal data.
    */
    struct _COMBOBOX_INTERNALDATA;

    _COMBOBOX_INTERNALDATA  *m_pComboBoxData;       // The combo-box data.
};

END_NAMESPACE_VIEWS

#endif // _SDKCOMBOBOX_H_
#endif // __cplusplus

